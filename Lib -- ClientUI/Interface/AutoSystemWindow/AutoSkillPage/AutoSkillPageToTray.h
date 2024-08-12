/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillPageToTray.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"
#include "../Lib -- Engine/G-Logic/GLDefine.h"

class CSkillImage;
class CAutoSkillPageToTray : public CUIWindowEx
{
protected:
	static const DWORD	dwDEFAULT_TRANSPARENCY;
	static const float	fDEFAULT_MOUSE_INTERPOLIATION;

public:
	CAutoSkillPageToTray();
	void CreateSubControl();

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	void		SetSkill(SNATIVEID sNativeID);
	SNATIVEID	GetSkill() { return m_sNativeID; }
	void		ResetSkill();

private:
	CSkillImage*	m_pSkillImage;
	SNATIVEID		m_sNativeID;

	BOOL			m_bUseSnap;

public:
	void	SetUseSnap(BOOL bUseSnap)	{ m_bUseSnap = bUseSnap; }
	BOOL	IsUseSnap()					{ return m_bUseSnap; }
};