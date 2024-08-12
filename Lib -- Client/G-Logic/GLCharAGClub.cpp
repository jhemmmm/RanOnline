#include "stdafx.h"
#include "./GLCharAG.h"
#include "./GLAgentServer.h"
#include "./GLItemLMT.h"
#include "./DbActionLogic.h"
#include "../../Lib -- ClientUI/Interface/GameTextControl.h"
#include "RanFilter.h"
#include "GLClubDeathMatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLCharAG::MsgClubMemberReqAgt ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_MEMBER_REQ_2AGT *pNetMsg = (GLMSG::SNET_CLUB_MEMBER_REQ_2AGT *) nmg;

	GLMSG::SNET_CLUB_MEMBER_REQ_FB NetMsgFB;

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetChar ( pNetMsg->dwReqGID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	StringCchCopy ( NetMsgFB.szReqName, CHAR_SZNAME, pCHAR->m_szName );

	//	Note : 탈퇴후 유보시간 점검.
	if ( pCHAR->m_tSECEDE!=0 )
	{
		CTime cSECEDE(pCHAR->m_tSECEDE);
		CTimeSpan sSpan(GLCONST_CHAR::dwCLUB_JOINBLOCK_DAY,0,10,0);

		cSECEDE += sSpan;

		CTime cCurTime = CTime::GetCurrentTime();
		if ( cCurTime < cSECEDE )
		{
			NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_SECEDED;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}
	}

	GLClubMan &cClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = cClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_CLUBBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMemberFlgJoin(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	bool bFOUND = pCLUB->IsMember ( pCHAR->m_dwCharID );
	if ( bFOUND )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_ALREADY;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	const GLCLUBRANK &sRANK_DATA = GLCONST_CHAR::sCLUBRANK[pCLUB->m_dwRank];
	if ( pCLUB->GetNemberNum() >= sRANK_DATA.m_dwMember )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_MAXNUM;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}


	//	Note : 클럽 가입 로그 기록.
	GLITEMLMT::GetInstance().ReqAction
	(
		pCLUB->m_dwMasterID,		//	당사자.
		EMLOGACT_CLUB_JOIN_MEMBER,	//	행위.
		ID_CHAR, pCHAR->m_dwCharID,	//	상대방.
		0,							//	exp
		0,							//	bright
		0,							//	life
		0							//	money
	);

	//	Note : 클럽에 가입시킴.
	//
	pCLUB->AddMember ( pCHAR->m_dwCharID, pCHAR->m_szName, NULL );
	pCHAR->m_dwGuild = pCLUB->m_dwID;

	//	Note : DB에 클럽 멤버 추가.
	CAddClubMember *pDbAction = new CAddClubMember(pCLUB->m_dwID,pCHAR->m_dwCharID);
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );

	//	Note : 가입 성공 FB.
	NetMsgFB.emFB = EMCLUB_MEMBER_REQ_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	Note : Field에 정보 전송.
	GLMSG::SNET_CLUB_MEMBER_ADD_2FLD NetMsgAdd;
	NetMsgAdd.dwClubID = pCLUB->m_dwID;
	NetMsgAdd.dwMember = pCHAR->m_dwCharID;
	StringCchCopy ( NetMsgAdd.szMember, CHAR_SZNAME, pCHAR->m_szName );
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgAdd );

	return S_OK;
}

HRESULT GLCharAG::MsgClubMemberDel ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_MEMBER_DEL *pNetMsg = (GLMSG::SNET_CLUB_MEMBER_DEL *) nmg;
	GLMSG::SNET_CLUB_MEMBER_DEL_FB NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwMasterID==pNetMsg->dwMember )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->IsSubMaster(pNetMsg->dwMember) && !pCLUB->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMemberFlgKick(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	bool bFOUND = pCLUB->IsMember ( pNetMsg->dwMember );
	if ( !bFOUND )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// [버그트레킹:클럽 탈퇴와 재가입에 대한 사소한 문제] 참고해서 주석처리함.

	//	탈퇴대상자가 온라인이면 탈퇴시간 설정해줌.
	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwMember );
	if ( pChar )
	{
		CTime cCurTime = CTime::GetCurrentTime();
		pChar->m_tSECEDE = cCurTime.GetTime();
	}
	
	//	Note : 클럽 맴버 제명 로그 기록.
	GLITEMLMT::GetInstance().ReqAction
	(
		pCLUB->m_dwMasterID,		//	당사자.
		EMLOGACT_CLUB_KICK_MEMBER,	//	행위.
		ID_CHAR, pNetMsg->dwMember,	//	상대방.
		0,							//	exp
		0,							//	bright
		0,							//	life
		0							//	money
	);

	//	Note : DB에서 삭제.
	CDeleteClubMember *pDbAction = new CDeleteClubMember(pNetMsg->dwMember);
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );

	pCLUB->SetMemberCDMFlag( pNetMsg->dwMember, FALSE );

	//	Note : 클럽 목록에서 제거.
	pCLUB->DelMember ( pNetMsg->dwMember );

	GLMSG::SNET_CLUB_MEMBER_DEL_2FLD NetMsgFld;
	NetMsgFld.dwClub = m_dwGuild;
	NetMsgFld.dwMember = pNetMsg->dwMember;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	Note : 삭제 성공 알림.
	//
	NetMsgFB.emFB = EMCLUB_MEMBER_DEL_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLCharAG::MsgClubMemberSecede ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_MEMBER_SECEDE *pNetMsg = (GLMSG::SNET_CLUB_MEMBER_SECEDE *) nmg;
	GLMSG::SNET_CLUB_MEMBER_SECEDE_FB NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_SECEDE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_SECEDE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwMasterID==m_dwCharID )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_SECEDE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	bool bFOUND = pCLUB->IsMember ( m_dwCharID );
	if ( !bFOUND )
	{
		NetMsgFB.emFB = EMCLUB_MEMBER_SECEDE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	Note : 탈퇴 시간 저장.
	CTime cCurTime = CTime::GetCurrentTime();
	m_tSECEDE = cCurTime.GetTime();

	m_dwGuild = CLUB_NULL;
	
	//	Note : 클럽 멤버 탈퇴 로그 기록.
	GLITEMLMT::GetInstance().ReqAction
	(
		pCLUB->m_dwMasterID,		//	당사자.
		EMLOGACT_CLUB_SECEDE_MEMBER,//	행위.
		ID_CHAR, m_dwCharID,		//	상대방.
		0,							//	exp
		0,							//	bright
		0,							//	life
		0							//	money
	);

	//	Note : DB에서 삭제.
	CDeleteClubMember *pDbAction = new CDeleteClubMember(m_dwCharID);
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );


	pCLUB->SetMemberCDMFlag( m_dwCharID, FALSE );

	//	Note : 클럽 목록에서 제거.
	pCLUB->DelMember ( m_dwCharID );

	GLMSG::SNET_CLUB_MEMBER_DEL_2FLD NetMsgFld;
	NetMsgFld.dwClub   = pCLUB->m_dwID;
	NetMsgFld.dwMember = m_dwCharID;
	NetMsgFld.tSECEDE  = m_tSECEDE;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	Note : 삭제 성공 알림.
	//
	NetMsgFB.emFB = EMCLUB_MEMBER_SECEDE_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLCharAG::MsgClubAuthorityReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_AUTHORITY_REQ *pNetMsg = (GLMSG::SNET_CLUB_AUTHORITY_REQ *) nmg;
	GLMSG::SNET_CLUB_AUTHORITY_REQ_FB NetMsgFB;
	NetMsgFB.dwCharID = pNetMsg->dwMember;

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwMasterID==pNetMsg->dwMember )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_NOMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	bool bFOUND = pCLUB->IsMember ( pNetMsg->dwMember );
	if ( !bFOUND )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	상대방이 없음
	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwMember );
	if ( !pChar )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_NOONLINE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	동맹 여부
	if ( pCLUB->IsAlliance() )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_ALLIANCE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	클럽배틀 여부
	if ( pCLUB->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_CLUBBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

    //	대련 여부
	if ( m_sCONFTING.emTYPE != EMCONFT_NONE || pChar->m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_CONFING;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	시간 여부
	if ( pCLUB->IsAuthority() )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_TIME;
		NetMsgFB.tAuthority = pCLUB->m_tAuthority;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	선도전 여부
	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_GUIDNBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if( GLClubDeathMatchAgentMan::GetInstance().IsBattle() ){
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_GUIDNBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	pCLUB->m_dwReqClubID = pCLUB->m_dwID;

	//	Note : 권한 위임 요청.
	GLMSG::SNET_CLUB_AUTHORITY_REQ_ASK NetMsgAsk;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgAsk );		

	return S_OK;
}

HRESULT GLCharAG::MsgClubAuthorityReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_AUTHORITY_REQ_ANS *pNetMsg = (GLMSG::SNET_CLUB_AUTHORITY_REQ_ANS *) nmg;
	GLMSG::SNET_CLUB_AUTHORITY_REQ_FB	NetMsgFB;
	NetMsgFB.dwCharID = m_dwCharID;

	if ( m_dwGuild==CLUB_NULL )	return S_FALSE;

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )	return S_FALSE;
	if ( !pCLUB->IsMember( m_dwCharID ))	return S_FALSE;
	if ( m_dwGuild!=pCLUB->m_dwReqClubID )	return S_FALSE;
	if ( pCLUB->IsMaster(m_dwCharID) )		return S_FALSE;

	// 마스터가 없음
	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID ( pCLUB->m_dwMasterID );
	if ( !pChar )	return S_FALSE;

	if ( !pNetMsg->bOK ) 
	{
		NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return S_OK;
	}

	//	동맹 여부
	if ( pCLUB->IsAlliance() )	return S_FALSE;
	
	//	클럽배틀 여부
	if ( pCLUB->GetAllBattleNum() > 0 )		return S_FALSE;

    //	대련 여부
	if ( m_sCONFTING.emTYPE != EMCONFT_NONE || pChar->m_sCONFTING.emTYPE != EMCONFT_NONE )	return S_FALSE;

	//	시간 여부
	if ( pCLUB->IsAuthority() )			return S_FALSE;

	//	선도전 여부
	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )	return S_FALSE;

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() ) return S_FALSE;

	// 클럽 마스터 변경 
	if ( ! pCLUB->SetAuthorityMaster( m_dwCharID ) )	return S_FALSE;

	//	Note : 클럽마스터 위임 로그 기록
	GLITEMLMT::GetInstance().ReqAction
	(
		pChar->m_dwCharID,		//	당사자.
		EMLOGACT_CLUB_AUTHORITY,	//	행위.
		ID_CHAR, m_dwCharID,	//	상대방.
		0,							//	exp
		0,							//	bright
		0,							//	life
		0							//	money
	);

	//	Note : DB에서 마스터변경
	CSetClubAuthority	*pDbAction1 = new CSetClubAuthority( pCLUB->m_dwID, m_dwCharID );
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction1 );

	//	권한 위임 시간 설정
	pCLUB->SetAuthority();

	CSetClubAuthorityTime *pDbAction2 = new CSetClubAuthorityTime( pCLUB->m_dwID, pCLUB->m_tAuthority );
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction2 );

	// 기존 클마에게 전송
	NetMsgFB.emFB = EMCLUB_AUTHORITY_REQ_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );			

	// 필드서버에 정보 전송
	GLMSG::SNET_CLUB_AUTHORITY_FLD	NetMsgFld;
	NetMsgFld.dwClubID = m_dwGuild;
	NetMsgFld.dwCharID = m_dwCharID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	Note : 자기 클럽원에게 알림.
	GLMSG::SNET_CLUB_SUBMASTER_BRD NetMsgBrdOld;
	NetMsgBrdOld.dwCharID = pChar->m_dwCharID;
	NetMsgBrdOld.dwFlags = 0;
	NetMsgBrdOld.dwCDCertifior = pCLUB->m_dwCDCertifior;
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB->m_dwID, &NetMsgBrdOld );

	//	Note : 필드에 알림.
	GLMSG::SNET_CLUB_SUBMASTER_FLD NetMsgFldOld;
	NetMsgFldOld.dwClubID = pCLUB->m_dwID;
	NetMsgFldOld.dwCharID = pChar->m_dwCharID;
	NetMsgFldOld.dwFlags = 0;
	NetMsgFldOld.dwCDCertifior = pCLUB->m_dwCDCertifior;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFldOld );

	return S_OK;
}

