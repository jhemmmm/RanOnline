#ifndef GLITEM_H_
#define GLITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
#include <vector>

#include "./GLItemDef.h"
#include "./GLCharDefine.h"

#include "../../Lib -- Engine/Common/ByteStream.h"
#include "../../Lib -- Engine/Common/SerialFile.h"
#include "../../Lib -- Engine/G-Logic/GLDefine.h"
#include "../../Lib -- Engine/G-Logic/GLList.h"
#include "../../Lib -- Engine/Meshs/DxSimpleMeshMan.h"
#include "../../Lib -- Engine/DxOctree/DxLandDef.h"
#include "../../Lib -- Engine/G-Logic/GLQuadTree.h"

#include "./GLItemBasic.h"
#include "./GLItemSuit.h"
#include "./GLItemDrug.h"
#include "./GLItemSkillBook.h"
#include "./GLItemGrinding.h"
#include "./GLItemGenerate.h"
#include "./GLItemBox.h"
#include "./GLItemRandomBox.h"
#include "./GLItemQuestion.h"
#include "./GLItemRandomOption.h"
#include "./GLItemPet.h"
#include "./GLItemVehicle.h"
#include "./GLItemPetSkin.h"
#include "./GLItemRVCard.h"


#include "./GLItemMix.h"

/*item link, Juver, 2017/07/31 */
#include "./GLItemLink.h"

//	Note : ¾ÆÀÌÅÛ °íÁ¤ ¼³Á¤.
//
struct SITEM
{
	enum
	{
		VERSION = 0x0104,

		FILE_SBASIC			= 1,	// ±âº»¾ÆÀÌÅÛ
		FILE_SSUIT			= 2,	// ÀÇº¹
		FILE_SDRUG			= 3,	// ¾àÇ°
		FILE_SSKILLBOOK		= 4,	// ½ºÅ³ºÏ
		FILE_SGRINDING		= 5,	// ¿¬¸¶¾ÆÀÌÅÛ
		FILE_SGENERATE		= 6,	// 
		FILE_BOX			= 7,	// ? ¼±¹°»óÀÚ
		FILE_RANDOMBOX		= 8,	// ·£´ý¹Ú½º
		FILE_QUESTIONITEM	= 9,	// Question Item
		FILE_RANDOMITEM		= 10,	// ·£´ý¾ÆÀÌÅÛ
		FILE_PET			= 11,	// Æê ¾ÆÀÌÅÛ	// PetData
		FILE_VEHICLE		= 12,   // Å» °Í 
		FILE_PETSKINPACK	= 13,	// Æê ½ºÅ²ÆÑ

		FILE_RVCARD			= 100,	/*rv card, Juver, 2017/11/25 */

		FILE_END_DATA		= 0xEDEDEDED,
	};

	ITEM::SITEMBASIC	sBasicOp;
	ITEM::SSUIT			sSuitOp;
	ITEM::SDRUG			sDrugOp;
	ITEM::SSKILLBOOK	sSkillBookOp;
	ITEM::SGRINDING		sGrindingOp;
	ITEM::SGENERATE		sGenerateOp;
	ITEM::SBOX			sBox;
	ITEM::SRANDOMBOX	sRandomBox;
	ITEM::SQUESTIONITEM	sQuestionItem;
	ITEM::SRANDOM_OPT	sRandomOpt;
	ITEM::SPET			sPet;	// PetData
	ITEM::SVEHICLE		sVehicle;
	ITEM::SPETSKINPACKITEM  sPetSkinPack;
	ITEM::SRVCARD		sRvCard;	/*rv card, Juver, 2017/11/25 */

	BOOL SaveFile ( CSerialFile &SFile );
	BOOL LoadFile ( basestream &SFile, bool bPastLoad );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray );

	SITEM ()
	{
	}

	SITEM ( const SITEM &value )
	{
		operator=(value);
	}

	SITEM& operator = ( const SITEM& rvalue );

	//	ÀÎº¥ Á¡À¯ »çÀÌÁî°¡ °°Àº ¾ÆÀÌÅÛÀÎÁö °Ë»ç.
	bool SAME_INVENSIZE ( const SITEM& sItem ) const
	{
		return ( sBasicOp.wInvenSizeX==sItem.sBasicOp.wInvenSizeX && sBasicOp.wInvenSizeY==sItem.sBasicOp.wInvenSizeY );
	}

	bool ISINSTANCE () const
	{
		return ( sDrugOp.bInstance!=FALSE );
	}

	bool IsTIMELMT ()
	{
		return (sBasicOp.dwFlags&ITEM_TIMELMT)!=NULL && sDrugOp.tTIME_LMT!=0;
	}

	//	°ãÄ§ °¡´ÉÇÑ ¾ÆÀÌÅÛÀÎÁö Á¡°Ë.
	bool ISPILE () const
	{
		return ( sDrugOp.bInstance && (sDrugOp.wPileNum>1) );
	}

	bool ISEVENTITEM () const
	{
		return (sBasicOp.dwFlags&TRADE_EVENT_SGL)!=NULL;
	}

	WORD GETAPPLYNUM () const;

	DWORD GETSELLPRICE ( WORD wTurnNum ) const
	{
		if ( ISPILE() )
		{
			WORD wAPPLYNUM = GETAPPLYNUM();
			DWORD dwPRICE = DWORD ( sBasicOp.dwSellPrice * wTurnNum / float(wAPPLYNUM) );
			return dwPRICE;
		}

		return sBasicOp.dwSellPrice;
	}

	float GetExpMultiple()				{ return sBasicOp.fExpMultiple; }

	const char* GetName ();
	const char* GetSelfBodyEffect ()	{	return sBasicOp.strSelfBodyEffect.c_str(); }
	const char* GetTargBodyEffect ()	{	return sBasicOp.strTargBodyEffect.c_str(); }
	const char* GetTargetEffect ()		{	return sBasicOp.strTargetEffect.c_str(); }

	const char* GetFieldFile ()			{	return sBasicOp.strFieldFile.c_str(); }
	const char* GetInventoryFile ()		{	return sBasicOp.strInventoryFile.c_str(); }
	const char* GetRandomOptFile ()		{	return sRandomOpt.szNAME; }

	const char* GetWearingFileR ( EMCHARINDEX emIndex )	{ return sBasicOp.strWearingFileRight[emIndex].c_str(); }
	const char* GetWearingFileL ( EMCHARINDEX emIndex )	{ return sBasicOp.strWearingFileLeft[emIndex].c_str(); }

	/*vehicle system, Juver, 2017/08/07 */
	//we need to adjust this if biketype exceed string array
	const char* GetVehicleWearingFile ( VEHICLE_SUBTYPE emType )	{ return sBasicOp.strWearingFileRight[emType].c_str(); }

	const char* GetComment ();

	// PET
	const char* GetPetWearingFile () { return sBasicOp.strPetWearingFile.c_str(); }

	/*item wrapper, Juver, 2018/01/11 */
	BOOL isWrappable();
};

