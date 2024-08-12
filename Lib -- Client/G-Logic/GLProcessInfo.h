/*!
 * \file GLProcessInfo.h
 *
 * \author Juver
 * \date June 2017
 *
 * get process command
 */
#ifndef GLPROCESS_INFO_H_
#define GLPROCESS_INFO_H_

#if _MSC_VER > 1000
#pragma once
#endif

#if _MSC_VER >= 1600
#include <unordered_map>
#endif

#include <map>

#define SPROCESS_DATA_EXE_NAME_SIZE 64
#define SPROCESS_DATA_MAX			200

struct SPROCESS_INFO_DATA
{
	char szExeName[SPROCESS_DATA_EXE_NAME_SIZE];
	unsigned int nprocid;
	int nRefCount;

	SPROCESS_INFO_DATA()
		: nprocid(0)
		, nRefCount(0)
	{
		SecureZeroMemory( szExeName, SPROCESS_DATA_EXE_NAME_SIZE );
	};
};


#if _MSC_VER >= 1600
typedef std::unordered_map<std::string,SPROCESS_INFO_DATA>	PROCESS_INFO_DATA_MAP;
typedef PROCESS_INFO_DATA_MAP::iterator						PROCESS_INFO_DATA_MAP_ITER;
#else
typedef std::map<std::string,SPROCESS_INFO_DATA>			PROCESS_INFO_DATA_MAP;
typedef PROCESS_INFO_DATA_MAP::iterator						PROCESS_INFO_DATA_MAP_ITER;
#endif

struct SPROCESS_INFO_DATA_CLIENT
{
	PROCESS_INFO_DATA_MAP mapData;
	DWORD dwTOTAL;
	
	SPROCESS_INFO_DATA_CLIENT()
		: dwTOTAL(0)
	{

	}
};

#if _MSC_VER >= 1600
typedef std::unordered_map<std::string,SPROCESS_INFO_DATA_CLIENT>	PROCESS_INFO_DATA_MAP_CLIENT;
typedef PROCESS_INFO_DATA_MAP_CLIENT::iterator						PROCESS_INFO_DATA_MAP_CLIENT_ITER;
#else
typedef std::map<std::string,SPROCESS_INFO_DATA_CLIENT>				PROCESS_INFO_DATA_MAP_CLIENT;
typedef PROCESS_INFO_DATA_MAP_CLIENT::iterator						PROCESS_INFO_DATA_MAP_CLIENT_ITER;
#endif

#endif // GLPROCESS_INFO_H_