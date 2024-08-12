#include "stdafx.h"
#include "s_CAgentServer.h"
#include "../Lib -- Engine/Common/StringUtils.h"
#include "RanFilter.h"
#include "s_CRandomNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CAgentServer::InsertMsg(int nClient, void* pMsg)
{
	if (NULL != pMsg) {
		NET_MSG_GENERIC* pNmg = (NET_MSG_GENERIC*) pMsg;
		m_pRecvMsgManager->MsgQueueInsert(nClient, pNmg, pNmg->dwSize);
	}
	return 0;
}

int CAgentServer::MsgProcess(MSG_LIST* pMsg)
{	
	if (pMsg == NULL) return 0;

	DWORD dwClient=0, dwGaeaID=0;
	NET_MSG_GENERIC* nmg = (NET_MSG_GENERIC*) pMsg->Buffer;
	dwClient = pMsg->dwClient;
	dwGaeaID = m_pClientManager->GetGaeaID ( dwClient );
	
	// 서버간 통신 메시지
	if ( (dwClient < NET_RESERVED_SLOT) && (dwClient >= 0) )
	{
		ServerMsgProcess( pMsg );
		return NET_OK;
	}

	switch (nmg->nType)
	{
	case NET_MSG_HEARTBEAT_CLIENT_ANS:
		{
			NET_HEARTBEAT_CLIENT_ANS* nmg2 = ( NET_HEARTBEAT_CLIENT_ANS* ) nmg;
			TCHAR* szEncKey = m_pClientManager->GetEncKey( pMsg->dwClient );
			if ( _tcslen( szEncKey ) > 0  )
			{				
				TCHAR szEncryptKey[ENCRYPT_KEY+1];
				memcpy( szEncryptKey, nmg2->szEnCrypt, sizeof(TCHAR) * ENCRYPT_KEY+1 );
				
				m_Tea.decrypt (szEncryptKey, ENCRYPT_KEY+1);
				if ( _tcscmp( szEncryptKey, szEncKey ) )
				{
					CConsoleMessage::GetInstance()->Write( _T("Error HeartBit Error %s, %s , UserID : %s, CloseClient( %d )"), 
															   szEncryptKey, szEncKey, m_pClientManager->GetUserID( pMsg->dwClient ), pMsg->dwClient );					
					break;
				}
			}

			m_pClientManager->SetEncKey( pMsg->dwClient, nmg2->szEnCrypt );
			m_pClientManager->SetHeartBeat( pMsg->dwClient );
		}
		break;

	// Client->Agent : 로그인 요청 데이타일때...		
	// Taiwan / Hongkong
	case NET_MSG_LOGIN_2 :					MsgLogIn( pMsg );					break;
	// China
	case CHINA_NET_MSG_LOGIN:				ChinaMsgLogin( pMsg );				break;
	//case CHINA_NET_MSG_REGISTER:			ChinaMsgRegister ( pMsg );			break; //itemmall
	case CHINA_NET_MSG_PIN:					MsgChangePin ( pMsg );			break;
	case CHINA_NET_MSG_PASS:					MsgChangePass ( pMsg );			break;
	case CHINA_NET_MSG_EMAIL:					MsgChangeEmail ( pMsg );			break;
	case CHINA_NET_MSG_TOPUP:					MsgTopUp ( pMsg );			break;
	case CHINA_NET_MSG_RESETPASS:				MsgResetPass ( pMsg ); break;
	case CHINA_NET_MSG_RESETPIN:				MsgResetPin ( pMsg ); break;
	case CHINA_NET_MSG_RESETPASS2:				MsgResetPass2 ( pMsg ); break;
	case CHINA_NET_MSG_GT2VP:					MsgGameTimeCVT ( pMsg );			break;
	case CHINA_NET_MSG_PP2VP:					MsgPremiumPointCVT ( pMsg );			break;
	/*security key on charselect, DevArt22*/
	case CHINA_NET_MSG_CHARSELECT_PASS:			MsgCharSelectPass ( pMsg );			break;

	// Thailand 로그인 요청 
	case THAI_NET_MSG_LOGIN :				ThaiMsgLogin( pMsg );				break;	
	case DAUM_NET_MSG_LOGIN :				DaumMsgLogin( pMsg );				break;
	case TERRA_NET_MSG_LOGIN:				TerraMsgLogin( pMsg );				break;
	case GSP_NET_MSG_LOGIN:					GspMsgLogin( pMsg );				break;
	case EXCITE_NET_MSG_LOGIN:				ExciteMsgLogin( pMsg );				break;
	// 일본 서비스사 이전(Gonzo)에 따른 추가
	case JAPAN_NET_MSG_LOGIN:				JapanMsgLogin( pMsg );				break;
	case GS_NET_MSG_LOGIN:					GsMsgLogin( pMsg );					break;

	// Daum 2차 패스워드 체크
	case DAUM_NET_MSG_PASSCHECK:			DaumMsgPassCheck( pMsg );			break;	
	// 말레이지아 2차 패스워드 체크
	case TERRA_NET_MSG_PASSCHECK:			TerraMsgPassCheck( pMsg );			break;

	// 일본 2차 패스워츠 체크
	case EXCITE_NET_MSG_PASSCHECK:			ExciteMsgPassCheck( pMsg );			break;

	// Client->Agent : 새로운 캐릭터 생성
	case NET_MSG_CHA_NEW :					MsgCreateCharacter( pMsg );			break;
	case NET_MSG_REQ_RAND_KEY:				MsgSndRandomNumber( dwClient );		break;
	// Client->Agent : 사용자 (디스플레이용)  캐릭터 기초정보 요청
	case NET_MSG_REQ_CHA_BAINFO :			MsgSndChaBasicBAInfo (pMsg);		break;
	// Client->Agent : 특정캐릭의 기초정보 요청 (로비 디스플레이용)
	case NET_MSG_REQ_CHA_BINFO : 			MsgSndChaBasicInfo(pMsg);			break;
	// Client->Agent : 캐릭터 삭제
	case NET_MSG_CHA_DEL :					MsgSndChaDelInfo( pMsg );			break;
	case DAUM_NET_MSG_CHA_DEL :				DaumMsgSndChaDelInfo(pMsg);			break;
	case TERRA_NET_MSG_CHA_DEL :			TerraMsgSndChaDelInfo(pMsg);		break;
	case EXCITE_NET_MSG_CHA_DEL :			ExciteMsgSndChaDelInfo(pMsg);		break;
	case GSP_NET_MSG_CHA_DEL:				GspMsgSndChaDelInfo( pMsg );		break;
	case JAPAN_NET_MSG_CHA_DEL:				JapanMsgSndChaDelInfo(pMsg);		break;
	case GS_NET_MSG_CHA_DEL:				GsMsgSndChaDelInfo(pMsg);			break;

	// Client->Agent : 캐릭터 선택후 조인
	case NET_MSG_LOBBY_GAME_JOIN : 			MsgGameJoin(pMsg);					break;
	// DB 스레드->에이젼트서버 : DB에서 읽어진 정보로 케릭터 접속 시도.
	case NET_MSG_AGENT_REQ_JOIN:			MsgAgentReqJoin(pMsg);				break;
	// Field->Agent : 캐릭터가 조인했음 
	case MET_MSG_GAME_JOIN_FIELDSVR_FB :	MsgLobbyCharJoinField(pMsg);		break;		
	// Apex Data 메세지	// Apex 적용
	case NET_MSG_APEX_DATA:					MsgApexData(pMsg);					break;
	// Apex 적용( 홍콩 )
	case NET_MSG_APEX_RETURN:				MsgApexReturn(pMsg);				break;
	
	// GameGuard 인증용 메세지
	case NET_MSG_GAMEGUARD_ANSWER:			MsgGameGuardAnswer(pMsg);			break;
	// GameGuard 인증용 메세지 #1
	case NET_MSG_GAMEGUARD_ANSWER_1:		MsgGameGuardAnswerFirst( pMsg );	break;
	// GameGuard 인증용 메세지 #2
	case NET_MSG_GAMEGUARD_ANSWER_2:		MsgGameGuardAnswerSecond( pMsg );	break;
	// 필드서버와 클라이언트에서 받은 추적 로그를 찍는다.
	case NET_MSG_LOG_UPDATE_TRACING_CHARACTER: MsgTracingLogPrint( pMsg );		break;

	/*register page, Juver, 2017/11/18 */
	case NET_MSG_REGISTER_INIT:				RegisterInit( pMsg );				break;
	case NET_MSG_REGISTER_ACTION:			RegisterAction( pMsg );				break;

	case NET_MSG_GCTRL_ACTSTATE:
	case NET_MSG_CHAT:
	case NET_MSG_GCTRL_REQ_REBIRTH:
	case NET_MSG_GCTRL_REQ_RECALL_AG:
	case NET_MSG_REQ_MUST_LEAVE_MAP_AG:
	case NET_MSG_GCTRL_REQ_TELEPORT_AG:
	case NET_MSG_GCTRL_REGEN_GATE_FB:
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_FB:
	case NET_MSG_GCTRL_REQ_GATEOUT_FB:
	case NET_MSG_GCTRL_REQ_REBIRTH_FB:
	case NET_MSG_GCTRL_REQ_SERVER_CLUB_BATTLE_REMAIN_AG:
	case NET_MSG_GCTRL_REQ_SERVER_CLUB_DEATHMATCH_REMAIN_AG:
	
	case NET_MSG_GCTRL_PARTY_LURE:
	case NET_MSG_GCTRL_PARTY_LURE_TAR_ANS:
	case NET_MSG_GCTRL_PARTY_SECEDE:
	case NET_MSG_GCTRL_PARTY_AUTHORITY:
	case NET_MSG_GCTRL_PARTY_DISSOLVE:

	case NET_MSG_GCTRL_CONFRONT:
	case NET_MSG_GCTRL_CONFRONT_ANS:
	case NET_MSG_GCTRL_CONFRONT_END2_AGT:
	case NET_MSG_GCTRL_CONFRONT_FB:
	
	case NET_MSG_GCTRL_CLUB_NEW_2AGT:
	case NET_MSG_GCTRL_CLUB_NEW_DB2AGT:
	case NET_MSG_GCTRL_CLUB_RANK_2AGT:
	case NET_MSG_GCTRL_CLUB_MEMBER_DB2DEL:
	case NET_MSG_CHARPOS_FROMDB2AGT:

	case NET_MSG_GCTRL_CLUB_DISSOLUTION:
	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_2AGT:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL:
	case NET_MSG_GCTRL_CLUB_MEMBER_SECEDE:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_MARK_INFO:
	case NET_MSG_GCTRL_CLUB_MARK_CHANGE:

	case NET_MSG_GCTRL_CLUB_COMMISSION:
	case NET_MSG_GCTRL_CLUB_NOTICE_REQ:
	case NET_MSG_GCTRL_CLUB_SUBMASTER:

	case NET_MSG_REQ_FRIENDLIST:
	case NET_MSG_REQ_FRIENDADD:
	case NET_MSG_REQ_FRIENDADD_ANS:
	case NET_MSG_REQ_FRIENDDEL:
	case NET_MSG_REQ_FRIENDBLOCK:

	case NET_MSG_GCTRL_2_FRIEND_REQ:

	case NET_MSG_SERVER_CTRL_WEATHER:
	case NET_MSG_SERVER_CTRL_WEATHER2:
	case NET_MSG_SERVER_CTRL_GENITEMHOLD:
	case NET_MSG_SERVER_CTRL_TIME:
	case NET_MSG_SERVER_CTRL_MONTH:


	/////////////////////////////////////////
	// 이벤트 관련 
	case NET_MSG_GM_EVENT_ITEM_GEN:
	case NET_MSG_GM_EVENT_ITEM_GEN_END:

	case NET_MSG_GM_EVENT_MONEY_GEN:
	case NET_MSG_GM_EVENT_MONEY_GEN_END:

	case NET_MSG_GM_EVENT_EXP:				
	case NET_MSG_GM_EVENT_EXP_END:		

	case NET_MSG_GM_EVENT_EX:
	case NET_MSG_GM_EVENT_EX_END:

	case NET_MSG_GM_LIMIT_EVENT_BEGIN:
	case NET_MSG_GM_LIMIT_EVENT_END:
	case NET_MSG_GM_LIMIT_EVENT_RESTART:
	case NET_MSG_GM_LIMIT_EVENT_TIME_REQ:

	case NET_MSG_CYBERCAFECLASS_UPDATE:

	case NET_MSG_GM_VIEWWORKEVENT:

	/////////////////////////////////////////
	case NET_MSG_GM_BIGHEAD:
	case NET_MSG_GM_BIGHAND:
	case NET_MSG_GM_MOVE2GATE:
	case NET_MSG_GM_MOVE2MAPPOS:
	case NET_MSG_GM_SHOWMETHEMONEY:
	case NET_MSG_GM_SET_PRIVATE_MARKET:		/*private market set, Juver, 2018/01/02 */
	case NET_MSG_GM_SET_MEGAPHONE:			/*megaphone set, Juver, 2018/01/02 */
	case NET_MSG_GM_FREEPK:
	case NET_MSG_GM_VIEWALLPLAYER:
	case NET_MSG_GM_GENCHAR:
	case NET_MSG_GM_MOVE2CHAR:
	case NET_MSG_GM_MOB_GEN:
	case NET_MSG_GM_MOB_GEN_EX:
	case NET_MSG_GM_MOB_DEL_EX:
	case NET_MSG_GM_MOB_DEL:
	case NET_MSG_GM_MOB_LEVEL:
	case NET_MSG_GM_MOB_LEVEL_CLEAR:
	case NET_MSG_GM_WHERE_NPC:
	case NET_MSG_GM_PRINT_CROWLIST:

	case NET_MSG_GM_GETWHISPERMSG:
	case NET_MSG_GCTRL_REQ_TAXI:

	case NET_MSG_GM_CHAT_BLOCK_UACCOUNT:
	case NET_MSG_GM_CHAT_BLOCK_CHARNAME:
	case NET_MSG_GM_CHAT_BLOCK_CHARID:
	case NET_MSG_GM_CHAR_INFO_4NAME:
	case NET_MSG_GM_CHAR_INFO_4CHARID:

	case NET_MSG_USER_CHAR_INFO_4NAME:

	case NET_MSG_BLOCK_DETECTED:
	case NET_MSG_GM_KICK_USER:

	case NET_MSG_CHAT_LOUDSPEAKER_AGT:
	case NET_MSG_GCTRL_INVEN_RENAME_AGTBRD:
	case NET_MSG_SMS_PHONE_NUMBER_AGTBRD:

	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_REQ:

	case NET_MSG_GCTRL_CLUB_BATTLE_REQ:
	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ:

	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ANS:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ:

	case NET_MSG_GCTRL_UPDATE_STARTCALL:

	case NET_MSG_SMS_SEND:
	case NET_MSG_SMS_SEND_FROM_DB:

	case NET_MSG_GM_WARNING_MSG:
	case NET_MSG_GCTRL_PUTON_CHANGE_AG:


	case NET_MSG_REQ_FRIEND_CLUB_OPEN:

	case NET_MSG_MARKET_CHECKTIME:
		/*get process command, Juver, 2017/06/08 */
	case NET_MSG_GM_CHAR_GETPROC:
	case NET_MSG_GM_CHAR_GETPROC_UPDATE:
	case NET_MSG_GM_CHAR_GETPROC_UPDATE_COMPLETE:

	case NET_QBOX_OPTION_REQ_AG:
	case NET_MSG_REQ_ATTENDLIST:
	case NET_MSG_REQ_ATTENDANCE:

		/*game stats, Juver, 2017/06/21 */
	case NET_MSG_GCTRL_PING_PACKET:
	
		/*event map move, Juver, 2017/08/25 */
	case NET_MSG_GM_MOVE2_MAPPOS:

	case NET_MSG_DROPCHAR_TOAGENT:
		//fastrevive fix
	case NET_MSG_GCTRL_DEATHTIMER_AG:

		/*pvp tyranny, Juver, 2017/08/25 */
	case NET_MSG_GCTRL_TYRANNY_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_TYRANNY_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_TYRANNY_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_TYRANNY_C2A_TOWER_INFO_REQ:

		/*school wars, Juver, 2018/01/19 */
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_SCORE_INFO_REQ:

		/*pvp capture the flag, Juver, 2018/01/27 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_SCORE_INFO_REQ:
	case NET_MSG_SPEEDHACK_DETECTED:
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING:
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	case NET_MSG_GCTRL_REQ_CHAR_VIEW:
	/////////////////////////////////////////////////////////////////////////////
		{
			//	Note : 만약 (dwMAX_CLIENT) 보다 큰 ID 라면 필드 서버에서 도착하였음.
			//		이 ID를 클라이언트 ID로 변환.
			DWORD dwAgentClient = dwClient;
			DWORD dwMAX_CLIENT = (DWORD) m_pClientManager->GetMaxClient();
			if ( dwMAX_CLIENT <= dwAgentClient )	dwAgentClient -= dwMAX_CLIENT;

			GLAgentServer::GetInstance().MsgProcess ( nmg, dwAgentClient, dwGaeaID );
		}
		break;


	case NET_MSG_GM_WHERE_PC_MAP:			MsgGmwherePcMap ( nmg, dwClient, dwGaeaID );	break;
	case NET_MSG_SERVER_PLAYERKILLING_MODE:	MsgGmPkMode ( nmg, dwClient, dwGaeaID );		break;

	case NET_MSG_VIETNAM_TIME_RESET:		MsgVietTimeReset( nmg, dwClient, dwGaeaID );  break; //vietnamtest%%% && vietnamtest2

	
	case NET_MSG_GCTRL_FIELDSVR_OUT_FB:
		{
			DWORD dwAgentClient = dwClient - m_pClientManager->GetMaxClient();
			MsgFieldSvrOutFb ( nmg, dwAgentClient, dwGaeaID );
		}
		break;

	case NET_MSG_GCTRL_REBIRTH_OUT_FB:
		{
			DWORD dwAgentClient = dwClient - m_pClientManager->GetMaxClient();
			MsgReBirthOutFb ( nmg, dwAgentClient, dwGaeaID );
		}
		break;

	case NET_MSG_GCTRL_REQ_RECALL_FB:
		{
			DWORD dwAgentClient = dwClient - m_pClientManager->GetMaxClient();
			MsgReCallFb ( nmg, dwAgentClient, dwGaeaID );
		}
		break;
	case NET_MSG_GCTRL_REQ_TELEPORT_FB:
		{
			DWORD dwAgentClient = dwClient - m_pClientManager->GetMaxClient();
			MsgTeleportFb ( nmg, dwAgentClient, dwGaeaID );
		}
		break;
	case NET_MSG_GCTRL_INVEN_PREMIUMSET_FB:
		{
			DWORD dwAgentClient = dwClient - m_pClientManager->GetMaxClient();
			MsgPremiumSetFb ( nmg, dwAgentClient, dwGaeaID );
		}
		break;

	default:
		{
			if (nmg->nType==NET_MSG_GCTRL_GOTO )
			{
				BOOL bOk = FALSE;
			}

			if (pMsg->dwClient >= (DWORD) m_nMaxClient)
			{
				// 서버에서 온 메시지 -> 클라이언트로 보내야 한다.
				m_pClientManager->Send(pMsg->dwClient-m_nMaxClient, (LPVOID) pMsg->Buffer);
			}
			else
			{
				// 클라이언트 에서 온 메시지 -> 서버로 보내야 한다.
				m_pClientManager->Send(pMsg->dwClient+m_nMaxClient, (LPVOID) pMsg->Buffer);
			}
		}
		break;
	}
	return 0;
}

void CAgentServer::ServerMsgProcess(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;
	NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *) pMsg->Buffer;
	
	if (nmg == NULL) return;

	//if (dwClient == m_dwSession)
	//{
	//	SessionMsgProcess( pMsg );
	//}
	//else
	//{	
	//	OtherMsgProcess( pMsg );
	//}

	switch ( dwClient )
	{
	case NET_RESERVED_SESSION:
		SessionMsgProcess( pMsg );
		break;	
	default :
		OtherMsgProcess( pMsg );
		break;
	}
}

void CAgentServer::OtherMsgProcess(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;
	
	DWORD dwClient = pMsg->dwClient;
	NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *) pMsg->Buffer;

	if (nmg == NULL) return;

	switch (nmg->nType)
	{
	case NET_MSG_HEARTBEAT_SERVER_ANS:
		m_pClientManager->SetHeartBeat( dwClient );
		break;
	case NET_MSG_GCTRL_REQ_GATEOUT:
	case NET_MSG_GCTRL_REQ_GENITEM_AGT:
	case NET_MSG_FIELDSVR_CHARCHK_FB:
	case NET_MSG_SERVER_GENERALCHAT:
	case NET_MSG_ALLIANCE_BATTLE_BEGIN:
	case NET_MSG_ALLIANCE_BATTLE_BEGIN_REFUSE:
	case NET_MSG_ALLIANCE_BATTLE_OVER_ARMISTICE:
	case NET_MSG_ALLIANCE_BATTLE_OVER_ARMISTICE_RESULT:
	case NET_MSG_ALLIANCE_BATTLE_OVER_DRAW:
	case NET_MSG_ALLIANCE_BATTLE_OVER_SUBMISSION:
	case NET_MSG_ALLIANCE_BATTLE_OVER_WIN:
	case NET_MSG_ALLIANCE_BATTLE_RESULT:
	case NET_MSG_CLUB_BATTLE_BEGIN:
	case NET_MSG_CLUB_BATTLE_BEGIN_REFUSE:
	case NET_MSG_CLUB_BATTLE_OVER_ARMISTICE:
	case NET_MSG_CLUB_BATTLE_OVER_ARMISTICE_RESULT:
	case NET_MSG_CLUB_BATTLE_OVER_DRAW:
	case NET_MSG_CLUB_BATTLE_OVER_SUBMISSION:
	case NET_MSG_CLUB_BATTLE_OVER_WIN:
	case NET_MSG_CLUB_BATTLE_RESULT:
	case NET_MSG_CLUB_DEATHMATCH_END_RANKING:
	case NET_MSG_EMCONFRONT_END_CDRAWN:
	case NET_MSG_EMCONFRONT_END_CWIN:
	case NET_MSG_EMCONFRONT_END_PDRAWN:
	case NET_MSG_EMCONFRONT_END_PWIN:
	case NET_MSG_EMCONFRONT_START_PARTY:
	case NET_MSG_EMCROWACT_KNOCK:
	case NET_MSG_EMCROWACT_REPULSE:
	case NET_MSG_EMGUIDCLUB_CERTIFIED:
	case NET_MSG_BRIGHT_EVENT_MSG:
	case NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_AGT:
	case NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_AGT:
	case NET_MSG_GCTRL_CLUB_CERTIFIED_AGT:
	case NET_MSG_GCTRL_CLUB_INCOME_MONEY_AGT:
	case NET_MSG_GCTRL_CLUB_INCOME_UP:
	//case NET_MSG_GCTRL_CLUB_INCOME_FB:
	case NET_MSG_GCTRL_2_FRIEND_AG:
	case NET_MSG_GM_MOVE2CHAR_AG:
	case NET_MSG_GM_CHAR_INFO_FLD_FB:
	case NET_MSG_USER_CHAR_INFO_FLD_FB:
	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_ING_BRD:
	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_BRD:
	case NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_AGT:
	case NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_AGT:
	case NET_MSG_GCTRL_REQ_TAXI_NPCPOS_FB:
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_REQ:
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_DEL:
	case NET_MSG_GCTRL_NPC_COMMISSION:

		/*pvp tyranny, Juver, 2017/08/25 */
	case NET_MSG_GCTRL_TYRANNY_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_TYRANNY_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_TYRANNY_F2A_TOWER_CAPTURE:
	case NET_MSG_GCTRL_TYRANNY_F2A_TOWER_SCORE:

		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_NOTIFY_AGENT:

		/*school wars, Juver, 2018/01/19 */
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_SCORE_UPDATE:
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_RANKINFO_PC:

		/*pvp capture the flag, Juver, 2018/01/26 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_KILL_UPDATE:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_CAPTURE_UPDATE:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_FLAG_HOLD:
	

	case NET_MSG_GCTRL_REQ_CLOSE_CLIENT_AGT:
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_UPDATE:
	/////////////////////////////////////////////////////////////////////////////
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_UPDATE:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_UPDATE:

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	case NET_MSG_GCTRL_REQ_CHAR_VIEW_FLD:
	/////////////////////////////////////////////////////////////////////////////
		GLAgentServer::GetInstance().MsgProcess( nmg, dwClient, GAEAID_NULL );
		break;

	default :
		break;
	};
}

//void CAgentServer::MsgSndRandomNumber( DWORD dwClient )
//{
//	int nRandomNumber = m_pClientManager->GetRandomPassNumber( dwClient );
//	if ( nRandomNumber <= 0 )
//	{
//		nRandomNumber = SERVER_UTIL::makeRandomNumber( MAX_RANDOM_PASSWORD_NUM );
//		m_pClientManager->SetRandomPassNumber( dwClient, nRandomNumber );
//	}
//	NET_RANDOMPASS_NUMBER tempMsg;
//	tempMsg.nRandomNumber = nRandomNumber;	
//	SendClient(dwClient, &tempMsg);
//}

//userpanel
void CAgentServer::MsgSndRandomNumber( DWORD dwClient )
{
	/*int nRandomNumber = m_pClientManager->GetRandomPassNumber( dwClient );
	if ( nRandomNumber <= 0 )
	{
		nRandomNumber = SERVER_UTIL::makeRandomNumber( MAX_RANDOM_PASSWORD_NUM );
		m_pClientManager->SetRandomPassNumber( dwClient, nRandomNumber );
	}*/
	
	TCHAR* szRandomString = m_pClientManager->GetRandomString(dwClient);
	//CConsoleMessage::GetInstance()->Write( "Generation started" );
		
		SERVER_UTIL::CRandomNumber random;
		CString str;
		if ( random.GenerateRandomString( str, 6 , 1, 1, 2 ) )
		{
			//CConsoleMessage::GetInstance()->Write( "Generation Status : TRUE, random generating..." );
			NET_RANDOMPASS_NUMBER tempMsg;
			StringCchCopy( tempMsg.szRandomString, 7, str  );
			//CConsoleMessage::GetInstance()->Write( "Generated Captcha : %s" , str );
			SendClient(dwClient, &tempMsg);
		}
		else
		{
			//CConsoleMessage::GetInstance()->Write( "Generation Status : FALSE" );
		}
}

void CAgentServer::MsgSndCryptKey(DWORD dwClient)
{
	CRYPT_KEY ck = m_pClientManager->GetNewCryptKey(); // Get crypt key
	
	m_pClientManager->SetCryptKey(dwClient, ck); // Set crypt key
	
	NET_CRYPT_KEY nck;
	nck.ck = ck;
	
	// In Taiwan some user can't receive crypt key
	// Send crypt key 3 times.
	SendClient(dwClient, &nck);
	// SendClient(dwClient, &nck);
	// SendClient(dwClient, &nck);
}

// 게임서버->클라이언트
// 일반적 채팅 메시지 전송
void CAgentServer::MsgSndChatNormal(DWORD dwClient, const char* szName, const char* szMsg)
{
	if ( (szName == NULL) || (szMsg == NULL) ) return;

    NET_CHAT nc;
	nc.nmg.nType = NET_MSG_CHAT;
	nc.emType    = CHAT_TYPE_NORMAL;
	::StringCchCopy(nc.szName, CHR_ID_LENGTH+1, szName);
	::StringCchCopy(nc.szChatMsg, CHAT_MSG_SIZE+1, szMsg);

	SendClient(dwClient, (NET_MSG_GENERIC*) &nc);
}

///////////////////////////////////////////////////////////////////////////////
// 클라이언트->게임서버 : 캐릭터 선택후 조인
void CAgentServer::MsgGameJoin(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;

	// nProtect 서버 인증을 사용한다면 캐릭터가 접속할때 인증이 왔는지 체크한다.	
	if (m_bGameGuardInit == true && 
		m_bGameGuardAuth == true)
	{
		if ( m_pClientManager->nProtectFirstAuthPass( dwClient ) == false )		
		{
			// nProtect 인증 실패
			// 연결을 종료시킨다.
			CloseClient(dwClient);
			return;
		}
		else
		{
			// 현재 nProtect 받기상태라면
			if (m_bHeartBeat == false)
			{
				// 다시 한번 인증쿼리 전송
				NET_GAMEGUARD_AUTH nga;
				// 인증상태를 리셋 시킨다.
				m_pClientManager->nProtectResetAuth(dwClient);
				// 게임가드 인증 쿼리 가져오기 준비
				if (m_pClientManager->nProtectSetAuthQuery(dwClient) == true)
				{
					// 게임가드 인증 쿼리 전송
					nga.ggad = m_pClientManager->nProtectGetAuthQuery(dwClient);
					SendClient(dwClient, &nga);
				}
			}
		}
	}

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (m_pClientManager->IsAccountPass(dwClient) == false)
	{
		CConsoleMessage::GetInstance()->Write(
			                                  _T("INFO:MsgGameJoin IsAccountPass false"));
		CloseClient(dwClient);
		return;
	}

	NET_GAME_JOIN* pNgj = reinterpret_cast<NET_GAME_JOIN*> (pMsg->Buffer);

	if (pNgj == NULL) return;	
	DWORD dwChaNum  = (DWORD) pNgj->nChaNum;
	DWORD dwUserID  = (DWORD) m_pClientManager->GetUserNum(dwClient);
	DWORD dwUserLvl = (DWORD) m_pClientManager->GetUserType(dwClient);
    __time64_t tPREMIUM   = m_pClientManager->GetPremiumDate(dwClient);
    __time64_t tCHATBLOCK = m_pClientManager->GetChatBlockDate(dwClient);
    
	// 캐릭터 번호 세팅
	m_pClientManager->SetChaNum(dwClient, dwChaNum);

	//	Note : db action에 케릭터 정보 가져오기 예약, 이후 케릭터 접속.
	//
	CGetChaInfoAndJoin *pDBAction = new CGetChaInfoAndJoin;
	pDBAction->Init ( dwClient, dwUserID, dwUserLvl, tPREMIUM, tCHATBLOCK, (DWORD)m_nServerGroup, dwChaNum );
	COdbcManager::GetInstance()->AddJob ( pDBAction );
}

void CAgentServer::MsgAgentReqJoin(MSG_LIST* pMsg)
{
	if (pMsg == NULL)			return;
	GLMSG::SNETJOIN2AGENTSERVER* pNgj = reinterpret_cast<GLMSG::SNETJOIN2AGENTSERVER*> (pMsg->Buffer);
	if (pNgj == NULL)			return;

	//	[주의] 이 호출 auto_ptr 구문이 가장 먼저 호출되어야 한다.
	//	이 함수에서 탈출시에 자동으로 제거되게 한다. ( 메시지 처리후 무효화. )
	//
	std::auto_ptr<GLCHARAG_DATA> pCHAR_DATA ( pNgj->m_pCharAgData );
	if ( !pCHAR_DATA.get() )	return;

	//	메시지가 전달된 clientid.
	DWORD dwClientID = pNgj->m_dwClientID;
	if ( !m_pClientManager->IsAccountPass(dwClientID) ) return;

	//	Note : 클라이언트 메니져에 설정된 것과 틀릴 경우 취소됨.
	DWORD dwCharNum = (DWORD) m_pClientManager->GetChaNum(dwClientID);
	if ( dwCharNum!=pCHAR_DATA->m_dwCharID )
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR:dwCharNum!=pCHAR_DATA->m_dwCharID") );
		return;
	}

	// strcpy ( pCHAR_DATA->m_szUserName, m_pClientManager->GetUserID(dwClientID) );
	StringCchCopy(pCHAR_DATA->m_szUserName, USR_ID_LENGTH+1, m_pClientManager->GetUserID(dwClientID));
	strcpy(pCHAR_DATA->m_szIp, m_pClientManager->GetClientIP(dwClientID));

	int nChannel = m_pClientManager->GetChannel(dwClientID);

	bool bPKCHANNEL = CCfg::GetInstance()->IsPkChannel ( nChannel );

	if ( !bPKCHANNEL && (pCHAR_DATA->m_nBright < GLCONST_CHAR::sPK_STATE[0].nPKPOINT) )
	{
		//	접속 시도자에게  메시지를 보냅니다.
		GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
		NetMsgFB.emCharJoinFB = EMCJOIN_FB_PKPOINT;
		m_pClientManager->SendClient( dwClientID, &NetMsgFB );
		return;
	}

	INT dwThaiCCafeClass	= 0;
	// 말레이시아 PC방 이벤트
	INT nMyCCafeClass		= 0;

#if defined(TH_PARAM)
	dwThaiCCafeClass = m_pClientManager->GetThaiClass(dwClientID);
#endif

	// 말레이시아 PC방 이벤트
#if defined(MYE_PARAM) || defined(MY_PARAM)
	nMyCCafeClass = m_pClientManager->GetMyClass(dwClientID);
#endif

	// 중국 누적 시간 셋팅
	SChinaTime sChinaTime;
	sChinaTime.Init();

	// 현재 중국이 아니더라도 이벤트 시간을 위해 현재 로그인한 시간을 구한다.
	// 현재 로그인한 시간
	CTime crtTime			   = CTime::GetCurrentTime();
	sChinaTime.loginTime	   = crtTime.GetTime();

	//////////////////////////////////////////////////////////////////////////
	// 중국 탐닉 방시 시스템 계산
#ifdef CH_PARAM_USEGAIN //chinaTest%%%
	// 적용 대상인지 아닌지
	sChinaTime.userAge   = m_pClientManager->GetChinaUserAge( dwClientID );

	// 게임 플레이 누적 시간
	sChinaTime.gameTime		   = m_pClientManager->GetChinaGameTime(dwClientID);

	// 오프라인 누적 시간
	sChinaTime.offlineTime     = m_pClientManager->GetChinaOfflineTime(dwClientID);
	CTimeSpan offlineSpan( 0, sChinaTime.offlineTime / 60, sChinaTime.offlineTime % 60, 0 );

	// 마지막 로그오프 시간
	sChinaTime.lastOffLineTime = m_pClientManager->GetLastLoginDate(dwClientID);

	// 현재 게임 누적 시간
	sChinaTime.currentGameTime = 0;

	if( sChinaTime.lastOffLineTime != 0 )
	{
		CTime lastOffTime		   = sChinaTime.lastOffLineTime;	
		CTimeSpan totalOffTime = (crtTime - lastOffTime) + offlineSpan;
		CTimeSpan totalGameTime( 0, (int)sChinaTime.gameTime / 60, (int)sChinaTime.gameTime % 60, 0 );
		// 로그오프한 시간이 5시간보다 크거나 같으면 누적시간을 초기화한다.
		if( totalOffTime.GetTotalHours() >= 5 )		
		//if( totalOffTime.GetTotalMinutes() >= 3 )		
		{
			sChinaTime.offlineTime	   = 0;
			sChinaTime.gameTime		   = 0;
			sChinaTime.lastOffLineTime = 0;
			sChinaTime.currentGameTime = 0;


			m_pClientManager->SetChinaOfflineTime(dwClientID, 0 );
			m_pClientManager->SetChinaGameTime(dwClientID, 0 );
		}else{
			if( totalGameTime.GetTotalHours() >= 5 && sChinaTime.userAge != 1 )
			//if( totalGameTime.GetTotalMinutes() >= 1 && sChinaTime.userAge != 1 )
			{
				// 중국 로그인시 플레이 누적시간이 5시간이 넘고 오프라인 누적시간이 5시간이 되지 않았을 경우 에러 메시지를 보낸다.
				GLMSG::SNETLOBBY_CHARJOIN_CHINA_ERROR NetMsg;
				m_pClientManager->SendClient( dwClientID, &NetMsg );
				return;

			}else{
				m_pClientManager->SetChinaOfflineTime(dwClientID, (int)totalOffTime.GetTotalMinutes() );
			}
		}
	}else{
		sChinaTime.offlineTime	   = 0;
		sChinaTime.gameTime		   = 0;
		sChinaTime.lastOffLineTime = 0;
		sChinaTime.currentGameTime = 0;
	}



#endif

	//////////////////////////////////////////////////////////////////////////
	// 베트남 탐닉 방지 시스템 계산
	SVietnamGainSystem sVietnamTime;
	sVietnamTime.Init();
#if defined(VN_PARAM) //vietnamtest%%%
	// 현재 로그인한 시간
	crtTime					   = CTime::GetCurrentTime();
	sVietnamTime.loginTime	   = crtTime.GetTime();

	// 게임 플레이 누적 시간
	sVietnamTime.gameTime		 = m_pClientManager->GetVTGameTime(dwClientID);

	// 마지막 로그오프 시간
	sVietnamTime.lastOffLineTime = m_pClientManager->GetLastLoginDate(dwClientID);

	// 현재 게임 누적 시간
	sVietnamTime.currentGameTime = 0;

	if( sVietnamTime.lastOffLineTime != 0 )
	{
		CTime lastOffTime		   = sVietnamTime.lastOffLineTime;	
		// 오프라인한 날짜가 다르면 초기화 한다.
		bool bInitialize = FALSE; // 테스트로 무조건 초기화 한다.

		// 년도, 달, 날짜가 다르면 초기화 한다.
		if( lastOffTime.GetYear() != crtTime.GetYear() ||
			lastOffTime.GetMonth() != crtTime.GetMonth() ||
			lastOffTime.GetDay() != crtTime.GetDay()  )
		{
//			CConsoleMessage::GetInstance()->Write( _T("INFO:Vietnam Accumulation Time Init Last Off Time Month %d, Day %d CharID[%s], CharAccount[%s]" ), 
//												   lastOffTime.GetMonth(), lastOffTime.GetDay(), pCHAR_DATA->m_szName, pCHAR_DATA->m_szUserName  );
			bInitialize = TRUE;
		}

		if( sVietnamTime.gameTime < 0 )
		{
//			CConsoleMessage::GetInstance()->Write( _T("INFO:Vietnam GameTime not correct value...") );
			bInitialize = TRUE;
		}
			
		if( bInitialize )
		{
			sVietnamTime.gameTime		 = 0;
			sVietnamTime.lastOffLineTime = 0;
			sVietnamTime.currentGameTime = 0;

			m_pClientManager->SetVTGameTime(dwClientID, 0 );

		}
	}else{
		sVietnamTime.gameTime		 = 0;
		sVietnamTime.lastOffLineTime = 0;
		sVietnamTime.currentGameTime = 0;

		m_pClientManager->SetVTGameTime(dwClientID, 0 );
	}


	pCHAR_DATA->m_sVietnamSystem = sVietnamTime;

#endif

	if( CheckTracingUser( pCHAR_DATA->m_szUserName, pCHAR_DATA->m_dwUserID ) != -1 )
	{
		pCHAR_DATA->m_bTracingUser   = TRUE;
	}


	//*/*/*/*
	//	캐릭터 생성
	PGLCHARAG pChar = GLAgentServer::GetInstance().CreatePC( pCHAR_DATA.get(), dwClientID, nChannel, dwThaiCCafeClass, sChinaTime, nMyCCafeClass );
	if ( !pChar )
	{
		//	접속 시도자에게  메시지를 보냅니다.
		GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
		NetMsgFB.emCharJoinFB = EMCJOIN_FB_ERROR;
		m_pClientManager->SendClient ( dwClientID, &NetMsgFB );

		CConsoleMessage::GetInstance()->Write( _T("ERROR:Initialize character instance failed") );
		CConsoleMessage::GetInstance()->Write( _T("ClientID[%u], Channel[%d]"), dwClientID, nChannel );
		CConsoleMessage::GetInstance()->Write( _T("CharIP[%s], CharIDNum[%u], UserIDNum[%u], CharID[%s], CharAccount[%s]"),
			pCHAR_DATA->m_szIp,
			pCHAR_DATA->m_dwCharID,
			pCHAR_DATA->m_dwUserID,
			pCHAR_DATA->m_szName,
			pCHAR_DATA->m_szUserName );
		return;
	}
	

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if( pCHAR_DATA->m_bTracingUser )
	{
		CDebugSet::ToTracingFile( pCHAR_DATA->m_szUserName, "Character Select!, [%s][%s], IP:[%s], Channel:[%d], UserNum:[%u], CharNum:[%u]",
								  pCHAR_DATA->m_szUserName, pCHAR_DATA->m_szName, pCHAR_DATA->m_szIp, pChar->m_nChannel, pCHAR_DATA->m_dwUserID, pCHAR_DATA->m_dwCharID );
	}
#endif


	//	가이아서버에 정보 세팅
	m_pClientManager->SetGaeaID( dwClientID, pChar->m_dwGaeaID );
	
	// 캐릭터 상태 온라인
	CSetCharacterOnline* pDBAction = new CSetCharacterOnline(
											m_pClientManager->GetChaNum(dwClientID) );
	COdbcManager::GetInstance()->AddJob(pDBAction);	
	
	// User DB 에 플레이 중인 캐릭터 이름을 세팅한다.
	if( pChar != NULL )
	{	
		CUserUpdateCha* pAction2 = new CUserUpdateCha(
										m_pClientManager->GetUserNum( dwClientID),
										pChar->m_szName );
										
		COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction2 );
	}
	///////////////////////////////////////////////////////////////////////////
	// 세션서버로 캐릭터정보 전송
	/*
	NET_GAME_JOIN_OK ngjo;
	ngjo.nmg.dwSize	= sizeof(NET_GAME_JOIN_OK);
	ngjo.nmg.nType	= NET_MSG_GAME_JOIN_OK;
	ngjo.nUsrNum	= pChar->m_dwUserID;
	ngjo.nChaNum	= pChar->m_dwCharID;
	ngjo.dwGaeaID	= pChar->m_dwGaeaID;
	::StringCchCopyN ( ngjo.szUserID, USR_ID_LENGTH, m_pClientManager->GetUserID(dwClientID), USR_ID_LENGTH);
	::StringCchCopyN ( ngjo.szChaName, CHR_ID_LENGTH, pCHAR_DATA->m_szName, CHR_ID_LENGTH);

	// 세션서버로 전송
	SendSession((LPVOID) &ngjo);
	*/
}


