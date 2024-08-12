#include "StdAfx.h"
#include "ItemSlot.h"
#include "ItemImage.h"
#include "InnerInterface.h"
#include "GameTextControl.h"
#include "ItemMove.h"
#include "BasicTextBox.h"
#include "BasicButton.h"
#include "BasicTextButton.h"
#include "BasicLineBoxEx.h"
#include "./BasicVarTextBox.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "d3dfont.h"
#include "GLGaeaClient.h"
#include "../Lib -- ClientUI/Interface/IngameItemMall/ItemShopWindow.h" //itemmall
#include "ModalWindow.h"
#include "ModalCallerID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CItemSlot::nOUTOFRANGE = -1;

CItemSlot::CItemSlot () :
	m_pMouseOver ( NULL ),
	m_nMaxColumn ( 0 ),
	//itemmall
	m_bItemShop( false ),
	m_bBLOCK ( false )
{
	SecureZeroMemory ( m_pItemImageArray, sizeof( m_pItemImageArray ) );	
	SecureZeroMemory ( m_pNumberBoxArray, sizeof( m_pNumberBoxArray ) );
}

CItemSlot::~CItemSlot ()
{
}

void CItemSlot::CreateSubControl ( int nMaxColumn, BOOL bNumberUse , BOOL bItemShop ) //itemmall
{
	m_nMaxColumn = nMaxColumn;
 	//itemmall
	if ( !bItemShop )
	{	//	밑바닥 배경
		CString	strSlotBack[nLIMIT_COLUMN] = 
		{
			"ITEM_IMAGE_BACK0",
			"ITEM_IMAGE_BACK1",
			"ITEM_IMAGE_BACK2",
			"ITEM_IMAGE_BACK3",
			"ITEM_IMAGE_BACK4",
			"ITEM_IMAGE_BACK5",
			"ITEM_IMAGE_BACK6",
			"ITEM_IMAGE_BACK7",
			"ITEM_IMAGE_BACK8",
			"ITEM_IMAGE_BACK9"
		};

		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			m_pItemImageArray[i] = CreateItemImage ( strSlotBack[i].GetString (), ITEM_IMAGE0 + i );			
		}
	}
	else
	{
		m_bItemShop = bItemShop;
		CreateItemShopText();
	}

	if ( bNumberUse )
	{
		CreateNumberBox ();
	}

	m_pMouseOver = new CUIControl;
	m_pMouseOver->CreateSub ( this, "ITEM_MOUSE_OVER" );
	RegisterControl ( m_pMouseOver );

	m_pBlock = new CUIControl;
	m_pBlock->CreateSub ( this, "ITEM_SLOT_BLOCK" );	
	RegisterControl ( m_pBlock );
}
//itemmall
CBasicTextButton*  CItemSlot::CreateTextButton ( const char* szButton, UIGUID ControlID, char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE18;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText );
	pTextButton->SetVisibleSingle ( FALSE );
	RegisterControl ( pTextButton );

	return pTextButton;
}

CItemImage*	CItemSlot::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CItemSlot::ResetItemImage ( int nIndex )
{
	if ( nIndex < 0 || m_nMaxColumn <= nIndex )
	{
//		GASSERT ( 0 && "범위를 넘어섭니다." );
		return ;
	}

	CItemImage* pItem = m_pItemImageArray[nIndex];
	pItem->ResetItem ();
	
	m_InvenItem[nIndex].sItemCustom.sNativeID = NATIVEID_NULL ();
}
void CItemSlot::SetFlipItem ( int nIndex, BOOL bFlip )
{
	if ( m_pFlipImage[nIndex] )
	{
		m_pFlipImage[nIndex]->SetVisibleSingle ( bFlip );
	}	
}

BOOL CItemSlot::IsFlipItem ( int nIndex )
{
	if ( m_pFlipImage[nIndex] )
	{
		return m_pFlipImage[nIndex]->IsVisible ();
	}
	else
	{
		return FALSE;
	}
}

CUIControl* CItemSlot::CreateFlipImage ( const char* szFlip )
{
	CUIControl* pFlipImage = new CUIControl;
	pFlipImage->CreateSub ( this, szFlip );
	pFlipImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pFlipImage );

	return pFlipImage;
}

