#include "StdAfx.h"

#include "BasicVarTextBoxItemLink.h"
#include "BasicTextBox.h"
#include "BasicButton.h"
#include "BasicLineBoxEx.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- Engine/GUInterface/UIDebugSet.h"
#include "UITextControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CBasicVarTextBoxItemLink::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBoxItemLink::CBasicVarTextBoxItemLink () :
	m_pTextBox ( NULL ),
	m_pLineBox ( NULL ),
	m_pFont ( NULL ),
	m_pSelfDummy ( NULL ),
	m_pCloseButton ( NULL ),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false )
{
}

CBasicVarTextBoxItemLink::~CBasicVarTextBoxItemLink ()
{
}

void CBasicVarTextBoxItemLink::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxVarTextBox ( "BASIC_VAR_LINE_BOX" );
	pLineBox->SetNoUpdate ( true );
	RegisterControl ( pLineBox );
	m_pLineBox = pLineBox;

	CUIControl* pSelfDummy = new CUIControl;
	pSelfDummy->CreateSub ( this, "BASIC_VAR_LINE_BOX" );
	pSelfDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( pSelfDummy );
	m_pSelfDummy = pSelfDummy;

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBox->SetFont ( m_pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pTextBox->SetNoUpdate ( true );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;

	m_pCloseButton = new CBasicButton;
	m_pCloseButton->CreateSub ( this, "INFO_DISPLAY_LINK_CLOSE" , UI_FLAG_DEFAULT, BUTTON_CLOSE );
	m_pCloseButton->CreateFlip ( "INFO_DISPLAY_LINK_CLOSE_F", CBasicButton::CLICK_FLIP );
	m_pCloseButton->CreateMouseOver( "INFO_DISPLAY_LINK_CLOSE_OVER" );
	m_pCloseButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pCloseButton );
}

void CBasicVarTextBoxItemLink::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
}

void CBasicVarTextBoxItemLink::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
}

int CBasicVarTextBoxItemLink::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int	CBasicVarTextBoxItemLink::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxItemLink::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBoxItemLink::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBoxItemLink::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxItemLink::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBoxItemLink::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

void CBasicVarTextBoxItemLink::RePosControl ( int x, int y )
{
	const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();

	const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
	const UIRECT& rcOriginPos = GetLocalPos ();

	AlignSubControl ( rcOriginPos, rcLocalPosDummy );

	if ( m_pTextBox )
	{
		int nTotalLine = m_pTextBox->GetTotalLine ();
		const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
		const float fTextBoxGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
		const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;

		UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

		AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

		SetGlobalPos ( rcLocalNewPos );		

		if ( m_pCloseButton )
		{
			const UIRECT& rcOriginPos1 = m_pTextBox->GetLocalPos();
			const UIRECT& rcOriginPos2 = m_pCloseButton->GetLocalPos();
			float fx = rcOriginPos1.right - rcOriginPos2.sizeX;
			float fy = rcOriginPos1.top;
			m_pCloseButton->SetLocalPos( D3DXVECTOR2 ( fx , fy ));
		}
	}
}

void CBasicVarTextBoxItemLink::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y );	
}

void CBasicVarTextBoxItemLink::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BUTTON_CLOSE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK );
				}

				AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN );
			}
		}break;
	};

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBasicVarTextBoxItemLink::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBoxItemLink::GetLongestLine ()
{
	if ( m_pTextBox )
	{
		return (float)m_pTextBox->GetLongestLine ();
	}
	return 0.0f;
}

void CBasicVarTextBoxItemLink::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	float fLongestLine = GetLongestLine ();

	NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet =
		NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongestLine, m_pFont );

	for ( int i = 0; i < (int)MLTextSet.size(); ++i )
	{
		CString strTemp = MLTextSet[i].strLine;
		if ( strTemp.GetLength() )
		{
			AddTextNoSplit ( strTemp, /*NS_UITEXTCOLOR::DEFAULT*/ dwColor );		
		}
	}	
}

int	CBasicVarTextBoxItemLink::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBoxItemLink::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBoxItemLink::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}

void CBasicVarTextBoxItemLink::ShowCloseButton( bool bSHOW )
{
	if ( m_pCloseButton )
		m_pCloseButton->SetVisibleSingle( bSHOW );
}