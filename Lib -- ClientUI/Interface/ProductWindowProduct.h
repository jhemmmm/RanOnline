/*!
 * \file ProductWindowType.h
 *
 * \author Juver
 * \date October 2017
 *
 * 
 */
#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

#include "GLItemMix.h"

#define PRODUCT_INFO_UPDATE_TIME 0.1f

class CBasicTextBox;
class CItemImage;
class CBasicTextButton;
class CBasicProgressBar;

class CProductWindowProduct : public CUIGroup
{
private:
	enum
	{
		PRODUCT_ITEM_IMAGE_RES = NO_ID + 1,
		PRODUCT_ITEM_IMAGE_REQ_0,
		PRODUCT_ITEM_IMAGE_REQ_1,
		PRODUCT_ITEM_IMAGE_REQ_2,
		PRODUCT_ITEM_IMAGE_REQ_3,
		PRODUCT_ITEM_IMAGE_REQ_4,
		PRODUCT_BUTTON_PRODUCTION,
	};


public:
	CProductWindowProduct ();
	virtual	~CProductWindowProduct ();


	void	CreateSubControl ();

private:
	ITEM_MIX			m_sItemMix;
	CBasicTextBox*		m_pTextProductLevelChar;
	CBasicTextBox*		m_pTextProductChance;
	CBasicTextBox*		m_pTextProductTime;
	CBasicTextBox*		m_pTextMaterial;
	CBasicTextBox*		m_pTextMoney;
	CBasicTextBox*		m_pTextResultNum;
	CBasicTextBox*		m_pTextResultCost;
	CBasicTextBox*		m_pTextInfo;
	CItemImage*			m_pItemImageResult;
	CItemImage*			m_pItemImageRequire[ITEMMIX_ITEMNUM];
	CBasicTextBox*		m_pTextItemRequire[ITEMMIX_ITEMNUM];
	CBasicTextButton*	m_pButtonProduction;
	CBasicProgressBar*	m_pTimer;

private:
	float				m_fUpdateTime;

	
public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	ResetData();
	void	SetData( DWORD dwID );
	void	UpdateInfo();
};