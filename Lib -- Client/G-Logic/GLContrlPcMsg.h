#pragma once

#include "GLContrlBaseMsg.h"
#include "GLVEHICLE.h"
#include "GLContrlPcMsgEnum.h"

/////////////////////////////////////////////////////////////////////////////
// LG-7 DLLCaller
#include "../LG7DLL.h"
/////////////////////////////////////////////////////////////////////////////

enum EMBUS_TAKE
{
	EMBUS_TAKE_FAIL			= 0,	//	일반오류.
	EMBUS_TAKE_OK			= 1,	//	탑승 성공.
	EMBUS_TAKE_TICKET		= 2,	//	티켓 부족.
	EMBUS_TAKE_CONDITION	= 3,	//	탑승 여건이 안됨.
	EMBUS_TAKE_PK_LEVEL		= 4,	//	pk 성향이 낮아서 탑승 불가능.
};

enum EMTAXI_TAKE
{
	EMTAXI_TAKE_FAIL		= 0,	//	일반오류.
	EMTAXI_TAKE_OK			= 1,	//	탑승 성공.
	EMTAXI_TAKE_TICKET		= 2,	//	티켓 부족.
	EMTAXI_TAKE_MONEY		= 3,	//	금액 부족.
	EMTAXI_TAKE_CONDITION	= 4,	//	탑승 여건이 안됨.
	EMTAXI_TAKE_MAPFAIL		= 5,	//  맵 오류
	EMTAXI_TAKE_STATIONFAIL	= 6,	//  정류장 오류
	EMTAXI_TAKE_NPCFAIL		= 7,	//	NPC 설정 오류
};

enum EMREQ_FIRECRACKER_FB
{
	EMREQ_FIRECRACKER_FB_FAIL	= 0,	//	일반 오류.
	EMREQ_FIRECRACKER_FB_OK		= 1,	//	사용 성공.
	EMREQ_FIRECRACKER_FB_NOITEM	= 2,	//	아이템 오류.
};
//itemmall
enum EMREQ_RETRIEVE_POINTS_FB
{
	EMREQ_RETRIEVE_POINTS_FAIL = 0,
	EMREQ_RETRIEVE_POINTS_OK = 1,
};

enum EMREQ_REVIVE_FB
{
	EMREQ_REVIVE_FB_FAIL		= 0,	//	일반오류.
	EMREQ_REVIVE_FB_OK			= 1,	//	사용성공.
	EMREQ_REVIVE_FB_NOITEM		= 2,	//	아이템이 없음.
	EMREQ_REVIVE_FB_NOTUSE		= 3,	//	사용 불가능한 지역.
	EMREQ_REVIVE_FB_COOLTIME	= 4,	//	아이템 쿨타임
};

enum EMREQ_RECOVERY_FB
{
	EMREQ_RECOVERY_FB_FAIL		= 0,	//	일반오류.
	EMREQ_RECOVERY_FB_OK		= 1,	//	사용성공.
	EMREQ_RECOVERY_FB_NOMONEY	= 2,	//	돈이 부족
	EMREQ_RECOVERY_FB_NOREEXP	= 3,	//	회복할 경험치가 없음
	EMREQ_RECOVERY_FB_NOTUSE	= 4,	//	사용 불가능한 지역.
};

enum EMREQ_RECOVERY_NPC_FB
{
	EMREQ_RECOVERY_NPC_FB_FAIL		= 0,	//	일반오류.
	EMREQ_RECOVERY_NPC_FB_OK		= 1,	//	사용성공.
	EMREQ_RECOVERY_NPC_FB_NOMONEY	= 2,	//	돈이 부족
	EMREQ_RECOVERY_NPC_FB_NOREEXP	= 3,	//	회복할 경험치가 없음
	EMREQ_RECOVERY_NPC_FB_NOTUSE	= 4,	//	사용 불가능한 지역.
};

enum EMINVEN_HAIR_CHANGE_FB
{
	EMINVEN_HAIR_CHANGE_FB_FAIL		= 0,	//	일반오류.
	EMINVEN_HAIR_CHANGE_FB_OK		= 1,	//	성공.
	EMINVEN_HAIR_CHANGE_FB_NOITEM	= 2,	//	아이템이 없음.
	EMINVEN_HAIR_CHANGE_FB_BADITEM	= 3,	//	사용 불가능 아이템.
	EMINVEN_HAIR_CHANGE_FB_BADCLASS	= 4,	//	케릭터 직업이 틀려서 사용 불가능 아이템.
	EMINVEN_HAIR_CHANGE_FB_COOLTIME = 5,	//	쿨타임 실패~
};

enum EMINVEN_FACE_CHANGE_FB
{
	EMINVEN_FACE_CHANGE_FB_FAIL		= 0,	//	일반오류.
	EMINVEN_FACE_CHANGE_FB_OK		= 1,	//	성공.
	EMINVEN_FACE_CHANGE_FB_NOITEM	= 2,	//	아이템이 없음.
	EMINVEN_FACE_CHANGE_FB_BADITEM	= 3,	//	사용 불가능 아이템.
	EMINVEN_FACE_CHANGE_FB_BADCLASS	= 4,	//	케릭터 직업이 틀려서 사용 불가능 아이템.
	EMINVEN_FACE_CHANGE_FB_COOLTIME	= 5,	//	쿨타임 실패
};

enum EMINVEN_GENDER_CHANGE_FB
{
	EMINVEN_GENDER_CHANGE_FB_FAIL		= 0,	//	일반오류.
	EMINVEN_GENDER_CHANGE_FB_OK			= 1,	//	성공.
	EMINVEN_GENDER_CHANGE_FB_NOITEM		= 2,	//	아이템이 없음.
	EMINVEN_GENDER_CHANGE_FB_ITEMTYPE	= 3,	//	사용 불가능 아이템.
	EMINVEN_GENDER_CHANGE_FB_BADCLASS	= 4,	//	케릭터 직업이 틀려서 사용 불가능 아이템.
	EMINVEN_GENDER_CHANGE_FB_NOTVALUE	= 5,	//  잘못된 데이터 
};

enum EMINVEN_RENAME_FB
{
	EMINVEN_RENAME_FB_FAIL		= 0,	//	일반오류.
	EMINVEN_RENAME_FB_OK		= 1,	//	성공.
	EMINVEN_RENAME_FB_NOITEM	= 2,	//	아이템이 없음.
	EMINVEN_RENAME_FB_BADITEM	= 3,	//	사용 불가능 아이템.
	EMINVEN_RENAME_FB_LENGTH	= 4,	//	이름의 길이가 4자 미만
	EMINVEN_RENAME_FROM_DB_OK	= 5,	//	성공.
	EMINVEN_RENAME_FROM_DB_FAIL	= 6,	//	실패.(이름 중복)
	EMINVEN_RENAME_FROM_DB_BAD	= 7,	//	실패.(이름 불가)
	EMINVEN_RENAME_FB_THAICHAR_ERROR = 8,	// 태국어 문자 조합 에러
	EMINVEN_RENAME_FB_VNCHAR_ERROR = 9,  // 베트남 문자 조합 에러
};

enum EMSMS_PHONE_NUMBER_FB
{
	EMSMS_PHONE_NUMBER_FB_FAIL		= 0,	//	일반오류.
	EMSMS_PHONE_NUMBER_FB_OK		= 1,	//	성공.
	EMSMS_PHONE_NUMBER_FROM_DB_OK	= 2,	//	성공.
	EMSMS_PHONE_NUMBER_FROM_DB_FAIL	= 3,	//	실패.(이름 중복)
};

enum EMSMS_SEND_FB
{
	EMSMS_SEND_FB_FAIL		= 0,	//	일반오류.
	EMSMS_SEND_FB_OK		= 1,	//	성공.
	EMSMS_SEND_FB_NOITEM	= 2,	//	아이템이 없음.
	EMSMS_SEND_FB_BADITEM	= 3,	//	사용 불가능 아이템.
	EMSMS_SEND_FROM_DB_OK	= 4,	//	성공.
	EMSMS_SEND_FROM_DB_FAIL	= 5,	//	실패.
};

enum EMMGAME_ODDEVEN
{
	EMMGAME_ODDEVEN_OK			= 0,	// 배팅 확인
	EMMGAME_ODDEVEN_CANCEL		= 1,	// 취소
	EMMGAME_ODDEVEN_SELECT		= 2,	// 홀,짝
	EMMGAME_ODDEVEN_AGAIN_OK	= 3,	// 반복 확인
	EMMGAME_ODDEVEN_SHUFFLE		= 4,
	EMMGAME_ODDEVEN_FINISH		= 5,
};

enum EMMGAME_ODDEVEN_FB
{
	EMMGAME_ODDEVEN_FB_OK			= 0,	// 배팅 완료 게임 시작
	EMMGAME_ODDEVEN_FB_FAIL			= 1,	// 게임 취소
	EMMGAME_ODDEVEN_FB_MONEY_FAIL	= 2,	// 소유금액 부족
	EMMGAME_ODDEVEN_FB_MAXBATTING	= 3,	// 최대 배팅가능 금액 초과
	EMMGAME_ODDEVEN_FB_MAXROUND		= 4,	// 최대 반복 횟수 도달
	EMMGAME_ODDEVEN_FB_WIN			= 5,	// 
	EMMGAME_ODDEVEN_FB_LOSE			= 6,	// 
	EMMGAME_ODDEVEN_FB_AGAIN_OK		= 7,	// 반복 허락
	EMMGAME_ODDEVEN_FB_FINISH_OK	= 8,
};

enum EMMGAME_ODDEVEN_CASE
{
	EMMGAME_ODDEVEN_EVEN			= 0,	// 짝
	EMMGAME_ODDEVEN_ODD				= 1,	// 홀
};

enum EM2FRIEND_FB
{
	EM2FRIEND_FB_FAIL				= 0,	//	일반 오류.
	EM2FRIEND_FB_OK					= 1,	//	성공.
	EM2FRIEND_FB_NO_ITEM			= 2,	//	아이템이 없음.
	EM2FRIEND_FB_FRIEND_CONDITION	= 3,	//	친구의 상태 이상.
	EM2FRIEND_FB_MY_CONDITION		= 4,	//	자신의 상태 이상.
	EM2FRIEND_FB_PK_CONDITION		= 5,	//	자신의 상태 이상.
	EM2FRIEND_FB_MAP_CONDITION		= 6,	//	맵 진입 조건.
	EM2FRIEND_FB_FRIEND_CHANNEL		= 7,	//	친구와 채널이 틀림.
	EM2FRIEND_FB_IMMOVABLE			= 8,	//  진입불가 ( 이벤트 지역 )
	EM2FRIEND_FB_FRIEND_BLOCK		= 9,	//	친구가 나를 차단.
	EM2FRIEND_FB_FRIEND_OFF			= 10,	//	친구가 나를 삭제.
};

enum EMGM_MOVE2CHAR_FB
{
	EMGM_MOVE2CHAR_FB_FAIL				= 0,	//	일반 오류.
	EMGM_MOVE2CHAR_FB_OK				= 1,	//	성공.
	EMGM_MOVE2CHAR_FB_TO_CONDITION		= 2,	//	상대의 상태 이상.
	EMGM_MOVE2CHAR_FB_MY_CONDITION		= 3,	//	자신의 상태 이상.
	EMGM_MOVE2CHAR_FB_CHANNEL			= 4,	//	채널이 틀림.
};

enum EMVEHICLE_SET_FB
{

	EMVEHICLE_SET_FB_FAIL			= 0,	// 일반오류
	EMVEHICLE_SET_FB_OK				= 1,	// 성공
	EMVEHICLE_SET_FB_NOTENOUGH_OIL	= 2,	// 기름없음
	EMVEHICLE_SET_FB_MAP_FAIL		= 3,	// 비활성화 맵
	EMVEHICLE_SET_FB_NO_ITEM		= 4,	// 탈것 아이템 찾을수 없음
	EMVEHICLE_SET_FB_RESET			= 5,	// 이미 타거나 내렸을때...
};

enum EMVEHICLE_GET_FB
{
	EMVEHICLE_GET_FB_OK			   = 0,		// 성공
	EMVEHICLE_GET_FB_FAIL		   = 1,		// 일반적인 오류
	EMVEHICLE_GET_FB_INVALIDITEM   = 2,		// 아이템이 정보가 다름
	EMVEHICLE_GET_FB_NOITEM		   = 3,		// 아이템이 없음
	EMVEHICLE_GET_FB_NODATA		   = 4,		// DB에 정보 없음
};