enum EMITEMGEN
{
	EMGEN_DEFAULT	= 0,
	EMGEN_INIT		= 1,
	EMGEN_SHOP		= 2,
	EMGEN_NPC		= 3,
	EMGEN_QUEST		= 4,
	EMGEN_MOB		= 5,
	EMGEN_GMEDIT	= 6,
	EMGEN_OLD		= 7,
	EMGEN_BILLING	= 8,
	EMGEN_GMEDIT2   = 9,
	EMGEN_ITEMMIX	= 10,
	EMGEN_GATHERING	= 11,
	EMGEN_SYSTEM	= 12,

	EMGEN_ALL		= -1,
};

struct SITEMCUSTOM_100
{
	SNATIVEID	sNativeID;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST_101	sResist;
};

struct SITEMCUSTOM_101
{
	SNATIVEID	sNativeID;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST		sResist;
};

struct SITEMCUSTOM_102
{
	SNATIVEID	sNativeID;

	BYTE		cGenType;
	BYTE		cFieldID;
	DWORD		dwGenNum;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST		sResist;
};

struct SITEMCUSTOM_103
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	BYTE		cReModelReq;				//	°³Á¶ ½Ãµµ È½¼ö.
	BYTE		cReModelSuc;				//	°³Á¶ ¼º°ø È½¼ö.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	short		nHitRate;					//	¸íÁßÀ². ( +/- ) (%)
	short		nAvoidRate;					//	È¸ÇÇÀ². ( +/- ) (%)

	short		nDamage;					//	´ë¹ÌÁö. ( +/- )
	short		nDefense;					//	¹æ¾î. ( +/- )

	SRESIST		sResist;					//	ÀúÇ×°ª.
};

struct SITEMCUSTOM_104
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	BYTE		cReModelReq;				//	°³Á¶ ½Ãµµ È½¼ö.
	BYTE		cReModelSuc;				//	°³Á¶ ¼º°ø È½¼ö.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	short		nHitRate;					//	¸íÁßÀ². ( +/- ) (%)
	short		nAvoidRate;					//	È¸ÇÇÀ². ( +/- ) (%)

	short		nDamage;					//	´ë¹ÌÁö. ( +/- )
	short		nDefense;					//	¹æ¾î. ( +/- )

	SRESIST		sResist;					//	ÀúÇ×°ª.
};

struct SITEMCUSTOM_105
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.

	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.
};

struct SITEMCUSTOM_106
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.

	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.
};

struct SITEMCUSTOM_107
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.

	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.
};

struct SITEMCUSTOM_108
{
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.
	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.
	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.

	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )

	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.

	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.
};

struct SITEMCUSTOM_109
{
public:
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;
};

struct SITEMCUSTOM_110
{
public:
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

	DWORD		dwPetID;	// Æê¾Æµð
};

struct SITEMCUSTOM_111
{
public:
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

	DWORD		dwPetID;	// Æê¾Æµð
	DWORD		dwVehicleID; // Å»°Í¾ÆµÚ(DB)
};


struct SITEMCUSTOM_QUEST_115
{
public:
	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1; // °ø°Ý·Â
	BYTE		cOptTYPE2; // ±âÀúÇ×
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // °ø°Ý·Â %
	short		nOptVALUE2; // ±âÀúÇ× %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;	 // Æê¾Æµð
	DWORD		dwVehicleID; // Å»°Í¾ÆµÚ(DB)

	bool		bVietnamGainItem;	 //	º£Æ®³² Å½´Ð ÀÎº¥ ¾ÆÀÌÅÛ( º£Æ®³² Àü¿ë )

