#ifndef GLITEM_GENERATE_H_
#define GLITEM_GENERATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib -- Engine/Common/BaseStream.h"
#include "../../Lib -- Engine/Common/StringFile.h"
#include "../../Lib -- Engine/Common/SerialFile.h"
#include "../../Lib -- Engine/Common/STRINGUTILS.h"
#include "../../Lib -- Engine/G-Logic/GLDefine.h"


namespace ITEM
{
	struct SGENERATE_100
	{
		DWORD dwSpecID;			//	아이템 분류 군.
		DWORD dwLimitTime;		//	생성 기준 시간.
		DWORD dwLimitTimeGen;	//	시간당 생성 제한 갯수.
	};

	struct SGENERATE_101
	{
		DWORD dwSpecID;			//	아이템 분류 군.
		DWORD dwLimitTimeGen;	//	시간당 생성 제한 갯수.
		DWORD dwLimitTime;		//	생성 기준 시간.
	};

	struct SGENERATE
	{
		enum { VERSION = 0x0200, SPECID_NULL = 0, LIMIT_NOUSE = 0 };

		DWORD dwSpecID;			//	아이템 분류 군.
		DWORD dwLimitTimeGen;	//	시간당 생성 제한 갯수.
		DWORD dwLimitTime;		//	생성 기준 시간.
		BOOL bGenEnable;

		SGENERATE ()
			: dwSpecID(SPECID_NULL)
			, dwLimitTimeGen(LIMIT_NOUSE)
			, dwLimitTime(LIMIT_NOUSE)
			, bGenEnable(TRUE)
		{
		}

		void Assign ( SGENERATE_100 &sOld );
		void Assign ( SGENERATE_101 &sOld );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray, int &iCsvCur  );
	};
};

#endif // GLITEM_GENERATE_H_