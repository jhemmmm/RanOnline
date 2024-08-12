#ifndef GLCHARDATA_H_
#define GLCHARDATA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <hash_map>

#include "./GLFactData.h"

#include "./GLInventory.h"
#include "./GLItem.h"
#include "./GLQuestPlay.h"
#include "./GLSkill.h"
#include "./GLVEHICLE.h"

#include "../../Lib -- Engine/Common/ByteStream.h"

/*skill summon, Juver, 2017/10/09 */
#include "GLSummonData.h"

/*activity system, Juver, 2017/10/23 */
#include "GLActivityData.h"

/*pvp capture the flag, Juver, 2018/01/29 */
#include "GLPVPCaptureTheFlagDefine.h"

#define SCALERANGE_MIN	(0.88f) //(0.75f)
#define SCALERANGE_MAX	(1.12f) //(1.20f)

/*charinfoview , Juver, 2017/11/11 */
#define REQ_CHARINFO_DELAY 5.0f

//dmk14 | 11-4-16 | pk ranking
#define CHAR_NAME		33
//#define RANKING_PKNUM		100

struct PLAYER_RANKING
{
	int		nIndex;   
	DWORD	dwCharID;

	EMCHARINDEX	dwClass;
	WORD	wSchool;
	WORD	wCharRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char szCharName[CHAR_NAME+1];

	PLAYER_RANKING() 
		: nIndex(-1)
		, dwCharID(0)
		, dwClass(GLCI_BRAWLER_M)
		, wSchool(0)
		, wCharRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_NAME+1) );
	}

	PLAYER_RANKING ( const PLAYER_RANKING &value )
	{
		operator=(value);
	}

	PLAYER_RANKING& operator = ( const PLAYER_RANKING& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		dwClass = rvalue.dwClass;
		wSchool = rvalue.wSchool;
		wCharRanking = rvalue.wCharRanking;		
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		StringCchCopy( szCharName, CHAR_NAME+1, rvalue.szCharName );

		return *this;
	}

	bool operator < ( const PLAYER_RANKING& sSwPlayerRank )
	{	
		/*
		float fKDR1 = 0.0f;
		float fKDR2 = 0.0f;
		
		fKDR1 = (float) ( wKillNum ) / ( max( 1,wDeathNum ) );
		fKDR2 = (float) ( sSwPlayerRank.wKillNum ) / ( max( 1, sSwPlayerRank.wDeathNum ));		
		if( fKDR1 > fKDR2 ) return true;
		if( fKDR1 >= fKDR2 ) return true;*/

		if ( wKillNum > sSwPlayerRank.wKillNum ) return true;
		else if ( wKillNum == sSwPlayerRank.wKillNum && wDeathNum < sSwPlayerRank.wDeathNum ) return true;


		return  false;
	}

	bool operator == ( const PLAYER_RANKING& sSwPlayerRank ) 
	{
		/*
		float fKDR1 = 0.0f;
		float fKDR2 = 0.0f;

		
		fKDR1 = (float) ( wKillNum ) / ( max( 1,wDeathNum ) );
		fKDR2 = (float) ( sSwPlayerRank.wKillNum ) / ( max( 1, sSwPlayerRank.wDeathNum ));		

		if ( fKDR1 == fKDR2 ) return true;
		*/
		if( wKillNum == sSwPlayerRank.wKillNum && wDeathNum == sSwPlayerRank.wDeathNum )	return true;

		return false;

	}
};

struct PLAYER_RANKING_EX
{
	EMCHARINDEX	dwClass;
	WORD	wSchool;
	WORD	wCharRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szCharName[CHAR_NAME+1];

	PLAYER_RANKING_EX()
		: dwClass(GLCI_BRAWLER_M)
		, wSchool(0)
		, wCharRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_NAME+1) );
	}

	void Init()
	{
		dwClass = GLCI_BRAWLER_M;
		wSchool = 0;
		wCharRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
		StringCchCopy( szCharName, CHAR_NAME+1, "" );
	}
};

typedef std::vector<PLAYER_RANKING>					PLAYER_RANKING_STATUS_VEC;
typedef PLAYER_RANKING_STATUS_VEC::iterator			PLAYER_RANKING_STATUS_VEC_ITER;

typedef std::map< DWORD, PLAYER_RANKING >			PLAYER_RANKING_STATUS_MAP;
typedef PLAYER_RANKING_STATUS_MAP::iterator			PLAYER_RANKING_STATUS_MAP_ITER;
//dmk14 | 11-4-16 | pk ranking end

enum EMCHARDATA
{
	EMSKILLQUICK_VERSION	= 0x0102,
	EMSKILLQUICK_SIZE_101	= 60,
	EMSKILLQUICK_SIZE		= 60,

	EMACTIONQUICK_SIZE		= 12,

	SKILLREALFACT_SIZE		= 14,
	SKILLFACT_SIZE			= 14,

	EMMAX_CLUB_NUM			= 100,

	EMSTORAGE_CHANNEL				= 5,
	
	EMSTORAGE_CHANNEL_DEF			= 0,
	EMSTORAGE_CHANNEL_DEF_SIZE		= 1,

	EMSTORAGE_CHANNEL_SPAN			= 1,
	EMSTORAGE_CHANNEL_SPAN_SIZE		= 3,

	EMSTORAGE_CHANNEL_PREMIUM		= 4,
	EMSTORAGE_CHANNEL_PREMIUM_SIZE	= 1,

	 /*itemfood system, Juver, 2017/05/24 */
	FITEMFACT_SIZE			= 4,
	FITEMFACT_DEFAULT_SIZE	= 2,

	/*system buffs, Juver, 2017/09/04 */
	SYSTEM_BUFF_SIZE			= 5,
};

