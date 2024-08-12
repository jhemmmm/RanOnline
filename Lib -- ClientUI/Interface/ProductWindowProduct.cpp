#include "StdAfx.h"
#include "ProductWindowProduct.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"
#include "ItemImage.h"
#include "BasicProgressBar.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#include "GLItemMixMan.h"
#include "GLItemMan.h"

#include "InnerInterface.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProductWindowProduct::CProductWindowProduct ()
	: m_pTextProductLevelChar(NULL)
	, m_pTextProductChance(NULL)
	, m_pTextProductTime(NULL)
	, m_pTextMaterial(NULL)
	, m_pItemImageResult(NULL)
	, m_pTextMoney(NULL)
	, m_pTextResultNum(NULL)
	, m_pTextResultCost(NULL)
	, m_pTextInfo(NULL)
	, m_pButtonProduction(NULL)
	, m_pTimer(NULL)
	, m_fUpdateTime(0.0f)
{
	for( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		m_pItemImageRequire[i] = NULL;
		m_pTextItemRequire[i] = NULL;
	}

	m_sItemMix = ITEM_MIX();
}

CProductWindowProduct::~CProductWindowProduct ()
{
}

void CProductWindowProduct::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_ITEMNAME" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_MIDDLE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_MIDDLE_LEFT" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_MIDDLE_RIGHT" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_BOTTOM" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "RAN_PRODUCT_WINDOW_GOODS_RESULT_ITEMSLOT" );
	RegisterControl ( pBasicLineBox );

	m_pTextProductLevelChar = new CBasicTextBox;
	m_pTextProductLevelChar->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_PRODUCT_LEVEL_CHAR_TEXT" );
	m_pTextProductLevelChar->SetFont ( pFont9 );
	m_pTextProductLevelChar->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextProductLevelChar->SetText( "--" );
	RegisterControl ( m_pTextProductLevelChar );

	m_pTextProductChance = new CBasicTextBox;
	m_pTextProductChance->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_PRODUCTCHANCE_TEXT" );
	m_pTextProductChance->SetFont ( pFont9 );
	m_pTextProductChance->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextProductChance->SetText( "--" );
	RegisterControl ( m_pTextProductChance );

	m_pTextProductTime = new CBasicTextBox;
	m_pTextProductTime->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_PRODUCTTIME_TEXT" );
	m_pTextProductTime->SetFont ( pFont9 );
	m_pTextProductTime->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextProductTime->SetText( "--" );
	RegisterControl ( m_pTextProductTime );

	m_pTextMaterial = new CBasicTextBox;
	m_pTextMaterial->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_NEEDMATERIAL_TEXT" );
	m_pTextMaterial->SetFont ( pFont9 );
	m_pTextMaterial->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextMaterial->SetText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 6 ), NS_UITEXTCOLOR::PALEGREEN );
	RegisterControl ( m_pTextMaterial );

	m_pTextMoney = new CBasicTextBox;
	m_pTextMoney->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_MONEY_TEXT" );
	m_pTextMoney->SetFont ( pFont9 );
	m_pTextMoney->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMoney->SetText( "--" );
	RegisterControl ( m_pTextMoney );

	m_pTextResultNum = new CBasicTextBox;
	m_pTextResultNum->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_ITEM_NUM_TEXT" );
	m_pTextResultNum->SetFont ( pFont9 );
	m_pTextResultNum->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMoney->SetText( "--" );
	RegisterControl ( m_pTextResultNum );

	m_pTextResultCost = new CBasicTextBox;
	m_pTextResultCost->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_ITEM_COST_TEXT" );
	m_pTextResultCost->SetFont ( pFont9 );
	m_pTextResultCost->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextResultCost->SetText( "--" );
	RegisterControl ( m_pTextResultCost );

	m_pTextInfo = new CBasicTextBox;
	m_pTextInfo->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_INFO_TEXT" );
	m_pTextInfo->SetFont ( pFont9 );
	m_pTextInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextInfo->SetText( "--" );
	RegisterControl ( m_pTextInfo );

	m_pItemImageResult = new CItemImage;
	m_pItemImageResult->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_RESULT_ITEMSLOT", UI_FLAG_DEFAULT, PRODUCT_ITEM_IMAGE_RES );
	m_pItemImageResult->CreateSubControl ();
	m_pItemImageResult->CreateTextBoxDownRight();
	RegisterControl( m_pItemImageResult );

	std::string stritem_slot_req[ITEMMIX_ITEMNUM] = 
	{
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_0",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_1",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_2",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_3",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_4",
	};

	std::string stritem_slot_text[ITEMMIX_ITEMNUM] = 
	{
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_TEXT_0",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_TEXT_1",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_TEXT_2",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_TEXT_3",
		"RAN_PRODUCT_WINDOW_GOODS_ITEMSLOT_TEXT_4",
	};

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		CBasicLineBox* pLineBoxItem = new CBasicLineBox;
		pLineBoxItem->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxItem->CreateBaseBoxQuestList( (char*)stritem_slot_req[i].c_str() );
		RegisterControl ( pLineBoxItem );

		CBasicLineBox* pLineBoxItemText = new CBasicLineBox;
		pLineBoxItemText->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBoxItemText->CreateBaseBoxQuestList( (char*)stritem_slot_text[i].c_str() );
		RegisterControl ( pLineBoxItemText );

		m_pItemImageRequire[i] = new CItemImage;
		m_pItemImageRequire[i]->CreateSub ( this, stritem_slot_req[i].c_str(), UI_FLAG_DEFAULT, PRODUCT_ITEM_IMAGE_REQ_0 + i );
		m_pItemImageRequire[i]->CreateSubControl ();
		//m_pItemImageRequire[i]->CreateTextBoxDownRight();
		RegisterControl( m_pItemImageRequire[i] );

		m_pTextItemRequire[i] = new CBasicTextBox;
		m_pTextItemRequire[i]->CreateSub ( this, stritem_slot_text[i].c_str() );
		m_pTextItemRequire[i]->SetFont ( pFont9 );
		m_pTextItemRequire[i]->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
		m_pTextItemRequire[i]->SetText( "0/0", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextItemRequire[i] );
	}

	CUIControl* pMoneyImage = new CUIControl;
	pMoneyImage->CreateSub ( this, "RAN_PRODUCT_WINDOW_GOODS_MONEY_BUTTON", UI_FLAG_DEFAULT );
	RegisterControl ( pMoneyImage );

	m_pButtonProduction = new CBasicTextButton;
	m_pButtonProduction->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, PRODUCT_BUTTON_PRODUCTION );
	m_pButtonProduction->CreateBaseButton ( "RAN_PRODUCT_WINDOW_PRODUCTION_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_PRODUCT_BUTTON", 0) );
	RegisterControl ( m_pButtonProduction );

	m_pTimer = new CBasicProgressBar;
	m_pTimer->CreateSub ( this, "RAN_PRODUCT_PROGRESS" );
	m_pTimer->CreateOverImage ( "RAN_PRODUCT_PROGRESS_OVER" );
	RegisterControl ( m_pTimer );
	
}

