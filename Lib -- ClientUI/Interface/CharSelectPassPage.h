#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;

class CCharSelectPassPage : public CUIOuterWindow
{
	enum
	{
		CHARSELECT_PASS_OK = ET_CONTROL_NEXT,
		CHARSELECT_PASS_CANCEL,
		CHARSELECT_PASS_EDIT_MAN,
		CHARSELECT_PASS_EDIT_PASS
	};

	UIGUID	m_PrevPageID;

public:
	static int nLIMIT_CODE;
	static int nLIMIT_PIN;
	static int nLIMIT_CP;

public:
	CCharSelectPassPage();
	virtual	~CCharSelectPassPage();

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	void CreateSubControl();
	CBasicButton * CreateFlipButton( char* szButton, char* szButtonFlip, UIGUID ControlID );

	void ResetAll();
	void SetCharToEditBox( TCHAR cKey );
	void DelCharToEditBox();
	void GoNextTab();

	void	SetPrevPageID ( UIGUID ControlID )	{ m_PrevPageID = ControlID; }
	UIGUID	GetPrevPageID ()					{ return m_PrevPageID; }

private:
	BOOL CheckString( CString strTemp );
private:
	CUIEditBoxMan * m_pEditBoxMan;
	CBasicButton * m_pIDSaveButton;
	CBasicTextBox * m_pRandTextBox;

	INT m_nRandPassNumber;
	TCHAR m_szRandomString[7];
	INT m_nRPUpdateCnt;
};