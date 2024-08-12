/////////////////////////////////////////////////////////////////////////////
//	FileName	: CharViewRender.cpp
//	Project		: Lib -- RanClientUI
//	Coder		: LG-7

#include "stdafx.h"
#include "CharViewRender.h"

#include "../Lib -- Engine/DxEffect/Char/DxEffCharData.h"
#include "../Lib -- Engine/DxCommon/DxBackUpRendTarget.h"
//#include "../Lib -- Engine/Meshs/DxSkinPieceRootData.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL g_bPREVIEW_CHAR_RENDER;

CCharViewRender::CCharViewRender()
	: m_bReady(FALSE)
	, m_pSkinChar(NULL)
	, m_vRot(0.0f, 0.0f, 0.0f)
	, m_fRenderDelay( 0.0f )
{
	D3DXMatrixIdentity(&m_matTrans);
}

CCharViewRender::~CCharViewRender()
{
}

void CCharViewRender::CreateSubControl()
{
}

void CCharViewRender::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

HRESULT CCharViewRender::RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = S_OK;

	if (m_pSkinChar)
		m_pSkinChar->RestoreDeviceObjects(pd3dDevice);

	hr = CUIGroup::RestoreDeviceObjects(pd3dDevice);
	return hr;
}

HRESULT CCharViewRender::DeleteDeviceObjects()
{
	HRESULT hr = S_OK;

	if (m_pSkinChar)
		m_pSkinChar->DeleteDeviceObjects();

	hr = CUIGroup::DeleteDeviceObjects();

	return hr;
}

HRESULT CCharViewRender::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = S_OK;

	if (m_pSkinChar)
	{
		g_bPREVIEW_CHAR_RENDER = TRUE;
		if ( m_fRenderDelay < 0.3f )	return S_OK;

		DWORD dwAlphaBlendEnable, dwSrcBlend, dwDestBlend, dwZWriteEnable, dwZEnable, dwZFunc, dwClipPlaneEnable, dwFogEnable, dwAmbient, dwLighting;
		DWORD dwColorOP, dwAlphaOP, dwMinFilter, dwMagFilter, dwMipFilter;
		D3DXMATRIX		matOrgView, matOrgProj, matOrgWorld;
		D3DVIEWPORTQ OrgViewPort;

		pd3dDevice->GetViewport(&OrgViewPort);

		pd3dDevice->GetTransform(D3DTS_VIEW, &matOrgView);
		pd3dDevice->GetTransform(D3DTS_PROJECTION, &matOrgProj);
		pd3dDevice->GetTransform(D3DTS_WORLD, &matOrgWorld);

		DxBackUpRendTarget sBackupTarget(pd3dDevice);

		pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable);
		pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
		pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
		pd3dDevice->GetRenderState(D3DRS_ZWRITEENABLE, &dwZWriteEnable);
		pd3dDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
		pd3dDevice->GetRenderState(D3DRS_ZFUNC, &dwZFunc);
		pd3dDevice->GetRenderState(D3DRS_CLIPPLANEENABLE, &dwClipPlaneEnable);
		pd3dDevice->GetRenderState(D3DRS_FOGENABLE, &dwFogEnable);
		pd3dDevice->GetRenderState(D3DRS_AMBIENT, &dwAmbient);
		pd3dDevice->GetRenderState(D3DRS_LIGHTING, &dwLighting);

		pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOP);
		pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOP);

		pd3dDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &dwMinFilter);
		pd3dDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &dwMagFilter);
		pd3dDevice->GetSamplerState(0, D3DSAMP_MIPFILTER, &dwMipFilter);

		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0);

		pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(220, 220, 220));
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXMATRIX		matView, matProj, matWorld;

		D3DVIEWPORTQ ViewPort;
		ViewPort.X = (DWORD)GetGlobalPos().left;
		ViewPort.Y = (DWORD)GetGlobalPos().top;
		ViewPort.Width = (DWORD)GetGlobalPos().sizeX;
		ViewPort.Height = (DWORD)GetGlobalPos().sizeY;
		ViewPort.MinZ = 0.0f;
		ViewPort.MaxZ = 1.0f;
		pd3dDevice->SetViewport(&ViewPort);

		D3DXVECTOR3 vFromPt = D3DXVECTOR3(0.0f, 0.3f, -35.0f);
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3(-0.02f, 0.0f, 5.0f);
		D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vFromPt, &vLookatPt, &vUpVec);
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		float fFieldOfView = D3DX_PI / 4.0f;
		float fAspectRatio = ((float)ViewPort.Width) / (float)ViewPort.Height;
		D3DXMatrixPerspectiveFovLH(&matProj, fFieldOfView, fAspectRatio, 1.0f, 80.0f);
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		D3DXMatrixIdentity(&matWorld);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		RenderModel(pd3dDevice);

		sBackupTarget.RestoreTarget(pd3dDevice);

		pd3dDevice->SetTransform(D3DTS_VIEW, &matOrgView);
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &matOrgProj);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matOrgWorld);

		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
		pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, dwZWriteEnable);
		pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable);
		pd3dDevice->SetRenderState(D3DRS_ZFUNC, dwZFunc);
		pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, dwClipPlaneEnable);
		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, dwFogEnable);
		pd3dDevice->SetRenderState(D3DRS_AMBIENT, dwAmbient);
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, dwLighting);

		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOP);
		pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOP);

		pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
		pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter);

		pd3dDevice->SetViewport(&OrgViewPort);

		g_bPREVIEW_CHAR_RENDER = FALSE;
	}
	else
	{
		if (m_bReady)
			CreateModel(pd3dDevice);
	}

	hr = CUIGroup::Render(pd3dDevice);

	return hr;
}