	DWORD		dwColor1;	 // Æê¾Æµð
	DWORD		dwColor2; // Å»°Í¾ÆµÚ(DB)
};

struct SITEMCUSTOM_112
{
public:
	enum { VERSION = 0x0112 };

	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1; // °ø°Ý·Â
	BYTE		cOptTYPE2; // ±âÀúÇ×
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // °ø°Ý·Â %
	short		nOptVALUE2; // ±âÀúÇ× %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;	 // Æê¾Æµð
	DWORD		dwVehicleID; // Å»°Í¾ÆµÚ(DB)

	bool		bVietnamGainItem;	 //	º£Æ®³² Å½´Ð ÀÎº¥ ¾ÆÀÌÅÛ( º£Æ®³² Àü¿ë )
};

//	Note : SITEMCUSTOM - »ý¼ºµÈ ¾ÆÀÌÅÛÀÇ Á¤º¸.
//		
//		¹öÀü º¯°æ½Ã "struct SINVENITEM_SAVE" µµ ¹öÀüÀ» º¯°æÇØÁà¾ß ÇÑ´Ù.
//		¾Æ·¡ ÇÔ¼öµéµµ ¹öÀü º¯°æ¿¡ µû¶ó ÀÔÃâ·Â ºÎºÐÀ» ¼öÁ¤ÇØ¾ß ÇÑ´Ù.
//					"BOOL SCHARDATA2::GETPUTONITEMS_BYBUF ()"
//					"BOOL SETPUTONITEMS_BYBUF ()"
//					"BOOL GLInventory::SETITEM_BYBUFFER ()"
//					"BOOL GLQUEST::LOAD ()" - INVENQARRAY
//					"BOOL GLQUEST_STEP::LOAD ()" - INVENQARRAY
//GLItemLink.h <- changes to SITEMCUSTOM must be applied here too

struct SITEM_LOBY;
struct SITEMCLIENT;

struct SITEMCUSTOM
{
public:
	enum { VERSION = 0x0200 };

	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².
	__time64_t	tBORNTIME;					//	½ÃÇÑºÎ ¾ÆÀÌÅÛÀÏ °æ¿ì À¯È¿ ±â°£.
	__time64_t	tDISGUISE;					//	½ÃÇÑºÎ ÄÚ½ºÅùÀÏ °æ¿ì À¯È¿ ±â°£.

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

public:
	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1; // °ø°Ý·Â
	BYTE		cOptTYPE2; // ±âÀúÇ×
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // °ø°Ý·Â %
	short		nOptVALUE2; // ±âÀúÇ× %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;	 // Æê¾Æµð
	DWORD		dwVehicleID; // Å»°Í¾ÆµÚ(DB)

	bool		bVietnamGainItem;	 //	º£Æ®³² Å½´Ð ÀÎº¥ ¾ÆÀÌÅÛ( º£Æ®³² Àü¿ë )

public:
	/*item color, Juver, 2018/01/04 */
	WORD		wColor1;
	WORD		wColor2;

public:
	SITEMCUSTOM ();
	SITEMCUSTOM ( SNATIVEID	sNID );

	void ResetGrind ( EMGRINDING_TYPE emTYPE );

	bool operator == ( const SITEMCUSTOM &value );
	bool operator != ( const SITEMCUSTOM &value );

	void Assign ( const SITEMCUSTOM_100 &sOld );
	void Assign ( const SITEMCUSTOM_101 &sOld );
	void Assign ( const SITEMCUSTOM_102 &sOld );
	void Assign ( const SITEMCUSTOM_103 &sOld );
	void Assign ( const SITEMCUSTOM_104 &sOld );
	void Assign ( const SITEMCUSTOM_105 &sOld );
	void Assign ( const SITEMCUSTOM_106 &sOld );
	void Assign ( const SITEMCUSTOM_107 &sOld );
	void Assign ( const SITEMCUSTOM_108 &sOld );
	void Assign ( const SITEMCUSTOM_109 &sOld );
	void Assign ( const SITEMCUSTOM_110 &sOld );
	void Assign ( const SITEMCUSTOM_111 &sOld );
	void Assign ( const SITEMCUSTOM_112 &sOld );
	void Assign ( const SITEMCUSTOM_QUEST_115 &sOld );
	void Assign ( const SITEM_LOBY &sOldLoby );
	void Assign ( const SITEMCLIENT &sClient );

	/*item link, Juver, 2017/07/31 */
	void Assign ( const SITEMLINK &sItemLink );
	void Convert ( SITEMLINK &sItemLink );

	void SetGM_GEN ( DWORD dwUserNum )
	{		
		// ÀÌ ºÎºÐÀ» ¼öÁ¤ÇÔÀ¸·Î¼­ GMCharEdit ¿¡¼­ ³ÖÀº ¾ÆÀÌÅÛÀÇ °Å·¡/ºñ°Å·¡ ¼³Á¤ÀÌ °¡´ÉÇÏ´Ù.
		// ´ë¸¸ ¿ä±¸·Î °Å·¡ Á¦ÇÑ ÀåÄ¡¸¦ ¾ø¾Ø´Ù.
		// 2005-12-16 Jgkim
		// cGenType = EMGEN_MOB; /// ÀÏ¹Ý ¸÷¿¡¼­ ¶³¾îÁø °É·Î Ã³¸®ÇÏ¸é °Å·¡°¡´É
		cGenType = EMGEN_GMEDIT; /// GMCharEdit ·Î ³Ö¾úÀ» °æ¿ì °Å·¡ºÒ°¡
		cFieldID = 0xff;
		lnGenNum = dwUserNum;
	}
	
