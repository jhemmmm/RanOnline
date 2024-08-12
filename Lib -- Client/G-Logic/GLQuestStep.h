#ifndef GLQUEST_STEP_H_
#define GLQUEST_STEP_H_

#if _MSC_VER > 1000
#pragma once
#endif


#include "../../Lib -- Engine/Common/BaseStream.h"
#include "../../Lib -- Engine/Common/StringFile.h"
#include "../../Lib -- Engine/Common/SerialFile.h"
#include "../../Lib -- Engine/Common/STRINGUTILS.h"
#include "../../Lib -- Engine/G-Logic/GLDefine.h"

#include <vector>
#include "./GLItem.h"
#include "./GLQuestGenItem.h"


typedef std::vector<SGENQUESTITEM>	GENMOBITEMARRAY;
typedef GENMOBITEMARRAY::iterator	GENMOBITEMARRAY_ITER;

typedef std::vector<SITEMCUSTOM>	INVENQARRAY;
typedef INVENQARRAY::iterator		INVENQARRAY_ITER;

struct GLQUEST_PROGRESS_MAP
{
	SNATIVEID	nidMAP;
	WORD		wPosX;
	WORD		wPosY;
	DWORD		dwGateID;

	GLQUEST_PROGRESS_MAP() :
		nidMAP(false),
		wPosX(USHRT_MAX),
		wPosY(USHRT_MAX),
		dwGateID(0)
	{
	}

	void Init()
	{
		nidMAP   = false;
		wPosX    = USHRT_MAX;
		wPosY    = USHRT_MAX;
		dwGateID = 0;
	}
};


struct GLQUEST_STEP
{
	enum { VERSION = 0x0201, };

	std::string		m_strTITLE;				//	현재 단계에 대한 제목.
	std::string		m_strCOMMENT;			//	현재 단계에 대한 설명.

	//	이수 조건 #3.
	DWORD			m_dwNID_MOBKILL;		//	죽여야할 Mob ID.
	DWORD			m_dwNUM_MOBKILL;		//	죽여야할 Mob 숫자.
	std::string		m_strOBJ_MOBKILL;		//	이수조건 설명.

	//	이수 조건 #2.
	GENMOBITEMARRAY	m_vecMOBGEN_QITEM;		//	MOB 발생 퀘스트 아이템.
	std::string		m_strOBJ_MOBGEN_QITEM;	//	이수조건 설명.

	//	이수 조건 #1.
	DWORD			m_dwNID_NPCTALK;		//	대화가 필요한 NPC ID.
	std::string		m_strOBJ_NPCTALK;		//	이수조건 설명.


	//	이수 조건 #5.
	SNATIVEID		m_sMAPID_REACH;			//	도달해야할 MAPID.
	WORD			m_wPOSX_REACH;			//	도달해야할 x좌표.	( 미니멥에서 쓰는 좌표. )
	WORD			m_wPOSY_REACH;			//	도달해야할 y좌표.	( 미니멥에서 쓰는 좌표. )
	WORD			m_wRADIUS_REACH;		//	도달 반경.
	std::string		m_strOBJ_REACH_ZONE;	//	이수 조건 설명.

	//	이수 조건 #7.
	WORD			m_wLevel;				//	레벨 도달.

	//	이수 조건 #4.
	DWORD			m_dwNID_NPCGUARD;		//	보호해야할 NPC.
	std::string		m_strOBJ_NPCGUARD;		//	이수조건 설명.

	//	이수 조건 #6.
	SNATIVEID		m_sMAPID_DEFENSE;		//	방어해야할 MAPID.
	WORD			m_wPOSX_DEFENSE;		//	방어해야할 x좌표.	( 미니멥에서 쓰는 좌표. )
	WORD			m_wPOSY_DEFENSE;		//	방어해야할 y좌표.	( 미니멥에서 쓰는 좌표. )
	WORD			m_wRADIUS_DEFENSE;		//	방어 반경.
	float			m_fDEFENSE_TIME;		//	방어해야할 시간.
	std::string		m_strOBJ_DEFENSE_ZONE;	//	이수 조건 설명.

	//	완료시 처리.
	INVENQARRAY			 m_vecGIFT_QITEM;		//	현제 STEP 완료시 보상 Quest Item.
	DWQARRAY			 m_vecRESET_QITEM;		//	다음 스텟으로 넘어갈때 리셋될 Quest Item.

	GLQUEST_PROGRESS_MAP m_stepMoveMap;

	GLQUEST_STEP () :
		m_strTITLE("Quest Step Title"),
		m_strCOMMENT("Quest Step Description"),

		m_dwNID_MOBKILL(UINT_MAX),
		m_dwNUM_MOBKILL(0),
		m_strOBJ_MOBKILL("Kill Mob"),

		m_strOBJ_MOBGEN_QITEM("Mob Generate Quest Item"),

		m_dwNID_NPCTALK(UINT_MAX),
		m_strOBJ_NPCTALK("Talk to NPC"),

		m_sMAPID_REACH(false),
		m_wPOSX_REACH(0),
		m_wPOSY_REACH(0),
		m_wRADIUS_REACH(60),
		m_strOBJ_REACH_ZONE("Reach Zone"),

