/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillSlot.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

class CBasicLineBox;
class CSkillImage;

class CAutoSkillSlot : public CUIWindowEx
{
protected:
	enum
	{
		QUICK_SKILL_MOUSE_OVER = NO_ID + 1,
	};

public:
	CAutoSkillSlot();
	void CreateSubControl();
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicLineBox*		m_pLBox;
	CSkillImage*		m_pSkillImage;

	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	CUIControl*			m_pMouseOver;

protected:
	void CreateSkillImage();
	void CreateMouseOver();

public:
	void	UpdateSlot(SNATIVEID sNativeID);
	UIRECT	GetAbsPosSkillImage();
	void	SetUseSkillImageTwinkle(bool bUse);
};