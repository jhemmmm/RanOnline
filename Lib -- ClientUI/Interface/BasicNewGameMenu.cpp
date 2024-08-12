#include "StdAfx.h"
#include "BasicNewGameMenu.h"
#include "BasicTextButton.h"
#include "InnerInterface.h"
#include "BasicVarTextBox.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "RANPARAM.h"
#include "ModalWindow.h"
#include "GameTextControl.h"
#include "BasicLineBoxEx.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/GUInterface/BasicButtonText.h"
#include "../Lib_ClientUI/Interface/BasicButton.h"
#include "../Lib_ClientUI/Interface/BasicTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicNewGameMenu::CBasicNewGameMenu ()
	: m_pInvenButton(NULL),
	  m_pCharButton(NULL),
	  m_pSkillButton(NULL),
	  m_pPartyButton(NULL),
	  m_pClubButton(NULL),
	  m_pQuestButton(NULL),
	  m_pFriendButton(NULL),
	  m_pMapButton(NULL),
	  m_pMacroButton(NULL),
	  m_pCompunButton(NULL),
	  m_pTyrannyButton(NULL),
	  m_pESCButton(NULL),
	  m_pInfo(NULL),
	  m_nOverMenu( NO_ID )
{
}

CBasicNewGameMenu::~CBasicNewGameMenu ()
{
}

CBasicButtonText* CBasicNewGameMenu::CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonBlue ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

