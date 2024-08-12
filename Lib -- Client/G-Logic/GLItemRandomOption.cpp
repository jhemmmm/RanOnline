#include "stdafx.h"
#include "GLItemRandomOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{
	BOOL SRANDOM_OPT::LOAD ( basestream &SFile )
	{
		SFile.ReadBuffer ( szNAME, SRANDOM_DATA::NAME_LEN );
		return TRUE;
	}

	BOOL SRANDOM_OPT::SAVE ( CSerialFile &SFile )
	{
		SFile.WriteBuffer ( szNAME, SRANDOM_DATA::NAME_LEN );

		return TRUE;
	}

	VOID SRANDOM_OPT::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "szNAME" << ",";
	}

	VOID SRANDOM_OPT::SaveCsv ( std::fstream &SFile )
	{
		if( strlen( szNAME ) < 1 )
			SFile << " " << ",";
		else
			SFile << szNAME << ",";
	}

	VOID SRANDOM_OPT::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		DWORD dwSize = sizeof(char)*SRANDOM_DATA::NAME_LEN;
		memset( szNAME, 0, dwSize );

		if( strlen( StrArray[ iCsvCur ] ) > 1 && StrArray[ iCsvCur ].GetAt( 0 ) != ' ' )
			StringCchCopy( szNAME, dwSize, StrArray[ iCsvCur ] );

		++iCsvCur;
	}
};