void CProductWindowProduct::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= PRODUCT_INFO_UPDATE_TIME )
	{
		m_fUpdateTime = 0.0f;
		UpdateInfo();
	}

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( pCharacter )
	{
		float fPer = 0.0f;
		if ( pCharacter->m_bItemCompoundTask )
			fPer = pCharacter->m_fItemCompoundTaskTimer / pCharacter->m_fItemCompoundTaskTime;

		if ( m_pTimer )	m_pTimer->SetPercent( fPer );
	}
}


void CProductWindowProduct::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case PRODUCT_ITEM_IMAGE_RES:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sItemMix.sResultItem.sNID;
				sitemcustom.wTurnNum = m_sItemMix.sResultItem.nNum;

				sitemcustom.cDAMAGE = m_sItemMix.cDAMAGE;
				sitemcustom.cDEFENSE = m_sItemMix.cDEFENSE;
				sitemcustom.cRESIST_FIRE = m_sItemMix.cRESIST_FIRE;
				sitemcustom.cRESIST_ICE = m_sItemMix.cRESIST_ICE;
				sitemcustom.cRESIST_ELEC = m_sItemMix.cRESIST_ELEC;
				sitemcustom.cRESIST_POISON = m_sItemMix.cRESIST_POISON;
				sitemcustom.cRESIST_SPIRIT = m_sItemMix.cRESIST_SPIRIT;
				
				if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

				SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
				if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM() * m_sItemMix.sResultItem.nNum;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0, FALSE, TRUE, m_sItemMix.bGenerateRandomValue );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}
			}
		}break;

	case PRODUCT_ITEM_IMAGE_REQ_0:
	case PRODUCT_ITEM_IMAGE_REQ_1:
	case PRODUCT_ITEM_IMAGE_REQ_2:
	case PRODUCT_ITEM_IMAGE_REQ_3:
	case PRODUCT_ITEM_IMAGE_REQ_4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - PRODUCT_ITEM_IMAGE_REQ_0;

				if ( nSelect >= ITEMMIX_ITEMNUM )	return;

				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sItemMix.sMeterialItem[nSelect].sNID;
				sitemcustom.wTurnNum = m_sItemMix.sMeterialItem[nSelect].nNum;
				
				if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}
			}
		}break;

	case PRODUCT_BUTTON_PRODUCTION:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ItemCompound( m_sItemMix.dwKey );
			}
		}break;

	
	};
}

