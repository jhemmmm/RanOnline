#include "StdAfx.h"
#include "BasicButton.h"
#include "BasicProgressBar.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicLineBoxEx.h"
#include "InnerInterface.h"
#include "../Lib -- ClientUI/Interface/IngameItemMall/ItemMallIcon.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CItemMallIcon::CItemMallIcon()
	: m_pImage( NULL )
{
}

CItemMallIcon::~CItemMallIcon()
{
}

void CItemMallIcon::CreateSubControl ()
{
	m_pImage =  new CBasicButton;
	m_pImage->CreateSub ( this, "ITEM_MALLICON_IMAGE", UI_FLAG_DEFAULT, MouseOpen );	
	m_pImage->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pImage );
	//m_pImage->SetVisibleSingle(FALSE);

}

void	CItemMallIcon::SetItemMallIcon() 
{
	m_pImage->SetVisibleSingle( TRUE );
}

void CItemMallIcon::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
switch ( ControlID )
	{
	case MouseOpen:
		{
			
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					{
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Item Mall", NS_UITEXTCOLOR::WHITE  );				
					}
					if ( dwMsg & UIMSG_LB_UP )
					{
						if ( !CInnerInterface::GetInstance().IsVisibleGroup ( ITEMSHOP_WINDOW ) )
						{
							CInnerInterface::GetInstance().SetItemShopWindowOpen ();
							
						}
						else
						{
							CInnerInterface::GetInstance().SetItemShopWindowOpen ();
						}
					}
				}
			
		}
		break;
	}
}