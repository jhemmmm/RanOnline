/////////////////////////////////////////////////////////////////////////////
//	FileName	: LG7DLL.h
//	Project		: RanClientLib
//	Coder		: LG-7

#pragma once

#include "../Lib -- Engine/GUInterface/UIDataType.h"
#include "s_NetClient.h"
#include "./GLItem.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
//#define GLOBAL_RANKING_SIZE	12	//shaper ranking
#define GLOBAL_RANKING_SIZE	9	// 6class

#define MAX_TOP_RANK		50
#define MAX_ONE_VIEW_RANK	10

struct STOP_RANK_KILL
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_BR
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_BR()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_SW
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_SW()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_AR
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_AR()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_SH
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_SH()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_EX
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_EX()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_SC
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_SC()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_KILL_AS
{
	DWORD	dwChaNum;
	char	szChaName[CHR_ID_LENGTH];
	WORD	wChaLevel;
	int		nChaClass;
	WORD	wChaSchool;
	BOOL	bChaOnline;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szGuName[CHR_ID_LENGTH];
	DWORD	dwChaPKWin;
	DWORD	dwChaPKLoss;

	STOP_RANK_KILL_AS()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, dwChaPKWin(0)
		, dwChaPKLoss(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
		//SecureZeroMemory(szChaName,	sizeof(char) * 33);
		//SecureZeroMemory(szGuName,	sizeof(char) * 33);
	}
};
/*re-coded LG-7 Global Ranking by DevArt22*/
struct STOP_RANK_RICH
{
	DWORD		dwChaNum;
	char		szChaName[CHR_ID_LENGTH];
	WORD		wChaLevel;
	int			nChaClass;
	WORD		wChaSchool;
	BOOL		bChaOnline;
	WORD		wGuNum;
	WORD		wGuMarkVer;
	char		szGuName[CHR_ID_LENGTH];
	LONGLONG	llnMoney;

	STOP_RANK_RICH()
		: dwChaNum(0)
		, wChaLevel(0)
		, nChaClass(0)
		, wChaSchool(0)
		, bChaOnline(FALSE)
		, wGuNum(0)
		, wGuMarkVer(0)
		, llnMoney(0)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
	}
};

struct STOP_RANK_GUILD
{
	WORD	wGuNum;
	char	szGuName[CHR_ID_LENGTH];
	WORD	wRank;
	WORD	wGuMarkVer;
	WORD	wAlliance;
	WORD	wOnline;
	DWORD	dwWin;
	DWORD	dwLoss;
	DWORD	dwDraw;

	STOP_RANK_GUILD()
		: wGuNum(0)
		, wRank(0)
		, wGuMarkVer(0)
		, wAlliance(0)
		, wOnline(0)
		, dwWin(0)
		, dwLoss(0)
		, dwDraw(0)
	{
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
	}
};

typedef std::vector<STOP_RANK_KILL>		VEC_TOP_KILL;
typedef VEC_TOP_KILL::iterator			VEC_TOP_KILL_ITER;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_BR>		VEC_TOP_KILL_BR;
typedef VEC_TOP_KILL_BR::iterator			VEC_TOP_KILL_ITER_BR;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_SW>		VEC_TOP_KILL_SW;
typedef VEC_TOP_KILL_SW::iterator			VEC_TOP_KILL_ITER_SW;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_AR>		VEC_TOP_KILL_AR;
typedef VEC_TOP_KILL_AR::iterator			VEC_TOP_KILL_ITER_AR;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_SH>		VEC_TOP_KILL_SH;
typedef VEC_TOP_KILL_SH::iterator			VEC_TOP_KILL_ITER_SH;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_EX>		VEC_TOP_KILL_EX;
typedef VEC_TOP_KILL_EX::iterator			VEC_TOP_KILL_ITER_EX;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_SC>		VEC_TOP_KILL_SC;
typedef VEC_TOP_KILL_SC::iterator			VEC_TOP_KILL_ITER_SC;

/*re-coded LG-7 Global Ranking by DevArt22*/
typedef std::vector<STOP_RANK_KILL_AS>		VEC_TOP_KILL_AS;
typedef VEC_TOP_KILL_AS::iterator			VEC_TOP_KILL_ITER_AS;

typedef std::vector<STOP_RANK_RICH>		VEC_TOP_RICH;
typedef VEC_TOP_RICH::iterator			VEC_TOP_RICH_ITER;

typedef std::vector<STOP_RANK_GUILD>	VEC_TOP_GUILD;
typedef VEC_TOP_GUILD::iterator			VEC_TOP_GUILD_ITER;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// LG-7 CharView
struct SCHAR_VIEW
{
	SITEMCUSTOM	m_PutOnItems_[SLOT_NSIZE_S_2];
	WORD		wChaHair;
	WORD		wChaHairColor;
	WORD		wChaFace;
	WORD		wChaLevel;
	int			nChaClass;
	WORD		wChaSchool;

	WORD		wGuNum;
	WORD		wGuMarkVer;

	char		szChaName[CHR_ID_LENGTH];
	char		szGuName[CHR_ID_LENGTH];

	int			nAttLow;
	int			nAttHigh;
	int			nDef;
	WORD		wMelee;
	WORD		wRange;
	WORD		wMagic;
	DWORD		dwMaxHP;
	int			nMaxMP;
	int			nMaxSP;
	int			nAcc;
	int			nEva;

	SCHAR_VIEW()
		: wChaHair(-1)
		, wChaHairColor(-1)
		, wChaFace(-1)
		, wChaLevel(-1)
		, nChaClass(0)
		, wChaSchool(0)

		, wGuNum(0)
		, wGuMarkVer(0)

		, nAttLow(-1)
		, nAttHigh(-1)
		, nDef(-1)
		, wMelee(-1)
		, wRange(-1)
		, wMagic(-1)
		, dwMaxHP(-1)
		, nMaxMP(-1)
		, nMaxSP(-1)
		, nAcc(-1)
		, nEva(-1)
	{
		memset(szChaName, 0, sizeof(char) * CHR_ID_LENGTH);
		memset(szGuName, 0, sizeof(char) * CHR_ID_LENGTH);
	}
};
/////////////////////////////////////////////////////////////////////////////