void CProductWindowProduct::ResetData()
{
	m_sItemMix = ITEM_MIX();

	if ( m_pTextProductLevelChar )	m_pTextProductLevelChar->ClearText();
	if ( m_pTextProductChance )		m_pTextProductChance->ClearText();
	if ( m_pTextProductTime )		m_pTextProductTime->ClearText();
	if ( m_pTextMoney )				m_pTextMoney->ClearText();
	if ( m_pTextResultNum )			m_pTextResultNum->ClearText();
	if ( m_pTextResultCost )		m_pTextResultCost->ClearText();
	if ( m_pTextInfo )				m_pTextInfo->ClearText();

	if ( m_pItemImageResult )
	{
		m_pItemImageResult->ResetItem();
		m_pItemImageResult->SetTextDownRight( "", NS_UITEXTCOLOR::WHITE );
	}

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		if ( m_pItemImageRequire[i] )
			m_pItemImageRequire[i]->ResetItem();

		if ( m_pTextItemRequire[i] )
		{
			m_pTextItemRequire[i]->ClearText();
			m_pTextItemRequire[i]->SetText( "0/0", NS_UITEXTCOLOR::WHITE );
		}
	}

	/*if ( m_pButtonProduction )
		m_pButtonProduction->SetVisibleSingle( FALSE );

	if ( m_pTimer )
		m_pTimer->SetVisibleSingle( FALSE );*/
}

void CProductWindowProduct::SetData( DWORD dwID )
{
	const ITEM_MIX* pitem_mix = GLItemMixMan::GetInstance().GetProduct( dwID );
	if ( !pitem_mix )	return;

	m_sItemMix = *pitem_mix;

	CString strTemp;

	if ( m_pTextProductLevelChar )
	{
		m_pTextProductLevelChar->ClearText();

		if ( m_sItemMix.wLevelReq )
		{
			int nIndex = m_pTextProductLevelChar->AddText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 11 ), NS_UITEXTCOLOR::WHITE );
			strTemp.Format( " %d", m_sItemMix.wLevelReq );
			m_pTextProductLevelChar->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
		}
	}

	if ( m_pTextProductChance )
	{
		m_pTextProductChance->ClearText();

		int nIndex = m_pTextProductChance->AddText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 15 ), NS_UITEXTCOLOR::WHITE );
		strTemp.Format( " %d %%", m_sItemMix.dwRate );
		m_pTextProductChance->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
	}

	if ( m_pTextProductTime )
	{
		m_pTextProductTime->ClearText();
		int nIndex = m_pTextProductTime->AddText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 5 ), NS_UITEXTCOLOR::WHITE );
		strTemp.Format( " %g Second", m_sItemMix.fTime );
		m_pTextProductTime->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
	}

	if ( m_pItemImageResult )
	{
		m_pItemImageResult->ResetItem();
		m_pItemImageResult->SetTextDownRight( "", NS_UITEXTCOLOR::WHITE );

		SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( m_sItemMix.sResultItem.sNID );
		if ( pitem_data_result )
		{
			m_pItemImageResult->SetItem( pitem_data_result->sBasicOp.sICONID, pitem_data_result->GetInventoryFile() );

			if ( pitem_data_result->ISPILE() )
			{
				strTemp.Format( "%d", pitem_data_result->GETAPPLYNUM() * m_sItemMix.sResultItem.nNum );
				m_pItemImageResult->SetTextDownRight( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
			}
		}
	}

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{
		if ( m_pItemImageRequire[i] )
		{
			m_pItemImageRequire[i]->ResetItem();

			SITEM* pitem_data_require = GLItemMan::GetInstance().GetItem( m_sItemMix.sMeterialItem[i].sNID );
			if ( pitem_data_require )
			{
				m_pItemImageRequire[i]->SetItem( pitem_data_require->sBasicOp.sICONID, pitem_data_require->GetInventoryFile() );
			}
		}	
	}

	if ( m_pTextResultNum )
	{
		m_pTextResultNum->ClearText();
		int nIndex = m_pTextResultNum->AddText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 7 ), NS_UITEXTCOLOR::WHITE );
		strTemp.Format( " : %d", m_sItemMix.sResultItem.nNum );
		m_pTextResultNum->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
	}

	if ( m_pTextResultCost )
	{
		m_pTextResultCost->ClearText();
		int nIndex = m_pTextResultCost->AddText( ID2GAMEWORD( "RAN_PRODUCT_WINDOW_TEXT", 8 ), NS_UITEXTCOLOR::WHITE );
		strTemp.Format( " : %d", m_sItemMix.dwPrice );
		m_pTextResultCost->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
	}
}