HRESULT GLCharAG::MsgClubMarkInfo ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_MARK_INFO *pNetMsg = (GLMSG::SNET_CLUB_MARK_INFO *) nmg;
	GLMSG::SNET_CLUB_MARK_INFO_FB NetMsgFB;

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_MARK_INFO_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	NetMsgFB.dwClubID = pNetMsg->dwClubID;
	NetMsgFB.emFB = EMCLUB_MARK_INFO_FB_OK;
	NetMsgFB.dwMarkVER = pCLUB->m_dwMarkVER;
	memcpy ( NetMsgFB.aryMark, pCLUB->m_aryMark, sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY );
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLCharAG::MsgClubMarkChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_MARK_CHANGE *pNetMsg = (GLMSG::SNET_CLUB_MARK_CHANGE *) nmg;
	GLMSG::SNET_CLUB_MARK_CHANGE_FB NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_MARK_CHANGE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_MARK_CHANGE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMemberFlgMarkChange ( m_dwCharID ) )
	{
		NetMsgFB.emFB = EMCLUB_MARK_CHANGE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	Note : 이미지 내용 변경.
	++pCLUB->m_dwMarkVER;
	memcpy ( pCLUB->m_aryMark, pNetMsg->aryMark, sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY );

	//	Note : DB에 저장.
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		CWriteClubMarkImage *pDbAction = new CWriteClubMarkImage(pCLUB->m_dwID,pCLUB->m_dwMarkVER,(BYTE*) pCLUB->m_aryMark,sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY);
		pDbMan->AddJob ( pDbAction );
	}

	//	Note : 필드 서버로 알림?
	GLMSG::SNET_CLUB_MARK_CHANGE_2FLD NetMsgFld;
	NetMsgFld.dwClub = m_dwGuild;
	NetMsgFld.dwMarkVER = pCLUB->m_dwMarkVER;
	memcpy ( NetMsgFld.aryMark, pCLUB->m_aryMark, sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY );
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	Note : 클라이언트들에게 알림.
	GLMSG::SNET_CLUB_MARK_CHANGE_2CLT NetMsgClt;
	NetMsgClt.dwMarkVER = pCLUB->m_dwMarkVER;
	memcpy ( NetMsgClt.aryMark, pCLUB->m_aryMark, sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY );

	PGLCHARAG pMASTER = GLAgentServer::GetInstance().GetCharID(pCLUB->m_dwMasterID);
	if ( pMASTER )
	{
		GLAgentServer::GetInstance().SENDTOCLIENT ( pMASTER->m_dwClientID, &NetMsgClt );
	}

	PGLCHARAG pMEMEBER = NULL;
	CLUBMEMBERS_ITER pos = pCLUB->m_mapMembers.begin();
	CLUBMEMBERS_ITER end = pCLUB->m_mapMembers.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBMEMBER &cMEMBER = (*pos).second;
		
		pMEMEBER = GLAgentServer::GetInstance().GetCharID(cMEMBER.dwID);
		if ( pMEMEBER )
		{
			GLAgentServer::GetInstance().SENDTOCLIENT ( pMEMEBER->m_dwClientID, &NetMsgClt );
		}
	}

	return S_OK;

}

HRESULT GLCharAG::MsgClubRank2Agt ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_RANK_FB NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_RANK_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_RANK_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwMasterID != m_dwCharID )
	{
		NetMsgFB.emFB = EMCLUB_RANK_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwRank >= (GLCONST_CHAR::dwMAX_CLUBRANK-1) )
	{
		NetMsgFB.emFB = EMCLUB_RANK_FB_MAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	++pCLUB->m_dwRank;

	//	Note : db에 저장.
	CSetClubRank *pDbAction = new CSetClubRank(pCLUB->m_dwID,pCLUB->m_dwRank);
	GLDBMan *pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );

	//	Note : 필드 서버에 알림.
	GLMSG::SNET_CLUB_RANK_2FLD NetMsgFld;
	NetMsgFld.dwClubID = pCLUB->m_dwID;
	NetMsgFld.dwRank = pCLUB->m_dwRank;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	Note : 처리 결과 알림.
	NetMsgFB.emFB = EMCLUB_RANK_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLCharAG::MsgClubGuidCommission ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_GUID_COMMISSION *pNetMsg = (GLMSG::SNET_CLUB_GUID_COMMISSION *) nmg;
	GLMSG::SNET_CLUB_GUID_COMMISSION_FB NetMsgFB;

	if ( m_dwGuild == CLUB_NULL )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_NOTCLUB;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_NOTCLUB;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB->m_dwMasterID!=m_dwCharID )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_NOTCLUB;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLGuidance *pGuid = GLGuidanceAgentMan::GetInstance().FindByClubID ( m_dwGuild );
	if ( !pGuid )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_NOTCLUB;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pGuid->m_bBattle )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_BATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pNetMsg->fCommission>GLCONST_CHAR::fMAX_COMMISSION )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_RANGE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pNetMsg->fCommission<0.0f )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_RANGE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pGuid->m_bNewCommission )
	{
		NetMsgFB.emFB = EMGUIDCOMMISSION_FB_ALREADY;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;	
	}

	pGuid->m_bNewCommission = true;
	pGuid->m_fCommissionTimer = 0.0f;
	pGuid->m_fNewCommissionPercent = pNetMsg->fCommission;

	//	Note : 수수료 변경 성공 알림.
	NetMsgFB.emFB = EMGUIDCOMMISSION_FB_OK;
	NetMsgFB.fCommission = pNetMsg->fCommission;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	Note : 클라이언트들에게 알림.
	GLMSG::SNET_CLUB_GUID_COMMISSION_RESERVE_BRD NetMsgBrd;
	NetMsgBrd.fCommission = pNetMsg->fCommission;

	std::vector<DWORD>::size_type nSize = pGuid->m_vecMaps.size();
	for ( std::vector<DWORD>::size_type i=0; i<nSize; ++i )
	{
		SNATIVEID nidMAP = (pGuid->m_vecMaps[i]);
		GLAGLandMan *pLand = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
		if ( !pLand )	continue;

		pLand->SENDTOALLCLIENT ( &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLCharAG::MsgClubNoticeReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_NOTICE_REQ *pNetMsg = (GLMSG::SNET_CLUB_NOTICE_REQ *) nmg;
	GLMSG::SNET_CLUB_NOTICE_FB NetMsgFB;

	if ( m_dwGuild == CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_NOTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUB_NOTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMemberFlgNotice(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_NOTICE_FB_NOTMATER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	Note : 클럽의 공지 내용 변경.
	CString strTemp( pNetMsg->szNotice );
	if ( CRanFilter::GetInstance().ChatFilter( strTemp ) )
		strTemp = CRanFilter::GetInstance().GetProverb();

	StringCchCopy ( pCLUB->m_szNotice, EMCLUB_NOTICE_LEN+1, strTemp );

	CSetClubNotice *pDbAction = new CSetClubNotice ( pCLUB->m_dwID, pCLUB->m_szNotice );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );

	//	Note : 변경 성공 알림.
	NetMsgFB.emFB = EMCLUB_NOTICE_FB_OK;
	StringCchCopy ( NetMsgFB.szNotice, EMCLUB_NOTICE_LEN+1, pCLUB->m_szNotice );
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	Note : 클럽의 공지를 필드 서버에 반영.
	GLMSG::SNET_CLUB_NOTICE_FLD NetMsgFld;
	NetMsgFld.dwCLUB_ID = m_dwGuild;
	StringCchCopy ( NetMsgFld.szNotice, EMCLUB_NOTICE_LEN+1, pCLUB->m_szNotice );
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	return S_OK;
}

HRESULT GLCharAG::MsgClubSubMaster ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_SUBMASTER *pNetMsg = (GLMSG::SNET_CLUB_SUBMASTER *) nmg;
	GLMSG::SNET_CLUB_SUBMASTER_FB NetMsgFB;

	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_BATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_CDM;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild == CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}


	//	CDM 인증권한 설정시
	if ( pNetMsg->dwFlags & EMCLUB_SUBMASTER_CDM )
	{
		if ( !pCLUB->EnableCDMFlag( pNetMsg->dwCharID ) )
		{
			NetMsgFB.emFB = EMCLUBSUBMASTER_FB_CDM_MEM;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}
	}

	//	Note : 클럽 부마 설정.
	DWORD dwCD_OLD = pCLUB->m_dwCDCertifior;
	bool bOK = pCLUB->SetMemberFlag ( pNetMsg->dwCharID, pNetMsg->dwFlags );
	if ( !bOK )
	{
		NetMsgFB.emFB = EMCLUBSUBMASTER_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	Note : 변경 성공 알림.
	NetMsgFB.dwCharID = pNetMsg->dwCharID;
	NetMsgFB.dwFlags = pNetMsg->dwFlags;
	if ( pCLUB->IsSubMaster(pNetMsg->dwCharID) )	NetMsgFB.emFB = EMCLUBSUBMASTER_FB_OK;
	else											NetMsgFB.emFB = EMCLUBSUBMASTER_FB_RESET;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	Note : DB에 저장.
	GLDBMan* pDBMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDBMan )
	{
		//	Note : Flag 저장.
		DWORD dwSetFlags = pCLUB->GetMemberFlag(pNetMsg->dwCharID);
		CSetClubMasterFlags *pDbAction = new CSetClubMasterFlags(m_dwGuild,pNetMsg->dwCharID,dwSetFlags);
		pDBMan->AddJob ( pDbAction );

		CDbAction *pNewDbAction = new CSetClubDeputy ( pCLUB->m_dwID, pCLUB->m_dwCDCertifior );
		pDBMan->AddJob ( pNewDbAction );

		//	Note : 클럽 인증 CD 권한 설정에 대한 로그
		GLITEMLMT::GetInstance().ReqAction
		(
			pCLUB->m_dwMasterID,		//	당사자.
			EMLOGACT_CLUB_CLUBDEPUTY_MEMBER,	//	행위.
			ID_CHAR, pCLUB->m_dwCDCertifior,	//	상대방.
			0,							//	exp
			0,							//	bright
			0,							//	life
			0							//	money
		);

		//	Note : 선도클럽 인증자 저장.
		if ( dwCD_OLD != 0 && dwCD_OLD != pCLUB->m_dwCDCertifior )
		{
			//	이전 선도인증권자의 Flag 를 갱신한다.
			DWORD dwOldSetFlags = pCLUB->GetMemberFlag( dwCD_OLD );
			CSetClubMasterFlags *pOldDbAction = new CSetClubMasterFlags( m_dwGuild, dwCD_OLD, dwOldSetFlags );
			pDBMan->AddJob ( pOldDbAction );

			//	Note : 자기 클럽원에게 알림.
			GLMSG::SNET_CLUB_SUBMASTER_BRD NetMsgBrdOld;
			NetMsgBrdOld.dwCharID = dwCD_OLD;
			NetMsgBrdOld.dwFlags = dwOldSetFlags;
			NetMsgBrdOld.dwCDCertifior = pCLUB->m_dwCDCertifior;
			GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( m_dwGuild, &NetMsgBrdOld );

			//	Note : 필드에 알림.
			GLMSG::SNET_CLUB_SUBMASTER_FLD NetMsgFldOld;
			NetMsgFldOld.dwClubID = m_dwGuild;
			NetMsgFldOld.dwCharID = dwCD_OLD;
			NetMsgFldOld.dwFlags = dwOldSetFlags;
			NetMsgFldOld.dwCDCertifior = pCLUB->m_dwCDCertifior;
			GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFldOld );
		}
	}

	//	Note : 자기 클럽원에게 알림.
	GLMSG::SNET_CLUB_SUBMASTER_BRD NetMsgBrd;
	NetMsgBrd.dwCharID = pNetMsg->dwCharID;
	NetMsgBrd.dwFlags = pNetMsg->dwFlags;
	NetMsgBrd.dwCDCertifior = pCLUB->m_dwCDCertifior;
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( m_dwGuild, &NetMsgBrd );

	//	Note : 필드에 알림.
	GLMSG::SNET_CLUB_SUBMASTER_FLD NetMsgFld;
	NetMsgFld.dwClubID = m_dwGuild;
	NetMsgFld.dwCharID = pNetMsg->dwCharID;
	NetMsgFld.dwFlags = pNetMsg->dwFlags;
	NetMsgFld.dwCDCertifior = pCLUB->m_dwCDCertifior;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	return S_OK;
}

