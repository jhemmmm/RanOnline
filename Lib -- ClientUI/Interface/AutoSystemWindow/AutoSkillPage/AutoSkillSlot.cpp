/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillSlot.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSkillSlot.h"

#include "../../BasicLineBox.h"
#include "../../SkillImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoSkillSlot::CAutoSkillSlot()
	: m_pLBox(NULL)
	, m_pSkillImage(NULL)
	, m_pMouseOver(NULL)
	, m_pd3dDevice(NULL)
{
}

void CAutoSkillSlot::CreateSubControl()
{
	m_pLBox = new CBasicLineBox;
	m_pLBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLBox->CreateBaseBoxOuter("AUTOSKILL_SLOT_LBOX");
	RegisterControl(m_pLBox);

	CreateSkillImage();
	CreateMouseOver();
}

void CAutoSkillSlot::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GetskillRunSlot();
	UpdateSlot(sNativeID);
}

void CAutoSkillSlot::CreateSkillImage()
{
	m_pSkillImage = new CSkillImage;
	m_pSkillImage->CreateSub(this, "AUTOSKILL_SLOT_IMAGE");
	m_pSkillImage->CreateSubControl();
	m_pSkillImage->SetUseRender(TRUE);
	m_pSkillImage->SetVisibleSingle(FALSE);
	RegisterControl(m_pSkillImage);
}

void CAutoSkillSlot::CreateMouseOver()
{
	m_pMouseOver = new CUIControl;
	m_pMouseOver->CreateSub(this, "AUTOSKILL_SLOT_MOUSE_OVER", UI_FLAG_DEFAULT, QUICK_SKILL_MOUSE_OVER);
	m_pMouseOver->SetVisibleSingle(FALSE);
	RegisterControl(m_pMouseOver);
}

void CAutoSkillSlot::UpdateSlot(SNATIVEID sNativeID)
{
	if (!m_pSkillImage)
	{
		GASSERT(0 && "스킬 이미지가 만들어지지 않았습니다.");
		return;
	}

	if (sNativeID != NATIVEID_NULL())
	{
		m_pSkillImage->SetSkill(sNativeID);
		m_pSkillImage->SetVisibleSingle(TRUE);
		m_pSkillImage->SetSkillProcess(sNativeID);
	}
	else
	{
		m_pSkillImage->SetVisibleSingle(FALSE);
		m_pSkillImage->ResetSkill();
	}
}

UIRECT CAutoSkillSlot::GetAbsPosSkillImage()
{
	if (!m_pSkillImage)
	{
		GASSERT(0 && "널입니다.");
		return UIRECT();
	}

	return m_pSkillImage->GetGlobalPos();
}

void CAutoSkillSlot::SetUseSkillImageTwinkle(bool bUse)
{
	if (!m_pSkillImage)
	{
		GASSERT(0 && "CBasicQuickSkillSlot::SetUseSkillImageTwinkle(), m_pSkillImage is NULL.");
		return;
	}

	m_pSkillImage->SetUseTwinkle(bUse);
}