		m_wLevel(USHRT_MAX),

		m_dwNID_NPCGUARD(UINT_MAX),
		m_strOBJ_NPCGUARD("Guard NPC"),

		m_sMAPID_DEFENSE(false),
		m_wPOSX_DEFENSE(0),
		m_wPOSY_DEFENSE(0),
		m_wRADIUS_DEFENSE(0),
		m_fDEFENSE_TIME(0.0f),
		m_strOBJ_DEFENSE_ZONE("Defense Zone")	
	{
	}

	const char* GetTITLE () const				{ return m_strTITLE.c_str(); }
	const char* GetCOMMENT () const				{ return m_strCOMMENT.c_str(); }

	const char* GetOBJ_NPCTALK () const			{ return m_strOBJ_NPCTALK.c_str(); }
	const char* GetOBJ_MOBGEN_QITEM () const	{ return m_strOBJ_MOBGEN_QITEM.c_str(); }
	const char* GetOBJ_MOBKILL () const			{ return m_strOBJ_MOBKILL.c_str(); }
	const char* GetOBJ_NPCGUARD () const		{ return m_strOBJ_NPCGUARD.c_str(); }
	const char* GetOBJ_REACH_ZONE () const		{ return m_strOBJ_REACH_ZONE.c_str(); }
	const char* GetOBJ_DEFENSE_ZONE () const	{ return m_strOBJ_DEFENSE_ZONE.c_str(); }

	bool IsNEED_NPCTALK () const		{ return (m_dwNID_NPCTALK!=UINT_MAX); }
	bool IsNEED_QITEM () const			{ return (!m_vecMOBGEN_QITEM.empty()); }
	bool IsNEED_MOBKILL () const		{ return (m_dwNID_MOBKILL!=UINT_MAX); }
	bool IsNEED_NPCGUARD () const		{ return (m_dwNID_NPCGUARD!=UINT_MAX); }
	bool IsNEED_REACHZONE () const		{ return (m_sMAPID_REACH!=SNATIVEID(false)); }
	bool IsNEED_DEFENSEZONE () const	{ return (m_sMAPID_DEFENSE!=SNATIVEID(false)); }
	bool IsNEED_LEVEL () const			{ return (m_wLevel!=USHRT_MAX); }

	void RESET_NPCTALK ()
	{
		m_strOBJ_NPCTALK = "";
		m_dwNID_NPCTALK = UINT_MAX;
	}

	void RESET_MOBGEN_QITEM ()
	{
		m_strOBJ_MOBGEN_QITEM = "";
		m_vecMOBGEN_QITEM.clear();
	}

	void RESET_MOBKILL ()
	{
		m_strOBJ_MOBKILL = "";
		m_dwNID_MOBKILL = UINT_MAX;
		m_dwNUM_MOBKILL = 0;
	}

	void RESET_NPCGUARD ()
	{
		m_strOBJ_NPCGUARD = "";
		m_dwNID_NPCGUARD = UINT_MAX;
	}

	void RESET_REACHZONE ()
	{
		m_strOBJ_REACH_ZONE = "";
		m_sMAPID_REACH = SNATIVEID(false);
		m_wPOSX_REACH = (0);
		m_wPOSY_REACH = (0);
		m_wRADIUS_REACH = (60);
	}

	void RESET_DEFENSE ()
	{
		m_strOBJ_DEFENSE_ZONE = "";
		m_sMAPID_DEFENSE = SNATIVEID(false);
		m_wPOSX_DEFENSE = (0);
		m_wPOSY_DEFENSE = (0);
		m_wRADIUS_DEFENSE = (0);
		m_fDEFENSE_TIME = (0.0f);
	}

	void RESET_LEVEL ()
	{
		m_wLevel = USHRT_MAX;
	}

	bool LOAD_0001 ( basestream &SFile );
	bool LOAD_0002 ( basestream &SFile );
	bool LOAD_0003 ( basestream &SFile );
	bool LOAD_0004 ( basestream &SFile );
	bool LOAD_0005 ( basestream &SFile );
	bool LOAD_0006 ( basestream &SFile );
	bool LOAD_0007 ( basestream &SFile );
	bool LOAD_0008 ( basestream &SFile );
	bool LOAD_0009 ( basestream &SFile );
	bool LOAD_0010 ( basestream &SFile );
	bool LOAD_0011 ( basestream &SFile );
	bool LOAD_0012 ( basestream &SFile );
	bool LOAD_0013 ( basestream &SFile );
	bool LOAD_0200 ( basestream &SFile );
	bool LOAD_0201 ( basestream &SFile );

	bool LOAD ( basestream &SFile );
	bool SAVE ( CSerialFile &SFile );

	GLQUEST_STEP& operator= ( const GLQUEST_STEP &rVALUE );

	void RESETITEM_ERASE ( DWORD dwIndex );
	void MOBGENITEM_ERASE ( DWORD dwIndex );
	void GIFTITEM_ERASE ( DWORD dwIndex );
};

#endif // GLQUEST_STEP_H_