// *****************************************************
// Desc: Agent에게 동맹작업(가입/결성)을 요청함.
// *****************************************************
HRESULT GLCharAG::MsgClubAllianceReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_ALLIANCE_REQ *pNetMsg = (GLMSG::SNET_CLUB_ALLIANCE_REQ *) nmg;
	GLMSG::SNET_CLUB_ALLIANCE_REQ_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 동맹가입 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsAlliance() && !pCLUB_CHIEF->IsChief() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_NOTCHIEF;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// 클럽 배틀이 진행중일때 동맹을 맺을수 없다.
	if ( pCLUB_CHIEF->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_CLUBBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
	}

	//	최초 결성일때.
	if ( !pCLUB_CHIEF->IsAlliance() )
	{
		//	결성 제한 시간 검사.
		if ( pCLUB_CHIEF->IsAllianceDis() )
		{
			NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_DISTIME;
			NetMsgFB.tBlock = pCLUB_CHIEF->m_tAllianceDis;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}
	}

	if ( pCLUB_CHIEF->IsRegDissolution() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_DISSOLUTION;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;

	}

	//	가입 제한시간. ( 제명시간 )
	if ( pCLUB_CHIEF->IsAllianceSec() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_SECTIME;
		NetMsgFB.tBlock = pCLUB_CHIEF->m_tAllianceSec;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	클럽 동맹 최대 숫자 검사.
	if ( pCLUB_CHIEF->IsAlliance() && pCLUB_CHIEF->m_setAlliance.size()>=GLCONST_CHAR::dwCLUB_ALLIANCE_NUM )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_MAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwCharID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !GLCONST_CHAR::bCLUB_2OTHERSCHOOL && pCHAR->m_wSchool!=m_wSchool )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_OTHERSCHOOL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	if ( pCLUB_INDIAN->IsAllianceSec() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_TOSECTIME;
		NetMsgFB.tBlock = pCLUB_INDIAN->m_tAllianceSec;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(pCHAR->m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_INDIAN->IsAlliance() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_ALREADY;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_INDIAN->IsRegDissolution() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_DISSOLUTION2;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// 클럽 배틀이 진행중일때 동맹을 맺을수 없다.
	if ( pCLUB_INDIAN->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_TARCLUBBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
	}



	DWORD dwCHIEFGuidID = GLGuidanceAgentMan::GetInstance().GetGuidID ( pCLUB_CHIEF->m_dwID );
	DWORD dwINDIANGuidID = GLGuidanceAgentMan::GetInstance().GetGuidID ( pCLUB_INDIAN->m_dwID );

	// 두 클럽이 모두 선도클럽이면 동맹 불가능
	if ( dwCHIEFGuidID != UINT_MAX && dwINDIANGuidID != UINT_MAX )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_BOTHGUID;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	//	가입 요청한 클럽 id 저장.
	pCLUB_CHIEF->m_dwReqClubID = pCLUB_INDIAN->m_dwID;

	//	Note : 동맹 요청.
	GLMSG::SNET_CLUB_ALLIANCE_REQ_ASK NetMsgAsk;
	NetMsgAsk.dwChiefCharID = m_dwCharID;
	StringCchCopy ( NetMsgAsk.szChiefName, CHAR_SZNAME, pCLUB_CHIEF->m_szName );
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgAsk );

	return S_OK;
}

// *****************************************************
// Desc: Agent에게 동맹작업(가입/결성) 결과 FeedBack.
// *****************************************************
HRESULT GLCharAG::MsgClubAllianceReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_ALLIANCE_REQ_ANS *pNetMsg = (GLMSG::SNET_CLUB_ALLIANCE_REQ_ANS *) nmg;
	GLMSG::SNET_CLUB_ALLIANCE_REQ_FB NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( pCLUB_INDIAN->IsAlliance() )
	{
		return S_FALSE;
	}
	
	if ( pCLUB_INDIAN->GetAllBattleNum() > 0 )
	{
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwChiefCharID );
	if ( !pCHAR )
	{
		return S_FALSE;
	}

	if ( !pNetMsg->bOK )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		return S_FALSE;
	}
	
	if ( !pCLUB_CHIEF->IsMaster(pCHAR->m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsAlliance() && !pCLUB_CHIEF->IsChief() )
	{
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->GetAllBattleNum() > 0 )
	{
		return S_FALSE;
	}

	//	요청한 클럽인지 검사.
	if ( pCLUB_CHIEF->m_dwReqClubID!=pCLUB_INDIAN->m_dwID )
	{
		return S_FALSE;
	}

	//	동맹 요청 결과.
	NetMsgFB.emFB = EMCLUB_ALLIANCE_REQ_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

	//	동맹 결성 및 동맹 클럽 추가.
	pCLUB_CHIEF->m_dwAlliance = pCLUB_CHIEF->m_dwID;
	pCLUB_CHIEF->AddAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_CHIEF->m_szName );
	pCLUB_CHIEF->AddAlliance ( pCLUB_INDIAN->m_dwID, pCLUB_INDIAN->m_szName );

	//	추가되는 클럽에 동맹 설정.
	pCLUB_INDIAN->m_dwAlliance = pCLUB_CHIEF->m_dwID;
	pCLUB_INDIAN->m_dwAllianceBattleWin = pCLUB_CHIEF->m_dwAllianceBattleWin;
	pCLUB_INDIAN->m_dwAllianceBattleLose = pCLUB_CHIEF->m_dwAllianceBattleLose;
	pCLUB_INDIAN->m_dwAllianceBattleDraw = pCLUB_CHIEF->m_dwAllianceBattleDraw;

	// 추가되는 동맹에 동맹 마스터 추가.
	pCLUB_INDIAN->AddAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_CHIEF->m_szName );

	//	필드에 알림.
	GLMSG::SNET_CLUB_ALLIANCE_ADD_FLD NetMsgFld;
	NetMsgFld.dwChiefClubID = pCLUB_CHIEF->m_dwID;
	NetMsgFld.dwIndianClubID = pCLUB_INDIAN->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	클라이언트들에 알림.
	GLMSG::SNET_CLUB_ALLIANCE_ADD_CLT NetMsgClt;
	NetMsgClt.dwChiefClubID = pCLUB_CHIEF->m_dwID;
	NetMsgClt.dwIndianClubID = pCLUB_INDIAN->m_dwID;
	
	NetMsgClt.dwAllianceBattleWin = pCLUB_CHIEF->m_dwAllianceBattleWin;
	NetMsgClt.dwAllianceBattleLose = pCLUB_CHIEF->m_dwAllianceBattleLose;
	NetMsgClt.dwAllianceBattleDraw = pCLUB_CHIEF->m_dwAllianceBattleDraw;
	
	StringCchCopy ( NetMsgClt.szChiefClub, CHAR_SZNAME, pCLUB_CHIEF->m_szName );
	StringCchCopy ( NetMsgClt.szIndianClub, CHAR_SZNAME, pCLUB_INDIAN->m_szName );
	
	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;

		//if ( sALLIANCE.m_dwID == pCLUB_CHIEF->m_dwID ) continue;

		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );

		CLUB_ALLIANCE_ITER posMem = pCLUB_CHIEF->m_setAlliance.begin();
		CLUB_ALLIANCE_ITER endMem = pCLUB_CHIEF->m_setAlliance.end();
		for ( ; posMem!=endMem; ++posMem )
		{
			const GLCLUBALLIANCE &sALLIANCEMEM = *posMem;

			if ( !pCLUB )								   continue;
			pCLUB->AddAlliance ( sALLIANCEMEM.m_dwID, sALLIANCEMEM.m_szName );

			NetMsgFld.dwChiefClubID = pCLUB_CHIEF->m_dwID;
			NetMsgFld.dwIndianClubID = sALLIANCE.m_dwID;
			GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

			NetMsgClt.dwChiefClubID = pCLUB_CHIEF->m_dwID;
			NetMsgClt.dwIndianClubID = sALLIANCE.m_dwID;
			StringCchCopy ( NetMsgClt.szChiefClub, CHAR_SZNAME, pCLUB_CHIEF->m_szName );
			StringCchCopy ( NetMsgClt.szIndianClub, CHAR_SZNAME, sALLIANCE.m_szName );

			GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( sALLIANCEMEM.m_dwID, &NetMsgClt );
		}
	}

	//	db에 저장.
	CSetClubAlliance *pDbAction = new CSetClubAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );

	return S_OK;
}

