/////////////////////////////////////////////////////////////////////////////
//	FileName	: CharViewWindow.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../UIWindowEx.h"
#include "DxGlobalStage.h"

#define SLOT_NSIZE_S_2_OLD	14 

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;
class CCharViewRender;
class CCharViewWindow : public CUIWindowEx
{
private:
	enum
	{
		nMAX_LBOXA	= 8,
		nMAX_TEXT	= 15

	};

	enum
	{
		ITEM_IMAGE_0	= ET_CONTROL_NEXT,
		ITEM_IMAGE_END	= ITEM_IMAGE_0 + SLOT_NSIZE_S_2_OLD,

		CLASS_DUMMY,
		SCHOOL_DUMMY,
		GUILD_DUMMY
	};

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

private:
	CBasicLineBox*		m_pLBoxA_[nMAX_LBOXA];
	CBasicTextBox*		m_pText_[nMAX_TEXT];
	CItemImage*			m_pItemImage_[SLOT_NSIZE_S_2_OLD];
	CUIControl*			m_pClassImage;
	CUIControl*			m_pSchoolImage;
	CUIControl*			m_pGuildImage;
	CCharViewRender*	m_pRender;

	SCHAR_VIEW			m_sCharView;

public:
	CCharViewWindow();
	virtual ~CCharViewWindow();

public:
	void CreateSubControl();
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

private:
	CBasicTextBox* CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	void DefaultPos();

public:
	void RESET();
	void AddCharView(SCHAR_VIEW sCharView);
};