enum EMGM_EVENT_TYPE
{
	EMGM_EVENT_NONE		= 0x00,	//	없음.
	EMGM_EVENT_SPEED	= 0x01,	//	이동속도.
	EMGM_EVENT_ASPEED	= 0x02,	//	공격속도.
	EMGM_EVENT_ATTACK	= 0x04,	//	공격력.
};

struct SCHARSKILL
{
	static DWORD VERSION;
	static DWORD SIZE;

	SNATIVEID	sNativeID;
	WORD		wLevel;

	SCHARSKILL () :
		sNativeID(SNATIVEID::ID_NULL,SNATIVEID::ID_NULL),
		wLevel(0)
	{
	}

	SCHARSKILL ( const SNATIVEID &_sNID, const WORD _wLvl ) :
		sNativeID(_sNID),
		wLevel(_wLvl)
	{
	}
};

struct DAMAGE_SPEC
{
	float		m_fPsyDamageReduce;				// 물리 데미지 흡수율
	float		m_fMagicDamageReduce;			// 마법 데미지 흡수율
	float		m_fPsyDamageReflection;			// 물리 데미지 반사율
	float		m_fPsyDamageReflectionRate;		// 물리 데미지 반사확률
	float		m_fMagicDamageReflection;		// 마법 데미지 반사율
	float		m_fMagicDamageReflectionRate;	// 마법 데미지 반사확률

	DAMAGE_SPEC()	:
		m_fPsyDamageReduce(0.0f),
		m_fMagicDamageReduce(0.0f),
		m_fPsyDamageReflection(0.0f),
		m_fPsyDamageReflectionRate(0.0f),
		m_fMagicDamageReflection(0.0f),
		m_fMagicDamageReflectionRate(0.0f)
	{
	}


	void RESET()
	{
		*this = DAMAGE_SPEC();
	}
};

struct DEFENSE_SKILL
{

	SNATIVEID	m_dwSkillID;					//	발동형 스킬 MID/SID
	WORD		m_wLevel;						//	발동형 스킬 레벨
	float		m_fRate;						//	발동형 스킬 MID/SID 확률
	bool		m_bActive;						//	발동형 스킬 구동상태

	DEFENSE_SKILL() 
		: m_dwSkillID ( NATIVEID_NULL() )
		, m_wLevel ( 0 )
		, m_fRate ( 0.0f )
		, m_bActive(false)
	{
	}

	void RESET()
	{
		m_dwSkillID = NATIVEID_NULL();
		m_wLevel = 0;
		m_fRate = 0.0f;
	}
};

enum EMACTION_SLOT
{
	EMACT_SLOT_NONE	= 0,
	EMACT_SLOT_DRUG	= 1,
};

struct SACTION_SLOT
{
	enum { VERSION = 0x0100, };

	WORD		wACT;
	SNATIVEID	sNID;

	SACTION_SLOT () :
		wACT(EMACT_SLOT_NONE),
		sNID(false)
	{
	}

	bool VALID () const
	{
		return wACT!=EMACT_SLOT_NONE && sNID!=SNATIVEID(false);
	}

	void RESET ()
	{
		wACT = EMACT_SLOT_NONE;
		sNID = SNATIVEID(false);
	}
};

//	Note : 케릭터 1차 항목.
//
struct SCHARDATA
{
protected:
	DWORD				m_dwUserID;
public:
	void SetUserID( DWORD dwUserID) { m_dwUserID = dwUserID; }
	DWORD GetUserID() { return m_dwUserID; }

public:
	DWORD				m_dwUserLvl;				//	사용자 권한 Level.
	__time64_t			m_tPREMIUM;					//	프리미엄 기한.
	bool				m_bPREMIUM;					//	현제 프리미엄 상태.
	__time64_t			m_tCHATBLOCK;				//	귓말 차단.

	__time64_t			m_tSTORAGE[EMSTORAGE_CHANNEL_SPAN_SIZE];	//	창고 기한.
	bool				m_bSTORAGE[EMSTORAGE_CHANNEL_SPAN_SIZE];	//	창고 2 사용.

	WORD				m_wINVENLINE;				//	추가된 인벤토리 줄수.

	DWORD				m_dwServerID;			
	DWORD				m_dwCharID;

	char				m_szName[CHAR_SZNAME];		//	이름. (고정)

	EMTRIBE				m_emTribe;					//	종족. (고정)
	EMCHARCLASS			m_emClass;					//	직업. (고정)
	WORD				m_wSchool;					//	학원.
	WORD				m_wSex;						//	성별.
	WORD				m_wHair;					//	머리스타일.
	WORD				m_wHairColor;				//	머리색상
	WORD				m_wFace;					//	얼굴모양.

	int					m_nBright;					//	속성.
	int					m_nLiving;					//	생활.
	
	WORD				m_wLevel;					//	레벨.
	LONGLONG			m_lnMoney;					//	소지 금액.

	bool				m_bMoneyUpdate;				// 돈이 업데이트 되었는지 아닌지
	bool				m_bStorageMoneyUpdate;		// 창고 돈이 업데이트 되었는지 아닌지
	WORD				m_wTempLevel;				// 비교용 레벨 임시 변수
	LONGLONG			m_lnTempMoney;				// 비교용 소지 금액 임시 변수
	LONGLONG			m_lnTempStorageMoney;		// 비교용 창고  금액 임시 변수

	LONGLONG			m_lVNGainSysMoney;			//  베트남 탐닉 방지 저장 금액

	DWORD				m_dwGuild;					//	길드 번호.
	char				m_szNick[CHAR_SZNAME];		//	별명.
	__time64_t			m_tSECEDE;					//	탈퇴시간.

	SCHARSTATS			m_sStats;					//	Stats.
	WORD				m_wStatsPoint;				//	가용 stats 포인트.

	WORD				m_wAP;						//	기본 공격력.
	WORD				m_wDP;						//	기본 방어력.