// *****************************************************
// Desc : 동맹에서 클럽 제명요청 처리.
// *****************************************************
HRESULT GLCharAG::MsgClubAllianceDelReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_ALLIANCE_DEL_REQ *pNetMsg = (GLMSG::SNET_CLUB_ALLIANCE_DEL_REQ *) nmg;
	GLMSG::SNET_CLUB_ALLIANCE_DEL_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 동맹탈퇴 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_NOTCHIEF;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsChief() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_NOTCHIEF;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pNetMsg->dwDelClubID==pCLUB_CHIEF->m_dwID )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	CLUB_ALLIANCE_ITER del = pCLUB_CHIEF->m_setAlliance.find ( GLCLUBALLIANCE(pNetMsg->dwDelClubID,"") );
	if ( del==pCLUB_CHIEF->m_setAlliance.end() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwDelClubID );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}


	if ( pCLUB_CHIEF->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_BATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	동맹에서 제외.
	pCLUB_CHIEF->m_setAlliance.erase ( del );

	//	제명시 제한시간 설정.
	pCLUB_CHIEF->SetAllianceSec();
	pCLUB_INDIAN->SetAllianceSec();

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	CDbAction *pDbActionSec1 = new CSetClubAllianceSec(pCLUB_CHIEF->m_dwID,pCLUB_CHIEF->m_tAllianceSec);
	if ( pDbMan )	pDbMan->AddJob ( pDbActionSec1 );

	CDbAction *pDbActionSec2 = new CSetClubAllianceSec(pCLUB_INDIAN->m_dwID,pCLUB_INDIAN->m_tAllianceSec);
	if ( pDbMan )	pDbMan->AddJob ( pDbActionSec2 );
    
	GLMSG::SNET_CLUB_ALLIANCE_DEL_CLT NetMsgClt;
	NetMsgClt.dwDelClubID = pCLUB_INDIAN->m_dwID;
	StringCchCopy ( NetMsgClt.szDelClub, CHAR_SZNAME, pCLUB_INDIAN->m_szName );

	//	삭제되는 클럽에 전송.
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );

	//	해당 클럽 초기화.
	pCLUB_INDIAN->ClearAlliance();

	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;

		//	기타 동맹 클럽에 전송.
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( sALLIANCE.m_dwID, &NetMsgClt );
	}

	//	동맹 클럽 삭제 필드에 알림.
	GLMSG::SNET_CLUB_ALLIANCE_DEL_FLD NetMsgFld;
	NetMsgFld.dwAlliance = pCLUB_CHIEF->m_dwAlliance;
	NetMsgFld.dwDelClubID = pNetMsg->dwDelClubID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	동맹 제명 결과 알림.
	NetMsgFB.emFB = EMCLUB_ALLIANCE_DEL_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	db에 저장.
	CDelClubAlliance *pDbAction = new CDelClubAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID );
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );
	

	// 만약 동맹에 클럽원이 없을때
	if ( pCLUB_CHIEF->m_setAlliance.size() < 2 )
	{
		//	동맹 해체
		GLMSG::SNET_CLUB_ALLIANCE_DIS_CLT NetMsgClt;
		StringCchCopy ( NetMsgClt.szChiefClub, CHAR_SZNAME, pCLUB_CHIEF->m_szName );

		//	해체시 제한시간 설정.
		pCLUB_CHIEF->SetAllianceDis();

		CDbAction *pDbActionDis = new CSetClubAllianceDis(pCLUB_CHIEF->m_dwID,pCLUB_CHIEF->m_tAllianceDis);
		GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if ( pDbMan )	pDbMan->AddJob ( pDbActionDis );

		//	클럽장의 연합 정보 삭제.
		pCLUB_CHIEF->ClearAlliance();

		CDbAction *pDbActionReset = new CReSetAllianceBattle( pCLUB_CHIEF->m_dwID );
		pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if ( pDbMan )	pDbMan->AddJob ( pDbActionReset );


		// 클라이언트에 전송.
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

		//	동맹 클럽 삭제 필드에 알림.
		GLMSG::SNET_CLUB_ALLIANCE_DIS_FLD NetMsgDisFld;
		NetMsgDisFld.dwChiefClubID = pCLUB_CHIEF->m_dwID;
		GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgDisFld );
	}

	return S_OK;
}

// *****************************************************
// Desc : 동맹 탈퇴요청 처리. (위에꺼랑 같은 동작을 함)
// *****************************************************
HRESULT GLCharAG::MsgClubAllianceSecReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_ALLIANCE_SEC_REQ *pNetMsg = (GLMSG::SNET_CLUB_ALLIANCE_SEC_REQ *) nmg;
	GLMSG::SNET_CLUB_ALLIANCE_SEC_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 동맹탈퇴 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsAlliance() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_ALLIANCE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
		
	if ( pCLUB_INDIAN->IsChief() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCLUB_INDIAN->m_dwAlliance );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	CLUB_ALLIANCE_ITER del = pCLUB_CHIEF->m_setAlliance.find ( GLCLUBALLIANCE(pCLUB_INDIAN->m_dwID,"") );
	if ( del==pCLUB_CHIEF->m_setAlliance.end() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_BATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	탈퇴시 제한시간 설정.
	pCLUB_INDIAN->SetAllianceSec();

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	CDbAction *pDbActionSec = new CSetClubAllianceSec(pCLUB_INDIAN->m_dwID,pCLUB_INDIAN->m_tAllianceSec);
	if ( pDbMan )	pDbMan->AddJob ( pDbActionSec );


	//	클라이언트에 알릴 msg.
	GLMSG::SNET_CLUB_ALLIANCE_DEL_CLT NetMsgClt;
	NetMsgClt.dwDelClubID = pCLUB_INDIAN->m_dwID;
	StringCchCopy ( NetMsgClt.szDelClub, CHAR_SZNAME, pCLUB_INDIAN->m_szName );
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );

	//	동맹 가입 정보 리셋.
	pCLUB_INDIAN->ClearAlliance();

	//	동맹들에게 동맹 탈퇴 알림.
	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;

		//	기타 동맹 클럽에 전송.
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( sALLIANCE.m_dwID, &NetMsgClt );
	}

	// 동맹에서 제거
	pCLUB_CHIEF->m_setAlliance.erase ( del );

	//	동맹 클럽 삭제 필드에 알림.
	GLMSG::SNET_CLUB_ALLIANCE_DEL_FLD NetMsgFld;
	NetMsgFld.dwAlliance = pCLUB_CHIEF->m_dwAlliance;
	NetMsgFld.dwDelClubID = pCLUB_INDIAN->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	동맹 탈퇴 결과 알림.
	NetMsgFB.emFB = EMCLUB_ALLIANCE_SEC_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	db에 저장.
	CDelClubAlliance *pDbAction = new CDelClubAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID );
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );

	// 만약 동맹에 클럽원이 없을때
	if ( pCLUB_CHIEF->m_setAlliance.size() < 2 )
	{
		//	동맹 해체
		GLMSG::SNET_CLUB_ALLIANCE_DIS_CLT NetMsgClt;
		StringCchCopy ( NetMsgClt.szChiefClub, CHAR_SZNAME, pCLUB_CHIEF->m_szName );

		//	해체시 제한시간 설정.
		pCLUB_CHIEF->SetAllianceDis();

		CDbAction *pDbActionDis = new CSetClubAllianceDis(pCLUB_CHIEF->m_dwID,pCLUB_CHIEF->m_tAllianceDis);
		GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if ( pDbMan )	pDbMan->AddJob ( pDbActionDis );

		//	클럽장의 연합 정보 삭제.
		pCLUB_CHIEF->ClearAlliance();

		CDbAction *pDbActionReset = new CReSetAllianceBattle( pCLUB_CHIEF->m_dwID );
		pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if ( pDbMan )	pDbMan->AddJob ( pDbActionReset );


		// 클라이언트에 전송.
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

		//	동맹 클럽 삭제 필드에 알림.
		GLMSG::SNET_CLUB_ALLIANCE_DIS_FLD NetMsgDisFld;
		NetMsgDisFld.dwChiefClubID = pCLUB_CHIEF->m_dwID;
		GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgDisFld );		

	}

	return S_OK;
}