	void SetGM_GEN2 ( DWORD dwUserNum )
	{		
		// ÀÌ ºÎºÐÀ» ¼öÁ¤ÇÔÀ¸·Î¼­ GMCharEdit ¿¡¼­ ³ÖÀº ¾ÆÀÌÅÛÀÇ °Å·¡/ºñ°Å·¡ ¼³Á¤ÀÌ °¡´ÉÇÏ´Ù.
		// ´ë¸¸ ¿ä±¸·Î °Å·¡ Á¦ÇÑ ÀåÄ¡¸¦ ¾ø¾Ø´Ù.
		// 2005-12-16 Jgkim
		// cGenType = EMGEN_MOB; /// ÀÏ¹Ý ¸÷¿¡¼­ ¶³¾îÁø °É·Î Ã³¸®ÇÏ¸é °Å·¡°¡´É
		cGenType = EMGEN_GMEDIT2; /// GMCharEdit ·Î ³Ö¾úÀ» °æ¿ì °Å·¡ºÒ°¡
		cFieldID = 0xff;
		lnGenNum = dwUserNum;
	}

	//! GMCharEdit ·Î »ý¼ºµÈ ¾ÆÀÌÅÛÀÎÁö °Ë»çÇÑ´Ù.
	bool IsGM_GENITEM () const
	{
		return cGenType == EMGEN_GMEDIT;
	}

public:
	WORD GETGRADE_DAMAGE () const;			//	¿¬¸¶ µî±Þ¿¡ µû¸¥ °ø°Ý·Â °¡»ê·®.
	WORD GETGRADE_DEFENSE () const;			//	¿¬¸¶ µî±Þ¿¡ µû¸¥ ¹æ¾î·Â °¡»ê·®.
	WORD GETGRADE_RESIST_FIRE () const;		//	¿¬¸¶ µî±Þ¿¡ µû¸¥ ºÒÀúÇ× °¡»ê·®.
	WORD GETGRADE_RESIST_ICE () const;		//	¿¬¸¶ µî±Þ¿¡ µû¸¥ ºùÀúÇ× °¡»ê·®.
	WORD GETGRADE_RESIST_ELEC () const;		//	¿¬¸¶ µî±Þ¿¡ µû¸¥ ÀüÀúÇ× °¡»ê·®.
	WORD GETGRADE_RESIST_POISON () const;	//	¿¬¸¶ µî±Þ¿¡ µû¸¥ µ¶ÀúÇ× °¡»ê·®.
	WORD GETGRADE_RESIST_SPIRIT () const;	//	¿¬¸¶ µî±Þ¿¡ µû¸¥ ±âÀúÇ× °¡»ê·®.

	BYTE GETGRADE ( EMGRINDING_TYPE emTYPE ) const;
	BYTE GETGRADE_EFFECT () const;

public:
	BYTE grade_damage () const;
	BYTE grade_defense () const;
	BYTE grade_fire () const				{ return cRESIST_FIRE; }
	BYTE grade_ice () const					{ return cRESIST_ICE; }
	BYTE grade_elec () const				{ return cRESIST_ELEC; }
	BYTE grade_poison () const				{ return cRESIST_POISON; }
	BYTE grade_spirit () const				{ return cRESIST_SPIRIT; }

public:
	GLPADATA GETDAMAGE () const;			//	±âº» ´ë¹̀Áö + ·£´ư¿É + ¿¬¸¶.
	DWORD GETMaDAMAGE () const;				//	±âº» ¸¶·ÂÄ¡ + ·£´ư¿É + ¿¬¸¶.
	short GETDEFENSE () const;				//	±âº» ¹æ¾î·Â + ·£´ư¿É + ¿¬¸¶.
	short GETHITRATE () const;				//	±âº» ¸íÁßÀ² + ·£´ư¿É.
	short GETAVOIDRATE () const;			//	±âº» ¸íÁßÀ² + ·£´ư¿É.

	float GETAVOIDRATE_PER () const;
	float GETHITRATE_PER () const;


	WORD GETATTRANGE () const;				//	±âº» °ø°Ư°Å¸® + ·£´ư¿É.
	WORD GETREQ_SP () const;				//	±âº» sp¼̉¸đ + ·£´ư¿É.

	WORD GETRESIST_FIRE () const;			//	±âº» ÀúÇ×(È­) + ·£´ư¿É.
	WORD GETRESIST_ICE () const;			//	±âº» ÀúÇ×(ºù) + ·£´ư¿É.
	WORD GETRESIST_ELEC () const;			//	±âº» ÀúÇ×(Àü) + ·£´ư¿É.
	WORD GETRESIST_POISON () const;			//	±âº» ÀúÇ×(µ¶) + ·£´ư¿É.
	WORD GETRESIST_SPIRIT () const;			//	±âº» ÀúÇ×(±â) + ·£´ư¿É.