///////////////////////////////////////////////////////////////////////////////
// Field->Agent : 캐릭터가 게임에 조인했음
int CAgentServer::MsgLobbyCharJoinField(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return NET_ERROR;

	DWORD dwClient = pMsg->dwClient - (DWORD) m_nMaxClient;
	NET_GAME_JOIN_FIELDSVR_FB* pMsgJoinFB = (NET_GAME_JOIN_FIELDSVR_FB*) pMsg->Buffer;

	if (pMsgJoinFB == NULL) return NET_ERROR;

	int nChannel = m_pClientManager->GetChannel(dwClient);
    
	F_SERVER_INFO* pField = CCfg::GetInstance()->GetFieldServer ( (int) pMsgJoinFB->dwFieldSVR, nChannel );
	
	// Check GaeaID
	if ( pMsgJoinFB->dwGaeaID != m_pClientManager->GetGaeaID(dwClient) )		return NET_ERROR;
	m_pClientManager->SetSlotFieldAgent ( dwClient, pMsgJoinFB->dwSlotFieldAgent );
	
	NET_CONNECT_CLIENT_TO_FIELD MsgConnect;
	MsgConnect.dwGaeaID = m_pClientManager->GetGaeaID(dwClient);
	MsgConnect.dwSlotFieldAgent = m_pClientManager->GetSlotFieldAgent(dwClient);
	MsgConnect.emType = pMsgJoinFB->emType;

	::StringCchCopy( MsgConnect.szServerIP, MAX_IP_LENGTH+1, pField->szServerIP );
	MsgConnect.nServicePort = pField->nServicePort;

    // m_pClientManager->GetChaNum(dwClient);

	return SendClient ( dwClient, &MsgConnect );
}