void CItemSlot::SetItemImage ( int nIndex, SINVENITEM& ref_InvenItem )
{
	if ( nIndex < 0 || m_nMaxColumn <= nIndex )
	{
//		GASSERT ( 0 && "범위를 넘어섭니다." );
		return ;
	}

	SITEMCUSTOM &sItemCustom = ref_InvenItem.sItemCustom;
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItem )
	{
		GASSERT ( 0 && "등록되지 않은 ID입니다." );
		return ;
	}
    
	CItemImage* pItemImage = m_pItemImageArray[nIndex];
	pItemImage->SetItem ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), pItem->sBasicOp.sNativeID );

	/*item wrapper, Juver, 2018/01/12 */
	if ( pItem->sBasicOp.emItemType == ITEM_WRAPPER_BOX )
	{
		SITEM* pitem_disguise = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
		if ( pitem_disguise )
			pItemImage->SetBackItem( pitem_disguise->sBasicOp.sICONID, pitem_disguise->GetInventoryFile() );
	}

	m_InvenItem[nIndex] = ref_InvenItem;
	//itemmall
	if ( m_bItemShop )
	{
		for ( int i = 0; i < m_nMaxColumn; i++ )
			{
				m_pItemPrice[i]->ClearText();
				m_pItemStock[i]->ClearText();
				m_pItemName[i]->ClearText();
				m_pItemCur[i]->ClearText();
				m_pItemNameStatic[i]->ClearText();
				m_pItemPriceStatic[i]->ClearText();
				m_pItemStockStatic[i]->ClearText();
				m_pItemNameStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemPriceStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemStockStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemPrice[i]->SetVisibleSingle ( FALSE );
				m_pItemStock[i]->SetVisibleSingle ( FALSE );
				m_pItemName[i]->SetVisibleSingle ( FALSE );
				m_bBButton[i]->SetVisibleSingle ( FALSE );
				m_pItemCur[i]->SetVisibleSingle ( FALSE );
			}

		CString strText1,strText2,strText3;
		strText1.Format("%d",m_InvenItem[nIndex].wItemPrice);
		strText2.Format("%d",m_InvenItem[nIndex].wItemStock);
		if ( m_InvenItem[nIndex].wCurrency == 1 )
			strText3.Format("V-Points");
		else if ( m_InvenItem[nIndex].wCurrency == 2 )
			strText3.Format("E-Points");
		else	strText3.Format("Unkown Currency");

		m_pItemPrice[nIndex]->SetText ( strText1.GetString() , NS_UITEXTCOLOR::WHITE );
		m_pItemPrice[nIndex]->SetVisibleSingle ( TRUE );
		if ( m_InvenItem[nIndex].wItemStock > 50 )
				m_pItemStock[nIndex]->SetText ( strText2.GetString() , NS_UITEXTCOLOR::WHITE );
		else
			m_pItemStock[nIndex]->SetText ( strText2.GetString() , NS_UITEXTCOLOR::ORANGE );

		m_pItemStock[nIndex]->SetVisibleSingle ( TRUE );
		m_pItemName[nIndex]->SetText ( pItem->GetName() , NS_UITEXTCOLOR::WHITE );
		m_pItemName[nIndex]->SetVisibleSingle ( TRUE );
		if ( m_InvenItem[nIndex].wCurrency == 1 )
			m_pItemCur[nIndex]->SetText ( strText3.GetString() , NS_UITEXTCOLOR::GOLD );
		else if ( m_InvenItem[nIndex].wCurrency == 2 )
			m_pItemCur[nIndex]->SetText ( strText3.GetString() , NS_UITEXTCOLOR::PALEGREEN );

		m_pItemNameStatic[nIndex]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 1 ), NS_UITEXTCOLOR::YELLOW );
		m_pItemPriceStatic[nIndex]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 2 ), NS_UITEXTCOLOR::YELLOW );
		m_pItemStockStatic[nIndex]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 3 ), NS_UITEXTCOLOR::YELLOW );	
		
		m_pItemNameStatic[nIndex]->SetVisibleSingle ( TRUE );
		m_pItemPriceStatic[nIndex]->SetVisibleSingle ( TRUE );
		m_pItemStockStatic[nIndex]->SetVisibleSingle ( TRUE );
		m_bBButton[nIndex]->SetVisibleSingle ( TRUE );
	}
}

void CItemSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
    SetItemIndex ( nOUTOFRANGE );
	m_pMouseOver->SetVisibleSingle ( FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	/*Ingame Features , 03/04/2021*/
	BOOL bItemShop = bCheckItemShop();

	m_bItemShop = bItemShop;

	if ( m_bItemShop )
	{
		for ( int i = 0; i < m_nMaxColumn; i++ )
			{
				m_pItemPrice[i]->ClearText();
				m_pItemStock[i]->ClearText();
				m_pItemName[i]->ClearText();
				m_pItemCur[i]->ClearText();
				m_pItemNameStatic[i]->ClearText();
				m_pItemPriceStatic[i]->ClearText();
				m_pItemStockStatic[i]->ClearText();
				m_pItemNameStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemPriceStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemStockStatic[i]->SetVisibleSingle ( FALSE );
				m_pItemPrice[i]->SetVisibleSingle ( FALSE );
				m_pItemStock[i]->SetVisibleSingle ( FALSE );
				m_pItemName[i]->SetVisibleSingle ( FALSE );
				m_pItemCur[i]->SetVisibleSingle ( FALSE );
				m_bBButton[i]->SetVisibleSingle ( FALSE );
				
			}
	}

	if ( m_pNumberBoxArray[0] )
	{
		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			ResetNumber ( i );

			SNATIVEID sNativeID = m_InvenItem[i].sItemCustom.sNativeID;
			if ( sNativeID != NATIVEID_NULL () )
			{	
				SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
				if ( pItemData )
				{
					const WORD wPileNum = pItemData->sDrugOp.wPileNum;
					const WORD wTurnNum = m_InvenItem[i].sItemCustom.wTurnNum;		

					if ( wPileNum>1 || wTurnNum>1 )
					{
						SetNumber ( i, wTurnNum, wPileNum );
					}
					/*Ingame Features , 03/04/2021*/
					if ( bCheckItemShop() )
					{

						CString strText1,strText2,strText3;
						strText1.Format("%d",m_InvenItem[i].wItemPrice);
						strText2.Format("%d",m_InvenItem[i].wItemStock);
						if ( m_InvenItem[i].wCurrency == 1 )
							strText3.Format("V-Points");
						else if ( m_InvenItem[i].wCurrency == 2 )
							strText3.Format("E-Points");
						else	strText3.Format("Unkown Currency");
						
						m_pItemPrice[i]->SetText ( strText1.GetString() , NS_UITEXTCOLOR::WHITE );
						m_pItemPrice[i]->SetVisibleSingle ( TRUE );

						if ( m_InvenItem[i].wItemStock > 50 )
							m_pItemStock[i]->SetText ( strText2.GetString() , NS_UITEXTCOLOR::WHITE );
						else
							m_pItemStock[i]->SetText ( strText2.GetString() , NS_UITEXTCOLOR::ORANGE );

						m_pItemStock[i]->SetVisibleSingle ( TRUE );
						m_pItemName[i]->SetText ( pItemData->GetName() , NS_UITEXTCOLOR::WHITE );
						m_pItemName[i]->SetVisibleSingle ( TRUE );
						if ( m_InvenItem[i].wCurrency == 1 )
							m_pItemCur[i]->SetText ( strText3.GetString() , NS_UITEXTCOLOR::GOLD );
						else if ( m_InvenItem[i].wCurrency == 2 )
							m_pItemCur[i]->SetText ( strText3.GetString() , NS_UITEXTCOLOR::GREENYELLOW );
						m_pItemCur[i]->SetVisibleSingle ( TRUE );
						m_pItemNameStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 1 ), NS_UITEXTCOLOR::YELLOW );
						m_pItemPriceStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 2 ), NS_UITEXTCOLOR::YELLOW );
						m_pItemStockStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 3 ), NS_UITEXTCOLOR::YELLOW );	
						
						m_pItemNameStatic[i]->SetVisibleSingle ( TRUE );
						m_pItemPriceStatic[i]->SetVisibleSingle ( TRUE );
						m_pItemStockStatic[i]->SetVisibleSingle ( TRUE );
						m_bBButton[i]->SetVisibleSingle ( TRUE );
						
					}
				}
			}
		}
	}
}

void CItemSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ITEM_IMAGE0:
	case ITEM_IMAGE1:
	case ITEM_IMAGE2:
	case ITEM_IMAGE3:
	case ITEM_IMAGE4:
	case ITEM_IMAGE5:
	case ITEM_IMAGE6:
	case ITEM_IMAGE7:
	case ITEM_IMAGE8:
	case ITEM_IMAGE9:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_nIndex = ControlID - ITEM_IMAGE0;			

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;								

				//	마우스 표시 테두리
				if ( m_pItemImageArray[m_nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pItemImageArray[m_nIndex]->GetGlobalPos ();
					const UIRECT& rcImageLocalPos = m_pItemImageArray[m_nIndex]->GetLocalPos ();
					m_pMouseOver->SetLocalPos ( rcImageLocalPos );
					m_pMouseOver->SetGlobalPos ( rcImagePos );
					m_pMouseOver->SetVisibleSingle ( TRUE );
				}

				//	스냅, 스킬 이미지 붙이기
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					GASSERT ( 0 && "CItemMove가 널입니다." );
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pItemImageArray[m_nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						

					AddMessageEx ( UIMSG_MOUSEIN_ITEMSLOT | UIMSG_TOTOPPARENT );
				}
			}
		}
		break;
	/*Ingame Features , 03/04/2021*/
	case ITEMSHOP_BUY_BUTTON0:
	case ITEMSHOP_BUY_BUTTON1:
	case ITEMSHOP_BUY_BUTTON2:
	case ITEMSHOP_BUY_BUTTON3:
	case ITEMSHOP_BUY_BUTTON4:
	case ITEMSHOP_BUY_BUTTON5:
	case ITEMSHOP_BUY_BUTTON6:
	case ITEMSHOP_BUY_BUTTON7:
	case ITEMSHOP_BUY_BUTTON8:
	case ITEMSHOP_BUY_BUTTON9:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				
				CItemShopWindow* pItemShopWindow = CInnerInterface::GetInstance().GetItemShop ();
				m_nIndex = ControlID - ITEMSHOP_BUY_BUTTON0;
				if ( dwMsg & UIMSG_LB_UP )
				{
					SITEMCUSTOM &sItemCustom = m_InvenItem[m_nIndex].sItemCustom;
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
					if ( pItem )
					{
						CString strText;
						strText.Format("Are you sure you want to buy Item Name :%s for [%d]", pItem->GetName() ,m_InvenItem[m_nIndex].wItemPrice );
						
						pItemShopWindow->SetPosItem(m_InvenItem[m_nIndex].wPosX,m_InvenItem[m_nIndex].wPosY);
						DoModal ( strText, MODAL_QUESTION, YESNO, MODAL_BUYITEM );//
					}
				}
			}
		}
		break;
	};
}
/*Ingame Features , 03/04/2021*/
CBasicTextBox* CItemSlot::CreateStaticControl ( const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	pStaticText->SetVisibleSingle ( FALSE );
	RegisterControl ( pStaticText );

	return pStaticText;
}
CBasicTextBox* CItemSlot::CreateNumberBox ( const char* szControl )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
    pTextBox->SetFont ( pFont8 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( pTextBox );

	return pTextBox;
}

void CItemSlot::SetNumber ( int nIndex, int nNumber, int nMaxNumber )
{
	CString strNumber;
	strNumber.Format ( "%d", nNumber );
	DWORD dwColor = NS_UITEXTCOLOR::GREENYELLOW;
	if ( nNumber == nMaxNumber ) dwColor = NS_UITEXTCOLOR::ORANGE;
	m_pNumberBoxArray[nIndex]->SetOneLineText ( strNumber, dwColor );
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( TRUE );
}