	int GETADDHP () const;					//	ºÎ°¡ ¿É¼Ç hp Áơ°¡ + ·£´ư¿É.
	int GETADDMP () const;					//	ºÎ°¡ ¿É¼Ç hp Áơ°¡ + ·£´ư¿É.
	int GETADDSP () const;					//	ºÎ°¡ ¿É¼Ç hp Áơ°¡ + ·£´ư¿É.
	int GETADDMA () const;					//	ºÎ°¡ ¿É¼Ç hp Áơ°¡ + ·£´ư¿É. 
	int GETADDPA () const;
	int GETADDSA () const;
	int GETADDENERGY () const;


	float GETINCHP () const;				//	Æ¯¼ö ¿É¼Ç hp Áơ°¡À² + ·£´ư¿É.
	float GETINCMP () const;				//	Æ¯¼ö ¿É¼Ç mp Áơ°¡À² + ·£´ư¿É.
	float GETINCSP () const;				//	Æ¯¼ö ¿É¼Ç sp Áơ°¡À² + ·£´ư¿É.
	float GETINCAP () const;				//	Æ¯¼ö ¿É¼Ç ap Áơ°¡À² + ·£´ư¿É.

	float GETMOVESPEED() const;				//  Æ¯¼ö ¿É¼Ç À̀µ¿¼Óµµ Áơ°¡ + ·£´ư¿É
	float GETMOVESPEEDR() const;			//  Æ¯¼ö ¿É¼Ç À̀µ¿¼Óµµ Áơ°¡À² 


	WORD GET_STAT_POW () const;	
	WORD GET_STAT_VIT () const;		
	WORD GET_STAT_INT () const;		
	WORD GET_STAT_DEX () const;		
	WORD GET_STAT_STM () const;	

	int GET_HP_POTION () const;	
	int GET_MP_POTION () const;	
	int GET_SP_POTION () const;
	
public:
	GLPADATA getdamage () const;	//	´ë¹ÌÁö + ·£´ý¿É Èû.
	WORD getmadamage () const;		//	¸¶·Â + ·£´ý¿É ¸¶·Â.
	short getdefense () const;		//	¹æ¾î·Â + ·£´ý¿É ¹æ¾î.

public:
	float GETOptVALUE ( EMRANDOM_OPT emOPT ) const;
	short GETOptVALUE_SHORT ( EMRANDOM_OPT emOPT ) const;

public:
	EMRANDOM_OPT GETOptTYPE1 () const	{	return (EMRANDOM_OPT)cOptTYPE1; }
	EMRANDOM_OPT GETOptTYPE2 () const	{	return (EMRANDOM_OPT)cOptTYPE2; }
	EMRANDOM_OPT GETOptTYPE3 () const	{	return (EMRANDOM_OPT)cOptTYPE3; }
	EMRANDOM_OPT GETOptTYPE4 () const	{	return (EMRANDOM_OPT)cOptTYPE4; }

	float GETOptVALUE1 () const;
	float GETOptVALUE2 () const;
	float GETOptVALUE3 () const;
	float GETOptVALUE4 () const;

	BOOL IsPerRandOpt( INT nRandOPT ) const;
	BOOL IsSetRandOpt() const;

protected:
	float GETOptVALUE ( EMRANDOM_OPT emOPT, short nVALUE ) const;
	void SETOptVALUE ( int nIndex, EMRANDOM_OPT emOPT, float fVALUE );
	void SETOptVALUEEmpty( EMRANDOM_OPT emOPT, float fVALUE );	// ITEMREBUILD_MARK
	int RESETOptVALUE( int nOpt = -1 );
	bool IsValidRESETOpt( int nIndex );

public:
	bool GENERATE_RANDOM_OPT( bool bNew = true, int nOpt = -1 );
	int GENERATE_RANDOM_OPT_SE( const SRANDOM_OPT_SET* pOptSet );

public:
	friend struct SITEMCLIENT;
	friend class GLITEMLMT;

public:
	BOOL	isExistStat( BYTE _ctype, short _cval );
};

BOOL SETPUTONITEMS_BYBUF ( SITEMCUSTOM *pPutOnItems, CByteStream &ByteStream );


struct SITEM_LOBY
{
public:
	enum { VERSION = 0x0101 };

	SNATIVEID	sNativeID;					//	°íÀ¯ ID. ( ¾ÆÀÌÅÛ ¼³Á¤ ÃÖÃÊ »ý¼º½Ã¿¡ ºÎ¿©µÇ´Â °íÀ¯ ID )
	SNATIVEID	nidDISGUISE;				//	ÄÚ½ºÅù º¹Àå ½ºÅ².

	LONGLONG	lnGenNum;					//	»ý¼º ¹øÈ£.
	WORD		wTurnNum;					//	»ç¿ë°¡´É·®. ( wUsedNum )
	BYTE		cGenType;					//	»ý¼º Å¸ÀÔ.
	BYTE		cChnID;						//	Ã¤³Î ¹øÈ£.

	BYTE		cFieldID;					//	ÇÊµå ¹øÈ£.

public:
	BYTE		cDAMAGE;					//	°ø°Ý·Â °³Á¶ µî±Þ.
	BYTE		cDEFENSE;					//	¹æ¾î·Â °³Á¶ µî±Þ.
	BYTE		cRESIST_FIRE;				//	ÀúÇ×(È­) °³Á¶ µî±Þ.
	BYTE		cRESIST_ICE;				//	ÀúÇ×(ºù) °³Á¶ µî±Þ.
	BYTE		cRESIST_ELEC;				//	ÀúÇ×(Àü) °³Á¶ µî±Þ.
	BYTE		cRESIST_POISON;				//	ÀúÇ×(µ¶) °³Á¶ µî±Þ.
	BYTE		cRESIST_SPIRIT;				//	ÀúÇ×(Á¤) °³Á¶ µî±Þ.