// *****************************************************
// Desc : 동맹 해체
// *****************************************************
HRESULT GLCharAG::MsgClubAllianceDisReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_ALLIANCE_DIS_REQ *pNetMsg = (GLMSG::SNET_CLUB_ALLIANCE_DIS_REQ *) nmg;
	GLMSG::SNET_CLUB_ALLIANCE_DIS_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 동맹탈퇴 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_NOTCHIEF;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
		
	if ( !pCLUB_CHIEF->IsChief() )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_NOTCHIEF;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->GetAllBattleNum() > 0 )
	{
		NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_BATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_ALLIANCE_DIS_CLT NetMsgClt;
	StringCchCopy ( NetMsgClt.szChiefClub, CHAR_SZNAME, pCLUB_CHIEF->m_szName );


	//	동맹들에게 동맹 해체 알림.
	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;
		if ( sALLIANCE.m_dwID==pCLUB_CHIEF->m_dwID )	continue;	//	클럽장은 나중에 삭제.

		GLCLUB *pCLUB_IND = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
		if ( !pCLUB_IND )	continue;

		pCLUB_IND->ClearAlliance();

		//	기타 동맹 클럽의 클라이언트에 전송.
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( sALLIANCE.m_dwID, &NetMsgClt );

		//	db에 저장.
		CDelClubAlliance *pDbAction = new CDelClubAlliance ( pCLUB_CHIEF->m_dwID, pCLUB_IND->m_dwID );
		GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if ( pDbMan )	pDbMan->AddJob ( pDbAction );
	}

	//	해체시 제한시간 설정.
	pCLUB_CHIEF->SetAllianceDis();

	CDbAction *pDbAction = new CSetClubAllianceDis(pCLUB_CHIEF->m_dwID,pCLUB_CHIEF->m_tAllianceDis);
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )	pDbMan->AddJob ( pDbAction );

	//	클럽장의 연합 정보 삭제.
	pCLUB_CHIEF->ClearAlliance();

	CDbAction *pDbAction2 = new CReSetAllianceBattle( pCLUB_CHIEF->m_dwID );
	pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )	pDbMan->AddJob ( pDbAction2 );


	// 클라이언트에 전송.
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	//	동맹 클럽 삭제 필드에 알림.
	GLMSG::SNET_CLUB_ALLIANCE_DIS_FLD NetMsgFld;
	NetMsgFld.dwChiefClubID = pCLUB_CHIEF->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	동맹 해체 결과 알림.
	NetMsgFB.emFB = EMCLUB_ALLIANCE_DIS_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

// *****************************************************
// Desc: Agent에게 배틀을 요청함.
// *****************************************************
HRESULT GLCharAG::MsgClubBattleReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_BATTLE_REQ *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_REQ *) nmg;
	GLMSG::SNET_CLUB_BATTLE_REQ_FB NetMsgFB;	
	
	if ( !GLCONST_CHAR::bCLUB_BATTLE )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_CONFT;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_GUIDNBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pNetMsg->dwBattleTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TIMEMIN;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pNetMsg->dwBattleTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TIMEMAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}
	

	// 선도클럽존에서 배틀 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsRegDissolution() )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_DISSOLUTION;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// 최대 동시 진행 갯수 확인
	if ( pCLUB_CHIEF->GetAllBattleNum() >= GLCONST_CHAR::dwCLUB_BATTLE_MAX )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_MAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->m_dwRank+1 < GLCONST_CHAR::dwCLUB_BATTLE_RANK )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_RANK;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwCharID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCHAR->GETCONFRONTING().emTYPE != EMCONFT_NONE )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARCONFT;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsAllianceGuild( pCHAR->m_dwGuild ) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_ALLIANCE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	if ( pCLUB_CHIEF->IsBattle( pCHAR->m_dwGuild ) || pCLUB_CHIEF->IsBattleReady( pCHAR->m_dwGuild ))
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_ALREADY;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(pCHAR->m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_INDIAN->IsBattle( m_dwGuild ) || pCLUB_INDIAN->IsBattleReady( m_dwGuild ))
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_ALREADY;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsBattleAlliance( pCLUB_INDIAN->m_dwAlliance ) || pCLUB_CHIEF->IsBattleReady( pCLUB_INDIAN->m_dwAlliance ))
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_ALREADY2;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_INDIAN->IsRegDissolution() )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_DISSOLUTION2;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;

	}

	// 최대 동시 진행 갯수 확인
	if ( pCLUB_INDIAN->GetAllBattleNum() >= GLCONST_CHAR::dwCLUB_BATTLE_MAX )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARMAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	//	클럽 랭크 확인
	if ( pCLUB_INDIAN->m_dwRank+1 < GLCONST_CHAR::dwCLUB_BATTLE_RANK )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_TARRANK;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	배틀 요청한 클럽 id 저장.
	pCLUB_CHIEF->m_dwReqClubID = pCLUB_INDIAN->m_dwID;
	pCLUB_CHIEF->m_dwBattleTime = pNetMsg->dwBattleTime;

	//	Note : 동맹 요청.
	GLMSG::SNET_CLUB_BATTLE_REQ_ASK NetMsgAsk;
	NetMsgAsk.dwClubCharID = m_dwCharID;
	NetMsgAsk.dwBattleTime = pNetMsg->dwBattleTime;
	StringCchCopy ( NetMsgAsk.szClubName, CHAR_SZNAME, pCLUB_CHIEF->m_szName );
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgAsk );

	return S_OK;
}



// *****************************************************
// Desc: Agent에게 배틀을 요청함. ( 동맹 )
// *****************************************************
HRESULT	GLCharAG::MsgAllianceBattleReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLIANCE_BATTLE_REQ *pNetMsg = (GLMSG::SNET_ALLIANCE_BATTLE_REQ *) nmg;
	GLMSG::SNET_ALLIANCE_BATTLE_REQ_FB NetMsgFB;
	
	if ( !GLCONST_CHAR::bCLUB_BATTLE || !GLCONST_CHAR::bCLUB_BATTLE_ALLIANCE )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_CONFT;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_GUIDNBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pNetMsg->dwBattleTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TIMEMIN;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pNetMsg->dwBattleTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TIMEMAX;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}
	

	// 선도클럽존에서 배틀 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() || !pCLUB_CHIEF->IsChief() )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwCharID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCHAR->GETCONFRONTING().emTYPE != EMCONFT_NONE )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARCONFT;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;		
	}

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pCLUB_CHIEF->IsAllianceGuild( pCHAR->m_dwGuild ) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_ALLIANCE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(pCHAR->m_dwCharID) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsAlliance() || !pCLUB_INDIAN->IsChief() )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}


	if ( pCLUB_CHIEF->IsBattleAlliance( pCLUB_INDIAN->m_dwAlliance ) || pCLUB_CHIEF->IsBattleReady( pCLUB_INDIAN->m_dwAlliance ))
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_ALREADY2;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// 자신 클럽 확인
	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{

		const GLCLUBALLIANCE &sALLIANCE = *pos;
		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );

		if ( !pCLUB ) continue;

		if ( pCLUB->IsRegDissolution() )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_DISSOLUTION;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}

		// 최대 동시 진행 갯수 확인
		if ( pCLUB->GetAllBattleNum() >= GLCONST_CHAR::dwCLUB_BATTLE_MAX )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_MAX;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}

		if ( pCLUB->m_dwRank+1 < GLCONST_CHAR::dwCLUB_BATTLE_RANK )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_RANK;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}

	}


	//  상대 클럽 확인
	pos = pCLUB_INDIAN->m_setAlliance.begin();
	end = pCLUB_INDIAN->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;
		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );

		if ( !pCLUB ) continue;

		if ( pCLUB->IsRegDissolution() )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_DISSOLUTION2;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}

		// 최대 동시 진행 갯수 확인
		if ( pCLUB->GetAllBattleNum() >= GLCONST_CHAR::dwCLUB_BATTLE_MAX )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARMAX;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}	

		//	클럽 랭크 확인
		if ( pCLUB->m_dwRank+1 < GLCONST_CHAR::dwCLUB_BATTLE_RANK )
		{
			NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_TARRANK;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_FALSE;
		}
	}



	//	동맹내 클럽들끼리 배틀중인지 확인
	{		
		// 자신 클럽 확인
		CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
		CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
		for ( ; pos!=end; ++pos )
		{
			const GLCLUBALLIANCE &sALLIANCE = *pos;
			GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
			if( !pCLUB ) continue;

			CLUB_ALLIANCE_ITER posTar = pCLUB_CHIEF->m_setAlliance.begin();
			CLUB_ALLIANCE_ITER endTar = pCLUB_CHIEF->m_setAlliance.end();
			for ( ; posTar!=endTar; ++posTar )
			{
				const GLCLUBALLIANCE &sALLIANCE = *pos;

				if ( pCLUB->IsBattle( sALLIANCE.m_dwID ) || 
					pCLUB->IsBattleReady( sALLIANCE.m_dwID ))
				{
					NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_ALREADY;
					GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
					return S_FALSE;
				}
			}
		}
	}

	//	배틀 요청한 클럽 id 저장.
	pCLUB_CHIEF->m_dwReqClubID = pCLUB_INDIAN->m_dwID;
	pCLUB_CHIEF->m_dwBattleTime = pNetMsg->dwBattleTime;

	//	Note : 동맹 요청.
	GLMSG::SNET_ALLIANCE_BATTLE_REQ_ASK NetMsgAsk;
	NetMsgAsk.dwClubCharID = m_dwCharID;
	NetMsgAsk.dwBattleTime = pNetMsg->dwBattleTime;
	StringCchCopy ( NetMsgAsk.szClubName, CHAR_SZNAME, pCLUB_CHIEF->m_szName );
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgAsk );

	return S_OK;
}


