/////////////////////////////////////////////////////////////////////////////
//	FileName	: CharViewWindow.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "CharViewWindow.h"
#include "CharViewRender.h"

#include "../BasicLineBox.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"
#include "../ItemImage.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- Engine/DxCommon/DxClubMan.h"
#include "BasicTextBox.h"
#include "BasicButton.h"
#include "UIWindowTitle.h"
#include "UIWindowBody.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharViewWindow::CCharViewWindow()
	: m_pd3dDevice(NULL)
	
	, m_pClassImage(NULL)
	, m_pSchoolImage(NULL)
	, m_pGuildImage(NULL)
	, m_pRender(NULL)
{
	memset(m_pLBoxA_,		NULL, sizeof(m_pLBoxA_));
	memset(m_pText_,		NULL, sizeof(m_pText_));
	memset(m_pItemImage_,	NULL, sizeof(m_pItemImage_));
}

CCharViewWindow::~CCharViewWindow()
{
}

void CCharViewWindow::CreateSubControl()
{
	for (int i = 0; i < nMAX_LBOXA; i++)
	{
		m_pLBoxA_[i] = new CBasicLineBox;
		m_pLBoxA_[i]->CreateSub(this, "BASIC_LINE_BOX_BODY_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBoxA_[i]->CreateBaseBoxOuterWhite("BASIC_LINE_BOX_BODY_OUTER");
		RegisterControl(m_pLBoxA_[i]);
	}

	CD3DFontPar* pFont8		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignMid		= TEXT_ALIGN_CENTER_X	| TEXT_ALIGN_CENTER_Y;
	const int nAlignLeft	= TEXT_ALIGN_LEFT		| TEXT_ALIGN_CENTER_Y;
	const int nAlignRight	= TEXT_ALIGN_RIGHT		| TEXT_ALIGN_CENTER_Y;

	m_pText_[0] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);
	m_pText_[1] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);

	for (int i = 2; i < 10; i++)
		m_pText_[i] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignLeft);

	for (int i = 10; i < nMAX_TEXT; i++)
		m_pText_[i] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignRight);

	char szTemp[50];

	for (int i = 0; i < SLOT_NSIZE_S_2_OLD; i++)
	{
		sprintf(szTemp, "ITEM_IMAGE_%d", i);

		m_pItemImage_[i] = new CItemImage;
		m_pItemImage_[i]->CreateSub(this, szTemp, UI_FLAG_DEFAULT, ITEM_IMAGE_0 + i);
		m_pItemImage_[i]->CreateSubControl();
		RegisterControl(m_pItemImage_[i]);
	}

	m_pClassImage = new CUIControl;
	m_pClassImage->CreateSub(this, "PK_ANNOUNCER_SLOT_CLASS_IMAGE", UI_FLAG_DEFAULT, CLASS_DUMMY);
	RegisterControl(m_pClassImage);

	m_pSchoolImage = new CUIControl;
	m_pSchoolImage->CreateSub(this, "PK_ANNOUNCER_SLOT_SCHOOL_IMAGE", UI_FLAG_DEFAULT, SCHOOL_DUMMY);
	RegisterControl(m_pSchoolImage);

	m_pGuildImage = new CUIControl;
	m_pGuildImage->CreateSub(this, "NAME_DISPLAY_CLUB_MARK", UI_FLAG_DEFAULT, GUILD_DUMMY);
	RegisterControl(m_pGuildImage);

	m_pRender = new CCharViewRender;
	m_pRender->CreateSub(this, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pRender->CreateSubControl();
	RegisterControl(m_pRender);

	DefaultPos();
}

void CCharViewWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	if (ITEM_IMAGE_0 <= ControlID && ControlID < ITEM_IMAGE_END)
	{
		int nIndex = ControlID - ITEM_IMAGE_0;

		if (CHECK_MOUSE_IN(dwMsg))
			if (m_sCharView.m_PutOnItems_[nIndex].sNativeID != NATIVEID_NULL())
				CInnerInterface::GetInstance().SHOW_ITEM_INFO(m_sCharView.m_PutOnItems_[nIndex], FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX);
	}

	switch (ControlID)
	{
	case CLASS_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sCharView.nChaClass));
			CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
		}
	}
	break;

	case SCHOOL_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sCharView.wChaSchool);
			CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
		}
	}
	break;

	case GUILD_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			BOOL bGuild = m_sCharView.wGuNum ? TRUE : FALSE;
			if (bGuild)
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sCharView.szGuName, NS_UITEXTCOLOR::WHITE);
		}
	}
	break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