void CBasicNewGameMenu::CreateSubControl ()
{
	{
	InitShotCutString();
		//{
		//	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
		//	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_SKILL_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//	pLineBox->CreateBaseBoxDialogueNewBase ( "GAMEMENU_BACK" );
		//	RegisterControl ( pLineBox );
		//}

		CBasicButton* pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_INVENTORY", UI_FLAG_DEFAULT, MENU_INVENTORY );
		pButton->CreateFlip ( "MENU_INVENTORY_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_CHARACTER", UI_FLAG_DEFAULT, MENU_CHARACTER );
		pButton->CreateFlip ( "MENU_CHARACTER_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_SKILL", UI_FLAG_DEFAULT, MENU_SKILL );
		pButton->CreateFlip ( "MENU_SKILL_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_PARTY", UI_FLAG_DEFAULT, MENU_PARTY );
		pButton->CreateFlip ( "MENU_PARTY_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_GUILD", UI_FLAG_DEFAULT, MENU_CLUB );
		pButton->CreateFlip ( "MENU_GUILD_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_QUEST", UI_FLAG_DEFAULT, MENU_QUEST );
		pButton->CreateFlip ( "MENU_QUEST_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_FRIEND", UI_FLAG_DEFAULT, MENU_FRIEND );
		pButton->CreateFlip ( "MENU_FRIEND_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_MAP", UI_FLAG_DEFAULT, MENU_MAP );
		pButton->CreateFlip ( "MENU_MAP_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_CHATMACRO", UI_FLAG_DEFAULT, MENU_CHATMACRO );
		pButton->CreateFlip ( "MENU_CHATMACRO_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_COMBINATION", UI_FLAG_DEFAULT, MENU_COMBINATION );
		pButton->CreateFlip ( "MENU_COMBINATION_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_TYRANNY", UI_FLAG_DEFAULT, MENU_TYRANNY );
		pButton->CreateFlip ( "MENU_TYRANNY_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );
	
		pButton = new CBasicButton;
		pButton->CreateSub ( this, "MENU_ESC", UI_FLAG_DEFAULT, MENU_ESC );
		pButton->CreateFlip ( "MENU_ESC_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );

		m_pInfo = new CBasicVarTextBox;
		m_pInfo->CreateSub ( this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfo->CreateSubControl ();
		m_pInfo->SetMousePointGap ( D3DXVECTOR2(-40.0f,-40.0f) );
		m_pInfo->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfo );
	}

	{
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

		m_pInvenButton = CreateButtonBlue ( "MENU_INVENTORY_BUTTON", "MENU_INVENTORY_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_INVENTORY_BUTTON, "Inventory (I)" );	
		m_pInvenButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pInvenButton );
	
		m_pCharButton = CreateButtonBlue ( "MENU_CHARACTER_BUTTON", "MENU_CHARACTER_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_CHARACTER_BUTTON, "Character (C)" );	
		m_pCharButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pCharButton );

		m_pSkillButton = CreateButtonBlue ( "MENU_SKILL_BUTTON", "MENU_SKILL_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_SKILL_BUTTON, "Skill (K)" );	
		m_pSkillButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pSkillButton );

		m_pPartyButton = CreateButtonBlue ( "MENU_PARTY_BUTTON", "MENU_PARTY_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_PARTY_BUTTON, "Party (P)" );	
		m_pPartyButton->SetUseGlobalAction ( TRUE );
		RegisterControl (m_pPartyButton );

		m_pClubButton = CreateButtonBlue ( "MENU_CLUB_BUTTON", "MENU_CLUB_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_CLUB_BUTTON, "Club (G)" );	
		m_pClubButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pClubButton );

		m_pQuestButton = CreateButtonBlue ( "MENU_QUEST_BUTTON", "MENU_QUEST_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_QUEST_BUTTON, "Quest (T)" );	
		m_pQuestButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pQuestButton );

		m_pFriendButton = CreateButtonBlue ( "MENU_FRIEND_BUTTON", "MENU_FRIEND_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_FRIEND_BUTTON, "Friend (F)" );	
		m_pFriendButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pFriendButton );

		m_pMapButton = CreateButtonBlue ( "MENU_MAP_BUTTON", "MENU_MAP_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_MAP_BUTTON, "Map (M)" );	
		m_pMapButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pMapButton );

		m_pMacroButton = CreateButtonBlue ( "MENU_CHATMACRO_BUTTON", "MENU_CHATMACRO_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_CHATMACRO_BUTTON, "Chat Macro (J)" );	
		m_pMacroButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pMacroButton );

		m_pCompunButton = CreateButtonBlue ( "MENU_COMBINATION_BUTTON", "MENU_COMBINATION_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_COMBINATION_BUTTON, "Item Combination (;)" );	
		m_pCompunButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pCompunButton );

		m_pTyrannyButton = CreateButtonBlue ( "MENU_TYRANNY_BUTTON", "MENU_TYRANNY_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_TYRANNY_BUTTON, "BattleField UI (U)" );		
		m_pTyrannyButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pTyrannyButton );

		m_pESCButton = CreateButtonBlue ( "MENU_ESC_BUTTON", "MENU_ESC_BUTTON_TEXT", pFont, nAlignCenterBoth, MENU_ESC_BUTTON, "Game Setting (ESC)" );		
		m_pESCButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( m_pESCButton );
	}	
}

void CBasicNewGameMenu::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	m_pInfo->SetVisibleSingle( FALSE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CBasicNewGameMenu::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case MENU_INVENTORY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[0], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( INVENTORY_WINDOW )
						&&  !CInnerInterface::GetInstance().IsVisibleGroup ( TRADEINVENTORY_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( INVENTORY_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( INVENTORY_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_CHARACTER_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[1], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CHARACTER_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( CHARACTER_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( CHARACTER_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_SKILL_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[2], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( SKILL_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( SKILL_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( SKILL_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_PARTY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[3], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( PARTY_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( PARTY_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( PARTY_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_QUEST_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				//if ( m_nOverMenu != ControlID )
				//{
				//	m_pInfo->SetTextNoSplit ( m_ShotcutText[4], NS_UITEXTCOLOR::SILVER );
				//}
				//m_pInfo->SetVisibleSingle ( FALSE );
				//m_nOverMenu = ControlID;

				//if ( dwMsg & UIMSG_LB_UP )
				//{
				//	if ( !CInnerInterface::GetInstance().IsVisibleGroup ( QUEST_WINDOW ) )
				//	{
				//		DWORD dwEventQuestID = CInnerInterface::GetInstance().GetEventQuestID ();
				//		if ( NATIVEID_NULL().dwID == dwEventQuestID )
				//		{
				//			CInnerInterface::GetInstance().ShowGroupFocus ( QUEST_WINDOW );
				//			CInnerInterface::GetInstance().REFRESH_QUEST_WINDOW ();							
				//		}
				//		else
				//		{
				//			CInnerInterface::GetInstance().SetQuestWindowOpen ( dwEventQuestID );
				//		}
				//	}
				//	else
				//	{
				//		CInnerInterface::GetInstance().HideGroup ( QUEST_WINDOW );
				//	}
				//}
			}
		}
		break;

	case MENU_CLUB_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[5], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CLUB_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( CLUB_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( CLUB_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_FRIEND_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[6], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( FRIEND_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( FRIEND_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( FRIEND_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_MAP_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[7], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( LARGEMAP_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( LARGEMAP_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( LARGEMAP_WINDOW );
					}
				}
			}
		}
		break;
	case MENU_CHATMACRO_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[11], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CHATMACRO_WINDOW ) )
					{
						CInnerInterface::GetInstance().GetChatMacro();
						CInnerInterface::GetInstance().ShowGroupFocus ( CHATMACRO_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( CHATMACRO_WINDOW );
					}
				}
			}
		}
		break;

	case MENU_COMBINATION_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[8], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

			/*	if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( NPCSHOP_BUTTON ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( NPCSHOP_BUTTON );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( NPCSHOP_BUTTON );
					}
				}*/
			}
		}
		break;
		
	case MENU_TYRANNY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( m_ShotcutText[10], NS_UITEXTCOLOR::SILVER );
				}
				m_pInfo->SetVisibleSingle ( FALSE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( COMPETITION_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( COMPETITION_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( COMPETITION_WINDOW );
					}
				}
			}
		}
		break;
		
	case MENU_ESC_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( ESC_MENU ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( ESC_MENU );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( ESC_MENU );
					}
				}
			}
		}
		break;
	}
}

void CBasicNewGameMenu::InitShotCutString()
{
	int i=0;
	int nIndex = 0;
	CString strTemp;
	
	for ( i=0; i< BASIC_MENU_NUM; ++i){
		m_ShotcutText[i] = ID2GAMEWORD ("GAME_MENU",i);
		
		nIndex = RANPARAM::BasicMenuToRanparam[i];
		strTemp = CInnerInterface::GetInstance().GetdwKeyToString(RANPARAM::MenuShotcut[nIndex]);
		
		SetShotcutText(i,strTemp);
	}
 
}

void CBasicNewGameMenu::SetShotcutText ( DWORD nID, CString& strTemp )
{
	CString ShotcutTemp = m_ShotcutText[nID];

	ShotcutTemp = ShotcutTemp.Left(ShotcutTemp.Find("("));
	ShotcutTemp += "(" + strTemp + ")";
	m_ShotcutText[nID] = ShotcutTemp;	
}

