#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"


class CThaiCCafeMark : public CUIGroup
{
public:
	CUIControl*		m_pImage;
	DWORD			m_dwClass;

public:
	CThaiCCafeMark();
	virtual ~CThaiCCafeMark();
	
	
	void	CreateSubControl ();
	DWORD	GetClass()	{ return m_dwClass; };
	void	SetClass( DWORD dwClass );
};