	//	·£´ý ¿É¼Ç »ç¿ë.
	BYTE		cOptTYPE1; // °ø°Ý·Â
	BYTE		cOptTYPE2; // ±âÀúÇ×
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // °ø°Ý·Â %
	short		nOptVALUE2; // ±âÀúÇ× %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	/*item color, Juver, 2018/01/04 */
	WORD		wColor1;
	WORD		wColor2;

public:
	SITEM_LOBY ();
	SITEM_LOBY ( SNATIVEID	sNID );

	bool operator == ( const SITEM_LOBY &value );
	bool operator != ( const SITEM_LOBY &value );

	void Assign ( const SITEMCUSTOM &sItemCustom );
};

BOOL SETPUTONITEMS_BYBUF ( SITEM_LOBY *pPutOnItems, CByteStream &ByteStream );


//	Note : Å¬¶óÀÌ¾ðÆ®¿ë °£·« Á¤º¸. ( ÇÃ·¡ÀÌ¾î ÀÚ½Å °Í ÀÌ¿ÜÀÇ °æ¿ì. )
//
struct SITEMCLIENT
{
	SNATIVEID	sNativeID;
	SNATIVEID	nidDISGUISE;

	BYTE		cOptTYPE1; // ÀÌ¼Ó·£´ý¿É¼Ç¸¸ ¹Þ¾Æ¿É´Ï´Ù.
	short		nOptVALUE1; // ÀÌ¼Ó·£´ý¿É¼Ç¸¸ ¹Þ¾Æ¿É´Ï´Ù.

	BYTE		cGRADE;
	WORD		wTurnNum;

public:
	/*item color, Juver, 2018/01/04 */
	WORD		wColor1;
	WORD		wColor2;

	SITEMCLIENT () 
		: sNativeID(false)
		, nidDISGUISE(false)
		, cGRADE(0)
		, wTurnNum(0)
		, cOptTYPE1(0) 
		, nOptVALUE1(0) 

		/*item color, Juver, 2018/01/04 */
		, wColor1(ITEMCOLOR_WHITE)
		, wColor2(ITEMCOLOR_WHITE)
	{
	}

	SITEMCLIENT ( const SNATIVEID &sNID ) 
		: sNativeID(sNID)
		, nidDISGUISE(false)
		, cGRADE(0)
		, wTurnNum(0)
		, cOptTYPE1(0) 
		, nOptVALUE1(0) 

		/*item color, Juver, 2018/01/04 */
		, wColor1(ITEMCOLOR_WHITE)
		, wColor2(ITEMCOLOR_WHITE)
	{
	}


	SITEMCLIENT& Assign ( const SITEMCUSTOM &Input )
	{
		sNativeID = Input.sNativeID;
		nidDISGUISE = Input.nidDISGUISE;
		wTurnNum = Input.wTurnNum;
		cGRADE = Input.grade_damage()>Input.grade_defense() ? Input.grade_damage() : Input.grade_defense();

		nOptVALUE1 = Input.GETOptVALUE_SHORT( EMR_OPT_MOVE_SPEED );		
		if ( nOptVALUE1 != 0 ) cOptTYPE1 = (BYTE) EMR_OPT_MOVE_SPEED;
		
		/*item color, Juver, 2018/01/04 */
		wColor1 = Input.wColor1;
		wColor2 = Input.wColor2;

		return *this;
	}

	EMRANDOM_OPT GETOptTYPE1 () const	{	return (EMRANDOM_OPT)cOptTYPE1; }

	BYTE GETGRADE () const		{ return cGRADE; }
	BYTE GETGRADE_EFFECT () const;
};

//	Note : ¾ÆÀÌÅÛÀÌ ÇÊµå¿¡ ¶³¾îÁ³À» ¶§ÀÇ Á¤º¸.
//
struct SDROP_ITEM
{
	SITEMCUSTOM	sItemCustom;			//	¾ÆÀÌÅÛ ´ëÀÌÅÍ.

	SNATIVEID	sMapID;					//	¸Ê ID.
	DWORD		dwCeID;					//	¼¿ ID.
	DWORD		dwGlobID;				//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë.
	D3DXVECTOR3	vPos;					//	¸Ê À§Ä¡.

	SDROP_ITEM ()
		: dwCeID(0)
		, dwGlobID(0)
		, vPos(0,0,0)
	{
	}
};

struct SDROP_CLIENT_ITEM
{
	SITEMCLIENT	sItemClient;

	DWORD		dwGlobID;				//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë.
	D3DXVECTOR3	vPos;					//	¸Ê À§Ä¡.

	float		fAge;

	SDROP_CLIENT_ITEM ()
		: dwGlobID(0)
		, vPos(0,0,0)
		, fAge(0)
	{
	}

	SDROP_CLIENT_ITEM& Assign ( SDROP_ITEM &Input )
	{
		sItemClient.Assign ( Input.sItemCustom );

		dwGlobID = Input.dwGlobID;
		vPos = Input.vPos;

		return *this;
	}
};

