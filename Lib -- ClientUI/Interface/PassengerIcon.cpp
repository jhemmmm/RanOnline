#include "StdAfx.h"
#include "../Lib -- ClientUI/Interface/BasicButton.h"
#include "../Lib -- ClientUI/Interface/BasicProgressBar.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicLineBoxEx.h"
#include "InnerInterface.h"
#include "PassengerIcon.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPassenger::CPassenger()
	: m_pImage( NULL )
{
}

CPassenger::~CPassenger()
{
}

void CPassenger::CreateSubControl ()
{
	m_pImage =  new CBasicButton;
	m_pImage->CreateSub ( this, "PASSENGER_ICON_IMAGE", UI_FLAG_DEFAULT, MouseOpen );	
	m_pImage->SetUseGlobalAction ( TRUE );
	m_pImage->SetVisibleSingle(FALSE);
	RegisterControl ( m_pImage );

}
void CPassenger::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter ) return;

	bool bBool = pCharacter->m_bPassenger;

	if ( bBool )
		m_pImage->SetVisibleSingle(bBool);
	else
		m_pImage->SetVisibleSingle(bBool);

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPassenger::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case MouseOpen:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEINTEXT( "CLICK_TO_DROP_OFF" ), NS_UITEXTCOLOR::WHITE  );					
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( GLPassengerClient::GetInstance().Valid() )
					{
						GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
						if ( !pCharacter ) return;
						bool bBool = pCharacter->m_bPassenger;
						bool m_bPassenger = GLPassengerClient::GetInstance().IsPassenger();
						if ( m_bPassenger && bBool )
						{
							GLMSG::SNET_PASSENGER_CANCEL NetMsg;
							NETSENDTOFIELD ( (NET_MSG_GENERIC*) &NetMsg );
						}
					}
				}
			}
			
		}
		break;
	}
}