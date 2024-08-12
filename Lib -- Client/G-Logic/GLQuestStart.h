#ifndef GLQUEST_START_H_
#define GLQUEST_START_H_

#if _MSC_VER > 1000
#pragma once
#endif


#include "../../Lib -- Engine/Common/BaseStream.h"
#include "../../Lib -- Engine/Common/StringFile.h"
#include "../../Lib -- Engine/Common/SerialFile.h"
#include "../../Lib -- Engine/Common/STRINGUTILS.h"
#include "../../Lib -- Engine/G-Logic/GLDefine.h"

#include "./GLCharDefine.h"
#include "./GLogicData.h"

struct GLQUEST_START
{
	enum { VERSION = 0x0003, };

	DWORD		dwCLASS;
	WORD		wSCHOOL;
	WORD		wLEVEL;
	SNATIVEID	nidSKILL;
	SNATIVEID	nidITEM;
	SNATIVEID	nidMAP;
	WORD		wSIZE;
	WORD		wPosX;
	WORD		wPosY;

	GLQUEST_START () :
		dwCLASS(GLCC_ALL_8CLASS),
		wSCHOOL(MAX_SCHOOL),
		wLEVEL(USHRT_MAX),
		nidSKILL(false),
		nidITEM(false),
		nidMAP(false),
		wSIZE(0),
		wPosX(USHRT_MAX),
		wPosY(USHRT_MAX)	
	{
	}

	bool LOAD ( basestream &SFile );
	bool LOAD_001( basestream &SFile );
	bool LOAD_002( basestream &SFile );
	bool LOAD_003( basestream &SFile );
	bool SAVE ( CSerialFile &SFile );
};


#endif // GLQUEST_START_H_