HRESULT CCharViewRender::FrameMove(LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime)
{
	HRESULT hr = S_OK;

	if (m_pSkinChar)
		m_pSkinChar->FrameMove(fTime, fElapsedTime, TRUE, FALSE, FALSE);

	if ( m_bReady ) 
		m_fRenderDelay += fElapsedTime;

	hr = CUIGroup::FrameMove(pd3dDevice, fTime, fElapsedTime);

	return hr;
}

void CCharViewRender::CreateModel(LPDIRECT3DDEVICEQ pd3dDevice)
{
	EMCHARINDEX emIndex			= CharClassToIndex((EMCHARCLASS)m_sCharView.nChaClass);
	DxSkinCharData* pSkinChar	= DxSkinCharDataContainer::GetInstance().LoadData(GLCONST_CHAR::szCharSkin[emIndex], pd3dDevice, true);
	if (!pSkinChar)
		return;

	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;
	m_pSkinChar->InitDeviceObjects( pd3dDevice );
	m_pSkinChar->SetCharData ( pSkinChar, pd3dDevice );
	m_pSkinChar->SELECTANI( AN_PLACID, AN_SUB_NONE );
	UpdateSuit( pd3dDevice );
	m_fRenderDelay = 0.0f;
	m_vRot = D3DXVECTOR3( 0.0f,0.0f,0.0f );
}

