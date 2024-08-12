#include "StdAfx.h"
#include "./BasicTextButton.h"
#include "./BasicTextButtonImage.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "./BasicTextBox.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CBasicTextButton::CreateBaseButton( const char* szButtonControl, 
										 int nType, 
										 WORD wFlipMode, 
										 const char* szText,
										 DWORD dwFlags )
{
	GASSERT( szButtonControl && szText );

	m_wFlipMode = wFlipMode;
	
	if ( m_wFlipMode == RADIO_FLIP )	SetRadioButton ();
	else								SetGeneralButton ();

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, dwFlags );
	CreateButtonImage( nType, pFont, (m_wFlipMode==CLICK_FLIP?TRUE:FALSE) );

    CUIControl TempControl;
	TempControl.Create ( 1, szButtonControl );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );

	CString strText( szText );
	//m_pButtonText->SetOneLineText( strText, NS_UITEXTCOLOR::BLACK );
	m_pButtonText->SetOneLineText( strText );
}

void CBasicTextButton::CreateButtonImage ( int nType, CD3DFontPar* pFont, BOOL bReversal )
{
	//	기본색이 회색, 플립된 것이 금색!!!!!
	//
	const int nIMAGE = 3;

	CString strButton("BASIC_TEXT_BUTTON_IMAGE");
	CString strTextBox("BASIC_TEXT_BUTTON_IMAGE_TEXTBOX");
	CString strButtonImage[nIMAGE];
	CString strButtonImageFlip[nIMAGE];
	
	// 플립이 반전된 버튼을 생성한다.
	if( bReversal )
	{
		strButtonImage[0] = "BASIC_TEXT_BUTTON_IMAGE_LEFT_F";
		strButtonImage[1] = "BASIC_TEXT_BUTTON_IMAGE_CENTER_F";
		strButtonImage[2] = "BASIC_TEXT_BUTTON_IMAGE_RIGHT_F";

		strButtonImageFlip[0] = "BASIC_TEXT_BUTTON_IMAGE_LEFT";
		strButtonImageFlip[1] = "BASIC_TEXT_BUTTON_IMAGE_CENTER";
		strButtonImageFlip[2] = "BASIC_TEXT_BUTTON_IMAGE_RIGHT";
	}
	else
	{
		strButtonImage[0] = "BASIC_TEXT_BUTTON_IMAGE_LEFT";
		strButtonImage[1] = "BASIC_TEXT_BUTTON_IMAGE_CENTER";
		strButtonImage[2] = "BASIC_TEXT_BUTTON_IMAGE_RIGHT";

		strButtonImageFlip[0] = "BASIC_TEXT_BUTTON_IMAGE_LEFT_F";
		strButtonImageFlip[1] = "BASIC_TEXT_BUTTON_IMAGE_CENTER_F";
		strButtonImageFlip[2] = "BASIC_TEXT_BUTTON_IMAGE_RIGHT_F";
	}

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 AutoSystem
	const int nSIZENUMBER[SIZE_NSIZE] = { 14, 16, 161, 18, 19, 191, 22, 23, 24, 40 };
	/////////////////////////////////////////////////////////////////////////////

	{	//	스트링 변환
		CString strNumber;
		strNumber.Format ( "%d", nSIZENUMBER[nType] );

		strButton += strNumber;
		for ( int i = 0; i < nIMAGE; i++ )	
		{
			strButtonImage[i] += strNumber;
			strButtonImageFlip[i] += strNumber;
		}
        strTextBox += strNumber;        
	}

	{	//	라디오버튼과 아닌것에 따른 플립
		CString strButtonImageTemp[nIMAGE];		
		if ( m_wFlipMode == RADIO_FLIP )
		{
			for ( int i = 0; i < nIMAGE; i++ )
				strButtonImageTemp[i] = strButtonImage[i];
		}
		else
		{
			for ( int i = 0; i < nIMAGE; i++ )
				strButtonImageTemp[i] = strButtonImageFlip[i];
		}

		m_pButton = new CBasicTextButtonImage;
		m_pButton->CreateSub ( this, strButton.GetString(), UI_FLAG_XSIZE );
		m_pButton->CreateButtonImage (	strButtonImageTemp[0].GetString(), 
										strButtonImageTemp[1].GetString(), 
										strButtonImageTemp[2].GetString() );
		RegisterControl ( m_pButton );
	}

	m_pButtonFlip = new CBasicTextButtonImage;
	m_pButtonFlip->CreateSub ( this, strButton.GetString(), UI_FLAG_XSIZE );
	m_pButtonFlip->CreateButtonImage (	strButtonImageFlip[0].GetString(), 
										strButtonImageFlip[1].GetString(), 
										strButtonImageFlip[2].GetString() );
	RegisterControl ( m_pButtonFlip );

	m_pButtonText = new CBasicTextBox;
	m_pButtonText->CreateSub ( this, strTextBox.GetString(), UI_FLAG_XSIZE );
	m_pButtonText->SetFont ( pFont );
	m_pButtonText->SetTextAlign ( TEXT_ALIGN_CENTER_X );	
	RegisterControl ( m_pButtonText );

	m_pButtonTextBack = new CUIControl;
	m_pButtonTextBack->CreateSub ( this, strTextBox.GetString(), UI_FLAG_XSIZE );
	RegisterControl ( m_pButtonTextBack );
}

void CBasicTextButton::CreateBaseButtonEx( const char* szButtonControl, 
										  int nType, 
										  WORD wFlipMode, 
										  const char* szText,
										  DWORD dwFlags )
{
	GASSERT( szButtonControl && szText );

	m_wFlipMode = wFlipMode;

	if ( m_wFlipMode == RADIO_FLIP )	SetRadioButtonEx ();
	else								SetGeneralButtonEx ();

	
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CreateButtonImage( nType, pFont, (m_wFlipMode==CLICK_FLIP?TRUE:FALSE) );

	CUIControl TempControl;
	TempControl.Create ( 1, szButtonControl );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );

	CString strText( szText );
	//m_pButtonText->SetOneLineText( strText, NS_UITEXTCOLOR::BLACK );
	m_pButtonText->SetOneLineText( strText );
}