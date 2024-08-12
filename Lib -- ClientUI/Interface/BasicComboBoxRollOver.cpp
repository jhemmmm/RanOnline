#include "StdAfx.h"
#include "BasicComboBoxRollOver.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "BasicLineBox.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CBasicComboBoxRollOver::nSTARTLINE = 0;
const int CBasicComboBoxRollOver::nLIMITLINE = 100;
const int CBasicComboBoxRollOver::nNOT_SELECTED = -1;
const D3DCOLOR	CBasicComboBoxRollOver::dwMOUSEOVERCOLOR = D3DCOLOR_ARGB(0xFF,0x69,0x69,0x69);

CBasicComboBoxRollOver::CBasicComboBoxRollOver()
	: m_nSelectIndex ( nNOT_SELECTED )
	, m_pLineBox(NULL)
	, m_pTextBox(NULL)
	, m_pScrollBar(NULL)
{
}

CBasicComboBoxRollOver::~CBasicComboBoxRollOver ()
{
}

void CBasicComboBoxRollOver::CreateBaseComboBoxRollOver ( char* szComboBoxControl, bool bSelectInit /*= TRUE*/ )
{
	CreateComboBoxRollOverImage (bSelectInit);
	CreateScrollBar ();
	CreateSelectionImage ();

    CUIControl TempControl;
	TempControl.Create ( 1, szComboBoxControl );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CBasicComboBoxRollOver::CreateSelectionImage ()
{
}

void CBasicComboBoxRollOver::CreateComboBoxRollOverImage ( bool bSelectInit )
{
	CBasicLineBox* pLineBoxCombo = new CBasicLineBox;
	pLineBoxCombo->CreateSub ( this, "BASIC_LINE_BOX_COMBO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxCombo->CreateBaseBoxCombo ( "BASIC_COMBOBOX_ROLLOVER_BACK" );
	RegisterControl ( pLineBoxCombo );

	//	의도적으로 윗라인을 보이지 않게 합니다.
	pLineBoxCombo->SetUseRenderLine ( CBasicLineBox::TOP, FALSE );

	//	폰트
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	//	텍스트 박스
	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER_TEXTBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMBOBOX_ROLLOVER_TEXT_BOX );
	pTextBox->CreateMouseOver ( dwMOUSEOVERCOLOR );
	pTextBox->SetFont ( pFont8 );
	pTextBox->SetSensitive ( true );
	pTextBox->SetLimitLine ( nLIMITLINE );	
	pTextBox->SetSelectInit( bSelectInit );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;
}

void CBasicComboBoxRollOver::CreateScrollBar ()
{
	//	스크롤바 뒷면 검은 라인, 회색 배경
	CBasicLineBox* pLineBoxCombo = new CBasicLineBox;
	pLineBoxCombo->CreateSub ( this, "BASIC_LINE_BOX_COMBO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxCombo->CreateBaseBoxCombo ( "BASIC_COMBOBOX_ROLLOVER_SCROLLBAR_BOX" );
	RegisterControl ( pLineBoxCombo );
	m_pLineBox = pLineBoxCombo;

	//	사각 박스 사이즈를 정상 사이즈로 재조절하고 난 이후가 실제로
	//	원하는 크기의 이미지입니다. 즉, 'COMBOBOX_ROLLOVER_SCROLLBAR'에 설정한
	//	만큼의 크기란 것이지요. 그러나, 그 이후에 스크롤바가 어떻게 리사이징이 되더라도,
	//	XSIZE는 재조절되어서는 안되기 때문에 그 플래그는 빼 버린 것입니다.
	pLineBoxCombo->ResetAlignFlagLine ();
	pLineBoxCombo->SetAlignFlagLine ( CBasicLineBox::LEFT, UI_FLAG_YSIZE );
	pLineBoxCombo->SetAlignFlagLine ( CBasicLineBox::RIGHT,UI_FLAG_YSIZE );
	pLineBoxCombo->SetAlignFlagLine ( CBasicLineBox::BOTTOM, UI_FLAG_BOTTOM );
	pLineBoxCombo->SetAlignFlag ( UI_FLAG_YSIZE | UI_FLAG_RIGHT );

	int nTotalLine = m_pTextBox->GetVisibleLine();

	//	스크롤바
	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, BASIC_COMBOBOX_ROLLOVER_SCROLLBAR );
	pScrollBar->CreateBaseScrollBar ( "BASIC_COMBOBOX_ROLLOVER_SCROLLBAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pScrollBar = pScrollBar;
}
void CBasicComboBoxRollOver::SetTextAlign( int Align )
{
	m_pTextBox->SetTextAlign( Align );
}

int CBasicComboBoxRollOver::AddText( CString str, bool bVisibleScroll /*= FALSE*/, bool bOneLine /*= FALSE*/ )
{
	int nRet;
	if( bOneLine )
		nRet = m_pTextBox->AddOneLineText( str, NS_UITEXTCOLOR::WHITE );
	else
		nRet = m_pTextBox->AddText( str, NS_UITEXTCOLOR::WHITE );

	int nTotal = m_pTextBox->GetTotalLine ();
	int nViewPerPage = m_pTextBox->GetVisibleLine();

	if ( nTotal <= nViewPerPage )
	{
		if( !bVisibleScroll ) 
		{
			m_pScrollBar->SetVisibleSingle ( FALSE );
			m_pLineBox->SetVisibleSingle ( FALSE );
		}else{
			m_pScrollBar->SetVisibleSingle ( TRUE );
			m_pLineBox->SetVisibleSingle ( TRUE );
		}
	}
	else
	{
		m_pScrollBar->SetVisibleSingle ( TRUE );
		m_pLineBox->SetVisibleSingle ( TRUE );

		m_pScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	}

	return nRet;
}

void  CBasicComboBoxRollOver::ClearText ()
{
	m_pTextBox->ClearText ();

	m_pScrollBar->SetVisibleSingle ( FALSE );
	m_pLineBox->SetVisibleSingle ( FALSE );

	m_pScrollBar->GetThumbFrame()->SetState ( 1, 1 );
}

void CBasicComboBoxRollOver::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BASIC_COMBOBOX_ROLLOVER_SCROLLBAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UI_MSG_COMBOBOX_ROLLOVER_SCROLL );
			}
		}
		break;

	case COMBOBOX_ROLLOVER_TEXT_BOX:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_nSelectIndex = m_pTextBox->GetSelectPos ();
			}
		}
		break;
	}
}

