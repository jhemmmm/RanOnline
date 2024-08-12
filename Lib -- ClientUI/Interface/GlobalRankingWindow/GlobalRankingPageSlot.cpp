/////////////////////////////////////////////////////////////////////////////
//	FileName	: GlobalRankingPageSlot.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "GlobalRankingPageSlot.h"
#include "GlobalRankingWindow.h"

#include "../BasicLineBox.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- Engine/DxCommon/DxClubMan.h"
#include "./BasicTextBox.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGlobalRankingPageSlot::CGlobalRankingPageSlot()
	: m_nIndex1(-1)
	, m_nIndex2(-1)
	, m_bGuild(FALSE)
	, m_bHaveData(FALSE)
	, m_pd3dDevice(NULL)

	, m_pDummy(NULL)
	, m_pClassImage(NULL)
	, m_pSchoolImage(NULL)
	, m_pGuildImage(NULL)
{
	memset(m_pLBoxA_,	NULL, sizeof(m_pLBoxA_));
	memset(m_pLBoxB_,	NULL, sizeof(m_pLBoxB_));
	memset(m_pText_,	NULL, sizeof(m_pText_));
}

CGlobalRankingPageSlot::~CGlobalRankingPageSlot()
{
}

void CGlobalRankingPageSlot::CreateSubControl(int nIndex1, int nIndex2, BOOL bGuild)
{
	m_nIndex1	= nIndex1;
	m_nIndex2	= nIndex2;
	m_bGuild	= bGuild;

	if (m_bGuild)	m_nSize = 7;
	else			m_nSize = 8;

	for (int i = 0; i < m_nSize; i++)
	{
		m_pLBoxA_[i] = new CBasicLineBox;
		m_pLBoxA_[i]->CreateSub(this, "BASIC_LINE_BOX_BODY_OUTER_BLANK", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBoxA_[i]->CreateBaseBoxOuterBlank("BASIC_LINE_BOX_BODY_OUTER_BLANK");
		RegisterControl(m_pLBoxA_[i]);

		m_pLBoxB_[i] = new CBasicLineBox;
		m_pLBoxB_[i]->CreateSub(this, "BASIC_LINE_BOX_BODY_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBoxB_[i]->CreateBaseBoxOuterWhite("BASIC_LINE_BOX_BODY_OUTER");
		RegisterControl(m_pLBoxB_[i]);
	}

	m_pDummy = new CUIControl;
	m_pDummy->CreateSub(this, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE, DUMMY);
	RegisterControl(m_pDummy);

	CD3DFontPar* pFont8		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont11	= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignMid		= TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;

	m_pText_[0] = CreateStaticControl("BASIC_WINDOW", pFont11,	nAlignMid);
	m_pText_[1] = CreateStaticControl("BASIC_WINDOW", pFont8,	nAlignMid);
	m_pText_[2] = CreateStaticControl("BASIC_WINDOW", pFont8,	nAlignMid);

	if(m_bGuild)	m_pText_[3] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);
	else			m_pText_[3] = CreateStaticControl("BASIC_WINDOW", pFont11,	nAlignMid);

	m_pText_[4] = CreateStaticControl("BASIC_WINDOW", pFont8,	nAlignMid);
	m_pText_[5] = CreateStaticControl("BASIC_WINDOW", pFont8,	nAlignMid);

	m_pClassImage = new CUIControl;
	m_pClassImage->CreateSub(this, "PK_ANNOUNCER_SLOT_CLASS_IMAGE", UI_FLAG_DEFAULT, CLASS_DUMMY);
	RegisterControl(m_pClassImage);

	m_pSchoolImage = new CUIControl;
	m_pSchoolImage->CreateSub(this, "PK_ANNOUNCER_SLOT_SCHOOL_IMAGE", UI_FLAG_DEFAULT, SCHOOL_DUMMY);
	RegisterControl(m_pSchoolImage);

	m_pGuildImage = new CUIControl;
	m_pGuildImage->CreateSub(this, "NAME_DISPLAY_CLUB_MARK", UI_FLAG_DEFAULT, GUILD_DUMMY);
	RegisterControl(m_pGuildImage);

	DefaultPos();
	RESET();
}

void CGlobalRankingPageSlot::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			for (int i = 0; i < m_nSize; i++)
				m_pLBoxB_[i]->SetVisibleSingle(TRUE);

			if (CHECK_LB_UP_LIKE(dwMsg))
			{
				switch (m_nIndex1)
				{
				case 0:
				case 10:
				case 11:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKill.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 3:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillBr.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 4:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillSw.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 5:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillAr.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 6:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillSh.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 7:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillEx.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 8:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillSc.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				/*re-coded LG-7 Global Ranking by DevArt22*/
				case 9:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopKillAs.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;

				case 1:
				{
					if (CInnerInterface::GetInstance().IsVisibleGroup(CHAR_VIEW_WINDOW))
						CInnerInterface::GetInstance().SetCharViewWindowOpen(FALSE);

					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if (pCharacter)
						pCharacter->ReqCharView(m_sTopRich.dwChaNum);

					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetDelay(TRUE);
				}
				break;
				}
			}
		}
		else
		{
			for (int i = 0; i < m_nSize; i++)
				m_pLBoxB_[i]->SetVisibleSingle(FALSE);
		}
	}
	break;

	case CLASS_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			switch (m_nIndex1)
			{
			case 0:
			case 10:
			case 11:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKill.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 3:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillBr.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 4:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillSw.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 5:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillAr.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 6:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillSh.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 7:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillEx.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 8:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillSc.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 9:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopKillAs.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			case 1:
			{
				CString strTemp = (char*)ID2GAMEWORD("CHAR_CLASS_NAME", CharClassToIndex((EMCHARCLASS)m_sTopRich.nChaClass));
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			}
		}
	}
	break;

	case SCHOOL_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			switch (m_nIndex1)
			{
			case 0:
			case 10:
			case 11:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKill.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 3:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillBr.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 4:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillSw.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 5:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillAr.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 6:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillSh.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 7:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillEx.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 8:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillSc.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 9:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopKillAs.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;

			case 1:
			{
				CString strTemp = (char*)ID2GAMEWORD("ACADEMY_NAME", m_sTopRich.wChaSchool);
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(strTemp, NS_UITEXTCOLOR::WHITE);
			}
			break;
			}
		}
	}
	break;

	case GUILD_DUMMY:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			switch (m_nIndex1)
			{
			case 0:
			case 10:
			case 11:
			{
				BOOL bGuild = m_sTopKill.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKill.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 3:
			{
				BOOL bGuild = m_sTopKillBr.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillBr.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 4:
			{
				BOOL bGuild = m_sTopKillSw.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillSw.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 5:
			{
				BOOL bGuild = m_sTopKillAr.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillAr.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 6:
			{
				BOOL bGuild = m_sTopKillSh.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillSh.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 7:
			{
				BOOL bGuild = m_sTopKillEx.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillEx.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 8:
			{
				BOOL bGuild = m_sTopKillSc.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillSc.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			/*re-coded LG-7 Global Ranking by DevArt22*/
			case 9:
			{
				BOOL bGuild = m_sTopKillAs.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopKillAs.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			case 1:
			{
				BOOL bGuild = m_sTopRich.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopRich.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;

			case 2:
			{
				BOOL bGuild = m_sTopGuild.wGuNum ? TRUE : FALSE;
				if (bGuild)
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(m_sTopGuild.szGuName, NS_UITEXTCOLOR::WHITE);
			}
			break;
			}
		}
	}
	break;
	}

	CUIGroup::TranslateUIMessage(ControlID, dwMsg);
}

HRESULT CGlobalRankingPageSlot::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CGlobalRankingPageSlot::Render(LPDIRECT3DDEVICEQ pd3dDevice)
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

	switch (m_nIndex1)
	{
	case 0:
	case 10:
	case 11:
	{
		BOOL bGuild = m_sTopKill.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKill.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKill.wGuNum, m_sTopKill.wGuMarkVer);

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
	}
	break;
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 3:
	{
		BOOL bGuild = m_sTopKillBr.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillBr.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillBr.wGuNum, m_sTopKillBr.wGuMarkVer);

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
	}
	break;
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 4:
	{
		BOOL bGuild = m_sTopKillSw.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillSw.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillSw.wGuNum, m_sTopKillSw.wGuMarkVer);

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
	}
	break;
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 5:
	{
		BOOL bGuild = m_sTopKillAr.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillAr.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillAr.wGuNum, m_sTopKillAr.wGuMarkVer);

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
	}
	break;
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 6:
	{
		BOOL bGuild = m_sTopKillSh.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillSh.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillSh.wGuNum, m_sTopKillSh.wGuMarkVer);

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
	}
	break;

	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 7:
	{
		BOOL bGuild = m_sTopKillEx.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillEx.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillEx.wGuNum, m_sTopKillEx.wGuMarkVer);

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
	}
	break;

	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 8:
	{
		BOOL bGuild = m_sTopKillSc.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillSc.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillSc.wGuNum, m_sTopKillSc.wGuMarkVer);

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
	}
	break;

	/*re-coded LG-7 Global Ranking by DevArt22*/
	case 9:
	{
		BOOL bGuild = m_sTopKillAs.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopKillAs.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopKillAs.wGuNum, m_sTopKillAs.wGuMarkVer);

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
	}
	break;

	case 1:
	{
		BOOL bGuild = m_sTopRich.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopRich.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopRich.wGuNum, m_sTopRich.wGuMarkVer);

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
	}
	break;

	case 2:
	{
		BOOL bGuild = m_sTopGuild.wGuNum ? TRUE : FALSE;
		if (bGuild && m_sTopGuild.wGuNum > 0)
		{
			m_pGuildImage->SetVisibleSingle(TRUE);
			const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTopGuild.wGuNum, m_sTopGuild.wGuMarkVer);

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
	}
	break;
	}

	return hr;
}