// *****************************************************
// Desc: Agent에게 배틀요청 결과 FeedBack.
// *****************************************************
HRESULT GLCharAG::MsgClubBattleReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_BATTLE_REQ_ANS *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_REQ_ANS *) nmg;
	GLMSG::SNET_CLUB_BATTLE_REQ_FB NetMsgFB;

	if ( !GLCONST_CHAR::bCLUB_BATTLE )
	{
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		return S_FALSE;
	}

	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		return S_FALSE;		
	}

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() )
	{
		return S_FALSE;		
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwClubCharID );
	if ( !pCHAR )
	{
		return S_FALSE;
	}	

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( pCHAR->m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		return S_FALSE;
	}
	
	if ( !pCLUB_CHIEF->IsMaster(pCHAR->m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( !pNetMsg->bOK )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

		/*CString strText;
		strText.Format( ID2SERVERTEXT( "CLUB_BATTLE_BEGIN_REFUSE" ), pCLUB_INDIAN->m_szName, pCLUB_CHIEF->m_szName );

		GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
		NetMsg.SETTEXT ( strText.GetString() );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );*/

		GLMSG::SNET_CLUB_BATTLE_BEGIN_REFUSE NetMsg;
		StringCchCopy ( NetMsg.szGuild1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
		StringCchCopy ( NetMsg.szGuild2, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

		return S_FALSE;
	}

	//	요청한 클럽인지 검사.
	if ( pCLUB_CHIEF->m_dwReqClubID!=pCLUB_INDIAN->m_dwID )
	{
		return S_FALSE;
	}

	// 시간 검사
	if ( pCLUB_CHIEF->m_dwBattleTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN 
		|| pCLUB_CHIEF->m_dwBattleTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX  )
	{
		return S_FALSE;
	}

	NetMsgFB.emFB = EMCLUB_BATTLE_REQ_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

	GLCLUBBATTLE	sClubBattle;
	CTime cCurTime = CTime::GetCurrentTime();
	CTimeSpan cDelayTime(0,0,1,0);	
	CTime cStartTime = cCurTime + cDelayTime;
	CTimeSpan cBattleTime(0,0,pCLUB_CHIEF->m_dwBattleTime,0);
	CTime cEndTime = cStartTime + cBattleTime;

	// 클럽 배틀 대기열에 등록( 신청자 기준 ) 
	sClubBattle.m_dwCLUBID = pCLUB_INDIAN->m_dwID;
	sClubBattle.m_tStartTime = cStartTime.GetTime();
	sClubBattle.m_tEndTime = cEndTime.GetTime();
	StringCchCopy ( sClubBattle.m_szClubName, CHAR_SZNAME, pCLUB_INDIAN->m_szName);
	pCLUB_CHIEF->m_mapBattleReady.insert ( make_pair( sClubBattle.m_dwCLUBID, sClubBattle ) );	

	//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_BEGIN_CLT NetMsgClt;
	StringCchCopy ( NetMsgClt.szClubName, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );	
	
	StringCchCopy ( NetMsgClt.szClubName, CHAR_SZNAME, pCLUB_INDIAN->m_szName  );	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );


	//	db에 저장 ( A기준, B기준 저장 )
	CSetClubBattle *pDbActionA = new CSetClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwBattleTime );
	CSetClubBattle *pDbActionB = new CSetClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID, pCLUB_CHIEF->m_dwBattleTime );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}


	return S_OK;
}

// *****************************************************
// Desc: Agent에게 배틀요청 결과 FeedBack. ( 동맹 ) 
// *****************************************************
HRESULT	GLCharAG::MsgAllianceBattleReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLIANCE_BATTLE_REQ_ANS *pNetMsg = (GLMSG::SNET_ALLIANCE_BATTLE_REQ_ANS *) nmg;
	GLMSG::SNET_ALLIANCE_BATTLE_REQ_FB NetMsgFB;

	if ( !GLCONST_CHAR::bCLUB_BATTLE || !GLCONST_CHAR::bCLUB_BATTLE_ALLIANCE )
	{
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		return S_FALSE;
	}

	if ( GLGuidanceAgentMan::GetInstance().IsBattle() )
	{
		return S_FALSE;		
	}

	if ( GLClubDeathMatchAgentMan::GetInstance().IsBattle() )
	{
		return S_FALSE;		
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsAlliance() || !pCLUB_INDIAN->IsChief() )
	{
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwClubCharID );
	if ( !pCHAR )
	{
		return S_FALSE;
	}	

	if ( pCHAR->m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( pCHAR->m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		return S_FALSE;
	}
	
	if ( !pCLUB_CHIEF->IsMaster(pCHAR->m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() || !pCLUB_CHIEF->IsChief() )
	{
		return S_FALSE;
	}

	if ( !pNetMsg->bOK )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

		/*CString strText;
		strText.Format( ID2SERVERTEXT( "ALLIANCE_BATTLE_BEGIN_REFUSE" ), pCLUB_INDIAN->m_szName, pCLUB_CHIEF->m_szName );

		GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
		NetMsg.SETTEXT ( strText.GetString() );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );*/

		GLMSG::SNET_ALLIANCE_BATTLE_BEGIN_REFUSE NetMsg;
		StringCchCopy ( NetMsg.szParty1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
		StringCchCopy ( NetMsg.szParty2, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

		return S_FALSE;
	}

	//	요청한 클럽인지 검사.
	if ( pCLUB_CHIEF->m_dwReqClubID!=pCLUB_INDIAN->m_dwID )
	{
		return S_FALSE;
	}

	// 시간 검사
	if ( pCLUB_CHIEF->m_dwBattleTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN 
		|| pCLUB_CHIEF->m_dwBattleTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX  )
	{
		return S_FALSE;
	}

	NetMsgFB.emFB = EMALLIANCE_BATTLE_REQ_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

	GLCLUBBATTLE	sClubBattle;
	CTime cCurTime = CTime::GetCurrentTime();
	CTimeSpan cDelayTime(0,0,1,0);	
	CTime cStartTime = cCurTime + cDelayTime;
	CTimeSpan cBattleTime(0,0,pCLUB_CHIEF->m_dwBattleTime,0);
	CTime cEndTime = cStartTime + cBattleTime;

	// 클럽 배틀 대기열에 등록( 신청자 기준 ) 
	sClubBattle.m_dwCLUBID = pCLUB_INDIAN->m_dwID;
	sClubBattle.m_tStartTime = cStartTime.GetTime();
	sClubBattle.m_tEndTime = cEndTime.GetTime();
	sClubBattle.m_bAlliance = true;
	StringCchCopy ( sClubBattle.m_szClubName, CHAR_SZNAME, pCLUB_INDIAN->m_szName);
	pCLUB_CHIEF->m_mapBattleReady.insert ( make_pair( sClubBattle.m_dwCLUBID, sClubBattle ) );	

	
	//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_BEGIN_CLT NetMsgClt;

	StringCchCopy ( NetMsgClt.szClubName, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	NetMsgClt.bAlliance = true;
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );			
	
	StringCchCopy ( NetMsgClt.szClubName, CHAR_SZNAME, pCLUB_INDIAN->m_szName  );	
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	//	db에 저장 ( A기준, B기준 저장 )
	CSetClubBattle *pDbActionA = new CSetClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwBattleTime, true );
	CSetClubBattle *pDbActionB = new CSetClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID, pCLUB_CHIEF->m_dwBattleTime, true );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}


	return S_OK;

}

// *****************************************************
// Desc: Agent에게 배틀 휴전요청 
// *****************************************************
HRESULT GLCharAG::MsgClubBattleArmisticeReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ *) nmg;
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 휴전 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattle( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_NOBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattleStop( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_DISTIME;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pCLUB_INDIAN->m_dwMasterID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_OFFMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	배틀 요청한 클럽 id 저장.
	pCLUB_CHIEF->m_dwBattleArmisticeReq = pNetMsg->dwClubID;

	//	Note : 휴전 요청.
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_ASK NetMsgAsk;
	NetMsgAsk.dwClubID = pCLUB_CHIEF->m_dwID;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgAsk );

	return S_OK;
}


// *****************************************************
// Desc: Agent에게 배틀 휴전요청 ( 동맹 ) 
// *****************************************************
HRESULT	GLCharAG::MsgAllianceBattleArmisticeReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ *pNetMsg = (GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ *) nmg;
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 휴전 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	// 선도클럽존에서 휴전 안됨
	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() || !pCLUB_CHIEF->IsChief() ) 
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;

	}

	if ( !pCLUB_CHIEF->IsBattleAlliance( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_NOBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattleStop( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_DISTIME;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pCLUB_INDIAN->m_dwMasterID );
	if ( !pCHAR )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_OFFMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	배틀 요청한 클럽 id 저장.
	pCLUB_CHIEF->m_dwBattleArmisticeReq = pNetMsg->dwClubID;

	//	Note : 휴전 요청.
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_ASK NetMsgAsk;
	NetMsgAsk.dwClubID = pCLUB_CHIEF->m_dwID;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgAsk );

	return S_OK;

}