void CCharViewRender::RenderModel(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (m_pSkinChar)
	{
		float fScale = 7.0f;
		D3DXMATRIX matTrans, matScale, matRot;

		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);

		switch ((EMCHARCLASS)m_sCharView.nChaClass)
		{
		case GLCC_BRAWLER_M:	D3DXMatrixTranslation(&matTrans, 0.0f, -123.0f, 0.0f);		break;
		case GLCC_BRAWLER_W:	D3DXMatrixTranslation(&matTrans, 0.0f, -108.0f, 0.0f);		break;

		case GLCC_SWORDSMAN_M:		D3DXMatrixTranslation(&matTrans, 0.0f, -114.0f, 0.0f);		break;
		case GLCC_SWORDSMAN_W:		D3DXMatrixTranslation(&matTrans, 0.0f, -106.0f, 0.0f);		break;

		case GLCC_ARCHER_M:		D3DXMatrixTranslation(&matTrans, 0.0f, -119.0f, 0.0f);		break;
		case GLCC_ARCHER_W:		D3DXMatrixTranslation(&matTrans, 0.0f, -110.0f, 0.0f);		break;

		case GLCC_SHAMAN_M:		D3DXMatrixTranslation(&matTrans, 0.0f, -114.0f, 0.0f);		break;
		case GLCC_SHAMAN_W:		D3DXMatrixTranslation(&matTrans, 0.0f, -102.0f, 0.0f);		break;

		case GLCC_EXTREME_M:	D3DXMatrixTranslation(&matTrans, 0.0f, -106.0f, 0.0f);		break;
		case GLCC_EXTREME_W:	D3DXMatrixTranslation(&matTrans, 0.0f, -106.0f, 0.0f);		break;
		}

		D3DXMatrixRotationYawPitchRoll(&matRot, m_vRot.x, m_vRot.y, m_vRot.z);
		m_matTrans = matScale * matRot * matTrans;

		m_pSkinChar->Render(pd3dDevice, m_matTrans);
	}
}

void CCharViewRender::UpdateSuit(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (!m_pSkinChar)
		return;

	EMCHARINDEX emIndex			= CharClassToIndex((EMCHARCLASS)m_sCharView.nChaClass);
	DxSkinCharData* pSkinChar	= DxSkinCharDataContainer::GetInstance().FindData(GLCONST_CHAR::szCharSkin[emIndex]);
	if (!pSkinChar)
		return;

	const GLCONST_CHARCLASS& sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	if (sCONST.dwHEADNUM > m_sCharView.wChaFace)
	{
		std::string strHEAD_CPS	= sCONST.strHEAD_CPS[m_sCharView.wChaFace];
		PDXCHARPART pCharPart	= m_pSkinChar->GetPiece(PIECE_HEAD);

		if (pCharPart && strcmp(strHEAD_CPS.c_str(), pCharPart->m_szFileName))
			m_pSkinChar->SetPiece(strHEAD_CPS.c_str(), pd3dDevice, 0x0, 0, TRUE);
	}

	if (sCONST.dwHAIRNUM > m_sCharView.wChaHair)
	{
		std::string strHAIR_CPS	= sCONST.strHAIR_CPS[m_sCharView.wChaHair];
		PDXCHARPART pCharPart	= m_pSkinChar->GetPiece(PIECE_HAIR);

		if (pCharPart && strcmp(strHAIR_CPS.c_str(), pCharPart->m_szFileName))
			m_pSkinChar->SetPiece(strHAIR_CPS.c_str(), pd3dDevice, 0x0, 0, TRUE);
	}

	m_pSkinChar->SetHairColor(m_sCharView.wChaHairColor);

	for (int i = 0; i < SLOT_NSIZE_S_2_OLD; i++)
	{
		SITEMCUSTOM ItemCustom;
		ItemCustom = m_sCharView.m_PutOnItems_[i];

		SITEM* pItem = GLItemMan::GetInstance().GetItem(ItemCustom.sNativeID);
		if (pItem)
		{
			/*Item Self Effect, Juver, 2017/06/09 */
			std::string strSelfEffect = pItem->GetSelfBodyEffect();

			if (pItem->sBasicOp.emItemType != ITEM_VEHICLE)
			{
				std::string strFileName = pItem->GetWearingFileR( emIndex );
				if ( i == SLOT_LHAND || i == SLOT_LHAND_S )	strFileName = pItem->GetWearingFileL( emIndex );
				m_pSkinChar->SetPiece ( strFileName.c_str(), pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE, strSelfEffect.c_str() );
			}
		}
	}
}

void CCharViewRender::RESET()
{
	if (m_pSkinChar)
		SAFE_DELETE(m_pSkinChar);

	m_fRenderDelay = 0.0f;

	m_bReady	= FALSE;
	m_sCharView	= SCHAR_VIEW();
}

void CCharViewRender::StartRender(SCHAR_VIEW sCharView)
{
	RESET();

	m_sCharView	= sCharView;
	m_bReady	= TRUE;
}