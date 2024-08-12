#pragma once
#include "GLContrlBaseMsg.h"

namespace GLMSG
{
	#pragma pack(1)
	
	
	//dmk14 | 11-4-16 | pk ranking
	struct SNET_MSG_REQ_PLAYERRANKING
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwMapID;		
		
		SNET_MSG_REQ_PLAYERRANKING()
			: dwMapID(UINT_MAX)
		{
			nmg.dwSize = sizeof(SNET_MSG_REQ_PLAYERRANKING);
			nmg.nType = NET_MSG_GCTRL_REQ_PLAYERRANKING;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_REQ_PLAYERRANKING_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wRankNum;
		PLAYER_RANKING		sPlayerRank[RANKING_PKNUM];

		SNET_MSG_REQ_PLAYERRANKING_UPDATE()
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_REQ_PLAYERRANKING_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		bool ADDCHAR ( const PLAYER_RANKING& sRank )
		{
			if ( RANKING_PKNUM==wRankNum )		return false;

			sPlayerRank[wRankNum] = sRank;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(PLAYER_RANKING)*wRankNum;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			return true;
		}
		void RESET ()
		{
			wRankNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};

	struct SNET_MSG_REQ_PLAYERRANKING_UPDATE_EX
	{
		NET_MSG_GENERIC			nmg;
		
		PLAYER_RANKING_EX		sMyPKRank;		

		SNET_MSG_REQ_PLAYERRANKING_UPDATE_EX () 
		{
			nmg.dwSize = sizeof(SNET_MSG_REQ_PLAYERRANKING_UPDATE_EX);
			nmg.nType = NET_MSG_GCTRL_REQ_PLAYERRANKING_UPDATE_EX;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}        
	};


	// Revert to default structure packing
	#pragma pack()
};