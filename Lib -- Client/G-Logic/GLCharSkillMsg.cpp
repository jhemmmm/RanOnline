#include "stdafx.h"
#include "./GLChar.h"

#include "./GLGaeaServer.h"
#include "./GLSchoolFreePK.h"
#include "GLClubDeathMatch.h"

/*pvp tyranny, Juver, 2017/08/24 */
#include "GLPVPTyrannyField.h"  

/*school wars, Juver, 2018/01/19 */
#include "GLPVPSchoolWarsField.h"  

/*pvp capture the flag, Juver, 2018/01/31 */
#include "GLPVPCaptureTheFlagField.h"
#include "../Lib -- ClientUI/Interface/GameTextControl.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLChar::MsgReqLearnSkill ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_LEARNSKILL *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL *) nmg;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return S_FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_SKILL )	return E_FAIL;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		//	이미 습득한 스킬.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = EMSKILL_LEARN_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		//	스킬 습득 요구 조건을 충족하지 못합니다.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = emSKILL_LEARNCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	//	Note : 스킬 습득함.
	//
	LEARN_SKILL(sSKILL_ID);

	//	Note : 스킬 습득으로 인한 퀘스트 시작 점검.
	//
	QuestStartFromGetSKILL ( sSKILL_ID );

	//	Note : 소모성 아이템 제거.
	//
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY, true );

	//	스킬 배움 성공.
	GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
	NetMsgFB.skill_id = sSKILL_ID;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	스킬 배울때 소모된 포인트 업데이트.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//	Note : passive skill 의 경우 케릭에 적용되는 속성값을 클라이언트들에 동기화.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}
//LearnSkill
	HRESULT GLChar::MsgReqLearnSkillNonInven ( NET_MSG_GENERIC* nmg )

		{

		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN *) nmg;

		SNATIVEID sSKILL_ID = pNetMsg->sSKILLID;

		if ( !IsValidBody() ) return E_FAIL;

		if( !GLCONST_CHAR::bClickToUnlockSkills ) // TDEV DISABLE CLICK TO UNLOCK SKILLS

		{

		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB NetMsgFB;

		NetMsgFB.skill_id = sSKILL_ID;

		NetMsgFB.emCHECK = EMSKILL_LEARN_TJ;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;

		}

		else

		{

		if ( ISLEARNED_SKILL(sSKILL_ID) )

			{

		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB NetMsgFB;

		NetMsgFB.skill_id = sSKILL_ID;

		NetMsgFB.emCHECK = EMSKILL_LEARN_ALREADY;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;

				}

		EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);

		if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )

			{

		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB NetMsgFB;

		NetMsgFB.skill_id = sSKILL_ID;

		NetMsgFB.emCHECK = emSKILL_LEARNCHECK;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;

			}

		LEARN_SKILL_NONSCROLL(sSKILL_ID);

		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB NetMsgFB;

		NetMsgFB.skill_id = sSKILL_ID;

		NetMsgFB.emCHECK = EMSKILL_LEARN_OK;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;

		NetMsgSkp.dwSkillPoint = m_dwSkillPoint;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

		//Update Any money type

		GLMSG::SNETPC_UPDATE_MONEYTYPE NetMsgMoney;

		NetMsgMoney.lnGold = m_lnMoney;

		//NetMsgMoney.lnPremP = m_lnPremP;

		//NetMsgMoney.lnVoteP = m_lnVoteP;

		//NetMsgMoney.lnContribP = m_lnContribP; //add contributionpoint by CNDev

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );

		if ( !pSkill ) return E_FAIL;

		if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )

		{

		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;

		NetMsgBrd.dwGaeaID = m_dwGaeaID;

		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;

		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

		}

		return S_OK;

		}

			}