	WORD				m_wPA;						//	격투치.
	WORD				m_wSA;						//	사격치.
	WORD				m_wMA;						//	마력치.

	GLLLDATA			m_sExperience;				//	경험치. ( 현제/다음레밸도달값 )
	LONGLONG			m_lnReExp;					//  회복할수 있는 경험치

	DWORD				m_dwSkillPoint;				//	Skill Point.

	LONGLONG			m_lVNGainSysExp;			// 베트남 탐닉 방지 저장 경험치

	GLPADATA			m_sHP;						//	생명량. ( 현제/최대량 )
	GLPADATA			m_sMP;						//	정신량. ( 현제/최대량 )
	GLPADATA			m_sSP;						//	근력량.	( 현제/최대량 )
	GLPADATA			m_sCombatPoint;				/*combatpoint logic logic, Juver, 2017/05/27 */

	WORD				m_wPK;						//	총 PK 횟수.

	bool				m_bEventBuster;				//	이벤트 중인지 아닌지

	__time64_t			m_tLoginTime;				//  로긴 시간이나 이벤트 시작시간

	int					m_EventStartLv;				//  이벤트 최소 레벨
	int					m_EventEndLv;				//  이벤트 최대 레벨

	int					m_RemainEventTime;			//  이벤트 적용까지 남은 시간
	int					m_RemainBusterTime;			//  부스터 남은시간

	bool				m_bEventApply;				//  이벤트 적용대사 여부
	bool				m_bEventStart;				//  이벤트 시작했는지 아닌지, 이벤트 대상인지

	int					m_EventStartTime;			//  이벤트 적용 시작 시간
	int					m_EventBusterTime;			//  이벤트 부스터 타임


	/// 베트남 탐닉 방지 시스템 변수
	LONGLONG			m_VietnamGameTime;			//	저장된 누적 시간
	BYTE				m_dwVietnamGainType;		//  베트남 탐닉 방지 시스템 
	DWORD				m_dwVietnamInvenCount;		// 옮길수 있는 아이템의 갯수

    TCHAR				m_szPhoneNumber[SMS_RECEIVER]; // 캐릭터 폰 번호

	float				m_fScaleRange;

	/*contribution point, Juver, 2017/08/23 */
	LONGLONG			m_llContributionPoint;	

	/*activity point, Juver, 2017/08/23 */
	DWORD				m_dwActivityPoint;

	/*activity system, Juver, 2017/11/04 */
	char				m_szBadge[CHAR_SZNAME];

	/*pk info, Juver, 2017/11/16 */
	DWORD				m_dwPKScore;
	DWORD				m_dwPKDeath;

	DWORD				m_dwCWKill;
	DWORD				m_dwCWDeath;

	/*equipment lock, Juver, 2018/01/13 */
	BOOL				m_bEnableEquipmentLock;
	BOOL				m_bEquipmentLockStatus;

	/*pvp capture the flag, Juver, 2018/01/29 */
	WORD				m_wCaptureTheFlagTeam;
	bool				m_bCaptureTheFlagHoldFlag;

	//add directval
	int				m_nHP_Potion_Rate;
	int				m_nMP_Potion_Rate;
	int				m_nSP_Potion_Rate;
	/*inventory lock, DevArt22, 2020/02/11 */
	BOOL				m_bEnableInventoryLock;
	BOOL				m_bInventoryLockStatus;

	/*storage lock, DevArt22, 2020/02/11 */
	BOOL				m_bEnableStorageLock;
	BOOL				m_bStorageLockStatus;

	//itemmall
	DWORD			m_dwPPoints;
	DWORD			m_dwVPoints;
	
	SCHARDATA () 
		: m_dwUserID(0)
		, m_dwUserLvl(0)
		, m_tPREMIUM(0)
		, m_bPREMIUM(false)
		, m_tCHATBLOCK(0)
		, m_wINVENLINE(0)
		, m_dwServerID(0)
		, m_dwCharID(0)
		, m_emTribe(TRIBE_HUMAN)
		, m_emClass(GLCC_BRAWLER_M)
		, m_wSchool(0)
		, m_wSex(0)
		, m_wHair(0)
		, m_wHairColor(0)
		, m_wFace(0)
		, m_nBright(1)
		, m_nLiving(0)
		, m_wLevel(0)
		, m_lnMoney(0)
		, m_lVNGainSysMoney(0)
		, m_bMoneyUpdate(FALSE)
		, m_bStorageMoneyUpdate(FALSE)
		, m_wTempLevel(0)
		, m_lnTempMoney(0)
		, m_lnTempStorageMoney(0)
		, m_dwGuild(CLUB_NULL)
		, m_tSECEDE(0)
		, m_wStatsPoint(0)
		, m_wAP(0)
		, m_wDP(0)
		, m_wPA(0)
		, m_wSA(0)
		, m_wMA(0)
		, m_dwSkillPoint(0)
		, m_wPK(0)
		, m_EventStartTime(0)
		, m_EventBusterTime(0)
		, m_bEventBuster(FALSE)
		, m_bEventStart(FALSE)
		, m_tLoginTime(0)
		, m_EventStartLv(0)
		, m_EventEndLv(0)
		, m_bEventApply(FALSE)
		, m_RemainEventTime(0)
		, m_RemainBusterTime(0)
		, m_VietnamGameTime(0)
		, m_dwVietnamGainType(0)
		, m_lVNGainSysExp(0)
		, m_dwVietnamInvenCount(0)
		, m_lnReExp ( 0 )
		, m_fScaleRange( 1.0f )
		, m_nHP_Potion_Rate(0) //add directval
		, m_nMP_Potion_Rate(0)
		, m_nSP_Potion_Rate(0)
		
		/*contribution point, Juver, 2017/08/23 */
		, m_llContributionPoint(0) 