///////////////////////////////////////////////////////////////////////////////
// Agent->Client
// 클라이언트가 캐릭터 선택 화면에서 디스플레이 해야할
// 캐릭터의 고유번호를 전송한다.
void CAgentServer::MsgSndChaBasicBAInfo(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	DWORD dwSndBytes   = 0;	
	int nUserNum       = 0;
	int nSvrGrp        = 0;
	int nServerChannel = 0;
//	int nChannel;
	DWORD dwClient     = pMsg->dwClient;

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (m_pClientManager->IsAccountPass(dwClient) == false)
	{
		CConsoleMessage::GetInstance()->Write(
			                                  _T("INFO:MsgSndChaBasicBAInfo IsAccountPass false"));
		CloseClient(dwClient);
		return;
	}

	NET_CHA_REQ_BA_INFO* pNcrbi = reinterpret_cast<NET_CHA_REQ_BA_INFO*> (pMsg->Buffer);

	/*
	nChannel = pNcrbi->nChannel;

	// 채널 번호가 올바른지 검사한다.
	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER)
	{
		CConsoleMessage::GetInstance()->Write(
			                                  _T("ERROR:MsgSndChaBasicBAInfo Invalid Channel Number (Channel:%d)"),
			                                  nChannel);
		return;
	}

	// 해당 채널을 조사해서 full 인지 검사해야 한다.
	if (IsChannelFull(nChannel) == true)
	{
		// 해당 채널이 full 이면 연결을 끊어야 한다.
		// Full 일 경우 처리한다.		
		CloseClient(dwClient);
		CConsoleMessage::GetInstance()->Write(
											  _T("CH(%d) is FULL"),
			                                  nChannel);
		return;
	}
	else
	{
		// Full 이 아닐경우 해당 채널 유저수를 1 증가시킨다.
		IncreaseChannelUser(nChannel);
		// 클라이언트에 채널번호를 세팅한다.
		m_pClientManager->SetChannel(dwClient,nChannel);                
	}
	*/

	nUserNum = m_pClientManager->GetUserNum(dwClient);
	if (nUserNum < 1) // 치명적 에러, 허가되지 않은 사용자 번호
	{
		CConsoleMessage::GetInstance()->Write(_T("ERROR:MsgSndChaBasicBAInfo nUserNum(%d)"),
			                                  nUserNum);
		return;
	}

	nSvrGrp = m_nServerGroup;

	CGetChaBAInfo* pAction = new CGetChaBAInfo(nUserNum, 
											   nSvrGrp, 
											   dwClient, 
											   m_pClientManager->GetClientIP(dwClient),		
											   m_pClientManager->GetClientPort(dwClient));
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);	
}

