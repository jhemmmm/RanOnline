#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "CMemPool.h"

class	CHeadChatDisplay;

class CHeadChatDisplayMan : public CUIGroup
{
	static const float fVISIBLE_TIME;

	enum
	{
		BASE_CONTROL_ID = NO_ID + 1,
		BASE_CONTROL_ID_END = BASE_CONTROL_ID + 10000,
	};

public:
	typedef std::map<CString,CHeadChatDisplay*>	CHATMAP;
	typedef CHATMAP::iterator					CHATMAP_ITER;

	typedef CMemPool<CHeadChatDisplay>			HEADCHAT_POOL;

public:
	CHeadChatDisplayMan ();
	virtual	~CHeadChatDisplayMan ();

private:
	virtual	HRESULT	InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void AddChat( const CString& strName, const D3DCOLOR& dwIDColor, const CString& strChat, const D3DCOLOR& dwChatColor, const D3DXVECTOR2& vPos );
	CHATMAP & GetChatMap() { return m_mapChat; }

	// MEMO : 재정의
	bool DeleteControl ( UIGUID ControlID, int refactoring ); // 컨트롤을 삭제한다.

	CHeadChatDisplay*	MAKE_HEADCHATDISPLAY ();

private:
	CHATMAP	m_mapChat;
	UIGUID	m_CurID;

	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	HEADCHAT_POOL		m_HeadChatPool;
};