HRESULT GLChar::MsgReqSkillUp ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_SKILLUP *pNetMsg = (GLMSG::SNETPC_REQ_SKILLUP *) nmg;

	EMSKILL_LEARNCHECK emSKILL_LVLUPCHECK = EMSKILL_LEARN_UNKNOWN;
	SCHARSKILL* pCHARSKILL = GETLEARNED_SKILL(pNetMsg->skill_id);
	if ( !pCHARSKILL )
	{
		//	정상적으로 발생 할 수 없는 상황.
		return E_FAIL;
	}

	emSKILL_LVLUPCHECK = CHECKLEARNABLE_SKILL(pNetMsg->skill_id);
	if ( emSKILL_LVLUPCHECK!=EMSKILL_LEARN_OK )
	{
		//	랩업 조건이 부족합니다. FB 메시지.
		GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
		NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
		NetMsgFB.emCHECK = emSKILL_LVLUPCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//	Note : 스킬 LEVEL UP.
	//
	WORD wToLevel = pCHARSKILL->wLevel + 1;
	LVLUP_SKILL ( pNetMsg->skill_id, wToLevel );

	//	랩업. FB 메시지.
	GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
	
	NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
	NetMsgFB.sSkill.wLevel = wToLevel;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	GLMSG::SNETPC_UPDATE_MONEYTYPE NetMsgMoney;
	NetMsgMoney.lnGold = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );

	//	스킬 배울때 소모된 포인트 업데이트.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//	Note : passive skill 의 경우 케릭에 적용되는 속성값을 클라이언트들에 동기화.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkill ( NET_MSG_GENERIC* nmg )
{
	static bool bFirst = true;
	//static int nCnt = 0;

	CString strTextGen = "";

	//CONSOLEMSG_WRITE( "MsgReqSkill Start" );

	//if( GLGaeaServer::GetInstance().m_bEmptyMsg )						return S_OK;
	if ( !IsValidBody() )												return E_FAIL;
	if ( m_pLandMan && m_pLandMan->IsPeaceZone() )						return E_FAIL;

#if !defined(KR_PARAM) && !defined(KRT_PARAM)

	// 딜래이 체크
	if ( m_fSkillDelay < 0.3f )											return E_FAIL;  // 05.11.30  0.5f -> 0.3f

#endif

	bool bFreePKMap		= m_pLandMan->IsFreePK();
	bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
	bool bBRIGHTEVENT   = GLGaeaServer::GetInstance().IsBRIGHTEVENT();

	GLMSG::SNETPC_REQ_SKILL *pNetMsg = (GLMSG::SNETPC_REQ_SKILL *) nmg;
	SNATIVEID skill_id = pNetMsg->skill_id;

	// 발동 스킬일경우 현재 발동 스킬인지 검사
	if ( pNetMsg->bDefenseSkill )
	{
		if ( pNetMsg->skill_id != m_sDefenseSkill.m_dwSkillID ) return E_FAIL;
		if ( !m_bDefenseSkill ) return E_FAIL;
	}

	//	스킬 정보 가져옴.
	const PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )	return E_FAIL;
	const SKILL::SEXT_DATA &sEXT_DATA = pSkill->m_sEXT_DATA;
	bool benermy = ( SIDE_ENEMY==pSkill->m_sBASIC.emIMPACT_SIDE );

	if( pNetMsg->bHover && !m_bHoverConfirm && !m_bVehicle ){
		CDebugSet::ToFileWithTime( "_skill_hover.txt", "[%d]%s [%d~%d]%s vehicle activation illegal", m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName() );
		return E_FAIL;
	}

	if( pNetMsg->wIMPACT_TAR != (WORD)pSkill->m_sBASIC.emIMPACT_TAR ){
		CDebugSet::ToFileWithTime( "_skill_impact.txt", "[%d]%s [%d~%d]%s emIMPACT_TAR mismatched %d != %d", m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pNetMsg->wIMPACT_TAR, pSkill->m_sBASIC.emIMPACT_TAR );
			GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
			DropMsg.dwCHARID  = GetCharID();
			DropMsg.bCheat = true;
			DropMsg.nCheatType = 2;
			GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

			/*GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
			//strTextGen.Format( "%s has been automatically banned due to skill impact manipulation.", m_szName );
			strTextGen.Format( ID2SERVERTEXT("GERBERSEC_IMPACT_MANI"),m_szName );
			NetMsgGen.SETTEXT ( strTextGen.GetString() );
			NetMsgGen.bNotify = true;
			NetMsgGen.iColor = 5;
			GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
			strTextGen = "";

			GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);*/
		return E_FAIL;
	}

	if( pNetMsg->wIMPACT_SIDE != (WORD)pSkill->m_sBASIC.emIMPACT_SIDE ){
		CDebugSet::ToFileWithTime( "_skill_impact.txt", "[%d]%s [%d~%d]%s emIMPACT_SIDE mismatched %d != %d", m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pNetMsg->wIMPACT_SIDE, pSkill->m_sBASIC.emIMPACT_SIDE );
			GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
			DropMsg.dwCHARID  = GetCharID();
			DropMsg.bCheat = true;
			DropMsg.nCheatType = 2;
			GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

			/*GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
			//strTextGen.Format( "%s has been automatically banned due to skill impact manipulation.", m_szName );
			strTextGen.Format( ID2SERVERTEXT("GERBERSEC_IMPACT_MANI"),m_szName );
			NetMsgGen.SETTEXT ( strTextGen.GetString() );
			NetMsgGen.bNotify = true;
			NetMsgGen.iColor = 5;
			GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
			strTextGen = "";

			GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);*/
		return E_FAIL;
	}

	if( pNetMsg->wIMPACT_REALM != (WORD)pSkill->m_sBASIC.emIMPACT_REALM ){
		CDebugSet::ToFileWithTime( "_skill_impact.txt", "[%d]%s [%d~%d]%s emIMPACT_REALM mismatched %d != %d", m_dwCharID, m_szName, 
			pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pNetMsg->wIMPACT_REALM, pSkill->m_sBASIC.emIMPACT_REALM );
			GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
			DropMsg.dwCHARID  = GetCharID();
			DropMsg.bCheat = true;
			DropMsg.nCheatType = 2;
			GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

			/*GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
			//strTextGen.Format( "%s has been automatically banned due to skill impact manipulation.", m_szName );
			strTextGen.Format( ID2SERVERTEXT("GERBERSEC_IMPACT_MANI"),m_szName );
			NetMsgGen.SETTEXT ( strTextGen.GetString() );
			NetMsgGen.bNotify = true;
			NetMsgGen.iColor = 5;
			GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
			strTextGen = "";

			GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);*/
		return E_FAIL;
	}

	float twHighestVelo = 0.0f;
	bool bCheckProceedTW(false);
	for( int i = 0; i<SYSTEM_BUFF_SIZE; i++ ){
		if ( m_sSYSTEM_BUFF[i].sNATIVEID != NATIVEID_NULL() ){
			PGLSKILL pSkSystemBuff = GLSkillMan::GetInstance().GetData ( m_sSYSTEM_BUFF[i].sNATIVEID );
			if( pSkSystemBuff ){
				for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec ){
					if ( pSkSystemBuff->m_sAPPLY.sSpecs[nSpec].emSPEC != EMSPECA_MOVEVELO ) continue;
					if ( pSkSystemBuff->m_sAPPLY.sSpecs[nSpec].sSPEC[m_sSYSTEM_BUFF[i].wLEVEL].fVAR1 > twHighestVelo )
						twHighestVelo = pSkSystemBuff->m_sAPPLY.sSpecs[nSpec].sSPEC[m_sSYSTEM_BUFF[i].wLEVEL].fVAR1;
						bCheckProceedTW = true;
				}
			}
		}	
	}

	if( pNetMsg->twVelo > twHighestVelo && bCheckProceedTW ){
		CDebugSet::ToFileWithTime( "_skill_twHighestVelo.txt", "[%d]%s highest system buff velo mismatch:%f client:%f", m_dwCharID, m_szName, 
			twHighestVelo, pNetMsg->twVelo );
		return E_FAIL;
	}

	if( pNetMsg->fRunVelo > GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO ){
		CDebugSet::ToFileWithTime( "_skill_charVelo.txt", "[%d]%s char run velo mismatch:%f client:%f", m_dwCharID, m_szName, 
			GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO, pNetMsg->fRunVelo );
		return E_FAIL;
	}

	if( pNetMsg->fWalkVelo > GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO ){
		CDebugSet::ToFileWithTime( "_skill_charVelo.txt", "[%d]%s char walk velo mismatch:%f client:%f", m_dwCharID, m_szName, 
			GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO, pNetMsg->fWalkVelo );
		return E_FAIL;
	}



	/*skill validity check, Juver, 2017/11/26 */
	SKILL_MAP_ITER find_iter = m_ExpSkills.find ( pSkill->m_sBASIC.sNATIVEID.dwID );
	if ( find_iter!=m_ExpSkills.end() )	
	{
		WORD wlevel = (*find_iter).second.wLevel;
		if ( wlevel < SKILL::MAX_LEVEL )
		{
			if ( pNetMsg->wskill_level != wlevel )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s level mismatch server:%d client:%d", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), wlevel, pNetMsg->wskill_level );
				return E_FAIL;
			}

			bool bCheckProceedSK(false);
			float skHighestVelo = 0.0f;
			for( int i = 0; i<SKILLFACT_SIZE; i++ ){
				if ( m_sSKILLFACT[i].sNATIVEID != NATIVEID_NULL() ){
					PGLSKILL pSkFactBuff = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
					if( pSkFactBuff ){
						for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec ){
							if( pSkFactBuff->m_sAPPLY.sSpecs[nSpec].emSPEC != EMSPECA_MOVEVELO ) continue;
							if( pSkFactBuff->m_sAPPLY.sSpecs[nSpec].sSPEC[m_sSKILLFACT[i].wLEVEL].fVAR1 > skHighestVelo )
								skHighestVelo = pSkFactBuff->m_sAPPLY.sSpecs[nSpec].sSPEC[m_sSKILLFACT[i].wLEVEL].fVAR1;
								bCheckProceedSK = true;
						}
					}
				}
			}

		if( pNetMsg->skVelo > skHighestVelo && bCheckProceedSK ){
			CDebugSet::ToFileWithTime( "_skill_skHighestVelo.txt", "[%d]%s highest skill buff velo mismatch:%f client:%f", m_dwCharID, m_szName, 
				skHighestVelo, pNetMsg->skVelo );
			return E_FAIL;
		}

			if ( pNetMsg->wtar_range != pSkill->m_sBASIC.wTARRANGE )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s wTARRANGE mismatch server:%d client:%d", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pSkill->m_sBASIC.wTARRANGE, pNetMsg->wtar_range );
				return E_FAIL;
			}

			const SKILL::CDATA_LVL &sapply_data = pSkill->m_sAPPLY.sDATA_LVL[wlevel];

			if( pNetMsg->wapply_tarnum != sapply_data.wTARNUM ){
				CDebugSet::ToFileWithTime( "_skill_tarnum.txt", "[%d]%s [%d~%d]%s target quantity not matched [%d~%d]", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), pNetMsg->wapply_tarnum, sapply_data.wTARNUM );

				GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
				DropMsg.dwCHARID  = GetCharID();
				DropMsg.bCheat = true;
				DropMsg.nCheatType = 2;
				GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

				/*GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
				//strTextGen.Format( "%s has been automatically banned due to target quantity manipulation.", m_szName );
				strTextGen.Format( ID2SERVERTEXT("GERBERSEC_QTY_MANI"),m_szName );
				NetMsgGen.SETTEXT ( strTextGen.GetString() );
				NetMsgGen.bNotify = true;
				NetMsgGen.iColor = 5;
				GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
				strTextGen = "";

				GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);*/
				return E_FAIL;
			}

			if ( pNetMsg->wapply_range != sapply_data.wAPPLYRANGE )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s wAPPLYRANGE mismatch server:%d client:%d", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), sapply_data.wAPPLYRANGE, pNetMsg->wapply_range );
				return E_FAIL;
			}

			if ( pNetMsg->wapply_num != sapply_data.wAPPLYNUM )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s wAPPLYNUM mismatch server:%d client:%d", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), sapply_data.wAPPLYNUM, pNetMsg->wapply_num );
				return E_FAIL;
			}

			if ( pNetMsg->wapply_angle != sapply_data.wAPPLYANGLE )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s wAPPLYANGLE mismatch server:%d client:%d", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), sapply_data.wAPPLYANGLE, pNetMsg->wapply_angle );
				return E_FAIL;
			}
		
			if ( GET_SLOT_ITEMDATA( GetCurRHand() ) )
			{
				if ( pNetMsg->wrange_rhand != GET_SLOT_ITEMDATA( GetCurRHand() )->sSuitOp.wAttRange )
				{
					//CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s RWeapon Range mismatch server:%d client:%d", m_dwCharID, m_szName, 
						//pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), GET_SLOT_ITEMDATA( GetCurRHand() )->sSuitOp.wAttRange, pNetMsg->wrange_rhand );
					return E_FAIL;
				}
			}

			if ( GET_SLOT_ITEMDATA( GetCurLHand() ) )
			{
				if ( pNetMsg->wrange_lhand != GET_SLOT_ITEMDATA( GetCurLHand() )->sSuitOp.wAttRange )
				{
					//CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s LWeapon Range mismatch server:%d client:%d", m_dwCharID, m_szName, 
						//pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), GET_SLOT_ITEMDATA( GetCurLHand() )->sSuitOp.wAttRange, pNetMsg->wrange_lhand );
					return E_FAIL;
				}
			}
		}
	}


	/*bool bPassiveTriggers(false);

	if( pNetMsg->fTarRange > (GLCHARLOGIC::m_fSUM_TARRANGE+GLCHARLOGIC::m_sSUM_PASSIVE.m_fTARRANGE) ) bPassiveTriggers = true;
	if( pNetMsg->fAttackRange > (GLCHARLOGIC::m_fSUM_SKILL_ATTACKRANGE+GLCHARLOGIC::m_sSUM_PASSIVE.m_fSUM_SKILL_ATTACKRANGE) ) bPassiveTriggers = true;
	if( pNetMsg->fApplyRange > (GLCHARLOGIC::m_fSUM_SKILL_APPLYRANGE+GLCHARLOGIC::m_sSUM_PASSIVE.m_fSUM_SKILL_APPLYRANGE) ) bPassiveTriggers = true;

	if( bPassiveTriggers ){
		CDebugSet::ToFileWithTime( 
			"_skill_passive-v2.txt", "[%d]%s passive trigger TarRange:%f/%f - AtkRange:%f/%f ApplyRange:%f/%f", 
			m_dwCharID,m_szName, 
			pNetMsg->fTarRange, GLCHARLOGIC::m_fSUM_TARRANGE, 
			pNetMsg->fAttackRange , GLCHARLOGIC::m_fSUM_SKILL_ATTACKRANGE,
			pNetMsg->fApplyRange, GLCHARLOGIC::m_fSUM_SKILL_APPLYRANGE
		);
		GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
		DropMsg.dwCHARID  = GetCharID();
		GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

		GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
		strTextGen.Format( ID2SERVERTEXT("GERBERSEC_RANGE_MANI"),m_szName );
		NetMsgGen.SETTEXT ( strTextGen.GetString() );
		NetMsgGen.bNotify = true;
		NetMsgGen.iColor = 5;
		GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
		strTextGen = "";

		GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);
		return E_FAIL;
	}*/


	//	Note : 타겟 지정.
	//
	m_TargetID.vPos = pNetMsg->vTARPOS;
	m_TargetID.emCrow = pNetMsg->sTARGET_IDS[0].GETCROW();
	m_TargetID.dwID = pNetMsg->sTARGET_IDS[0].GETID();

	WORD wSkillRange = GETSKILLRANGE_TAR(*pSkill);

	float movementSpdMismatch = abs(pNetMsg->moveVelo - GetMoveVelo());

	bool noNeedtoCompare(true);
	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec ){
		if( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC == EMSPECA_MOVEVELO ){
			noNeedtoCompare = false;
			break;
		}
	}
	if( GetMoveVelo() > pNetMsg->moveVelo ) noNeedtoCompare = false;
	

	if( pNetMsg->moveVelo >= 1000.0f && noNeedtoCompare ){
		CDebugSet::ToFileWithTime( "_skill_veloV2.txt", "[%d]%s casted %s Velo, garapal (%f,%f)", m_dwCharID,m_szName, pSkill->GetName(), pNetMsg->moveVelo, GetMoveVelo() );
		GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
		DropMsg.dwCHARID  = GetCharID();
		GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);
		return E_FAIL;
	}

	if( pNetMsg->moveVelo != GetMoveVelo() && (movementSpdMismatch > 100.0f) && noNeedtoCompare ){
		CDebugSet::ToFileWithTime( "_skill_veloV2.txt", "[%d]%s casted %s Velo not matched %f != %f", m_dwCharID,m_szName, pSkill->GetName(), pNetMsg->moveVelo, GetMoveVelo() );
		return E_FAIL;
	}

	if ( pNetMsg->moveVelo > GetMoveVelo() && (movementSpdMismatch > 100.0f) && noNeedtoCompare ){
		CDebugSet::ToFileWithTime( "_skill_veloV2.txt", "[%d]%s casted %s Velo not matched %f > %f", m_dwCharID,m_szName, pSkill->GetName(), pNetMsg->moveVelo, GetMoveVelo() );
		return E_FAIL;
	}

	// 대련 종료후 무적타임이면 공격 무시
	if ( m_sCONFTING.IsPOWERFULTIME () ) return E_FAIL;
	
	//	Note : 타겟의 유효성 점검.
	//

	/*skill validity check, Juver, 2017/11/27 */
	std::vector<STARID> vectargets;
	vectargets.reserve( EMTARGET_NET );
	WORD wAdditionalRange = 30;
	float fDetectRange = 0.0f;
	float fDistance = 0.0f;

	bool bRangeCheck(true);
	if( pSkill->m_sBASIC.sNATIVEID.wMainID == 14 && pSkill->m_sBASIC.sNATIVEID.wSubID == 14 ) 
		bRangeCheck = false;

	for ( WORD i=0; i<pNetMsg->wTARGET_NUM; ++i )
	{
		//	Note : 유효성 검사.
		//
		if ( i >= EMTARGET_NET )	continue;

		STARGETID sTARID(pNetMsg->sTARGET_IDS[i].GETCROW(),pNetMsg->sTARGET_IDS[i].GETID());
		GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
		if ( !pTARGET )	continue;

		 fDistance = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-pTARGET->GetPosition()) );
		 fDetectRange = GETBODYRADIUS() + pTARGET->GetBodyRadius() + GETSKILLRANGE_TAR(*pSkill) + GETSKILLRANGE_APPLY(*pSkill,pNetMsg->wskill_level) + wAdditionalRange;
		 if( fDistance > fDetectRange && bRangeCheck )
		 {
			 //CDebugSet::ToFileWithTime( "_skill_longrange.txt", "[%d]%s casted %s detected range: %g, distance %g", m_dwCharID,m_szName, pSkill->GetName(), fDetectRange, fDistance );
			 //if( pNetMsg->tarLength[i] < fDistance ){
				 //CDebugSet::ToFileWithTime( "_skill_longrangeEXP.txt", "[%d]%s casted %s distance client: %g, distance server %g . no action done.", m_dwCharID,m_szName, pSkill->GetName(), pNetMsg->tarLength[i], fDistance );
			 //}
			 continue;
		 }

		// 공격스킬만 검사
		if ( benermy )
		{

#if !defined(KR_PARAM) && !defined(KRT_PARAM) && !defined(JP_PARAM)

			// 타겟과의 거리 검사
			D3DXVECTOR3 vTarPos = pTARGET->GetPosition();
			float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
			WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + wSkillRange + 7;
			if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();
			WORD wAttackAbleDis = wAttackRange + 20;

			// 공격 유효거리에 있는 케릭터 검사
			if ( fDist > wAttackAbleDis )
			{
				//	Note : 자신에게.
				GLMSG::SNETPC_ATTACK_AVOID NetMsg;
				NetMsg.emTarCrow	= sTARID.emCrow;
				NetMsg.dwTarID		= sTARID.dwID;

				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

				//	Note : 주변 클라이언트들에게 메세지 전송.
				//
				GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
				NetMsgBrd.dwGaeaID	= m_dwGaeaID;
				NetMsgBrd.emTarCrow	= sTARID.emCrow;
				NetMsgBrd.dwTarID	= sTARID.dwID;

				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
				continue;
			}

#endif

		}

		bool bREACTIONALBE  = IsReActionable( pTARGET, benermy );			
		if ( !bREACTIONALBE )												continue;

		bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
		if ( GetSchool() == pTARGET->GetSchool() )	bSCHOOL_FREEPK = false;

		if( pSkill->m_sBASIC.sNATIVEID.wMainID == 14 && 
			pSkill->m_sBASIC.sNATIVEID.wSubID == 15 && 
			m_pLandMan->GetMapID().wMainID == 224 ) continue;

		if( pSkill->m_sBASIC.sNATIVEID.wMainID == 33 && 
			pSkill->m_sBASIC.sNATIVEID.wSubID == 37 && 
			m_pLandMan->GetMapID().wMainID == 224 ) continue;

		//	Note : 학원간 자유 pk일 때는 다른 학원생을 부활 시킬수 없음.
		if ( pSkill->m_sAPPLY.IsSpec( EMSPECA_REBIRTH ) && bSCHOOL_FREEPK ) continue;
		
		if ( pTARGET->GetCrow() == CROW_PC )
		{
			PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetChar ( sTARID.dwID );
			if ( pCHAR )
			{
				// 부활일 경우에 상대방이 부활을 가능할 경우에만 실행 된다.
				if( pSkill->m_sBASIC.sNATIVEID.wMainID == 14 && pSkill->m_sBASIC.sNATIVEID.wSubID == 15 && pCHAR->m_bNon_Rebirth == TRUE )
				{
					//	Note : 스킬 구동 실패 FB 메시지.
					{
						GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
						strcpy( NetMsgFB.szName, pCHAR->m_szName );
						NetMsgFB.emSKILL_FB = EMSKILL_NOTREBIRTH;
						GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
					}
					{
						GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED NetMsgFB;
						strcpy( NetMsgFB.szName, m_szName );
						GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
					}
					return E_FAIL;
				}
				// 대련 종료후 무적타임이면 공격 무시
				if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )					continue;

				bool bClubBattle = false;
				bool bClubDeathMatch = false; 
				bool bPVPTyranny = false;			/*pvp tyranny, Juver, 2017/08/24 */
				bool bPVPSchoolWars = false;		/*school wars, Juver, 2018/01/19 */
				bool bPVPCaptureTheFlag = false;	/*pvp capture the flag, Juver, 2018/01/31 */

				if ( m_pLandMan->IsClubBattleZone() )
				{
					GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
					GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
					GLCLUB *pTarClub = cClubMan.GetClub ( pCHAR->m_dwGuild );

					if ( pMyClub && pTarClub )
					{
						bool bClub = pMyClub->IsBattle( pCHAR->m_dwGuild );
						bool bAlliance = pMyClub->IsBattleAlliance ( pTarClub->m_dwAlliance );

						bClubBattle = ( bClub || bAlliance );
					}
				}

				if ( m_pLandMan->m_bClubDeathMatchMap )
				{
					GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( m_pLandMan->m_dwClubMapID );
					if ( pCDM && pCDM->IsBattle() ) bClubDeathMatch = true;	
				}

				/*pvp tyranny, Juver, 2017/08/24 */
				if ( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() )
					bPVPTyranny = true;	

				/*school wars, Juver, 2018/01/19 */
				if ( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() )
					bPVPSchoolWars = true;	

				/*pvp capture the flag, Juver, 2018/01/31 */
				if ( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() )
					bPVPCaptureTheFlag = true;	

				/*pvp tyranny, Juver, 2017/08/24 */
				/*school wars, Juver, 2018/01/19 */
				/*pvp capture the flag, Juver, 2018/01/31 */
				if ( !(bGuidBattleMap|| bClubDeathMatch ||bSCHOOL_FREEPK||bFreePKMap||bBRIGHTEVENT||IsConflictTarget(pTARGET) || bClubBattle ||
					bPVPTyranny||bPVPSchoolWars || bPVPCaptureTheFlag  ) )
				{
					//	Note : 상대방이 범죄자가 아니고 정당방위 시간이 지정되지 않았을때는 
					//		자신이 적대행위를 시 작함.
					if ( !IS_PLAYHOSTILE(pCHAR->m_dwCharID) && !pCHAR->ISOFFENDER() && benermy )
					{
						int nBRIGHT(GLCONST_CHAR::nPK_TRY_BRIGHT_POINT), nLIFE(GLCONST_CHAR::nPK_TRY_LIVING_POINT);

						//	Note : 속성수치 변화.
						m_nBright += nBRIGHT;

						GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
						NetMsg.nBright = m_nBright;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

						GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
						NetMsgBrd.dwGaeaID = m_dwGaeaID;
						NetMsgBrd.nBright = m_nBright;
						SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

						//	Note : 생활점수 변화.
						m_nLiving += nLIFE;

						GLMSG::SNETPC_UPDATE_LP NetMsgLp;
						NetMsgLp.nLP = m_nLiving;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgLp);

						//	Note : 적대행위 로그 기록.
						GLITEMLMT::GetInstance().ReqAction
						(
							m_dwCharID,					//	당사자.
							EMLOGACT_HOSTILE,			//	행위.
							ID_CHAR, pCHAR->m_dwCharID,	//	상대방.
							0,							//	exp
							nBRIGHT,					//	bright
							nLIFE,						//	life
							0							//	money
						);
					}
				}

				if ( benermy && !IsConflictTarget(pTARGET) )
				{
					//	Note : 적대 행위자에게 피해자 등록. ( pk )
					AddPlayHostile ( pCHAR->m_dwCharID, TRUE, bClubBattle );

					//	Note : 적대 피해자에게 적대자 등록. ( PK )
					pCHAR->AddPlayHostile ( m_dwCharID, FALSE, bClubBattle );

					STARGETID sTargetID = STARGETID ( CROW_PC, m_dwGaeaID, m_vPos );
					
					
					// 공격자와 공격대상자 모두 팻을 공격모드로 변경
					PGLPETFIELD pEnemyPet = GLGaeaServer::GetInstance().GetPET ( pCHAR->m_dwPetGUID );
					if ( pEnemyPet && pEnemyPet->IsValid () && !pEnemyPet->IsSTATE ( EM_PETACT_ATTACK ) )
					{
						D3DXVECTOR3 vOwnerPos, vDist;
						float fDist;
						vOwnerPos = pCHAR->GetPosition ();
						vDist = pEnemyPet->m_vPos - vOwnerPos;
						fDist = D3DXVec3Length(&vDist);

						// 일정거리 안에 있으면 
						if ( fDist <= GLCONST_PET::fWalkArea )
						{
							pEnemyPet->ReSetAllSTATE ();
							pEnemyPet->SetSTATE ( EM_PETACT_ATTACK );

							GLMSG::SNETPET_ATTACK NetMsg;
							NetMsg.sTarID = sTargetID;
							GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );
							
							GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
							NetMsgBRD.dwGUID = pEnemyPet->m_dwGUID;
							NetMsgBRD.sTarID = sTargetID;
							pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
						}

					}

					// 맞는자만 Summon Attack 처리
					/*skill summon, Juver, 2017/10/09 */
					for ( int ii=0; ii<SKILL_SUMMON_MAX_CLIENT_NUM; ++ii )
					{
						PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( pCHAR->m_dwSummonGUID_FLD[ii] );
						if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
							pEnemySummon->GetAttackTarget( sTargetID ) && pEnemySummon->IsTargetReady() )
						{
							/*D3DXVECTOR3 vOwnerPos, vDist;
							float fDist;
							vOwnerPos = pCHAR->GetPosition ();
							vDist = pEnemySummon->m_vPos - vOwnerPos;
							fDist = D3DXVec3Length(&vDist);

							if ( fDist <= pEnemySummon->m_fWalkArea )*/
							{
								pEnemySummon->SetAttackTarget( sTargetID );
							}
						}
					}
					

					PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
					if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
					{
						D3DXVECTOR3 vDist;
						float fDist;
						vDist = pMyPet->m_vPos - m_vPos;
						fDist = D3DXVec3Length(&vDist);

						// 일정거리 안에 있으면 
						if ( fDist <= GLCONST_PET::fWalkArea )
						{
							pMyPet->ReSetAllSTATE ();
							pMyPet->SetSTATE ( EM_PETACT_ATTACK );

							GLMSG::SNETPET_ATTACK NetMsg;
							NetMsg.sTarID = m_TargetID;
							GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
							
							GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
							NetMsgBRD.dwGUID = pMyPet->m_dwGUID;
							NetMsgBRD.sTarID = m_TargetID;
							SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
						}
					}
				}
			}

		}

		/*skill validity check, Juver, 2017/11/27 */
		BOOL bCheckPC = FALSE;
		BOOL bCheckPCParty = FALSE;
		BOOL bCheckRange = FALSE;
		if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
		{
			if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_SELF )
			{
				if ( sTARID.emCrow != GETCROW() || sTARID.dwID != m_dwGaeaID )
				{
					CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s target not self [%d~%d]", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), sTARID.emCrow, sTARID.dwID );


					GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
					DropMsg.dwCHARID  = GetCharID();
					DropMsg.bCheat = true;
					DropMsg.nCheatType = 2;
					GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

					/*GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
					//strTextGen.Format( "%s has been permanently banned due to transfer buff cheat attempt.", m_szName );
					strTextGen.Format( ID2SERVERTEXT("GERBERSEC_TRANS_BUFF"),m_szName );
					NetMsgGen.SETTEXT ( strTextGen.GetString() );
					NetMsgGen.bNotify = true;
					NetMsgGen.iColor = 5;
					GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
					strTextGen = "";

					GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);*/
					return E_FAIL;

					//continue;
				}
			}
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_ZONE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_OUR )
				bCheckPCParty = TRUE;
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_ZONE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;
		}
		else if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SPEC )
		{
			if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_SELF && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_OUR )
			{
				bCheckPCParty = TRUE;
				bCheckRange = TRUE;
			}
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_SELF && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;	
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_SELF && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ANYBODY )
			{
				bCheckPC = TRUE;
				bCheckRange = TRUE;
			}
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_ZONE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_OUR )
			{
				bCheckPCParty = TRUE;
				bCheckRange = TRUE;
			}
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_ZONE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_ZONE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ANYBODY )
			{
				bCheckRange = TRUE;
				bCheckPC = TRUE;
			}
			else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_FANWIZE && pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;
		}
		else if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )
		{
			if ( pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;
		}
		else if ( pSkill->m_sBASIC.emIMPACT_SIDE == TAR_ZONE )
		{
			if ( pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_OUR )
			{
				bCheckPCParty = TRUE;
				bCheckRange = TRUE;
			}
			else if ( pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
				bCheckRange = TRUE;
			else if ( pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ANYBODY )
			{
				bCheckPC = TRUE;
				bCheckRange = TRUE;
			}
		}

		if ( bCheckPC )
		{
			if ( sTARID.emCrow != CROW_PC )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s [%s-%s-%s] target not pc [%d~%d]", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
					COMMENT::IMPACT_TAR[pSkill->m_sBASIC.emIMPACT_TAR].c_str(), 
					COMMENT::IMPACT_REALM[pSkill->m_sBASIC.emIMPACT_REALM].c_str(), 
					COMMENT::IMPACT_SIDE[pSkill->m_sBASIC.emIMPACT_SIDE].c_str(), 
					sTARID.emCrow, sTARID.dwID );
				continue;
			}
		}

		if ( bCheckPCParty )
		{	
			if ( sTARID.emCrow != CROW_PC )
			{
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s [%s-%s-%s] target not pc [%d~%d]", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
					COMMENT::IMPACT_TAR[pSkill->m_sBASIC.emIMPACT_TAR].c_str(), 
					COMMENT::IMPACT_REALM[pSkill->m_sBASIC.emIMPACT_REALM].c_str(), 
					COMMENT::IMPACT_SIDE[pSkill->m_sBASIC.emIMPACT_SIDE].c_str(), 
					sTARID.emCrow, sTARID.dwID );
				continue;
			}


			if ( sTARID != STARGETID( GETCROW(), m_dwGaeaID ) )
			{
				GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
				GLPARTY_FIELD* pparty_field = sPartyFieldMan.GetParty ( m_dwPartyID );
				if ( !pparty_field )
				{
					CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s [%s-%s-%s] caster no party [%d~%d]", m_dwCharID, m_szName, 
						pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
						COMMENT::IMPACT_TAR[pSkill->m_sBASIC.emIMPACT_TAR].c_str(), 
						COMMENT::IMPACT_REALM[pSkill->m_sBASIC.emIMPACT_REALM].c_str(), 
						COMMENT::IMPACT_SIDE[pSkill->m_sBASIC.emIMPACT_SIDE].c_str(), 
						sTARID.emCrow, sTARID.dwID );

					/*GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
					DropMsg.dwCHARID  = GetCharID();
					DropMsg.bCheat = true;
					DropMsg.nCheatType = 2;
					GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

					GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
					strTextGen.Format( "%s has been permanently banned due to transfer buff cheat attempt.", m_szName );
					NetMsgGen.SETTEXT ( strTextGen.GetString() );
					NetMsgGen.bNotify = true;
					NetMsgGen.iColor = 5;
					GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
					strTextGen = "";

					GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);
					return E_FAIL;*/

					continue;
				}

				if ( !pparty_field->ISMEMBER( sTARID.dwID ) )
				{
					CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s [%s-%s-%s] target not party [%d~%d]", m_dwCharID, m_szName, 
						pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
						COMMENT::IMPACT_TAR[pSkill->m_sBASIC.emIMPACT_TAR].c_str(), 
						COMMENT::IMPACT_REALM[pSkill->m_sBASIC.emIMPACT_REALM].c_str(), 
						COMMENT::IMPACT_SIDE[pSkill->m_sBASIC.emIMPACT_SIDE].c_str(), 
						sTARID.emCrow, sTARID.dwID );

					/*GLMSG::SNETPC_DROPCHAR_TOAGENT DropMsg;
					DropMsg.dwCHARID  = GetCharID();
					DropMsg.bCheat = true;
					DropMsg.nCheatType = 2;
					GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &DropMsg);

					GLMSG::SNET_SERVER_GENERALCHAT NetMsgGen;
					strTextGen.Format( "%s has been permanently banned due to transfer buff cheat attempt.", m_szName );
					NetMsgGen.SETTEXT ( strTextGen.GetString() );
					NetMsgGen.bNotify = true;
					NetMsgGen.iColor = 5;
					GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsgGen );
					strTextGen = "";

					GLITEMLMT::GetInstance().ReqAction(m_dwCharID,EMLOGACT_CHEATER_BAN,ID_CHAR, 1,0,0,0,0);
					return E_FAIL;*/

					continue;
				}
			}
		}

		if ( bCheckRange )
		{
			if ( sTARID != STARGETID( GETCROW(), m_dwGaeaID ) )
			{
				const SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[pNetMsg->wskill_level];
				float fDetectRange = GETBODYRADIUS() + pTARGET->GetBodyRadius() + GETSKILLRANGE_TAR(*pSkill) + GETSKILLRANGE_APPLY(*pSkill,pNetMsg->wskill_level);
				fDetectRange = fDetectRange += float(wAdditionalRange);
				float fLength = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-pTARGET->GetPosition()) );
				if ( fLength > fDetectRange && (fLength > 375) )
				{
					CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s [%s-%s-%s] target distance [%d~%d] checker:%g distance:%g", m_dwCharID, m_szName, 
						pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName(), 
						COMMENT::IMPACT_TAR[pSkill->m_sBASIC.emIMPACT_TAR].c_str(), 
						COMMENT::IMPACT_REALM[pSkill->m_sBASIC.emIMPACT_REALM].c_str(), 
						COMMENT::IMPACT_SIDE[pSkill->m_sBASIC.emIMPACT_SIDE].c_str(), 
						sTARID.emCrow, sTARID.dwID, fDetectRange, fLength );
					continue;
				}
			}
		}

		SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[0];

		BOOL bexist = FALSE;
		for( int n=0; n<(int)vectargets.size(); ++n )
		{
			if( pSkill->m_sAPPLY.emBASIC_TYPE == SKILL::EMFOR_HP && sSKILL_DATA.fBASIC_VAR < 0.0f ){
				if(  pNetMsg->sTARGET_IDS[i].wCrow == static_cast<WORD>(CROW_PC) && pNetMsg->sTARGET_IDS[i].wID == m_dwGaeaID )
				{
					CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s self-targeting attempt.", m_dwCharID, m_szName, 
						pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName() );
					continue;
				}
			}

			if ( pNetMsg->sTARGET_IDS[i].wCrow == vectargets[n].wCrow &&  
				pNetMsg->sTARGET_IDS[i].wID == vectargets[n].wID )
			{
				bexist = TRUE;
				CDebugSet::ToFileWithTime( "_skill_check.txt", "[%d]%s [%d~%d]%s multi-hit attempt", m_dwCharID, m_szName, 
					pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill->GetName() );
				break;
			}
		}

		if ( !bexist )	vectargets.push_back( pNetMsg->sTARGET_IDS[i] );

		//	Note : 타겟을 공격 가능한지 검사.
		//		( 상대방이 대련시에 자신과 대련이 아닐 경우 공격 불가. )
		//m_sTARIDS[j++] = pNetMsg->sTARIDS[i];
	}


	/*skill validity check, Juver, 2017/11/26 */
	m_wTARNUM = (WORD)vectargets.size();
	for( int n=0; n<EMTARGET_NET; ++n )
	{
		m_sTARIDS[n].wCrow = 0;
		m_sTARIDS[n].wID = USHRT_MAX;
	}

	for( int n=0; n<(int)vectargets.size(); ++n )
	{
		if ( n >= EMTARGET_NET )	continue;
		m_sTARIDS[n] = vectargets[n];
	}


	if ( m_wTARNUM == 0 )
	{
		//	Note : 자신에게.
		GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
		NetMsg.emTarCrow		= m_TargetID.emCrow;
		NetMsg.dwTarID			= m_TargetID.dwID;
		NetMsg.nDamage			= 0;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

		return E_FAIL;
	}

	//	Note : 스킬 구동 조건 검사.
	//
	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[sEXT_DATA.emANIMTYPE][sEXT_DATA.emANISTYPE];
	if ( vecAniAttack.empty() )
	{
		// 캐릭터의 스킬에니메이션이 없습니다.
		DEBUGMSG_WRITE ( _T("Can't find skill animation. char[%d] animation[%d][%d]"),
			             m_CHARINDEX,
						 sEXT_DATA.emANIMTYPE,
						 sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	const SANIATTACK &sAniAttack = vecAniAttack[0];
	WORD wStrikeNum = sAniAttack.m_wDivCount;
	if ( wStrikeNum==0 )
	{
		// 스킬 에니메이션에 타격 지점이 지정되지 않았습니다
		DEBUGMSG_WRITE ( _T("Can't find target position of skill animation. char[%d] animation[%d][%d]"),
			             m_CHARINDEX,
						 sEXT_DATA.emANIMTYPE,
						 sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	EMSKILLCHECK emCHECK = GLCHARLOGIC::CHECHSKILL ( skill_id, 1, pNetMsg->bDefenseSkill );
	if ( emCHECK != EMSKILL_OK && emCHECK != EMSKILL_NOTSP )
	{
		//	Note : 스킬 구동 실패 FB 메시지.
		GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
		NetMsgFB.emSKILL_FB = emCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//	Note : SKILL 구동.
	//
	SETACTIVESKILL ( skill_id );
	SetDefenseSkill( pNetMsg->bDefenseSkill );
	if ( pNetMsg->bDefenseSkill ) m_bDefenseSkill = false;

	m_SKILLMTYPE = sEXT_DATA.emANIMTYPE;
	m_SKILLSTYPE = sEXT_DATA.emANISTYPE;

	BOOL bLowSP =  (emCHECK==EMSKILL_NOTSP) ? TRUE : FALSE;
	PreStrikeProc ( TRUE, bLowSP );

	TurnAction ( GLAT_SKILL );

	// 스킬이 정상적으로 발동되면 딜래이를 초기화
	m_fSkillDelay = 0.0f;
	
	//	Note : 스킬 사용시 소모값들 소모시킴.
	//
	GLCHARLOGIC::ACCOUNTSKILL ( skill_id, 1, true );


	//	Note : SKILL 구동 성공시 - 자신의 주변 Char에게 자신의 Msg를 전달.
	//
	WORD wLevel;
	if ( pNetMsg->bDefenseSkill )	wLevel = m_sDefenseSkill.m_wLevel;
	else wLevel = GETLEARNED_SKILL(pNetMsg->skill_id)->wLevel;

	GLMSG::SNETPC_REQ_SKILL_BRD NetMsgBRD;
	NetMsgBRD.emCrow = GETCROW();
	NetMsgBRD.dwID = m_dwGaeaID; //- 시전자 정보. 
	NetMsgBRD.skill_id = pNetMsg->skill_id;
	NetMsgBRD.wLEVEL = wLevel; //- 시전 스킬 종류.
	NetMsgBRD.vTARPOS = pNetMsg->vTARPOS;
	for ( WORD i=0; i<m_wTARNUM; ++i )		NetMsgBRD.ADDTARGET ( m_sTARIDS[i] );

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );



	//	Note : 스킬 구동 성공 FB 메시지.
	GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
	NetMsgFB.emSKILL_FB = EMSKILL_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//CONSOLEMSG_WRITE( "MsgReqSkill End [%u]", skill_id.dwID );

	// 몹을 공격하면 팻이 공격모션을 취함
	if ( m_TargetID.emCrow==CROW_MOB )
	{
		PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
		if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
		{
			D3DXVECTOR3 vDist;
			float fDist;
			vDist = pMyPet->m_vPos - m_vPos;
			fDist = D3DXVec3Length(&vDist);

			// 일정거리 안에 있으면 
			if ( fDist <= GLCONST_PET::fWalkArea )
			{
				GLMSG::SNETPET_ATTACK NetMsg;
				NetMsg.sTarID = m_TargetID;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
				
				GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
				NetMsgBRD.dwGUID = m_dwPetGUID;
				NetMsgBRD.sTarID = m_TargetID;
				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
			}
		}
	}

	/*skill summon, Juver, 2017/10/11 */
	ReTargetSummon( STARID(m_TargetID.emCrow,m_TargetID.dwID) );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillCancel ( NET_MSG_GENERIC* nmg )
{
	//CONSOLEMSG_WRITE( "MsgReqSkillCancel" );



	//	Note : 액션 취소.
	if ( IsACTION(GLAT_SKILL) )		TurnAction ( GLAT_IDLE );

	//	Note : 자신의 주변 Char에게 자신의 Msg를 전달.
	//
	GLMSG::SNETPC_SKILL_CANCEL_BRD NetMsgBRD;
	NetMsgBRD.dwGaeaID = m_dwGaeaID;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_SET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_SET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 배운 스킬이 아닐 경우 취소됨.
	if ( !ISLEARNED_SKILL(pNetMsg->skill_id) )	return E_FAIL;

	//	Note : 슬롯에 넣어줌.
	m_sSKILLQUICK[pNetMsg->wSLOT] = pNetMsg->skill_id;

	//	Note : 클라이언트에 통보.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQReSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_RESET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_RESET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 슬롯에 넣어줌.
	m_sSKILLQUICK[pNetMsg->wSLOT] = NATIVEID_NULL();

	//	Note : 클라이언트에 통보.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSetActive ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 액티브된 슬롯 스킬을 지정.
	m_wSKILLQUICK_ACT = pNetMsg->wSLOT;

	return S_OK;
}

