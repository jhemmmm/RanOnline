/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoFollowPageUser.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "../Lib -- Client/G-Logic/GLFriendClient.h"

class CBasicLineBox;
class CBasicTextBoxEx;
class CBasicScrollBarEx;
struct GLPARTY_CLIENT;
class CAutoFollowPageUser : public CUIGroup
{
protected:
	enum
	{
		nMAXLBOX = 1,
	};

	enum
	{
		FRIEND_LIST_TEXTBOX = NO_ID + 1,
		FRIEND_LIST_SCROLLBAR,
	};

public:
	CAutoFollowPageUser();
	void CreateSubControl(int nType);
	virtual	void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID cID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);

private:
	CBasicLineBox*		m_pLBox_[nMAXLBOX];
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;

	int					m_nType;
	int					m_nSelectIndex;
	DWORD				m_dwPartySize;

public:
	CString				m_strSelectName;

private:
	void LoadFriend(const SFRIEND& sFriend);
	void ReloadParty(BOOL bValue);

	GLPARTY_CLIENT* FindSelfClient();

public:
	void LoadFriendList();
};