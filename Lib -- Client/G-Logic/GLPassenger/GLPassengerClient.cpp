#include "StdAfx.h"

#include "GLogicData.h"
#include "GLItemMan.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"

#include "../Lib -- ClientUI/Interface/UITextControl.h"
#include "../Lib -- ClientUI/Interface/GameTextControl.h"
#include "../Lib -- ClientUI/Interface/InnerInterface.h"

#include "GLPassengerclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPassengerClient& GLPassengerClient::GetInstance()
{
	static GLPassengerClient Instance;
	return Instance;
}

GLPassengerClient::GLPassengerClient(void) :
	m_bPassenger(FALSE)
{
}

GLPassengerClient::~GLPassengerClient(void)
{
}

BOOL GLPassengerClient::Start (  DWORD dwDriverID , DWORD dwPassengerID )
{
	Reset ();

	if (  GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID == dwPassengerID )
	{
		PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetActiveMap()->GetChar(dwDriverID);
		if ( !pChar )	return FALSE;
	}

	if (  GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID == dwDriverID )
	{
		PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetActiveMap()->GetChar(dwPassengerID);
		if ( !pChar )	return FALSE;
	}

	m_sTarPassenger.SetPassenger ( dwPassengerID );
	m_sTarPassenger.SetDriver ( dwDriverID );

	m_sMyPassenger.SetDriver ( dwDriverID );
	m_sMyPassenger.SetPassenger ( dwPassengerID );

	return TRUE;
}

BOOL GLPassengerClient::Reset ()
{
	m_sMyPassenger.Reset();
	m_sTarPassenger.Reset();

	return TRUE;
}

BOOL GLPassengerClient::Valid ()
{
	return (m_sMyPassenger.Valid() && m_sTarPassenger.Valid());
}

BOOL GLPassengerClient::MsgPassengerFB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_PASSENGER_FB *pNetMsg = (GLMSG::SNET_PASSENGER_FB *) nmg;
	
	switch ( pNetMsg->emAns )
	{
	case EMPASSENGER_OK:
		{
			m_bPassenger = FALSE;
			BOOL bOk = Start ( pNetMsg->dwDriverID , pNetMsg->dwPassengerID );
			if ( !bOk )
			{
				//	Note : 거래가 시작 되지 못했습니다.
				GLMSG::SNET_PASSENGER_CANCEL NetMsg;
				NETSENDTOFIELD ( (NET_MSG_GENERIC*) &NetMsg );
				break;
			}

			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();

			CString strName;
			PGLCHARCLIENT pChar = pLand->GetChar ( pNetMsg->dwDriverID );
			if ( pChar )	strName = pChar->GetName();

			if ( pNetMsg->dwPassengerID == GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID )
			{
				m_bPassenger = TRUE;
				GLGaeaClient::GetInstance().GetCharacter()->ReqTogglePeaceMode();
				GLGaeaClient::GetInstance().GetCharacter()->SetSTATE(EM_ACT_PEACEMODE);
			}
		}
		break;

	case EMPASSENGER_CANCEL:
		//	상대방이 거래를 거절 하였습니다.
		if ( IsPassenger() )
		{
			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			PGLCHARCLIENT pChar = pLand->GetChar ( pNetMsg->dwPassengerID );
			if ( !pChar )	return FALSE;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PASSENGER_FB_CANCEL"), pChar->GetName() );
			GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.SetActiveValue( false );
			GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.RESET();
			GLGaeaClient::GetInstance().GetCharacter()->ReqVehicleUpdate();
			GLGaeaClient::GetInstance().GetCharacter()->ReqVehicleInvenUpdate();
		}
		else
		{
			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			PGLCHARCLIENT pChar = pLand->GetChar ( pNetMsg->dwDriverID );
			if ( !pChar )	return FALSE;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PASSENGER_FB_CANCEL"), pChar->GetName() );
		}
		break;
	};

	return TRUE;
}

BOOL GLPassengerClient::MsgPassengerCancelTar ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_PASSENGER_CANCEL_TAR *pNetMsg = (GLMSG::SNET_PASSENGER_CANCEL_TAR *) nmg;

	switch ( pNetMsg->emCancel )
	{
	case EMPASSENGER_CANCEL_NORMAL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PASSENGER_CANCEL_NORMAL") );
		break;

	case EMPASSENGER_CANCEL_OTHER:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PASSENGER_CANCEL_OTHER") );
		break;
	};

	if ( IsPassenger() )
	{
		GLGaeaClient::GetInstance().GetCharacter()->m_bPassenger = false;
		GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.SetActiveValue( false );
		GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.RESET();
		GLGaeaClient::GetInstance().GetCharacter()->ReqVehicleUpdate();
		GLGaeaClient::GetInstance().GetCharacter()->ReqVehicleInvenUpdate();
		
	}

	Reset ();

	return TRUE;
}

void GLPassengerClient::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_PASSENGER_FB:
		MsgPassengerFB(nmg);
		break;

	case NET_MSG_GCTRL_PASSENGER_CANCEL_TAR:
		MsgPassengerCancelTar(nmg);
		break;
	};
}

HRESULT GLPassengerClient::FrameMove ( float fTime, float fElapsedTime )
{
	if ( !Valid() )		return S_FALSE;

	m_sMyPassenger.FrameMove ( fTime, fElapsedTime );
	m_sTarPassenger.FrameMove ( fTime, fElapsedTime );

	return S_OK;
}