void CAgentServer::MsgSndChaBasicBAInfoBack(NET_CHA_BBA_INFO* pStruct, DWORD dwClient, const char* szUserIP, USHORT uPort)
{
	// IP/PORT 가 동일할때만 처리
	if ( (strcmp(szUserIP, m_pClientManager->GetClientIP(dwClient)) == 0) &&
		 (m_pClientManager->GetClientPort(dwClient) == uPort) )
	{
		SendClient(dwClient, pStruct);
	}
}

void CAgentServer::MsgSndChaBasicInfo(DWORD dwClient, int nChaNum)
{
	// 해킹을 통해서 nChaNum 을 0 으로 만들수 있음	
	if (nChaNum <= 0) 
	{
		return;
	}

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (m_pClientManager->IsAccountPass(dwClient) == false)
	{
		CConsoleMessage::GetInstance()->Write(
			                                  _T("INFO:MsgSndChaBasicInfo IsAccountPass false"));
		CloseClient(dwClient);
		return;
	}

	int nUserNum = m_pClientManager->GetUserNum(dwClient);

	CGetChaBInfo* pAction = new CGetChaBInfo(nUserNum,
		                                     nChaNum, 
											 dwClient, 
											 m_pClientManager->GetClientIP(dwClient),		
											 m_pClientManager->GetClientPort(dwClient));
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
}