HRESULT CCharViewWindow::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIWindowEx::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CCharViewWindow::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (!CInnerInterface::GetInstance().IsVisibleGroup(GLOBAL_RANKING_WINDOW))
	{
		m_pGuildImage->SetVisibleSingle(FALSE);
		return CUIGroup::Render(pd3dDevice);
	}

	HRESULT hr = S_OK;

	m_pGuildImage->SetVisibleSingle(FALSE);

	{
		hr = CUIGroup::Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}

	BOOL bGuild = m_sCharView.wGuNum ? TRUE : FALSE;
	if (bGuild && m_sCharView.wGuNum > 0)
	{
		m_pGuildImage->SetVisibleSingle(TRUE);
		const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sCharView.wGuNum, m_sCharView.wGuMarkVer);

		m_pGuildImage->SetTexturePos(0, sMarkData.vTex_1_LU);
		m_pGuildImage->SetTexturePos(1, sMarkData.vTex_2_RU);
		m_pGuildImage->SetTexturePos(3, sMarkData.vTex_3_LD);
		m_pGuildImage->SetTexturePos(2, sMarkData.vTex_4_RD);

		const UIRECT& rcGlobalPos = m_pGuildImage->GetGlobalPos();
		m_pGuildImage->SetGlobalPos(UIRECT(
			ceil(rcGlobalPos.left),
			ceil(rcGlobalPos.top),
			ceil(rcGlobalPos.sizeX),
			ceil(rcGlobalPos.sizeY)));

		m_pGuildImage->SetTexture(sMarkData.pddsTexture);
		m_pGuildImage->Render(pd3dDevice);
	}

	return hr;
}

