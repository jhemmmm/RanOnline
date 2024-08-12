/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillPageToTray.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSkillPageToTray.h"

#include "../../SkillImage.h"
#include "../../InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD CAutoSkillPageToTray::dwDEFAULT_TRANSPARENCY		= D3DCOLOR_ARGB(200, 255, 255, 255);
const float CAutoSkillPageToTray::fDEFAULT_MOUSE_INTERPOLIATION	= 20.0f;

CAutoSkillPageToTray::CAutoSkillPageToTray()
	: m_pSkillImage(NULL)
	, m_sNativeID(NATIVEID_NULL())
	, m_bUseSnap(FALSE)
{
}

void CAutoSkillPageToTray::CreateSubControl()
{
	m_pSkillImage = new CSkillImage;
	m_pSkillImage->CreateSub(this, "AUTOSKILL_PAGE_TO_TRAY_IMAGE");
	m_pSkillImage->CreateSubControl();
	m_pSkillImage->SetUseRender(TRUE);
	m_pSkillImage->SetVisibleSingle(FALSE);
	RegisterControl(m_pSkillImage);

	ResetSkill();
}

void CAutoSkillPageToTray::SetSkill(SNATIVEID sNativeID)
{
	if (sNativeID == NATIVEID_NULL())
	{
		GASSERT(0 && "무효화한 ID가 설정되려고 합니다.");

		return;
	}

	m_sNativeID = sNativeID;
	m_pSkillImage->SetSkill(m_sNativeID);
	m_pSkillImage->SetVisibleSingle(TRUE);
	m_pSkillImage->SetDiffuse(dwDEFAULT_TRANSPARENCY);
}

void CAutoSkillPageToTray::ResetSkill()
{
	m_pSkillImage->ResetSkill();
	m_pSkillImage->SetVisibleSingle(FALSE);
	m_sNativeID = NATIVEID_NULL();
}

void CAutoSkillPageToTray::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (!m_pSkillImage)	return;

	if (!CInnerInterface::GetInstance().IsVisibleGroup(AUTOSYSTEM_WINDOW))
	{
		if (m_sNativeID != NATIVEID_NULL())	ResetSkill();
		
		return;
	}

	if (m_sNativeID != NATIVEID_NULL())
	{
		const D3DXVECTOR2 vImagePos(x - fDEFAULT_MOUSE_INTERPOLIATION, y - fDEFAULT_MOUSE_INTERPOLIATION);
		SetGlobalPos(vImagePos);
	}
}