void CAgentServer::MsgSndChaBasicInfoBack(GLMSG::SNETLOBBY_CHARINFO* snci, DWORD dwClient, const char* szUserIP, USHORT uPort)
{
	EMCHARCLASS emCharClass = snci->Data.m_emClass;

	if( emCharClass == GLCC_EXTREME_M || emCharClass == GLCC_EXTREME_W )
	{
		snci->Data.m_sExperience.lnMax = GLOGICEX::GLNEEDEXP2( snci->Data.m_wLevel );
	}
	else
	{
		snci->Data.m_sExperience.lnMax = GLOGICEX::GLNEEDEXP( snci->Data.m_wLevel );
	}

	SendClient(dwClient, snci);
}

void CAgentServer::MsgSndChaBasicInfo(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	DWORD dwSndBytes = 0;
	DWORD dwClient = pMsg->dwClient;
	INT nChaNum = 0;
	INT nUsrNum = 0;
	INT nSvrGrp = 0;

	NET_CHA_BA_INFO* ncbi;
	ncbi = (NET_CHA_BA_INFO*) pMsg->Buffer;

	if (ncbi == NULL) return;

	nChaNum = ncbi->nChaNum;

	MsgSndChaBasicInfo(dwClient, nChaNum);
}

void CAgentServer::MsgSndChatGlobal(char* szChatMsg)
{
	if (szChatMsg == NULL) return;

	DWORD dwSndBytes = 0;
	CConsoleMessage::GetInstance()->WriteConsole( szChatMsg );
	NET_CHAT_CTRL_FB ncf;
	ncf.emType = CHAT_TYPE_GLOBAL;
	::StringCchCopy(ncf.szChatMsg, GLOBAL_CHAT_MSG_SIZE+1, szChatMsg);
	SendAllClient(&ncf);
}

