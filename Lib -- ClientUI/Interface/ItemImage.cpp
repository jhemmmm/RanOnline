#include "StdAfx.h"
#include "ItemImage.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "SkillFunc.h"
#include "./BasicProgressBar.h"
#include "./BasicTextBox.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	리차징 컬러
const DWORD	RECHARGECOLOR = D3DCOLOR_ARGB(150,0,0,0);

CItemImage::CItemImage () 
	: m_pItemImageBack(NULL)
	, m_pItemImage ( NULL )
	, m_pd3dDevice ( NULL )
	, m_sICONINDEXBACK ( NATIVEID_NULL () )
	, m_sICONINDEX ( NATIVEID_NULL () )
	, m_TotElapsedTime(0.0f)
	, m_bItemUsed(false)
	, m_bItemEnd(false)
	, m_bTwinkle(true)
	, m_pItemProcess( NULL )
	, m_sNativeID(false)
	, m_pTextBoxUpLeft( NULL )
	, m_pTextBoxUpRight( NULL )
	, m_pTextBoxDownLeft( NULL )
	, m_pTextBoxDownRight( NULL )
{
}

CItemImage::~CItemImage ()
{
}

HRESULT CItemImage::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_pd3dDevice = pd3dDevice;

	return S_OK;
}

void	CItemImage::CreateSubControl ()
{
	m_pItemImageBack = new CUIControl;
	m_pItemImageBack->CreateSub ( this, "ITEM_IMAGE" );	
	m_pItemImageBack->SetUseRender ( TRUE );
	m_pItemImageBack->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemImageBack );

	m_pItemImage = new CUIControl;
	m_pItemImage->CreateSub ( this, "ITEM_IMAGE" );	
	m_pItemImage->SetUseRender ( TRUE );
	m_pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemImage );

	CBasicProgressBar* pItemProcess = new CBasicProgressBar;
	pItemProcess->CreateSub ( this, "ITEM_IMAGE_PROGRESS" );
	pItemProcess->CreateOverImage ( "ITEM_IMAGE_PROGRESS_OVER" );
	pItemProcess->SetType ( CBasicProgressBar::VERTICAL );
	pItemProcess->SetOverImageUseRender ( TRUE );
	pItemProcess->SetOverImageDiffuse ( RECHARGECOLOR );
	pItemProcess->SetControlNameEx ( "프로그래스바" );	
	RegisterControl ( pItemProcess );
	m_pItemProcess = pItemProcess;

}

void	CItemImage::CreateTextBoxUpLeft ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextBoxUpLeft = new CBasicTextBox;
	m_pTextBoxUpLeft->CreateSub ( this, "ITEM_IMAGE_TEXT_UP_LEFT" );
	m_pTextBoxUpLeft->SetFont ( pFont );
	m_pTextBoxUpLeft->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextBoxUpLeft );
}

void	CItemImage::CreateTextBoxUpRight ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextBoxUpRight = new CBasicTextBox;
	m_pTextBoxUpRight->CreateSub ( this, "ITEM_IMAGE_TEXT_UP_RIGHT" );
	m_pTextBoxUpRight->SetFont ( pFont );
	m_pTextBoxUpRight->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( m_pTextBoxUpRight );
}

void	CItemImage::CreateTextBoxDownLeft ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextBoxDownLeft = new CBasicTextBox;
	m_pTextBoxDownLeft->CreateSub ( this, "ITEM_IMAGE_TEXT_DOWN_LEFT" );
	m_pTextBoxDownLeft->SetFont ( pFont );
	m_pTextBoxDownLeft->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextBoxDownLeft );
}

void	CItemImage::CreateTextBoxDownRight ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextBoxDownRight = new CBasicTextBox;
	m_pTextBoxDownRight->CreateSub ( this, "ITEM_IMAGE_TEXT_DOWN_RIGHT" );
	m_pTextBoxDownRight->SetFont ( pFont );
	m_pTextBoxDownRight->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( m_pTextBoxDownRight );
}