		/*activity point, Juver, 2017/08/23 */
		, m_dwActivityPoint(0) 

		/*pk info, Juver, 2017/11/16 */
		, m_dwPKScore(0)
		, m_dwPKDeath(0)

		, m_dwCWKill(0)
		, m_dwCWDeath(0)

		/*equipment lock, Juver, 2018/01/13 */
		, m_bEnableEquipmentLock(FALSE)
		, m_bEquipmentLockStatus(FALSE)

		/*pvp capture the flag, Juver, 2018/01/29 */
		, m_wCaptureTheFlagTeam(CAPTURE_THE_FLAG_TEAM_SIZE)
		, m_bCaptureTheFlagHoldFlag(false)

		/*inventory lock, DevArt22, 2020/02/11 */
		, m_bEnableInventoryLock(FALSE)
		, m_bInventoryLockStatus(FALSE)
		
		//itemmall
		, m_dwPPoints(0)
		, m_dwVPoints(0)
	{
		memset(m_szName, 0, sizeof(char) * CHAR_SZNAME);
		memset(m_szNick, 0, sizeof(char) * CHAR_SZNAME);
		memset(m_szPhoneNumber, 0, sizeof(TCHAR) * SMS_RECEIVER);

		/*activity system, Juver, 2017/11/05 */
		memset(m_szBadge, 0, sizeof(char) * CHAR_SZNAME);

		for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )	m_tSTORAGE[i] = 0;
		for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )	m_bSTORAGE[i] = false;
	}

	SCHARDATA &GETCHARDATA ()			{ return *this; }
	EMCHARINDEX GETCHARINDEX () const	{ return CharClassToIndex ( m_emClass ); }

	bool IsKEEP_STORAGE ( DWORD dwCHANNEL );	//	해당 번호의 락커에 물건을 맡길수 있는지 검사.
	CTime GetStorageTime (  DWORD dwCHANNEL );
	WORD GetOnINVENLINE ();
	void CalcPREMIUM ();
};
typedef SCHARDATA* PCHARDATA;

//itemmall
typedef std::vector<ITEMSHOP>			VECITEMSHOP;

typedef std::map<std::string,ITEMSHOP>		ITEMMAPSHOP;
typedef ITEMMAPSHOP::iterator					ITEMMAPSHOP_ITER;

typedef std::map<DWORD,std::string>			ITEMMAPSHOP_KEY;
typedef ITEMMAPSHOP_KEY::iterator				ITEMMAPSHOP_KEY_ITER;

typedef std::vector<SHOPPURCHASE>			VECSHOP;

typedef std::map<std::string,SHOPPURCHASE>	MAPSHOP;
typedef MAPSHOP::iterator					MAPSHOP_ITER;

typedef std::map<DWORD,std::string>			MAPSHOP_KEY;
typedef MAPSHOP_KEY::iterator				MAPSHOP_KEY_ITER;

struct SCHARDATA2 : public SCHARDATA
{
	typedef stdext::hash_map<DWORD,SCHARSKILL>		SKILL_MAP;
	typedef SKILL_MAP::iterator						SKILL_MAP_ITER;
	typedef SKILL_MAP::const_iterator				SKILL_MAP_CITER;

	char					m_szUID[USR_ID_LENGTH+1];

	SKILL_MAP				m_ExpSkills;						//	수련 스킬 속성.
	SITEMCUSTOM				m_PutOnItems[SLOT_TSIZE];			//	착용 Item.
	
	SITEMCUSTOM				m_pHoldArray[300];			//	착용 Item.

	WORD					m_wSKILLQUICK_ACT;					//	스킬 퀵슬롯중 액티브된 스킬.
	SNATIVEID				m_sSKILLQUICK[EMSKILLQUICK_SIZE];	//	스킬 퀵슬롯.
	SACTION_SLOT			m_sACTIONQUICK[EMACTIONQUICK_SIZE];	//	액션 퀵슬롯.

	GLInventory				m_cInventory;						//	인벤토리.

	BOOL					m_bServerStorage;					//	창고 유효함. ( 서버측. ) ( 새로 생성되는 캐릭터는 무조건 유효하게 생성시 체크함. - GLCHARLOGIC::INIT_DATA() )
	LONGLONG				m_lnStorageMoney;					//	창고 소지 금액.
	BOOL					m_bStorage[EMSTORAGE_CHANNEL];		//	창고 유효함. ( 클라이언트측. )
	GLInventory				m_cStorage[EMSTORAGE_CHANNEL];		//	창고.

	GLQuestPlay				m_cQuestPlay;						//	퀘스트.

	//	서버 전용.
	MAPSHOP					m_mapCharged;						//	구입한 아이템 목록.
	
	//	클라이언트 전용.
	GLInventory				m_cInvenCharged;					//	구입한 아이템 인벤.
	MAPSHOP_KEY				m_mapChargedKey;				

	//itemmall
	ITEMMAPSHOP				m_mapItemShop;
	
	GLInventory				m_cInvenItemShopVote[13];					
	ITEMMAPSHOP_KEY			m_mapItemShopVoteKey[13];					
	
	GLInventory				m_cInvenItemShopPremium[13];					
	ITEMMAPSHOP_KEY			m_mapItemShopPremiumKey[13];					

	GLInventory				m_cInvenItemShop[13];					
	ITEMMAPSHOP_KEY			m_mapItemShopKey[13];					

	//	Note : 초기 시작될 맵, 맵 위치값.
	//
	SNATIVEID			m_sStartMapID;				//	초기 시작 맵.
	DWORD				m_dwStartGate;				//	초기 시작 개이트.
	D3DXVECTOR3			m_vStartPos;				//	초기 시작 위치.

	SNATIVEID			m_sSaveMapID;				//	종료 맵.
	D3DXVECTOR3			m_vSavePos;					//	종료 위치.

