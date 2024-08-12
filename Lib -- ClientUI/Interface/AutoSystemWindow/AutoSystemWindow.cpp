/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSystemWindow.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSystemWindow.h"
#include "AutoSkillPage/AutoSkillPage.h"
#include "AutoPotionPage/AutoPotionPage.h"
#include "AutoFollowPage/AutoFollowPage.h"

#include "../BasicLineBox.h"
#include "../BasicTextBoxEx.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"
#include "../BasicTextButton.h"
#include "../InnerInterface.h"
#include "../ModalWindow.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/UIWindowBody.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoSystemWindow::CAutoSystemWindow()
	: m_pWhiteBG(NULL)
	, m_nCurPage(-1)

	, m_pAutoSkillPage(NULL)
	, m_pAutoPotionPage(NULL)
	, m_pAutoFollowPage(NULL)

	, m_bAutoSKILL(FALSE)
	, m_bAutoPOTION(FALSE)
	, m_bAutoFOLLOW(FALSE)
{
	memset(m_pLBox_,	NULL, sizeof(m_pLBox_));
	memset(m_pText_,	NULL, sizeof(m_pText_));
	memset(m_pButton_,	NULL, sizeof(m_pButton_));
	memset(m_pIconBG_,	NULL, sizeof(m_pIconBG_));
}

void CAutoSystemWindow::CreateSubControl()
{
	m_pWhiteBG = new CUIControl;
	m_pWhiteBG->CreateSub(this, "AUTOSYSTEM_WINDOW_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	RegisterControl(m_pWhiteBG);

	char strTemp[50];

	for (int i = 0; i < nMAXLBOX; i++)
	{
		sprintf(strTemp, "AUTOSYSTEM_WINDOW_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	for (int i = 0; i < nMAXICON; i++)
	{
		sprintf(strTemp, "AUTOSYSTEM_WINDOW_ICON_%d", i);

		m_pIconBG_[i] = new CUIControl;
		m_pIconBG_[i]->CreateSub(this, strTemp, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pIconBG_[i]->SetVisibleSingle(FALSE);
		RegisterControl(m_pIconBG_[i]);
	}

	CD3DFontPar* pFont9		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignCenter	= TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	const DWORD& dwWhite	= NS_UITEXTCOLOR::WHITESMOKE;

	for (int i = 0; i < nMAXTEXT; i++)
	{
		sprintf(strTemp, "AUTOSYSTEM_WINDOW_TEXT_%d", i);

		m_pText_[i] = CreateStaticControl(strTemp, pFont9, nAlignCenter);
		m_pText_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_TEXT", i), dwWhite);
	}

	for (int i = 0; i < nMAXBUTTON; i++)
	{
		sprintf(strTemp, "AUTOSYSTEM_WINDOW_BUTTON_%d", i);

		m_pButton_[i] = new CBasicTextButton;
		m_pButton_[i]->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, BUTTON_0 + i);
		m_pButton_[i]->CreateBaseButton(strTemp, CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, "", _DEFAULT_FONT_SHADOW_FLAG);
		RegisterControl(m_pButton_[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		m_pButton_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 6));
	}

	m_pButton_[3]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 0));
	m_pButton_[4]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 1));
	m_pButton_[5]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 2));

	m_pAutoSkillPage = new CAutoSkillPage;
	m_pAutoSkillPage->CreateSub(this, "AUTOSKILL_PAGE", UI_FLAG_DEFAULT, PAGE_0);
	m_pAutoSkillPage->CreateSubControl();
	m_pAutoSkillPage->SetVisibleSingle(FALSE);
	RegisterControl(m_pAutoSkillPage);

	m_pAutoPotionPage = new CAutoPotionPage;
	m_pAutoPotionPage->CreateSub(this, "AUTOPOTION_PAGE", UI_FLAG_DEFAULT, PAGE_1);
	m_pAutoPotionPage->CreateSubControl();
	m_pAutoPotionPage->SetVisibleSingle(FALSE);
	RegisterControl(m_pAutoPotionPage);

	m_pAutoFollowPage = new CAutoFollowPage;
	m_pAutoFollowPage->CreateSub(this, "AUTOFOLLOW_PAGE", UI_FLAG_DEFAULT, PAGE_2);
	m_pAutoFollowPage->CreateSubControl();
	m_pAutoFollowPage->SetVisibleSingle(FALSE);
	RegisterControl(m_pAutoFollowPage);
}

void CAutoSystemWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg))
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface(AUTOSYSTEM_WINDOW);
		}
	}
	break;

	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().SetAutoSystemWindowClose();
		}
	}
	break;

	case BUTTON_0:
	case BUTTON_1:
	case BUTTON_2:
	{
		int nIndex = ControlID - BUTTON_0;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ShowPage(nIndex);
		}
	}
	break;

	case BUTTON_3:
	case BUTTON_4:
	case BUTTON_5:
	{
		int nIndex = ControlID - BUTTON_3;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetAutoSystem(nIndex);
		}
	}
	break;
	}

	if (UIKeyCheck::GetInstance()->Check(DIK_F6, DXKEY_DOWN))
	{
		SetAutoSystem(0);
	}
	else if (UIKeyCheck::GetInstance()->Check(DIK_F7, DXKEY_DOWN))
	{
		SetAutoSystem(1);
	}
	else if (UIKeyCheck::GetInstance()->Check(DIK_F8, DXKEY_DOWN))
	{
		SetAutoSystem(2);
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CAutoSystemWindow::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		m_pAutoFollowPage->RefreshFriendList();
	}
}

CBasicTextBox* CAutoSystemWindow::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CAutoSystemWindow::ResetPos()
{
	// LG-7 White BG
	{
		UIRECT rcGlobal_LBox_0 = m_pLBox_[0]->GetGlobalPos();
		UIRECT rcLocal_LBox_0 = m_pLBox_[0]->GetLocalPos();

		UIRECT rcOLD_Global = m_pWhiteBG->GetGlobalPos();
		UIRECT rcOLD_Local = m_pWhiteBG->GetLocalPos();

		UIRECT rcNEW_Global = UIRECT(
			rcOLD_Global.left,
			rcOLD_Global.top,
			rcOLD_Global.sizeX,
			rcGlobal_LBox_0.sizeY + 10.0f
		);

		UIRECT rcNEW_Local = UIRECT(
			rcOLD_Local.left,
			rcOLD_Local.top,
			rcOLD_Local.sizeX,
			rcLocal_LBox_0.sizeY + 10.0f
		);

		m_pWhiteBG->AlignSubControl(rcOLD_Global, rcNEW_Global);
		m_pWhiteBG->SetLocalPos(rcNEW_Local);
		m_pWhiteBG->SetGlobalPos(rcNEW_Global);
	}

	// LG-7 AutoSystemWindow
	{
		UIRECT rcGlobal_White_BG = m_pWhiteBG->GetGlobalPos();
		UIRECT rcLocal_White_BG = m_pWhiteBG->GetLocalPos();

		UIRECT rcOLD_Global = GetGlobalPos();
		UIRECT rcOLD_Local = GetLocalPos();

		UIRECT rcNEW_Global = UIRECT(
			rcOLD_Global.left,
			rcOLD_Global.top,
			rcOLD_Global.sizeX,
			rcGlobal_White_BG.sizeY + 20.0f
		);

		UIRECT rcNEW_Local = UIRECT(
			rcOLD_Local.left,
			rcOLD_Local.top,
			rcOLD_Local.sizeX,
			rcLocal_White_BG.sizeY + 20.0f
		);

		AlignSubControl(rcOLD_Global, rcNEW_Global);
		SetLocalPos(rcNEW_Local);
		SetGlobalPos(rcNEW_Global);
	}

	// LG-7 AutoSystemWindow Body
	{
		UIRECT rcGlobal_This = GetGlobalPos();
		UIRECT rcLocal_This = GetLocalPos();

		UIRECT rcOLD_Global = GetBody()->GetGlobalPos();
		UIRECT rcOLD_Local = GetBody()->GetLocalPos();

		UIRECT rcNEW_Global = UIRECT(
			rcOLD_Global.left,
			rcOLD_Global.top,
			rcOLD_Global.sizeX,
			rcGlobal_This.sizeY - 18.0f
		);

		UIRECT rcNEW_Local = UIRECT(
			rcOLD_Local.left,
			rcOLD_Local.top,
			rcOLD_Local.sizeX,
			rcLocal_This.sizeY - 18.0f
		);

		GetBody()->AlignSubControl(rcOLD_Global, rcNEW_Global);
		GetBody()->SetLocalPos(rcNEW_Local);
		GetBody()->SetGlobalPos(rcNEW_Global);
	}
}

