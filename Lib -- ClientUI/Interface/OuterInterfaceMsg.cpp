#include "StdAfx.h"
#include "OuterInterface.h"
#include "DxGlobalStage.h"
#include "ModalWindow.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "RANPARAM.h"
#include "SelectCharacterPage.h"
#include "./BasicButton.h"
#include "SelectServerPage.h"
#include "CharacterCreatePage.h"
#include "CharacterCreateInfo.h"
#include "CharacterCreateRotate.h"

//userpanel
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangePinPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangePasswordPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPasswordPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPasswordPage2.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPinPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangeEmailPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/TopUpPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/GameTimeCVTPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/PremiumPointCVTPage.h"
#include "SelectCharacterUserPanel.h"
//#include "../Lib -- ClientUI/Interface/IngameUserPanel/SelectCharacterUserPanel.h"

/*register page, Juver, 2017/11/18 */
#include "RegisterPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void COuterInterface::MsgProcess ( LPVOID pBuffer )
{
	if( !pBuffer ) return ;

	NET_MSG_GENERIC* nmg = (NET_MSG_GENERIC*) pBuffer;

	switch ( nmg->nType )
	{
	case NET_MSG_LOGIN_FB:
		{
			NET_LOGIN_FEEDBACK_DATA* nlfd = (NET_LOGIN_FEEDBACK_DATA *) nmg;

			int nServerPatchProgramVer = nlfd->nPatchProgramVer;
			int nServerGameProgramVer = nlfd->nGameProgramVer;

			SetCharRemainExM( nlfd->nExtremeM );
			SetCharRemainExW( nlfd->nExtremeW );
			SetCharRemain( nlfd->uChaRemain );

            int nClientPatchProgramVer;
			int nClientGameProgramVer;

			BOOL bResult = FALSE;
			bResult = LoadClientVersion ( nClientPatchProgramVer, nClientGameProgramVer );
			if ( !bResult )
			{
				HideGroup ( MODAL_WINDOW_OUTER );
				DoModalOuter ( ID2GAMEEXTEXT ("LOGIN_FAILED_VER_FILE"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				return ;
			}

            BOOL bDifferentVer = FALSE;
            if ( (nClientPatchProgramVer < nServerPatchProgramVer) ||
				(nClientGameProgramVer < nServerGameProgramVer) )
			{
				bDifferentVer = TRUE;
			}
	
			switch ( nlfd->nResult )
			{
			case EM_LOGIN_FB_SUB_OK:	// 로그인 성공	
		{
			CString strEmail , strText,strText2,strText3 , strUserID;
			strUserID.Format("%s",nlfd->szDaumGID);
			strEmail.Format("%s",nlfd->szUserEmail);
			int AtOffset = -1;
			unsigned int Length = (int)strlen(nlfd->szUserEmail); // Length = StringLength (strlen) of EmailAddress
			for(unsigned int i = 0; i < Length; i++)
			{
				if(nlfd->szUserEmail[i] == '@') // If one of the characters is @, store it's position in AtOffset
					AtOffset = (int)i;
			}
			
			strText.Format("%s", strEmail.Left( 1 ));
			for (unsigned int i = 0; i < AtOffset-3; i++)
			{
				strText += "*";
			}
			strText2.Format("%s",strEmail.Mid(AtOffset-1,Length));
			strText3.Format("%s%s",strText.GetString(),strText2.GetString());
			SetUserInfo ( nlfd->uUserPoints , nlfd->uBattlePoints  , nlfd->dwGameTime , strText3.GetString() , strUserID );
				HideGroup ( MODAL_WINDOW_OUTER );

				//	버전이 틀릴 경우
	
				if ( bDifferentVer )
				{
					DoModalOuter ( ID2GAMEEXTEXT ("LOGIN_FAILED_INVALID_VER"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
					return ;
				}
/*
//#if defined(KRT_PARAM) || defined(KR_PARAM)
#if defined ( JP_PARAM )	// 2차 패스워드
				if ( RANPARAM::VALIDIDPARAM () )
				{
					SetDaumGID( CString(nlfd->szDaumGID) );
					ToSecPassPage ( SELECT_SERVER_PAGE, nlfd->nCheckFlag );
				}
				else
				{
					ToSecPassPage ( LOGIN_PAGE, nlfd->nCheckFlag );
				}
#else
*/
				//	로그인 성공 & 캐릭터 선택 화면으로 이동
				DxGlobalStage::GetInstance().OnInitLogin ();
				DxGlobalStage::GetInstance().GetNetClient()->SndChaAllBasicInfo();

                if ( RANPARAM::VALIDIDPARAM () )
				{
					ToSelectCharacterPage ( SELECT_SERVER_PAGE );
				}
				else
				{
					ToSelectCharacterPage ( LOGIN_PAGE );
				}
//#endif


#if defined ( CH_PARAM ) // 중국 MMOSPEED
                RunMMOSpeed();
#endif
			}
				break;

			case EM_LOGIN_FB_SUB_FAIL:		// 로그인 실패				
				DoModalOuter ( ID2GAMEEXTEXT ("LOGIN_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );				
				break;

			case EM_LOGIN_FB_SUB_SYSTEM:	// 시스템에러로 로그인 실패
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_8"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );				
				break;

			case EM_LOGIN_FB_SUB_USAGE:		// 허가된 사용시간 에러
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_9"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );				
				break;

			case EM_LOGIN_FB_SUB_DUP:		// 중복된 접속
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_10"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );				
				break;

			case EM_LOGIN_FB_SUB_INCORRECT:	// ID / PWD 불일치 (새로운 계정을 생성 또는 재입력요구)
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_11"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );				
				break;

			case EM_LOGIN_FB_SUB_IP_BAN:	// ID/PWD 가 일치하지만 IP 가 사용불가능 상태(접근 차단된 IP)
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_12"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				break;

			case EM_LOGIN_FB_CH_FULL: // 최대 동접 인원 도달시 NET_MSG_SVR_FULL 메시지가 온다.
				m_pSelectServerPage->SetChannelFull();
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_FULL"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				ToSelectServerPage ( LOGIN_PAGE );
				break;

			// 2004-02-26 jgkim 추가
			case EM_LOGIN_FB_SUB_BLOCK:		// ID/PWD 일치하지만 ID 가 Block 된 상태
                DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_BLOCK"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				break;

			case EM_LOGIN_FB_SUB_RANDOM_PASS:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_RANDOM_PASS_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				break;

			case EM_LOGIN_FB_SUB_ALREADYOFFLINE: // // 로그인 실패이고 이미 오프라인일때
				DoModalOuter ( ID2GAMEEXTEXT ("LOGIN_ALREADYOFFLINE"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Daum : 비정상적 로그인데이타 (새롭게 웹에서 로그인후 게임실행)
			case EM_LOGIN_FB_SUB_UNCON:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_INVALID_LOGIN"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Daum : 인증시간이 초과된 사용자이기 때문에, 웹에 재접속을 하라는 메시지를 보낸다
			case EM_LOGIN_FB_SUB_EXPIRED:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_EXPIRED_USER"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Daum : GID가 없음 웹에서 새롭게 로그인
			case EM_LOGIN_FB_SUB_GID_ERR:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_NOGID"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Daum : UID가 없음 웹에서 새롭게 로그인
			case EM_LOGIN_FB_SUB_UID_ERR:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_NOUID"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Daum : 유저 삽입실패
			case EM_LOGIN_FB_SUB_UNKNOWN:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_UNKNOWN_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;		

			// Daum : 주민번호오류
			case EM_LOGIN_FB_SUB_SSNHEAD:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_SSN_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;		

			// Daum : 성인인증실패
			case EM_LOGIN_FB_SUB_ADULT:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_ADULT_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Thai : 18 세미만 3 시간 경과
			case EM_LOGIN_FB_THAI_UNDER18_3HOUR:
				DoModalOuter( ID2GAMEEXTEXT("LOGINSTAGE_18LESS_TIME_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Thai : 18 세미만 접속불가능 시간
			case EM_LOGIN_FB_THAI_UNDER18_TIME:
				DoModalOuter( ID2GAMEEXTEXT("LOGINSTAGE_18LESS_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

			// Thai : 18 세이상 접속불가능 시간
			case EM_LOGIN_FB_THAI_OVER18_TIME:
				DoModalOuter( ID2GAMEEXTEXT("LOGINSTAGE_18MORE_FAILED"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
				break;

				//login delay
			case EM_LOGIN_FB_SUB_REQUIRETIME:
				DoModalOuter( ID2GAMEEXTEXT("LOGINSTAGE_30"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				break;

				//login delay
			/*case EM_LOGIN_FB_SUB_PCID_USED:
				DoModalOuter( ID2GAMEEXTEXT("LOGINSTAGE_31"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				break;*/

			//case WAIT_MSG_FROM_SERVER:
			//	DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_6"), MODAL_INFOMATION, OK );				
			//	break;

			default:
				break;
			};
		}
		break;

	case NET_MSG_PASSCHECK_FB:
		{
			NET_PASSCHECK_FEEDBACK_DATA* nlfd = (NET_PASSCHECK_FEEDBACK_DATA *) nmg;

			HideGroup ( MODAL_WINDOW_OUTER );

			switch ( nlfd->nResult )
			{
			case EM_LOGIN_FB_SUB_OK : // 로그인 성공
				//	로그인 성공 & 캐릭터 선택 화면으로 이동
				DxGlobalStage::GetInstance().OnInitLogin ();
				DxGlobalStage::GetInstance().GetNetClient()->SndChaAllBasicInfo();

				ToSelectCharacterPage ( SECPASS_CHECKPAGE );
				break;

			case EM_LOGIN_FB_SUB_FAIL : // ID / PWD 불일치 (로그인 실패)
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_11"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				SetModalCallWindowID( SECPASS_CHECKPAGE );
				break;

			case EM_LOGIN_FB_SUB_PASS_OK : // 초기 패스워드 입력 성공
				//	로그인 성공 & 캐릭터 선택 화면으로 이동
				DxGlobalStage::GetInstance().OnInitLogin ();
				DxGlobalStage::GetInstance().GetNetClient()->SndChaAllBasicInfo();

				ToSelectCharacterPage ( SECPASS_SETPAGE );
				break;
			case EM_LOGIN_FB_SUB_SECID_ALREADY:
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_13"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				SetModalCallWindowID( SECPASS_SETPAGE );
				break;

			case DB_ERROR : // 초기 패스워드 업데이트 실패
			case USHRT_MAX :
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_11"), MODAL_INFOMATION, OK, OUTER_MODAL_GENERAL_LOGINFAIL );
				SetModalCallWindowID( SECPASS_SETPAGE );
				break;	
			}
		}
		break;
	case JAPAN_NET_MSG_UUID:
		{
			JAPAN_NET_LOGIN_UUID* netMsg = (JAPAN_NET_LOGIN_UUID*) nmg;
			RANPARAM::SETJAPANUUID( netMsg->szUUID );
		}
		break;
	case NET_MSG_LOBBY_REQ_GAME_JOIN:
		{
			//	NOTE
			//		조인
			//GetSelectCharacterPage()->SndGameJoin ();
			/*security key on charselect, DevArt22*/
			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
			if( pNetClient->IsOnline() == true )
			{
				COuterInterface::GetInstance().ToCharSelectPassPage ( CHARSELECT_PASS_PAGE );
			}
		}
		break;

	case NET_MSG_LOBBY_GAME_COMPLETE:
		{
			CDebugSet::ToListView ( "NET_MSG_LOBBY_GAME_COMPLETE" );
			DxGlobalStage::GetInstance().ToChangeStage ( DxGlobalStage::EM_STAGE_GAME );
		}
		break;

	case NET_MSG_LOBBY_CHAR_JOIN_FB:
		{
			GLMSG::SNETLOBBY_CHARJOIN_FB *pNetMsg = (GLMSG::SNETLOBBY_CHARJOIN_FB *) nmg;
			switch (pNetMsg->emCharJoinFB )
			{
			case EMCJOIN_FB_NOWLOGIN: //접속중
				DoModalOuter ( ID2GAMEEXTEXT ( "CHARACTERSTAGE_10" ), MODAL_INFOMATION, OK, OUTER_MODAL_ALREADYCONNECT );
				break;
			case EMCJOIN_FB_ERROR: //에러
				DoModalOuter ( ID2GAMEEXTEXT ( "CHARACTERSTAGE_11" ), MODAL_INFOMATION, OK, OUTER_MODAL_CREATEFAILED );
				break;
			case EMCJOIN_FB_PKPOINT: //pk 성향으로 오류
				DoModalOuter ( ID2GAMEEXTEXT ( "CHARACTERSTAGE_12" ), MODAL_INFOMATION, OK, OUTER_MODAL_NON_PK_CHANNEL );
				break;
			};
		}
		break;
	case CHINA_NET_MSG_RESETPIN_FB:
		{
			NET_RPN_FEEDBACK_DATA2* nfld = (NET_RPN_FEEDBACK_DATA2*) nmg;
			switch (nfld->nResult)
			{
			case EM_RESETPIN_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Reset Pin Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_RESETPIN_FB_SUB_OK :
				{
					CString strText;
					strText.Format("Reset Pin Success : %s",nfld->szUserpass2Hold);
					DoModalOuter ( strText, MODAL_INFOMATION, OK, OUTER_MODAL_RESETPIN );
				}
				break;
			default :
				{
					DoModalOuter ( "Reset Pin Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
		}break;
	case CHINA_NET_MSG_RESETPASS_FB:
		{
			NET_RPS_FEEDBACK_DATA2* nfld = (NET_RPS_FEEDBACK_DATA2*) nmg;
			switch (nfld->nResult)
			{
			case EM_RESETPASS_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Reset Pass Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_RESETPASS_FB_SUB_OK :
				{
					CString strText;
					strText.Format("Reset Password : %s",nfld->szUserid);
					CResetPasswordPage2* pResetPass2 = COuterInterface::GetInstance().GetResetPassPage2();
					DoModalOuter ( "Information , Passed. the system is now prompting you to Change Password Page.", MODAL_INFOMATION, OK, OUTER_MODAL_RESETPASS );
					pResetPass2->SetTitleName(strText);
				}
				break;
			default :
				{
					DoModalOuter ( "Reset Pass Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
		}break;
	case CHINA_NET_MSG_RESETPASS2_FB:
		{
			NET_RPS2_FEEDBACK_DATA2* nfld = (NET_RPS2_FEEDBACK_DATA2*) nmg;
			switch (nfld->nResult)
			{
			case EM_EMAIL_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Reset Pass Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_EMAIL_FB_SUB_OK :
				{
					DoModalOuter ( "Reset Pass Success", MODAL_INFOMATION, OK, OUTER_MODAL_RESETPASS2 );
				}
				break;
			default :
				{
					DoModalOuter ( "Reset Pass Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
		}break;
		
		case CHINA_NET_MSG_EMAIL_FB:
		{
			NET_CE_FEEDBACK_DATA2* nfld = (NET_CE_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_EMAIL_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Email Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_EMAIL_FB_SUB_OK :
				{
					DoModalOuter ( "Email Update Success", MODAL_INFOMATION, OK, OUTER_MODAL_CHANGEEMAIL );
				}
				break;
			default :
				{
					DoModalOuter ( "Email Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		case CHINA_NET_MSG_GT2VP_FB:
		{
			NET_GTCVT_FEEDBACK_DATA2* nfld = (NET_GTCVT_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_GT2VP_FB_SUB_FAIL : 
				{
					DoModalOuter ( "GameTime Conversion Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_GT2VP_FB_SUB_OK :
				{
					DoModalOuter ( "GameTime Conversion Success", MODAL_INFOMATION, OK, OUTER_MODAL_GAMETIMECVT );
				}
				break;
			default :
				{
					DoModalOuter ( "GameTime Conversion Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		case CHINA_NET_MSG_PP2VP_FB:
		{
			NET_PPCVT_FEEDBACK_DATA2* nfld = (NET_PPCVT_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_PP2VP_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Premium Point Conversion Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_PP2VP_FB_SUB_OK :
				{
					DoModalOuter ( "Premium Point Conversion Success", MODAL_INFOMATION, OK, OUTER_MODAL_PREMIUMPOINTCVT );
				}
				break;
			default :
				{
					DoModalOuter ( "Premium Point Conversion Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		case CHINA_NET_MSG_TOPUP_FB:
		{
			NET_TOPUP_FEEDBACK_DATA2* nfld = (NET_TOPUP_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_TOPUP_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Top Up Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_TOPUP_FB_SUB_OK :
				{
					DoModalOuter ( "Top Up Success", MODAL_INFOMATION, OK, OUTER_MODAL_TOPUP );
				}
				break;
			default :
				{
					DoModalOuter ( "Top Up Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		/*security key on charselect, DevArt22*/
		case CHINA_NET_MSG_CHARSELECT_PASS_FB:
		{
			NET_CHARSELECT_PASS_FEEDBACK_DATA2* nfld = (NET_CHARSELECT_PASS_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_CHARSELECT_PASS_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Incorrect Security Key!", MODAL_INFOMATION, OK );
					m_nCharPassAttempt++;
				}
				break;
			case EM_CHARSELECT_PASS_FB_SUB_OK :
				{
					DoModalOuter ( "Security Key Accepted", MODAL_INFOMATION, OK, OUTER_MODAL_CHARSELECT_PASS );
					m_nCharPassAttempt = 0;
				}
				break;
			default :
				{
					DoModalOuter ( "Incorrect Security Key!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		case CHINA_NET_MSG_PIN_FB:
		{
			NET_CPN_FEEDBACK_DATA2* nfld = (NET_CPN_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_PIN_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Pin Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_PIN_FB_SUB_OK :
				{
					DoModalOuter ( "Pin Update Success", MODAL_INFOMATION, OK, OUTER_MODAL_CHANGEPIN );
				}
				break;
			default :
				{
					DoModalOuter ( "Pin Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

		case CHINA_NET_MSG_PASS_FB:
		{
			NET_CP_FEEDBACK_DATA2* nfld = (NET_CP_FEEDBACK_DATA2*) nmg;
			
			switch (nfld->nResult)
			{
			case EM_PASS_FB_SUB_FAIL : 
				{
					DoModalOuter ( "Password Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			case EM_PASS_FB_SUB_OK :
				{
					DoModalOuter ( "Password Update Success", MODAL_INFOMATION, OK, OUTER_MODAL_CHANGEPASS );
				}
				break;
			default :
				{
					DoModalOuter ( "Password Update Error!", MODAL_INFOMATION, OK );
					
				}
				break;
			}
			
		}break;

	case NET_MSG_CHA_DEL_FB_OK:
		{
			HideGroup( MODAL_WINDOW_OUTER, true );

			NET_CHA_DEL_FB* pCharDelFb = (NET_CHA_DEL_FB*) nmg;
			GASSERT( m_pDxLobyStage );
			m_pDxLobyStage->DelChar( pCharDelFb->nChaNum );

			//	캐릭터 카드수 재조정
			SetCharRemainExM( pCharDelFb->nExtremeM );
			SetCharRemainExW( pCharDelFb->nExtremeW );
			SetCharRemain( static_cast<USHORT>(pCharDelFb->wChaRemain) );

			DoModalOuter ( ID2GAMEEXTEXT ( "CHAR_DEL_SUCCEED" ), MODAL_INFOMATION, OK );
		}
		break;

	case NET_MSG_CHA_DEL_FB_CLUB:
		{
			HideGroup( MODAL_WINDOW_OUTER, true );
			DoModalOuter ( ID2GAMEEXTEXT ( "CHAR_DEL_CLUB_FAILED" ), MODAL_INFOMATION, OK );
		}
		break;

	case NET_MSG_CHA_DEL_FB_ERROR:
		{
			HideGroup( MODAL_WINDOW_OUTER, true );
			DoModalOuter ( ID2GAMEEXTEXT ( "CHAR_DEL_FAILED" ), MODAL_INFOMATION, OK );
		}
		break;

	case NET_MSG_CHA_NEW_FB:
		{
			NET_NEW_CHA_FB* pCharNewFb = (NET_NEW_CHA_FB*) nmg;
			//pCharNewFb->nChaNum;

			switch ( pCharNewFb->nResult )
			{
			case EM_CHA_NEW_FB_SUB_OK:	// 캐릭터 생성 성공
				{						
					DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_4" ), MODAL_INFOMATION, OK, OUTER_MODAL_CREATESUCCEED );

					//	캐릭터 카드수 재조정
					SetCharRemainExM( pCharNewFb->nExtremeM );
					SetCharRemainExW( pCharNewFb->nExtremeW );
					SetCharRemain( static_cast<USHORT>(pCharNewFb->wChaRemain) );
				}
				break;		

			case EM_CHA_NEW_FB_SUB_DUP: // 이미 같은 이름의 캐릭터가 있음
				DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_5" ), MODAL_INFOMATION, OK );
				break;

			case EM_CHA_NEW_FB_SUB_LENGTH: // 허용된 캐릭터 이름 길이보다 짧음
				DoModalOuter ( ID2GAMEEXTEXT ( "NEWCHAR_NAME_TOO_SHORT" ), MODAL_INFOMATION, OK );
				break;

			case EM_CHA_NEW_FB_SUB_MAX: // 생성할수 있는 최대 캐릭터수 초과
				DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_6" ), MODAL_INFOMATION, OK );
				break;

			case EM_CHA_NEW_FB_SUB_ERROR: // 시스템에러 또는 허용되지 않는 캐릭터 이름
				DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_7" ), MODAL_INFOMATION, OK );
				break;
			case EM_CHA_NEW_FB_SUB_THAICHAR_ERROR:
				DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_9" ), MODAL_INFOMATION, OK );
				break;
			case EM_CHA_NEW_FB_SUB_VNCHAR_ERROR:
				DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_10" ), MODAL_INFOMATION, OK );
				break;
			case EM_CHA_NEW_FB_SUB_BAD_NAME:
				DoModalOuter ( ID2GAMEEXTEXT ( "CHARACTER_BADNAME" ), MODAL_INFOMATION, OK );
				break;

			case EM_CHA_NEW_FB_SUB_BAD_SCALE:
				DoModalOuter ( ID2GAMEEXTEXT ( "CHARACTER_BADSCALE" ), MODAL_INFOMATION, OK );
				break;
			}

			CreateCharacterSetUseCancel( TRUE );
		}
		break;

	case NET_MSG_REGISTER_ACTION_FB:
		{
			NET_REGISTER_ACTION_FB* nlfb = (NET_REGISTER_ACTION_FB *) nmg;
			switch( nlfb->emFB )
			{
			case EMREGISTER_FB_ERROR_SERVER:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_ERROR_SERVER"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_NOTREG:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_ERROR_NOTREG"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_ONREGWAIT:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_ERROR_ONREGWAIT"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_GOOD_WAITING:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_GOOD_WAITING"), NS_UITEXTCOLOR::GREENYELLOW );
				}break;

			case EMREGISTER_FB_ERROR_LESS_USERID:
			case EMREGISTER_FB_ERROR_MAX_USERID:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_USER_SIZE"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_LESS_PASS1:
			case EMREGISTER_FB_ERROR_MAX_PASS1:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_PASS_SIZE"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_LESS_PASS2:
			case EMREGISTER_FB_ERROR_MAX_PASS2:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_PASS2_SIZE"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_LESS_MAIL:
			case EMREGISTER_FB_ERROR_MAX_MAIL:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_MAIL_SIZE"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_LESS_SA:
			case EMREGISTER_FB_ERROR_MAX_SA:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_SA_SIZE"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_INVALID_USERID:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_USER_STRING"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_INVALID_PASS1:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_PASS_STRING"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_INVALID_PASS2:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_PASS2_STRING"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_INVALID_MAIL:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_MAIL_STRING"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_ERROR_INVALID_SA:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_SA_STRING"), NS_UITEXTCOLOR::RED );
				}break;

			
			case EMREGISTER_FB_GOOD_ERROR:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_RESULT_ERROR"), NS_UITEXTCOLOR::RED );

					CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
					if( pNetClient->IsOnline() == true )
					{
						pNetClient->CloseConnect();
						SetCancelToLogin();
					}

					ToSelectServerPage( REGISTER_PAGE );

					DoModalOuter ( ID2GAMEINTEXT("RAN_REGISTER_FB_RESULT_ERROR"), MODAL_INFOMATION, OK );

				}break;

			case EMREGISTER_FB_GOOD_TAKEN:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_RESULT_TAKEN"), NS_UITEXTCOLOR::RED );
				}break;

			case EMREGISTER_FB_GOOD_DONE:
				{
					GetRegisterPage()->AddErrorString( ID2GAMEINTEXT("RAN_REGISTER_FB_RESULT_DONE"), NS_UITEXTCOLOR::GREENYELLOW );
					GetRegisterPage()->EndRegister();
					DoModalOuter ( ID2GAMEINTEXT("RAN_REGISTER_FB_RESULT_DONE"), MODAL_INFOMATION, OK );
				}break;
			};
		}break;


#ifdef CH_PARAM_USEGAIN //chinaTest%%%
	case NET_MSG_LOBBY_CHINA_ERROR:
		DoModalOuter ( ID2GAMEEXTEXT ( "CHINA_LOGIN_GAINTIME_ERROR" ), MODAL_INFOMATION, OK );
#endif
		break;
	};
}


void COuterInterface::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{	
	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case CHINA_TITLE:
	case COPYRIGHT:
	case LOGIN_PAGE:
	case CHANGEPASS_PAGE:
	case RESETPASS_PAGE:
	case RESETPASS_PAGE2:
	case RESETPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case CHANGEPIN_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:
	case TOPUP_PAGE:
	/*security key on charselect, DevArt22*/
	case CHARSELECT_PASS_PAGE:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case PASS_KEYBOARD:
	case MODAL_WINDOW_OUTER:
	case OUTER_UP_IMAGE:
	case OUTER_DN_IMAGE:
	case CHARACTER_CREATE_PAGE:
	case CHARACTER_CREATE_INFO:
	case CHARACTER_CREATE_ROTATE:
	case REGISTER_PAGE: /*register page, Juver, 2017/11/17 */
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ) SetOnTheInterface ();
		}
		break;

	case SELECT_CHARACTER_PAGE:
		{
			if ( UIMSG_MOUSEIN_SELECT_CHARACTERPAGE & dwMsg ) SetOnTheInterface ();
		}break;
	case SELECT_CHARACTER_USERPANEL:
		{
			if ( UIMSG_MOUSEIN_SELECT_USERPANEL & dwMsg ) SetOnTheInterface ();
		}
		break;

	case FULLSCREEN_OUTER:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )
				{
					const float fBUTTON_CLICK = CBasicButton::fBUTTON_CLICK;

					const UIRECT& rcPos = m_pFullScreenButtonDummy->GetGlobalPos ();				
					m_pFullScreenButton->SetGlobalPos ( D3DXVECTOR2 ( rcPos.left + fBUTTON_CLICK, rcPos.top + fBUTTON_CLICK ) );
				}

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{					
					DxGlobalStage::GetInstance().ChangeWndMode ();
				}
			}
		}
		break;
	}

	switch ( ControlID )
	{
	case MODAL_WINDOW_OUTER:
		{			
			if ( m_pModalWindow )
			{
				UIGUID CallerID = m_pModalWindow->GetCallerID ();
				ModalMsgProcess ( CallerID, dwMsg );
			}
		}
		break;		
	}
}
