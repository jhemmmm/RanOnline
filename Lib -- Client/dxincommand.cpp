#include "StdAfx.h"
#include "StringUtils.h"

#include "GLPeriod.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "GLFriendClient.h"

#include "../Lib -- ClientUI/Interface/InnerInterface.h"
#include "../Lib -- ClientUI/Interface/UITextControl.h"

#include "Emoticon.h"
#include "dxincommand.h"

#include "GLLandManClient.h"

/*item link, Juver, 2017/07/31 */
#include "G-Logic/GLItemLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern BOOL g_bFRAME_LIMIT;


namespace dxincommand
{
#if defined(NDEBUG)
	bool bDISP_FPS = false;
	bool bDISP_CONSOLE = false;
#else
	bool bDISP_FPS = true;
	bool bDISP_CONSOLE = true;
#endif

	CString BOOL2STR ( bool bOn )
	{
		return bOn ? "ON" : "OFF";
	}

	/*item link, Juver, 2017/07/31 */
	bool command ( const char* szcommand, SITEMLINK* pItemLink ) 
	{
		if ( szcommand==NULL || szcommand[0]==NULL )	return false;
		if ( szcommand[0]!='/' && szcommand[0]!='&' )	return false;

		STRUTIL::ClearSeparator ();
		STRUTIL::RegisterSeparator ( "\t" );
		STRUTIL::RegisterSeparator ( " " );
		STRUTIL::RegisterSeparator ( "," );

		CString strLINE(szcommand);
	
		CStringArray strArray ;
		STRUTIL::StringSeparate ( strLINE, strArray );
		if ( strArray.GetSize()==0 )					return true;

		CDebugSet::ToListView ( strLINE );

		CString strCOMMAND = strArray.GetAt(0);

		//	Note : 제스쳐.
		int nMOTION = CEmoticon::GetInstance().GetEmoticonOfMotion ( strCOMMAND.GetString() );
		if ( nMOTION!=UINT_MAX )
		{
			bool bACTION = GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( nMOTION, true );
			if ( bACTION )								return true;
		}

		/*if ( strCOMMAND=="/GM" )
		{
			GLGaeaClient::GetInstance().GetCharacter()->SET_RECORD_CHAT();
			return true;
		}*/
		if ( strCOMMAND=="/?" )
		{
			LISTEMOTICON& listEMOTICON = CEmoticon::GetInstance().GetEmoticon();
			LISTEMOTICON_ITER pos = listEMOTICON.begin();
			LISTEMOTICON_ITER end = listEMOTICON.end();

			for ( ; pos!=end; ++pos )
			{
				SEMOTICON &sEMOTICON = (*pos);
				CInnerInterface::GetInstance().PrintConsoleText ( "%s", sEMOTICON.strMotion.c_str() );
			}

			return true;
		}
		//add addstats cmd
		if( strCOMMAND =="/pow")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMPOW, value);
			return true;
		}
		else if( strCOMMAND =="/dex")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMDEX, value);
			return true;
		}
		else if( strCOMMAND =="/int")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMSPI, value);
			return true;
		}
		else if( strCOMMAND =="/vit")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMSTR, value);
			return true;
		}

		else if( strCOMMAND =="/stm")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMSTA, value);
			return true;
		}
		/*if ( strCOMMAND=="/whois" )
		{
			if ( strArray.GetSize() != 3 ) return true;

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwTIME = (DWORD) atoi (strPARAM_01.GetString());
			DWORD dwPASS = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_EMPTYMSG NetMsg;
			NetMsg.dwPass = dwPASS;
			NETSEND ( &NetMsg );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Hot damn!");
			return true;

		}*/

		
		//Prevent Normal Char from Executing GM Commands
			DWORD dwUSERLVL = GLGaeaClient::GetInstance().GetCharacter()->m_dwUserLvl;
			if ( dwUSERLVL < USER_MASTER )								  return true;


		if ( szcommand[0]=='&' )
		{
			int nLEN = (int) strlen(szcommand) - 1;
			if ( nLEN==0 )								return true;

			if ( nLEN > CHAT_MSG_SIZE )	 nLEN = CHAT_MSG_SIZE;

			NET_CHAT NetMsg;	
			NetMsg.nmg.nType	= NET_MSG_CHAT;
			NetMsg.emType		= CHAT_TYPE_GLOBAL;
			::StringCchCopyN ( NetMsg.szChatMsg, CHAT_MSG_SIZE, szcommand+1, nLEN );

			/*item link, Juver, 2017/07/31 */
			if ( pItemLink )
				NetMsg.sItemLink = *pItemLink;

			NETSEND ( &NetMsg );

			return true;
		}

		if ( strCOMMAND=="/dsp" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="off" )
			{
				bDISP_CONSOLE = false;
				bDISP_FPS = false;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "console, fps", BOOL2STR(false) );
			}
			if ( strPARAM_01=="console" )
			{
				bDISP_CONSOLE = !bDISP_CONSOLE;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, BOOL2STR(bDISP_CONSOLE) );
			}
			else if ( strPARAM_01=="fps" )
			{
				bDISP_FPS = !bDISP_FPS;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, BOOL2STR(bDISP_FPS) );
			}
			else if ( strPARAM_01=="gate" )
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				if ( !pLand )							return false;
				
				DxLandGateMan* pGateMan = &pLand->GetLandGateMan();
				if ( !pGateMan )						return false;

				PDXLANDGATE pLandGate = pGateMan->GetListLandGate();
				while ( pLandGate )
				{
					CString strGATE;
					strGATE.Format ( "%d ", pLandGate->GetGateID() );
					
					strGATE += "[";
					if ( pLandGate->GetFlags() & DxLandGate::GATE_OUT )	strGATE += "out";
					if ( (pLandGate->GetFlags()&DxLandGate::GATE_OUT) && (pLandGate->GetFlags()&DxLandGate::GATE_IN) )
						strGATE += "/";
					if ( pLandGate->GetFlags() & DxLandGate::GATE_IN )	strGATE += "in";
					strGATE += "]";

					if ( pLandGate->GetFlags() & DxLandGate::GATE_OUT )
					{
						strGATE += "=>";
						const char *szMAPNAME = GLGaeaClient::GetInstance().GetMapName ( pLandGate->GetToMapID() );
						if ( szMAPNAME )	strGATE += szMAPNAME;
					}

					CInnerInterface::GetInstance().PrintConsoleText ( strGATE.GetString() );

					pLandGate = pLandGate->m_pNext;
				};
			}
			else if ( strPARAM_01=="player" )
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				if ( !pLand )							return false;

				GLCHARCLIENTLIST* pLIST = pLand->GetCharList();
				GLCHARCLIENTNODE *pCharCur = pLIST->m_pHead;
				for ( ; pCharCur; pCharCur = pCharCur->pNext )
				{
					CInnerInterface::GetInstance().PrintConsoleText ( "%s	: charid = %d",
						pCharCur->Data->m_strName.c_str(), pCharCur->Data->GetCharData().dwCharID );
				}
			}
			else if ( strPARAM_01=="allplayer" )
			{
				GLMSG::SNET_GM_VIEWALLPLAYER NetMsg;
				NETSEND ( &NetMsg );
			}
			else if ( strPARAM_01=="event" )
			{
				GLMSG::SNET_GM_VIEWWORKEVENT NetMsg;
				NETSEND ( &NetMsg );
			}
		}

		else if ( strCOMMAND== "/shop_info" )
		{
			if ( strArray.GetSize() != 3 &&  strArray.GetSize() != 1  )			return true;

			SNATIVEID mapID = GLGaeaClient::GetInstance().GetActiveMapID();

			if( mapID.wMainID != 22 || mapID.wSubID != 0 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Can't use the command in this map." );
				return true;
			}

			GLMSG::SNET_GM_SHOP_INFO_REQ NetMsg;

			if( strArray.GetSize() == 3 )
			{
				CString strPARAM_01 = strArray.GetAt(1);
				CString strPARAM_02 = strArray.GetAt(2);

				NetMsg.bBasicInfo = FALSE;
				
				NetMsg.sSearchItemID.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				NetMsg.sSearchItemID.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				NETSENDTOFIELD ( &NetMsg );
			}else{
				NETSENDTOFIELD( &NetMsg );
			}
		}
