#pragma once

#include "GLContrlBaseMsg.h"

namespace GLMSG
{
	#pragma pack(1)

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER
	{
		NET_MSG_GENERIC		nmg;
		
		DWORD				dwTargetID;

		SNET_PASSENGER () :
			dwTargetID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_FB
	{
		NET_MSG_GENERIC		nmg;
		
		DWORD				dwDriverID;
		DWORD				dwPassengerID;
		EMPASSENGER_ANS			emAns;
		VEHICLE_TYPE		emTYPE;
		SNATIVEID			sVehicleID;	

		SNET_PASSENGER_FB () :
			dwDriverID(0),
			dwPassengerID(0),
			emTYPE( VEHICLE_TYPE_NONE ),
			sVehicleID ( NATIVEID_NULL() ),
			emAns(EMPASSENGER_OK)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_BRD : public SNETPC_BROAD
	{
		DWORD				dwDriverID;
		DWORD				dwPassengerID;
		bool				bActive;
		VEHICLE_TYPE		emTYPE;
		SNATIVEID			sVehicleID;	

		SNET_PASSENGER_BRD () :
			dwDriverID(0),
			dwPassengerID(0),
			emTYPE( VEHICLE_TYPE_NONE ),
			sVehicleID ( NATIVEID_NULL() ),
			bActive(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_BRD;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_TAR
	{
		NET_MSG_GENERIC		nmg;
		
		DWORD				dwMasterID;

		SNET_PASSENGER_TAR () :
			dwMasterID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_TAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_TAR_ANS
	{
		NET_MSG_GENERIC		nmg;
		
		DWORD				dwMasterID;
		EMPASSENGER_ANS			emAns;

		SNET_PASSENGER_TAR_ANS () :
			dwMasterID(0),
			emAns(EMPASSENGER_OK)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_TAR_ANS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_CANCEL
	{
		NET_MSG_GENERIC		nmg;

		SNET_PASSENGER_CANCEL ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_CANCEL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_PASSENGER_CANCEL_TAR
	{
		NET_MSG_GENERIC		nmg;
		EMPASSENGER_CANCEL_TAR	emCancel;

		SNET_PASSENGER_CANCEL_TAR () :
			emCancel(EMPASSENGER_CANCEL_NORMAL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PASSENGER_CANCEL_TAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	// Revert to default structure packing
	#pragma pack()
};