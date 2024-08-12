#ifndef GLITEM_RANDOMOPTION_H_
#define GLITEM_RANDOMOPTION_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib -- Engine/Common/BaseStream.h"
#include "../../Lib -- Engine/Common/StringFile.h"
#include "../../Lib -- Engine/Common/SerialFile.h"
#include "../../Lib -- Engine/Common/STRINGUTILS.h"
#include "../../Lib -- Engine/G-Logic/GLDefine.h"

#include "./GLItemDef.h"

namespace ITEM
{
	struct SRANDOM_OPT
	{
		enum { VERSION = 0x0100 };

		char	szNAME[SRANDOM_DATA::NAME_LEN];

		SRANDOM_OPT ()
		{
			memset(szNAME, 0, sizeof(char) * SRANDOM_DATA::NAME_LEN);
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray, int &iCsvCur );
	};
};

#endif // GLITEM_RANDOMOPTION_H_