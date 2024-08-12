#include "stdafx.h"
#include "./GLItem.h"
#include "./GLItemMan.h"
#include "./GLogicData.h"
#include "./GLStringTable.h"

#include "../../Lib -- Engine/Common/StringUtils.h"
#include "../../Lib -- Engine/DxCommon/Collision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////
//	$1.	2003-01-17 Txx:xx	[최초 릴리즈]
////////////////////////////////////////////////////////////////////////////////////


BOOL SITEM::SaveFile ( CSerialFile &SFile )
{
	SFile << DWORD(VERSION);

	SFile << (DWORD)FILE_SBASIC;
	{
		SFile << DWORD(ITEM::SITEMBASIC::VERSION);

		SFile.BeginBlock();
		{
			sBasicOp.SAVE ( SFile );
		}
		SFile.EndBlock();
	}

	SFile << (DWORD)FILE_SSUIT;
	{
		SFile << DWORD(ITEM::SSUIT::VERSION);
		SFile << (DWORD)sizeof(ITEM::SSUIT);
		SFile.WriteBuffer ( &sSuitOp, sizeof(ITEM::SSUIT) );
	}

	SFile << (DWORD)FILE_SDRUG;
	{
		SFile << DWORD(ITEM::SDRUG::VERSION);
		SFile << (DWORD)sizeof(ITEM::SDRUG);
		SFile.WriteBuffer ( &sDrugOp, sizeof(ITEM::SDRUG) );
	}

	SFile << (DWORD)FILE_SSKILLBOOK;
	{
		SFile << DWORD(ITEM::SSKILLBOOK::VERSION);
		SFile << (DWORD)sizeof(ITEM::SSKILLBOOK);
		SFile.WriteBuffer ( &sSkillBookOp, sizeof(ITEM::SSKILLBOOK) );
	}

	SFile << SITEM::FILE_SGENERATE;
	{
		SFile << DWORD(ITEM::SGENERATE::VERSION);
		SFile << (DWORD)sizeof(ITEM::SGENERATE);
		SFile.WriteBuffer ( &sGenerateOp, sizeof(ITEM::SGENERATE) );
	}

	SFile << SITEM::FILE_SGRINDING;
	{
		SFile << DWORD(ITEM::SGRINDING::VERSION);

		SFile.BeginBlock();
		{
			sGrindingOp.SAVE ( SFile );
		}
		SFile.EndBlock();
	}

	SFile << SITEM::FILE_BOX;
	{
		sBox.SAVE ( SFile );
	}

	SFile << SITEM::FILE_RANDOMBOX;
	{
		sRandomBox.SAVE ( SFile );
	}

	SFile << SITEM::FILE_QUESTIONITEM;
	{
		sQuestionItem.SAVE ( SFile );
	}

	SFile << SITEM::FILE_RANDOMITEM;
	{
		sRandomOpt.SAVE ( SFile );
	}

	SFile << SITEM::FILE_PET;	// PetData
	{
		sPet.SAVE( SFile );
	}

	SFile << SITEM::FILE_VEHICLE;
	{
		sVehicle.SAVE( SFile );
	}

	SFile << SITEM::FILE_PETSKINPACK;
	{
		sPetSkinPack.SAVE( SFile );
	}

	/*rv card, Juver, 2017/11/25 */
	SFile << SITEM::FILE_RVCARD;
	sRvCard.SAVE( SFile );


	SFile << (DWORD)FILE_END_DATA;

	return TRUE;
}

BOOL SITEM::LoadFile ( basestream &SFile, bool bPastLoad  )
{
	DWORD dwVersion;
	SFile >> dwVersion;

	if ( dwVersion > VERSION )
	{
		CDebugSet::ErrorVersion( "SITEM::LoadFile", dwVersion );
		return FALSE;
	}

	DWORD dwDataType;
	DWORD dwVer, dwSize;

	SFile >> dwDataType;
	while ( dwDataType!=SITEM::FILE_END_DATA )
	{
		switch ( dwDataType )
		{
		case SITEM::FILE_SBASIC:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if( bPastLoad )
				{
					if ( dwVer==0x0100 )
					{
						ITEM::SITEMBASIC_100 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0101 )
					{
						ITEM::SITEMBASIC_101 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0102 )
					{
						ITEM::SITEMBASIC_102 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0103 )
					{
						sBasicOp.LOAD_103(SFile);
					}
					else if ( dwVer == 0x0104 )
					{
						sBasicOp.LOAD_104( SFile );
					}
					else if ( dwVer == 0x0105 )
					{
						sBasicOp.LOAD_105( SFile );
					}
					else if ( dwVer == 0x0106 )
					{
						sBasicOp.LOAD_106( SFile );
					}
					else if ( dwVer == 0x0107 )
					{
						sBasicOp.LOAD_107( SFile );
					}
					else if ( dwVer == 0x0108 )
					{
						sBasicOp.LOAD_108( SFile );
					}
					else if ( dwVer == 0x0109 )
					{
						sBasicOp.LOAD_109( SFile );
					}
					else if ( dwVer == 0x0110 )
					{
						sBasicOp.LOAD_110( SFile );
					}
					else if ( dwVer == 0x0111 )
					{
						sBasicOp.LOAD_111( SFile );
					}
					else if ( dwVer == 0x0112 )
					{
						sBasicOp.LOAD_112( SFile );
					}
					else if ( dwVer == 0x0113 )
					{
						sBasicOp.LOAD_113( SFile );
					}
					else if ( dwVer == 0x0114 )
					{
						sBasicOp.LOAD_114( SFile );
					}
					else if ( dwVer == 0x0115 )
					{
						sBasicOp.LOAD_115( SFile );
					}
					else if ( dwVer == 0x0116 )
					{
						sBasicOp.LOAD_116( SFile );
					}
					else if ( dwVer == 0x0117 )
					{
						sBasicOp.LOAD_117( SFile );
					}
					else if ( dwVer == 0x0118 )
					{
						sBasicOp.LOAD_118( SFile );
					}
					else if ( dwVer == 0x0119 )
					{
						sBasicOp.LOAD_119( SFile );
					}
					else if ( dwVer == 0x0121 )
					{
						sBasicOp.LOAD_121( SFile );
					}
					else if ( dwVer == 0x0122 )
					{
						sBasicOp.LOAD_122( SFile );
					}
					else if ( dwVer == 0x0200 )
					{
						sBasicOp.LOAD_200( SFile );
					}
					else if ( dwVer == 0x0201 )
					{
						sBasicOp.LOAD_201( SFile );
					}
					else if ( dwVer == 0x0202 )
					{
						sBasicOp.LOAD_202( SFile );
					}
					else if ( dwVer == 0x0203 )
					{
						sBasicOp.LOAD_203( SFile );
					}
					else if ( dwVer == 0x0204 )
					{
						sBasicOp.LOAD_204( SFile );
					}
					else if ( ITEM::SITEMBASIC::VERSION==dwVer )
					{
						sBasicOp.LOAD( SFile );
					}
					else
					{
						CDebugSet::ErrorVersion( "ITEM::SITEMBASIC PastLoad", dwVer );
						SFile.SetOffSet ( SFile.GetfTell()+dwSize );
					}
				}else{
					if ( ITEM::SITEMBASIC::VERSION==dwVer )
					{
						sBasicOp.LOAD( SFile );
					}
					else
					{
						CDebugSet::ErrorVersion( "ITEM::SITEMBASIC", dwVer );
						SFile.SetOffSet ( SFile.GetfTell()+dwSize );
						return FALSE;
					}
				}
			}break;

		case SITEM::FILE_SSUIT:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SSUIT_100 sSuit100;
					GASSERT(sizeof(ITEM::SSUIT_100)==dwSize);
					SFile.ReadBuffer ( &sSuit100, sizeof(ITEM::SSUIT_100) );

					sSuitOp.Assign ( sSuit100 );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SSUIT_101 sSuit101;
					GASSERT(sizeof(ITEM::SSUIT_101)==dwSize);
					SFile.ReadBuffer ( &sSuit101, sizeof(ITEM::SSUIT_101) );

					sSuitOp.Assign ( sSuit101 );
				}
				else if ( dwVer==0x0102 )
				{
					ITEM::SSUIT_102 sSuit102;
					GASSERT(sizeof(ITEM::SSUIT_102)==dwSize);
					SFile.ReadBuffer ( &sSuit102, sizeof(ITEM::SSUIT_102) );

					sSuitOp.Assign ( sSuit102 );
				}
				else if ( dwVer==0x0103 )
				{
					ITEM::SSUIT_103 sSuit103;
					GASSERT(sizeof(ITEM::SSUIT_103)==dwSize);
					SFile.ReadBuffer ( &sSuit103, sizeof(ITEM::SSUIT_103) );

					sSuitOp.Assign ( sSuit103 );
				}
				else if ( dwVer==0x0104 )
				{
					ITEM::SSUIT_104 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0105 )
				{
					ITEM::SSUIT_105 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0106 ) // By 경대
				{
					ITEM::SSUIT_106 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0107 )
				{
					ITEM::SSUIT_107 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0108 )
				{
					ITEM::SSUIT_108 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0109 )
				{
					ITEM::SSUIT_109 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0110 )
				{
					ITEM::SSUIT_110 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==ITEM::SSUIT::VERSION )
				{
					GASSERT(sizeof(ITEM::SSUIT)==dwSize);
					SFile.ReadBuffer ( &sSuitOp, sizeof(ITEM::SSUIT) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SSUIT", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SDRUG:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SDRUG_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SDRUG_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==ITEM::SDRUG::VERSION )
				{
					GASSERT(sizeof(ITEM::SDRUG)==dwSize);
					SFile.ReadBuffer ( &sDrugOp, sizeof(ITEM::SDRUG) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SDRUG", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SSKILLBOOK:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SSKILLBOOK_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sSkillBookOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SSKILLBOOK_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sSkillBookOp.Assign ( sOLD );
				}
				else if ( dwVer==ITEM::SSKILLBOOK::VERSION )
				{
					GASSERT(sizeof(ITEM::SSKILLBOOK)==dwSize);
					SFile.ReadBuffer ( &sSkillBookOp, sizeof(ITEM::SSKILLBOOK) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SSKILLBOOK", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SGENERATE:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SGENERATE_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sGenerateOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SGENERATE_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sGenerateOp.Assign ( sOLD );
				}
				else if ( dwVer==ITEM::SGENERATE::VERSION )
				{
					GASSERT(sizeof(ITEM::SGENERATE)==dwSize);
					SFile.ReadBuffer ( &sGenerateOp, sizeof(ITEM::SGENERATE) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SGENERATE", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SGRINDING:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if (ITEM::SGRINDING::VERSION==dwVer )
				{
					sGrindingOp.LOAD( SFile );
				}
				else if ( dwVer==0x0105 )
				{
					sGrindingOp.LOAD_0105( SFile );
				}
				else if ( dwVer==0x0104 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_104)==dwSize);

					ITEM::SGRINDING_104 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0103 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_103)==dwSize);

					ITEM::SGRINDING_103 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0102 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_102)==dwSize);

					ITEM::SGRINDING_102 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0101 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_101)==dwSize);

					ITEM::SGRINDING_101 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0100 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_100)==dwSize);

					ITEM::SGRINDING_100 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SGRINDING", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_BOX:
			{
				sBox.LOAD ( SFile );
			}break;

		case SITEM::FILE_RANDOMBOX:
			{
				sRandomBox.LOAD ( SFile );
			}break;

		case SITEM::FILE_QUESTIONITEM:
			{
				sQuestionItem.LOAD ( SFile );
			}break;

		case SITEM::FILE_RANDOMITEM:
			{
				sRandomOpt.LOAD ( SFile );
			}break;

		case SITEM::FILE_PET:	// PetData
			{
				sPet.LOAD( SFile );
			}break;

		case SITEM::FILE_VEHICLE:
			{
				sVehicle.LOAD( SFile );			
			}break;

		case SITEM::FILE_PETSKINPACK:
			{
				sPetSkinPack.LOAD( SFile );			
			}break;

			/*rv card, Juver, 2017/11/25 */
		case SITEM::FILE_RVCARD:
			{
				sRvCard.LOAD( SFile );			
			}break;

		default:
			{
				CDebugSet::ErrorCode( "SITEM::LoadFile", "Unknown Data Type:%d", dwDataType );

				SFile >> dwVer;
				SFile >> dwSize;
				SFile.SetOffSet ( SFile.GetfTell()+dwSize );
			}
			break;
		};

		SFile >> dwDataType;
	};

	return TRUE;
}