// *****************************************************
// Desc: Agent에게 배틀 휴전요청 결과 FeedBack.
// *****************************************************
HRESULT GLCharAG::MsgClubBattleArmisticeReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_ANS *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_ANS *) nmg;
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_FB   NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_CHIEF )
	{
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pCLUB_CHIEF->m_dwMasterID );
	if ( !pCHAR )
	{
		return S_FALSE;
	}

	if ( !pNetMsg->bOK )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	요청한 클럽인지 검사.
	if ( pCLUB_CHIEF->m_dwBattleArmisticeReq!=pCLUB_INDIAN->m_dwID )
	{
		return S_FALSE;
	}

	GLCLUBBATTLE* pClubBattle = pCLUB_CHIEF->GetClubBattle( pCLUB_INDIAN->m_dwID );
	if ( !pClubBattle )	return S_FALSE;

	int nKill = pClubBattle->m_wKillPoint;
	int nDeath = pClubBattle->m_wDeathPoint;

	//	휴전 요청 결과.
	NetMsgFB.emFB = EMCLUB_BATTLE_ARMISTICE_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );


	pCLUB_CHIEF->m_tLastBattle = CTime::GetCurrentTime().GetTime();
	pCLUB_CHIEF->m_dwBattleDraw++;
	pCLUB_CHIEF->DELBATTLECLUB( pCLUB_INDIAN->m_dwID );

	pCLUB_INDIAN->m_tLastBattle = CTime::GetCurrentTime().GetTime();
	pCLUB_INDIAN->m_dwBattleDraw++;
	pCLUB_INDIAN->DELBATTLECLUB( pCLUB_CHIEF->m_dwID );

	//	필드에 알림.
	GLMSG::SNET_CLUB_BATTLE_OVER_FLD NetMsgFld;
	NetMsgFld.emFB = EMCLUB_BATTLE_OVER_ARMISTICE;
    NetMsgFld.dwWinClubID = pCLUB_CHIEF->m_dwID;
	NetMsgFld.dwLoseClubID = pCLUB_INDIAN->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );


	//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_OVER_CLT NetMsgClt;
	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_ARMISTICE;
	
	NetMsgClt.dwClubID = pCLUB_CHIEF->m_dwID;	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );
	
	NetMsgClt.dwClubID = pCLUB_INDIAN->m_dwID;	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	GLMSG::SNET_CLUB_BATTLE_OVER_ARMISTICE NetMsgOvrArm;
	StringCchCopy ( NetMsgOvrArm.szGuild1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	StringCchCopy ( NetMsgOvrArm.szGuild2, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgOvrArm );

	GLMSG::SNET_CLUB_BATTLE_OVER_ARMISTICE_RESULT NetMsgOvrArmRes;
	NetMsgOvrArmRes.dwRes1 = nKill;
	NetMsgOvrArmRes.dwRes2 = nDeath;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgOvrArmRes );

	GLMSG::SNET_CLUB_BATTLE_RESULT NetMsgCbRes1;
	NetMsgCbRes1.dwWin = pCLUB_CHIEF->m_dwBattleWin;
	NetMsgCbRes1.dwLose = pCLUB_CHIEF->m_dwBattleLose;
	NetMsgCbRes1.dwDraw = pCLUB_CHIEF->m_dwBattleDraw;
	StringCchCopy ( NetMsgCbRes1.szString1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgCbRes1 );

	GLMSG::SNET_CLUB_BATTLE_RESULT NetMsgCbRes2;
	NetMsgCbRes2.dwWin = pCLUB_INDIAN->m_dwBattleWin;
	NetMsgCbRes2.dwLose = pCLUB_INDIAN->m_dwBattleLose;
	NetMsgCbRes2.dwDraw = pCLUB_INDIAN->m_dwBattleDraw;
	StringCchCopy ( NetMsgCbRes2.szString1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgCbRes2 );

	/*CString strText;
	strText.Format( ID2SERVERTEXT( "CLUB_BATTLE_OVER_ARMISTICE" ), pCLUB_CHIEF->m_szName, pCLUB_INDIAN->m_szName );

	
	GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	strText.Format ( ID2SERVERTEXT( "CLUB_BATTLE_OVER_ARMISTICE_RESULT" ), nKill, nDeath );	
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	strText.Format ( ID2SERVERTEXT( "CLUB_BATTLE_RESULT" ), pCLUB_CHIEF->m_szName, 
					   pCLUB_CHIEF->m_dwBattleWin, pCLUB_CHIEF->m_dwBattleLose, pCLUB_CHIEF->m_dwBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	
	strText.Format ( ID2SERVERTEXT( "CLUB_BATTLE_RESULT" ), pCLUB_INDIAN->m_szName, 
					   pCLUB_INDIAN->m_dwBattleWin, pCLUB_INDIAN->m_dwBattleLose, pCLUB_INDIAN->m_dwBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );*/



	//	db에 저장 ( A기준, B기준 저장 )
	CEndClubBattle *pDbActionA = new CEndClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, 
													  GLCLUBBATTLE::CLUB_BATTLE_ARMISTICE, 0, 0 );
	CEndClubBattle *pDbActionB = new CEndClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID,
													  GLCLUBBATTLE::CLUB_BATTLE_ARMISTICE, 0, 0 );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}

	return S_OK;
}



// *****************************************************
// Desc: Agent에게 배틀 휴전요청 결과 FeedBack. ( 동맹 )
// *****************************************************
HRESULT GLCharAG::MsgAllianceBattleArmisticeReqAns ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS *pNetMsg = (GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS *) nmg;
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_FB   NetMsgFB;

	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_INDIAN )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( !pCLUB_INDIAN->IsAlliance() || !pCLUB_INDIAN->IsChief() )
	{
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_CHIEF )
	{
		return S_FALSE;
	}

	PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pCLUB_CHIEF->m_dwMasterID );
	if ( !pCHAR )
	{
		return S_FALSE;
	}

	if ( !pNetMsg->bOK )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_REFUSE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	//	요청한 클럽인지 검사.
	if ( pCLUB_CHIEF->m_dwBattleArmisticeReq!=pCLUB_INDIAN->m_dwID )
	{
		return S_FALSE;
	}

	GLCLUBBATTLE* pClubBattle = pCLUB_CHIEF->GetClubBattle( pCLUB_INDIAN->m_dwID );
	if ( !pClubBattle )	return S_FALSE;

	int nKill = pClubBattle->m_wKillPoint;
	int nDeath = pClubBattle->m_wDeathPoint;

	//	휴전 요청 결과.
	NetMsgFB.emFB = EMALLIANCE_BATTLE_ARMISTICE_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );


	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;

		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
		if ( !pCLUB ) continue;

		pCLUB->m_tLastBattle = CTime::GetCurrentTime().GetTime();
		pCLUB->m_dwAllianceBattleDraw++;
		pCLUB->DELBATTLECLUB( pCLUB_INDIAN->m_dwID );
	}

	pos = pCLUB_INDIAN->m_setAlliance.begin();
	end = pCLUB_INDIAN->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE = *pos;

		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
		if ( !pCLUB ) continue;

		pCLUB->m_tLastBattle = CTime::GetCurrentTime().GetTime();
		pCLUB->m_dwAllianceBattleDraw++;
		pCLUB->DELBATTLECLUB( pCLUB_CHIEF->m_dwID );
	}

	//	필드에 알림.
	GLMSG::SNET_CLUB_BATTLE_OVER_FLD NetMsgFld;
	NetMsgFld.emFB = EMCLUB_BATTLE_OVER_ARMISTICE;
    NetMsgFld.dwWinClubID = pCLUB_CHIEF->m_dwID;
	NetMsgFld.dwLoseClubID = pCLUB_INDIAN->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_OVER_CLT NetMsgClt;
	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_ARMISTICE;
	
	NetMsgClt.dwClubID = pCLUB_CHIEF->m_dwID;	
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );
	
	NetMsgClt.dwClubID = pCLUB_INDIAN->m_dwID;	
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	GLMSG::SNET_ALLIANCE_BATTLE_OVER_ARMISTICE NetMsg;
	StringCchCopy ( NetMsg.szParty1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	StringCchCopy ( NetMsg.szParty2, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	GLMSG::SNET_ALLIANCE_BATTLE_OVER_ARMISTICE_RESULT NetMsgResult;
	NetMsgResult.dwRes1 = nKill;
	NetMsgResult.dwRes2 = nDeath;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgResult );

	GLMSG::SNET_ALLIANCE_BATTLE_RESULT NetMsgResult1;
	NetMsgResult1.dwWinner = pCLUB_CHIEF->m_dwAllianceBattleWin;
	NetMsgResult1.dwLose = pCLUB_CHIEF->m_dwAllianceBattleLose;
	NetMsgResult1.dwDraw = pCLUB_CHIEF->m_dwAllianceBattleDraw;
	StringCchCopy ( NetMsgResult1.szString1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgResult1 );

	GLMSG::SNET_ALLIANCE_BATTLE_RESULT NetMsgResult2;
	NetMsgResult2.dwWinner = pCLUB_INDIAN->m_dwAllianceBattleWin;
	NetMsgResult2.dwLose = pCLUB_INDIAN->m_dwAllianceBattleLose;
	NetMsgResult2.dwDraw = pCLUB_INDIAN->m_dwAllianceBattleDraw;
	StringCchCopy ( NetMsgResult2.szString1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgResult2 );

	/*CString strText;
	strText.Format( ID2SERVERTEXT( "ALLIANCE_BATTLE_OVER_ARMISTICE" ), pCLUB_CHIEF->m_szName, pCLUB_INDIAN->m_szName );	
	
	GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	strText.Format ( ID2SERVERTEXT( "ALLIANCE_BATTLE_OVER_ARMISTICE_RESULT" ), nKill, nDeath );	
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	strText.Format ( ID2SERVERTEXT( "ALLIANCE_BATTLE_RESULT" ), pCLUB_CHIEF->m_szName, 
					pCLUB_CHIEF->m_dwAllianceBattleWin, pCLUB_CHIEF->m_dwAllianceBattleLose, pCLUB_CHIEF->m_dwAllianceBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

		
	strText.Format ( ID2SERVERTEXT( "ALLIANCE_BATTLE_RESULT" ), pCLUB_INDIAN->m_szName, 
					pCLUB_INDIAN->m_dwAllianceBattleWin, pCLUB_INDIAN->m_dwAllianceBattleLose, pCLUB_INDIAN->m_dwAllianceBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );*/


	//	db에 저장 ( A기준, B기준 저장 )
	CEndClubBattle *pDbActionA = new CEndClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, 
													  GLCLUBBATTLE::CLUB_BATTLE_ARMISTICE, 0, 0, true );
	CEndClubBattle *pDbActionB = new CEndClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID,
													  GLCLUBBATTLE::CLUB_BATTLE_ARMISTICE, 0, 0, true );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}


	return S_OK;
}