void CAutoSystemWindow::ShowPage(int nIndex)
{
	for (int i = 0; i < 3; i++)
	{
		m_pButton_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 6));
	}

	m_pAutoSkillPage->SetVisibleSingle(FALSE);
	m_pAutoPotionPage->SetVisibleSingle(FALSE);
	m_pAutoFollowPage->SetVisibleSingle(FALSE);

	if (m_nCurPage == nIndex)
	{
		m_nCurPage = -1;

		ResetPos();
	}
	else
	{
		m_nCurPage = nIndex;

		ResetPos();

		switch (nIndex)
		{
		case 0:
		{
			//m_pAutoSkillPage->SetVisibleSingle(TRUE);
			//m_pButton_[0]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 7));

			// LG-7 m_pAutoSkillPage
			//{
			//	UIRECT rcGlobal_LBox_0	= m_pLBox_[0]->GetGlobalPos();
			//	UIRECT rcLocal_LBox_0	= m_pLBox_[0]->GetLocalPos();

			//	UIRECT rcOLD_Global		= m_pAutoSkillPage->GetGlobalPos();
			//	UIRECT rcOLD_Local		= m_pAutoSkillPage->GetLocalPos();

			//	UIRECT rcNEW_Global		= UIRECT(
			//		rcGlobal_LBox_0.left,
			//		rcGlobal_LBox_0.top + rcGlobal_LBox_0.sizeY + 4.0f,
			//		rcOLD_Global.sizeX,
			//		rcOLD_Global.sizeY
			//	);

			//	UIRECT rcNEW_Local		= UIRECT(
			//		rcLocal_LBox_0.left,
			//		rcLocal_LBox_0.top + rcLocal_LBox_0.sizeY + 4.0f,
			//		rcOLD_Local.sizeX,
			//		rcOLD_Local.sizeY
			//	);

			//	m_pAutoSkillPage->AlignSubControl(rcOLD_Global, rcNEW_Global);
			//	m_pAutoSkillPage->SetLocalPos(rcNEW_Local);
			//	m_pAutoSkillPage->SetGlobalPos(rcNEW_Global);
			//}

			// LG-7 White BG
			//{
			//	UIRECT rcGlobal_LBox_0			= m_pLBox_[0]->GetGlobalPos();
			//	UIRECT rcLocal_LBox_0			= m_pLBox_[0]->GetLocalPos();

			//	UIRECT rcGlobal_AutoSkillPage	= m_pAutoSkillPage->GetGlobalPos();
			//	UIRECT rcLocal_AutoSkillPage	= m_pAutoSkillPage->GetLocalPos();

			//	UIRECT rcOLD_Global				= m_pWhiteBG->GetGlobalPos();
			//	UIRECT rcOLD_Local				= m_pWhiteBG->GetLocalPos();

			//	UIRECT rcNEW_Global				= UIRECT(
			//		rcOLD_Global.left,
			//		rcOLD_Global.top,
			//		rcOLD_Global.sizeX,
			//		rcGlobal_LBox_0.sizeY + rcGlobal_AutoSkillPage.sizeY + 14.0f
			//	);

			//	UIRECT rcNEW_Local				= UIRECT(
			//		rcOLD_Local.left,
			//		rcOLD_Local.top,
			//		rcOLD_Local.sizeX,
			//		rcLocal_LBox_0.sizeY + rcLocal_AutoSkillPage.sizeY + 14.0f
			//	);

			//	m_pWhiteBG->AlignSubControl(rcOLD_Global, rcNEW_Global);
			//	m_pWhiteBG->SetLocalPos(rcNEW_Local);
			//	m_pWhiteBG->SetGlobalPos(rcNEW_Global);
			//}

			// LG-7 AutoSystemWindow
			//{
			//	UIRECT rcGlobal_White_BG = m_pWhiteBG->GetGlobalPos();
			//	UIRECT rcLocal_White_BG = m_pWhiteBG->GetLocalPos();

			//	UIRECT rcOLD_Global = GetGlobalPos();
			//	UIRECT rcOLD_Local = GetLocalPos();

			//	UIRECT rcNEW_Global = UIRECT(
			//		rcOLD_Global.left,
			//		rcOLD_Global.top,
			//		rcOLD_Global.sizeX,
			//		rcGlobal_White_BG.sizeY + 20.0f
			//	);

			//	UIRECT rcNEW_Local = UIRECT(
			//		rcOLD_Local.left,
			//		rcOLD_Local.top,
			//		rcOLD_Local.sizeX,
			//		rcLocal_White_BG.sizeY + 20.0f
			//	);

			//	AlignSubControl(rcOLD_Global, rcNEW_Global);
			//	SetLocalPos(rcNEW_Local);
			//	SetGlobalPos(rcNEW_Global);
			//}

			// LG-7 AutoSystemWindow Body
			//{
			//	UIRECT rcGlobal_This = GetGlobalPos();
			//	UIRECT rcLocal_This = GetLocalPos();

			//	UIRECT rcOLD_Global = GetBody()->GetGlobalPos();
			//	UIRECT rcOLD_Local = GetBody()->GetLocalPos();

			//	UIRECT rcNEW_Global = UIRECT(
			//		rcOLD_Global.left,
			//		rcOLD_Global.top,
			//		rcOLD_Global.sizeX,
			//		rcGlobal_This.sizeY - 18.0f
			//	);

			//	UIRECT rcNEW_Local = UIRECT(
			//		rcOLD_Local.left,
			//		rcOLD_Local.top,
			//		rcOLD_Local.sizeX,
			//		rcLocal_This.sizeY - 18.0f
			//	);

			//	GetBody()->AlignSubControl(rcOLD_Global, rcNEW_Global);
			//	GetBody()->SetLocalPos(rcNEW_Local);
			//	GetBody()->SetGlobalPos(rcNEW_Global);
			//}
			CInnerInterface::GetInstance().PrintConsoleText ( "[System]Function Currently Disable" );
		}
		break;

		case 1:
		{
			m_pAutoPotionPage->SetVisibleSingle(TRUE);
			m_pButton_[1]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 7));

			// LG-7 m_pAutoPotionPage
			{
				UIRECT rcGlobal_LBox_0	= m_pLBox_[0]->GetGlobalPos();
				UIRECT rcLocal_LBox_0	= m_pLBox_[0]->GetLocalPos();

				UIRECT rcOLD_Global		= m_pAutoPotionPage->GetGlobalPos();
				UIRECT rcOLD_Local		= m_pAutoPotionPage->GetLocalPos();

				UIRECT rcNEW_Global		= UIRECT(
					rcGlobal_LBox_0.left,
					rcGlobal_LBox_0.top + rcGlobal_LBox_0.sizeY + 4.0f,
					rcOLD_Global.sizeX,
					rcOLD_Global.sizeY
				);

				UIRECT rcNEW_Local		= UIRECT(
					rcLocal_LBox_0.left,
					rcLocal_LBox_0.top + rcLocal_LBox_0.sizeY + 4.0f,
					rcOLD_Local.sizeX,
					rcOLD_Local.sizeY
				);

				m_pAutoPotionPage->AlignSubControl(rcOLD_Global, rcNEW_Global);
				m_pAutoPotionPage->SetLocalPos(rcNEW_Local);
				m_pAutoPotionPage->SetGlobalPos(rcNEW_Global);
			}

			// LG-7 White BG
			{
				UIRECT rcGlobal_LBox_0			= m_pLBox_[0]->GetGlobalPos();
				UIRECT rcLocal_LBox_0			= m_pLBox_[0]->GetLocalPos();

				UIRECT rcGlobal_AutoPotionPage	= m_pAutoPotionPage->GetGlobalPos();
				UIRECT rcLocal_AutoPotionPage	= m_pAutoPotionPage->GetLocalPos();

				UIRECT rcOLD_Global				= m_pWhiteBG->GetGlobalPos();
				UIRECT rcOLD_Local				= m_pWhiteBG->GetLocalPos();

				UIRECT rcNEW_Global				= UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_LBox_0.sizeY + rcGlobal_AutoPotionPage.sizeY + 14.0f
				);

				UIRECT rcNEW_Local				= UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_LBox_0.sizeY + rcLocal_AutoPotionPage.sizeY + 14.0f
				);

				m_pWhiteBG->AlignSubControl(rcOLD_Global, rcNEW_Global);
				m_pWhiteBG->SetLocalPos(rcNEW_Local);
				m_pWhiteBG->SetGlobalPos(rcNEW_Global);
			}

			// LG-7 AutoSystemWindow
			{
				UIRECT rcGlobal_White_BG = m_pWhiteBG->GetGlobalPos();
				UIRECT rcLocal_White_BG = m_pWhiteBG->GetLocalPos();

				UIRECT rcOLD_Global = GetGlobalPos();
				UIRECT rcOLD_Local = GetLocalPos();

				UIRECT rcNEW_Global = UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_White_BG.sizeY + 20.0f
				);

				UIRECT rcNEW_Local = UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_White_BG.sizeY + 20.0f
				);

				AlignSubControl(rcOLD_Global, rcNEW_Global);
				SetLocalPos(rcNEW_Local);
				SetGlobalPos(rcNEW_Global);
			}

			// LG-7 AutoSystemWindow Body
			{
				UIRECT rcGlobal_This = GetGlobalPos();
				UIRECT rcLocal_This = GetLocalPos();

				UIRECT rcOLD_Global = GetBody()->GetGlobalPos();
				UIRECT rcOLD_Local = GetBody()->GetLocalPos();

				UIRECT rcNEW_Global = UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_This.sizeY - 18.0f
				);

				UIRECT rcNEW_Local = UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_This.sizeY - 18.0f
				);

				GetBody()->AlignSubControl(rcOLD_Global, rcNEW_Global);
				GetBody()->SetLocalPos(rcNEW_Local);
				GetBody()->SetGlobalPos(rcNEW_Global);
			}
		}
		break;

		case 2:
		{
			m_pAutoFollowPage->SetVisibleSingle(TRUE);
			m_pButton_[2]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 7));

			// LG-7 m_pAutoFollowPage
			{
				UIRECT rcGlobal_LBox_0	= m_pLBox_[0]->GetGlobalPos();
				UIRECT rcLocal_LBox_0	= m_pLBox_[0]->GetLocalPos();

				UIRECT rcOLD_Global		= m_pAutoFollowPage->GetGlobalPos();
				UIRECT rcOLD_Local		= m_pAutoFollowPage->GetLocalPos();

				UIRECT rcNEW_Global		= UIRECT(
					rcGlobal_LBox_0.left,
					rcGlobal_LBox_0.top + rcGlobal_LBox_0.sizeY + 4.0f,
					rcOLD_Global.sizeX,
					rcOLD_Global.sizeY
				);

				UIRECT rcNEW_Local		= UIRECT(
					rcLocal_LBox_0.left,
					rcLocal_LBox_0.top + rcLocal_LBox_0.sizeY + 4.0f,
					rcOLD_Local.sizeX,
					rcOLD_Local.sizeY
				);

				m_pAutoFollowPage->AlignSubControl(rcOLD_Global, rcNEW_Global);
				m_pAutoFollowPage->SetLocalPos(rcNEW_Local);
				m_pAutoFollowPage->SetGlobalPos(rcNEW_Global);
			}

			// LG-7 White BG
			{
				UIRECT rcGlobal_LBox_0			= m_pLBox_[0]->GetGlobalPos();
				UIRECT rcLocal_LBox_0			= m_pLBox_[0]->GetLocalPos();

				UIRECT rcGlobal_AutoFollowPage	= m_pAutoFollowPage->GetGlobalPos();
				UIRECT rcLocal_AutoFollowPage	= m_pAutoFollowPage->GetLocalPos();

				UIRECT rcOLD_Global				= m_pWhiteBG->GetGlobalPos();
				UIRECT rcOLD_Local				= m_pWhiteBG->GetLocalPos();

				UIRECT rcNEW_Global				= UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_LBox_0.sizeY + rcGlobal_AutoFollowPage.sizeY + 14.0f
				);

				UIRECT rcNEW_Local				= UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_LBox_0.sizeY + rcLocal_AutoFollowPage.sizeY + 14.0f
				);

				m_pWhiteBG->AlignSubControl(rcOLD_Global, rcNEW_Global);
				m_pWhiteBG->SetLocalPos(rcNEW_Local);
				m_pWhiteBG->SetGlobalPos(rcNEW_Global);
			}

			// LG-7 AutoSystemWindow
			{
				UIRECT rcGlobal_White_BG = m_pWhiteBG->GetGlobalPos();
				UIRECT rcLocal_White_BG = m_pWhiteBG->GetLocalPos();

				UIRECT rcOLD_Global = GetGlobalPos();
				UIRECT rcOLD_Local = GetLocalPos();

				UIRECT rcNEW_Global = UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_White_BG.sizeY + 20.0f
				);

				UIRECT rcNEW_Local = UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_White_BG.sizeY + 20.0f
				);

				AlignSubControl(rcOLD_Global, rcNEW_Global);
				SetLocalPos(rcNEW_Local);
				SetGlobalPos(rcNEW_Global);
			}

			// LG-7 AutoSystemWindow Body
			{
				UIRECT rcGlobal_This = GetGlobalPos();
				UIRECT rcLocal_This = GetLocalPos();

				UIRECT rcOLD_Global = GetBody()->GetGlobalPos();
				UIRECT rcOLD_Local = GetBody()->GetLocalPos();

				UIRECT rcNEW_Global = UIRECT(
					rcOLD_Global.left,
					rcOLD_Global.top,
					rcOLD_Global.sizeX,
					rcGlobal_This.sizeY - 18.0f
				);

				UIRECT rcNEW_Local = UIRECT(
					rcOLD_Local.left,
					rcOLD_Local.top,
					rcOLD_Local.sizeX,
					rcLocal_This.sizeY - 18.0f
				);

				GetBody()->AlignSubControl(rcOLD_Global, rcNEW_Global);
				GetBody()->SetLocalPos(rcNEW_Local);
				GetBody()->SetGlobalPos(rcNEW_Global);
			}
		}
		break;
		}
	}
}