void CAgentServer::MsgVersion(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_CLIENT_VERSION		ncv;
	DWORD dwClient			= pMsg->dwClient;
	ncv.nmg.nType			= NET_MSG_VERSION_INFO;
	ncv.nGameProgramVer		= m_nVersion;
	ncv.nPatchProgramVer	= m_nPatchVersion;	
	SendClient(dwClient, &ncv);
}

// 새로운 캐릭터를 생성한다.
void CAgentServer::MsgCreateCharacter( MSG_LIST* pMsg )
{
	if (pMsg == NULL) return;

	DWORD			dwClient		= 0;
	NET_NEW_CHA*	nnc				= NULL;
	NET_NEW_CHA_FB	nncfd;
	DWORD			dwSndBytes		= 0;
	int				nUesrNum		= 0;
	int				nIndex			= 0;
	int				nExtremeM		= 0; // 극강부 남자 생성 갯수
	int				nExtremeW		= 0; // 극강부 여자 생성 갯수
	int				nExtremeCheck	= 0; // 극강부 생성 가능 체크변수
	int				nSvrGrp			= 0;
	int				nChaNum			= 0;
    WORD            wChaRemain		= 0;

	dwClient = pMsg->dwClient;
	
	// 인증을 통과하지 못함	
	if (m_pClientManager->IsAccountPass(dwClient) == false) 
	{
		CloseClient(dwClient);
		return;
	}

    if (m_bTestServer) // 테스트 서버라면
	{
        nncfd.wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
    }
    else
    {
        nncfd.wChaRemain = m_pClientManager->GetChaRemain(dwClient);
    }

	nncfd.nmg.nType = NET_MSG_CHA_NEW_FB;

	// 캐릭터 갯수 제한, 만들 수 있는 캐릭터 없음
	if ( (nncfd.wChaRemain <= 0) || (nncfd.wChaRemain > MAX_CHAR_LENGTH) )
	{
		nncfd.nResult  = EM_CHA_NEW_FB_SUB_MAX;
		nncfd.nChaNum  = 0;
		SendClient(dwClient, &nncfd);
		return;
	}
	
	nnc = reinterpret_cast<NET_NEW_CHA*> (pMsg->Buffer);

	// 캐릭터 인덱스
	nIndex = nnc->nIndex;
	
	// 캐릭터 인덱스 오류, 캐릭터 만들 수 없음.
	if (nIndex < 0 || nIndex >= GLCI_NUM_8CLASS )	
	{
		nChaNum = -1; // 캐릭터 에러
		nncfd.nResult = EM_CHA_NEW_FB_SUB_LENGTH; // 글자수 제한
		SendClient(dwClient, &nncfd);
		return;
	}	

	// 캐릭터명
	CString strChaName(nnc->szChaName);
	strChaName.Trim(_T(" ")); // 앞뒤 공백제거

	// 스페이스 찾기, 캐릭터이름 4글자 이하 에러, 캐릭터 만들 수 없음.
	if ((strChaName.FindOneOf(" ") != -1) || (strChaName.GetLength() < 4)) 
	{
		nChaNum = -1; // 캐릭터이름 에러
		nncfd.nResult = EM_CHA_NEW_FB_SUB_LENGTH; // 글자수 제한
		SendClient(dwClient, &nncfd);
		return;
	}

	BOOL bFILTER0 = STRUTIL::CheckString( strChaName );
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter( strChaName );
	if ( bFILTER0 || bFILTER1 )
	{
		nChaNum = -1; // 캐릭터이름 에러
		nncfd.nResult = EM_CHA_NEW_FB_SUB_BAD_NAME; // 글자수 제한
		SendClient(dwClient, &nncfd);
		return;
	}

	if ( nnc->fScaleRange < SCALERANGE_MIN || nnc->fScaleRange > SCALERANGE_MAX )
	{
		nChaNum = -1;
		nncfd.nResult = EM_CHA_NEW_FB_SUB_BAD_SCALE;
		SendClient(dwClient, &nncfd);
		return;
	}

#ifdef TH_PARAM	
	// 태국어 문자 조합 체크 
	if ( !m_pCheckString ) return;

	if ( !m_pCheckString(strChaName) )
	{
		nChaNum = -1; // 캐릭터이름 에러
		nncfd.nResult = EM_CHA_NEW_FB_SUB_THAICHAR_ERROR; // 글자수 제한
		SendClient(dwClient, &nncfd);
		return;
	}
#endif

#ifdef VN_PARAM
	// 베트남 문자 조합 체크 
	if( STRUTIL::CheckVietnamString( strChaName ) )
	{
		nChaNum = -1; // 캐릭터이름 에러
		nncfd.nResult = EM_CHA_NEW_FB_SUB_VNCHAR_ERROR; // 글자수 제한
		SendClient(dwClient, &nncfd);
		return;
	}

#endif 



	// 극강부 생성 가능 체크
	if( GLCI_EXTREME_M == nIndex )
	{
		// 서버에 저장된 극강부 생성 체크 변수 받아오기	
		nExtremeM = m_pClientManager->GetExtremeM( dwClient );

		// 극강부 남자 생성을 불가할 경우, nnc->wSex(여자) = 0
		if( 0 >= nExtremeM )
		{
			nncfd.nResult = EM_CHA_NEW_FB_SUB_MAX;
			SendClient(dwClient, &nncfd);
			return;
		}

		// 극강부 생성 가능을 설정한다.
		nExtremeCheck = 1;
		m_pClientManager->SetExtremeCheckM( dwClient, nExtremeCheck );		
	}
	else if( GLCI_EXTREME_W == nIndex )
	{
		nExtremeW = m_pClientManager->GetExtremeW( dwClient );

		// 극강부 여자 생성이 불가할 경우, nnc->wSex(남자) = 1
		if( 0 >= nExtremeW )
		{
			nncfd.nResult = EM_CHA_NEW_FB_SUB_MAX;
			SendClient(dwClient, &nncfd);
			return;
		}

		// 극강부 생성 가능을 설정한다.
		nExtremeCheck = 1;
		m_pClientManager->SetExtremeCheckW( dwClient, nExtremeCheck );
	}

	//jdevtodo
	//enable later when new classes are ready
	if ( nIndex == GLCI_ASSASSIN_M || nIndex == GLCI_ASSASSIN_W ||
		nIndex == GLCI_TRICKER_M || nIndex == GLCI_TRICKER_M )
	{
		nncfd.nResult = EM_CHA_NEW_FB_SUB_MAX;
		SendClient(dwClient, &nncfd);
		return;
	}


	if (m_bTestServer) // 테스트 서버라면
	{
		m_pClientManager->SetChaTestRemain(dwClient, nncfd.wChaRemain-1);
	}
	else // 정식 서버 라면...
	{
		m_pClientManager->SetChaRemain(dwClient, nncfd.wChaRemain-1);
	}

	CCreateNewCharacter* pAction = new CCreateNewCharacter(
											nIndex, 
                                            m_pClientManager->GetUserNum(dwClient),
                                            m_nServerGroup,
                                            strChaName,
                                            nnc->wSchool,
                                            nnc->wHair,
                                            nnc->wFace,
											nnc->wHairColor,
											nnc->wSex,
											nnc->fScaleRange,
                                            dwClient,
                                            m_pClientManager->GetClientIP(dwClient),
                                            m_pClientManager->GetClientPort(dwClient) );
    COdbcManager::GetInstance()->AddJob( (CDbAction*) pAction );
}