BOOL SITEM::isWrappable ()
{

           //if ( sBasicOp.dwReqActivityPoint != 0 ) return FALSE; //activity item
           //if ( sBasicOp.dwReqContributionPoint != 0 ) return FALSE; //contribution item
           if ( sBasicOp.IsEVENT() ) return FALSE; //event item
           if ( sBasicOp.IsDISGUISE() ) return FALSE; //costume
           if ( IsTIMELMT() ) return FALSE; //expiry
           if ( sBasicOp.sNativeID.wMainID == 76 ) return FALSE; //gm item
           if ( sBasicOp.emItemType != ITEM_SUIT &&
                      sBasicOp.emItemType != ITEM_VEHICLE ) return FALSE; //wear item


 if ( sSuitOp.emSuit != SUIT_HEADGEAR && 
		sSuitOp.emSuit != SUIT_UPPER &&
		sSuitOp.emSuit != SUIT_LOWER &&
		sSuitOp.emSuit != SUIT_HAND &&
		sSuitOp.emSuit != SUIT_FOOT &&
		sSuitOp.emSuit != SUIT_HANDHELD &&
		sSuitOp.emSuit != SUIT_VEHICLE &&
		sSuitOp.emSuit != SUIT_BELT &&
		sSuitOp.emSuit != SUIT_WING &&
		sSuitOp.emSuit != SUIT_FINGER &&
		sSuitOp.emSuit != SUIT_EARRING &&
		sSuitOp.emSuit != SUIT_ACCESSORY  &&
		sSuitOp.emSuit != SUIT_NECK  &&
		sSuitOp.emSuit != SUIT_WRIST  )		return FALSE;

	return TRUE;
}

VOID SITEM::SaveCsvHead ( std::fstream &SFile )
{
	ITEM::SITEMBASIC::SaveCsvHead( SFile );
	ITEM::SSUIT::SaveCsvHead( SFile );
	ITEM::SDRUG::SaveCsvHead( SFile );
	ITEM::SSKILLBOOK::SaveCsvHead( SFile );
	ITEM::SGRINDING::SaveCsvHead( SFile );
	ITEM::SGENERATE::SaveCsvHead( SFile );
	ITEM::SBOX::SaveCsvHead( SFile );
	ITEM::SRANDOMBOX::SaveCsvHead( SFile );
	ITEM::SQUESTIONITEM::SaveCsvHead( SFile );
	ITEM::SRANDOM_OPT::SaveCsvHead( SFile );
	ITEM::SPET::SaveCsvHead( SFile );	// PetData
	ITEM::SVEHICLE::SaveCsvHead( SFile );
	ITEM::SPETSKINPACKITEM::SaveCsvHead( SFile );
	ITEM::SRVCARD::SaveCsvHead( SFile ); /*rv card, Juver, 2017/11/25 */

	SFile << std::endl;
}

VOID SITEM::SaveCsv ( std::fstream &SFile )
{
	sBasicOp.SaveCsv( SFile );
	sSuitOp.SaveCsv( SFile );
	sDrugOp.SaveCsv( SFile );
	sSkillBookOp.SaveCsv( SFile );
	sGrindingOp.SaveCsv( SFile );
	sGenerateOp.SaveCsv( SFile );
	sBox.SaveCsv( SFile );
	sRandomBox.SaveCsv( SFile );
	sQuestionItem.SaveCsv( SFile );
	sRandomOpt.SaveCsv( SFile );
	sPet.SaveCsv( SFile );				// PetData
	sVehicle.SaveCsv( SFile );
	sPetSkinPack.SaveCsv( SFile );
	sRvCard.SaveCsv( SFile ); /*rv card, Juver, 2017/11/25 */

	SFile << std::endl;
}

VOID SITEM::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;

	sBasicOp.LoadCsv( StrArray, iCsvCur );
	sSuitOp.LoadCsv( StrArray, iCsvCur );
	sDrugOp.LoadCsv( StrArray, iCsvCur );
	sSkillBookOp.LoadCsv( StrArray, iCsvCur );
	sGrindingOp.LoadCsv( StrArray, iCsvCur );
	sGenerateOp.LoadCsv( StrArray, iCsvCur );
	sBox.LoadCsv( StrArray, iCsvCur );
	sRandomBox.LoadCsv( StrArray, iCsvCur );
	sQuestionItem.LoadCsv( StrArray, iCsvCur );
	sRandomOpt.LoadCsv( StrArray, iCsvCur );
	sPet.LoadCsv( StrArray, iCsvCur );	// PetData
	sVehicle.LoadCsv( StrArray, iCsvCur );
	sPetSkinPack.LoadCsv( StrArray, iCsvCur );
	sRvCard.LoadCsv( StrArray, iCsvCur ); /*rv card, Juver, 2017/11/25 */
}

WORD SITEM::GETAPPLYNUM () const
{
	switch ( sBasicOp.emItemType )
	{
	case ITEM_ARROW:
	case ITEM_CHARM:
	case ITEM_BULLET: /*gun-bullet logic, Juver, 2017/05/27 */

	case ITEM_FIRECRACKER:
	case ITEM_LOUDSPEAKER:
	case ITEM_TICKET:
	case ITEM_RECALL:
	case ITEM_STORAGE_CONNECT:
	case ITEM_DISJUNCTION:
	case ITEM_2FRIEND:
	case ITEM_REVIVE: // 귀혼주일 경우
	case ITEM_ANTI_DISAPPEAR:
	case ITEM_REMODEL:
	case ITEM_GARBAGE_CARD:
	case ITEM_TELEPORT_CARD:
		return sDrugOp.wCureVolume;
	};

	//	귀혼주일 경우.
	if ( sDrugOp.emDrug == ITEM_DRUG_CALL_REVIVE && sBasicOp.emItemType==ITEM_SUIT )
	{
		return sDrugOp.wCureVolume;
	}

	return 1;
}