/*
		else if ( strCOMMAND=="/fps_limit" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="on" )
			{
				g_bFRAME_LIMIT = TRUE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "fps_limit", "on" );
			}
			else if ( strPARAM_01=="off" )
			{
				g_bFRAME_LIMIT = FALSE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "fps_limit", "off" );
			}
		}
*/
		else if ( strCOMMAND=="/profiledis")
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="on" )
			{
				g_bProfile = TRUE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "profile", "on" );
			}
			else if ( strPARAM_01=="off" )
			{
				g_bProfile = FALSE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "profile", "off" );
			}
		}

		else if ( strCOMMAND=="/chatlogdis")
		{
			const	int	NORMAL		= 0x0001;	//	일반
			const	int	PRIVATE		= 0x0002;	//	귓속말
			const	int	PARTY		= 0x0004;	//	파티
			const	int	GUILD		= 0x0008;	//	클럽
			const	int	TOALL		= 0x0010;	//	확성기
			const	int ALLIANCE	= 0x0020;	//	동맹
			const	int	SYSTEM		= 0x0100;	//	시스템
			const	int	ALL			= NORMAL | PRIVATE | PARTY | GUILD | TOALL | ALLIANCE | SYSTEM;


			if ( strArray.GetSize() != 3 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			int nChatLogType = 0;

			if ( strPARAM_02 == "normal" )			nChatLogType = NORMAL;
			else if ( strPARAM_02 == "private" )	nChatLogType = PRIVATE;
			else if ( strPARAM_02 == "party" )		nChatLogType = PARTY;
			else if ( strPARAM_02 == "guild" )		nChatLogType = GUILD;
			else if ( strPARAM_02 == "toall" )		nChatLogType = TOALL;
			else if ( strPARAM_02 == "alliance" )	nChatLogType = ALLIANCE;
			else if ( strPARAM_02 == "system" )		nChatLogType = SYSTEM;
			else if ( strPARAM_02 == "all" )		nChatLogType = ALL;
			else	return true;

			if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().ChatLog( true, nChatLogType );
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s : %s", "chatlog", "on", strPARAM_02 );
			}
			else if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s : %s", "chatlog", "off", strPARAM_02 );
				CInnerInterface::GetInstance().ChatLog( false, nChatLogType );

			}
		}
