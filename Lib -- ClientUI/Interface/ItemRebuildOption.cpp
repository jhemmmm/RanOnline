#include "StdAfx.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "BasicTextBox.h"
#include "BasicButton.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "ItemRebuildOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CItemRebuildOption::CItemRebuildOption()
	: m_pNormal(NULL)
	, m_pFlip(NULL)
	, m_pTextTitle(NULL)
	, m_bCHECKED(FALSE)
{
}

CItemRebuildOption::~CItemRebuildOption()
{
}

void CItemRebuildOption::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	

	m_pNormal = new CUIControl;
	m_pNormal->CreateSub ( this, "ITEM_REBUILD_CHECKBOX_OFF" );
	m_pNormal->SetTransparentOption( TRUE );
	m_pNormal->SetVisibleSingle(FALSE);
	RegisterControl ( m_pNormal );
	
	m_pFlip = new CUIControl;
	m_pFlip->CreateSub ( this, "ITEM_REBUILD_CHECKBOX_ON" );
	m_pFlip->SetTransparentOption( TRUE );
	m_pFlip->SetVisibleSingle(FALSE);
	RegisterControl ( m_pFlip );
	
	m_pTextTitle = new CBasicTextBox;
	m_pTextTitle->CreateSub ( this, "ITEM_REBUILD_MAIN_CHECK_TEXT_TITLE_POS" );
	m_pTextTitle->SetFont ( pFont );
	m_pTextTitle->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextTitle );

	m_bCHECKED = TRUE;
	SetCheck();
	ManageOption( FALSE );
}

void CItemRebuildOption::SetCheck()
{
	m_bCHECKED = !m_bCHECKED;
	m_pFlip->SetVisibleSingle(m_bCHECKED);
	m_pNormal->SetVisibleSingle(!m_bCHECKED);
}

void CItemRebuildOption::ShowCheckOption()
{
	if ( m_pNormal )
		m_pNormal->SetVisibleSingle( TRUE );
	m_bCHECKED = FALSE;
}

void CItemRebuildOption::HideCheckOption()
{
	m_pNormal->SetVisibleSingle( FALSE );
	m_pFlip->SetVisibleSingle( FALSE );
}

int CItemRebuildOption::SetTextTitle(CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextTitle ) return m_pTextTitle->AddTextNoSplit( strText, dwColor );
	return -1;
}

void CItemRebuildOption::ManageOption( BOOL bOPT )
{
	m_pNormal->SetVisibleSingle( bOPT );
	m_pFlip->SetVisibleSingle( bOPT );

	if ( bOPT )
	{
		m_bCHECKED = !bOPT;
		m_pFlip->SetVisibleSingle(m_bCHECKED);
		m_pNormal->SetVisibleSingle(!m_bCHECKED);
	}
}