#ifndef GLLandMan
class GLLandMan;
class GLLandNode;
typedef SQUADNODE<GLLandNode> LANDQUADNODE;
#endif //GLLandMan

#ifndef GLLandManClient
class GLClientNode;
class GLLandNode;
typedef SQUADNODE<GLClientNode>		CLIENTQUADNODE;
#endif //GLLandMan

//	Note : Server ¿ë ItemDrop °ü¸® Å¬·¡½º.
//
class CItemDrop
{
public:
	union
	{
		struct
		{
			SDROP_ITEM			sDrop;			//	µå·Ó Á¤º¸.
		};

		struct
		{
			SITEMCUSTOM			sItemCustom;	//	¾ÆÀÌÅÛ ´ëÀÌÅÍ.

			SNATIVEID			sMapID;			//	¸Ê ID.
			DWORD				dwCeID;			//	¼¿ ID.
			DWORD				dwGlobID;		//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë.
			D3DXVECTOR3			vPos;			//	¸Ê À§Ä¡.
		};
	};

public:
	float					fAge;			//	°æ°ú ½Ã°£.

	EMGROUP					emGroup;		//	ÀÓ½Ã ¼ÒÀ¯±ÇÀÚ ´ÜÀ§.
	DWORD					dwHoldGID;		//	ÀÓ½Ã ¼ÒÀ¯±ÇÀÚ.

	LANDQUADNODE*			pQuadNode;		//	Äõµå Æ®¸® ³ëµå.
	SGLNODE<CItemDrop*>*	pCellList;		//	¼¿ ¸®½ºÆ® ³ëµå.
	SGLNODE<CItemDrop*>*	pGlobList;		//	Àü¿ª ¸®½ºÆ® ³ëµå.

	BOOL					bMobGen;

public:
	bool IsTakeItem ( DWORD dwPartyID, DWORD dwGaeaID );
	bool IsDropOut();

	void Update ( float fElapsedTime );

public:
	CItemDrop(void)
		: fAge(0.0f)
		
		, emGroup(EMGROUP_ONE)
		, dwHoldGID(GAEAID_NULL)
		
		, pQuadNode(NULL)
		, pCellList(NULL)
		, pGlobList(NULL)
		, bMobGen(FALSE)
	  {
	  }
};

typedef CItemDrop* PITEMDROP;

typedef CGLLIST<PITEMDROP>	ITEMDROPLIST;
typedef SGLNODE<PITEMDROP>	ITEMDROPNODE;


//	Note : Client ¿ë ItemDrop °ü¸® Å¬·¡½º.
//
class DxSimMesh;
struct CLIPVOLUME;
class CItemClientDrop
{
public:
	union
	{
		struct
		{
			SDROP_CLIENT_ITEM	sDrop;			//	µå·Ó Á¤º¸.
		};

		struct
		{
			SITEMCLIENT			sItemClient;	//	¾ÆÀÌÅÛ ´ëÀÌÅÍ.

			SNATIVEID			sMapID;			//	¸Ê ID.
			DWORD				dwCeID;			//	¼¿ ID.
			DWORD				dwGlobID;		//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë.
			D3DXVECTOR3			vPos;			//	¸Ê À§Ä¡.
		};
	};

public:
	float						fAge;			//	°æ°ú ½Ã°£.
	float						fNextMsgDelay;	//	ÀÌ¹Ì ÁÖÀ»·Á°í ¸Þ½ÃÁö º¸³ÁÀ» °æ¿ì ´ÙÀ½À¸·Î ½Ãµµ°¡´É½Ã°£ ¼³Á¤.

	D3DXMATRIX					matWld;			//	¾ÆÀÌÅÆ À§Ä¡.
	D3DXVECTOR3					vMax;
	D3DXVECTOR3					vMin;

	DxSimMesh*					pSimMesh;

	CLIENTQUADNODE*				pQuadNode;		//	Äõµå Æ®¸® ³ëµå.
	SGLNODE<CItemClientDrop*>*	pCellList;		//	¼¿ ¸®½ºÆ® ³ëµå.
	SGLNODE<CItemClientDrop*>*	pGlobList;		//	Àü¿ª ¸®½ºÆ® ³ëµå.

public:
	CItemClientDrop (void)
		: fAge(0.0f)
		, fNextMsgDelay(0.0f)
		
		, vMax(0,0,0)
		, vMin(0,0,0)
		
		, pSimMesh(NULL)
		
		, pQuadNode(NULL)
		, pCellList(NULL)
		, pGlobList(NULL)
	  {
	  }

	  bool IsCollision ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt ) const;
	  bool IsCollision ( CLIPVOLUME &cv ) const;
	  HRESULT RenderItem ( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matWld, D3DMATERIALQ* pMaterials=NULL );
};

typedef CItemClientDrop*			PITEMCLIENTDROP;

typedef CGLLIST<PITEMCLIENTDROP>	ITEMCLIENTDROPLIST;
typedef SGLNODE<PITEMCLIENTDROP>	ITEMCLIENTDROPNODE;


//	Note : °¢Á¾ ¾ÆÀÌÅÛ Á¤º¸ °ü¸®, ¸®½ºÆ® °ü¸® Å¬·¡½º.
//
enum EMITEMMAN
{
	ITEMMAN_EDITOR		= 0x0001,
	ITEMID_NOTFOUND		= 0xFFFF
};