SITEM& SITEM::operator = ( const SITEM& rvalue )
{
	if( this == &rvalue )
		return *this;

	sBasicOp = rvalue.sBasicOp;
	sSuitOp = rvalue.sSuitOp;
	sDrugOp = rvalue.sDrugOp;
	sSkillBookOp = rvalue.sSkillBookOp;
	sGrindingOp = rvalue.sGrindingOp;
	sGenerateOp = rvalue.sGenerateOp;
	sBox = rvalue.sBox;
	sRandomBox = rvalue.sRandomBox;
	sQuestionItem = rvalue.sQuestionItem;
	sRandomOpt = rvalue.sRandomOpt;
	sPet = rvalue.sPet;
	sVehicle = rvalue.sVehicle;
	sPetSkinPack = rvalue.sPetSkinPack;
	sRvCard = rvalue.sRvCard; /*rv card, Juver, 2017/11/25 */

	return *this;
}

const char* SITEM::GetName()
{
	const char* szName = GLStringTable::GetInstance().GetString( sBasicOp.strName.c_str(), GLStringTable::ITEM );
	if ( !szName )		return sBasicOp.strName.c_str();

	return szName;
}

const char* SITEM::GetComment()
{
	const char* szComment = GLStringTable::GetInstance().GetString( sBasicOp.strComment.c_str(), GLStringTable::ITEM );
	if ( !szComment )	return NULL;

	return szComment;
}

namespace
{
	BYTE CAST2GRADE ( int nVALUE, int nSTEP )
	{
		BYTE cGRADE(0);
		if ( nVALUE > 0 )	cGRADE += 1;
		cGRADE += BYTE ( (nVALUE-1)/nSTEP );

		if ( cGRADE>9 )		cGRADE = 9;
		return cGRADE;
	}

	BYTE CAST2GRADE_EX ( int nVALUE, int nSTEP1, int nSTEP2 )
	{
		nVALUE = nVALUE * nSTEP1;

		BYTE cGRADE(0);
		if ( nVALUE > 0 )	cGRADE += 1;
		cGRADE += BYTE ( (nVALUE-1)/nSTEP2 );

		if ( cGRADE>9 )		cGRADE = 9;
		return cGRADE;
	}
};

SITEMCUSTOM::SITEMCUSTOM ()
	: sNativeID(false)
	, nidDISGUISE(false)
	, tBORNTIME(0)
	, tDISGUISE(0)
	, lnGenNum(0)
	, wTurnNum(1)
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)
	
	, cOptTYPE1(0)
	, cOptTYPE2(0)
	, cOptTYPE3(0)
	, cOptTYPE4(0)
	
	, nOptVALUE1(0)
	, nOptVALUE2(0)
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	, dwPetID(0)
	, dwVehicleID( 0 )
	, bVietnamGainItem( false )

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

SITEMCUSTOM::SITEMCUSTOM ( SNATIVEID	sNID )
	: sNativeID(sNID)
	, nidDISGUISE(false)
	, tBORNTIME(0)
	, tDISGUISE(0)
	, lnGenNum(0)
	, wTurnNum(1)
	
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)
	
	, cOptTYPE1(0)
	, cOptTYPE2(0)
	, cOptTYPE3(0)
	, cOptTYPE4(0)
	
	, nOptVALUE1(0)
	, nOptVALUE2(0)
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	, dwPetID(0)
	, dwVehicleID( 0 )
	, bVietnamGainItem ( 0 )

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

void SITEMCUSTOM::ResetGrind ( EMGRINDING_TYPE emTYPE )
{
	switch ( emTYPE )
	{
	case EMGRINDING_NONE:
		cDAMAGE = (0);
		cDEFENSE = (0);

		cRESIST_FIRE = (0);
		cRESIST_ICE = (0);
		cRESIST_ELEC = (0);
		cRESIST_POISON = (0);
		cRESIST_SPIRIT = (0);
		break;

	case EMGRINDING_DAMAGE:
		cDAMAGE = (0);
		break;
	case EMGRINDING_DEFENSE:
		cDEFENSE = (0);
		break;

	case EMGRINDING_RESIST_FIRE:
		cRESIST_FIRE = (0);
		break;
	case EMGRINDING_RESIST_ICE:
		cRESIST_ICE = (0);
		break;
	case EMGRINDING_RESIST_ELEC:
		cRESIST_ELEC = (0);
		break;
	case EMGRINDING_RESIST_POISON:
		cRESIST_POISON = (0);
		break;
	case EMGRINDING_RESIST_SPIRIT:
		cRESIST_SPIRIT = (0);
		break;
	};
}

bool SITEMCUSTOM::operator == ( const SITEMCUSTOM &value )
{
	return !memcmp(this,&value,sizeof(SITEMCUSTOM));
}