void CBasicComboBoxRollOver::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_nSelectIndex = nNOT_SELECTED;
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( bFirstControl )
	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pScrollBar->GetThumbFrame ();
		const int nTotalLine = m_pTextBox->GetTotalLine ();
		const int nLinePerOneView = m_pTextBox->GetVisibleLine();
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			if ( nPos < nSTARTLINE ) nPos = nSTARTLINE;
			m_pTextBox->SetCurLine ( nPos );			
		}
	}
}

void CBasicComboBoxRollOver::SetSelectIndex ( int nIndex )
{ 
	m_pTextBox->SetSelectPos( nIndex );
//	m_pTextBox->SetCurLine( nIndex ); 
	m_nSelectIndex = nIndex; 

	m_pTextBox->UpdateMouseOverImage();

}

CString	CBasicComboBoxRollOver::GetSelectText ( int nIndex )
{
	if ( !m_pTextBox )
	{
		GASSERT ( 0 && "m_pTextBox이 NULL입니다. CreateBaseComboBoxRollOver()를 먼저 호출하십시오." );
		return CString();
	}

	return m_pTextBox->GetText ( nIndex );
}

void CBasicComboBoxRollOver::SetScrollPercent ( float fPercent )
{
	if ( !m_pScrollBar )
	{
		GASSERT ( 0 && "m_pScrollBar이 NULL입니다. CreateBaseComboBoxRollOver()를 먼저 호출하십시오." );
		return;
	}

	m_pScrollBar->GetThumbFrame()->SetPercent ( fPercent );
}

void CBasicComboBoxRollOver::SetTextData( int nIndex, DWORD dwData )
{
	if ( !m_pTextBox )
	{
		GASSERT ( 0 && "m_pTextBox이 NULL입니다. CreateBaseComboBoxRollOver()를 먼저 호출하십시오." );
		return;
	}

	m_pTextBox->SetTextData( nIndex, dwData );
}

DWORD CBasicComboBoxRollOver::GetTextData( int nIndex )
{
	if ( !m_pTextBox )
	{
		GASSERT ( 0 && "m_pTextBox이 NULL입니다. CreateBaseComboBoxRollOver()를 먼저 호출하십시오." );
		return UINT_MAX;
	}

	return m_pTextBox->GetTextData( nIndex );
}

void CBasicComboBoxRollOver::SetVisibleThumb( bool bVisible )
{
	if ( !m_pScrollBar )
	{
		GASSERT ( 0 && "m_pScrollBar이 NULL입니다. CreateBaseComboBoxRollOver()를 먼저 호출하십시오." );
		return;
	}

	m_pScrollBar->SetVisibleThumb( bVisible );
}