enum EMVEHICLE_REQ_SLOT_EX_HOLD_FB
{
	EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL		  = 0,		// 일반오류.
	EMVEHICLE_REQ_SLOT_EX_HOLD_FB_OK		  = 1,		// 아이템 교체 성공
	EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH	  = 2,		// 타입이 맞지 않는 아이템
	EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM = 3,		// 탈것용이 아니다.
};


enum EMVEHICLE_REQ_GIVE_BATTERY_FB
{
	EMVEHICLE_REQ_GIVE_BATTERY_FB_OK				= 0,	// 성공
	EMVEHICLE_REQ_GIVE_BATTERY_FB_FAIL				= 1,	// 일반오류
	EMVEHICLE_REQ_GIVE_BATTERY_FB_INVALIDBATTERY	= 2,	// 배터리 아님
	EMVEHICLE_REQ_GIVE_BATTERY_FB_INVALIDITEM		= 3,	// 탈것 아님
	EMVEHICLE_REQ_GET_BATTERY_FROMDB_OK				= 4,	// 디비에서 저장
	EMVEHICLE_REQ_GET_BATTERY_FROMDB_FAIL			= 5,	// 실패
};

enum EMREQ_ATTEND_FB
{
	EMREQ_ATTEND_FB_OK			= 0, // 성공
	EMREQ_ATTEND_FB_FAIL		= 1, // 실패
	EMREQ_ATTEND_FB_ALREADY		= 2, // 실패( 이미 했음 ) 
	EMREQ_ATTEND_FB_ATTENTIME	= 3, //	접속시간 체크
};

enum EMREQ_GATHER_FB
{
	EMREQ_GATHER_FB_OK			= 0,	//	채집요청 성공
	EMREQ_GATHER_FB_FAIL		= 1,	//	채집요청 실패
	EMREQ_GATHER_FB_DISTANCE	= 2,	//	채집 거리 
	EMREQ_GATHER_FB_NOTTYPE		= 3,	//	Crow Type
	EMREQ_GATHER_FB_NOCROW		= 4,	//	Crow 없음
	EMREQ_GATHER_FB_USE			= 5,	//	다른사람이 사용중
	EMREQ_GATHER_FB_GATHERING	= 6,	//	이미 채집중임
};

enum EMREQ_GATHER_RESULT
{
	EMREQ_GATHER_RESULT_SUCCESS		= 0,	// 채집 성공
	EMREQ_GATHER_RESULT_SUCCESS_EX	= 1,	// 성공이나 아이템 루팅 실패
	EMREQ_GATHER_RESULT_FAIL		= 2,	// 채집 실패
	EMREQ_GATHER_RESULT_ITEMFAIL	= 3,	// 아이템 획득 실패
};