bool SITEMCUSTOM::operator != ( const SITEMCUSTOM &value )
{
	return 0!=memcmp(this,&value,sizeof(SITEMCUSTOM));
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_100 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = EMGEN_OLD;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_101 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = EMGEN_OLD;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_102 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.dwGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_103 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_104 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_105 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE_EX ( sOld.cDAMAGE, 3, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE_EX ( sOld.cDEFENSE, 3, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE_EX ( sOld.cRESIST_FIRE, 3, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE_EX ( sOld.cRESIST_ICE, 3, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE_EX ( sOld.cRESIST_ELEC, 3, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE_EX ( sOld.cRESIST_POISON, 3, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE_EX ( sOld.cRESIST_SPIRIT, 3, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_106 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = 0;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_107 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = sOld.tDISGUISE;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	cChnID = 0;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_108 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = sOld.tDISGUISE;

	cGenType = sOld.cGenType;
	cChnID = sOld.cChnID;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;
	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_109 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_110 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_111 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_QUEST_115 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_112 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;
}

void SITEMCUSTOM::Assign ( const SITEM_LOBY &sOldLoby )
{
	sNativeID			= sOldLoby.sNativeID;	
	nidDISGUISE			= sOldLoby.nidDISGUISE;

	lnGenNum			= sOldLoby.lnGenNum;
	wTurnNum			= sOldLoby.wTurnNum;
	cGenType			= sOldLoby.cGenType;	
	cChnID				= sOldLoby.cChnID;		

	cFieldID			= sOldLoby.cFieldID;

	cDAMAGE				= sOldLoby.cDAMAGE;	
	cDEFENSE			= sOldLoby.cDEFENSE;	
	cRESIST_FIRE		= sOldLoby.cRESIST_FIRE;
	cRESIST_ICE			= sOldLoby.cRESIST_ICE;
	cRESIST_ELEC		= sOldLoby.cRESIST_ELEC;
	cRESIST_POISON		= sOldLoby.cRESIST_POISON;
	cRESIST_SPIRIT		= sOldLoby.cRESIST_SPIRIT;


    cOptTYPE1			= sOldLoby.cOptTYPE1;
	cOptTYPE2			= sOldLoby.cOptTYPE2;
	cOptTYPE3			= sOldLoby.cOptTYPE3;
	cOptTYPE4			= sOldLoby.cOptTYPE4;

	nOptVALUE1			= sOldLoby.nOptVALUE1;
	nOptVALUE2			= sOldLoby.nOptVALUE2;
	nOptVALUE3			= sOldLoby.nOptVALUE3;
	nOptVALUE4			= sOldLoby.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sOldLoby.wColor1;
	wColor2				= sOldLoby.wColor2;
}

void SITEMCUSTOM::Assign ( const SITEMCLIENT &sClient )
{
	nidDISGUISE = sClient.nidDISGUISE;
	sNativeID = sClient.sNativeID;
	wTurnNum = sClient.wTurnNum;

	if ( sClient.GETOptTYPE1() == EMR_OPT_MOVE_SPEED )
	{
		cOptTYPE1 = sClient.cOptTYPE1;
		nOptVALUE1 = sClient.nOptVALUE1;
	}

	/*item color, Juver, 2018/01/04 */
	wColor1				= sClient.wColor1;
	wColor2				= sClient.wColor2;
}

/*item link, Juver, 2017/07/31 */
void SITEMCUSTOM::Assign ( const SITEMLINK &sItemLink )
{
	sNativeID			= sItemLink.sNativeID;	
	nidDISGUISE			= sItemLink.nidDISGUISE;
	tBORNTIME			= sItemLink.tBORNTIME;
	tDISGUISE			= sItemLink.tDISGUISE;

	lnGenNum			= sItemLink.lnGenNum;
	wTurnNum			= sItemLink.wTurnNum;
	cGenType			= sItemLink.cGenType;	
	cChnID				= sItemLink.cChnID;		

	cFieldID			= sItemLink.cFieldID;

	cDAMAGE				= sItemLink.cDAMAGE;	
	cDEFENSE			= sItemLink.cDEFENSE;	
	cRESIST_FIRE		= sItemLink.cRESIST_FIRE;
	cRESIST_ICE			= sItemLink.cRESIST_ICE;
	cRESIST_ELEC		= sItemLink.cRESIST_ELEC;
	cRESIST_POISON		= sItemLink.cRESIST_POISON;
	cRESIST_SPIRIT		= sItemLink.cRESIST_SPIRIT;

	cOptTYPE1			= sItemLink.cOptTYPE1;
	cOptTYPE2			= sItemLink.cOptTYPE2;
	cOptTYPE3			= sItemLink.cOptTYPE3;
	cOptTYPE4			= sItemLink.cOptTYPE4;

	nOptVALUE1			= sItemLink.nOptVALUE1;
	nOptVALUE2			= sItemLink.nOptVALUE2;
	nOptVALUE3			= sItemLink.nOptVALUE3;
	nOptVALUE4			= sItemLink.nOptVALUE4;
	dwPetID				= sItemLink.dwPetID;
	dwVehicleID			= sItemLink.dwVehicleID;
	bVietnamGainItem	= sItemLink.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sItemLink.wColor1;
	wColor2				= sItemLink.wColor2;
}

/*item link, Juver, 2017/07/31 */
void SITEMCUSTOM::Convert ( SITEMLINK &sItemLink )
{
	sItemLink.sNativeID			= sNativeID;	
	sItemLink.nidDISGUISE		= nidDISGUISE;
	sItemLink.tBORNTIME			= tBORNTIME;
	sItemLink.tDISGUISE			= tDISGUISE;

	sItemLink.lnGenNum			= lnGenNum;
	sItemLink.wTurnNum			= wTurnNum;
	sItemLink.cGenType			= cGenType;	
	sItemLink.cChnID			= cChnID;		

	sItemLink.cFieldID			= cFieldID;

	sItemLink.cDAMAGE			= cDAMAGE;	
	sItemLink.cDEFENSE			= cDEFENSE;	
	sItemLink.cRESIST_FIRE		= cRESIST_FIRE;
	sItemLink.cRESIST_ICE		= cRESIST_ICE;
	sItemLink.cRESIST_ELEC		= cRESIST_ELEC;
	sItemLink.cRESIST_POISON	= cRESIST_POISON;
	sItemLink.cRESIST_SPIRIT	= cRESIST_SPIRIT;

	sItemLink.cOptTYPE1	= cOptTYPE1;
	sItemLink.cOptTYPE2	= cOptTYPE2;
	sItemLink.cOptTYPE3	= cOptTYPE3;
	sItemLink.cOptTYPE4	= cOptTYPE4;

	sItemLink.nOptVALUE1	= nOptVALUE1;
	sItemLink.nOptVALUE2	= nOptVALUE2;
	sItemLink.nOptVALUE3	= nOptVALUE3;
	sItemLink.nOptVALUE4	= nOptVALUE4;
	sItemLink.dwPetID		= dwPetID;
	sItemLink.dwVehicleID	= dwVehicleID;
	sItemLink.bVietnamGainItem	= bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	sItemLink.wColor1	= wColor1;
	sItemLink.wColor2	= wColor2;
}


// *****************************************************
// Desc: 인첸트 데미지
// *****************************************************
WORD SITEMCUSTOM::GETGRADE_DAMAGE () const
{
	BYTE cGRADE = grade_damage();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX;

    if ( cGRADE > GRADE_HIGH )
	{
		WORD wDAMAGE = getdamage().wHigh; // 물리 데미지
		WORD wMaDAMAGE = getmadamage();   // 기력 데미지
		WORD wDAMAGE_RATE = max (wDAMAGE,wMaDAMAGE);
		
		BYTE cOVER = cGRADE - GRADE_HIGH - 1;
		BYTE cOVER_N = GRADE_HIGH - GRADE_NORMAL;
		
		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDAMAGE_RATE = (WORD) ( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE * cOVER_N * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDAMAGE_RATE += GLCONST_CHAR::wDAMAGE_GRADE * GRADE_NORMAL;
		//		*, GRADE_HIGH 등급 이상은 등급별 비율로 증가
		wDAMAGE_RATE = (WORD)( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE_TOP[cOVER] );

		//	"고정"
		WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * GRADE_HIGH;
		//	"고정"
		wDAMAGE_VALUE = wDAMAGE_VALUE * GLCONST_CHAR::fDAMAGE_GRADE_TOP[cOVER];

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDAMAGE_RATE, wDAMAGE_VALUE );


	}
	else if ( cGRADE > GRADE_NORMAL )
	{
		//	"연마 공격력" 산출시 물리데미지와 기력치 증가율 두가지 중 큰것을 선택.
		WORD wDAMAGE = getdamage().wHigh; // 물리 데미지
		WORD wMaDAMAGE = getmadamage();   // 기력 데미지
		WORD wDAMAGE_RATE = max (wDAMAGE,wMaDAMAGE);

		BYTE cOVER = cGRADE-GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDAMAGE_RATE = (WORD) ( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE * cOVER * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDAMAGE_RATE += GLCONST_CHAR::wDAMAGE_GRADE * GRADE_NORMAL;

		//	"고정"
		WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * cGRADE;

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDAMAGE_RATE, wDAMAGE_VALUE );
	}

	//	"고정"
	WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * cGRADE;
	return wDAMAGE_VALUE;
}

WORD SITEMCUSTOM::GETGRADE_DEFENSE () const
{
	BYTE cGRADE = grade_defense();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX;

	//	방어력 비율 값.
	if ( cGRADE > GRADE_HIGH )
	{
		//	"연마 방어력" 산출.
		WORD wDEFENSE_RATE = getdefense();


		BYTE cOVER = cGRADE - GRADE_HIGH - 1;
		BYTE cOVER_N = GRADE_HIGH - GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE*GLCONST_CHAR::fDEFENSE_GRADE * cOVER_N * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDEFENSE_RATE += GLCONST_CHAR::wDEFENSE_GRADE*GRADE_NORMAL;
		//		*, GRADE_HIGH 등급 이상은 등급별 비율로 증가
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE * GLCONST_CHAR::fDEFENSE_GRADE_TOP[cOVER] );

		//	"고정"
		WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * GRADE_HIGH;
		
		//	"고정"
		wDEFENSE_VALUE = wDEFENSE_VALUE * GLCONST_CHAR::fDEFENSE_GRADE_TOP[cOVER];	

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDEFENSE_RATE, wDEFENSE_VALUE );


	}
	else if ( cGRADE > GRADE_NORMAL )
	{
		//	"연마 방어력" 산출.
		WORD wDEFENSE_RATE = getdefense();


		BYTE cOVER = cGRADE-GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE*GLCONST_CHAR::fDEFENSE_GRADE * cOVER * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDEFENSE_RATE += GLCONST_CHAR::wDEFENSE_GRADE*GRADE_NORMAL;

		//	"고정"
		WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * cGRADE;

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDEFENSE_RATE, wDEFENSE_VALUE );
	}

	//	"고정"
	WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * cGRADE;
	return wDEFENSE_VALUE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_FIRE () const
{
	BYTE cGRADE = grade_fire();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_FIRE_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_ICE () const
{
	BYTE cGRADE = grade_ice();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_ICE_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_ELEC () const
{
	BYTE cGRADE = grade_elec();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_ELEC_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_POISON () const
{
	BYTE cGRADE = grade_poison();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_POISON_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_SPIRIT () const
{
	BYTE cGRADE = grade_spirit();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_SPIRIT_GRADE * cGRADE;
}

BYTE SITEMCUSTOM::grade_damage () const
{
	return BYTE ( cDAMAGE + GETOptVALUE(EMR_OPT_GRIND_DAMAGE) );
}

BYTE SITEMCUSTOM::grade_defense () const
{
	return BYTE ( cDEFENSE + GETOptVALUE(EMR_OPT_GRIND_DEFENSE) );
}

BYTE SITEMCUSTOM::GETGRADE ( EMGRINDING_TYPE emTYPE ) const
{
	switch ( emTYPE )
	{
		//	Note : EMGRINDING_NUNE 으로 등급 질의시에 아이템 통합 등급 표시. ( 단 저항 부분 제외 )
	case EMGRINDING_NONE:
		{
			BYTE cDAM = grade_damage();
			BYTE cDEF = grade_defense();
			return cDAM > cDEF ? cDAM : cDEF;
		}

	case EMGRINDING_DAMAGE:
		return grade_damage();

	case EMGRINDING_DEFENSE:
		return grade_defense();

	case EMGRINDING_RESIST_FIRE:
		return grade_fire();

	case EMGRINDING_RESIST_ICE:
		return grade_ice();

	case EMGRINDING_RESIST_ELEC:
		return grade_elec();

	case EMGRINDING_RESIST_POISON:
		return grade_poison();

	case EMGRINDING_RESIST_SPIRIT:
		return grade_spirit();
	};

	return 0;
}

BYTE SITEMCUSTOM::GETGRADE_EFFECT () const
{
	BYTE cGRADE = GETGRADE(EMGRINDING_NONE);

	if ( cGRADE==0 )	return 0;

	// +9 등급 이하일 경우 
	if( cGRADE <= 9 )
	{
		return cGRADE = 1 + (cGRADE-1)/2;
	}
	
	return cGRADE = cGRADE - 4;
}

inline float SITEMCUSTOM::GETOptVALUE ( EMRANDOM_OPT emOPT, short nVALUE ) const
{
	switch ( emOPT )
	{
	default:
	case EMR_OPT_NULL:
		return 0.0f;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		return nVALUE*0.01f;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		return nVALUE*0.01f;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		return nVALUE;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		return nVALUE*0.0001f;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		return nVALUE;

	case EMR_OPT_RANGE:
		return nVALUE*0.01f;

	case EMR_OPT_DIS_SP:
		return nVALUE;

	case EMR_OPT_RESIST:
		return nVALUE;

	case EMR_OPT_ATTACK_VOL:
	case EMR_OPT_DEFENSE_VOL:
	case EMR_OPT_HIT_VOL:
	case EMR_OPT_AVOID_VOL:
	case EMR_OPT_POWER:	
	case EMR_OPT_VITALITY:	
	case EMR_OPT_SPIRIT:	
	case EMR_OPT_DEXTERITY:	
	case EMR_OPT_STAMINA:	
	case EMR_OPT_MELEE:
	case EMR_OPT_SHOOTING:
	case EMR_OPT_ENERGY:
		return nVALUE;
	};

	return 0.0f;
}

void SITEMCUSTOM::SETOptVALUE ( int nIndex, EMRANDOM_OPT emOPT, float fVALUE )
{
	short nVALUE(0);

	switch ( emOPT )
	{
	default:
	case EMR_OPT_NULL:
		nVALUE = 0;
		break;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		nVALUE = (short) (fVALUE*100.0f);
		break;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		nVALUE = (short) (fVALUE*100.0f);
		break;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		nVALUE = (short) (fVALUE*10000);
		break;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_RANGE:
		nVALUE = (short) (fVALUE*100);
		break;

	case EMR_OPT_DIS_SP:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_RESIST:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_ATTACK_VOL:
	case EMR_OPT_DEFENSE_VOL:
	case EMR_OPT_HIT_VOL:
	case EMR_OPT_AVOID_VOL:
	case EMR_OPT_POWER:	
	case EMR_OPT_VITALITY:	
	case EMR_OPT_SPIRIT:	
	case EMR_OPT_DEXTERITY:	
	case EMR_OPT_STAMINA:
	case EMR_OPT_MELEE:
	case EMR_OPT_SHOOTING:
	case EMR_OPT_ENERGY:
		nVALUE = (short) (fVALUE);
		break;
	};

	switch ( nIndex )
	{
	case 0:
		cOptTYPE1 = emOPT;
		nOptVALUE1 = nVALUE;
		break;

	case 1:
		cOptTYPE2 = emOPT;
		nOptVALUE2 = nVALUE;
		break;

	case 2:
		cOptTYPE3 = emOPT;
		nOptVALUE3 = nVALUE;
		break;

	case 3:
		cOptTYPE4 = emOPT;
		nOptVALUE4 = nVALUE;
		break;
	}
}

void SITEMCUSTOM::SETOptVALUEEmpty( EMRANDOM_OPT emOPT, float fVALUE )	// ITEMREBUILD_MARK
{
	if( cOptTYPE1 == 0 )
	{
		SETOptVALUE( 0, emOPT, fVALUE );
	}
	else if( cOptTYPE2 == 0 )
	{
		SETOptVALUE( 1, emOPT, fVALUE );
	}
	else if( cOptTYPE3 == 0 )
	{
		SETOptVALUE( 2, emOPT, fVALUE );
	}
	else if( cOptTYPE4 == 0 )
	{
		SETOptVALUE( 3, emOPT, fVALUE );
	}
}

int SITEMCUSTOM::RESETOptVALUE( int nOpt )
{
	int iEmpty = 0;

	if( IsValidRESETOpt( 0 ) )
	{
		if ( nOpt != 0 ) {
		cOptTYPE1 = 0;
		nOptVALUE1 = 0;
		++iEmpty;
		} 
	}

	if( IsValidRESETOpt( 1 ) )
	{
		if ( nOpt != 1 ) {
		cOptTYPE2 = 0;
		nOptVALUE2 = 0;
		++iEmpty;
		}
	}

	if( IsValidRESETOpt( 2 ) )
	{
		if ( nOpt != 2 ) {
		cOptTYPE3 = 0;
		nOptVALUE3 = 0;
		++iEmpty;
		}
	}

	if( IsValidRESETOpt( 3 ) )
	{
		if ( nOpt != 3 ) {
		cOptTYPE4 = 0;
		nOptVALUE4 = 0;
		++iEmpty;
		}
	}

	return ( SRANDOM_GEN::MAX_OPT - iEmpty );
}

bool SITEMCUSTOM::IsValidRESETOpt( int nIndex )
{
	BYTE optType[] = { cOptTYPE1, cOptTYPE2, cOptTYPE3, cOptTYPE4 };

	if( optType[ nIndex ] == EMR_OPT_GRIND_DAMAGE && cDAMAGE )
		return false;
	if( optType[ nIndex ] == EMR_OPT_GRIND_DEFENSE && cDEFENSE )
		return false;

	return true;
}

float SITEMCUSTOM::GETOptVALUE1 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE1();
	short nVALUE = nOptVALUE1;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE2 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE2();
	short nVALUE = nOptVALUE2;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE3 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE3();
	short nVALUE = nOptVALUE3;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE4 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE4();
	short nVALUE = nOptVALUE4;

	return GETOptVALUE ( emOPT, nVALUE );
}

BOOL SITEMCUSTOM::IsPerRandOpt( INT nRandOPT ) const
{
	switch( nRandOPT )
	{
	case EMR_OPT_GRIND_DAMAGE:	// 공격력 연마.
	case EMR_OPT_GRIND_DEFENSE:	// 방어력 연마.
	case EMR_OPT_RANGE:			// 공격 영역.
	case EMR_OPT_DIS_SP:		// sp 소모량.
	case EMR_OPT_ATTACK_VOL:
	case EMR_OPT_DEFENSE_VOL:
	case EMR_OPT_HIT_VOL:
	case EMR_OPT_AVOID_VOL:
	case EMR_OPT_POWER:	
	case EMR_OPT_VITALITY:
	case EMR_OPT_SPIRIT:	
	case EMR_OPT_DEXTERITY:	
	case EMR_OPT_STAMINA:	
	case EMR_OPT_MELEE:
	case EMR_OPT_SHOOTING:
	case EMR_OPT_ENERGY:
		return FALSE;

	default :
		return TRUE;
	}
}

float SITEMCUSTOM::GETOptVALUE ( EMRANDOM_OPT emOPT ) const
{
	if ( GETOptTYPE1()==emOPT )			return GETOptVALUE1();
	else if ( GETOptTYPE2()==emOPT )	return GETOptVALUE2();
	else if ( GETOptTYPE3()==emOPT )	return GETOptVALUE3();
	else if ( GETOptTYPE4()==emOPT )	return GETOptVALUE4();

	return 0;
}

short SITEMCUSTOM::GETOptVALUE_SHORT ( EMRANDOM_OPT emOPT ) const
{
	if ( GETOptTYPE1()==emOPT )			return nOptVALUE1;
	else if ( GETOptTYPE2()==emOPT )	return nOptVALUE2;
	else if ( GETOptTYPE3()==emOPT )	return nOptVALUE3;
	else if ( GETOptTYPE4()==emOPT )	return nOptVALUE4;

	return 0;
}

BOOL SITEMCUSTOM::IsSetRandOpt() const
{
	if( GETOptTYPE1() != EMR_OPT_NULL )			return TRUE;
	else if( GETOptTYPE2() != EMR_OPT_NULL )	return TRUE;
	else if( GETOptTYPE3() != EMR_OPT_NULL )	return TRUE;
	else if( GETOptTYPE4() != EMR_OPT_NULL )	return TRUE;

	return FALSE;
}

#define USAGE_RANDOM_OPT_SE
#ifdef USAGE_RANDOM_OPT_SE
//#ifdef DAUMTESTPARAM	// 한국 테섭에서만 2차 랜덤 옵션 파일 사용 ( 준혁 )
	bool SITEMCUSTOM::GENERATE_RANDOM_OPT( bool bNew, int nOpt )
	{
		SRANDOM_GEN *pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( sNativeID );
		if( !pRANDOM_SET )
			return false;

		const SRANDOM_GEN &sRANDOM = *pRANDOM_SET;

		//	랜덤 옵션 발생 확율 검사.
		if( bNew && !RANDOM_GEN( sRANDOM.fRATE ) )
			return true;

		int nGEN( 0 );

		//	설정한 옵션중 임의 순위로 부여 하기 위해서 순서를 섞음.
		std::vector< DWORD > vecORDER;
		vecORDER.reserve( SRANDOM_GEN::MAX_SET );
		for( DWORD i=0; i<sRANDOM.dwSET_NUM; ++i )
			vecORDER.push_back( i );
		std::random_shuffle( vecORDER.begin(), vecORDER.end() );

		// 개조 시 유저가 연마를 했으면 랜덤옵의 연마도 초기화 시키지 않으므로, 랜덤옵 붙을 수 있는 갯수를 보정한다
		if( !bNew )
			nGEN = RESETOptVALUE( nOpt );

		//	랜덤옵션 발생.
		for(int i=0; i<sRANDOM.dwSET_NUM && nGEN<SRANDOM_GEN::MAX_OPT; ++i )
		{
			if ( i == nOpt ) continue;
			float fNowRate = seqrandom::getpercent();
			DWORD dwORDER = vecORDER[i];

			if ( dwORDER >= SRANDOM_GEN::MAX_SET )
				continue;

			// 새로 생성되는 아이템과 개조하는 아이템의 랜덤 옵션 설정을 구분한다
			const SRANDOM_OPT_SET* pSET = &sRANDOM.sSET[ dwORDER ];

			if ( !pSET )
				continue;

			if( !bNew )
				pSET = &sRANDOM.sSETR[ dwORDER ];

			if( pSET->wTYPE == EMR_OPT_NULL )
				continue;

			// 개조 시 랜덤 옵션 타입이 공격력 연마이고, 유저가 공격력 연마를 이미 했으면 옵션 발생을 취소한다.
			// 개조 시 랜덤 옵션 타입이 방어력 연마이고, 유저가 방어력 연마를 이미 했으면 옵션 발생을 취소한다.
			if( !bNew )
			{
				if( pSET->wTYPE == EMR_OPT_GRIND_DAMAGE && cDAMAGE )
					continue;
				if( pSET->wTYPE == EMR_OPT_GRIND_DEFENSE && cDEFENSE )
					continue;
			}

			//	발생확율에 적합할때 발생.
			if( fNowRate < pSET->fRATE )
			{
				// 확률에 맞게 설정된 구간을 하나 얻어온다
				int iOptSe = GENERATE_RANDOM_OPT_SE( pSET );
				if( 0 <= iOptSe && iOptSe < pSET->wSECT )
				{
					float fPOS = seqrandom::getpercent();
					float fVALUE = pSET->pSECT[ iOptSe ].fLOW;
					fVALUE += ( pSET->pSECT[ iOptSe ].fHIGH - pSET->pSECT[ iOptSe ].fLOW ) * fPOS * 0.01f;

					SETOptVALUEEmpty( (EMRANDOM_OPT)pSET->wTYPE, fVALUE );
					++nGEN;
				}
			}
		}

		return true;
	}

	int SITEMCUSTOM::GENERATE_RANDOM_OPT_SE( const SRANDOM_OPT_SET* pOptSet )
	{
		if( !pOptSet )
			return INT_MIN;

		float fRatePer = seqrandom::getpercent();
		float fRateCur = 0.0f;

		for( WORD i=0; i<pOptSet->wSECT; ++i )
		{
			float fRate = pOptSet->pSECT[i].fRATE;

			if( ( fRateCur <= fRatePer ) && ( fRatePer < ( fRateCur + fRate ) ) )
				return i;

			fRateCur += fRate;
		}

		return INT_MIN;
	}
	#undef USAGE_RANDOM_OPT_SE
#else
	bool SITEMCUSTOM::GENERATE_RANDOM_OPT( bool bNew )
	{
		SRANDOM_GEN *pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( sNativeID );
		if( !pRANDOM_SET )
			return false;

		const SRANDOM_GEN &sRANDOM = *pRANDOM_SET;

		//	랜덤 옵션 발생 확율 검사.
		if( !RANDOM_GEN( sRANDOM.fRATE ) )
			return true;

		int nGEN( 0 );

		//	설정한 옵션중 임의 순위로 부여 하기 위해서 순서를 섞음.
		std::vector< DWORD > vecORDER;
		vecORDER.reserve( SRANDOM_GEN::MAX_SET );
		for( DWORD i=0; i<sRANDOM.dwSET_NUM; ++i )
			vecORDER.push_back( i );

		std::random_shuffle( vecORDER.begin(), vecORDER.end() );

		//	랜덤옵션 발생.
		for( i=0; i<sRANDOM.dwSET_NUM && nGEN<SRANDOM_GEN::MAX_OPT; ++i )
		{
			float fNowRate = seqrandom::getpercent();
			DWORD dwORDER = vecORDER[i];

			//	임의 순위 랜덤 설정.
			const SRANDOM_OPT_SET &sSET = sRANDOM.sSET[dwORDER];
			if( sSET.wTYPE == EMR_OPT_NULL )
				continue;

			//	발생확율에 적합할때 발생.
			if( fNowRate<sSET.fRATE )
			{
				float fPOS = seqrandom::getpercent();
				float fVALUE = sSET.fLOW + (sSET.fHIGH-sSET.fLOW) * fPOS * 0.01f;
				SETOptVALUE( nGEN++, (EMRANDOM_OPT)sSET.wTYPE, fVALUE );
			}
		}

		return true;
	}

	int SITEMCUSTOM::GENERATE_RANDOM_OPT_SE( const SRANDOM_OPT_SET* pOptSet )
	{
		return INT_MIN;
	}
#endif

// *****************************************************
// Desc: 아이템 공격 데미지 반환
// *****************************************************
GLPADATA SITEMCUSTOM::getdamage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return GLPADATA();
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	GLPADATA sDAMAGE = sSUIT.gdDamage;

	// 렌던 수치 합산
	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float fRATE2 = GETOptVALUE(EMR_OPT_ATTACK_VOL);//add directval damage

	if (sSUIT.gdDamage.wLow > sSUIT.gdDamage.wHigh )
	{
		sSUIT.gdDamage.wLow = sSUIT.gdDamage.wHigh;
	}

	if ( fRATE!=0 )
	{
		//sDAMAGE.dwLow = DWORD(sSUIT.gdDamage.dwLow*(100.0f+fRATE)*0.01f);
		//sDAMAGE.dwHigh = DWORD(sSUIT.gdDamage.dwHigh*(100.0f+fRATE)*0.01f);
		if ( fRATE2!=0 )
		{
			sDAMAGE.wLow = DWORD( ( sSUIT.gdDamage.wLow + fRATE2 )*(100.0f+fRATE)*0.01f);
			sDAMAGE.wHigh = DWORD( (sSUIT.gdDamage.wHigh + fRATE2 )*(100.0f+fRATE)*0.01f);
		}
		else
		{
			sDAMAGE.wLow = DWORD(sSUIT.gdDamage.wLow*(100.0f+fRATE)*0.01f);
			sDAMAGE.wHigh = DWORD(sSUIT.gdDamage.wHigh*(100.0f+fRATE)*0.01f);
		}
	}
	else
	{
		if ( fRATE2!=0 )
		{
			sDAMAGE.wLow += DWORD(fRATE2);//add directval damage
			sDAMAGE.wHigh += DWORD(fRATE2);//add directval damage
		}
	}

	return sDAMAGE;
}

// *****************************************************
// Desc: 기력(마력) 데미지 반환
// *****************************************************
WORD SITEMCUSTOM::getmadamage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wMaDAMAGE(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_MA )
		{
			wMaDAMAGE += sADDON.nVALUE;
		}
	}

	/*costume combine stats, Juver, 2017/09/01 */
	SITEM *pitem_data_disguise = GLItemMan::GetInstance().GetItem(nidDISGUISE);
	if ( pitem_data_disguise )
	{
		for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			const ITEM::SADDON &saddon_disguise = pitem_data_disguise->sSuitOp.sADDON[i];
			if ( saddon_disguise.emTYPE==EMADD_MA )
			{
				wMaDAMAGE += saddon_disguise.nVALUE;
			}
		}
	}
	
	// 기력 데미지에도 랜덤 수치를 가산한다
	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float fRATE2 = GETOptVALUE(EMR_OPT_ATTACK_VOL); //add directval damage
	if ( fRATE!=0 )
	{
		if ( fRATE2!=0 )	
			wMaDAMAGE = WORD( (wMaDAMAGE + fRATE2) *(100.0f+fRATE)*0.01f);
		else	
			wMaDAMAGE = WORD(wMaDAMAGE*(100.0f+fRATE)*0.01f);
	}
	else	
	{
		if ( fRATE2!=0 )	wMaDAMAGE += WORD (fRATE2);
	}

	return wMaDAMAGE;
}

short SITEMCUSTOM::getdefense () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nDEFENSE = sSUIT.nDefense;

	float fRATE = GETOptVALUE(EMR_OPT_DEFENSE);
	float fRATE2 = GETOptVALUE(EMR_OPT_DEFENSE_VOL); //add directval defense
	if ( fRATE!=0.0f )
	{
		if ( fRATE2!=0.0f )	nDEFENSE = short( (nDEFENSE + fRATE2)*(100.0f+fRATE)*0.01f);
		else	nDEFENSE = short(nDEFENSE*(100.0f+fRATE)*0.01f);
	}
	else
	{
		if ( fRATE2!=0.0f )	nDEFENSE += short(fRATE2);
	}

	return nDEFENSE;
}

float SITEMCUSTOM::GETMOVESPEED() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fSpeed = sSUIT.sVOLUME.fVolume;

	// 렌던 수치 합산
	float fRATE = GETOptVALUE(EMR_OPT_MOVE_SPEED);
	if ( fRATE!=0 )
	{
		fSpeed = ( fSpeed * ( 100.0f + fRATE )  * 0.01f );
	}

	return fSpeed;
}

float SITEMCUSTOM::GETMOVESPEEDR() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fSpeed = sSUIT.sVARIATE.fVariate;

/*
	// 렌던 수치 합산
	float fRATE = GETOptVALUE(EMR_OPT_MOVE_SPEED);
	if ( fRATE!=0 )
	{
		fSpeed = ( fSpeed * ( 100.0f + fRATE )  * 0.01f );
	}
*/
	return fSpeed;
}

GLPADATA SITEMCUSTOM::GETDAMAGE () const
{
	GLPADATA &sDAMAGE = getdamage();

	sDAMAGE.wLow += GETGRADE_DAMAGE();
	sDAMAGE.wHigh += GETGRADE_DAMAGE();

	return sDAMAGE;
}

DWORD SITEMCUSTOM::GETMaDAMAGE () const
{
	DWORD dwMaDAMAGE = getmadamage();

	dwMaDAMAGE += GETGRADE_DAMAGE();

	return dwMaDAMAGE;
}

short SITEMCUSTOM::GETDEFENSE () const
{
	short nDEFENSE = getdefense();

	nDEFENSE += GETGRADE_DEFENSE();

	return nDEFENSE;
}

short SITEMCUSTOM::GETHITRATE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nHITRATE = sSUIT.nHitRate;

	float dHITRATE = GETOptVALUE(EMR_OPT_HITRATE);
	float fRATE = GETOptVALUE(EMR_OPT_HIT_VOL);// change for add directval

	if ( fRATE!=0.0f  )
	{
		if( dHITRATE != 0.0f )
			nHITRATE =  short(nHITRATE+fRATE+dHITRATE);
		else
			nHITRATE = short(nHITRATE+fRATE);
	}
	else
	{
		nHITRATE = short(nHITRATE+dHITRATE);
	}
	
	return nHITRATE;
}