void CAutoSystemWindow::SetAutoSystem(int nIndex)
{
	switch (nIndex)
	{
	case 0:
	{
		CInnerInterface::GetInstance().PrintConsoleText ( "[System]Function Currently Disable" );
		/*if (m_bAutoSKILL)	SetAutoSKILL(FALSE);
		else				SetAutoSKILL(TRUE);*/
	}
	break;

	case 1:
	{
		if (m_bAutoPOTION)	SetAutoPOTION(FALSE);
		else				SetAutoPOTION(TRUE);
	}
	break;

	case 2:
	{
		if (m_bAutoFOLLOW)	SetAutoFOLLOW(FALSE);
		else				SetAutoFOLLOW(TRUE);
	}
	break;
	}
}

void CAutoSystemWindow::SetAutoSKILL(BOOL bValue)
{
	switch (bValue)
	{
	case TRUE:
	{
		int nTab = m_pAutoSkillPage->GetCurSkillTab();
		if (nTab < 30 || nTab > 50)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}

		BOOL bOK = m_pAutoSkillPage->CheckSkillTab(nTab);
		if (!bOK)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 1));
			return;
		}

		int nLevMin	= m_pAutoSkillPage->GetLevMin();
		int nLevMax	= m_pAutoSkillPage->GetLevMax();
		if (nLevMin < 0 || nLevMax < 0 || nLevMin > nLevMax )
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 2));
			return;
		}

		int nRange = m_pAutoSkillPage->GetRange();
		if (nRange < 1 || nRange > 300)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 3));
			return;
		}

		SAUTO_SKILL sAutoSkill;
		sAutoSkill.m_bActive	= TRUE;
		sAutoSkill.m_nTab		= nTab;
		sAutoSkill.m_nLevMin	= nLevMin;
		sAutoSkill.m_nLevMax	= nLevMax;
		sAutoSkill.m_nRange		= nRange;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}
		else
		{
			pCharacter->ReqAutoSkill(&sAutoSkill);
		}

		m_pButton_[3]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 3));
		m_bAutoSKILL = TRUE;
		m_pIconBG_[0]->SetVisibleSingle(TRUE);
	}
	break;

	case FALSE:
	{
		SAUTO_SKILL sAutoSkill;
		sAutoSkill.m_bActive = FALSE;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}
		else
		{
			pCharacter->ReqAutoSkill(&sAutoSkill);
		}

		m_pButton_[3]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 0));
		m_bAutoSKILL = FALSE;
		m_pIconBG_[0]->SetVisibleSingle(FALSE);
	}
	break;
	}
}