	SNATIVEID			m_sLastCallMapID;			//	직전귀환 맵.
	D3DXVECTOR3			m_vLastCallPos;				//	직전귀환 위치.

	INT					m_dwThaiCCafeClass;			// 태국 사이버 카페 
	INT					m_nMyCCafeClass;			// 말레이시아 PC방 이벤트 
	SChinaTime			m_sChinaTime;				// 중국 시간별 수익
	SEventTime			m_sEventTime;				// 이벤트 시간

	SVietnamGainSystem  m_sVietnamSystem;			// 베트남 탐닉방지 시스템
	GLInventory			m_cVietnamInventory;		// 베트남 저장한 탐닉 인벤토리
	bool				m_bVietnamLevelUp;			// 베트남 경험치 획득 아이템 사용시 여러단계의 레벨을 증가시킬 수 있다.

	// 아이템 개조 정보	// ITEMREBUILD_MARK
	BOOL				m_bRebuildOpen;
	SINVEN_POS			m_sRebuildCardPos;
	SINVEN_POS			m_sRebuildItem;
	SINVEN_POS			m_sPreInventoryItem;
	LONGLONG			m_i64RebuildCost;
	LONGLONG			m_i64RebuildInput;

	//sealed card NaJDeV
	SINVEN_POS			m_sRebuildSeal; 
	WORD				m_wSealType;

	bool				m_bTracingUser;			// 현재 추적중인 유저인지 아닌지

	typedef std::map<DWORD,ITEM_COOLTIME>	COOLTIME_MAP;
	typedef COOLTIME_MAP::iterator			COOLTIME_MAP_ITER;
	typedef COOLTIME_MAP::const_iterator	COOLTIME_MAP_CITER;

	COOLTIME_MAP		m_mapCoolTimeType;		//	아이템 타입별 쿨타임
    COOLTIME_MAP		m_mapCoolTimeID;		//	아이템 MID/SID별 쿨타임

	/*itemfood system, Juver, 2017/05/24 */
	SFITEMFACT			m_sFITEMFACT[FITEMFACT_SIZE];	

	/*system buffs, Juver, 2017/09/04 */
	SSYSTEM_BUFF		m_sSYSTEM_BUFF[SYSTEM_BUFF_SIZE]; 

	/*activity system, Juver, 2017/10/23 */
	SACTIVITY_CHAR_DATA_MAP		m_mapActivityProg;
	SACTIVITY_CHAR_DATA_MAP		m_mapActivityDone;

	SCHARDATA2();
	void Assign ( SCHARDATA2 &CharData );

	EMCHARINDEX GETCHARINDEX () const { return CharClassToIndex ( m_emClass ); }

	BOOL SETEXPSKILLS_BYBUF ( CByteStream &ByteStream );
	BOOL GETEXPSKILLS_BYBUF ( CByteStream &ByteStream ) const;

	BOOL GETPUTONITEMS_BYBUF ( CByteStream &ByteStream ) const;
	
	BOOL SETSKILL_QUICKSLOT ( CByteStream &ByteStream );
	BOOL GETSKILL_QUICKSLOT ( CByteStream &ByteStream ) const;

	BOOL SETACTION_QUICKSLOT ( CByteStream &ByteStream );
	BOOL GETACTION_QUICKSLOT ( CByteStream &ByteStream ) const;


	BOOL SETINVENTORY_BYBUF ( CByteStream &ByteStream );
	BOOL GETINVENTORYE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETSTORAGE_BYBUF ( CByteStream &ByteStream );
	BOOL GETSTORAGE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETQUESTPLAY ( CByteStream &ByteStream );
	BOOL GETQUESTPLAY ( CByteStream &ByteStream ) const;

	BOOL SETSHOPPURCHASE ( VECSHOP &vecSHOP );
	BOOL SETITEMSHOP ( VECITEMSHOP &vecSHOP ); //itemmall

	// 베트남 탐닉방지 시스템 추가에 따른 캐릭터 추가 인벤토리 확장
	BOOL SETVTADDINVENTORY_BYBUF ( CByteStream &ByteStream );
	BOOL GETVTADDINVENTORYE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETITEMCOOLTIME_BYBUF( CByteStream &ByteStream ); 
	BOOL GETITEMCOOLTIME_BYBUF( CByteStream &ByteStream ) const; 

	/*itemfood system, Juver, 2017/05/25 */
	BOOL SETITEMFOOD_BYBUF( CByteStream &ByteStream ); 
	BOOL GETITEMFOOD_BYBUF( CByteStream &ByteStream ) const; 

	/*activity system, Juver, 2017/10/23 */
	BOOL SETACTIVITY_BYBUF( CByteStream &ByteStream ); 
	BOOL GETACTIVITY_BYBUF( CByteStream &ByteStream ) const; 

public:
	BOOL LOADFILE ( const char* szFileName );

public:
	//	클라이언트 전용.
	BOOL ADDSHOPPURCHASE ( const char* szPurKey, SNATIVEID nidITEM );
	BOOL DELSHOPPURCHASE ( const DWORD dwID );
	//itemmall
	BOOL ADDITEMSHOP ( const char* szPurKey,SNATIVEID nidITEM , const WORD wPrice , const WORD wStock ,WORD wItemCtg , WORD wCurrency );

public:	// ITEMREBUILD_MARK
	const SITEMCUSTOM& GET_REBUILD_ITEM();
	const SITEMCUSTOM& GET_PREHOLD_ITEM();	// ??? ???? ???

public: //sealed card 
	const SITEMCUSTOM& GET_REBUILD_SEAL(); //sealed card
	const WORD GET_SEAL_TYE() { return m_wSealType; }


public:	// ITEMREBUILD_MARK
	VOID InitRebuildData();
	VOID OpenRebuild()							{ m_bRebuildOpen = TRUE; }
	VOID CloseRebuild()							{ m_bRebuildOpen = FALSE; }
	const BOOL ValidRebuildOpen()				{ return m_bRebuildOpen; }
	const LONGLONG GetRebuildCost()				{ return m_i64RebuildCost; }
	const LONGLONG GetRebuildInput()			{ return m_i64RebuildInput; }

private:
	SCHARDATA2(const SCHARDATA2 &Inven )		{ GASSERT(0&&"묵시적 복사 불허!"); }
	SCHARDATA2& operator= ( SCHARDATA2 &Inven )	{ GASSERT(0&&"묵시적 복사 불허!"); return *this; }
};
typedef SCHARDATA2* PCHARDATA2;