float SITEMCUSTOM::GETHITRATE_PER () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;

	float fHITRATE = 0;
	float fRATE = GETOptVALUE(EMR_OPT_HITRATE); // change for add directval	
	if ( fRATE!=0.0f )	fHITRATE += fRATE;
	return fHITRATE;
}

short SITEMCUSTOM::GETAVOIDRATE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nAVOIDRATE = sSUIT.nAvoidRate;

	float dAVOID = GETOptVALUE(EMR_OPT_AVOIDRATE);

	float fRATE = GETOptVALUE(EMR_OPT_AVOID_VOL);// change for add directval
	if ( fRATE!=0.0f )
	{
		if(dAVOID != 0.0f)
		nAVOIDRATE = short(nAVOIDRATE+fRATE+dAVOID);
		else
		nAVOIDRATE = short(nAVOIDRATE+fRATE);
	}
	else
	{
		nAVOIDRATE = short(nAVOIDRATE+dAVOID);
	}

	return nAVOIDRATE;
}

float SITEMCUSTOM::GETAVOIDRATE_PER () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;

	float fAVOIDRATE = 0.0f;
	float fRATE = GETOptVALUE(EMR_OPT_AVOIDRATE);// change for add directval
	if ( fRATE!=0.0f )	fAVOIDRATE += fRATE;
	return fAVOIDRATE;
}