// *****************************************************
// Desc: Agent에게 배틀 항복요청 
// *****************************************************
HRESULT GLCharAG::MsgClubBattleSubmissionReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_CLUB_BATTLE_SUBMISSION_REQ *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_SUBMISSION_REQ *) nmg;
	GLMSG::SNET_CLUB_BATTLE_SUBMISSION_REQ_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 항복 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattle( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_NOBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattleStop( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_DISTIME;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	//	동맹 요청 결과.
	NetMsgFB.emFB = EMCLUB_BATTLE_SUBMISSION_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	pCLUB_CHIEF->m_tLastBattle = CTime::GetCurrentTime().GetTime();
	pCLUB_CHIEF->m_dwBattleLose++;
	pCLUB_CHIEF->DELBATTLECLUB( pCLUB_INDIAN->m_dwID );

	pCLUB_INDIAN->m_tLastBattle = CTime::GetCurrentTime().GetTime();
	pCLUB_INDIAN->m_dwBattleWin++;
	pCLUB_INDIAN->DELBATTLECLUB( pCLUB_CHIEF->m_dwID );

	//	필드에 알림.
	GLMSG::SNET_CLUB_BATTLE_OVER_FLD NetMsgFld;
	NetMsgFld.emFB = EMCLUB_BATTLE_OVER_SUBMISSION;
    NetMsgFld.dwWinClubID = pCLUB_INDIAN->m_dwID;
	NetMsgFld.dwLoseClubID = pCLUB_CHIEF->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );


	//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_OVER_CLT NetMsgClt;
	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_SUBMISSION;	
	NetMsgClt.dwClubID = pCLUB_INDIAN->m_dwID;	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_TARSUBMISSION;		
	NetMsgClt.dwClubID = pCLUB_CHIEF->m_dwID;	
	GLAgentServer::GetInstance().SENDTOCLUBCLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );

	GLMSG::SNET_CLUB_BATTLE_OVER_SUBMISSION NetMsgCbOvrSub;
	StringCchCopy ( NetMsgCbOvrSub.szGuild1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	StringCchCopy ( NetMsgCbOvrSub.szGuild2, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgCbOvrSub );

	GLMSG::SNET_CLUB_BATTLE_RESULT NetMsgCbResult1;
	NetMsgCbResult1.dwWin = pCLUB_CHIEF->m_dwBattleWin;
	NetMsgCbResult1.dwLose = pCLUB_CHIEF->m_dwBattleLose;
	NetMsgCbResult1.dwDraw = pCLUB_CHIEF->m_dwBattleDraw;
	StringCchCopy ( NetMsgCbResult1.szString1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgCbResult1 );

	GLMSG::SNET_CLUB_BATTLE_RESULT NetMsgCbResult2;
	NetMsgCbResult2.dwWin = pCLUB_INDIAN->m_dwBattleWin;
	NetMsgCbResult2.dwLose = pCLUB_INDIAN->m_dwBattleLose;
	NetMsgCbResult2.dwDraw = pCLUB_INDIAN->m_dwBattleDraw;
	StringCchCopy ( NetMsgCbResult2.szString1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgCbResult2 );

	//CString strText;
	//strText.Format( ID2SERVERTEXT( "CLUB_BATTLE_OVER_SUBMISSION" ), pCLUB_CHIEF->m_szName, pCLUB_INDIAN->m_szName );

	//GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
	//NetMsg.SETTEXT ( strText.GetString() );
	//GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
	//
	//strText.Format ( ID2SERVERTEXT( "CLUB_BATTLE_RESULT" ), pCLUB_CHIEF->m_szName, 
	//				   pCLUB_CHIEF->m_dwBattleWin, pCLUB_CHIEF->m_dwBattleLose, pCLUB_CHIEF->m_dwBattleDraw );
	//NetMsg.SETTEXT ( strText.GetString() );
	//GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
	//
	//strText.Format ( ID2SERVERTEXT( "CLUB_BATTLE_RESULT" ), pCLUB_INDIAN->m_szName, 
	//				   pCLUB_INDIAN->m_dwBattleWin, pCLUB_INDIAN->m_dwBattleLose, pCLUB_INDIAN->m_dwBattleDraw );
	//NetMsg.SETTEXT ( strText.GetString() );
	//GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );


	//	db에 저장 ( A기준, B기준 저장 )
	CEndClubBattle *pDbActionA = new CEndClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, 
													  GLCLUBBATTLE::CLUB_BATTLE_SUBMISSION, 0, 0 );
	CEndClubBattle *pDbActionB = new CEndClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID,
													  GLCLUBBATTLE::CLUB_BATTLE_SUBMISSION_WIN, 0, 0 );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}

	return S_OK;
}


// *****************************************************
// Desc: Agent에게 배틀 항복요청 ( 동맹 ) 
// *****************************************************
HRESULT GLCharAG::MsgAllianceBattleSubmissionReq ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLIANCE_BATTLE_SUBMISSION_REQ *pNetMsg = (GLMSG::SNET_ALLIANCE_BATTLE_SUBMISSION_REQ *) nmg;
	GLMSG::SNET_ALLIANCE_BATTLE_SUBMISSION_REQ_FB NetMsgFB;

	PGLCHARAG  pChar = GLAgentServer::GetInstance().GetCharID ( m_dwCharID );
	if ( !pChar  ) return S_FALSE;

	// 선도클럽존에서 항복 안됨
	GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID ( pChar->m_sCurMapID );
	if ( pLandMan->m_bGuidBattleMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_GUIDMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( pLandMan->m_bClubDeathMatchMap )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_CDMMAP;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	

	if ( m_dwGuild==CLUB_NULL )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	GLCLUB *pCLUB_CHIEF = GLAgentServer::GetInstance().GetClubMan().GetClub ( m_dwGuild );
	if ( !pCLUB_CHIEF )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsMaster(m_dwCharID) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsAlliance() || !pCLUB_CHIEF->IsChief() )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattleAlliance( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_NOBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}

	if ( !pCLUB_CHIEF->IsBattleStop( pNetMsg->dwClubID ) )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_DISTIME;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}
	
	GLCLUB *pCLUB_INDIAN = GLAgentServer::GetInstance().GetClubMan().GetClub ( pNetMsg->dwClubID );
	if ( !pCLUB_INDIAN )
	{
		NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return S_FALSE;
	}	

	//	동맹 요청 결과.
	NetMsgFB.emFB = EMALLIANCE_BATTLE_SUBMISSION_FB_OK;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	CLUB_ALLIANCE_ITER pos = pCLUB_CHIEF->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_CHIEF->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{

		const GLCLUBALLIANCE &sALLIANCE = *pos;
		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
		if ( !pCLUB ) continue;

		pCLUB->m_tLastBattle = CTime::GetCurrentTime().GetTime();
		pCLUB->m_dwAllianceBattleLose++;
		pCLUB->DELBATTLECLUB( pCLUB_INDIAN->m_dwID );

	}

	pos = pCLUB_INDIAN->m_setAlliance.begin();
	end = pCLUB_INDIAN->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{

		const GLCLUBALLIANCE &sALLIANCE = *pos;
		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( sALLIANCE.m_dwID );
		if ( !pCLUB ) continue;

		pCLUB->m_tLastBattle = CTime::GetCurrentTime().GetTime();
		pCLUB->m_dwAllianceBattleWin++;
		pCLUB->DELBATTLECLUB( pCLUB_CHIEF->m_dwID );
	}

	//	필드에 알림.
	GLMSG::SNET_CLUB_BATTLE_OVER_FLD NetMsgFld;
	NetMsgFld.emFB = EMCLUB_BATTLE_OVER_SUBMISSION;
    NetMsgFld.dwWinClubID = pCLUB_INDIAN->m_dwID;
	NetMsgFld.dwLoseClubID = pCLUB_CHIEF->m_dwID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	
		//	클라이언트들에 알림. ( 동맹원들에게 알림 )
	GLMSG::SNET_CLUB_BATTLE_OVER_CLT NetMsgClt;
	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_SUBMISSION;	
	NetMsgClt.dwClubID = pCLUB_INDIAN->m_dwID;	
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_CHIEF->m_dwID, &NetMsgClt );

	NetMsgClt.emFB = EMCLUB_BATTLE_OVER_TARSUBMISSION;		
	NetMsgClt.dwClubID = pCLUB_CHIEF->m_dwID;	
	GLAgentServer::GetInstance().SENDTOALLIANCECLIENT ( pCLUB_INDIAN->m_dwID, &NetMsgClt );

	GLMSG::SNET_ALLIANCE_BATTLE_OVER_SUBMISSION NetMsgOvrSub;
	StringCchCopy ( NetMsgOvrSub.szParty1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	StringCchCopy ( NetMsgOvrSub.szParty2, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgOvrSub );

	GLMSG::SNET_ALLIANCE_BATTLE_RESULT NetMsgResult3;
	NetMsgResult3.dwWinner = pCLUB_CHIEF->m_dwAllianceBattleWin;
	NetMsgResult3.dwLose = pCLUB_CHIEF->m_dwAllianceBattleLose;
	NetMsgResult3.dwDraw = pCLUB_CHIEF->m_dwAllianceBattleDraw;
	StringCchCopy ( NetMsgResult3.szString1, CHAR_SZNAME, pCLUB_CHIEF->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgResult3 );

	GLMSG::SNET_ALLIANCE_BATTLE_RESULT NetMsgResult4;
	NetMsgResult4.dwWinner = pCLUB_INDIAN->m_dwAllianceBattleWin;
	NetMsgResult4.dwLose = pCLUB_INDIAN->m_dwAllianceBattleLose;
	NetMsgResult4.dwDraw = pCLUB_INDIAN->m_dwAllianceBattleDraw;
	StringCchCopy ( NetMsgResult4.szString1, CHAR_SZNAME, pCLUB_INDIAN->m_szName );	
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgResult4 );

	/*CString strText;
	strText.Format( ID2SERVERTEXT( "ALLIANCE_BATTLE_OVER_SUBMISSION" ), pCLUB_CHIEF->m_szName, pCLUB_INDIAN->m_szName );

	GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
	
	strText.Format ( ID2SERVERTEXT( "ALLIANCE_BATTLE_RESULT" ), pCLUB_CHIEF->m_szName, 
					   pCLUB_CHIEF->m_dwAllianceBattleWin, pCLUB_CHIEF->m_dwAllianceBattleLose, pCLUB_CHIEF->m_dwAllianceBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
	
	strText.Format ( ID2SERVERTEXT( "ALLIANCE_BATTLE_RESULT" ), pCLUB_INDIAN->m_szName, 
					   pCLUB_INDIAN->m_dwAllianceBattleWin, pCLUB_INDIAN->m_dwAllianceBattleLose, pCLUB_INDIAN->m_dwAllianceBattleDraw );
	NetMsg.SETTEXT ( strText.GetString() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );*/


	//	db에 저장 ( A기준, B기준 저장 )
	CEndClubBattle *pDbActionA = new CEndClubBattle ( pCLUB_CHIEF->m_dwID, pCLUB_INDIAN->m_dwID, 
													  GLCLUBBATTLE::CLUB_BATTLE_SUBMISSION, 0, 0, true );
	CEndClubBattle *pDbActionB = new CEndClubBattle ( pCLUB_INDIAN->m_dwID, pCLUB_CHIEF->m_dwID,
													  GLCLUBBATTLE::CLUB_BATTLE_SUBMISSION_WIN, 0, 0, true );
	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( pDbMan )
	{
		pDbMan->AddJob ( pDbActionA );
		pDbMan->AddJob ( pDbActionB );
	}



	return S_OK;
}