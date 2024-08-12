/////////////////////////////////////////////////////////////////////////////
//	FileName	: CharViewRender.h
//	Project		: Lib -- RanClientUI
//	Coder		: LG-7

#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "DxSkinChar.h"
#include "GLGaeaClient.h"

#define SLOT_NSIZE_S_2_OLD	14 

class CCharViewRender : public CUIGroup
{
public:
	CCharViewRender();
	virtual ~CCharViewRender();

public:
	void CreateSubControl();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT FrameMove(LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime);

public:
	BOOL					m_bReady;
	DxSkinChar* 			m_pSkinChar;
	D3DXVECTOR3				m_vRot;
	D3DXMATRIX				m_matTrans;
	float					m_fRenderDelay;
	SCHAR_VIEW				m_sCharView;

private:
	void CreateModel(LPDIRECT3DDEVICEQ pd3dDevice);
	void RenderModel(LPDIRECT3DDEVICEQ pd3dDevice);
	void UpdateSuit(LPDIRECT3DDEVICEQ pd3dDevice);

public:
	void RESET();
	void StartRender(SCHAR_VIEW sCharView);
};