/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
enum
{
	EM_GLOBAL_RANKING_FB_NONE			= 0,
	EM_GLOBAL_RANKING_FB_DELAY			= 1,
	EM_GLOBAL_RANKING_FB_TOP_KILL		= 2,
	EM_GLOBAL_RANKING_FB_TOP_BRAWLER	= 3,
	EM_GLOBAL_RANKING_FB_TOP_SWORD		= 4,
	EM_GLOBAL_RANKING_FB_TOP_ARCHER		= 5,
	EM_GLOBAL_RANKING_FB_TOP_SHAMAN		= 6,
	EM_GLOBAL_RANKING_FB_TOP_EXTREME	= 7,
	EM_GLOBAL_RANKING_FB_TOP_GUNNER		= 8,
	EM_GLOBAL_RANKING_FB_TOP_ASSASSIN	= 9,
	EM_GLOBAL_RANKING_FB_TOP_MAGICIAN	= 10,
	EM_GLOBAL_RANKING_FB_TOP_SHAPER		= 11,
	EM_GLOBAL_RANKING_FB_TOP_RICH		= 12,
	EM_GLOBAL_RANKING_FB_TOP_GUILD		= 13,
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// LG-7 CharView
enum
{
	EM_CHAR_VIEW_FB_NONE	= 0,
	EM_CHAR_VIEW_FB_DELAY	= 1,
	EM_CHAR_VIEW_FB_OFFLINE	= 2,
	EM_CHAR_VIEW_FB_OK		= 3,
};
/////////////////////////////////////////////////////////////////////////////

/*vehicle booster system, Juver, 2017/08/10 */
enum EMVEHICLE_REQ_ENABLE_BOOSTER_FB
{
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_OK				= 0,
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL			= 1,
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD	= 2,
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM	= 3,
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE	= 4,
	EMVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED	= 5,
};

namespace GLMSG
{
	#pragma pack(1)

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	struct SNETPC_REQ_GLOBAL_RANKING
	{
		NET_MSG_GENERIC	nmg;

		SNETPC_REQ_GLOBAL_RANKING()
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_FB
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		float					fDelay;

		SNETPC_REQ_GLOBAL_RANKING_FB()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, fDelay(0.0f)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_FB;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_KILL_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL	sTopKill;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_KILL_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_KILL_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL			sTopKill_[10];

		SNETPC_REQ_GLOBAL_RANKING_KILL_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_BR_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_BR	sTopKillBr;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_BR_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_BR_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_BR			sTopKill_Br[10];

		SNETPC_REQ_GLOBAL_RANKING_BR_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SW_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_SW	sTopKillSw;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_SW_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SW_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_SW			sTopKill_Sw[10];

		SNETPC_REQ_GLOBAL_RANKING_SW_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_AR_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_AR	sTopKillAr;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_AR_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_AR_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_AR			sTopKill_Ar[10];

		SNETPC_REQ_GLOBAL_RANKING_AR_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SH_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_SH	sTopKillSh;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_SH_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SH_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_SH			sTopKill_Sh[10];

		SNETPC_REQ_GLOBAL_RANKING_SH_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_EX_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_EX	sTopKillEx;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_EX_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_EX_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_EX			sTopKill_Ex[10];

		SNETPC_REQ_GLOBAL_RANKING_EX_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SC_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_SC	sTopKillSc;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_SC_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_SC_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_SC			sTopKill_Sc[10];

		SNETPC_REQ_GLOBAL_RANKING_SC_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_AS_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_KILL_AS	sTopKillAs;
		BOOL			bWIN;

		SNETPC_REQ_GLOBAL_RANKING_AS_UPDATE()
			: bWIN(FALSE)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_AS_AGT
	{
		NET_MSG_GENERIC			nmg;
		int						emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_KILL_AS			sTopKill_As[10];

		SNETPC_REQ_GLOBAL_RANKING_AS_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_RICH_UPDATE
	{
		NET_MSG_GENERIC	nmg;
		STOP_RANK_RICH	sTopRich;

		SNETPC_REQ_GLOBAL_RANKING_RICH_UPDATE()
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_UPDATE;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_RICH_AGT
	{
		NET_MSG_GENERIC			nmg;
		WORD					emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_RICH			sTopRich_[10];

		SNETPC_REQ_GLOBAL_RANKING_RICH_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_AGT;
		}
	};

	struct SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT
	{
		NET_MSG_GENERIC			nmg;
		WORD					emFB;
		WORD					wPart;
		WORD					wSize;
		STOP_RANK_GUILD			sTopGuild_[10];

		SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT()
			: emFB(EM_GLOBAL_RANKING_FB_NONE)
			, wPart(0)
			, wSize(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_GLOBAL_RANKING_GUILD_AGT;
		}
	};
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	struct SNETPC_REQ_CHAR_VIEW
	{
		NET_MSG_GENERIC	nmg;
		DWORD			dwCharID;

		SNETPC_REQ_CHAR_VIEW()
			: dwCharID(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_CHAR_VIEW;
		}
	};

	struct SNETPC_REQ_CHAR_VIEW_FB
	{
		NET_MSG_GENERIC	nmg;
		WORD			emFB;
		float			fDelay;
		SCHAR_VIEW		sCharView;

		SNETPC_REQ_CHAR_VIEW_FB()
			: emFB(EM_CHAR_VIEW_FB_NONE)
			, fDelay(0.0f)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_CHAR_VIEW_FB;
		}
	};

	struct SNETPC_REQ_CHAR_VIEW_AGT
	{
		NET_MSG_GENERIC	nmg;
		DWORD			dwGaeaID;

		SNETPC_REQ_CHAR_VIEW_AGT()
			: dwGaeaID(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_CHAR_VIEW_AGT;
		}
	};

	struct SNETPC_REQ_CHAR_VIEW_FLD
	{
		NET_MSG_GENERIC	nmg;
		WORD			emFB;
		DWORD			dwGaeaID;
		SCHAR_VIEW		sCharView;

		SNETPC_REQ_CHAR_VIEW_FLD()
			: emFB(EM_CHAR_VIEW_FB_NONE)
			, dwGaeaID(0)
		{
			nmg.dwSize	= sizeof(*this);
			nmg.nType	= NET_MSG_GCTRL_REQ_CHAR_VIEW_FLD;
		}
	};
	/////////////////////////////////////////////////////////////////////////////

	//itemmall
	struct SNETPC_RETRIEVE_POINTS
	{
		NET_MSG_GENERIC	nmg;
		
		SNETPC_RETRIEVE_POINTS()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_RETRIEVE_POINTS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

	};

	
	struct SNETPC_RETRIEVE_POINTS_FB
	{
		NET_MSG_GENERIC	nmg;
		DWORD	PPoints;
		DWORD	VPoints;
		EMREQ_RETRIEVE_POINTS_FB emFB;
		
		SNETPC_RETRIEVE_POINTS_FB() : 
			PPoints(0),
			 VPoints(0),
			 emFB(EMREQ_RETRIEVE_POINTS_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_RETRIEVE_POINTS_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	//---------------------------------------------------------------------------NET
	struct SNETPC_GOTO
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwActState;
		D3DXVECTOR3			vCurPos;
		D3DXVECTOR3			vTarPos;

		SNETPC_GOTO () :
			dwActState(NULL),
			vCurPos(0,0,0),
			vTarPos(0,0,0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_GOTO);
			nmg.nType = NET_MSG_GCTRL_GOTO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_GOTO_BRD : public SNETPC_BROAD
	{
		DWORD				dwActState;
		D3DXVECTOR3			vCurPos;
		D3DXVECTOR3			vTarPos;

		float				fDelay;

		SNETPC_GOTO_BRD () :
			dwActState(NULL),
			vCurPos(0,0,0),
			vTarPos(0,0,0),

			fDelay(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GOTO_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	//---------------------------------------------------------------------------NET
	struct SNETPC_ACTSTATE
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwActState;

		SNETPC_ACTSTATE () :
			dwActState(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTSTATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_MOVESTATE
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwActState;

		SNETPC_MOVESTATE () :
			dwActState(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_MOVESTATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET

	struct SNETPC_MOVESTATE_BRD : public SNETPC_BROAD
	{
		DWORD				dwActState;

		SNETPC_MOVESTATE_BRD () :
			dwActState(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_MOVESTATE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_UPDATE_FLAGS_BRD : public SNETPC_BROAD
	{
		DWORD				dwFLAGS;

		SNETPC_UPDATE_FLAGS_BRD () :
			dwFLAGS(NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_FLAGS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_JUMP_POS_BRD : public SNETPC_BROAD
	{
		D3DXVECTOR3			vPOS;
		bool				bPassenger;
		D3DXVECTOR3			vDir;

		SNETPC_JUMP_POS_BRD () :
			vPOS(0,0,0),
			bPassenger(false),
			vDir(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_JUMP_POS_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emTarCrow;
		DWORD				dwTarID;
		DWORD				dwAniSel;
		DWORD				dwFlags;

		SNETPC_ATTACK () :
			emTarCrow(CROW_MOB),
			dwTarID(0),
			dwAniSel(0),
			dwFlags(NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_BRD : public SNETPC_BROAD
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;
		DWORD				dwAniSel;

		SNETPC_ATTACK_BRD () :
			emTarCrow(CROW_MOB),
			dwTarID(0),
			dwAniSel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_CANCEL
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_ATTACK_CANCEL ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_CANCEL;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_CANCEL_BRD : public SNETPC_BROAD
	{
		SNETPC_ATTACK_CANCEL_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_CANCEL_BRD;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_AVOID
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emTarCrow;
		DWORD				dwTarID;

		SNETPC_ATTACK_AVOID () :
			emTarCrow(CROW_MOB),
			dwTarID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_AVOID;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_AVOID_BRD : public SNETPC_BROAD
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;

		SNETPC_ATTACK_AVOID_BRD () :
			emTarCrow(CROW_MOB),
			dwTarID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_AVOID_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_DAMAGE
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emTarCrow;
		DWORD				dwTarID;
		int					nDamage;
		DWORD				dwDamageFlag;

		SNETPC_ATTACK_DAMAGE () :
			emTarCrow(CROW_MOB),
			dwTarID(0),
			nDamage(0),
			dwDamageFlag( DAMAGE_TYPE_NONE )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_DAMAGE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_DEFENSE_SKILL_ACTIVE
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emTarCrow;
		DWORD				dwTarID;
		SNATIVEID			sNativeID;
		WORD				wLevel;

		SNETPC_DEFENSE_SKILL_ACTIVE () :
			emTarCrow(CROW_MOB),
			dwTarID(0),
			sNativeID(NATIVEID_NULL()),
			wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DEFENSE_SKILL_ACTIVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	

	//---------------------------------------------------------------------------NET
	struct SNETPC_ATTACK_DAMAGE_BRD : public SNETPC_BROAD
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;
		int					nDamage;
		DWORD			dwDamageFlag;

		SNETPC_ATTACK_DAMAGE_BRD () :
			emTarCrow(CROW_MOB),
			dwTarID(0),
			nDamage(0),
			dwDamageFlag(DAMAGE_TYPE_NONE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ATTACK_DAMAGE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_POSITIONCHK_BRD : public SNETCROW_BROAD
	{
		D3DXVECTOR3 vPOS;

		SNET_POSITIONCHK_BRD () :
			vPOS(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_POSITIONCHK_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_ACTION_BRD : public SNETCROW_BROAD
	{
		EMACTIONTYPE		emAction;
		DWORD				dwFLAG;

		SNET_ACTION_BRD () :
			emAction(GLAT_IDLE),
			dwFLAG(NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTION_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_STATE
	{
		NET_MSG_GENERIC		nmg;
		
		GLPADATA			sHP;
		GLPADATA			sMP;
		GLPADATA			sSP;
		GLPADATA			sCP; /*combatpoint logic, Juver, 2017/05/28 */

		//mismatch check
		char				szCharName[CHAR_SZNAME];
		DWORD				dwCharGaeaID;
		DWORD				dwCharID;

		void ENCODE ( DWORD dwKEY );
		void DECODE ( DWORD dwKEY );

		SNETPC_UPDATE_STATE ()
			: dwCharGaeaID( GAEAID_NULL )
			, dwCharID( GAEAID_NULL )
		{
			memset (szCharName, 0, sizeof(char) * CHAR_SZNAME);
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_STATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_STATE_BRD : public SNETPC_BROAD
	{
		GLPADATA			sHP;

		SNETPC_UPDATE_STATE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_STATE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_UPDATE_PASSIVE_BRD : public SNETPC_BROAD
	{
		SPASSIVE_SKILL_DATA	sSKILL_DATA;

		SNETPC_UPDATE_PASSIVE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_PASSIVE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_EXP
	{
		NET_MSG_GENERIC		nmg;
		LONGLONG			lnNowExp;

		SNETPC_UPDATE_EXP() :
			lnNowExp(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_EXP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_MONEY
	{
		NET_MSG_GENERIC		nmg;
		LONGLONG			lnMoney;

		SNETPC_UPDATE_MONEY () :
			lnMoney(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_MONEY;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_SP
	{
		NET_MSG_GENERIC		nmg;
		WORD				wNowSP;

		SNETPC_UPDATE_SP () :
			wNowSP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_SP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_LP
	{
		NET_MSG_GENERIC		nmg;
		int					nLP;

		SNETPC_UPDATE_LP () :
			nLP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_LP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_SKP
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwSkillPoint;

		SNETPC_UPDATE_SKP () :
			dwSkillPoint(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_SKP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_BRIGHT
	{
		NET_MSG_GENERIC		nmg;
		int					nBright;

		SNETPC_UPDATE_BRIGHT () :
			nBright(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_BRIGHT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_BRIGHT_BRD : public SNETPC_BROAD
	{
		int					nBright;

		SNETPC_UPDATE_BRIGHT_BRD () :
			nBright(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_BRIGHT_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_UPDATE_STATS
	{
		NET_MSG_GENERIC		nmg;
		WORD				wStatsPoint;

		SNETPC_UPDATE_STATS () :
			wStatsPoint(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_STATS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PUSHPULL_BRD : public SNETCROW_BROAD
	{
		D3DXVECTOR3			vMovePos;

		/*push pull skill logic, Juver, 2017/06/04 */
		float	fSpeed;
		bool	bSkillMove;
		bool	bReverseDir; 

		/*dash skill logic, Juver, 2017/06/17 */
		bool	bSkillDash;
		WORD	wActionAnim;

		SNET_PUSHPULL_BRD () :
			vMovePos(0,0,0)

			 /*push pull skill logic, Juver, 2017/06/04 */
			, fSpeed(0.0f)
			, bSkillMove(false)
			, bReverseDir(false)

			 /*dash skill logic, Juver, 2017/06/17 */
			, bSkillDash(false)
			, wActionAnim(AN_SUB_00_SIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PUSHPULL_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_REQ_LEVELUP
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_REQ_LEVELUP ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_LEVELUP;
		}
	};
	//---------------------------------------------------------------------------NET
	struct SNETPC_REQ_LEVELUP_FB
	{
		NET_MSG_GENERIC		nmg;
		WORD				wLevel;
		WORD				wStatsPoint;
		DWORD				dwSkillPoint;
		bool				bInitNowLevel;
		SNATIVEID			sMapID;

		SNETPC_REQ_LEVELUP_FB () :
			wLevel(0),
			wStatsPoint(0),
			dwSkillPoint(0),
			bInitNowLevel(TRUE),
			sMapID(NATIVEID_NULL())
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_LEVELUP_FB;
		}
	};

	struct SNETPC_REQ_LEVELUP_BRD : public SNETPC_BROAD
	{
		SNETPC_REQ_LEVELUP_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_LEVELUP_BRD;
		}
	};

	struct SNETPC_UPDATE_MONEYTYPE
	{
		NET_MSG_GENERIC		nmg;
		LONGLONG	lnGold;

		SNETPC_UPDATE_MONEYTYPE () : 
		 		lnGold(0)	
		 		
		 {			
		 	nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_MONEYTYPE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	struct SNETPC_REQ_STATSUP
	{
		NET_MSG_GENERIC		nmg;
		SCHARSTATS				sStats;
		EMSTATS				emStats;

		SNETPC_REQ_STATSUP () :
			emStats(EMPOW)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUP;
		}
	};

	//add addstats cmd
	struct SNETPC_REQ_STATSUPCMD
	{
		NET_MSG_GENERIC		nmg;
		EMSTATS				emStats;
		DWORD				dwValue;

		SNETPC_REQ_STATSUPCMD () :
			emStats(EMPOW),
			dwValue(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUPCMD;
		}
	};

	struct SNETPC_REQ_STATSUP_FB
	{
		NET_MSG_GENERIC		nmg;

		SCHARSTATS			sStats;
		EMSTATS				emStats;
		DWORD				wStatsPoint;

		SNETPC_REQ_STATSUP_FB () :
			emStats(EMPOW),
			wStatsPoint(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUP_FB;
		}
	};
//add addstats cmd
	struct SNETPC_REQ_STATSUPCMD_FB
	{
		NET_MSG_GENERIC		nmg;

		EMSTATS				emStats;
		DWORD				wStatsPoint;
		DWORD				dwValue;

		SNETPC_REQ_STATSUPCMD_FB () :
			emStats(EMPOW),
			wStatsPoint(0),
			dwValue(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUPCMD_FB;
		}
	};
	struct SNETPC_REQ_BUS
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		DWORD				dwNPC_ID;
		DWORD				dwSTATION_ID;

		SNETPC_REQ_BUS () :
			wPosX(0),
			wPosY(0),
			dwNPC_ID(0),
			dwSTATION_ID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_BUS;
		}
	};

	struct SNETPC_REQ_BUS_FB
	{
		NET_MSG_GENERIC		nmg;

		EMBUS_TAKE			emFB;

		SNETPC_REQ_BUS_FB () :
			emFB(EMBUS_TAKE_OK)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_BUS_FB;
		}
	};

	struct SNETPC_REQ_TAXI
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		DWORD				dwSelectMap;
		DWORD				dwSelectStop;
		DWORD				dwGaeaID;
		D3DXVECTOR3			vPos;		

		SNETPC_REQ_TAXI () :
			wPosX(0),
			wPosY(0),
			dwSelectMap(0),
			dwSelectStop(0),
			dwGaeaID(0),
			vPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_TAXI;
		}
	};

	struct SNETPC_REQ_TAXI_NPCPOS
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		DWORD				dwSelectMap;
		DWORD				dwSelectStop;
		DWORD				dwGaeaID;
		D3DXVECTOR3			vPos;		

		SNETPC_REQ_TAXI_NPCPOS () :
			wPosX(0),
			wPosY(0),
			dwSelectMap(0),
			dwSelectStop(0),
			dwGaeaID(0),
			vPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_TAXI_NPCPOS;
		}
	};

	struct SNETPC_REQ_TAXI_NPCPOS_FB
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		DWORD				dwSelectMap;
		DWORD				dwSelectStop;
		DWORD				dwGaeaID;
		D3DXVECTOR3			vPos;		

		SNETPC_REQ_TAXI_NPCPOS_FB () :
			wPosX(0),
			wPosY(0),
			dwSelectMap(0),
			dwSelectStop(0),
			dwGaeaID(0),
			vPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_TAXI_NPCPOS_FB;
		}
	};

	struct SNETPC_REQ_TAXI_FB
	{
		NET_MSG_GENERIC		nmg;

		EMTAXI_TAKE			emFB;

		SNETPC_REQ_TAXI_FB () :
			emFB(EMTAXI_TAKE_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_TAXI_FB;
		}
	};

	struct SNETPC_STORAGE_STATE
	{
		NET_MSG_GENERIC		nmg;
		bool				bVALID[EMSTORAGE_CHANNEL_SPAN_SIZE];

		SNETPC_STORAGE_STATE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_STORAGE_STATE;

			for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i  )
				bVALID[i] = false;
		}
	};

	struct SNETPC_PREMIUM_STATE
	{
		NET_MSG_GENERIC		nmg;
		bool				bPREMIUM;

		SNETPC_PREMIUM_STATE () :
			bPREMIUM(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PREMIUM_STATE;
		}
	};

	struct SNETPC_REQ_FIRECRACKER
	{
		NET_MSG_GENERIC		nmg;
		D3DXVECTOR3			vPOS;

		SNETPC_REQ_FIRECRACKER () :
			vPOS(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_FIRECRACKER;
		}
	};
	
	struct SNETPC_REQ_FIRECRACKER_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_FIRECRACKER_FB	emFB;

		SNETPC_REQ_FIRECRACKER_FB () :
			emFB(EMREQ_FIRECRACKER_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_FIRECRACKER_FB;
		}
	};

	struct SNETPC_REQ_FIRECRACKER_BRD
	{
		NET_MSG_GENERIC			nmg;

		D3DXVECTOR3				vPOS;
		SNATIVEID				nidITEM;

		SNETPC_REQ_FIRECRACKER_BRD () :
			vPOS(0,0,0),
			nidITEM(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_FIRECRACKER_BRD;
		}
	};

	struct SNETPC_REQ_REVIVE
	{
		NET_MSG_GENERIC			nmg;
		
		SNETPC_REQ_REVIVE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REVIVE;
		}
	};

	struct SNETPC_REQ_REVIVE_FB
	{
		NET_MSG_GENERIC			nmg;
		bool					bEventRevive; /*pvp aanny, Juver, 2017/08/25 */
		EMREQ_REVIVE_FB			emFB;
		
		SNETPC_REQ_REVIVE_FB () 
			: emFB(EMREQ_REVIVE_FB_FAIL)
			, bEventRevive(false) /*pvp tyranny, Juver, 2017/08/25 */
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REVIVE_FB;
		}
	};


	struct SNETPC_REQ_GETEXP_RECOVERY
	{
		NET_MSG_GENERIC			nmg;

		SNETPC_REQ_GETEXP_RECOVERY ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GETEXP_RECOVERY;
		}
	};

	struct SNETPC_REQ_GETEXP_RECOVERY_FB
	{
		NET_MSG_GENERIC			nmg;

		LONGLONG				nDecExp;
		LONGLONG				nReExp;
		LONGLONG				nDecMoney;

		SNETPC_REQ_GETEXP_RECOVERY_FB ()
			: nDecExp ( 0 )
			, nReExp ( 0 )
			, nDecMoney ( 0 ) 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GETEXP_RECOVERY_FB;
		}
	};

	struct SNETPC_REQ_GETEXP_RECOVERY_NPC
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwNPCID;

		SNETPC_REQ_GETEXP_RECOVERY_NPC ()
			: dwNPCID( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GETEXP_RECOVERY_NPC;
		}
	};

	struct SNETPC_REQ_GETEXP_RECOVERY_NPC_FB
	{
		NET_MSG_GENERIC			nmg;

		LONGLONG				nReExp;
		LONGLONG				nDecMoney;
		DWORD					dwNPCID;

		SNETPC_REQ_GETEXP_RECOVERY_NPC_FB ()
			: nReExp ( 0 )
			, nDecMoney ( 0 ) 
			, dwNPCID( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GETEXP_RECOVERY_NPC_FB;
		}
	};

	struct SNETPC_REQ_RECOVERY
	{
		NET_MSG_GENERIC			nmg;

		SNETPC_REQ_RECOVERY ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_RECOVERY;
		}
	};

	struct SNETPC_REQ_RECOVERY_FB
	{
		NET_MSG_GENERIC			nmg;

		EMREQ_RECOVERY_FB		emFB;
		LONGLONG				nReExp;
		

		SNETPC_REQ_RECOVERY_FB ()
			: emFB(EMREQ_RECOVERY_FB_FAIL)
			, nReExp( 0 )		
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_RECOVERY_FB;
		}
	};

	struct SNETPC_REQ_RECOVERY_NPC
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwNPCID;

		SNETPC_REQ_RECOVERY_NPC ()
			: dwNPCID ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_RECOVERY_NPC;
		}
	};

	struct SNETPC_REQ_RECOVERY_NPC_FB
	{
		NET_MSG_GENERIC			nmg;

		EMREQ_RECOVERY_NPC_FB	emFB;
		LONGLONG				nReExp;

		SNETPC_REQ_RECOVERY_NPC_FB ()
			: emFB(EMREQ_RECOVERY_NPC_FB_FAIL)
			, nReExp( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_RECOVERY_NPC_FB;
		}
	};

	struct SNETPC_PLAYERKILLING_ADD
	{
		NET_MSG_GENERIC			nmg;
		
		DWORD					dwCharID;
		WORD					wSchoolID;
		BOOL					bBAD;
		BOOL					bClubBattle;
		float					fTime;

		char					szName[CHAR_SZNAME];

		SNETPC_PLAYERKILLING_ADD () 
			: dwCharID(0)
			, wSchoolID(0)
			, bBAD(FALSE)
			, fTime(0)
			, bClubBattle(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_PLAYERKILLING_ADD);
			nmg.nType = NET_MSG_GCTRL_PLAYERKILLING_ADD;
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);
		}
	};

	struct SNETPC_PLAYERKILLING_DEL
	{
		NET_MSG_GENERIC			nmg;

		DWORD					dwCharID;

		SNETPC_PLAYERKILLING_DEL () 
			: dwCharID(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_PLAYERKILLING_DEL);
			nmg.nType = NET_MSG_GCTRL_PLAYERKILLING_DEL;
		}
	};

	struct SNETPC_INVEN_HAIR_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;

		SNETPC_INVEN_HAIR_CHANGE () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_INVEN_HAIR_CHANGE);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIR_CHANGE;
		}
	};

	struct SNETPC_INVEN_HAIR_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_HAIR_CHANGE_FB	emFB;
		DWORD					dwID;

		SNETPC_INVEN_HAIR_CHANGE_FB () :
			emFB(EMINVEN_HAIR_CHANGE_FB_FAIL),
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIR_CHANGE_FB;
		}
	};

	struct SNETPC_INVEN_HAIR_CHANGE_BRD : public SNETPC_BROAD
	{
		DWORD					dwID;

		SNETPC_INVEN_HAIR_CHANGE_BRD () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIR_CHANGE_BRD;
		}
	};

	struct SNETPC_INVEN_HAIRSTYLE_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;
		WORD					wHairStyle;

		SNETPC_INVEN_HAIRSTYLE_CHANGE () 
			: wPosX(0)
			, wPosY(0)
			, wHairStyle(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE;
		}
	};

	struct SNETPC_INVEN_HAIRSTYLE_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_HAIR_CHANGE_FB	emFB;
		DWORD					dwID;

		SNETPC_INVEN_HAIRSTYLE_CHANGE_FB () :
			emFB(EMINVEN_HAIR_CHANGE_FB_FAIL),
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_FB;
		}
	};

	struct SNETPC_INVEN_HAIRSTYLE_CHANGE_BRD : public SNETPC_BROAD
	{
		DWORD					dwID;

		SNETPC_INVEN_HAIRSTYLE_CHANGE_BRD () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_BRD;
		}
	};

	struct SNETPC_INVEN_HAIRCOLOR_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;
		WORD					wHairColor;

		SNETPC_INVEN_HAIRCOLOR_CHANGE () 
			: wPosX(0)
			, wPosY(0)
			, wHairColor(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE;
		}
	};

	struct SNETPC_INVEN_HAIRCOLOR_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_HAIR_CHANGE_FB	emFB;
		WORD					wHairColor;

		SNETPC_INVEN_HAIRCOLOR_CHANGE_FB () :
			emFB(EMINVEN_HAIR_CHANGE_FB_FAIL),
			wHairColor(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_FB;
		}
	};