struct SSTATEBLOW
{
	EMSTATE_BLOW	emBLOW;			//	상태 이상 종류.
	float			fAGE;			//	적용 시간.
	float			fSTATE_VAR1;	//	적용 값 1.
	float			fSTATE_VAR2;	//	적용 값 2.

	SSTATEBLOW () :
		emBLOW(EMBLOW_NONE),
		fAGE(0),
		fSTATE_VAR1(0),
		fSTATE_VAR2(0)
	{

	}
};

struct SLANDEFFECT
{
	D3DXVECTOR2			vMinPos;
	D3DXVECTOR2			vMaxPos;
	EMLANDEFFECT_TYPE	emLandEffectType;
	float				fValue;

	SLANDEFFECT() :
		vMinPos( 0.0f, 0.0f ),
		vMaxPos( 0.0f, 0.0f ),
		emLandEffectType(EMLANDEFFECT_ATK_SPEED),
		fValue( 0.0f )
	{
	}

	void Init()
	{
		vMinPos			 = D3DXVECTOR2( 0.0f, 0.0f );
		vMaxPos			 = D3DXVECTOR2( 0.0f, 0.0f );
		emLandEffectType = EMLANDEFFECT_ATK_SPEED;
		fValue			 = 0.0f;
	}

	bool IsUse()
	{
		if( vMinPos		 	 == D3DXVECTOR2( 0.0f, 0.0f ) &&
			vMaxPos			 == D3DXVECTOR2( 0.0f, 0.0f ) &&
			emLandEffectType == EMLANDEFFECT_ATK_SPEED &&
			fValue			 == 0.0f ) return FALSE;
		return TRUE;
	}

	bool operator == ( const SLANDEFFECT &value )
	{
		if( vMaxPos			 != value.vMaxPos )			 return FALSE;
		if( vMinPos			 != value.vMinPos )			 return FALSE;
		if( emLandEffectType != value.emLandEffectType ) return FALSE;
		if( fValue		     != value.fValue )			 return FALSE;

		return TRUE;
	}

};

typedef std::vector<SLANDEFFECT> VEC_LANDEFF;
typedef VEC_LANDEFF::iterator	 VEC_LANDEFF_ITER;

struct SPASSIVE_SKILL_DATA
{
	short	m_nHP;
	short	m_nMP;
	short	m_nSP;

	short	m_nDAMAGE;
	short	m_nDEFENSE;

	short	m_nHIT;
	short	m_nAVOID;

	short	m_nPIERCE;
	float	m_fTARRANGE;

	float	m_fMOVEVELO;
	float	m_fATTVELO;
	float	m_fSKILLDELAY;

	float	m_fINCR_HP;
	float	m_fINCR_MP;
	float	m_fINCR_SP;

	float	m_fDAMAGE_RATE;
	float	m_fDEFENSE_RATE;

	DAMAGE_SPEC m_sDamageSpec;

	short	m_nPA;
	short	m_nSA;
	short	m_nMA;

	float	m_fHP_RATE;
	float	m_fMP_RATE;
	float	m_fSP_RATE;

	SRESIST	m_sSUMRESIST;					//	저항값.

	/*skill range spec logic, Juver, 2017/06/06 */
	float	m_fSUM_SKILL_ATTACKRANGE;
	float	m_fSUM_SKILL_APPLYRANGE;

	/*summon time, Juver, 2017/12/12 */
	int		m_nSummonTime;

	SPASSIVE_SKILL_DATA () :
		m_nHP(0),
		m_nMP(0),
		m_nSP(0),

		m_fINCR_HP(0),
		m_fINCR_MP(0),
		m_fINCR_SP(0),

		m_nDAMAGE(0),
		m_nDEFENSE(0),

		m_nHIT(0),
		m_nAVOID(0),

		m_nPIERCE(0),
		m_fTARRANGE(0),

		m_fMOVEVELO(0),
		m_fATTVELO(0),
		m_fSKILLDELAY(0),

		m_fDAMAGE_RATE(0),
		m_fDEFENSE_RATE(0),

		m_nPA(0),
		m_nSA(0),
		m_nMA(0),

		m_fHP_RATE(0),
		m_fMP_RATE(0),
		m_fSP_RATE(0),

		/*skill range spec logic, Juver, 2017/06/06 */
		m_fSUM_SKILL_ATTACKRANGE(0.0f),
		m_fSUM_SKILL_APPLYRANGE(0.0f),

		/*summon time, Juver, 2017/12/12 */
		m_nSummonTime(0)
	{
	}
};

struct SPKCOMBO_COUNT
{
	bool		bShow;
	int			nCount;
	float		fTime;

	SPKCOMBO_COUNT() :
	bShow(FALSE),
		nCount(0),
		fTime(0)
	{
	}

	bool IsACTIVE()
	{
		return bShow != FALSE;
	}