void CAgentServer::MsgCreateCharacterBack(
	int nChaNum,
    DWORD dwClient,
    DWORD dwUserNum,
    const char* szUserIP,
    USHORT uPort )
{	
    NET_NEW_CHA_FB	nncfd;
    
    nncfd.nmg.nType = NET_MSG_CHA_NEW_FB;
	nncfd.nChaNum   = 0;

	WORD wChaRemain		= 0;
	int	 nExtremeM		= 0;
	int  nExtremeW		= 0;
	int	 nExtremeDBM	= 0;
	int  nExtremeDBW	= 0;
	int  nExtremeCheckM = 0;
	int  nExtremeCheckW = 0;
	
	if (nChaNum > 0) // 캐릭터 생성 성공
	{
		nncfd.nChaNum = nChaNum;
		nncfd.nResult = EM_CHA_NEW_FB_SUB_OK;

		// 로직상의 극강부 남/여 생성 가능 숫자를 받아온다.
		nExtremeM = m_pClientManager->GetExtremeM( dwClient );
		nExtremeW = m_pClientManager->GetExtremeW( dwClient );

		// DB연산상의 극강부 남/여 생성 가능 숫자를 받아온다.
		nExtremeDBM = m_pClientManager->GetExtremeDBM( dwClient );
		nExtremeDBW = m_pClientManager->GetExtremeDBW( dwClient );
		
		// 극강부 생성 가능을 가져온다.
		nExtremeCheckM = m_pClientManager->GetExtremeCheckM( dwClient );
		nExtremeCheckW = m_pClientManager->GetExtremeCheckW( dwClient );

		// 극강부 남/여 일 경우 1을 뺀값을 설정한다.
		if( EXTREME_OK == nExtremeCheckM )
		{			
			if(nExtremeDBW < 0)
			{
				nExtremeDBW += 1;
				nExtremeDBM -= 2;

				nExtremeM = nExtremeDBM;
				m_pClientManager->SetExtremeCreateDBM( dwClient, nExtremeDBM );
				m_pClientManager->SetExtremeCreateDBW( dwClient, nExtremeDBW );
				m_pClientManager->SetExtremeCreateM( dwClient, nExtremeM );
			}
			else
			{
				nExtremeM -= 1;
				nExtremeDBM -= 1;
				m_pClientManager->SetExtremeCreateM( dwClient, nExtremeM );				
				m_pClientManager->SetExtremeCreateDBM( dwClient, nExtremeDBM );
			}
			nExtremeCheckM = 0;
			m_pClientManager->SetExtremeCheckM( dwClient, nExtremeCheckM );
		}
		else if( EXTREME_OK == nExtremeCheckW )
		{
			if( nExtremeDBM < 0 )
			{
				nExtremeDBM += 1;
				nExtremeDBW -= 2;

				nExtremeW = nExtremeDBW;
				m_pClientManager->SetExtremeCreateDBM( dwClient, nExtremeDBM );
				m_pClientManager->SetExtremeCreateDBW( dwClient, nExtremeDBW );
				m_pClientManager->SetExtremeCreateW( dwClient, nExtremeW );
			}
			else
			{
				nExtremeW -= 1;
				nExtremeDBW -= 1;
				m_pClientManager->SetExtremeCreateW( dwClient, nExtremeW );				
				m_pClientManager->SetExtremeCreateDBW( dwClient, nExtremeDBW );
			}
			
			nExtremeCheckW = 0;
			m_pClientManager->SetExtremeCheckW( dwClient, nExtremeCheckW );
		}

		// 극강부 남/여 생성 가능 숫자를 넘겨준다.
		nncfd.nExtremeM = m_pClientManager->GetExtremeM( dwClient );
		nncfd.nExtremeW = m_pClientManager->GetExtremeW( dwClient );

		if (m_bTestServer) // 테스트 서버라면
		{
            wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
            if (wChaRemain < 0 || wChaRemain > MAX_CHAR_LENGTH) 
                wChaRemain = 0;
            //m_pClientManager->SetChaTestRemain(dwClient, wChaRemain);
            nncfd.wChaRemain = wChaRemain;

			COdbcManager::GetInstance()->UserTestChaNumDecrease(dwUserNum);
		}
		else // 정식 서버 라면...
		{
            wChaRemain = m_pClientManager->GetChaRemain(dwClient);
            if (wChaRemain < 0 || wChaRemain > MAX_CHAR_LENGTH) 
                wChaRemain = 0;
            //m_pClientManager->SetChaRemain(dwClient, wChaRemain);
            nncfd.wChaRemain = wChaRemain;

			COdbcManager::GetInstance()->UserChaNumDecrease(dwUserNum);
		}
	}
	else
	{
		if (m_bTestServer) // 테스트 서버라면
		{
			wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
			m_pClientManager->SetChaTestRemain(dwClient, wChaRemain+1);
		}
		else // 정식 서버 라면...
		{
			wChaRemain = m_pClientManager->GetChaRemain(dwClient);
			m_pClientManager->SetChaRemain(dwClient, wChaRemain+1);
		}

		if (nChaNum == DB_ERROR) // 캐릭터 생성(DB) 에러발생...
		{    
			if (m_bTestServer) wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
			else               wChaRemain = m_pClientManager->GetChaRemain(dwClient);
			nncfd.wChaRemain = wChaRemain;
			nncfd.nResult    = EM_CHA_NEW_FB_SUB_DUP;
		}
		else if (nChaNum == DB_CHA_MAX) // 만들수 있는 캐릭터 숫자 초과
		{
			if (m_bTestServer) wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
			else               wChaRemain = m_pClientManager->GetChaRemain(dwClient);
			nncfd.wChaRemain = wChaRemain;
			nncfd.nResult  = EM_CHA_NEW_FB_SUB_MAX;
		}
		else if (nChaNum == DB_CHA_DUF) // 같은 이름을 가진 캐릭터 존재
		{
			if (m_bTestServer) wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
			else               wChaRemain = m_pClientManager->GetChaRemain(dwClient);
			nncfd.wChaRemain = wChaRemain;
			nncfd.nResult  = EM_CHA_NEW_FB_SUB_DUP;
		}
		else // 알수 없는 에러
		{
			if (m_bTestServer) wChaRemain = m_pClientManager->GetChaTestRemain(dwClient);
			else               wChaRemain = m_pClientManager->GetChaRemain(dwClient);
			nncfd.wChaRemain = wChaRemain;
			nncfd.nResult  = EM_CHA_NEW_FB_SUB_ERROR;
		}
	}

     // IP/PORT 가 동일할때만 처리
	if ((strcmp(szUserIP, m_pClientManager->GetClientIP(dwClient)) == 0) &&
		(m_pClientManager->GetClientPort(dwClient) == uPort))
	{
	    SendClient(dwClient, &nncfd);
	    if (nChaNum > 0)
	    {
		    MsgSndChaBasicInfo(dwClient, nChaNum);
	    }
    }
}

void CAgentServer::MsgSndLoginInfoToField(DWORD dwClient)
{
	NET_LOGIN_FEEDBACK_DATA3 nlfd3;
	nlfd3.nmg.nType = NET_MSG_LOGIN_FB_SUB_OK_FIELD;
	nlfd3.nUserNum  = m_pClientManager->GetUserNum(dwClient);	
	::StringCchCopy(nlfd3.szUserid, 
		            USR_ID_LENGTH+1, 
		            m_pClientManager->GetUserID(dwClient));
	m_pClientManager->SendField(dwClient+m_nMaxClient, (LPVOID) &nlfd3);
	CConsoleMessage::GetInstance()->Write(_T("INFO:MsgSndLoginInfoToField %d, %s"),
										  nlfd3.nUserNum,
										  nlfd3.szUserid);
}