CBasicTextBox* CCharViewWindow::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CCharViewWindow::DefaultPos()
{
	UIRECT RcOLD;
	UIRECT RcNEW1;


	UIRECT RcLBoxA_[nMAX_LBOXA];
	UIRECT RcText_[nMAX_TEXT];
	UIRECT RcItemImage_[SLOT_NSIZE_S_2_OLD];
	
	RcLBoxA_[0] = UIRECT(5.00, 22.00, 376.00, 290.00);
	RcLBoxA_[1] = UIRECT(5.00, 22.00, 376.00, 20.00);
	RcLBoxA_[2] = UIRECT(45.00, 46.00, 296.00, 141.00);
	RcLBoxA_[3] = UIRECT(9.00, 226.00, 86.00, 40.00);
	RcLBoxA_[4] = UIRECT(9.00, 268.00, 86.00, 40.00);
	RcLBoxA_[5] = UIRECT(291.00, 226.00, 86.00, 40.00);
	RcLBoxA_[6] = UIRECT(291.00, 268.00, 86.00, 40.00);
	RcLBoxA_[7] = UIRECT(97.00, 226.00, 192.00, 82.00);

	RcText_[0] = UIRECT(5.00, 22.00, 376.00, 19.00);
	RcText_[1] = UIRECT(9.00, 226.00, 86.00, 39.00);
	RcText_[2] = UIRECT(17.00, 268.00, 78.00, 39.00);
	RcText_[3] = UIRECT(296.00, 226.00, 81.00, 39.00);
	RcText_[4] = UIRECT(300.00, 268.00, 77.00, 39.00);
	RcText_[5] = UIRECT(102.00, 228.00, 120.00, 17.00);
	RcText_[6] = UIRECT(102.00, 243.00, 120.00, 17.00);
	RcText_[7] = UIRECT(102.00, 258.00, 120.00, 17.00);
	RcText_[8] = UIRECT(102.00, 273.00, 120.00, 17.00);
	RcText_[9] = UIRECT(102.00, 288.00, 120.00, 17.00);
	RcText_[10] = UIRECT(164.00, 228.00, 120.00, 17.00);
	RcText_[11] = UIRECT(164.00, 243.00, 120.00, 17.00);
	RcText_[12] = UIRECT(164.00, 258.00, 120.00, 17.00);
	RcText_[13] = UIRECT(164.00, 273.00, 120.00, 17.00);
	RcText_[14] = UIRECT(164.00, 288.00, 120.00, 17.00);

	RcItemImage_[0] = UIRECT(8.00, 45.00, 35.00, 35.00);
	RcItemImage_[1] = UIRECT(8.00, 81.00, 35.00, 35.00);
	RcItemImage_[2] = UIRECT(8.00, 117.00, 35.00, 35.00);
	RcItemImage_[3] = UIRECT(8.00, 153.00, 35.00, 35.00);
	RcItemImage_[4] = UIRECT(8.00, 189.00, 35.00, 35.00);
	RcItemImage_[5] = UIRECT(44.00, 189.00, 35.00, 35.00);
	RcItemImage_[6] = UIRECT(80.00, 189.00, 35.00, 35.00);
	RcItemImage_[7] = UIRECT(343.00, 45.00, 35.00, 35.00);
	RcItemImage_[8] = UIRECT(343.00, 81.00, 35.00, 35.00);
	RcItemImage_[9] = UIRECT(343.00, 117.00, 35.00, 35.00);
	RcItemImage_[10] = UIRECT(343.00, 153.00, 35.00, 35.00);
	RcItemImage_[11] = UIRECT(271.00, 189.00, 35.00, 35.00);
	RcItemImage_[12] = UIRECT(307.00, 189.00, 35.00, 35.00);
	RcItemImage_[13] = UIRECT(343.00, 189.00, 35.00, 35.00);


	// m_pLBoxA_
	for (int i = 0; i < nMAX_LBOXA; i++)
	{
		RcNEW1 = RcLBoxA_[i];

		RcOLD = m_pLBoxA_[i]->GetLocalPos();
		m_pLBoxA_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pLBoxA_[i]->SetLocalPos(RcNEW1);
		m_pLBoxA_[i]->SetGlobalPos(RcNEW1);
	}

	// m_pText_
	for (int i = 0; i < nMAX_TEXT; i++)
	{
		RcNEW1 = RcText_[i];

		RcOLD = m_pText_[i]->GetLocalPos();
		m_pText_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pText_[i]->SetLocalPos(RcNEW1);
	}

	// m_pItemImage_
	for (int i = 0; i < SLOT_NSIZE_S_2_OLD; i++)
	{
		RcNEW1 = RcItemImage_[i];

		RcOLD = m_pItemImage_[i]->GetLocalPos();
		m_pItemImage_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pItemImage_[i]->SetLocalPos(RcNEW1);
	}

	// m_pClassImage
	{
		RcNEW1 = UIRECT(62.00, 275.00, 26.00, 26.00);

		RcOLD = m_pClassImage->GetLocalPos();
		m_pClassImage->AlignSubControl(RcOLD, RcNEW1);
		m_pClassImage->SetLocalPos(RcNEW1);
	}

	// m_pSchoolImage
	{
		RcNEW1 = UIRECT(344.00, 233.00, 26.00, 26.00);

		RcOLD = m_pSchoolImage->GetLocalPos();
		m_pSchoolImage->AlignSubControl(RcOLD, RcNEW1);
		m_pSchoolImage->SetLocalPos(RcNEW1);
	}

	// m_pGuildImage
	{
		RcNEW1 = UIRECT(346.00, 283.00, 16.00, 11.00);

		RcOLD = m_pGuildImage->GetLocalPos();
		m_pGuildImage->AlignSubControl(RcOLD, RcNEW1);
		m_pGuildImage->SetLocalPos(RcNEW1);
	}

	// m_pRender
	{
		RcNEW1 = UIRECT(45.00, 46.00, 296.00, 141.00);

		RcOLD = m_pRender->GetLocalPos();
		m_pRender->AlignSubControl(RcOLD, RcNEW1);
		m_pRender->SetLocalPos(RcNEW1);
	}

	{
		D3DXVECTOR2 vPos;
		vPos.x = GetGlobalPos().left;
		vPos.y = GetGlobalPos().top;

		RcNEW1 = UIRECT(0.00, 0.00, 386.00, 317.00);

		SetLocalPos(RcNEW1);
		SetGlobalPos(RcNEW1);

		{
			RcOLD = GetTitle()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetLocalPos().left,
				GetLocalPos().top,
				GetLocalPos().sizeX,
				RcOLD.sizeY
				);
			GetTitle()->AlignSubControl(RcOLD, RcNEW1);
			GetTitle()->SetLocalPos(RcNEW1);
			GetTitle()->SetGlobalPos(RcNEW1);

#if defined(KRT_PARAM) || defined(KR_PARAM) || defined(TW_PARAM) || defined(JP_PARAM) || defined(CH_PARAM) || defined ( HK_PARAM )
			GetTitleFocus()->AlignSubControl(RcOLD, RcNEW1);
			GetTitleFocus()->SetLocalPos(RcNEW1);
			GetTitleFocus()->SetGlobalPos(RcNEW1);
#endif
		}

		{
			RcOLD = GetBody()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetLocalPos().left,
				GetTitle()->GetLocalPos().top + GetTitle()->GetLocalPos().sizeY,
				GetLocalPos().sizeX,
				GetLocalPos().sizeY - GetTitle()->GetLocalPos().sizeY
				);
			GetBody()->AlignSubControl(RcOLD, RcNEW1);
			GetBody()->SetLocalPos(RcNEW1);
			GetBody()->SetGlobalPos(RcNEW1);
		}

		{
			RcOLD = GetClose()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetTitle()->GetLocalPos().left + GetTitle()->GetLocalPos().sizeX - 20.0f,
				RcOLD.top,
				RcOLD.sizeX,
				RcOLD.sizeY
				);
			GetClose()->AlignSubControl(RcOLD, RcNEW1);
			GetClose()->SetLocalPos(RcNEW1);
			GetClose()->SetGlobalPos(RcNEW1);
		}

		SetGlobalPos(vPos);
	}

	this->SetTitleName((char*)ID2GAMEWORD("CHAR_VIEW_WINDOW"));
}