	void RESET()
	{
		bShow = FALSE;
		nCount = 0;
		fTime = 0;
	};
};
struct SEventState
{
	float				fItemGainRate;		// 이벤트 시 얻게되는 아이템 드랍율
	float				fExpGainRate;		// 이벤트 시 얻게되는 경헙치 배율
	int					MinEventLevel;		// 이벤트 최소 레벨
	int					MaxEventLevel;		// 이벤트 최대 레벨
	bool				bEventStart;		// 이벤트가 시작됐는지 아닌지
	int					EventPlayTime;		// 이벤트가 적용 되는 플레이 시간
	int					EventBusterTime;	// 이벤트가 지속되는 시간
	CTime				EventStartTime;		// 이벤트가 최초 시작한 시간
	DWORD				dwEventEndMinute;	// 이벤트 적용 시간


	SEventState()
	{
		Init();
	}
	void Init()
	{
		fItemGainRate    = 1.0f;
		fExpGainRate     = 1.0f;
		MinEventLevel    = 0;		// 이벤트 최소 레벨
		MaxEventLevel    = 0;		// 이벤트 최대 레벨
		bEventStart      = FALSE;			// 이벤트가 시작됐는지 아닌지
		EventPlayTime    = 0;		// 이벤트가 시작 시간
		EventBusterTime  = 0;		// 이벤트가 지속되는 시간
		EventStartTime   = 0;
		dwEventEndMinute = 0;

	}
};

struct SEVENT_FACT
{
	INT				nType;
	WORD			wSpeed;
	WORD			wASpeed;
	WORD			wAttack;

	SEVENT_FACT () 
		: nType(EMGM_EVENT_NONE)
		, wSpeed(0)
		, wASpeed(0)
		, wAttack(0)
	{
	}

	bool IsACTIVE( EMGM_EVENT_TYPE emType )
	{
		return (nType&emType)!=EMGM_EVENT_NONE;
	}

	void SetEVENT( EMGM_EVENT_TYPE emType, WORD wValue )
	{
		nType |= emType;

		switch( emType )
		{
		case EMGM_EVENT_SPEED:	wSpeed=wValue;	break;
		case EMGM_EVENT_ASPEED:	wASpeed=wValue;	break;
		case EMGM_EVENT_ATTACK:	wAttack=wValue;	break;
		}
	}

	void ResetEVENT( EMGM_EVENT_TYPE emType )
	{
		nType &= ~emType;
	}
};

struct SDROP_STATEBLOW
{
	EMSTATE_BLOW	emBLOW;			//	상태 이상 종류.
	float			fAGE;			//	적용 시간.
	float			fSTATE_VAR1;	//	적용 값 1.
	float			fSTATE_VAR2;	//	적용 값 2.

	SDROP_STATEBLOW () 
		: emBLOW(EMBLOW_NONE)
		, fAGE(0)
		, fSTATE_VAR1(0)
		, fSTATE_VAR2(0)
	{
	}

	SDROP_STATEBLOW& operator= ( SSTATEBLOW &sblow )
	{
		emBLOW = sblow.emBLOW;
		fAGE = sblow.fAGE;
		fSTATE_VAR1 = sblow.fSTATE_VAR1;
		fSTATE_VAR2 = sblow.fSTATE_VAR2;

		return *this;
	}
};


struct SDROP_CHAR
{
	enum
	{
		CHAR_GEN	= 0x001,
		CLUB_CD		= 0x002
	};
	DWORD			dwUserLvl;				//	사용자 권한 Level.
	char			szName[CHAR_SZNAME];		//	이름.
	EMTRIBE			emTribe;					//	종족.
	EMCHARCLASS		emClass;					//	직업.
	WORD			wSchool;					//	학원.
	WORD			wHair;						//	머리카락.
	WORD			wHairColor;					//  머리카락 컬러
	WORD			wFace;						//	얼굴모양.
	WORD			wSex;						//  성별

	int				nBright;					//	속성.

	DWORD			dwCharID;					//	케릭터ID.
	WORD			wLevel;						//	레벨.
	DWORD			dwGuild;					//	길드 번호.
	DWORD			dwAlliance;					//	동맹 번호
	char			szClubName[CHAR_SZNAME];	//	클럽 이름.
	DWORD			dwGuildMarkVer;				//	길드 마크 번호.
	DWORD			dwGuildMaster;				//	길드 마스터.
	char			szNick[CHAR_SZNAME];		//	별명.
	
	DWORD			dwParty;					//	파티 번호.
	DWORD			dwPMasterID;				//	파티 마스터 ID.

	GLPADATA		sHP;						//	생명량. ( 현제/최대량 )

	DWORD			dwGaeaID;					//	생성 메모리 인덱스용.
	SNATIVEID		sMapID;						//	생성 맵 ID.
	DWORD			dwCeID;						//	셀 ID.
	D3DXVECTOR3		vPos;						//	위치.
	D3DXVECTOR3		vDir;						//	위치.

	EMACTIONTYPE	Action;						//	현제 액션.
	DWORD			dwActState;					//	현제 액션 플래그.
	D3DXVECTOR3		vTarPos;					//	현제 목표 위치.

	/*skill summon, Juver, 2017/10/09 */
	DWORD			dwSummonGUID[SKILL_SUMMON_MAX_CLIENT_NUM];				//  소환수 ID

	SDROP_SKILLFACT	sSKILLFACT[SKILLFACT_SIZE];

	 /*itemfood system, Juver, 2017/05/24 */
	SDROP_FITEMFACT sFITEMFACT[FITEMFACT_SIZE];

	/*system buffs, Juver, 2017/09/04 */
	SDROP_SYSTEM_BUFF sSYSTEM_BUFF[SYSTEM_BUFF_SIZE]; 

	SDROP_STATEBLOW	sSTATEBLOWS[EMBLOW_MULTI];
	int				nLandEffect[EMLANDEFFECT_MULTI];	//  지형 이상 효과들

	SQITEM_FACT		sQITEMFACT;
	SEVENT_FACT		sEVENTFACT;
	SEventState		sEventState;
	SPKCOMBO_COUNT	sPKCOMBOCOUNT;
	