//	Note : ÀÎº¥Åä¸®¿¡ ¾ÆÀÌÅÛÀ» ³ÖÀ»¶§ ÀúÀåµÇ´Â ±¸Á¶Ã¼.
//
struct SINVENITEM_SAVE_100
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_100	sItemCustom;
};

struct SINVENITEM_SAVE_101
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_101	sItemCustom;
};

struct SINVENITEM_SAVE_102
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_102	sItemCustom;
};

struct SINVENITEM_SAVE_103
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_103	sItemCustom;
};

struct SINVENITEM_SAVE_104
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_104	sItemCustom;
};

struct SINVENITEM_SAVE_105
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_105	sItemCustom;
};

struct SINVENITEM_SAVE_106
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_106	sItemCustom;
};

struct SINVENITEM_SAVE_107
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_107	sItemCustom;
};

struct SINVENITEM_SAVE_108
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_108	sItemCustom;
};

struct SINVENITEM_SAVE_109
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_109	sItemCustom;
};

struct SINVENITEM_SAVE_110
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_110	sItemCustom;
};

struct SINVENITEM_SAVE_111
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_111	sItemCustom;
};

struct SINVENITEM_SAVE_112
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_112	sItemCustom;
};

struct SINVENITEM_SAVE
{
	enum { VERSION = 0x0200 };

	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM	sItemCustom;

	SINVENITEM_SAVE ()
		: wPosX(0)
		, wPosY(0)
	{
	}

	void Assign ( const SINVENITEM_SAVE_100 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_101 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_102 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_103 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_104 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_105 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_106 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_107 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_108 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_109 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_110 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_111 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_112 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}
};

//	Note : ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®¿¡ ³ÖÀ»¶§ ¾²´Â ±¸Á¶.
//
struct SINVENITEM
{
	union
	{
		struct
		{
			SINVENITEM_SAVE		sSaveData;
		};

		struct
		{
			WORD				wPosX;
			WORD				wPosY;

			SITEMCUSTOM			sItemCustom;
		};
	};

	//itemmall
	WORD				wItemPrice;
	WORD				wItemStock;
	WORD				wItemCtg;
	WORD				wCurrency;
	char				szPurKey[PURKEY_LENGTH+1];

	WORD					wBackX;
	WORD					wBackY;

	SINVENITEM () 
		: wBackX(0)
		, wBackY(0)
	{
	}

	SINVENITEM ( WORD _wPosX, WORD _wPosY ) 
		: wBackX( _wPosX )
		, wBackY( _wPosY )
	{
	}

	bool operator== ( SINVENITEM &value )
	{
		return ( wPosX==value.wPosX&&wPosY==value.wPosY&&sItemCustom==value.sItemCustom);
	}

	bool operator!= ( SINVENITEM &value )
	{
		return ( !operator==(value) );
	}
};

struct SINVEN_POS	// ITEMREBUILD_MARK
{
	WORD	wPosX, wPosY;	// ÀÎº¥Åä¸®¿¡¼­ÀÇ À§Ä¡

	SINVEN_POS()
		: wPosX( USHRT_MAX )
		, wPosY( USHRT_MAX )
	{
	}
	SINVEN_POS( WORD _wposx, WORD _wposy )
		: wPosX( _wposx )
		, wPosY( _wposy )
	{
	}
	VOID SET( WORD _wposx, WORD _wposy )
	{
		wPosX = _wposx;
		wPosY = _wposy;
	}
	BOOL VALID()
	{
		return (BOOL)( wPosX != USHRT_MAX && wPosY != USHRT_MAX );
	}
	VOID RESET()
	{
		wPosX = USHRT_MAX;
		wPosY = USHRT_MAX;
	}
};

/*item preview, Juver, 2017/07/27 */
enum EMITEM_PREVIEW_SLOT
{ 
	EMITEM_PREVIEW_SLOT_HEAD	= 0, 
	EMITEM_PREVIEW_SLOT_UPPER	= 1, 
	EMITEM_PREVIEW_SLOT_LOWER	= 2, 
	EMITEM_PREVIEW_SLOT_HAND	= 3, 
	EMITEM_PREVIEW_SLOT_FOOT	= 4, 
	EMITEM_PREVIEW_SLOT_WEAPON	= 5, 
	EMITEM_PREVIEW_SLOT_TOTAL	= 6, 
};

/*item preview, Juver, 2017/07/27 */
struct SITEM_PREVIEW_DATA
{
	WORD wFace;
	WORD wHair;
	WORD wHairColor;
	SITEMCUSTOM sPutOn[EMITEM_PREVIEW_SLOT_TOTAL];

	SITEM_PREVIEW_DATA()
		: wFace( 0 )
		, wHair ( 0 )
		, wHairColor ( 0 )
	{
	}
};

struct SITEMSEALDATA
{
	int				nOption;
	EMRANDOM_OPT	emOpt;
	//float			fDataValue;
	short			cDataValue;

	SITEMSEALDATA()
		: nOption( -1 )
		, emOpt( EMR_OPT_NULL )
		//, fDataValue( 0.0f )
		, cDataValue(0)
	{
	}

	void RESET()
	{
		nOption = -1;
		emOpt = EMR_OPT_NULL;
		//fDataValue = 0.0f;
		cDataValue = 0;
	}
};
#endif // GLITEM_H_