void CCharViewWindow::RESET()
{
	m_sCharView = SCHAR_VIEW();

	for (int i = 0; i < nMAX_TEXT; i++)
		m_pText_[i]->ClearText();

	for (int i = 0; i < SLOT_NSIZE_S_2_OLD; i++)
		m_pItemImage_[i]->ResetItem();

	m_pClassImage->SetVisibleSingle(FALSE);
	m_pSchoolImage->SetVisibleSingle(FALSE);
}

void CCharViewWindow::AddCharView(SCHAR_VIEW sCharView)
{
	m_sCharView = sCharView;

	m_pText_[0]->SetOneLineText(m_sCharView.szChaName, NS_UITEXTCOLOR::GOLD);

	for (int i = 0; i < SLOT_NSIZE_S_2_OLD; i++)
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem(m_sCharView.m_PutOnItems_[i].sNativeID);
		if (pItem)
			m_pItemImage_[i]->SetItem(pItem->sBasicOp.sICONID, pItem->GetInventoryFile());
	}

	CBasicTextBox::MULTITEXT ml_Text_Level;
	ml_Text_Level.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 0);
	ml_Text_Level.dwColor[0] = NS_UITEXTCOLOR::GOLD;
	ml_Text_Level.strText[1].Format(" %d", m_sCharView.wChaLevel);
	ml_Text_Level.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
	m_pText_[1]->ClearText();
	m_pText_[1]->AddMultiText(ml_Text_Level);

	m_pText_[2]->SetOneLineText((char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 1), NS_UITEXTCOLOR::GOLD);

	CString strClass;
	switch (m_sCharView.nChaClass)
	{
	case 1:		strClass = "BR_M.png"; break;
	case 2:		strClass = "SM_M.png"; break;
	case 4:		strClass = "AR_W.png"; break;
	case 8:		strClass = "SH_W.png"; break;
	case 64:	strClass = "BR_W.png"; break;
	case 128:	strClass = "SM_W.png"; break;
	case 256:	strClass = "AR_M.png"; break;
	case 512:	strClass = "SH_M.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);
	m_pClassImage->SetVisibleSingle(TRUE);

	m_pText_[3]->SetOneLineText((char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 2), NS_UITEXTCOLOR::GOLD);

	CString strSchool;
	switch (m_sCharView.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);
	m_pSchoolImage->SetVisibleSingle(TRUE);

	m_pText_[4]->SetOneLineText((char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 3), NS_UITEXTCOLOR::GOLD);

	CBasicTextBox::MULTITEXT ml_TextLeft_Con;
	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 4);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d-%d", m_sCharView.nAttLow, m_sCharView.nAttHigh);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[5]->ClearText();
		m_pText_[5]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 5);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.nDef);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[6]->ClearText();
		m_pText_[6]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 6);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.wMelee);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[7]->ClearText();
		m_pText_[7]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 7);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.wRange);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[8]->ClearText();
		m_pText_[8]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 8);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.wMagic);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[9]->ClearText();
		m_pText_[9]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 9);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.dwMaxHP);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[10]->ClearText();
		m_pText_[10]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 10);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.nMaxMP);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[11]->ClearText();
		m_pText_[11]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 11);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.nMaxSP);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[12]->ClearText();
		m_pText_[12]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 12);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.nAcc);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[13]->ClearText();
		m_pText_[13]->AddMultiText(ml_TextLeft_Con);
	}

	{
		ml_TextLeft_Con.strText[0] = (char*)ID2GAMEWORD("CHAR_VIEW_WINDOW_TEXT", 13);
		ml_TextLeft_Con.dwColor[0] = NS_UITEXTCOLOR::GOLD;
		ml_TextLeft_Con.strText[1].Format(" %d", m_sCharView.nEva);
		ml_TextLeft_Con.dwColor[1] = NS_UITEXTCOLOR::ENABLE;
		m_pText_[14]->ClearText();
		m_pText_[14]->AddMultiText(ml_TextLeft_Con);
	}

	m_pRender->StartRender(m_sCharView);
}