WORD SITEMCUSTOM::GETATTRANGE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wATTRANGE = sSUIT.wAttRange;

	float fRATE = GETOptVALUE(EMR_OPT_RANGE);
	if ( fRATE!=0.0f )
	{
		wATTRANGE = WORD(wATTRANGE+fRATE);
	}

	return wATTRANGE;
}

WORD SITEMCUSTOM::GETREQ_SP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wREQSP = sSUIT.wReqSP;

	float fVALUE = GETOptVALUE(EMR_OPT_DIS_SP);
	if ( fVALUE!=0.0f )
	{
		if ( fVALUE+wREQSP>0.0f )	wREQSP = WORD(wREQSP+fVALUE);
		else						wREQSP = 0;
	}

	return wREQSP;
}

int SITEMCUSTOM::GETADDHP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nHP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_HP )
		{
			nHP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nHP += (int) GETOptVALUE(EMR_OPT_HP);

	return nHP;
}

int SITEMCUSTOM::GETADDMP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nMP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_MP )
		{
			nMP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nMP += (int) GETOptVALUE(EMR_OPT_MP);

	return nMP;
}

int SITEMCUSTOM::GETADDSP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nSP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_SP )
		{
			nSP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nSP += (int) GETOptVALUE(EMR_OPT_SP);

	return nSP;
}