void CItemSlot::SetNumber2 ( int nIndex, int nNumber )
{
	CString strNumber;
	strNumber.Format ( "+%d", nNumber );
	DWORD dwColor = NS_UITEXTCOLOR::WHITE;
	m_pNumberBoxArray[nIndex]->SetOneLineText ( strNumber, dwColor );
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( TRUE );
}

void CItemSlot::SetNumber3 ( int nIndex, int nNumber )
{
	CString strNumber;
	strNumber.Format ( "+%d", nNumber );
	DWORD dwColor = NS_UITEXTCOLOR::WHITE;
	m_pNumberBoxArray[nIndex]->SetOneLineText ( strNumber, dwColor );
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( TRUE );
}

void CItemSlot::ResetNumber ( int nIndex )
{
	m_pNumberBoxArray[nIndex]->ClearText ();
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( FALSE );
}
/*Ingame Features , 03/04/2021*/
void CItemSlot::CreateItemShopText()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG/*D3DFONT_SHADOW | D3DFONT_ASCII*/ );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	{
		CString	strBackWhite[nLIMIT_COLUMN] = 
		{
			"ITEM_SHOP_IMAGE_BACKWHITE0",
			"ITEM_SHOP_IMAGE_BACKWHITE1",
			"ITEM_SHOP_IMAGE_BACKWHITE2",
			"ITEM_SHOP_IMAGE_BACKWHITE3",
			"ITEM_SHOP_IMAGE_BACKWHITE4",
			"ITEM_SHOP_IMAGE_BACKWHITE5",
			"ITEM_SHOP_IMAGE_BACKWHITE6",
			"ITEM_SHOP_IMAGE_BACKWHITE7",
			"ITEM_SHOP_IMAGE_BACKWHITE8",
			"ITEM_SHOP_IMAGE_BACKWHITE9"
		};

		CString	strSlotBack[nLIMIT_COLUMN] = 
		{
			"ITEM_SHOP_IMAGE_BACK0",
			"ITEM_SHOP_IMAGE_BACK1",
			"ITEM_SHOP_IMAGE_BACK2",
			"ITEM_SHOP_IMAGE_BACK3",
			"ITEM_SHOP_IMAGE_BACK4",
			"ITEM_SHOP_IMAGE_BACK5",
			"ITEM_SHOP_IMAGE_BACK6",
			"ITEM_SHOP_IMAGE_BACK7",
			"ITEM_SHOP_IMAGE_BACK8",
			"ITEM_SHOP_IMAGE_BACK9"
		};

		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			m_pWhiteBack[i] = new CBasicLineBoxEx;
			m_pWhiteBack[i]->CreateSub ( this, strBackWhite[i].GetString (), UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pWhiteBack[i]->CreateBaseBoxDialogueWhite ( "ITEMSHOP_LINEBOX_BODY_WHITE" );
			m_pWhiteBack[i]->SetVisibleSingle ( TRUE );
			RegisterControl ( m_pWhiteBack[i] );

			m_pItemImageArray[i] = CreateItemImage ( strSlotBack[i].GetString (), ITEM_IMAGE0 + i );	
		}

		static CString strStrKeyword[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXT_SLOT0",
			"ITEMSHOP_ITEM_TEXT_SLOT1",
			"ITEMSHOP_ITEM_TEXT_SLOT2",
			"ITEMSHOP_ITEM_TEXT_SLOT3",
			"ITEMSHOP_ITEM_TEXT_SLOT4",
			"ITEMSHOP_ITEM_TEXT_SLOT5",
			"ITEMSHOP_ITEM_TEXT_SLOT6",
			"ITEMSHOP_ITEM_TEXT_SLOT7",
			"ITEMSHOP_ITEM_TEXT_SLOT8",
		};

		CString strStrKeywordP[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTP_SLOT0",
			"ITEMSHOP_ITEM_TEXTP_SLOT1",
			"ITEMSHOP_ITEM_TEXTP_SLOT2",
			"ITEMSHOP_ITEM_TEXTP_SLOT3",
			"ITEMSHOP_ITEM_TEXTP_SLOT4",
			"ITEMSHOP_ITEM_TEXTP_SLOT5",
			"ITEMSHOP_ITEM_TEXTP_SLOT6",
			"ITEMSHOP_ITEM_TEXTP_SLOT7",
			"ITEMSHOP_ITEM_TEXTP_SLOT8",
		};

		CString strStrKeywordS[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTS_SLOT0",
			"ITEMSHOP_ITEM_TEXTS_SLOT1",
			"ITEMSHOP_ITEM_TEXTS_SLOT2",
			"ITEMSHOP_ITEM_TEXTS_SLOT3",
			"ITEMSHOP_ITEM_TEXTS_SLOT4",
			"ITEMSHOP_ITEM_TEXTS_SLOT5",
			"ITEMSHOP_ITEM_TEXTS_SLOT6",
			"ITEMSHOP_ITEM_TEXTS_SLOT7",
			"ITEMSHOP_ITEM_TEXTS_SLOT8",
		};

		CString strStrKeywordN[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTN_SLOT0",
			"ITEMSHOP_ITEM_TEXTN_SLOT1",
			"ITEMSHOP_ITEM_TEXTN_SLOT2",
			"ITEMSHOP_ITEM_TEXTN_SLOT3",
			"ITEMSHOP_ITEM_TEXTN_SLOT4",
			"ITEMSHOP_ITEM_TEXTN_SLOT5",
			"ITEMSHOP_ITEM_TEXTN_SLOT6",
			"ITEMSHOP_ITEM_TEXTN_SLOT7",
			"ITEMSHOP_ITEM_TEXTN_SLOT8",
		};

		CString strStrKeywordPP[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTPP_SLOT0",
			"ITEMSHOP_ITEM_TEXTPP_SLOT1",
			"ITEMSHOP_ITEM_TEXTPP_SLOT2",
			"ITEMSHOP_ITEM_TEXTPP_SLOT3",
			"ITEMSHOP_ITEM_TEXTPP_SLOT4",
			"ITEMSHOP_ITEM_TEXTPP_SLOT5",
			"ITEMSHOP_ITEM_TEXTPP_SLOT6",
			"ITEMSHOP_ITEM_TEXTPP_SLOT7",
			"ITEMSHOP_ITEM_TEXTPP_SLOT8",
		};

		CString strStrKeywordSS[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTSS_SLOT0",
			"ITEMSHOP_ITEM_TEXTSS_SLOT1",
			"ITEMSHOP_ITEM_TEXTSS_SLOT2",
			"ITEMSHOP_ITEM_TEXTSS_SLOT3",
			"ITEMSHOP_ITEM_TEXTSS_SLOT4",
			"ITEMSHOP_ITEM_TEXTSS_SLOT5",
			"ITEMSHOP_ITEM_TEXTSS_SLOT6",
			"ITEMSHOP_ITEM_TEXTSS_SLOT7",
			"ITEMSHOP_ITEM_TEXTSS_SLOT8",
		};

		CString strStrKeywordNN[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTNN_SLOT0",
			"ITEMSHOP_ITEM_TEXTNN_SLOT1",
			"ITEMSHOP_ITEM_TEXTNN_SLOT2",
			"ITEMSHOP_ITEM_TEXTNN_SLOT3",
			"ITEMSHOP_ITEM_TEXTNN_SLOT4",
			"ITEMSHOP_ITEM_TEXTNN_SLOT5",
			"ITEMSHOP_ITEM_TEXTNN_SLOT6",
			"ITEMSHOP_ITEM_TEXTNN_SLOT7",
			"ITEMSHOP_ITEM_TEXTNN_SLOT8",
		};

		CString strStrKeywordButton[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTB_SLOT0",
			"ITEMSHOP_ITEM_TEXTB_SLOT1",
			"ITEMSHOP_ITEM_TEXTB_SLOT2",
			"ITEMSHOP_ITEM_TEXTB_SLOT3",
			"ITEMSHOP_ITEM_TEXTB_SLOT4",
			"ITEMSHOP_ITEM_TEXTB_SLOT5",
			"ITEMSHOP_ITEM_TEXTB_SLOT6",
			"ITEMSHOP_ITEM_TEXTB_SLOT7",
			"ITEMSHOP_ITEM_TEXTB_SLOT8",
		};

		CString strStrKeywordCur[nLIMIT_COLUMN] =
		{
			"ITEMSHOP_ITEM_TEXTC_SLOT0",
			"ITEMSHOP_ITEM_TEXTC_SLOT1",
			"ITEMSHOP_ITEM_TEXTC_SLOT2",
			"ITEMSHOP_ITEM_TEXTC_SLOT3",
			"ITEMSHOP_ITEM_TEXTC_SLOT4",
			"ITEMSHOP_ITEM_TEXTC_SLOT5",
			"ITEMSHOP_ITEM_TEXTC_SLOT6",
			"ITEMSHOP_ITEM_TEXTC_SLOT7",
			"ITEMSHOP_ITEM_TEXTC_SLOT8",
		};
		for ( int i = 0; i < m_nMaxColumn; i++ )
			{
				m_bBButton[i] = CreateTextButton ( strStrKeywordButton[i].GetString() , ITEMSHOP_BUY_BUTTON0 + i, const_cast<char*>(ID2GAMEWORD("ITEMSHOP_BUY_BUTTON")) );
			}

			for ( int i = 0; i < m_nMaxColumn; i++ )
			{
				m_pItemNameStatic[i] = CreateStaticControl ( strStrKeywordNN[i].GetString (), pFont, TEXT_ALIGN_LEFT );
				m_pItemPriceStatic[i] = CreateStaticControl ( strStrKeywordPP[i].GetString (), pFont, TEXT_ALIGN_LEFT );
				m_pItemStockStatic[i] = CreateStaticControl ( strStrKeywordSS[i].GetString (), pFont, TEXT_ALIGN_LEFT );
				m_pItemName[i] = CreateStaticControl ( strStrKeywordN[i].GetString (), pFont10, TEXT_ALIGN_LEFT );
				m_pItemPrice[i] = CreateStaticControl ( strStrKeywordP[i].GetString (), pFont, TEXT_ALIGN_LEFT );
				m_pItemStock[i] = CreateStaticControl ( strStrKeywordS[i].GetString (), pFont, TEXT_ALIGN_LEFT );
				m_pItemCur[i] = CreateStaticControl ( strStrKeywordCur[i].GetString (), pFont10, TEXT_ALIGN_LEFT );
				
				
				m_pItemNameStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 1 ), NS_UITEXTCOLOR::YELLOW );
				m_pItemPriceStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 2 ), NS_UITEXTCOLOR::YELLOW );
				m_pItemStockStatic[i]->SetText( ID2GAMEWORD( "ITEMSHOP_TEXTBOX", 3 ), NS_UITEXTCOLOR::YELLOW );	

			}
	}
}
void CItemSlot::CreateNumberBox ()
{
	{	//	밑바닥 배경
		CString	strSlotBack[nLIMIT_COLUMN] = 
		{
			"ITEM_NUMBER0",
			"ITEM_NUMBER1",
			"ITEM_NUMBER2",
			"ITEM_NUMBER3",
			"ITEM_NUMBER4",
			"ITEM_NUMBER5",
			"ITEM_NUMBER6",
			"ITEM_NUMBER7",
			"ITEM_NUMBER8",
			"ITEM_NUMBER9"
		};

		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			m_pNumberBoxArray[i] = CreateNumberBox ( strSlotBack[i].GetString () );			
		}
	}
}

void	CItemSlot::SetBLOCK ()
{
	m_bBLOCK = true;
	m_pBlock->SetUseRender ( TRUE );
	m_pBlock->SetVisibleSingle ( TRUE );
	m_pBlock->SetDiffuse (D3DCOLOR_ARGB(125,0,0,0));
}

void	CItemSlot::ResetBLOCK ()
{
	m_bBLOCK = false;
	m_pBlock->SetUseRender ( FALSE );
	m_pBlock->SetVisibleSingle ( FALSE );
}