void CProductWindowProduct::UpdateInfo()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	CString strTemp;

	BOOL bitem_cost_fail = FALSE;
	BOOL bitem_num_fail = FALSE;
	BOOL bchar_level_fail = FALSE;

	if( pCharacter->m_lnMoney < m_sItemMix.dwPrice )
		bitem_cost_fail = TRUE;

	if ( pCharacter->m_wLevel < m_sItemMix.wLevelReq )
		bchar_level_fail = TRUE;

	if ( m_pTextMoney )
	{
		m_pTextMoney->ClearText();
		strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( pCharacter->m_lnMoney, 3, "," );
		m_pTextMoney->SetOneLineText( strTemp, NS_UITEXTCOLOR::WHITE );
	}

	GLInventory& ref_Inventory = pCharacter->m_cInventory;

	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{	
		DWORD dwitem_num = 0;
		BOOL bitem_num_complete = FALSE;

		SITEM* pitem_data_require = GLItemMan::GetInstance().GetItem( m_sItemMix.sMeterialItem[i].sNID );
		if ( pitem_data_require )
		{
			if ( pitem_data_require->ISPILE() ){
				dwitem_num = ref_Inventory.CountTurnItem( pitem_data_require->sBasicOp.sNativeID );
			}else{
				SINVENITEM* pinven_item = ref_Inventory.FindItem( pitem_data_require->sBasicOp.sNativeID );
				if ( pinven_item ) dwitem_num = 1;
			}
		}

		if ( dwitem_num >= (DWORD)m_sItemMix.sMeterialItem[i].nNum )
			bitem_num_complete = TRUE;

		if ( !bitem_num_complete )
			bitem_num_fail = TRUE;

		if ( m_pTextItemRequire[i] )
		{
			m_pTextItemRequire[i]->ClearText();
			strTemp.Format( "%d/%d", dwitem_num, m_sItemMix.sMeterialItem[i].nNum );
			m_pTextItemRequire[i]->AddText( strTemp.GetString(), bitem_num_complete? NS_UITEXTCOLOR::WHITE:NS_UITEXTCOLOR::RED );
		}
	}

	if ( m_pTextInfo )
	{
		m_pTextInfo->ClearText();

		if ( m_sItemMix.dwKey != UINT_MAX )
		{
			if ( bitem_cost_fail )
				m_pTextInfo->AddText( ID2GAMEWORD( "RAN_PRODUCT_GOODS_MESSAGE", 4 ), NS_UITEXTCOLOR::ORANGE );
			else if ( bitem_num_fail )
				m_pTextInfo->AddText( ID2GAMEWORD( "RAN_PRODUCT_GOODS_MESSAGE", 0 ), NS_UITEXTCOLOR::ORANGE );
			else if ( bchar_level_fail )
				m_pTextInfo->AddText( ID2GAMEWORD( "RAN_PRODUCT_GOODS_MESSAGE", 5 ), NS_UITEXTCOLOR::ORANGE );
			else
				m_pTextInfo->AddText( ID2GAMEWORD( "RAN_PRODUCT_GOODS_MESSAGE", 2 ), NS_UITEXTCOLOR::ORANGE );
		}
	}

	/*if ( m_pButtonProduction )
	{
		if ( pCharacter->m_bItemCompoundTask )
			m_pButtonProduction->SetOneLineText( ID2GAMEWORD("RAN_PRODUCT_BUTTON", 1) );
		else
			m_pButtonProduction->SetOneLineText( ID2GAMEWORD("RAN_PRODUCT_BUTTON", 0) );
	}*/
}