	SITEMCLIENT		m_PutOnItems[SLOT_NSIZE_S_2];	//	착용 Item.
	BOOL			m_bVehicle;
	CLIENT_VEHICLE	m_sVehicle;

	bool			m_bItemShopOpen;		// ItemShopOpen

	BOOL			m_bUseArmSub;				// 극강부를 위한 보조 무기 사용여부

	DWORD			dwFLAGS;					//	기타 속성.

	SPASSIVE_SKILL_DATA	sPASSIVE_SKILL;			//	passive skill data.
	
	DWORD			m_dwANISUBTYPE;				//	애니메이션 서브타입

	float			m_fScaleRange;

	bool			m_bBoosterStart; /*vehicle booster system, Juver, 2017/08/12 */

	/*activity system, Juver, 2017/11/04 */
	char			szBadge[CHAR_SZNAME];

	/*hide gm info, Juver, 2017/11/26 */
	DWORD			dwUserID;
	DWORD			dwUserLevel;

	/*pvp capture the flag, Juver, 2018/01/29 */
	WORD			m_wCaptureTheFlagTeam;
	bool			m_bCaptureTheFlagHoldFlag;
	BOOL			m_bPassenger;

	SDROP_CHAR () 
		: emTribe(TRIBE_HUMAN)
		, dwUserLvl(0)
		, emClass(GLCC_BRAWLER_M)
		, wSchool(0)
		, m_bPassenger(FALSE)
		, wHairColor(0)
		, wSex(0)
		, wHair(0)
		, wFace(0)
		, nBright(0)
		, dwCharID(0)
		, wLevel(1)
		, dwGuild(CLUB_NULL)
		, dwGuildMarkVer(0)
		, dwGuildMaster(0)
		, dwAlliance(0)
		, dwParty(PARTY_NULL)
		, dwPMasterID(GAEAID_NULL)
		, dwGaeaID(0)
		, dwCeID(0)
		, vPos(0,0,0)
		, vDir(0,0,-1)
		, Action(GLAT_IDLE)
		, dwActState(NULL)
		, vTarPos(0,0,0)
		, dwFLAGS(NULL)
		, m_bUseArmSub(FALSE)
		, m_bVehicle ( FALSE )
		, m_bItemShopOpen( false )
		//, dwSummonGUID(GAEAID_NULL)
		, m_dwANISUBTYPE( 0 )
		, m_fScaleRange( 1.0f )
		, m_bBoosterStart(false) /*vehicle booster system, Juver, 2017/08/12 */

		/*hide gm info, Juver, 2017/11/26 */
		, dwUserID(0)
		, dwUserLevel(0)

		/*pvp capture the flag, Juver, 2018/01/29 */
		, m_wCaptureTheFlagTeam(CAPTURE_THE_FLAG_TEAM_SIZE)
		, m_bCaptureTheFlagHoldFlag(false)
	{
		for( int i=0; i < EMLANDEFFECT_MULTI; i++)
		{
			nLandEffect[i] = -1;
		}
		memset(szName, 0, sizeof(char) * CHAR_SZNAME);
		memset(szNick, 0, sizeof(char) * CHAR_SZNAME);
		memset(szClubName, 0, sizeof(char) * CHAR_SZNAME);

		/*activity system, Juver, 2017/11/05 */
		memset(szBadge, 0, sizeof(char) * CHAR_SZNAME);

		/*skill summon, Juver, 2017/10/09 */
		for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
			dwSummonGUID[i] = GAEAID_NULL;
	}
};


struct SINFO_CHAR
{
	DWORD			dwCharID;
	DWORD			dwGaeaID;
	char			szName[CHAR_SZNAME];
	EMCHARCLASS		emClass;
	WORD			wSchool;
	WORD			wHair;
	WORD			wHairColor;
	WORD			wFace;
	WORD			wLevel;

	char			szClubName[CHAR_SZNAME];

	GLPADATA		sdDAMAGE;
	WORD			wHP;
	WORD			wMP;
	WORD			wSP;

	SITEMCUSTOM		sPutOnItems[SLOT_NSIZE_S_2];

	SCHARSTATS		sSUMSTATS;

	WORD		wSUM_PA;
	WORD		wSUM_SA;
	WORD		wSUM_MA;

	SRESIST		sSUMRESIST;

	int			nSUM_DEF;
	int			nSUM_HIT;
	int			nSUM_AVOID;
	bool		bUseArmSub;
	bool		bPrivate;

	/*activity system, Juver, 2017/11/04 */
	char			szBadge[CHAR_SZNAME];

	float		fScaleRange;

	SINFO_CHAR () 
		: emClass(GLCC_BRAWLER_M)
		, wSchool(0)
		, wHairColor(0)
		, wHair(0)
		, wFace(0)
		, dwGaeaID(GAEAID_NULL)
		, dwCharID(GAEAID_NULL)
		, wLevel(1)
		, wSUM_PA( 0 )
		, wSUM_SA( 0 )
		, wSUM_MA( 0 )
		, nSUM_DEF( 0 )
		, nSUM_HIT( 0 )
		, nSUM_AVOID( 0 )
		, bUseArmSub( FALSE )
		, bPrivate(false)
		, fScaleRange(1.0f)
	{
		memset(szName, 0, sizeof(char) * CHAR_SZNAME);
		memset(szClubName, 0, sizeof(char) * CHAR_SZNAME);
		StringCchCopy ( szClubName, CHAR_SZNAME, "No Guild" );

		/*activity system, Juver, 2017/11/05 */
		memset(szBadge, 0, sizeof(char) * CHAR_SZNAME);
		StringCchCopy ( szBadge, CHAR_SZNAME, "No Badge" );
	}
};

#endif // GLCHARDATA_H_