	struct SNETPC_INVEN_HAIRCOLOR_CHANGE_BRD : public SNETPC_BROAD
	{
		WORD					wHairColor;

		SNETPC_INVEN_HAIRCOLOR_CHANGE_BRD () :
			wHairColor(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_BRD;
		}
	};

	struct SNETPC_INVEN_FACE_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;

		SNETPC_INVEN_FACE_CHANGE () :
			wPosX(0),
			wPosY(0)			
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACE_CHANGE;
		}
	};

	struct SNETPC_INVEN_FACE_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_FACE_CHANGE_FB	emFB;
		DWORD					dwID;

		SNETPC_INVEN_FACE_CHANGE_FB () :
			emFB(EMINVEN_FACE_CHANGE_FB_FAIL),
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACE_CHANGE_FB;
		}
	};

	struct SNETPC_INVEN_FACE_CHANGE_BRD : public SNETPC_BROAD
	{
		DWORD					dwID;

		SNETPC_INVEN_FACE_CHANGE_BRD () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACE_CHANGE_BRD;
		}
	};

	struct SNETPC_INVEN_FACESTYLE_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;
		WORD					wFaceStyle;

		SNETPC_INVEN_FACESTYLE_CHANGE () 
			: wPosX(0)
			, wPosY(0)
			, wFaceStyle(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE;
		}
	};

	struct SNETPC_INVEN_FACESTYLE_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_HAIR_CHANGE_FB	emFB;
		DWORD					dwID;

		SNETPC_INVEN_FACESTYLE_CHANGE_FB () :
			emFB(EMINVEN_HAIR_CHANGE_FB_FAIL),
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_FB;
		}
	};

	struct SNETPC_INVEN_FACESTYLE_CHANGE_BRD : public SNETPC_BROAD
	{
		DWORD					dwID;

		SNETPC_INVEN_FACESTYLE_CHANGE_BRD () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_BRD;
		}
	};

	struct SNETPC_INVEN_GENDER_CHANGE
	{
		NET_MSG_GENERIC			nmg;
		
		WORD					wPosX;
		WORD					wPosY;
		WORD					wFace;
		WORD					wHair;

		SNETPC_INVEN_GENDER_CHANGE () 
			: wPosX(0)
			, wPosY(0)
			, wFace(0)
			, wHair(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_GENDER_CHANGE;
		}
	};

	struct SNETPC_INVEN_GENDER_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		
		EMINVEN_GENDER_CHANGE_FB	emFB;
		WORD						wFace;
		WORD						wHair;
		WORD						wHairColor;
		EMCHARCLASS					emClass;
		WORD						wSex;

		SNETPC_INVEN_GENDER_CHANGE_FB () :
			emFB(EMINVEN_GENDER_CHANGE_FB_FAIL),
			wFace(0),
			wHair(0),
			wHairColor(0),
			emClass ( GLCC_NONE ),
			wSex ( 0 )

		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_GENDER_CHANGE_FB;
		}
	};


	struct SNETPC_INVEN_RENAME
	{
		NET_MSG_GENERIC			nmg;

		char					szName[CHAR_SZNAME];
		WORD					wPosX;
		WORD					wPosY;

		SNETPC_INVEN_RENAME () :
			wPosX(0),
			wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			memset ( szName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.nType  = NET_MSG_GCTRL_INVEN_RENAME;
		}
	};

	struct SNETPC_INVEN_RENAME_FB
	{
		NET_MSG_GENERIC			nmg;

		EMINVEN_RENAME_FB		emFB;
		char					szName[CHAR_SZNAME];

		SNETPC_INVEN_RENAME_FB () :
			emFB(EMINVEN_RENAME_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			memset ( szName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.nType = NET_MSG_GCTRL_INVEN_RENAME_FB;
		}
	};

	struct SNETPC_INVEN_RENAME_FROM_DB
	{
		NET_MSG_GENERIC				nmg;

		EMINVEN_RENAME_FB			emFB;
		DWORD						dwCharID;
		WORD						wPosX;
		WORD						wPosY;
		char						szName[CHAR_SZNAME];

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(DWORD)*3+CHAR_SZNAME };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNETPC_INVEN_RENAME_FROM_DB () :
			emFB(EMINVEN_RENAME_FROM_DB_FAIL),
			dwCharID(0),
			wPosX(0),
			wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			memset ( szName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.nType = NET_MSG_GCTRL_INVEN_RENAME_FROM_DB;
		}
	};

	struct SNETPC_INVEN_RENAME_BRD : public SNETPC_BROAD
	{
		char 		szName[CHAR_SZNAME];

		SNETPC_INVEN_RENAME_BRD ()
		{
			memset ( szName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_RENAME_BRD;
		}
	};

	struct SNETPC_INVEN_RENAME_AGTBRD
	{
		NET_MSG_GENERIC		nmg;
		
		DWORD				dwID;
		char				szOldName[CHAR_SZNAME];
		char				szNewName[CHAR_SZNAME];

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(DWORD)+(CHAR_SZNAME)*2 };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNETPC_INVEN_RENAME_AGTBRD () :
			dwID(0)
		{
			memset ( szOldName, 0, sizeof(char)*CHAR_SZNAME );
			memset ( szNewName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_RENAME_AGTBRD;
		}
	};

	struct SNETPC_CHANGE_NAMEMAP : public SNETPC_BROAD
	{
		char 					szOldName[CHAR_SZNAME];
		char 					szNewName[CHAR_SZNAME];

		SNETPC_CHANGE_NAMEMAP ()
		{
			memset ( szOldName, 0, sizeof(char)*CHAR_SZNAME );
			memset ( szNewName, 0, sizeof(char)*CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CHANGE_NAMEMAP;
		}
	};




	struct SNETPC_REQ_GESTURE
	{
		NET_MSG_GENERIC			nmg;
		
		DWORD					dwID;

		SNETPC_REQ_GESTURE () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_GESTURE;
		}
	};

	struct SNETPC_REQ_GESTURE_BRD : public SNETPC_BROAD
	{
		DWORD					dwID;

		SNETPC_REQ_GESTURE_BRD () :
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_GESTURE_BRD;
		}
	};

	struct SNETPC_QITEMFACT_BRD : public SNETPC_BROAD
	{
		SQITEM_FACT sFACT;

		SNETPC_QITEMFACT_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_QITEMFACT_BRD;
		}
	};

	struct SNETPC_QITEMFACT_END_BRD : public SNETPC_BROAD
	{
		SNETPC_QITEMFACT_END_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_QITEMFACT_END_BRD;
		}
	};

	struct SNETPC_PKCOMBO_BRD : public SNETPC_BROAD
	{
		SPKCOMBO_COUNT		sCOMBO;

		SNETPC_PKCOMBO_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PKCOMBO_BRD;
		}
	};

	struct SNETPC_PKCOMBO_END_BRD : public SNETPC_BROAD
	{
		SNETPC_PKCOMBO_END_BRD()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PKCOMBO_END_BRD;
		}
	};

	struct SNETPC_EVENTFACT_BRD : public SNETPC_BROAD
	{
		EMGM_EVENT_TYPE		emType;
		WORD				wValue;

		SNETPC_EVENTFACT_BRD ()
			: emType(EMGM_EVENT_NONE)
			, wValue(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_EVENTFACT_BRD;
		}
	};

	struct SNETPC_EVENTFACT_END_BRD : public SNETPC_BROAD
	{
		EMGM_EVENT_TYPE		emType;

		SNETPC_EVENTFACT_END_BRD ()
			: emType(EMGM_EVENT_NONE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_EVENTFACT_END_BRD;
		}
	};

	struct SNETPC_EVENTFACT_INFO
	{
		NET_MSG_GENERIC			nmg;
		SEVENT_FACT				sEVENTFACT;

		SNETPC_EVENTFACT_INFO ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_EVENTFACT_INFO;
		}
	};

	struct SNETPC_2_FRIEND_REQ
	{
		NET_MSG_GENERIC			nmg;
		char					szFRIEND_NAME[CHAR_SZNAME];
		WORD					wItemPosX;
		WORD					wItemPosY;

		SNETPC_2_FRIEND_REQ () :
			wItemPosX(0),
			wItemPosY(0)
		{
			memset(szFRIEND_NAME, 0, sizeof(char) * CHAR_SZNAME);
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_2_FRIEND_REQ;
		}
	};

	struct SNETPC_2_FRIEND_CK
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwCOMMAND_CHARID;
		DWORD					dwFRIEND_CHARID;
		WORD					wItemPosX;
		WORD					wItemPosY;

		SNETPC_2_FRIEND_CK () :
			dwCOMMAND_CHARID(0),
			dwFRIEND_CHARID(0),
			wItemPosX(0),
			wItemPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_2_FRIEND_CK;
		}
	};

	struct SNETPC_2_FRIEND_AG
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwCOMMAND_CHARID;
		DWORD					dwFRIEND_CHARID;
		WORD					wItemPosX;
		WORD					wItemPosY;

		EM2FRIEND_FB			emFB;
		SNATIVEID				sFriendMapID;
		D3DXVECTOR3				vFriendPos;

		SNETPC_2_FRIEND_AG () :
			dwCOMMAND_CHARID(0),
			dwFRIEND_CHARID(0),
			wItemPosX(0),
			wItemPosY(0),

			emFB(EM2FRIEND_FB_FAIL),
			sFriendMapID(0,0),
			vFriendPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_2_FRIEND_AG;
		}
	};

	struct SNETPC_2_FRIEND_FLD
	{
		NET_MSG_GENERIC			nmg;
		WORD					wItemPosX;
		WORD					wItemPosY;

		SNATIVEID				sFriendMapID;
		D3DXVECTOR3				vFriendPos;

		SNETPC_2_FRIEND_FLD () :
			wItemPosX(0),
			wItemPosY(0),

			sFriendMapID(0,0),
			vFriendPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_2_FRIEND_FLD;
		}
	};

	struct SNETPC_2_FRIEND_FB
	{
		NET_MSG_GENERIC			nmg;
		EM2FRIEND_FB			emFB;

		SNETPC_2_FRIEND_FB () :
			emFB(EM2FRIEND_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_2_FRIEND_FB;
		}
	};

	struct SNETPC_GM_GENCHAR
	{
		NET_MSG_GENERIC			nmg;
		char					szNAME[CHAR_SZNAME];
		DWORD					dwCHARID;

		SNETPC_GM_GENCHAR ()
			: dwCHARID(0)
		{
			memset(szNAME, 0, sizeof(char) * (CHAR_SZNAME));
			nmg.dwSize = sizeof(SNETPC_GM_GENCHAR);
			nmg.nType = NET_MSG_GM_GENCHAR;
		}
	};

	struct SNETPC_GM_GENCHAR_FB
	{
		NET_MSG_GENERIC			nmg;

		int						nChannel;
		EMGM_MOVE2CHAR_FB		emFB;

		SNETPC_GM_GENCHAR_FB () :
			nChannel(0),
			emFB(EMGM_MOVE2CHAR_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GM_GENCHAR_FB;
		}
	};

	struct SNETPC_GM_GETWHISPERMSG
	{
		NET_MSG_GENERIC			nmg;
		char					szNAME[CHAR_SZNAME];

		SNETPC_GM_GETWHISPERMSG ()
		{
			nmg.dwSize = sizeof(SNETPC_GM_GETWHISPERMSG);
			nmg.nType = NET_MSG_GM_GETWHISPERMSG;
			memset(szNAME, 0, sizeof(char) * (CHAR_SZNAME));
		}
	};



	struct SNETPC_GM_MOVE2CHAR
	{
		NET_MSG_GENERIC			nmg;
		char					szNAME[CHAR_SZNAME];
		DWORD					dwCHARID;

		SNETPC_GM_MOVE2CHAR ()
			: dwCHARID(0)
		{
			nmg.dwSize = sizeof(SNETPC_GM_MOVE2CHAR);
			nmg.nType = NET_MSG_GM_MOVE2CHAR;
			memset(szNAME, 0, sizeof(char) * (CHAR_SZNAME));
		}
	};

	struct SNETPC_GM_MOVE2CHAR_POS
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwCOMMAND_CHARID;
		DWORD					dwTO_CHARID;

		SNETPC_GM_MOVE2CHAR_POS () 
			: dwCOMMAND_CHARID(0)
			, dwTO_CHARID(0)
		{
			nmg.dwSize = sizeof(SNETPC_GM_MOVE2CHAR_POS);
			nmg.nType = NET_MSG_GM_MOVE2CHAR_POS;
		}
	};

	struct SNETPC_GM_MOVE2CHAR_AG
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwCOMMAND_CHARID;
		DWORD					dwTO_CHARID;

		EMGM_MOVE2CHAR_FB		emFB;
		SNATIVEID				sToMapID;
		D3DXVECTOR3				vToPos;

		SNETPC_GM_MOVE2CHAR_AG () :
			dwCOMMAND_CHARID(0),
			dwTO_CHARID(0),

			emFB(EMGM_MOVE2CHAR_FB_FAIL),
			sToMapID(0,0),
			vToPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GM_MOVE2CHAR_AG;
		}
	};

	struct SNETPC_GM_MOVE2CHAR_FLD
	{
		NET_MSG_GENERIC			nmg;

		SNATIVEID				sToMapID;
		D3DXVECTOR3				vToPos;

		SNETPC_GM_MOVE2CHAR_FLD () :
			sToMapID(0,0),
			vToPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GM_MOVE2CHAR_FLD;
		}
	};

	struct SNETPC_GM_MOVE2CHAR_FB
	{
		NET_MSG_GENERIC			nmg;

		int						nChannel;
		EMGM_MOVE2CHAR_FB		emFB;

		SNETPC_GM_MOVE2CHAR_FB () :
			nChannel(0),
			emFB(EMGM_MOVE2CHAR_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GM_MOVE2CHAR_FB;
		}
	};

	struct SNETPC_UPDATE_LASTCALL
	{
		NET_MSG_GENERIC		nmg;
		
		SNATIVEID			sLastCallMapID;		//	직전귀환 맵.
		D3DXVECTOR3			vLastCallPos;		//	직전귀환 위치.

		SNETPC_UPDATE_LASTCALL () :
			vLastCallPos(0,0,0)
		{
			nmg.dwSize = sizeof(SNETPC_UPDATE_LASTCALL);
			nmg.nType = NET_MSG_GCTRL_UPDATE_LASTCALL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_UPDATE_STARTCALL
	{
		NET_MSG_GENERIC		nmg;
		
		SNATIVEID			sStartMapID;		// 직전귀환 맵
		DWORD				dwStartGateID;		// 직전귀환 게이트
		D3DXVECTOR3			vStartPos;			// 직전귀환 위치


		SNETPC_UPDATE_STARTCALL () :
			dwStartGateID(0),
			vStartPos(0,0,0)
		{
			nmg.dwSize = sizeof(SNETPC_UPDATE_STARTCALL);
			nmg.nType = NET_MSG_GCTRL_UPDATE_STARTCALL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_MGAME_ODDEVEN
	{
		NET_MSG_GENERIC	nmg;

		DWORD					dwNPCID;
		EMMGAME_ODDEVEN			emEvent;
		EMMGAME_ODDEVEN_CASE	emCase;
		UINT					uiBattingMoney;		// 배팅 금액

		SNETPC_MGAME_ODDEVEN () :
			dwNPCID(0),
			emEvent(EMMGAME_ODDEVEN_CANCEL),
			emCase(EMMGAME_ODDEVEN_ODD),
			uiBattingMoney(0)
		{
			nmg.dwSize = sizeof(SNETPC_MGAME_ODDEVEN);
			nmg.nType = NET_MSG_MGAME_ODDEVEN;
		}
	};

	struct SNETPC_MGAME_ODDEVEN_FB
	{
		NET_MSG_GENERIC	nmg;

		EMMGAME_ODDEVEN_FB		emResult;
		EMMGAME_ODDEVEN_CASE	emCase;
		ULONGLONG				ui64DividendMoney;	// 배당금
		ULONGLONG				ui64ActualMoney;	// 실수령금
		WORD					wRound;				// 게임 회수

		SNETPC_MGAME_ODDEVEN_FB () :
			emResult(EMMGAME_ODDEVEN_FB_FAIL),
			emCase(EMMGAME_ODDEVEN_ODD),
			ui64DividendMoney(0),
			ui64ActualMoney(0),
			wRound(0)
		{
			nmg.dwSize = sizeof(SNETPC_MGAME_ODDEVEN_FB);
			nmg.nType = NET_MSG_MGAME_ODDEVEN_FB;
		}
	};

	struct SNETPC_SEND_SMS
	{
		NET_MSG_GENERIC	nmg;

		DWORD			dwReceiveChaNum;
		TCHAR			szPhoneNumber[SMS_RECEIVER];
		TCHAR			szSmsMsg[SMS_LENGTH];

		WORD			wItemPosX;
		WORD			wItemPosY;

		SNETPC_SEND_SMS () :
			wItemPosX(0),
			wItemPosY(0),
			dwReceiveChaNum(0)
		{
			memset(szPhoneNumber, 0, sizeof(TCHAR) * (SMS_RECEIVER));
			memset(szSmsMsg, 0, sizeof(TCHAR) * (SMS_LENGTH));
			nmg.dwSize = sizeof(SNETPC_SEND_SMS);
			nmg.nType = NET_MSG_SMS_SEND;
		}
	};

	struct SNETPC_SEND_SMS_FB
	{
		NET_MSG_GENERIC			nmg;

		EMSMS_SEND_FB			emFB;

		SNETPC_SEND_SMS_FB () :
			emFB(EMSMS_SEND_FB_FAIL)
		{
			nmg.dwSize = sizeof(SNETPC_SEND_SMS_FB);
			nmg.nType = NET_MSG_SMS_SEND_FB;
		}
	};

	struct SNETPC_SEND_SMS_FROM_DB
	{
		NET_MSG_GENERIC			nmg;

		EMSMS_SEND_FB			emFB;

		WORD					wPosX;
		WORD					wPosY;

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(EMSMS_SEND_FB)+(sizeof(WORD)*2) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNETPC_SEND_SMS_FROM_DB () :
        	emFB(EMSMS_SEND_FROM_DB_FAIL),
			wPosX(0),
			wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_SMS_SEND_FROM_DB;
		}
	};

	struct SNETPC_PHONE_NUMBER
	{
		NET_MSG_GENERIC			nmg;

		TCHAR					szPhoneNumber[SMS_RECEIVER];

		SNETPC_PHONE_NUMBER ()
		{
			memset(szPhoneNumber, 0, sizeof(TCHAR) * (SMS_RECEIVER));
			nmg.dwSize = sizeof(SNETPC_PHONE_NUMBER);
			nmg.nType = NET_MSG_SMS_PHONE_NUMBER;
		}
	};

	struct SNETPC_PHONE_NUMBER_FB
	{
		NET_MSG_GENERIC			nmg;

		EMSMS_PHONE_NUMBER_FB	emFB;
		TCHAR					szPhoneNumber[SMS_RECEIVER];

		SNETPC_PHONE_NUMBER_FB () :
			emFB(EMSMS_PHONE_NUMBER_FB_FAIL)
		{
			memset(szPhoneNumber, 0, sizeof(TCHAR) * (SMS_RECEIVER));
			nmg.dwSize = sizeof(SNETPC_PHONE_NUMBER_FB);
			nmg.nType = NET_MSG_SMS_PHONE_NUMBER_FB;
		}
	};

	struct SNETPC_PHONE_NUMBER_FROM_DB
	{
		NET_MSG_GENERIC				nmg;

		EMSMS_PHONE_NUMBER_FB		emFB;
		DWORD						dwCharID;
		TCHAR						szPhoneNumber[SMS_RECEIVER];

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(EMSMS_PHONE_NUMBER_FB)+sizeof(DWORD)+(SMS_RECEIVER) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNETPC_PHONE_NUMBER_FROM_DB () :
			emFB(EMSMS_PHONE_NUMBER_FROM_DB_FAIL),
			dwCharID(0)
		{
			nmg.dwSize = sizeof(*this);
			memset ( szPhoneNumber, 0, sizeof(TCHAR)*SMS_RECEIVER );
			nmg.nType = NET_MSG_SMS_PHONE_NUMBER_FROM_DB;
		}
	};

	struct SNETPC_PHONE_NUMBER_AGTBRD
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwID;
		TCHAR				szName[CHAR_SZNAME];
		TCHAR				szNewPhoneNumber[SMS_RECEIVER];

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(DWORD)+(SMS_RECEIVER)+(CHAR_SZNAME) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNETPC_PHONE_NUMBER_AGTBRD () :
			dwID(0)
		{
			memset ( szName, 0, sizeof(TCHAR)*CHAR_SZNAME );
			memset ( szNewPhoneNumber, 0, sizeof(TCHAR)*SMS_RECEIVER );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_SMS_PHONE_NUMBER_AGTBRD;
		}
	};

	struct SNETPC_CHINA_GAINTYPE
	{
		NET_MSG_GENERIC		nmg;

		BYTE				dwGainType;


		SNETPC_CHINA_GAINTYPE () :
		dwGainType(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_CHINA_GAINTYPE;
		}
	};

	struct SNETPC_VIETNAM_GAINTYPE
	{
		NET_MSG_GENERIC		nmg;

		BYTE				dwGainType;


		SNETPC_VIETNAM_GAINTYPE () :
		dwGainType(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_GAINTYPE;
		}
	};

	struct SNETPC_VIETNAM_TIME_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwGaeaID;


		SNETPC_VIETNAM_TIME_REQ () :
								dwGaeaID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_TIME_REQ;
		}
	};

	struct SNETPC_VIETNAM_TIME_REQ_FB
	{
		NET_MSG_GENERIC		nmg;		
		LONGLONG			gameTime;  //	저장된 누적 시간
		__time64_t			loginTime; //	로그인 시간



		SNETPC_VIETNAM_TIME_REQ_FB () :
							gameTime(0),
							loginTime(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_TIME_REQ_FB;
		}
	};

	struct SNETPC_VIETNAM_GAIN_EXP
	{
		NET_MSG_GENERIC		nmg;		
		LONGLONG			gainExp;   //	저장된 누적 경험치

		SNETPC_VIETNAM_GAIN_EXP () :
							gainExp(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_GAIN_EXP;
		}
	};

	struct SNETPC_VIETNAM_GAIN_MONEY
	{
		NET_MSG_GENERIC		nmg;		
		LONGLONG			gainMoney;   //	저장된 누적 돈

		SNETPC_VIETNAM_GAIN_MONEY () :
							gainMoney(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_GAIN_MONEY;
		}
	};

	struct SNETPC_INVEN_VIETNAM_INVENGET
	{
		NET_MSG_GENERIC			nmg;

		WORD					wPosX;
		WORD					wPosY;
		bool					bGetExp;

		SNETPC_INVEN_VIETNAM_INVENGET () :
			wPosX(0),
			wPosY(0),
			bGetExp(TRUE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_VIETNAM_INVENGET;
		}
	};

	struct SNETPC_INVEN_VIETNAM_EXPGET_FB
	{
		NET_MSG_GENERIC			nmg;	

		LONGLONG				lnVnMoney;

		SNETPC_INVEN_VIETNAM_EXPGET_FB () 
			: lnVnMoney ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_VIETNAM_EXPGET_FB;
		}
	};

	struct SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE
	{
		NET_MSG_GENERIC			nmg;

		DWORD					dwVietnamInvenCount;
		

		SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE () :
			dwVietnamInvenCount(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGETNUM_UPDATE;
		}
	};

	struct SNETPC_INVEN_VIETNAM_ITEMGET_FB
	{
		NET_MSG_GENERIC			nmg;	
		DWORD					dwVietnamInvenCount;

		SNETPC_INVEN_VIETNAM_ITEMGET_FB () :
			dwVietnamInvenCount(0)

		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGET_FB;
		}
	};


	struct SNETPC_VIETNAM_ALLINITTIME
	{
		NET_MSG_GENERIC nmg;
		__time64_t		initTime;

		SNETPC_VIETNAM_ALLINITTIME () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_ALLINITTIME;
		}
	};