int SITEMCUSTOM::GETADDPA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nMelee = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_MELEE);
	if ( fVALUE!=0.0f )	nMelee += int(fVALUE);
	return nMelee;
}


int SITEMCUSTOM::GETADDMA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	INT wMaDAMAGE(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_MA )
		{
			wMaDAMAGE += sADDON.nVALUE;
		}
	}

	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float fRATE2 = GETOptVALUE(EMR_OPT_ATTACK_VOL); //add directval damage
	float fRATE3 = GETOptVALUE(EMR_OPT_ENERGY); //add directval energy
	if ( fRATE!=0 )	
	{
		if ( fRATE2!=0 )	wMaDAMAGE = INT( ( wMaDAMAGE + fRATE2 )*(100.0f+fRATE)*0.01f);
		else	wMaDAMAGE = INT(wMaDAMAGE*(100.0f+fRATE)*0.01f);
	}
	else	
	{
		if ( fRATE2!=0 )	wMaDAMAGE += INT (fRATE2);
	}

	if ( fRATE3!=0 )		wMaDAMAGE += INT (fRATE3);

	return wMaDAMAGE;
}

int SITEMCUSTOM::GETADDSA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nMessile = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_SHOOTING);
	if ( fVALUE!=0.0f )	nMessile += int(fVALUE);
	return nMessile;
}

//add directval energy
int SITEMCUSTOM::GETADDENERGY() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nEnergy = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_ENERGY);
	if ( fVALUE!=0.0f )	nEnergy += int(fVALUE);
	return nEnergy;
}

float SITEMCUSTOM::GETINCHP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_HP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_HP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCMP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_MP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_MP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCSP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_SP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_SP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCAP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_AP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_HMS_INC);

	return fINC;
}

WORD SITEMCUSTOM::GETRESIST_FIRE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nRESIST = sSUIT.sResist.nFire;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_FIRE();
}

WORD SITEMCUSTOM::GETRESIST_ICE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nRESIST = sSUIT.sResist.nIce;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_ICE();
}

WORD SITEMCUSTOM::GETRESIST_ELEC () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nRESIST = sSUIT.sResist.nElectric;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_ELEC();
}

WORD SITEMCUSTOM::GETRESIST_POISON () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nRESIST = sSUIT.sResist.nPoison;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_POISON();
}

WORD SITEMCUSTOM::GETRESIST_SPIRIT () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nRESIST = sSUIT.sResist.nSpirit;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_SPIRIT();
}
//add directval statspow
WORD SITEMCUSTOM::GET_STAT_POW() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	WORD wPow = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_POWER);
	if ( fVALUE!=0.0f )	wPow += WORD(fVALUE);
	return wPow;
}

//add directval statsvit
WORD SITEMCUSTOM::GET_STAT_VIT() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	WORD wVit = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_VITALITY);
	if ( fVALUE!=0.0f )	wVit += WORD(fVALUE);
	return wVit;
}

//add directval statsint
WORD SITEMCUSTOM::GET_STAT_INT() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	WORD wInt = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_SPIRIT);
	if ( fVALUE!=0.0f )	wInt += WORD(fVALUE);
	return wInt;
}

//add directval statsDex
WORD SITEMCUSTOM::GET_STAT_DEX() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	WORD wDex = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_DEXTERITY);
	if ( fVALUE!=0.0f )	wDex += WORD(fVALUE);
	return wDex;
}

//add directval statsStm
WORD SITEMCUSTOM::GET_STAT_STM() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	WORD wStm = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_STAMINA);
	if ( fVALUE!=0.0f )	wStm += WORD(fVALUE);
	return wStm;
}

//add directval hp potion
int SITEMCUSTOM::GET_HP_POTION() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nHpPotion = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_HP_REC);
	if ( fVALUE!=0.0f )	nHpPotion += int(fVALUE);
	return nHpPotion;
}

//add directval mp potion
int SITEMCUSTOM::GET_MP_POTION() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nMpPotion = 0;
	float fVALUE = GETOptVALUE(EMR_OPT_MP_REC);
	if ( fVALUE!=0.0f )	nMpPotion += int(fVALUE);
	return nMpPotion;
}

//add directval sp potion
int SITEMCUSTOM::GET_SP_POTION() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	
	int nSpPotion = 0.0f;
	float fVALUE = GETOptVALUE(EMR_OPT_SP_REC);
	if ( fVALUE!=0.0f )	nSpPotion += int(fVALUE);
	return nSpPotion;
}


