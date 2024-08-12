#ifndef GLITEM_BOX_H_
#define GLITEM_BOX_H_

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
	struct SBOX_ITEM_101
	{
		SNATIVEID	nidITEM;
		DWORD		dwAMOUNT;

		SBOX_ITEM_101 ()
			: nidITEM(false)
			, dwAMOUNT(1)
		{
		}
	};

	struct SBOX_ITEM
	{
		DWORD		dwAMOUNT;
		SNATIVEID	nidITEM;

		SBOX_ITEM ()
			: dwAMOUNT(1)
			, nidITEM(false)
		{
		}
	};

	struct SBOX
	{
		enum { VERSION = 0x0103, ITEM_SIZE = 10, };

		SBOX_ITEM	sITEMS[ITEM_SIZE];
		bool		bData;

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray, int &iCsvCur  );

		SBOX ();
		void CLEAR ();
		bool VALID ();

		bool INSERT ( const SNATIVEID &nidITEM, DWORD dwAMOUNT );
		bool DEL ( int nIndex );
	};

};

#endif // GLITEM_BOX_H_