void CAutoSystemWindow::SetAutoPOTION(BOOL bValue)
{
	switch (bValue)
	{
	case TRUE:
	{
		int nTab = m_pAutoPotionPage->GetCurPotionTab();
		if (nTab < 0 || nTab > 1)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}

		BOOL bOK = m_pAutoPotionPage->CheckPotionTab(nTab);
		if (!bOK)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 4));
			return;
		}

		int nHPSet = m_pAutoPotionPage->GetPotionSettings(0);
		int nMPSet = m_pAutoPotionPage->GetPotionSettings(1);
		int nSPSet = m_pAutoPotionPage->GetPotionSettings(2);
		if (nHPSet < 0 || nMPSet < 0 || nSPSet < 0)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 5));
			return;
		}

		SAUTO_POTION sAutoPotion;
		sAutoPotion.m_bActive	= TRUE;
		sAutoPotion.m_nTab		= nTab;
		sAutoPotion.m_nHPSet	= nHPSet;
		sAutoPotion.m_nMPSet	= nMPSet;
		sAutoPotion.m_nSPSet	= nSPSet;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}
		else
		{
			pCharacter->ReqAutoPotion(&sAutoPotion);
		}

		m_pButton_[4]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 4));
		m_bAutoPOTION = TRUE;
		m_pIconBG_[1]->SetVisibleSingle(TRUE);
	}
	break;

	case FALSE:
	{
		SAUTO_POTION sAutoPotion;
		sAutoPotion.m_bActive = FALSE;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}
		else
		{
			pCharacter->ReqAutoPotion(&sAutoPotion);
		}

		m_pButton_[4]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 1));
		m_bAutoPOTION = FALSE;
		m_pIconBG_[1]->SetVisibleSingle(FALSE);
	}
	break;
	}
}

