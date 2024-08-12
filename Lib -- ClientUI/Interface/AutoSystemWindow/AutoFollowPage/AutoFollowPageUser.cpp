/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoFollowPageUser.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoFollowPageUser.h"

#include "../../BasicLineBox.h"
#include "../../BasicTextBoxEx.h"
#include "../../UITextControl.h"
#include "../../BasicScrollBarEx.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicScrollThumbFrame.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"
#include "../Lib -- Client/G-Logic/GLPartyClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoFollowPageUser::CAutoFollowPageUser()
	: m_pListText(NULL)
	, m_nType(-1)
	, m_nSelectIndex(-1)
	, m_pListScrollBar(NULL)
	, m_dwPartySize(UINT_MAX)
{
	memset(m_pLBox_, NULL, sizeof(m_pLBox_));
	m_strSelectName.Empty();
}

void CAutoFollowPageUser::CreateSubControl(int nType)
{
	m_nType = nType;

	char strTemp[50];

	for (int i = 0; i < nMAXLBOX; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_USER_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	m_pListText = new CBasicTextBoxEx;
	m_pListText->CreateSub(this, "AUTOFOLLOW_PAGE_FRIEND_TEXTBOX", UI_FLAG_DEFAULT, FRIEND_LIST_TEXTBOX);
	m_pListText->SetFont(pFont9);
	m_pListText->SetLineInterval(3.0f);
	m_pListText->SetSensitive(true);
	m_pListText->SetLimitLine(10000);
	RegisterControl(m_pListText);

	if (m_nType == 0)
	{
		int nTotalLine = m_pListText->GetVisibleLine();

		m_pListScrollBar = new CBasicScrollBarEx;
		m_pListScrollBar->CreateSub(this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, FRIEND_LIST_SCROLLBAR);
		m_pListScrollBar->CreateBaseScrollBar("AUTOFOLLOW_SCROLLBAR");
		m_pListScrollBar->GetThumbFrame()->SetState(1, nTotalLine);
		RegisterControl(m_pListScrollBar);
	}
}

void CAutoFollowPageUser::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	switch (m_nType)
	{
	case 0:
	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame();
		const int nTotalLine = m_pListText->GetTotalLine();
		const int nLinePerOneView = m_pListText->GetVisibleLine();

		CDebugSet::ToView(1, 10, "보이는 라인 %d", nLinePerOneView);

		pThumbFrame->SetState(nTotalLine, nLinePerOneView);
		if (nLinePerOneView < nTotalLine)
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine(nPos);
		}
	}
	break;

	case 1:
	{
		GLPARTY_CLIENT* pMaster = GLPartyClient::GetInstance().GetMaster();
		if (pMaster)
		{
			DWORD dwMEMBER_SIZE = GLPartyClient::GetInstance().GetMemberNum();

			if (dwMEMBER_SIZE != m_dwPartySize)
			{
				if (dwMEMBER_SIZE > 1)
				{
					ReloadParty(TRUE);
				}
			}
		}
		else
		{
			if (m_dwPartySize != -1)
			{
				ReloadParty(FALSE);
			}
		}
	}
	break;
	}
}

void CAutoFollowPageUser::TranslateUIMessage(UIGUID cID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(cID, dwMsg);

	switch (cID)
	{
	case FRIEND_LIST_TEXTBOX:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			const int nIndex = m_pListText->GetSelectPos();

			if (UIMSG_LB_UP & dwMsg)
			{
				m_nSelectIndex = nIndex;
				m_pListText->SetUseOverColor(TRUE);

				int nSize = m_pListText->GetCount();
				for (int i = 0; i < nSize; i++)
				{
					m_pListText->SetOverColor(i, NS_UITEXTCOLOR::WHITE);
				}
				
				m_pListText->SetOverColor(nIndex, NS_UITEXTCOLOR::DARKORANGE);

				switch (m_nType)
				{
				case 0:
				{
					m_strSelectName.Empty();
					m_strSelectName = m_pListText->GetText(m_nSelectIndex);
				}
				break;

				case 1:
				{
					m_strSelectName.Empty();
					m_strSelectName = m_pListText->GetText(m_nSelectIndex);
				}
				break;
				}
			}
		}
	}
	break;
	}
}

void CAutoFollowPageUser::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		LoadFriendList();

		m_nSelectIndex = -1;
	}
}

void CAutoFollowPageUser::LoadFriend(const SFRIEND& sFriend)
{
	const CString& strName	= sFriend.szCharName;
	D3DCOLOR dwTextColor	= NS_UITEXTCOLOR::WHITE;

	int nIndex				= m_pListText->AddText(strName, dwTextColor);
}

void CAutoFollowPageUser::LoadFriendList()
{
	if (m_nType == 0)
	{
		m_pListText->ClearText();

		GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end();
		for (; iter != iter_end; ++iter)
		{
			SFRIEND& sFriend = (*iter).second;
			if (sFriend.IsBLOCK())	continue;
			if (!sFriend.bONLINE)	continue;

			LoadFriend(sFriend);
		}

		m_pListText->SetCurLine(0);

		const int nTotal = m_pListText->GetCount();
		const int nViewPerPage = m_pListText->GetVisibleLine();

		m_pListScrollBar->GetThumbFrame()->SetState(nTotal, nViewPerPage);
		m_pListScrollBar->GetThumbFrame()->SetPercent(0.0f);
	}
}

void CAutoFollowPageUser::ReloadParty(BOOL bValue)
{
	m_pListText->ClearText();

	switch (bValue)
	{
	case TRUE:
	{
		GLPARTY_CLIENT* pSelf = FindSelfClient();
		if (!pSelf) return;

		DWORD	dwMEMBER_SIZE	= GLPartyClient::GetInstance().GetMemberNum();
		DWORD	dwMemberSize	= dwMEMBER_SIZE;
		dwMemberSize -= 1;

		for (DWORD i = 0; i < dwMemberSize; i++)
		{
			GLPARTY_CLIENT* pMember = GLPartyClient::GetInstance().GetMember(i);
			if (pMember && pMember->m_dwGaeaID != pSelf->m_dwGaeaID)
			{
				const CString& strName	= pMember->m_szName;
				D3DCOLOR dwTextColor	= NS_UITEXTCOLOR::WHITE;
				int nIndex				= m_pListText->AddText(strName, dwTextColor);
			}
		}

		m_dwPartySize = dwMEMBER_SIZE;
	}
	break;

	case FALSE:
	{
		m_dwPartySize = -1;
	}
	break;
	}

	m_pListText->SetCurLine(0);
}

GLPARTY_CLIENT* CAutoFollowPageUser::FindSelfClient()
{
	GLPARTY_CLIENT* pMaster = GLPartyClient::GetInstance().GetMaster();
	if (pMaster && pMaster->ISONESELF())
	{
		return pMaster;
	}
	else
	{
		DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
		if (nMEMBER_NUM > 1)
		{
			nMEMBER_NUM -= 1;
			for (DWORD i = 0; i < nMEMBER_NUM; i++)
			{
				GLPARTY_CLIENT* pMember = GLPartyClient::GetInstance().GetMember(i);
				if (pMember && pMember->ISONESELF())
				{
					return pMember;
				}
			}
		}
	}

	return NULL;
}