SITEM_LOBY::SITEM_LOBY ()
	: sNativeID(false)
	, nidDISGUISE(false)

	, lnGenNum(0)
	, wTurnNum(1)
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)

	, cOptTYPE1(0) // 공격력
	, cOptTYPE2(0) // 기저항
	, cOptTYPE3(0)
	, cOptTYPE4(0)

	, nOptVALUE1(0) // 공격력 %
	, nOptVALUE2(0) // 기저항 %
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

SITEM_LOBY::SITEM_LOBY ( SNATIVEID	sNID )
	: sNativeID(sNID)
	, nidDISGUISE(false)
	, lnGenNum(0)
	, wTurnNum(1)
	
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)

	, cOptTYPE1(0) // 공격력
	, cOptTYPE2(0) // 기저항
	, cOptTYPE3(0)
	, cOptTYPE4(0)

	, nOptVALUE1(0) // 공격력 %
	, nOptVALUE2(0) // 기저항 %
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

bool SITEM_LOBY::operator == ( const SITEM_LOBY &value )
{
	return !memcmp(this,&value,sizeof(SITEM_LOBY));
}

bool SITEM_LOBY::operator != ( const SITEM_LOBY &value )
{
	return 0!=memcmp(this,&value,sizeof(SITEM_LOBY));
}

void SITEM_LOBY::Assign ( const SITEMCUSTOM &sItemCustom )
{
	sNativeID			= sItemCustom.sNativeID;	
	nidDISGUISE			= sItemCustom.nidDISGUISE;

	lnGenNum			= sItemCustom.lnGenNum;
	wTurnNum			= sItemCustom.wTurnNum;
	cGenType			= sItemCustom.cGenType;	
	cChnID				= sItemCustom.cChnID;		

	cFieldID			= sItemCustom.cFieldID;

	cDAMAGE				= sItemCustom.cDAMAGE;	
	cDEFENSE			= sItemCustom.cDEFENSE;	
	cRESIST_FIRE		= sItemCustom.cRESIST_FIRE;
	cRESIST_ICE			= sItemCustom.cRESIST_ICE;
	cRESIST_ELEC		= sItemCustom.cRESIST_ELEC;
	cRESIST_POISON		= sItemCustom.cRESIST_POISON;
	cRESIST_SPIRIT		= sItemCustom.cRESIST_SPIRIT;

	cOptTYPE1			= sItemCustom.cOptTYPE1;
	cOptTYPE2			= sItemCustom.cOptTYPE2;
	cOptTYPE3			= sItemCustom.cOptTYPE3;
	cOptTYPE4			= sItemCustom.cOptTYPE4;
	
	nOptVALUE1			= sItemCustom.nOptVALUE1;
	nOptVALUE2			= sItemCustom.nOptVALUE2;
	nOptVALUE3			= sItemCustom.nOptVALUE3;
	nOptVALUE4			= sItemCustom.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sItemCustom.wColor1;
	wColor2				= sItemCustom.wColor2;

};

BYTE SITEMCLIENT::GETGRADE_EFFECT () const
{
	BYTE cGRADE = GETGRADE();

	if ( cGRADE==0 )	return 0;

	// +9 등급 이하일 경우 
	if( cGRADE <= 9 )
	{
		return cGRADE = 1 + (cGRADE-1)/2;
	}
	
	return cGRADE = cGRADE - 4;
}

BOOL SETPUTONITEMS_BYBUF ( SITEMCUSTOM *pPutOnItems, CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	//	DWORD[VERSION] + DWORD[SIZE] + DWORD[NUMBER] + PUTONITEMS_ARRAY[SIZE]
	//
	DWORD dwVersion, dwSize, dwNum;

	ByteStream >> dwVersion;
	ByteStream >> dwSize;
	ByteStream >> dwNum;

	SITEMCUSTOM sItemCustom;
	for ( DWORD i=0; i<dwNum; i++ )
	{
		if ( i>=SLOT_TSIZE )	break;

		//	Note : 향후 버전에서 이전 버전을 대입 연산으로 처리 할 부분.
		//
		if ( dwVersion==0x0100 )
		{
			SITEMCUSTOM_100 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0101 )
		{
			SITEMCUSTOM_101 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0102 )
		{
			SITEMCUSTOM_102 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0103 )
		{
			SITEMCUSTOM_103 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0104 )
		{
			SITEMCUSTOM_104 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0105 )
		{
			SITEMCUSTOM_105 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0106 )
		{
			SITEMCUSTOM_106 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0107 )
		{
			SITEMCUSTOM_107 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0108 )
		{
			SITEMCUSTOM_108 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0109 )
		{
			SITEMCUSTOM_109 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0110)
		{
			SITEMCUSTOM_110 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0111)
		{
			SITEMCUSTOM_111 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0112)
		{
			SITEMCUSTOM_112 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==SITEMCUSTOM::VERSION )
		{
			GASSERT(dwSize==sizeof(SITEMCUSTOM));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustom, sizeof(SITEMCUSTOM) );

			pPutOnItems[i] = sItemCustom;
		}
		else
		{
		}
	}

	return TRUE;
}


BOOL SETPUTONITEMS_BYBUF ( SITEM_LOBY *pPutOnItems, CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	//	DWORD[VERSION] + DWORD[SIZE] + DWORD[NUMBER] + PUTONITEMS_ARRAY[SIZE]
	//
	DWORD dwVersion, dwSize, dwNum;

	ByteStream >> dwVersion;
	ByteStream >> dwSize;
	ByteStream >> dwNum;

	SITEMCUSTOM sItemCustom;
	for ( DWORD i=0; i<dwNum; i++ )
	{
		if ( i>=SLOT_TSIZE )	break;

		//	Note : 향후 버전에서 이전 버전을 대입 연산으로 처리 할 부분.
		//
		if ( dwVersion==0x0100 )
		{
			SITEMCUSTOM_100 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0101 )
		{
			SITEMCUSTOM_101 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0102 )
		{
			SITEMCUSTOM_102 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0103 )
		{
			SITEMCUSTOM_103 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0104 )
		{
			SITEMCUSTOM_104 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0105 )
		{
			SITEMCUSTOM_105 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0106 )
		{
			SITEMCUSTOM_106 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0107 )
		{
			SITEMCUSTOM_107 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0108 )
		{
			SITEMCUSTOM_108 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0109 )
		{
			SITEMCUSTOM_109 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0110 )
		{
			SITEMCUSTOM_110 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0111 )
		{
			SITEMCUSTOM_111 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0112 )
		{
			SITEMCUSTOM_112 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==SITEMCUSTOM::VERSION )
		{
			GASSERT(dwSize==sizeof(SITEMCUSTOM));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustom, sizeof(SITEMCUSTOM) );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else
		{
		}
	}

	return TRUE;
}


bool CItemDrop::IsTakeItem ( DWORD dwPartyID, DWORD dwGaeaID )
{
	bool bhold(false);

	switch ( emGroup )
	{
	case EMGROUP_ONE:
		if ( dwHoldGID==GAEAID_NULL )		bhold = true;
		else if ( dwHoldGID==dwGaeaID )		bhold = true;
		break;

	case EMGROUP_PARTY:
		if ( dwHoldGID==PARTY_NULL )		bhold = true;
		else if ( dwHoldGID==dwPartyID )	bhold = true;
		break;

	case EMGROUP_GUILD:
		break;
	};

	return bhold;
}

bool CItemDrop::IsDropOut()
{
	return ( GLCONST_CHAR::wMAXITEM_AGE < fAge );
}

void CItemDrop::Update ( float fElapsedTime )
{
	fAge += fElapsedTime;

	//	Note : 임시 소유자 유효성 검사.
	//
	if ( dwHoldGID!=GAEAID_NULL )
	{
		if ( GLCONST_CHAR::wMAXITEM_HOLD < fAge )
		{
			switch ( emGroup )
			{
			case EMGROUP_ONE:
				dwHoldGID = GAEAID_NULL;
				break;

			case EMGROUP_PARTY:
				dwHoldGID = PARTY_NULL;
				break;

			case EMGROUP_GUILD:
				break;
			};
		}
	}	
}

bool CItemClientDrop::IsCollision ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt ) const
{
	BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, vMax, vMin );

	return FALSE != bCol;
}

bool CItemClientDrop::IsCollision ( CLIPVOLUME &cv ) const
{
	BOOL bCol = COLLISION::IsCollisionVolume ( cv, vMax, vMin );

	return FALSE != bCol;
}

HRESULT CItemClientDrop::RenderItem ( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matWld, D3DMATERIALQ* pMaterials )
{
	if ( pSimMesh )
	{
		pSimMesh->RenderItem ( pd3dDevice, matWld, pMaterials );
	}

	return S_OK;
}

BOOL SITEMCUSTOM::isExistStat( BYTE _ctype, short _cval )
{
	if ( cOptTYPE1 == _ctype && nOptVALUE1 == _cval )	return TRUE;
	if ( cOptTYPE2 == _ctype && nOptVALUE2 == _cval )	return TRUE;
	if ( cOptTYPE3 == _ctype && nOptVALUE3 == _cval )	return TRUE;
	if ( cOptTYPE4 == _ctype && nOptVALUE4 == _cval )	return TRUE;

	return FALSE;
}

