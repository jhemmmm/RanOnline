#include "stdafx.h"
#include "s_CAgentServer.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//set register flag to client 
void CAgentServer::RegisterInit( MSG_LIST* pMsg )
{
	if ( pMsg == NULL) return;	

	if (m_pClientManager->IsAccountPassing( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_INIT IsAccountPassing()") );
		return;
	}
	

	NET_REGISTER_INIT* pNmr = NULL;
	DWORD dwClient   = 0;

	dwClient = pMsg->dwClient;

	pNmr = reinterpret_cast<NET_REGISTER_INIT *> (pMsg->Buffer);

	if (sizeof(NET_REGISTER_INIT) != pNmr->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_INIT Wrong Message Size") );
		return;
	}

	int nChannel = pNmr->nChannel;

	if ( nChannel < 0 || nChannel >= m_nServerChannelNumber)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_INIT Invalid Channel Number (Channel:%d)"), nChannel );
		return;
	}


	m_pClientManager->SetRegister( dwClient, pNmr->bInit );

	//this could happen if player close the registration page and player is in the process of registration
	if ( m_pClientManager->IsRegisterWait( dwClient ) )
	{
		m_pClientManager->SetRegisterWait( dwClient, FALSE );
	}
}

void CAgentServer::RegisterAction( MSG_LIST* pMsg )
{
	if ( !RANPARAM::bFeatureRegister )	return;

	NET_REGISTER_ACTION_FB nmfb;

	if ( pMsg == NULL) 
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_SERVER;
		SendClient(pMsg->dwClient, &nmfb);
		return;	
	}
	
	if (m_pClientManager->IsAccountPassing( pMsg->dwClient ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_ACTION IsAccountPassing()") );
		return;
	}

	if ( !m_pClientManager->IsRegister( pMsg->dwClient ) )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_NOTREG;
		SendClient(pMsg->dwClient, &nmfb);
		return;
	}

	if ( m_pClientManager->IsRegisterWait( pMsg->dwClient ) )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_ONREGWAIT;
		SendClient(pMsg->dwClient, &nmfb);
		return;
	}

	NET_REGISTER_ACTION* pNmr = NULL;
	
	DWORD dwClient   = 0;

	dwClient = pMsg->dwClient;

	pNmr = reinterpret_cast<NET_REGISTER_ACTION *> (pMsg->Buffer);

	if (sizeof(NET_REGISTER_ACTION) != pNmr->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_ACTION Wrong Message Size") );
		nmfb.emFB = EMREGISTER_FB_ERROR_SERVER;
		SendClient(dwClient, &nmfb);
		return;
	}

	int nChannel = pNmr->nChannel;

	if ( nChannel < 0 || nChannel >= m_nServerChannelNumber)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::REGISTER_ACTION Invalid Channel Number (Channel:%d)"), nChannel );
		nmfb.emFB = EMREGISTER_FB_ERROR_SERVER;
		SendClient(dwClient, &nmfb);
		return;
	}

	m_Tea.decrypt( pNmr->szUser, USR_ID_LENGTH ); 
	m_Tea.decrypt( pNmr->szMail, USR_INFOMAIL_LENGTH ); 
	m_Tea.decrypt( pNmr->szSA, USR_PASS_LENGTH ); 

	TCHAR szUserid[USR_ID_LENGTH] = {0};
	TCHAR szPass[USR_PASS_LENGTH] = {0};
	TCHAR szPass2[USR_PASS_LENGTH] = {0};
	TCHAR szSA[USR_PASS_LENGTH] = {0};
	TCHAR szMail[USR_INFOMAIL_LENGTH] = {0};
	
	StringCchCopy( szUserid, USR_ID_LENGTH, pNmr->szUser );
	StringCchCopy( szPass, USR_PASS_LENGTH, pNmr->szPass );
	StringCchCopy( szPass2, USR_PASS_LENGTH, pNmr->szPass2 );
	StringCchCopy( szSA, USR_PASS_LENGTH, pNmr->szSA );
	StringCchCopy( szMail, USR_INFOMAIL_LENGTH, pNmr->szMail );

	if ( strlen( szUserid ) < USR_INFO_MIN )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_LESS_USERID;
		nmfb.nERRORVAR = USR_INFO_MIN;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szPass ) < USR_INFO_MIN )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_LESS_PASS1;
		nmfb.nERRORVAR = USR_INFO_MIN;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szPass2 ) < USR_INFO_MIN )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_LESS_PASS2;
		nmfb.nERRORVAR = USR_INFO_MIN;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szSA ) < USR_INFO_MIN )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_LESS_PASS1;
		nmfb.nERRORVAR = USR_INFO_MIN;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szMail ) < USR_INFO_MIN )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_LESS_MAIL;
		nmfb.nERRORVAR = USR_INFO_MIN;
		SendClient(dwClient, &nmfb);
		return;
	}


	if ( strlen( szUserid ) > USR_ID_LENGTH )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_MAX_USERID;
		nmfb.nERRORVAR = USR_ID_LENGTH;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szPass ) > USR_PASS_LENGTH )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_MAX_PASS1;
		nmfb.nERRORVAR = USR_PASS_LENGTH;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szPass2 ) > USR_PASS_LENGTH )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_MAX_PASS2;
		nmfb.nERRORVAR = USR_PASS_LENGTH;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szSA ) > USR_PASS_LENGTH )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_MAX_PASS1;
		nmfb.nERRORVAR = USR_PASS_LENGTH;
		SendClient(dwClient, &nmfb);
		return;
	}

	if ( strlen( szMail ) > USR_INFOMAIL_LENGTH )
	{
		nmfb.emFB = EMREGISTER_FB_ERROR_MAX_MAIL;
		nmfb.nERRORVAR = USR_INFOMAIL_LENGTH;
		SendClient(dwClient, &nmfb);
		return;
	}

	
	{
		CString str = szUserid;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMREGISTER_FB_ERROR_INVALID_USERID;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	{
		CString str = szPass;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMREGISTER_FB_ERROR_INVALID_PASS1;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	{
		CString str = szPass2;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMREGISTER_FB_ERROR_INVALID_PASS2;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	{
		CString str = szSA;
		str.Trim();
		if ( STRUTIL::CheckString( str.GetString() ) )
		{
			nmfb.emFB = EMREGISTER_FB_ERROR_INVALID_SA;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	
	{
		CString str = szMail;
		str.Trim();
		if ( STRUTIL::CheckString_Special2( str.GetString() ) )
		{
			nmfb.emFB = EMREGISTER_FB_ERROR_INVALID_MAIL;
			SendClient(dwClient, &nmfb);
			return;
		}
	}

	m_pClientManager->SetRegisterWait( dwClient, true );
	m_pClientManager->SetUserID( dwClient, szUserid );

	nmfb.emFB = EMREGISTER_FB_GOOD_WAITING;
	SendClient(dwClient, &nmfb);

	CAgentUserRegister* pAction = new CAgentUserRegister( 
		szUserid,
		szPass, 
		szPass2,
		szSA, 
		szMail,
		m_pClientManager->GetClientIP( dwClient ),
		m_nServerGroup,
		m_nServerNum,
		0,
		dwClient );
	COdbcManager::GetInstance()->AddUserJob((CDbAction*) pAction);

	CDebugSet::ToFileWithTime( "_userreg.txt", "Client ID:%d User:%s Pass:%s Pass2:%s SA:%s Email:%s IP:%s", 
		dwClient, szUserid, szPass, szPass2, szSA, szMail, m_pClientManager->GetClientIP( dwClient ) );
}

void CAgentServer::RegisterFeedback( NET_REGISTER_ACTION_FB2* fb2 )
{
	NET_REGISTER_ACTION_FB nmfb;
	if ( !fb2 )
		return;

	DWORD dwCLIENTID = fb2->nClient;
	BOOL bERROR = FALSE;

	if ( m_pClientManager->IsAccountPassing( dwCLIENTID ) == true)
	{
		CConsoleMessage::GetInstance()->Write( _T("ERROR::RegisterFeedback IsAccountPassing()") );
		bERROR = TRUE;
	}

	if ( !m_pClientManager->IsRegister( dwCLIENTID ) )
		bERROR = TRUE;

	if ( !m_pClientManager->IsRegisterWait(dwCLIENTID ) )
		bERROR = TRUE;

	BOOL bOK = ( (m_pClientManager->IsOnline( dwCLIENTID ) == true) && 
		(strcmp( m_pClientManager->GetClientIP( dwCLIENTID ), fb2->szIp ) == 0) && 
		(strcmp( m_pClientManager->GetUserID( dwCLIENTID ),   fb2->szUserid ) == 0) );

	if ( !bOK )
		bERROR = TRUE;

	if( bERROR )
	{
		m_pClientManager->SetRegisterWait( dwCLIENTID, FALSE );
		m_pClientManager->ResetUserID( dwCLIENTID );
		m_pClientManager->SetRegister( dwCLIENTID, FALSE );
		nmfb.emFB = EMREGISTER_FB_GOOD_ERROR;
		SendClient( dwCLIENTID, &nmfb);
		return;
	}

	switch( fb2->emFB )
	{
	case EMREGISTER_FB2_OK:
		{
			nmfb.emFB = EMREGISTER_FB_GOOD_DONE;
			SendClient( dwCLIENTID, &nmfb);
		}break;
	case EMREGISTER_FB2_TAKEN:
		{
			nmfb.emFB = EMREGISTER_FB_GOOD_TAKEN;
			SendClient( dwCLIENTID, &nmfb);
		}break;
	case EMREGISTER_FB2_ERROR:
		{	
			m_pClientManager->SetRegister( dwCLIENTID, FALSE );
			nmfb.emFB = EMREGISTER_FB_GOOD_ERROR;
			SendClient( dwCLIENTID, &nmfb);
		}break;
	};	

	m_pClientManager->SetRegisterWait( dwCLIENTID, FALSE );
	m_pClientManager->ResetUserID( dwCLIENTID );
}

void CAgentServer::MsgChangePass( MSG_LIST* pMsg )
{

	CHINA_NET_CP_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_CP_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_CP_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_CP_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szPassword, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szOPassword, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	TCHAR szOPassword[USR_PASS_LENGTH] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;

	
	dwClient = pMsg->dwClient;

	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szOPassword, USR_PASS_LENGTH, pNml->szOPassword );
	StringCchCopy( szPassword, USR_PASS_LENGTH, pNml->szPassword );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserChangePassword* pAction = new CAgentUserChangePassword(
		szUserid,
		szOPassword,
		szPassword,
		szPassword2,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgChangePassBack (NET_CP_FEEDBACK_DATA2* nlfd2)
{
	NET_CP_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_PASS_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_PASS_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_PASS_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}



void CAgentServer::MsgChangePin( MSG_LIST* pMsg )
{

	CHINA_NET_CPN_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_CPN_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_CPN_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_CPN_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szOPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPassword, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szOPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;

	
	dwClient = pMsg->dwClient;

	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szOPassword2, USR_PASS_LENGTH, pNml->szOPassword2 );
	StringCchCopy( szPassword, USR_PASS_LENGTH, pNml->szPassword );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserChangePin* pAction = new CAgentUserChangePin(
		szUserid, 
		szOPassword2,
		szPassword,
		szPassword2,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgChangePinBack (NET_CPN_FEEDBACK_DATA2* nlfd2)
{
	NET_CPN_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_PIN_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_PIN_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_PIN_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}




void CAgentServer::MsgChangeEmail( MSG_LIST* pMsg )
{

	CHINA_NET_CE_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_CE_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_CE_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_CE_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szPassword, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );
	m_Tea.decrypt( pNml->szUserEmail, USR_EMAIL+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	TCHAR szUserEmail[USR_EMAIL+1] = {0};
	DWORD dwClient;

	
	dwClient = pMsg->dwClient;

	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szPassword, USR_PASS_LENGTH, pNml->szPassword );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	StringCchCopy( szUserEmail, USR_EMAIL+1, pNml->szUserEmail );
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserChangeEmail* pAction = new CAgentUserChangeEmail(
		szUserid, 
		szPassword,
		szPassword2,
		szUserEmail,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgChangeEmailBack (NET_CE_FEEDBACK_DATA2* nlfd2)
{
	NET_CE_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_EMAIL_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_EMAIL_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_EMAIL_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}



void CAgentServer::MsgTopUp( MSG_LIST* pMsg )
{

	CHINA_NET_TOPUP_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_TOPUP_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_TOPUP_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_TOPUP_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szCode, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPin, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szCode[USR_PASS_LENGTH] = {0};
	TCHAR szPin[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;

	
	dwClient = pMsg->dwClient;

	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szCode, USR_PASS_LENGTH, pNml->szCode );
	StringCchCopy( szPin, USR_PASS_LENGTH, pNml->szPin );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	
	CAgentUserTopUp* pAction = new CAgentUserTopUp(
		szUserid, 
		szCode,
		szPin,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgTopUpBack (NET_TOPUP_FEEDBACK_DATA2* nlfd2)
{
	NET_TOPUP_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_TOPUP_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_TOPUP_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_TOPUP_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}



void CAgentServer::MsgGameTimeCVT( MSG_LIST* pMsg )
{

	CHINA_NET_GTCVT_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_GTCVT_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_GTCVT_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_GTCVT_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	DWORD dwGameTime;
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;
	
	dwClient = pMsg->dwClient;
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	dwGameTime = (DWORD)m_pClientManager->GetChinaGameTime(pMsg->dwClient);
	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	
	CAgentUserGameTimeCVT* pAction = new CAgentUserGameTimeCVT(
		szUserid, 
		szCaptcha,
		dwGameTime,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgGameTimeCVTBack (NET_GTCVT_FEEDBACK_DATA2* nlfd2)
{
	NET_GTCVT_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_GT2VP_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_GT2VP_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_GT2VP_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}
//
void CAgentServer::MsgPremiumPointCVT( MSG_LIST* pMsg )
{

	CHINA_NET_PPCVT_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_PPCVT_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_PPCVT_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_PPCVT_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );
	m_Tea.decrypt( pNml->szValue,  USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szValue[USR_CAPTCHA+1] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;
	
	dwClient = pMsg->dwClient;
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	StringCchCopy( szValue, USR_ID_LENGTH+1,  pNml->szValue );
	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	
	CAgentUserPremiumPointCVT* pAction = new CAgentUserPremiumPointCVT(
		szUserid, 
		szValue,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}

void CAgentServer::MsgPremiumPointCVTBack (NET_PPCVT_FEEDBACK_DATA2* nlfd2)
{
	NET_PPCVT_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_PP2VP_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_PP2VP_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_PP2VP_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}
void CAgentServer::MsgResetPin ( MSG_LIST* pMsg )
{

	CHINA_NET_RPN_DATA* pNml = NULL;
	
	pNml = reinterpret_cast<CHINA_NET_RPN_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_RPN_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_RPN_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szUserid, USR_ID_LENGTH+1 );
	m_Tea.decrypt( pNml->szUserEmail, USR_EMAIL+1 );
	m_Tea.decrypt( pNml->szPassword, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szPassword2Hold, USR_PASS_LENGTH );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szUserEmail[USR_EMAIL+1] = {0};
	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szPassword2Hold[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;

	dwClient = pMsg->dwClient;
	
	
	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szPassword, USR_PASS_LENGTH, pNml->szPassword );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szPassword2Hold, USR_PASS_LENGTH, pNml->szPassword2Hold );
	StringCchCopy( szUserEmail, USR_EMAIL+1, pNml->szUserEmail );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
		
	
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserResetPin* pAction = new CAgentUserResetPin(
		szUserid,
		szUserEmail,
		szPassword,
		szPassword2,
		szPassword2Hold,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );

}
void CAgentServer::MsgResetPinBack ( NET_RPN_FEEDBACK_DATA2* nlfd2 )
{
	NET_RPN_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_RESETPIN_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_PASS_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		StringCchCopy(nlfd.szUserpass2Hold,USR_ID_LENGTH+6, nlfd2->szUserpass2Hold); // ip
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_PASS_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}
void CAgentServer::MsgResetPass ( MSG_LIST* pMsg )
{
	CHINA_NET_RPS_DATA* pNml = NULL;
	
	if (m_pClientManager->IsAccountPassing( pMsg->dwClient ) == true)
	{
		return;
	}
	else // 인증중으로 세팅
	{
		m_pClientManager->SetAccountPassing( pMsg->dwClient, true );
	}

	pNml = reinterpret_cast<CHINA_NET_RPS_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_RPS_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_RPS_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szUserid, USR_ID_LENGTH+1 );
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	DWORD dwClient;

	dwClient = pMsg->dwClient;

	StringCchCopy( szUserid, USR_ID_LENGTH+1, pNml->szUserid );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );
	m_pClientManager->SetUserID( dwClient, szUserid );
	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserResetPassword* pAction = new CAgentUserResetPassword(
		szUserid,
		szPassword2,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );
}
void CAgentServer::MsgResetPassBack ( NET_RPS_FEEDBACK_DATA2* nlfd2 )
{
	NET_RPS_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_RESETPASS_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_PASS_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		StringCchCopy(nlfd.szUserid,USR_ID_LENGTH+6, nlfd2->szUserid); // ip
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_PASS_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}
void CAgentServer::MsgResetPass2 ( MSG_LIST* pMsg )
{
	CHINA_NET_RPS2_DATA* pNml = NULL;

	pNml = reinterpret_cast<CHINA_NET_RPS2_DATA *> (pMsg->Buffer);

	if (sizeof(CHINA_NET_RPS2_DATA) != pNml->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			
			_T("ERROR:CHINA_NET_RPS2_DATA Wrong Message Size") );
		return;
	}
	// 복호화
	m_Tea.decrypt( pNml->szPassword2, USR_PASS_LENGTH );
	m_Tea.decrypt( pNml->szCaptcha, USR_CAPTCHA+1 );

	TCHAR szCaptcha[USR_CAPTCHA+1] = {0};
	TCHAR szPassword[USR_PASS_LENGTH] = {0};
	TCHAR szUserid[USR_ID_LENGTH+1] = {0};
	TCHAR szPassword2[USR_PASS_LENGTH] = {0};
	DWORD dwClient;

	dwClient = pMsg->dwClient;

	StringCchCopy( szUserid, USR_ID_LENGTH+1, m_pClientManager->GetUserID(pMsg->dwClient) );
	StringCchCopy( szPassword, USR_PASS_LENGTH, pNml->szPassword );
	StringCchCopy( szPassword2, USR_PASS_LENGTH, pNml->szPassword2 );
	StringCchCopy( szCaptcha, USR_CAPTCHA+1, pNml->szCaptcha );

	m_pClientManager->SetAccountPass( pMsg->dwClient, false );
	CAgentUserResetPassword2* pAction = new CAgentUserResetPassword2(
		szUserid,
		szPassword,
		szPassword2,
		szCaptcha,
		dwClient);
	COdbcManager::GetInstance()->AddUserJob( (CDbAction*) pAction );
}

void CAgentServer::MsgResetPassBack2 ( NET_RPS2_FEEDBACK_DATA2* nlfd2 )
{
	NET_RPS2_FEEDBACK_DATA2		nlfd;

	nlfd.nmg.nType = CHINA_NET_MSG_RESETPASS2_FB;
	
	DWORD	dwClient   = (DWORD) nlfd2->nClient;
	DWORD	dwSndBytes = 0;
	int		nUserNum   = 0;		

	if ( nlfd2->nResult == EM_RESETPASS_FB_SUB_OK )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}else if ( nlfd2->nResult == EM_RESETPASS_FB_SUB_FAIL )
	{
		nlfd.nResult   = nlfd2->nResult;
		if ( m_pClientManager->IsOnline( dwClient ) == true )
		{
		
			SendClient(dwClient, &nlfd);
	
		}
	}
}

	