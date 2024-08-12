#include "StdAfx.h"
#include "ProductWindow.h"
#include "ProductWindowType.h"
#include "ProductWindowList.h"
#include "ProductWindowProduct.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#include "GLItemMixMan.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CProductWindow::CProductWindow ()
	: m_pType(NULL)
	, m_pList(NULL)
	, m_pSelectCategory(NULL)
	, m_pSelectProduct(NULL)
	, m_nSelectType(0)
	, m_pProduct(NULL)
{
}

CProductWindow::~CProductWindow ()
{
}

void CProductWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_TYPE_TITLE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_TYPE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_LIST_TITLE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_LIST" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_TITLE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS" );
	RegisterControl ( pBasicLineBox );

	m_pType = new CProductWindowType;
	m_pType->CreateSub ( this, "RAN_PRODUCT_WINDOW_TYPE", UI_FLAG_DEFAULT, PRODUCT_WINDOW_TYPE );
	m_pType->CreateSubControl ();
	m_pType->SetVisibleSingle( TRUE );
	RegisterControl ( m_pType );

	m_pSelectCategory = new CBasicTextBox;
	m_pSelectCategory->CreateSub ( this, "RAN_PRODUCT_WINDOW_LIST_TITLE" );
	m_pSelectCategory->SetFont ( pFont9 );
	m_pSelectCategory->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSelectCategory->SetText( "--" );
	RegisterControl ( m_pSelectCategory );

	m_pSelectProduct = new CBasicTextBox;
	m_pSelectProduct->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_TITLE" );
	m_pSelectProduct->SetFont ( pFont9 );
	m_pSelectProduct->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSelectProduct->SetText( "--" );
	RegisterControl ( m_pSelectProduct );

	m_pList = new CProductWindowList;
	m_pList->CreateSub ( this, "RAN_PRODUCT_WINDOW_LIST", UI_FLAG_DEFAULT, PRODUCT_WINDOW_LIST );
	m_pList->CreateSubControl ();
	m_pList->SetVisibleSingle( TRUE );
	RegisterControl ( m_pList );

	m_pProduct = new CProductWindowProduct;
	m_pProduct->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS", UI_FLAG_DEFAULT, PRODUCT_WINDOW_PRODUCT );
	m_pProduct->CreateSubControl ();
	m_pProduct->SetVisibleSingle( TRUE );
	RegisterControl ( m_pProduct );
}

void CProductWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CProductWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PRODUCT_WINDOW_TYPE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_PRODUCT_WINDOW_TYPE_SELECT & dwMsg )
				{
					if( m_pType )
						SelectCategory( m_pType->GetSelect() );
				}
			}
		}break;

	case PRODUCT_WINDOW_LIST:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_PRODUCT_WINDOW_LIST_SELECT & dwMsg )
				{
					if( m_pList )
					{
						if ( m_pSelectProduct )	
						{
							m_pSelectProduct->ClearText();
							m_pSelectProduct->AddText( m_pList->GetSelectText().c_str(), NS_UITEXTCOLOR::WHITE );
						}

						if ( m_pProduct )
						{
							m_pProduct->ResetData();
							m_pProduct->SetData( m_pList->GetSelectData() );
						}
					}
				}
			}
		}break;

	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( PRODUCT_WINDOW );
				return ;
			}
		}break;	
	};
}

void CProductWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}else{
	}
}

void CProductWindow::LoadData()
{
	ResetData();
	SelectCategory( 0 );
}

void CProductWindow::ResetData()
{
	m_nSelectType = 0;

	if ( m_pType )		m_pType->DefaultSelect();
	if ( m_pList )		m_pList->ClearData();
	if ( m_pProduct )	m_pProduct->ResetData();
	if ( m_pSelectCategory )	m_pSelectCategory->ClearText();
	if ( m_pSelectProduct )		m_pSelectProduct->ClearText();
}

void CProductWindow::SelectCategory( int nSelect )
{
	if ( m_pSelectCategory )	m_pSelectCategory->ClearText();
	if ( m_pSelectProduct )		m_pSelectProduct->ClearText();
	if ( m_pProduct )			m_pProduct->ResetData();

	m_nSelectType = nSelect;

	if ( m_nSelectType < 0 )	
		return;

	if ( m_nSelectType >= CProductWindowType::PRODUCT_WINDOW_TYPE_SIZE )
		return;

	if ( m_pSelectCategory )
		m_pSelectCategory->AddText( ID2GAMEWORD( "RAN_PRODUCT_TYPE_TEXT", m_nSelectType ), NS_UITEXTCOLOR::WHITE );

	if ( m_pList )
	{
		m_pList->LoadData( m_nSelectType, 0 );

		if ( m_pSelectProduct )	
			m_pSelectProduct->AddText( m_pList->GetSelectText().c_str(), NS_UITEXTCOLOR::WHITE );

		if ( m_pProduct )
			m_pProduct->SetData( m_pList->GetSelectData() );
	}
}