CBasicTextBox* CGlobalRankingPageSlot::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CGlobalRankingPageSlot::DefaultPos()
{
	UIRECT RcOLD;
	UIRECT RcNEW1;

	UIRECT RcLBoxA_[8];
	UIRECT RcText_[nMAX_TEXT];

	RcLBoxA_[0] = UIRECT(1.00, 0.00, 41.00, 32.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[1] = UIRECT(49.00, 0.00, 191.00, 32.00);
	else RcLBoxA_[1] = UIRECT(49.00, 0.00, 169.00, 32.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[2] = UIRECT(245.00, 0.00, 52.00, 32.00);
	else RcLBoxA_[2] = UIRECT(225.00, 0.00, 52.00, 32.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[3] = UIRECT(304.00, 0.00, 52.00, 32.00);
	else RcLBoxA_[3] = UIRECT(284.00, 0.00, 52.00, 32.00);

	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[4] = UIRECT(363.00, 0.00, 62.00, 32.00);
	else RcLBoxA_[4] = UIRECT(343.00, 0.00, 52.00, 32.00);

	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[5] = UIRECT(432.00, 0.00, 62.00, 32.00);
	else RcLBoxA_[5] = UIRECT(402.00, 0.00, 52.00, 32.00);

	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcLBoxA_[6] = UIRECT(501.00, 0.00, 181.00, 32.00);
	else RcLBoxA_[6] = UIRECT(461.00, 0.00, 52.00, 32.00);

	RcLBoxA_[7] = UIRECT(520.00, 0.00, 169.00, 32.00);

	RcText_[0] = UIRECT(1.00, 0.00, 41.00, 31.00);
	RcText_[1] = UIRECT(49.00, 0.00, 169.00, 31.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcText_[2] = UIRECT(245.00, 0.00, 52.00, 31.00);
	else RcText_[2] = UIRECT(225.00, 0.00, 52.00, 31.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcText_[3] = UIRECT(370.00, 0.00, 52.00, 31.00);
	else RcText_[3] = UIRECT(461.00, 0.00, 52.00, 31.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	if ( m_bGuild ) RcText_[4] = UIRECT(441.00, 0.00, 52.00, 31.00);
	else RcText_[4] = UIRECT(520.00, 0.00, 169.00, 31.00);
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	RcText_[5] = UIRECT(505.00, 0.00, 169.00, 31.00);

	// m_pLBoxA_
	for (int i = 0; i < m_nSize; i++)
	{
		RcNEW1 = RcLBoxA_[i];

		RcOLD = m_pLBoxA_[i]->GetLocalPos();
		m_pLBoxA_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pLBoxA_[i]->SetLocalPos(RcNEW1);
		m_pLBoxA_[i]->SetGlobalPos(RcNEW1);

		RcOLD = m_pLBoxB_[i]->GetLocalPos();
		m_pLBoxB_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pLBoxB_[i]->SetLocalPos(RcNEW1);
		m_pLBoxB_[i]->SetGlobalPos(RcNEW1);
	}

	// m_pDummy
	{
		RcNEW1 = UIRECT(0.00, 0.00, 688.00, 32.00);

		RcOLD = m_pDummy->GetLocalPos();
		m_pDummy->AlignSubControl(RcOLD, RcNEW1);
		m_pDummy->SetLocalPos(RcNEW1);
	}

	int nRank = m_nIndex2 + 1;
	CString strRank;
	strRank.Format("%d", nRank);
	if (nRank <= 10)					m_pText_[0]->SetOneLineText(strRank, NS_UITEXTCOLOR::GOLD);
	else if (nRank > 10 && nRank <= 20)	m_pText_[0]->SetOneLineText(strRank, NS_UITEXTCOLOR::LIGHTYELLOW);
	else if (nRank > 20 && nRank <= 30)	m_pText_[0]->SetOneLineText(strRank, NS_UITEXTCOLOR::PALEGREEN);
	else if (nRank > 30 && nRank <= 40)	m_pText_[0]->SetOneLineText(strRank, NS_UITEXTCOLOR::LIGHTSKYBLUE);
	else								m_pText_[0]->SetOneLineText(strRank, NS_UITEXTCOLOR::DEFAULT);

	// m_pText_
	for (int i = 0; i < nMAX_TEXT; i++)
	{
		RcNEW1 = RcText_[i];

		RcOLD = m_pText_[i]->GetLocalPos();
		m_pText_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pText_[i]->SetLocalPos(RcNEW1);
	}

	// m_pClassImage
	{
		RcNEW1 = UIRECT(297.00, 3.00, 26.00, 26.00);
		RcOLD = m_pClassImage->GetLocalPos();
		m_pClassImage->AlignSubControl(RcOLD, RcNEW1);
		m_pClassImage->SetLocalPos(RcNEW1);
	}

	// m_pSchoolImage
	{
		RcNEW1 = UIRECT(356.00, 3.00, 26.00, 26.00);
		RcOLD = m_pSchoolImage->GetLocalPos();
		m_pSchoolImage->AlignSubControl(RcOLD, RcNEW1);
		m_pSchoolImage->SetLocalPos(RcNEW1);
	}

	// m_pGuildImage
	{
		/*re-coded LG-7 Global Ranking by DevArt22*/
		if ( m_bGuild ) RcNEW1 = UIRECT(320.00, 10.00, 16.00, 11.00);
		else RcNEW1 = UIRECT(420.00, 10.00, 16.00, 11.00);

		RcOLD = m_pGuildImage->GetLocalPos();
		m_pGuildImage->AlignSubControl(RcOLD, RcNEW1);
		m_pGuildImage->SetLocalPos(RcNEW1);
	}

	{
		RcNEW1 = UIRECT(6.00, 24.00, 688.00, 32.00);
		SetLocalPos(RcNEW1);
	}
}

void CGlobalRankingPageSlot::RESET()
{
	m_sTopKill	= STOP_RANK_KILL();
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	m_sTopKillBr	= STOP_RANK_KILL_BR();
	m_sTopKillSw	= STOP_RANK_KILL_SW();
	m_sTopKillAr	= STOP_RANK_KILL_AR();
	m_sTopKillSh	= STOP_RANK_KILL_SH();
	m_sTopKillEx	= STOP_RANK_KILL_EX();
	m_sTopKillSc	= STOP_RANK_KILL_SC();
	m_sTopKillAs	= STOP_RANK_KILL_AS();
	m_sTopRich	= STOP_RANK_RICH();
	m_sTopGuild	= STOP_RANK_GUILD();
	m_bHaveData = FALSE;
	SetVisibleSingle(FALSE);
}

void CGlobalRankingPageSlot::SetRankingKill(STOP_RANK_KILL sTopKill)
{
	m_sTopKill = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKill.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKill.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKill.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKill.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKill.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKill.nChaClass)
	{
	case 1:		strClass = "BR_M.png"; break;
	case 2:		strClass = "SM_M.png"; break;
	case 4:		strClass = "AR_W.png"; break;
	case 8:		strClass = "SH_W.png"; break;
	case 16:	strClass = "EX_M.png"; break;
	case 32:	strClass = "EX_W.png"; break;
	case 64:	strClass = "BR_W.png"; break;
	case 128:	strClass = "SM_W.png"; break;
	case 256:	strClass = "AR_M.png"; break;
	case 512:	strClass = "SH_M.png"; break;
	case 1024:	strClass = "SC_M.png"; break;
	case 2048:	strClass = "SC_W.png"; break;
	case 4096:	strClass = "AS_M.png"; break;
	case 8192:	strClass = "AS_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKill.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillBr(STOP_RANK_KILL_BR sTopKill)
{
	m_sTopKillBr = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillBr.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillBr.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillBr.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillBr.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillBr.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillBr.nChaClass)
	{
	case 1:		strClass = "BR_M.png"; break;
	case 64:	strClass = "BR_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillBr.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillSw(STOP_RANK_KILL_SW sTopKill)
{
	m_sTopKillSw = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillSw.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillSw.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillSw.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillSw.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillSw.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillSw.nChaClass)
	{
	case 2:		strClass = "SM_M.png"; break;
	case 128:	strClass = "SM_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillSw.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillAr(STOP_RANK_KILL_AR sTopKill)
{
	m_sTopKillAr = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillAr.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillAr.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillAr.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillAr.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillAr.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillAr.nChaClass)
	{
	case 4:		strClass = "AR_W.png"; break;
	case 256:	strClass = "AR_M.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillAr.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillSh(STOP_RANK_KILL_SH sTopKill)
{
	m_sTopKillSh = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillSh.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillSh.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillSh.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillSh.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillSh.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillSh.nChaClass)
	{
	case 8:		strClass = "SH_W.png"; break;
	case 512:	strClass = "SH_M.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillSh.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillEx(STOP_RANK_KILL_EX sTopKill)
{
	m_sTopKillEx = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillEx.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillEx.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillEx.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillEx.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillEx.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillEx.nChaClass)
	{
	case 16:		strClass = "EX_M.png"; break;
	case 32:		strClass = "EX_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillEx.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillSc(STOP_RANK_KILL_SC sTopKill)
{
	m_sTopKillSc = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillSc.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillSc.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillSc.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillSc.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillSc.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillSc.nChaClass)
	{
	case 1024:		strClass = "SC_M.png"; break;
	case 2048:		strClass = "SC_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillSc.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingKillAs(STOP_RANK_KILL_AS sTopKill)
{
	m_sTopKillAs = sTopKill;
	for( int i=0;i<nMAX_TEXT;++i)	if( !m_pText_[i] )	return;
		
	m_pText_[1]->SetOneLineText(m_sTopKillAs.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopKillAs.wChaLevel);
	
		
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopKillAs.bChaOnline)	m_pText_[3]->SetOneLineText("ON",	NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF",	NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopKillAs.dwChaPKWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopKillAs.dwChaPKLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	m_pText_[4]->ClearText();
	m_pText_[4]->AddMultiText(ml_Text);

	CString strClass;
	switch (m_sTopKillAs.nChaClass)
	{
	case 4096:		strClass = "AS_M.png"; break;
	case 8192:		strClass = "AS_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopKillAs.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingRich(STOP_RANK_RICH sTopRich)
{
	m_sTopRich = sTopRich;

	m_pText_[1]->SetOneLineText(m_sTopRich.szChaName);

	CString strLevel;
	strLevel.Format("%d", m_sTopRich.wChaLevel);
	m_pText_[2]->SetOneLineText(strLevel);

	if (m_sTopRich.bChaOnline)	m_pText_[3]->SetOneLineText("ON", NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[3]->SetOneLineText("OFF", NS_UITEXTCOLOR::DISABLE);

	CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT(m_sTopRich.llnMoney, 3, ",");
	m_pText_[4]->SetOneLineText(strMoney);

	CString strClass;
	switch (m_sTopRich.nChaClass)
	{
	case 1:		strClass = "BR_M.png"; break;
	case 2:		strClass = "SM_M.png"; break;
	case 4:		strClass = "AR_W.png"; break;
	case 8:		strClass = "SH_W.png"; break;
	case 16:	strClass = "EX_M.png"; break;
	case 32:	strClass = "EX_W.png"; break;
	case 64:	strClass = "BR_W.png"; break;
	case 128:	strClass = "SM_W.png"; break;
	case 256:	strClass = "AR_M.png"; break;
	case 512:	strClass = "SH_M.png"; break;
	case 1024:	strClass = "SC_M.png"; break;
	case 2048:	strClass = "SC_W.png"; break;
	case 4096:	strClass = "AS_M.png"; break;
	case 8192:	strClass = "AS_W.png"; break;
	}
	m_pClassImage->SetTextureName(strClass);
	m_pClassImage->LoadTexture(m_pd3dDevice);

	CString strSchool;
	switch (m_sTopRich.wChaSchool)
	{
	case 0: strSchool = "SG.png";	break;
	case 1: strSchool = "MP.png";	break;
	case 2: strSchool = "PHX.png";	break;
	}
	m_pSchoolImage->SetTextureName(strSchool);
	m_pSchoolImage->LoadTexture(m_pd3dDevice);

	m_bHaveData = TRUE;
	SetVisibleSingle(TRUE);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingPageSlot::SetRankingGuild(STOP_RANK_GUILD sTopGuild)
{
	m_sTopGuild = sTopGuild;

	m_pText_[1]->SetOneLineText(m_sTopGuild.szGuName);
	m_pText_[2]->SetOneLineText((char*)ID2GAMEWORD("CLUB_GRADE", m_sTopGuild.wRank));

	CString strAlliance;
	strAlliance.Format("%d", m_sTopGuild.wAlliance);
	if (m_sTopGuild.wAlliance == 1)			m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::DEFAULT);
	else if (m_sTopGuild.wAlliance == 2)	m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::LIGHTSKYBLUE);
	else if (m_sTopGuild.wAlliance == 3)	m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::PALEGREEN);
	else if (m_sTopGuild.wAlliance == 4)	m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::LIGHTYELLOW);
	else if (m_sTopGuild.wAlliance == 5)	m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::GOLD);
	else									m_pText_[3]->SetOneLineText(strAlliance, NS_UITEXTCOLOR::DISABLE);

	CString strOnline;
	strOnline.Format("%d", m_sTopGuild.wOnline);
	if(m_sTopGuild.wOnline > 0)	m_pText_[4]->SetOneLineText(strOnline, NS_UITEXTCOLOR::PALEGREEN);
	else						m_pText_[4]->SetOneLineText(strOnline, NS_UITEXTCOLOR::DISABLE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[0].Format("%d", m_sTopGuild.dwWin);
	ml_Text.dwColor[0] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[1] = " - ";
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[2].Format("%d", m_sTopGuild.dwLoss);
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::DISABLE;
	ml_Text.strText[3] = " - ";
	ml_Text.dwColor[3] = NS_UITEXTCOLOR::DEFAULT;
	ml_Text.strText[4].Format("%d", m_sTopGuild.dwDraw);
	ml_Text.dwColor[4] = NS_UITEXTCOLOR::LIGHTYELLOW;
	m_pText_[5]->ClearText();
	m_pText_[5]->AddMultiText(ml_Text);

	m_bHaveData = TRUE;
	m_pClassImage->SetVisibleSingle(FALSE);
	m_pSchoolImage->SetVisibleSingle(FALSE);
	SetVisibleSingle(TRUE);
}