void CAutoSystemWindow::SetAutoFOLLOW(BOOL bValue)
{
	switch (bValue)
	{
	case TRUE:
	{
		CString strFollowName = m_pAutoFollowPage->GetFollowName();
		if (!strFollowName.GetLength())
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 6));
			return;
		}

		int nFollowRange = m_pAutoFollowPage->GetFollowRange();
		if (nFollowRange < 0)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 7));
			return;
		}

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)
		{
			DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
			return;
		}
		else
		{
			pCharacter->ReqAutoFollow(strFollowName);
		}
	}
	break;

	case FALSE:
	{
		StopAutoFollow();
	}
	break;
	}
}

void CAutoSystemWindow::StartAutoFOLLOW(SAUTO_FOLLOW sAutoFollow)
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCharacter)
	{
		DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
		return;
	}
	else
	{
		sAutoFollow.m_nRange = m_pAutoFollowPage->GetFollowRange();
		pCharacter->StartAutoFOLLOW(sAutoFollow);
	}

	m_pButton_[5]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 5));
	m_bAutoFOLLOW = TRUE;
	m_pIconBG_[2]->SetVisibleSingle(TRUE);
}

void CAutoSystemWindow::StopAutoFollow()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCharacter)
	{
		DoModal(ID2GAMEWORD("AUTOSYSTEM_WINDOW_STAGE", 0));
		return;
	}
	else
	{
		pCharacter->StopAutoFOLLOW();
	}

	m_pButton_[5]->SetOneLineText((char*)ID2GAMEWORD("AUTOSYSTEM_WINDOW_BUTTON", 2));
	m_bAutoFOLLOW = FALSE;
	m_pIconBG_[2]->SetVisibleSingle(FALSE);
}

void CAutoSystemWindow::ReRunAutoSkill()
{
	if (!m_bAutoSKILL) return;

	SetAutoSKILL(TRUE);
}

void CAutoSystemWindow::ReRunAutoPotion()
{
	if (!m_bAutoPOTION) return;

	SetAutoPOTION(TRUE);
}