//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.27]
//			@ 작성
//

#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "GLDefine.h"


class	CBasicProgressBar;
class	CBasicTextBox;

class	CItemImage : public CUIGroup
{
public:
	CItemImage ();
	virtual	~CItemImage ();

public:
	void	CreateSubControl ();
	void	CreateTextBoxUpLeft();
	void	CreateTextBoxUpRight();
	void	CreateTextBoxDownLeft();
	void	CreateTextBoxDownRight();

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void SetDiffuse(D3DCOLOR _diffuse);

public:
    void	SetItem ( SNATIVEID sICONINDEX, const char* szTexture, SNATIVEID sNativeID = false );
	void	SetBackItem( SNATIVEID sICONINDEX, const char* szTexture );
	SNATIVEID	GetItem ();
	const CString&	GetItemTextureName () const;
	void	ResetItem ();

	void SetUseTwinkle( bool bUse )					{ m_bTwinkle = bUse; }

	void SetItemProcess( SNATIVEID sNativeID );
	void ReSetItemProcess();


private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

private:
	SNATIVEID	m_sICONINDEX;
	SNATIVEID	m_sICONINDEXBACK;
	SNATIVEID	m_sNativeID;
	CString		m_strTextureName;

	CBasicProgressBar*	m_pItemProcess;
	CBasicTextBox*		m_pTextBoxUpLeft;
	CBasicTextBox*		m_pTextBoxUpRight;
	CBasicTextBox*		m_pTextBoxDownLeft;
	CBasicTextBox*		m_pTextBoxDownRight;

	float m_TotElapsedTime;
	bool m_bItemUsed;
	bool m_bItemEnd;

	bool m_bTwinkle;


private:
	CUIControl*		m_pItemImageBack;
	CUIControl*		m_pItemImage;

public:
	void	SetTextUpLeft( CString strText, DWORD dwColor );
	void	SetTextUpRight( CString strText, DWORD dwColor );
	void	SetTextDownLeft( CString strText, DWORD dwColor );
	void	SetTextDownRight( CString strText, DWORD dwColor );
};