//vietnamtest%%% && vietnamtest2
	struct SNETPC_VIETNAM_TIME_RESET
	{
		NET_MSG_GENERIC nmg;

		DWORD			dwGaeaID;

		SNETPC_VIETNAM_TIME_RESET () 
		: dwGaeaID ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VIETNAM_TIME_RESET;
		}
	};


	struct SNETPC_ACTIVE_VEHICLE
	{
		NET_MSG_GENERIC		nmg;

		bool				bActive;

		SNETPC_ACTIVE_VEHICLE () :
		bActive(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTIVE_VEHICLE;
		}	
	};

	/* anti-zoomout cheat fix, SeiferXIII 2020/05/17 */
	struct SNETPC_ZOOMOUT_CHEAT_DETECTED
	{
		NET_MSG_GENERIC		nmg;

		float				fZoomOutValue;

		SNETPC_ZOOMOUT_CHEAT_DETECTED () :
		fZoomOutValue(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_ZOOMOUT_CHEAT_DETECTED;
		}

	};


	struct SNETPC_ACTIVE_VEHICLE_CONFIRM
	{
		NET_MSG_GENERIC		nmg;

		bool				bActive;

		SNETPC_ACTIVE_VEHICLE_CONFIRM () :
		bActive(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTIVE_VEHICLE_CONFIRM;
		}	
	};


	struct SNETPC_ACTIVE_VEHICLE_FB
	{
		NET_MSG_GENERIC		nmg;

		EMVEHICLE_SET_FB	emFB;
        bool				bActive;
		bool				bLeaveFieldServer;

		SNETPC_ACTIVE_VEHICLE_FB () :
		emFB(EMVEHICLE_SET_FB_FAIL),
		bActive(0),
		bLeaveFieldServer( false )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTIVE_VEHICLE_FB;
		}	
	};

	struct SNETPC_ACTIVE_VEHICLE_BRD : public SNETPC_BROAD
	{
		bool				bActive;		
		

		SNETPC_ACTIVE_VEHICLE_BRD () :
		bActive(0)		
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ACTIVE_VEHICLE_BRD;
		}	
	};

	struct SNETPC_GET_VEHICLE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			nItemID;		

		SNETPC_GET_VEHICLE () :
		nItemID(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GET_VEHICLE;
		}	
	};

	struct SNETPC_UNGET_VEHICLE_FB
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_UNGET_VEHICLE_FB () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UNGET_VEHICLE_FB;
		}	
	};

	struct SNETPC_UNGET_VEHICLE_BRD	 : public SNETPC_BROAD
	{
		SNETPC_UNGET_VEHICLE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UNGET_VEHICLE_BRD;
		}	
	};

	struct SNETPC_GET_VEHICLE_FB
	{
		NET_MSG_GENERIC		nmg;

		VEHICLE_TYPE		emTYPE;
		DWORD				dwGUID;					// 탈것의 GUID
		DWORD				dwOwner;					// 주인 GaeaID
		SNATIVEID			sVehicleID;				// 소환을 요청한 탈것 아이템의 Mid/Sid
		int					nFull;
		SITEMCUSTOM			PutOnItems[VEHICLE_ACCETYPE_SIZE];
		bool				bBooster; /*vehicle booster system, Juver, 2017/08/10 */
		WORD				wColor[BIKE_COLOR_SLOT_PART_SIZE]; /*bike color , Juver, 2017/11/13 */

		EMVEHICLE_GET_FB	emFB;

		SNETPC_GET_VEHICLE_FB () :
			emTYPE( VEHICLE_TYPE_NONE ),
			dwGUID ( UINT_MAX ),
			dwOwner ( 0 ),
			sVehicleID ( NATIVEID_NULL() ),
			nFull ( 0 ),
			emFB( EMVEHICLE_GET_FB_FAIL ),
			bBooster(false) /*vehicle booster system, Juver, 2017/08/10 */
		{
			/*bike color , Juver, 2017/11/13 */
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GET_VEHICLE_FB;
		}	
	};

	struct SNETPC_GET_VEHICLE_BRD : public SNETPC_BROAD
	{
	
		DWORD				dwGUID;					// 탈것의 GUID
		VEHICLE_TYPE		emTYPE;
		SNATIVEID			sVehicleID;				// 소환을 요청한 탈것 아이템의 Mid/Sid
		SITEMCUSTOM			PutOnItems[VEHICLE_ACCETYPE_SIZE];
		WORD				wColor[BIKE_COLOR_SLOT_PART_SIZE]; /*bike color , Juver, 2017/11/13 */


		SNETPC_GET_VEHICLE_BRD () :
			dwGUID ( 0 ),
			emTYPE ( VEHICLE_TYPE_NONE ),
			sVehicleID ( NATIVEID_NULL() )
		{
			/*bike color , Juver, 2017/11/13 */
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GET_VEHICLE_BRD;
		}
	};

	struct SNET_VEHICLE_CREATE_FROMDB_FB
	{
		NET_MSG_GENERIC		nmg;
		
		VEHICLE_TYPE		emTYPE;
		SNATIVEID			sVehicleID;					// 정보 요청한 ITEM ID
		DWORD				dwVehicleID;				// 탈것의 고유ID

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(VEHICLE_TYPE)+sizeof(SNATIVEID)+sizeof(DWORD) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNET_VEHICLE_CREATE_FROMDB_FB () :
		emTYPE ( VEHICLE_TYPE_NONE ),
		sVehicleID ( NATIVEID_NULL() ),
		dwVehicleID ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_CREATE_FROMDB_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNET_VEHICLE_GET_FROMDB_FB
	{
		NET_MSG_GENERIC		nmg;
		
		PGLVEHICLE			pVehicle;
		DWORD				dwVehicleNum;			// 소환 요청의 탈것의 고유 ID
		bool				bLMTItemCheck;
		bool				bCardInfo;
		bool				bTrade;

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(PGLVEHICLE)+sizeof(DWORD)+sizeof(bool)*3 };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNET_VEHICLE_GET_FROMDB_FB () :
			pVehicle ( NULL ),
			dwVehicleNum ( UINT_MAX ),
			bLMTItemCheck( false ),
			bCardInfo( false ),
			bTrade ( false )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_GET_FROMDB_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_VEHICLE_GET_FROMDB_ERROR
	{
		NET_MSG_GENERIC		nmg;
		
		bool				bLMTItemCheck;
		bool				bCardInfo;
		bool				bTrade;

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(bool)*3 };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNET_VEHICLE_GET_FROMDB_ERROR () :
		bLMTItemCheck( false ),
		bCardInfo( false ),
		bTrade ( false )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_GET_FROMDB_ERROR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_VEHICLE_ACCESSORY_DELETE
	{
		NET_MSG_GENERIC		nmg;
		
		VEHICLE_ACCESSORY_TYPE	accetype;
		DWORD					dwVehicleNum;			// 소환 요청의 탈것의 고유 ID

		SNET_VEHICLE_ACCESSORY_DELETE () :
		accetype ( VEHICLE_ACCETYPE_SIZE ),
		dwVehicleNum ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_ACCESSORY_DELETE;
		}
	};

	struct SNET_VEHICLE_ACCESSORY_DELETE_BRD : public SNETPC_BROAD
	{

		DWORD				dwVehicleNum;
		VEHICLE_ACCESSORY_TYPE		accetype;

		SNET_VEHICLE_ACCESSORY_DELETE_BRD () :
		dwVehicleNum ( 0 ),
		accetype ( VEHICLE_ACCETYPE_SIZE )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_ACCESSORY_DELETE_BRD;
		}

	};

	struct SNET_VEHICLE_REQ_SLOT_EX_HOLD
	{
		NET_MSG_GENERIC		nmg;

		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_SLOT_EX_HOLD () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD;
		}
	};

	struct SNET_VEHICLE_REQ_SLOT_EX_HOLD_FB
	{
		NET_MSG_GENERIC			  nmg;

		EMVEHICLE_REQ_SLOT_EX_HOLD_FB emFB;
		SITEMCUSTOM				  sItemCustom;
		EMSUIT					  emSuit;

		SNET_VEHICLE_REQ_SLOT_EX_HOLD_FB () 
			: emSuit(SUIT_NSIZE)
			, emFB(EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_FB;
		}
	};

	struct SNET_VEHICLE_REQ_SLOT_EX_HOLD_BRD : public SNETPC_BROAD
	{
		SITEMCUSTOM			sItemCustom;
		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_SLOT_EX_HOLD_BRD () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_BRD;
		}
	};

	struct SNET_VEHICLE_REQ_HOLD_TO_SLOT
	{
		NET_MSG_GENERIC		nmg;

		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_HOLD_TO_SLOT () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT;
		}
	};

	struct SNET_VEHICLE_REQ_HOLD_TO_SLOT_FB
	{
		NET_MSG_GENERIC		nmg;

		EMVEHICLE_REQ_SLOT_EX_HOLD_FB emFB;
		SITEMCUSTOM				  sItemCustom;
		EMSUIT					  emSuit;

		SNET_VEHICLE_REQ_HOLD_TO_SLOT_FB () 
			: emSuit(SUIT_NSIZE)
			, emFB(EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT_FB;
		}
	};

	struct SNET_VEHICLE_REQ_SLOT_TO_HOLD
	{
		NET_MSG_GENERIC		nmg;

		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_SLOT_TO_HOLD () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD;
		}
	};

	struct SNET_VEHICLE_REQ_SLOT_TO_HOLD_FB
	{
		NET_MSG_GENERIC		nmg;

		EMVEHICLE_REQ_SLOT_EX_HOLD_FB emFB;
		SITEMCUSTOM				  sItemCustom;
		EMSUIT					  emSuit;

		SNET_VEHICLE_REQ_SLOT_TO_HOLD_FB () 
			: emSuit(SUIT_NSIZE)
			, emFB(EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD_FB;
		}
	};

	struct SNET_VEHICLE_REQ_REMOVE_SLOTITEM
	{
		NET_MSG_GENERIC		nmg;

		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_REMOVE_SLOTITEM () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REMOVE_SLOTITEM;
		}
	};

	struct SNET_VEHICLE_REQ_REMOVE_SLOTITEM_FB
	{
		NET_MSG_GENERIC		nmg;

		EMSUIT				emSuit;

		SNET_VEHICLE_REQ_REMOVE_SLOTITEM_FB () 
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REMOVE_SLOTITEM_FB;
		}
	};

	struct SNET_VEHICLE_REQ_REMOVE_SLOTITEM_BRD : public SNETPC_BROAD
	{
		EMSUIT			emSuit;

		SNET_VEHICLE_REQ_REMOVE_SLOTITEM_BRD ()
			: emSuit(SUIT_NSIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REMOVE_SLOTITEM_BRD;
		}
	};

	struct SNET_VEHICLE_UPDATE_CLIENT_BATTERY
	{
		NET_MSG_GENERIC		nmg;

		int					nFull;

		SNET_VEHICLE_UPDATE_CLIENT_BATTERY () 
			: nFull(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_UPDATE_CLIENT_BATTERY;
		}

	};

	struct SNET_VEHICLE_REQ_GIVE_BATTERY
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		SNET_VEHICLE_REQ_GIVE_BATTERY () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_GIVE_BATTERY;
		}

	};


	struct SNET_VEHICLE_REQ_GIVE_BATTERY_FB
	{
		NET_MSG_GENERIC				nmg;

		DWORD						dwVehicleID;
		SNATIVEID					sItemID;
		SNATIVEID					sBatteryID;
		int							nFull;

		EMVEHICLE_REQ_GIVE_BATTERY_FB		emFB;

		SNET_VEHICLE_REQ_GIVE_BATTERY_FB () :
			dwVehicleID(0),
			sItemID(NATIVEID_NULL()),
			sBatteryID(NATIVEID_NULL()),
			nFull(0),
			emFB(EMVEHICLE_REQ_GIVE_BATTERY_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_GIVE_BATTERY_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_VEHICLE_REQ_GET_BATTERY_FROMDB_FB
	{
		NET_MSG_GENERIC					nmg;

		DWORD							dwVehicleID;
		SNATIVEID						sItemID;
		int								nFull;
		WORD							wPosX;
		WORD							wPosY;
		WORD							wCureVolume;
		BOOL							bRatio;
		SNATIVEID						sBatteryID;
		EMVEHICLE_REQ_GIVE_BATTERY_FB	emFB;
		VEHICLE_TYPE					emType;

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC)+sizeof(DWORD)+sizeof(SNATIVEID)+sizeof(int)+sizeof(WORD)*3+sizeof(BOOL)+sizeof(SNATIVEID)+sizeof(EMVEHICLE_REQ_GIVE_BATTERY_FB)+sizeof(VEHICLE_TYPE) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNET_VEHICLE_REQ_GET_BATTERY_FROMDB_FB () :
			dwVehicleID(0),
			sItemID( NATIVEID_NULL() ),
			nFull(0),
			wPosX(0),
			wPosY(0),
			wCureVolume(0),
			bRatio(false),
			sBatteryID(NATIVEID_NULL()),
			emFB(EMVEHICLE_REQ_GIVE_BATTERY_FB_FAIL),
			emType( VEHICLE_TYPE_NONE )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_GET_BATTERY_FROMDB_FB;
		}
	};


	struct SNET_VEHICLE_REQ_ITEM_INFO
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwVehicleID;
		bool				bTrade;

		SNET_VEHICLE_REQ_ITEM_INFO () :
			dwVehicleID(0),
			bTrade(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_ITEM_INFO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_VEHICLE_REQ_ITEM_INFO_FB
	{
		NET_MSG_GENERIC		nmg;

		VEHICLE_TYPE		emTYPE;
		int					nFull;
		SITEMCUSTOM			PutOnItems[VEHICLE_ACCETYPE_SIZE];
		DWORD				dwVehicleID;
		bool				bTrade;
		bool				bBooster; /*vehicle booster system, Juver, 2017/08/10 */
		WORD				wColor[BIKE_COLOR_SLOT_PART_SIZE]; /*bike color , Juver, 2017/11/13 */


		SNET_VEHICLE_REQ_ITEM_INFO_FB () :
			emTYPE(VEHICLE_TYPE_NONE),
			nFull(0),
			dwVehicleID(0),
			bTrade(false),
			bBooster(false) /*vehicle booster system, Juver, 2017/08/10 */
		{
			/*bike color , Juver, 2017/11/13 */
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_ITEM_INFO_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_NON_REBIRTH_REQ
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bNon_Rebirth;

		SNET_NON_REBIRTH_REQ () :
			bNon_Rebirth(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_NON_REBIRTH_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNET_QBOX_OPTION_REQ_AG
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bQBoxEnable;

		SNET_QBOX_OPTION_REQ_AG () :
			bQBoxEnable(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_QBOX_OPTION_REQ_AG;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_QBOX_OPTION_REQ_FLD
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bQBoxEnable;
		DWORD				dwPartyID;		

		SNET_QBOX_OPTION_REQ_FLD () :
			bQBoxEnable(FALSE),
			dwPartyID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_QBOX_OPTION_REQ_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_QBOX_OPTION_MEMBER
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bQBoxEnable;

		SNET_QBOX_OPTION_MEMBER () :
		bQBoxEnable(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_QBOX_OPTION_MEMBER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_OPEN_ITEMSHOP
	{
		NET_MSG_GENERIC		nmg;

		bool				bOpen;

		SNETPC_OPEN_ITEMSHOP () :
		bOpen(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ITEMSHOPOPEN;
		}	
	};

	struct SNETPC_OPEN_ITEMSHOP_BRD : public SNETPC_BROAD
	{
		bool				bOpen;			

		SNETPC_OPEN_ITEMSHOP_BRD () :
		bOpen(0)		
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_ITEMSHOPOPEN_BRD;
		}	
	};

	struct SNETPC_REQ_ATTENDLIST
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_REQ_ATTENDLIST ()
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ATTENDLIST);
			nmg.nType = NET_MSG_REQ_ATTENDLIST;
		}
	};

	struct SNETPC_REQ_ATTENDLIST_FB
	{
		enum { EMMAXATTEND = 31 };

		NET_MSG_GENERIC		nmg;
		DWORD				dwAttenNum;
		DWORD				dwAttendCombo;
		
		__time64_t			tLoginTime;
		DWORD				dwAttendTime;

		USER_ATTEND_INFO 	sAttend[EMMAXATTEND];

		SNETPC_REQ_ATTENDLIST_FB ()
			: dwAttenNum(0)
			, dwAttendCombo(0)
			, tLoginTime(0)
			, dwAttendTime(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(DWORD) + sizeof(DWORD) +
						 sizeof(__time64_t) + sizeof(DWORD);
			nmg.nType = NET_MSG_REQ_ATTENDLIST_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}		

		void SetSize( int nSize )
		{
			dwAttenNum = nSize;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(DWORD) + sizeof(DWORD) +
						 sizeof(__time64_t) + sizeof(DWORD) +
						 sizeof(USER_ATTEND_INFO)*dwAttenNum ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		void RESET ()
		{
			dwAttenNum = 0;
			dwAttendCombo = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(DWORD) + sizeof(DWORD) +
						 sizeof(__time64_t) + sizeof(DWORD);
		}
	};
	

	struct SNETPC_REQ_ATTENDANCE
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_REQ_ATTENDANCE ()
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ATTENDANCE);
			nmg.nType = NET_MSG_REQ_ATTENDANCE;
		}
	};

	struct SNETPC_REQ_ATTENDANCE_FB
	{
		NET_MSG_GENERIC		nmg;
		EMREQ_ATTEND_FB		emFB;

		__time64_t			tAttendTime;
		DWORD				dwComboAttend;
		bool				bAttendReward;

		SNETPC_REQ_ATTENDANCE_FB ()
			: emFB ( EMREQ_ATTEND_FB_FAIL )
			, tAttendTime ( 0 )
			, dwComboAttend ( 0 )
			, bAttendReward (false)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ATTENDANCE_FB);
			nmg.nType = NET_MSG_REQ_ATTENDANCE_FB;
		}
	};

	struct SNETPC_REQ_ATTEND_REWARD_FLD
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			idAttendReward;

		SNETPC_REQ_ATTEND_REWARD_FLD ()
			: idAttendReward ( NATIVEID_NULL() )
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ATTEND_REWARD_FLD);
			nmg.nType = NET_MSG_REQ_ATTEND_REWARD_FLD;
		}
	};

	struct SNETPC_REQ_ATTEND_REWARD_CLT
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			idAttendReward;

		SNETPC_REQ_ATTEND_REWARD_CLT ()
			: idAttendReward ( NATIVEID_NULL() )
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ATTEND_REWARD_CLT);
			nmg.nType = NET_MSG_REQ_ATTEND_REWARD_CLT;
		}
	};


	struct SNETPC_REQ_GATHERING
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwTargetID;

		SNETPC_REQ_GATHERING ()
			: dwTargetID(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_GATHERING);
			nmg.nType = NET_MSG_REQ_GATHERING;
		}
	};

	struct SNETPC_REQ_GATHERING_FB
	{
		NET_MSG_GENERIC		nmg;
		EMREQ_GATHER_FB		emFB;
		DWORD				dwTime;
		DWORD				dwAniSel;
		DWORD				dwGaeaID;

		SNETPC_REQ_GATHERING_FB ()
			: emFB ( EMREQ_GATHER_FB_FAIL )
			, dwTime ( 0 )
			, dwGaeaID ( GAEAID_NULL )
			, dwAniSel ( 0 )
		{
			nmg.dwSize = sizeof(SNETPC_REQ_GATHERING_FB);
			nmg.nType = NET_MSG_REQ_GATHERING_FB;
		}
	};

	struct SNETPC_REQ_GATHERING_BRD : public SNETPC_BROAD
	{
		DWORD				dwAniSel;
		
		SNETPC_REQ_GATHERING_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_REQ_GATHERING_BRD;
		}
	};

	struct SNETPC_REQ_GATHERING_RESULT
	{
		NET_MSG_GENERIC		nmg;
		EMREQ_GATHER_RESULT	emFB;

		SNETPC_REQ_GATHERING_RESULT ()
			: emFB ( EMREQ_GATHER_RESULT_FAIL )			
		{
			nmg.dwSize = sizeof(SNETPC_REQ_GATHERING_RESULT);
			nmg.nType = NET_MSG_REQ_GATHERING_RESULT;
		}
	};

	struct SNETPC_REQ_GATHERING_RESULT_BRD : public SNETPC_BROAD
	{
		EMREQ_GATHER_RESULT	emFB;

		SNETPC_REQ_GATHERING_RESULT_BRD ()
			: emFB ( EMREQ_GATHER_RESULT_FAIL )			
		{
			nmg.dwSize = sizeof(SNETPC_REQ_GATHERING_RESULT_BRD);
			nmg.nType = NET_MSG_REQ_GATHERING_RESULT_BRD;
		}
	};

	struct SNETPC_REQ_GATHERING_CANCEL
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_REQ_GATHERING_CANCEL ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_REQ_GATHERING_CANCEL;
		}
	};

	struct SNETPC_REQ_GATHERING_CANCEL_BRD : public SNETPC_BROAD
	{
		SNETPC_REQ_GATHERING_CANCEL_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_REQ_GATHERING_CANCEL_BRD;
		}
	};

	struct SNETPC_LANDEFFECT : public SNETPC_BROAD
	{
		SLANDEFFECT		sLandEffect[EMLANDEFFECT_MULTI];

		SNETPC_LANDEFFECT ()
		{
			nmg.dwSize = sizeof(SNETPC_LANDEFFECT);
			nmg.nType = NET_MSG_GCTRL_LANDEFFECT;

			for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
				sLandEffect[i].Init();
		}

	};

	//fastrevive fix
	struct SNETPC_REQ_REVIVE_DEATHTIMER_FB
	{
		NET_MSG_GENERIC			nmg;
		SNETPC_REQ_REVIVE_DEATHTIMER_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_DEATHTIMER_FB_FLD;
		}
	};

	struct SNETPC_DROPCHAR_TOAGENT
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwCHARID;
		bool				bCheat;
		int					nCheatType;

		SNETPC_DROPCHAR_TOAGENT () :
			dwCHARID(0),
			bCheat(false),
			nCheatType(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_DROPCHAR_TOAGENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*itemfood system, Juver, 2017/05/25 */
	struct SNETPC_FITEMFACT_BRD : public SNETPC_BROAD
	{
		SNATIVEID	sidSkill;
		WORD		wSLOT;
		SFITEMFACT	sFACT;

		SNETPC_FITEMFACT_BRD ()
			: sidSkill(NATIVEID_NULL())
			, wSLOT( FITEMFACT_SIZE )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_FITEMFACT_BRD;
		}
	};

	/*combatpoint logic, Juver, 2017/05/28 */
	struct SNETPC_UPDATE_CP
	{
		NET_MSG_GENERIC		nmg;
		WORD				wNowCP;
		BOOL				bReset;
		BOOL				bDie;

		SNETPC_UPDATE_CP() :
			wNowCP(0)
			, bReset(FALSE)
			, bDie(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_CP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*game stats, Juver, 2017/06/21 */
	struct SNETPC_PING_PACKET
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PING_PACKET () 
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_PING_PACKET);
			nmg.nType = NET_MSG_GCTRL_PING_PACKET;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*game stats, Juver, 2017/06/21 */
	struct SNETPC_PING_PACKET_FB
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwType;

		SNETPC_PING_PACKET_FB()
			: dwType(0)
		{
			nmg.dwSize = sizeof( SNETPC_PING_PACKET_FB );
			nmg.nType = NET_MSG_GCTRL_PING_PACKET_FB;
			GASSERT( nmg.dwSize <= NET_DATA_BUFSIZE );
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNET_VEHICLE_REQ_ENABLE_BOOSTER
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		SNET_VEHICLE_REQ_ENABLE_BOOSTER () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNET_VEHICLE_REQ_ENABLE_BOOSTER_FB
	{
		NET_MSG_GENERIC				nmg;

		DWORD						dwVehicleID;
		SNATIVEID					sItemID;
		SNATIVEID					sCardID;

		EMVEHICLE_REQ_ENABLE_BOOSTER_FB		emFB;

		SNET_VEHICLE_REQ_ENABLE_BOOSTER_FB () :
			dwVehicleID(0),
			sItemID(false),
			sCardID(false),
			emFB(EMVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER
	{
		NET_MSG_GENERIC		nmg;
		bool				bStart;

		SNETPC_REQ_VEHICLE_BOOSTER () 
			: bStart(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_VEHICLE_BOOSTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER_STATE_CHARGE
	{
		NET_MSG_GENERIC		nmg;
		bool		bStart;
		float		fTime;

		SNETPC_REQ_VEHICLE_BOOSTER_STATE_CHARGE () :
			bStart(false),
			fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_CHARGE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER_STATE_START
	{
		NET_MSG_GENERIC		nmg;
		bool		bStart;
		float		fTime;

		SNETPC_REQ_VEHICLE_BOOSTER_STATE_START () :
			bStart(false),
			fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER_STATE_START_BRD : public SNETPC_BROAD
	{
		bool				bStart;

		SNETPC_REQ_VEHICLE_BOOSTER_STATE_START_BRD ()
			: bStart(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*vehicle booster system, Juver, 2017/08/10 */
	struct SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET_BRD : public SNETPC_BROAD
	{
		SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*contribution point, Juver, 2017/08/23 */
	struct SNETPC_UPDATE_CONTRIBUTION_POINT
	{
		NET_MSG_GENERIC		nmg;
		LONGLONG			llPoint;
		bool				bNotice;

		SNETPC_UPDATE_CONTRIBUTION_POINT ()
			: llPoint(0)
			, bNotice(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_CONTRIBUTION_POINT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*activity point, Juver, 2017/08/23 */
	struct SNETPC_UPDATE_ACTIVITY_POINT
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwPoint;
		bool				bNotice;

		SNETPC_UPDATE_ACTIVITY_POINT ()
			: dwPoint(0)
			, bNotice(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_ACTIVITY_POINT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	/*event map move, Juver, 2017/08/25 */
	struct SNETPC_REQ_EVENT_MOVEMAP_FB
	{
		NET_MSG_GENERIC		nmg;

		EMEVENT_MOVEMAP_FB emFB;

		SNETPC_REQ_EVENT_MOVEMAP_FB () :
			emFB(EMEVENT_MOVEMAP_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_EVENT_MOVEMAP_FB;
		}
	};

	/*event map move, Juver, 2017/08/25 */
	struct SNETPC_REQ_EVENT_MOVEMAP_FLD
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	nidMAP;
		DWORD		dwCharID;
		WORD		wPosX;
		WORD		wPosY;
		BOOL		bCurMapCheck;
		BOOL		bCalculatePos;

		SNETPC_REQ_EVENT_MOVEMAP_FLD () :
			nidMAP(false),
			dwCharID(GAEAID_NULL),
			wPosX(0),
			wPosY(0),
			bCurMapCheck(TRUE),
			bCalculatePos(TRUE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_EVENT_MOVEMAP_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*event map move, Juver, 2017/08/25 */
	struct SNETPC_GM_MOVE2_MAPPOS
	{
		NET_MSG_GENERIC			nmg;
		SNATIVEID	nidMAP;
		WORD		wPosX;
		WORD		wPosY;

		SNETPC_GM_MOVE2_MAPPOS () :
			nidMAP(false),
			wPosX(0),
			wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GM_MOVE2_MAPPOS;
		}
	};

	/*system buffs, Juver, 2017/09/05 */
	struct SNETPC_SYSTEMBUFF_BRD : public SNETPC_BROAD
	{
		SDROP_SYSTEM_BUFF	sSYSTEM_BUFF[SYSTEM_BUFF_SIZE];

		SNETPC_SYSTEMBUFF_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SYSTEM_BUFF_BRD;
		}
	};

	/*charinfoview , Juver, 2017/11/11 */
	struct SNETPC_REQ_CHARINFO
	{
		NET_MSG_GENERIC		nmg;
		DWORD dwGaeaID;

		SNETPC_REQ_CHARINFO () :
		dwGaeaID( GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_CHARINFO;
		}
	};


	/*charinfoview , Juver, 2017/11/11 */
	struct SNETPC_REQ_CHARINFO_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_CHARINFO_FB		emFB;
		SINFO_CHAR				sCharInfo;

		SNETPC_REQ_CHARINFO_FB () :
		emFB(EMREQ_CHARINFO_FB_OK)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_CHARINFO_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*charinfoview , Juver, 2017/11/12 */
	struct SNET_PRIVATE_STATS_REQ
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bPrivateStats;

		SNET_PRIVATE_STATS_REQ () :
			bPrivateStats(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_PRIVATE_STATS_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*bike color , Juver, 2017/11/13 */
	struct SNET_VEHICLE_REQ_CHANGE_COLOR
	{
		NET_MSG_GENERIC			nmg;

		WORD		wColor[BIKE_COLOR_SLOT_PART_SIZE];

		SNET_VEHICLE_REQ_CHANGE_COLOR () 
		{
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_COLOR;
		}
	};

	/*bike color , Juver, 2017/11/13 */
	struct SNET_VEHICLE_REQ_CHANGE_COLOR_FB
	{
		NET_MSG_GENERIC			nmg;

		EMREQ_CHANGE_BIKE_COLOR_FB	emFB;
		WORD		wColor[BIKE_COLOR_SLOT_PART_SIZE];
		DWORD		dwVehicleID;

		SNET_VEHICLE_REQ_CHANGE_COLOR_FB () :
			dwVehicleID(0),
			emFB(EMREQ_CHANGE_BIKE_COLOR_FB_CHANGE_FAIL)
		{
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_COLOR_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*bike color , Juver, 2017/11/13 */
	struct SNET_VEHICLE_REQ_CHANGE_COLOR_BRD : public SNETPC_BROAD
	{
		WORD		wColor[BIKE_COLOR_SLOT_PART_SIZE];
		DWORD		dwVehicleID;

		SNET_VEHICLE_REQ_CHANGE_COLOR_BRD () :
			dwVehicleID(0)
		{
			for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				wColor[i] = 0;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_COLOR_BRD;
		}
	};

	/* car, cart color, Juver, 2018/02/14 */
	struct SNET_VEHICLE_REQ_CHANGE_CAR_COLOR
	{
		NET_MSG_GENERIC			nmg;
		WORD		wColorA;
		WORD		wColorB;

		SNET_VEHICLE_REQ_CHANGE_CAR_COLOR () 
			: wColorA(0)
			, wColorB(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR;
		}
	};

	/* car, cart color, Juver, 2018/02/14 */
	struct SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_FB
	{
		NET_MSG_GENERIC			nmg;

		EMREQ_CHANGE_CAR_COLOR_FB	emFB;
		WORD		wColorA;
		WORD		wColorB;
		DWORD		dwVehicleID;

		SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_FB ()
			: dwVehicleID(0)
			, wColorA(0)
			, wColorB(0)
			, emFB(EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* car, cart color, Juver, 2018/02/14 */
	struct SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD : public SNETPC_BROAD
	{
		WORD		wColorA;
		WORD		wColorB;
		DWORD		dwVehicleID;

		SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD ()
			: dwVehicleID(0)
			, wColorA(0)
			, wColorB(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD;
		}
	};

	/* booster all vehicle, Juver, 2018/02/14 */
	struct SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER
	{
		NET_MSG_GENERIC		nmg;

		WORD				wPosX;
		WORD				wPosY;

		SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

	};

	/* booster all vehicle, Juver, 2018/02/14 */
	struct SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB
	{
		NET_MSG_GENERIC				nmg;

		DWORD						dwVehicleID;
		SNATIVEID					sItemID;
		SNATIVEID					sCardID;

		EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB		emFB;

		SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB () :
		dwVehicleID(0),
			sItemID(false),
			sCardID(false),
			emFB(EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_DISTRADE_REQ
	{
		NET_MSG_GENERIC		nmg;

		BOOL				bDisTrade;

		SNET_DISTRADE_REQ () :
		bDisTrade(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_DISTRADE_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_HP_CHECK
	{
		NET_MSG_GENERIC		nmg;
		GLPADATA			m_sClientHP;
		WORD				m_wClientLevel;
		WORD				m_wLastHPCounter;

		SNETPC_HP_CHECK ():
		m_wClientLevel(0),
			m_wLastHPCounter(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_HP_CHECK;
		}
	};

	/*hackshield implementation, Juver, 2018/06/19 */
	struct SNETPC_HS_CALLBACK
	{
		NET_MSG_GENERIC		nmg;
		int			type;
		char		info[MAX_PATH];
		char		file[MAX_PATH];

		SNETPC_HS_CALLBACK () : 
		type(0)
		{
			memset (info, 0, sizeof(char) * MAX_PATH);
			memset (file, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_HS_CALLBACK;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*hackshield implementation, Juver, 2018/06/19 */
	struct SNETPC_HS_HEARTBEAT
	{
		NET_MSG_GENERIC		nmg;
		DWORD hs_sdk_version;

		SNETPC_HS_HEARTBEAT () 
			: hs_sdk_version(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_HS_HEARTBEAT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*hackshield implementation, Juver, 2018/06/21 */
	struct SNETPC_HS_CLOSE_CLIENT
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_HS_CLOSE_CLIENT ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_HS_CLOSE_CLIENT;
		}
	};

	// Revert to default structure packing
	#pragma pack()
};