void CItemImage::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if( m_bItemUsed && m_bItemEnd )
	{
		m_TotElapsedTime += fElapsedTime;

		if( m_TotElapsedTime < 0.3f )		m_pItemProcess->SetPercent(0);
		else if( m_TotElapsedTime < 0.6f )	m_pItemProcess->SetPercent(1);
		else if( m_TotElapsedTime < 0.9f )	m_pItemProcess->SetPercent(0);
		else if( m_TotElapsedTime < 1.2f )	m_pItemProcess->SetPercent(1);
		else
		{
			m_pItemProcess->SetPercent(0);
			m_TotElapsedTime = 0.0f;
			m_bItemEnd = false;
			m_bItemUsed = false;
		}
	}

	//	쿨타임 표시
    if ( !m_pItemProcess )
	{
		GASSERT ( 0 && "프로그래스바가 만들어지지 않았습니다." );
		return ;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sNativeID );
	if ( !pItem )	return ;

	if ( !GLGaeaClient::GetInstance().GetCharacter()->IsCoolTime( m_sNativeID ) )
	{
		m_bItemEnd = true;
		return;
	}	

	ITEM_COOLTIME* pItemCoolTime = GLGaeaClient::GetInstance().GetCharacter()->GetCoolTime( m_sNativeID );
	if ( !pItemCoolTime )
	{
		m_bItemEnd = true;
		return;
	}

	m_bItemEnd = false;

	__time64_t tCurTime = GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
	__time64_t tUseTime = pItemCoolTime->tUseTime;
	__time64_t tCoolTime = pItemCoolTime->tCoolTime;

	float fCoolTime = tCoolTime - tUseTime;
	float fCurTime = fCoolTime - ( tCurTime - tUseTime );
	
	if ( fCoolTime == 0.0f )
	{
		fCoolTime = 1.0f;
	}

	const float fPercent = fCurTime / fCoolTime;
	m_pItemProcess->SetPercent ( fPercent );
	m_bItemUsed = true;
}
void CItemImage::SetItem ( SNATIVEID sICONINDEX, const char* szTexture, SNATIVEID sNativeID )
{	
	if ( !m_pItemImage )	return;

	//	NOTE
	//		이전 프레임과 아이콘 ID가 같고,
	//		텍스쳐가 동일하다면 같은 이미지 이므로,
	//		더 이상 작업을 수행할 필요가 없다.

	if( m_sNativeID == sNativeID && sNativeID != NATIVEID_NULL() )	return;

	if ( (sICONINDEX == m_sICONINDEX) && (szTexture == m_pItemImage->GetTextureName ()) )	return ;


	ResetItem ();
	SetItemProcess( sNativeID );

	if ( m_pItemImage )
	{
		m_pItemImage->SetTexturePos ( NS_SKILL::GetIconTexurePos ( sICONINDEX ) );
		m_pItemImage->SetTextureName ( szTexture );	

		if ( m_pd3dDevice )
		{	
			m_pItemImage->InitDeviceObjects ( m_pd3dDevice );
			m_pItemImage->SetVisibleSingle ( TRUE );

			m_sICONINDEX = sICONINDEX;
			m_strTextureName = szTexture;
		}
	}
}

void CItemImage::SetBackItem( SNATIVEID sICONINDEX, const char* szTexture )
{	
	if ( m_pItemImageBack )
	{
		if ( (sICONINDEX == m_sICONINDEXBACK) && (szTexture == m_pItemImageBack->GetTextureName ()) )	return ;

		m_pItemImageBack->SetTexturePos ( NS_SKILL::GetIconTexurePos ( sICONINDEX ) );
		m_pItemImageBack->SetTextureName ( szTexture );	

		if ( m_pd3dDevice )
		{	
			m_sICONINDEXBACK = sICONINDEX;

			m_pItemImageBack->InitDeviceObjects ( m_pd3dDevice );
			m_pItemImageBack->SetVisibleSingle ( TRUE );
		}
	}
}

SNATIVEID	 CItemImage::GetItem ()
{
	return m_sICONINDEX;
}

void CItemImage::ResetItem ()
{
	if ( m_pItemImage )
	{
		if ( m_sICONINDEX != NATIVEID_NULL () )
		{
			m_pItemImage->DeleteDeviceObjects ();
			m_pItemImage->SetTextureName ( NULL );
			m_pItemImage->SetVisibleSingle ( FALSE );

			m_sICONINDEX = NATIVEID_NULL ();

			ReSetItemProcess();

			m_strTextureName.Empty ();
		}		
	}

	if ( m_pItemImageBack )
	{
		m_sICONINDEXBACK = NATIVEID_NULL ();

		m_pItemImageBack->DeleteDeviceObjects ();
		m_pItemImageBack->SetTextureName ( NULL );
		m_pItemImageBack->SetVisibleSingle ( FALSE );
	}
}
const CString&	CItemImage::GetItemTextureName () const
{
	return m_strTextureName;
}

void CItemImage::SetItemProcess( SNATIVEID sNativeID )
{
	m_bItemUsed = false;	
	m_sNativeID = sNativeID;

	if ( sNativeID == NATIVEID_NULL() )
	{
		m_pItemProcess->SetVisibleSingle( FALSE );
	}
	else
	{
		m_pItemProcess->SetVisibleSingle( TRUE );
	}

	return;
}

void CItemImage::ReSetItemProcess()
{
	m_bItemUsed = false;
	m_sNativeID = NATIVEID_NULL();
	m_pItemProcess->SetVisibleSingle( FALSE );
	m_pItemProcess->SetPercent( 0.0f );
	
	return;
}

void CItemImage::SetDiffuse(D3DCOLOR _diffuse)
{
	CUIGroup::SetDiffuse( _diffuse );

	m_pItemProcess->SetOverImageDiffuse ( RECHARGECOLOR );
}

void CItemImage::SetTextUpLeft( CString strText, DWORD dwColor )
{
	if ( m_pTextBoxUpLeft )	m_pTextBoxUpLeft->SetText( strText.GetString(), dwColor );
}

void CItemImage::SetTextUpRight( CString strText, DWORD dwColor )
{
	if ( m_pTextBoxUpRight )	m_pTextBoxUpRight->SetText( strText.GetString(), dwColor );
}

void CItemImage::SetTextDownLeft( CString strText, DWORD dwColor )
{
	if ( m_pTextBoxDownLeft )	m_pTextBoxDownLeft->SetText( strText.GetString(), dwColor );
}

void CItemImage::SetTextDownRight( CString strText, DWORD dwColor )
{
	if ( m_pTextBoxDownRight )	m_pTextBoxDownRight->SetText( strText.GetString(), dwColor );
}