#if defined(_RELEASED)
		else if ( strCOMMAND=="/showmethemoneydis")
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

            GLMSG::SNET_GM_SHOWMETHEMONEY NetMsg;
			NetMsg.llMoney = _ttoi64( strPARAM_01.GetString() );
			NETSEND(&NetMsg);
		}
#endif
		else if ( strCOMMAND== "/get_itemdis" ) //add itemcmd
		{
			if ( strArray.GetSize() != 5 &&  strArray.GetSize() != 4  )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Invalid Command! :P" );
				return true;
			}

			if( strArray.GetSize() == 5 )
			{
				CString strPARAM_01 = strArray.GetAt(1); //mid
				CString strPARAM_02 = strArray.GetAt(2); //sid
				CString strPARAM_03 = strArray.GetAt(3); //pass
				CString strPARAM_04 = strArray.GetAt(4); //amount

				SNATIVEID	sItemReq;
				sItemReq.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				sItemReq.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				WORD	wPass	 = (WORD) atoi ( strPARAM_03.GetString() );
				WORD	wAmount	 = (WORD) atoi ( strPARAM_04.GetString() );
				GLGaeaClient::GetInstance().GetCharacter()->ReqGMItem(sItemReq,wAmount,wPass);

			}
			else if( strArray.GetSize() == 4 )
			{
				CString strPARAM_01 = strArray.GetAt(1); //mid
				CString strPARAM_02 = strArray.GetAt(2); //sid
				CString strPARAM_03 = strArray.GetAt(3); //pass

				SNATIVEID	sItemReq;
				sItemReq.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				sItemReq.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				WORD	wPass	 = (WORD) atoi ( strPARAM_03.GetString() );
				WORD	wAmount	 = (WORD) 1 ;
				GLGaeaClient::GetInstance().GetCharacter()->ReqGMItem(sItemReq,wAmount,wPass);

			}
		}
		else if ( strCOMMAND=="/weatherdis" )
		{
			if ( strArray.GetSize() < 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwWeather(NULL);
			bool bActiveWeather(true);
			WORD map_mID = 0, map_sID = 0;
			DWORD dwApplyTime = 0;
            bool  bEveryApply = TRUE;

			if( strArray.GetSize() == 2 )
			{
				bEveryApply = TRUE; 

				if ( strPARAM_01=="clear" )
				{
					if( strArray.GetSize() != 2 ) return true;

					bActiveWeather = FALSE;
					dwWeather	   = NULL;
					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, "clear" );
				}else if ( strPARAM_01=="rain" )
				{
					dwWeather = FGW_RAIN;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="snow" )
				{
					dwWeather = FGW_SNOW;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="spore" )
				{
					dwWeather = FGW_SPORE;

					CInnerInterface::GetInstance().PrintConsoleText( "%s  : %s", strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="leave" )
				{
					dwWeather = FGW_LEAVES;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, "begin" );
				}

			}
			// Add OneMap Weather
			else if( strArray.GetSize() == 5 )
			{

				bEveryApply = FALSE; 

				CString strPARAM_02 = strArray.GetAt(2);
				CString strPARAM_03 = strArray.GetAt(3);
				CString strPARAM_04 = strArray.GetAt(4);

				map_mID		= atoi(strPARAM_02.GetString());
				map_sID		= atoi(strPARAM_03.GetString());
				dwApplyTime = atoi(strPARAM_04.GetString());

				if ( strPARAM_01=="rain" )
				{
					dwWeather = FGW_RAIN;

					CInnerInterface::GetInstance().PrintConsoleText ( "One Map Weather MapID %d %d : %d Minutes : %s  : %s", 
																	  map_mID, map_sID, dwApplyTime, strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="snow" )
				{
					dwWeather = FGW_SNOW;

					CInnerInterface::GetInstance().PrintConsoleText ( "One Map Weather MapID %d %d : %d Minutes : %s  : %s", 
																	  map_mID, map_sID, dwApplyTime, strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="spore" )
				{
					dwWeather = FGW_SPORE;

					CInnerInterface::GetInstance().PrintConsoleText ( "One Map Weather MapID %d %d : %d Minutes : %s  : %s", 
																	  map_mID, map_sID, dwApplyTime, strPARAM_01, "begin" );
				}
				else if ( strPARAM_01=="leave" )
				{
					dwWeather = FGW_LEAVES;

					CInnerInterface::GetInstance().PrintConsoleText ( "One Map Weather MapID %d %d : %d Minutes : %s  : %s", 
																	  map_mID, map_sID, dwApplyTime, strPARAM_01, "begin" );
				}
			}else if( strArray.GetSize() == 4 )
			{
				CString strPARAM_02 = strArray.GetAt(2);
				CString strPARAM_03 = strArray.GetAt(3);

				map_mID		= atoi(strPARAM_02.GetString());
				map_sID		= atoi(strPARAM_03.GetString());

				if ( strPARAM_01=="clear" )
				{
					dwWeather	   = NULL;
					CInnerInterface::GetInstance().PrintConsoleText ( "One Map Weather MapID %d %d : Clear", 
																	   map_mID, map_sID );
				}

				bEveryApply = FALSE; 
			}else{
				return TRUE;
			}


			// Map 설정해서 보내는 메시지
			if( bEveryApply == FALSE )
			{
				GLMSG::SNETSERVER_CTRL_WEATHER2 NetMsg;
				NetMsg.dwWeather   = dwWeather;
				NetMsg.map_mID     = map_mID;
				NetMsg.map_sID     = map_sID;
				NetMsg.dwApplyTime = dwApplyTime;
				NETSEND(&NetMsg);
			// 전체 맵에 적용되는 메시지
			}else
			{
				GLMSG::SNETSERVER_CTRL_WEATHER NetMsg;
				NetMsg.bActive   = bActiveWeather;
				NetMsg.dwWeather = dwWeather;
				NETSEND(&NetMsg);
			}
		}
		else if ( strCOMMAND=="/timedis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwTIME = (DWORD) atoi ( strPARAM_01.GetString() );
			if ( dwTIME > 24 )	return true;

			GLMSG::SNETSERVER_CTRL_TIME NetMsg;
			NetMsg.dwTime = dwTIME;
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %d", "time", dwTIME );
		}
		else if ( strCOMMAND=="/monthdis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwMONTH = (DWORD) atoi ( strPARAM_01.GetString() );
			if ( dwMONTH > 12 )	return true;

			GLMSG::SNETSERVER_CTRL_MONTH NetMsg;
			NetMsg.dwMonth = dwMONTH;
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %d", "month", dwMONTH );
		}
		else if ( strCOMMAND=="/itemholddis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
		
			bool bHold(true);

			if ( strPARAM_01=="on" )
			{
				bHold = true;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "itemhold", "on" );
			}
			else if ( strPARAM_01=="off" )
			{
				bHold = false;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "itemhold", "off" );
			}

			GLMSG::SNETSERVER_CTRL_GENITEMHOLD NetMsg;
			NetMsg.bHold = bHold;
			NETSEND(&NetMsg);
		}
		else if ( strCOMMAND=="/move2gatedis" || strCOMMAND=="/m2g" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD dwGATE = (DWORD) atoi ( strPARAM_01.GetString() );

			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLand )								return false;
			
			DxLandGateMan* pGateMan = &pLand->GetLandGateMan();
			if ( !pGateMan )							return false;

			PDXLANDGATE pGATE = pGateMan->FindLandGate ( dwGATE );
			if ( !pGATE )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "%d gate not exist", dwGATE );
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOVE2GATE_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.dwGATE = dwGATE;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOVE2GATE NetMsg;
				NetMsg.dwGATE = dwGATE;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/move2posdis" || strCOMMAND=="/m2p" )
		{
			if ( strArray.GetSize() != 5 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOVE2MAPPOS_FLD NetMsgFld;
				NetMsgFld.nidMAP.wMainID = ( WORD ) atoi( strPARAM_01.GetString() );
				NetMsgFld.nidMAP.wSubID = ( WORD ) atoi( strPARAM_02.GetString() );
				NetMsgFld.dwPOSX = ( DWORD ) atoi( strPARAM_03.GetString() );
				NetMsgFld.dwPOSY = ( DWORD ) atoi( strPARAM_04.GetString() );
				NetMsgFld.dwGaeaID = 0;

				NETSEND( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOVE2MAPPOS NetMsg;
				NetMsg.nidMAP.wMainID = ( WORD ) atoi( strPARAM_01.GetString() );
				NetMsg.nidMAP.wSubID = ( WORD ) atoi( strPARAM_02.GetString() );
				NetMsg.dwPOSX = ( DWORD ) atoi( strPARAM_03.GetString() );
				NetMsg.dwPOSY = ( DWORD ) atoi( strPARAM_04.GetString() );

				NETSEND( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/move2char_cn" || strCOMMAND=="/m2c_cn" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_MOVE2CHAR NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "move to charactor : %s", NetMsg.szNAME );
		}

		else if ( strCOMMAND=="/move2char_ci" || strCOMMAND=="/m2c_ci" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());

			GLMSG::SNETPC_GM_MOVE2CHAR NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "move to charactor : %s", NetMsg.szNAME );
		}

		else if ( strCOMMAND=="/msgviewdis" )
		{
			if ( strArray.GetSize() < 2 ) return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_GETWHISPERMSG NetMsg;
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "message view : off" );
			}else{			
				StringCchCopy( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
				CInnerInterface::GetInstance().PrintConsoleText ( "message view : %s on", NetMsg.szNAME );
			}
			NETSEND(&NetMsg);		

		}



		else if ( strCOMMAND=="/genchar_cn" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_GENCHAR NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "Recall charactor : %s", NetMsg.szNAME );
		}

		else if ( strCOMMAND=="/genchar_ci" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());

			GLMSG::SNETPC_GM_GENCHAR NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NETSEND(&NetMsg);
		}

		else if ( strCOMMAND=="/visible" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="none" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleNone();
			}
			else if ( strPARAM_01=="off" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleOff();
			}
			else if ( strPARAM_01=="on" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleOn();
			}
		}
		else if ( strCOMMAND=="/kickuserdis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD dwID = (DWORD) atoi ( strPARAM_01.GetString() );

			GLMSG::SNET_GM_KICK_USER NetMsg;
			NetMsg.dwID = dwID;
			NetMsg.bGaeaID = FALSE;
			NetMsg.bName = FALSE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/kickdis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNET_GM_KICK_USER NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NetMsg.bName = TRUE;
			NetMsg.bGaeaID = FALSE;

			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Disconnecting Character : %s", strPARAM_01 );
		}
		else if ( strCOMMAND=="/kick_alldis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			if ( strPARAM_01 == "sure" )
			{
				GLMSG::SNET_GM_KICK_USER NetMsg;
				NetMsg.bGaeaID = TRUE;
				NetMsg.bName = FALSE;

				NETSEND ( &NetMsg );

				CInnerInterface::GetInstance().PrintConsoleText ( "Disconnecting All GaeaID Except Executor" );
			}
			else CInnerInterface::GetInstance().PrintConsoleText ( "Disconnecting Failed" );

		}
		else if( strCOMMAND=="/limit_event_begindis" )
		{
#ifdef CH_PARAM_USEGAIN 
			return true;
#endif
			if ( strArray.GetSize() != 7 && strArray.GetSize() != 8 ) return true;

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			CString strPARAM_06 = strArray.GetAt(6);
			
			int start_Lv	  = (int)atoi(strPARAM_01.GetString());
			int end_Lv		  = (int)atoi(strPARAM_02.GetString());
			int play_Time	  = (int)atoi(strPARAM_03.GetString());
			int buster_Time   = (int)atoi(strPARAM_04.GetString());
			float expGain_Rate  = (float)atof(strPARAM_05.GetString());
			float itemGain_Rate = (float)atof(strPARAM_06.GetString());

			DWORD dwEventMinute = 0;			
			if( strArray.GetSize() == 8 ) 
			{
				CString strPARAM_07;
				strPARAM_07 = strArray.GetAt(7);
				dwEventMinute   = (DWORD)atoi(strPARAM_07.GetString());
			}
			

#if !defined(_RELEASED)
			if( expGain_Rate > 10.0f ) expGain_Rate = 10.0f;
			if( itemGain_Rate > 10.0f ) itemGain_Rate = 10.0f;
#endif


			CInnerInterface::GetInstance().PrintConsoleText ( 
				"Limit Event Start, StartLv: %d, EndLv: %d, PlayTime: %d, BusterTime: %d ExpGainRate: %.3f ItemGainRate: %.3f EventMinute %d",
				start_Lv, end_Lv, play_Time, buster_Time, expGain_Rate, itemGain_Rate, dwEventMinute );

			GLMSG::SNET_GM_LIMIT_EVENT_BEGIN NetMsg;
			NetMsg.start_Lv		 = start_Lv;
			NetMsg.end_Lv		 = end_Lv;
			NetMsg.play_Time	 = play_Time;
			NetMsg.buster_Time   = buster_Time;
			NetMsg.expGain_Rate  = expGain_Rate;
			NetMsg.itemGain_Rate = itemGain_Rate;
			NetMsg.dwEventMinute = dwEventMinute;
			NETSEND ( &NetMsg );

		}
		else if( strCOMMAND=="/limit_event_enddis" )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( "Limit Event End" );
			GLMSG::SNET_GM_LIMIT_EVENT_END NetMsg;
			NETSEND ( &NetMsg );

		}
		else if ( strCOMMAND=="/eventexp_begindis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			GLMSG::SNET_GM_EVENT_EXP NetMsg;
			NetMsg.fRATE = fSCALE;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/eventexp_enddis" )
		{
			GLMSG::SNET_GM_EVENT_EXP_END NetMsg;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/eventitem_begindis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			GLMSG::SNET_GM_EVENT_ITEM_GEN NetMsg;
			NetMsg.fRATE = fSCALE;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/eventitem_enddis" )
		{
			GLMSG::SNET_GM_EVENT_ITEM_GEN_END NetMsg;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/eventmoney_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			GLMSG::SNET_GM_EVENT_MONEY_GEN NetMsg;
			NetMsg.fRATE = fSCALE;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/eventmoney_enddis" )
		{
			GLMSG::SNET_GM_EVENT_MONEY_GEN_END NetMsg;
			NetMsg.dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/pk_modedis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			BOOL bMODE(FALSE);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "pk_mode : off" );
				bMODE = FALSE;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "pk_mode : on" );
				bMODE = TRUE;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "pk_mode : invalid param" );
				return true;
			}

			GLMSG::SNET_SERVER_PLAYERKILLING_MODE NetMsg;
			NetMsg.bMODE = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/where_npcdis" )
		{
			if ( strArray.GetSize() != 3 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
		
			WORD mid = (WORD) atoi (strPARAM_01.GetString());
			WORD sid = (WORD) atoi (strPARAM_02.GetString());

			CInnerInterface::GetInstance().PrintConsoleText ( "where_npc : [%d/%d]", mid, sid );

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_WHERE_NPC_FLD NetMsgFld;
				NetMsgFld.nidNPC   = SNATIVEID(mid,sid);
				NetMsgFld.dwGaeaID = 0;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_WHERE_NPC NetMsg;
				NetMsg.nidNPC = SNATIVEID(mid,sid);
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/crow_listdis" )
		{
			if ( strArray.GetSize() > 2 )			return true;

			BYTE dwFindMob = 0;
			if( strArray.GetSize() != 1 )
			{
				CString strPARAM_01 = strArray.GetAt(1);				
				if ( strPARAM_01=="mob" )
				{
					dwFindMob = 1;
				}
				else if ( strPARAM_01=="npc" )
				{
					dwFindMob = 2;
				}
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_PRINT_CROWLIST_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.dwFindMob = dwFindMob;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_PRINT_CROWLIST NetMsg;
				NetMsg.dwFindMob = dwFindMob;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/bigheaddis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bMODE(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : off" );
				bMODE = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : on" );
				bMODE = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_BIGHEAD NetMsg;
			NetMsg.bBIGHEAD = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/bighanddis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bMODE(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : off" );
				bMODE = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : on" );
				bMODE = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_BIGHAND NetMsg;
			NetMsg.bBIGHAND = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/mob_gendis" )
		{
			if ( strArray.GetSize() != 5 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			
			WORD wMainID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wSubID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wPosX = (WORD) atoi (strPARAM_03.GetString() );
			WORD wPosY = (WORD) atoi (strPARAM_04.GetString() );

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMainID, wSubID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen : invalid mob_id" );
				return true;
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_GEN_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.wMainID = wMainID;
				NetMsgFld.wSubID = wSubID;
				NetMsgFld.wPosX = wPosX;
				NetMsgFld.wPosY = wPosY;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_GEN NetMsg;
				NetMsg.wMainID = wMainID;
				NetMsg.wSubID = wSubID;
				NetMsg.wPosX = wPosX;
				NetMsg.wPosY = wPosY;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_gen_exdis" )
		{
			if ( strArray.GetSize() != 10 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			CString strPARAM_06 = strArray.GetAt(6);
			CString strPARAM_07 = strArray.GetAt(7);
			CString strPARAM_08 = strArray.GetAt(8);
			CString strPARAM_09 = strArray.GetAt(9);
			
			WORD wMobMID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wMobSID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wMapMID = (WORD) atoi (strPARAM_03.GetString() );
			WORD wMapSID = (WORD) atoi (strPARAM_04.GetString() );
			WORD wPosX	 = (WORD) atoi (strPARAM_05.GetString() );
			WORD wPosY	 = (WORD) atoi (strPARAM_06.GetString() );
			WORD wRange  = (WORD) atoi (strPARAM_07.GetString() );
			WORD wNum	 = (WORD) atoi (strPARAM_08.GetString() );
			WORD wChannel = (WORD) atoi (strPARAM_09.GetString() );

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMobMID, wMobSID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid mob_id" );
				return false;
			}
			
			SNATIVEID sMapID(wMapMID, wMapSID);
			GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
			GLMapList::FIELDMAP_ITER iter = MapsList.find ( sMapID.dwID );
			if ( iter==MapsList.end() )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid map_id" );
				return false;
			}

			if ( wRange*100 < wNum )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : out of mob amount in range" );
				return false;
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_GEN_EX_FLD NetMsgFld;
				NetMsgFld.wMobMID  = wMobMID;
				NetMsgFld.wMobSID  = wMobSID;
				NetMsgFld.wMapMID  = wMapMID;
				NetMsgFld.wMapSID  = wMapSID;
				NetMsgFld.wPosX	   = wPosX;
				NetMsgFld.wPosY    = wPosY;
				NetMsgFld.wRange   = wRange;
				NetMsgFld.wNum     = wNum;
				NetMsgFld.dwGaeaID = 0;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_GEN_EX NetMsg;
				NetMsg.wMobMID = wMobMID;
				NetMsg.wMobSID = wMobSID;
				NetMsg.wMapMID = wMapMID;
				NetMsg.wMapSID = wMapSID;
				NetMsg.wPosX   = wPosX;
				NetMsg.wPosY   = wPosY;
				NetMsg.wRange  = wRange;
				NetMsg.wNum	   = wNum;
				NetMsg.bThisChannel = wChannel;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_deldis" )
		{
			if ( strArray.GetSize() > 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_del : invalid param" );
				return true;
			}

			WORD wMobMID = USHRT_MAX;
			WORD wMobSID = USHRT_MAX;
			bool bMaterial = false;

			if ( strArray.GetSize() == 2 )
			{
				 CString strPARAM_01 = strArray.GetAt(1);
				 bMaterial = (bool) atoi( strPARAM_01.GetString() );
			}

			if ( strArray.GetSize() == 3 )
			{
				CString strPARAM_01 = strArray.GetAt(1);
				CString strPARAM_02 = strArray.GetAt(2);
				wMobMID = (WORD) atoi (strPARAM_01.GetString() );
				wMobSID = (WORD) atoi (strPARAM_02.GetString() );
			}


			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_DEL_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;	
				NetMsgFld.sMobID.wMainID = wMobMID;
				NetMsgFld.sMobID.wSubID = wMobSID;
				NetMsgFld.bMaterial = bMaterial;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_DEL NetMsg;
				NetMsg.sMobID.wMainID = wMobMID;
				NetMsg.sMobID.wSubID = wMobSID;
				NetMsg.bMaterial = bMaterial;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_del_exdis" )
		{
			if ( strArray.GetSize() != 6 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_del_ex : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			
			WORD wMobMID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wMobSID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wMapMID = (WORD) atoi (strPARAM_03.GetString() );
			WORD wMapSID = (WORD) atoi (strPARAM_04.GetString() );
			WORD wChannel = (WORD) atoi (strPARAM_05.GetString() );

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMobMID, wMobSID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid mob_id" );
				return false;
			}
			
			SNATIVEID sMapID(wMapMID, wMapSID);
			GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
			GLMapList::FIELDMAP_ITER iter = MapsList.find ( sMapID.dwID );
			if ( iter==MapsList.end() )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid map_id" );
				return false;
			}
			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_DEL_EX_FLD NetMsgFld;
				NetMsgFld.wMobMID = wMobMID;
				NetMsgFld.wMobSID = wMobSID;
				NetMsgFld.wMapMID = wMapMID;
				NetMsgFld.wMapSID = wMapSID;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_DEL_EX NetMsg;
				NetMsg.wMobMID = wMobMID;
				NetMsg.wMobSID = wMobSID;
				NetMsg.wMapMID = wMapMID;
				NetMsg.wMapSID = wMapSID;
				NETSEND ( &NetMsg );
			}			
		}
		else if ( strCOMMAND=="/lev_newdis" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_new : invalid param" );
				return true;
			}
			CString strPARAM_01 = strArray.GetAt(1);

			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLAND )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_new : unknown error" );
				return true;
			}

			SNATIVEID nidMAP = pLAND->GetMapID();

			GLMSG::SNET_GM_MOB_LEV NetMsg;
			NetMsg.wMAP_MID = nidMAP.wMainID;
			NetMsg.wMAP_SID = nidMAP.wSubID;
			StringCchCopy ( NetMsg.szLevelFile, GLMSG::SNET_GM_MOB_LEV::FILE_NAME, strPARAM_01.GetString() );
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/lev_deldis" )
		{
			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLAND )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_del : unknown error" );
				return true;
			}

			SNATIVEID nidMAP = pLAND->GetMapID();

			GLMSG::SNET_GM_MOB_LEV_CLEAR NetMsg;
			NetMsg.wMAP_MID = nidMAP.wMainID;
			NetMsg.wMAP_SID = nidMAP.wSubID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/chatblock_uadis" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_ua : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_UACCOUNT	NetMsg;
			StringCchCopy ( NetMsg.szUACCOUNT, USR_ID_LENGTH+1, strPARAM_01.GetString() );
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "request chat block : uaccount %s, minute %d", NetMsg.szUACCOUNT, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/chatblock_cndis" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_cn : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_CHARNAME	NetMsg;
			StringCchCopy ( NetMsg.szCHARNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "request chat block : charname %s, minute %d", NetMsg.szCHARNAME, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/chatblock_cidis" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_ci : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());
			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_CHARID	NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "request chat block : charid %d, minute %d", NetMsg.dwCHARID, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/charinfo_cndis" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "charinfo_cn : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNET_GM_CHAR_INFO_4NAME NetMsg;
			StringCchCopy ( NetMsg.szCHARNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/charinfo_cidis" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "charinfo_ci : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());

			GLMSG::SNET_GM_CHAR_INFO_4CHARID NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/freepk_ondis" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "freepk : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwTIME = (DWORD) atoi (strPARAM_01.GetString());

			GLMSG::SNET_GM_FREEPK NetMsg;
			NetMsg.dwPKTIME = dwTIME;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/freepk_offdis" )
		{
			DWORD dwTIME(0);

			GLMSG::SNET_GM_FREEPK NetMsg;
			NetMsg.dwPKTIME = dwTIME;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/warning_msgdis" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bOn(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Warning Message : off" );
				bOn = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Warning Message : on" );
				bOn = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "warning_msg : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_WARNING_MSG NetMsg;
			NetMsg.bOn = bOn;
			NETSEND ( &NetMsg );
		}
		//	Memo :	(이동속도설정 1~50까지만 설정가능)
		//			(공격속도설정 1~50까지만 설정가능)
		//			(공격력설정 1~50까지만 설정가능)
		else if( strCOMMAND=="/event_ex_begindis" )
		{
			if ( strArray.GetSize() != 3 )			return true;
			
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);
			WORD wValue = (WORD)atoi( strPARAM_02.GetString() );

			if ( strPARAM_01=="speed" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_begin : speed" );
				emType = EMGM_EVENT_SPEED;
			}
			else if ( strPARAM_01=="aspeed" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_begin : aspeed" );
				emType = EMGM_EVENT_ASPEED;
			}
			else if ( strPARAM_01=="attack" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_begin : attack" );
				emType = EMGM_EVENT_ATTACK;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_begin : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_EVENT_EX NetMsg;
			NetMsg.emType = emType;
			NetMsg.wValue = wValue;
			NETSEND ( &NetMsg );
		}
		else if( strCOMMAND=="/event_ex_enddis" )
		{
			if ( strArray.GetSize() != 2 )			return true;

			CString strPARAM_01 = strArray.GetAt(1);
			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);

			if ( strPARAM_01=="speed" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_end : speed" );
				emType = EMGM_EVENT_SPEED;
			}
			else if ( strPARAM_01=="aspeed" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_end : aspeed" );
				emType = EMGM_EVENT_ASPEED;
			}
			else if ( strPARAM_01=="attack" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_end : attack" );
				emType = EMGM_EVENT_ATTACK;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "event_ex_end : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_EVENT_EX_END NetMsg;
			NetMsg.emType = emType;
			NETSEND ( &NetMsg );
		}
		
		/*event map move, Juver, 2017/08/25 */
		else if ( strCOMMAND=="/m2mp" )
		{
			if ( strArray.GetSize() != 5 )			return true;

			CString strPARAM_01 = strArray.GetAt(1); //map mid
			CString strPARAM_02 = strArray.GetAt(2); //map sid
			CString strPARAM_03 = strArray.GetAt(3); //map posx
			CString strPARAM_04 = strArray.GetAt(4); //map posy

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FLD net_msg_command;
				net_msg_command.nidMAP.wMainID = (WORD)atoi( strPARAM_01.GetString() );
				net_msg_command.nidMAP.wSubID = (WORD)atoi( strPARAM_02.GetString() );
				net_msg_command.wPosX = (WORD)atoi( strPARAM_03.GetString() );
				net_msg_command.wPosY = (WORD)atoi( strPARAM_04.GetString() );
				net_msg_command.bCurMapCheck = FALSE;
				net_msg_command.bCalculatePos = TRUE;
				net_msg_command.dwCharID = 0;

				NETSEND(&net_msg_command);
			}
			else
			{
				GLMSG::SNETPC_GM_MOVE2_MAPPOS net_msg_command;
				net_msg_command.nidMAP.wMainID = (WORD)atoi( strPARAM_01.GetString() );
				net_msg_command.nidMAP.wSubID = (WORD)atoi( strPARAM_02.GetString() );
				net_msg_command.wPosX = (WORD)atoi( strPARAM_03.GetString() );
				net_msg_command.wPosY = (WORD)atoi( strPARAM_04.GetString() );

				NETSEND(&net_msg_command);
			}
			
		}
		/*private market set, Juver, 2018/01/02 */
		else if ( strCOMMAND=="/setvend_on" )
		{
			GLMSG::SNET_GM_SET_PRIVATE_MARKET NetMsg;
			NetMsg.bEnable = TRUE;
			NETSEND ( &NetMsg );
			CInnerInterface::GetInstance().PrintConsoleText ( "vend on" );
		}
		/*private market set, Juver, 2018/01/02 */
		else if ( strCOMMAND=="/setvend_off" )
		{
			GLMSG::SNET_GM_SET_PRIVATE_MARKET NetMsg;
			NetMsg.bEnable = FALSE;
			NETSEND ( &NetMsg );
			CInnerInterface::GetInstance().PrintConsoleText ( "vend off" );
		}
		/*megaphone set, Juver, 2018/01/02 */
		else if ( strCOMMAND=="/setmega_on" )
		{
			GLMSG::SNET_GM_SET_MEGAPHONE NetMsg;
			NetMsg.bEnable = TRUE;
			NETSEND ( &NetMsg );
			CInnerInterface::GetInstance().PrintConsoleText ( "mega on" );
		}
		/*megaphone set, Juver, 2018/01/02 */
		else if ( strCOMMAND=="/setmega_off" )
		{
			GLMSG::SNET_GM_SET_MEGAPHONE NetMsg;
			NetMsg.bEnable = FALSE;
			NETSEND ( &NetMsg );
			CInnerInterface::GetInstance().PrintConsoleText ( "mega off" );
		}
			
		/*get process command, Juver, 2017/06/08 */
		else if ( strCOMMAND=="/getproc" )
		{
			if ( strArray.GetSize() != 2 ) return true;

			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNET_GM_CHAR_GETPROC NetMsg;
			StringCchCopy ( NetMsg.szCHARNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/kwreason" )
		{
			if ( strArray.GetSize() != 3 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			WORD nReasonType = (WORD) atoi (strPARAM_02.GetString() );

			if( nReasonType == 0 ) return true;

			GLMSG::SNET_GM_KICK_USER NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NetMsg.bName = TRUE;
			NetMsg.bReason = TRUE;
			NetMsg.nReasonType = nReasonType;

			NETSEND ( &NetMsg );

			//CInnerInterface::GetInstance().PrintConsoleText ( "Kicked Character %s with nReasonType %d", strPARAM_01, nReasonType );
		}
		else
		{
			CString strPARAM_00 = strArray.GetAt(0);
			CInnerInterface::GetInstance().PrintConsoleText ( "invalid command : %s", strPARAM_00.GetString() );
		}

		return true;
	}
};