void CAgentServer::MsgFieldSvrOutFb	(NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	if (nmg == NULL) return;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaID );
	if ( !pChar )										return;
	if ( pChar->m_dwNextFieldSvr == FIELDSERVER_MAX )	return;

	//CConsoleMessage::GetInstance()->Write(
	//	_T("MsgFieldSvrOutFb ClientID[%u], GaeaID[%u]"), dwClientID, dwGaeaID );

	//m_pClientManager->SetConnectionFieldInfo(	dwClientID, 
	//											dwGaeaID, 
	//											nmg );

	//	종전 필드서버와의 연결을 끊는다.
	m_pClientManager->CloseField ( dwClientID );

	//	새로운 필드 서버와 연결을 한다.
	if ( ConnectFieldSvr ( dwClientID, pChar->m_dwNextFieldSvr, dwGaeaID, pChar->m_nChannel ) == NET_OK )
	{
		//	에이젼트 서버로 메시지 전달.
		GLAgentServer::GetInstance().MsgProcess ( nmg, dwClientID, dwGaeaID );
		pChar->ResetNextFieldSvr (); //	연결할 필드서버 정보를 리셋.
	}
}

void CAgentServer::MsgReBirthOutFb	(NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	if (nmg == NULL) return;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaID );
	if ( !pChar )										return;
	if ( pChar->m_dwNextFieldSvr == FIELDSERVER_MAX )	return;

	//CConsoleMessage::GetInstance()->Write(
	//	_T("MsgReBirthOutFb ClientID[%u], GaeaID[%u]"), dwClientID, dwGaeaID );

	//m_pClientManager->SetConnectionFieldInfo(	dwClientID, 
	//											dwGaeaID, 
	//											nmg );

	//	종전 필드서버와의 연결을 끊는다.
	m_pClientManager->CloseField ( dwClientID );

	//	새로운 필드 서버와 연결을 한다.
	if ( ConnectFieldSvr ( dwClientID, pChar->m_dwNextFieldSvr, dwGaeaID, pChar->m_nChannel ) == NET_OK )
	{
		//	에이젼트 서버로 메시지 전달.
		GLAgentServer::GetInstance().MsgProcess ( nmg, dwClientID, dwGaeaID );
		pChar->ResetNextFieldSvr (); //	연결할 필드서버 정보를 리셋.
	}
}

void CAgentServer::MsgReCallFb ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_REQ_RECALL_FB *pNetMsg = (GLMSG::SNETPC_REQ_RECALL_FB *) nmg;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaID );
	if ( !pChar )										return;

	pChar->SetCurrentField ( pChar->m_dwCurFieldSvr, pNetMsg->sMAPID );

	SendClient ( pChar->m_dwClientID, nmg );
}

void CAgentServer::MsgTeleportFb ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_REQ_TELEPORT_FB *pNetMsg = (GLMSG::SNETPC_REQ_TELEPORT_FB *) nmg;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaID );
	if ( !pChar )										return;

	pChar->SetCurrentField ( pChar->m_dwCurFieldSvr, pNetMsg->sMAPID );

	SendClient ( pChar->m_dwClientID, nmg );
}

void CAgentServer::MsgPremiumSetFb ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNET_INVEN_PREMIUMSET_FB *pNetMsg = (GLMSG::SNET_INVEN_PREMIUMSET_FB *) nmg;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaID );
	if ( !pChar )										return;

	pChar->m_tPREMIUM = pNetMsg->tLMT;

	SendClient ( pChar->m_dwClientID, nmg );
}

void CAgentServer::MsgGmPkMode ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	int nUSERLVL = m_pClientManager->GetUserType ( dwClientID );

	if ( USER_GM1 <= nUSERLVL )
	{
		SendAllChannel ( nmg );
	}
}

void CAgentServer::MsgGmwherePcMap ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNET_GM_WHERE_PC_MAP *pNetMsg = (GLMSG::SNET_GM_WHERE_PC_MAP *) nmg;

	int nUSERLVL = m_pClientManager->GetUserType ( dwClientID );

	if ( USER_GM3 <= nUSERLVL )
	{
		PGLCHARAG pCHAR = GLAgentServer::GetInstance().GetCharID ( pNetMsg->dwCHARID );
		if ( pCHAR )
		{
			GLMSG::SNET_GM_WHERE_PC_MAP_FB NetMsgFB;
			NetMsgFB.dwChannel = (DWORD)pCHAR->m_nChannel;
			NetMsgFB.nidMAP = pCHAR->m_sCurMapID;
			SendClient ( dwClientID, &NetMsgFB );
		}
	}
}

void CAgentServer::MsgVietTimeReset ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
#if defined(VN_PARAM) //vietnamtest%%% && vietnamtest2
	GLMSG::SNETPC_VIETNAM_TIME_RESET* pNetMsg = (GLMSG::SNETPC_VIETNAM_TIME_RESET *) nmg;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar(dwGaeaID);
	if( !pChar )	return;

	m_pClientManager->SetVTGameTime(dwClientID,0);
	m_pClientManager->SetLoginTime(dwClientID);
	
	pChar->m_sVietnamSystem.gameTime   = 0;
	pChar->m_sVietnamSystem.loginTime  = CTime::GetCurrentTime().GetTime();


	GLMSG::SNETPC_VIETNAM_TIME_REQ_FB NetMsgFB;
	NetMsgFB.gameTime  = pChar->m_sVietnamSystem.gameTime;
	NetMsgFB.loginTime = pChar->m_sVietnamSystem.loginTime;
	SendField( dwClientID, &NetMsgFB );
#endif

	return;
}

///////////////////////////////////////////////////////////////////////////////
// 여름방학 PC 프로모션 아이템 '펑펑' 이벤트
// PC 방에서 하루에 한사람이 한번만 가능
// nResult
// 1 : 이벤트 응모가능
// 2 : 이미 이벤트에 응모했음, 하루에 한번만 가능.
// 3 : 응모불가 PC 방 IP 아님
void CAgentServer::MsgLogPungPungCheckBack(int nResult, DWORD dwClient, const char* szUserIP, USHORT uPort)
{
    // IP/PORT 가 동일할때만 처리
	if ((strcmp(szUserIP, m_pClientManager->GetClientIP(dwClient)) == 0) &&
		(m_pClientManager->GetClientPort(dwClient) == uPort))
	{
        int nPrize = 0; // 당첨된 상품.        
        // 1 번 응모가능일때만 아이템 당첨을...
        if (nResult == 1)
        {
            // 아이템 당첨처리

            // 응모했음을 로그로 남긴다.
            CLogPungPungWrite* pAction = new CLogPungPungWrite(m_pClientManager->GetUserID(dwClient),
                                                               m_pClientManager->GetUserNum(dwClient),
                                                               nPrize, // 당첨된 상품
                                                               m_pClientManager->GetClientIP(dwClient));
            COdbcManager::GetInstance()->AddLogJob((CDbAction*) pAction);
        }
        else // 2, 3 번일때는 오류메시지를...
        {

        }        
    }
}

///////////////////////////////////////////////////////////////////////////////
// 란 선물펑펑!! 이벤트 
// nResult
// 1  1 등 P-4 PC
// 2  2 등 효리폰
// 3  3 등 SONY 디카
// 4  4 등 아이리버 MP 플레이어
// 5  5 등 MAX 무비 영화 상품권 1 만원권
// 6  6 등 MBC Game 일일 무료 상품권
// 7  이미 복권을 입력한 사용자
// 8  PC 방 IP 가 아님
// 9  이미 사용한 복권
// 10 알수없는 오류
void CAgentServer::MsgLogLotteryBack(int nResult, DWORD dwClient, const char* szUserIP, USHORT uPort)
{
    // IP/PORT 가 동일할때만 처리
	if ((strcmp(szUserIP, m_pClientManager->GetClientIP(dwClient)) == 0) &&
		(m_pClientManager->GetClientPort(dwClient) == uPort))
	{
        NET_EVENT_LOTTERY_FB msg;
        msg.nResult = nResult;
        SendClient(dwClient, &msg);
    }
}

void CAgentServer::MsgApexData( MSG_LIST* pMsg )
{
	if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;
	NET_APEX_DATA* msg = reinterpret_cast<NET_APEX_DATA*> (pMsg->Buffer);
	int nLength = msg->nmg.dwSize - sizeof(NET_MSG_GENERIC);
	APEX_SERVER::NoticeApexProxy_UserData( dwClient, msg->szData, nLength );
}

// Apex 적용( 홍콩 )
void CAgentServer::MsgApexReturn( MSG_LIST* pMsg )
{
	if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;
	NET_APEX_RETURN* msg = reinterpret_cast<NET_APEX_RETURN*> (pMsg->Buffer);
	APEX_SERVER::NoticeApexProxy_UserReturn( dwClient, msg->nReturn );
}


void CAgentServer::MsgTracingLogPrint( MSG_LIST* pMsg )
{
	if (pMsg == NULL) return;
#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	NET_LOG_UPDATE_TRACINGCHAR* msg = reinterpret_cast<NET_LOG_UPDATE_TRACINGCHAR*> (pMsg->Buffer);
	int nTracing = CheckTracingUser( msg->szAccount, msg->nUserNum );
	if( nTracing == -1 ) return;
 	CDebugSet::ToTracingFile( m_vecTracingData[nTracing].strAccount.c_str(), msg->szLogMsg );
#endif
}
