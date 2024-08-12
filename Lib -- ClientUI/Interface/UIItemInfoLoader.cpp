#include "StdAfx.h"
#include "UIInfoLoader.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "GLItemDef.h"
#include "GLItem.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NS_ITEMINFO
{
    SITEMCUSTOM	m_sItemCustomBACK;
	BOOL		m_bShopOpenBACK;
    BOOL		m_bInMarketBACK;
	BOOL		m_bInPrivateMarketBACK;
	BOOL		m_bIsWEAR_ITEMBACK;
	WORD		m_wPosXBACK;
	WORD		m_wPosYBACK;

	 /*npc shop, Juver, 2017/07/26 */
	BOOL		m_bNPCShopBACK;
	WORD		m_wNPCShopTypeBACK;

	/*item link, Juver, 2017/07/31 */
	BOOL		m_bItemLinkBACK;
	BOOL		m_bInInventoryBACK;

	/*product item, Juver, 2017/10/15 */
	BOOL		m_bInCompoundResultBACK;
	BOOL		m_bCompoundGenerateRandomOptionBACK;

	void	RESET ()
	{
		m_sItemCustomBACK.sNativeID = NATIVEID_NULL ();
		m_bShopOpenBACK = FALSE;
		m_bInMarketBACK = FALSE;
		m_bInPrivateMarketBACK = FALSE;
		m_bIsWEAR_ITEMBACK = FALSE;

		 /*npc shop, Juver, 2017/07/26 */
		m_bNPCShopBACK = FALSE;
		m_wNPCShopTypeBACK = 0;

		/*item link, Juver, 2017/07/31 */
		m_bItemLinkBACK = FALSE;
		m_bInInventoryBACK = FALSE;

		/*product item, Juver, 2017/10/15 */
		m_bInCompoundResultBACK = FALSE;
		m_bCompoundGenerateRandomOptionBACK = FALSE;

	}

	void AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_NOSPLIT ( strText, dwColor );
	}

	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_LONGESTLINE_SPLIT ( strText, dwColor );
	}

	void RemoveAllInfo ()
	{
		CInnerInterface::GetInstance().CLEAR_TEXT ();
	}

	char*	GetNumberWithSign ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "(+%d)", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "(%d)", nValue );
		}

		return szNumber;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign ( nAddon ) );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void APPEND_ITEM_GRADE ( CString& strOrigin, BYTE uGRADE )
	{
		if ( !uGRADE ) return ;

		CString strGRADE;
		strGRADE.Format ( "[+%d]", uGRADE );
		strOrigin += strGRADE;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign( nAddon ) );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void AddInfoItemAddonRange ( int nBasicMin, int nBasicMax, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;

		if ( nAddon )
		{
			Text.Format ( "%s:%d%s~%d%s", strFormat, nBasicMin, GetNumberWithSign( nAddon ), nBasicMax, GetNumberWithSign( nAddon ) );
            APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasicMin || nBasicMax )
		{
			Text.Format ( "%s:%d~%d", strFormat, nBasicMin, nBasicMax );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void AddItemTurnInfo ( const SITEMCUSTOM &sItemCustom, const BOOL bInMarket, const BOOL bInPrivateMarket )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		CString strText;

		//switch ( pItemData->sBasicOp.emItemType )
		//{
		//case ITEM_CHARM:
		//case ITEM_ARROW:
		//case ITEM_CURE:
		//case ITEM_GRINDING:
			if ( pItemData->sDrugOp.wPileNum > 1 )
			{
				WORD wPileNum = pItemData->sDrugOp.wPileNum;
				WORD wTurnNum = sItemCustom.wTurnNum;				
				if ( bInPrivateMarket )	//	개인 상점
				{
					strText.Format("%s:%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum);
					AddTextNoSplit(strText,NS_UITEXTCOLOR::CHARTREUSE);

					return ;
				}

				if ( bInMarket )	wTurnNum = pItemData->GETAPPLYNUM();
				strText.Format("%s:%d/%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum, wPileNum);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
			}
			//break;
		//}
	}

	void AddInfoPetSkillItem( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : 스킬 정보 가져옴.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	기본 정보
			//{
			//	//	3. 속성
			//	strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
			//	AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);
			//}

			//{
			//	//	목표 가능거리
			//	const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			//	if ( sBASIC.wTARRANGE )
			//	{
			//		strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
			//		AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//	}
			//}

			//	이미 배운 스킬
			if ( pPetClient->ISLEARNED_SKILL( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}
		}
	}

	void AddInfoSkillItem ( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : 스킬 정보 가져옴.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	기본 정보
			{
				//	2. 등급
				strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO", 1), pSkill->m_sBASIC.dwGRADE);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	3. 속성
				strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	4. 직업
				strText.Format ("%s:", ID2GAMEWORD("SKILL_BASIC_INFO", 3) );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_BRAWLER_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[0].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_BRAWLER_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[6].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SWORDSMAN_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[1].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SWORDSMAN_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[7].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[8].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[2].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SHAMAN_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[9].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SHAMAN_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[3].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[4].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[5].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_GUNNER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[10].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_GUNNER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[11].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASSIN_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[12].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASSIN_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[13].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TRICKER_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[14].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TRICKER_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[15].c_str() );

				AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqCharClass & pCharacter->m_emClass ) );
			}

			{
				//	목표 가능거리, 왼손, 오른손 도구
				const SKILL::SSKILLBASIC& sBASIC = pSkill->m_sBASIC;		

				if ( sBASIC.wTARRANGE )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				if ( (sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != SKILLATT_NOCARE)) || 
					(sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != SKILLATT_NOCARE)) )
				{
					AddTextNoSplit(ID2GAMEWORD("SKILL_CATEGORY", 1),NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}

				CString strUSEITEM;
				if ( sBASIC.emUSE_LITEM )
				{
					if ( sBASIC.emUSE_LITEM != SKILLATT_NOCARE )
					{
						strText.Format( "%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), COMMENT::SKILLATTACK[sBASIC.emUSE_LITEM].c_str() );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}

				if ( sBASIC.emUSE_RITEM )
				{
					if ( sBASIC.emUSE_RITEM != SKILLATT_NOCARE )
					{
						strText.Format ( "%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), COMMENT::SKILLATTACK[sBASIC.emUSE_RITEM].c_str() );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			//	이미 배운 스킬
			if ( pCharacter->GETLEARNED_SKILL ( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}

			//	요구치 정보
			{
				AddTextNoSplit( ID2GAMEWORD("SKILL_CATEGORY", 7), NS_UITEXTCOLOR::LIGHTSKYBLUE);

				const WORD wLevel = 0;
				SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
				SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wLevel];
			
				//	1. 요구보유스킬
				SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
				if ( NeedSkillID != NATIVEID_NULL() )
				{
					BOOL bVALID = FALSE;
					BOOL bNeedSkillLevel = FALSE;

					CString strNeedSkillName;
					CString strNeedSkillLevel;

					PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
					strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
					bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

					//	2. 요구보유스킬레벨
					if ( 0 < sLVL.dwSKILL_LVL )
					{
						strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
						bNeedSkillLevel = TRUE;

						SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
						SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
						if ( iter != map.end() )
						{
							SCHARSKILL& rCharSkill = (*iter).second;

							//	색 조절
							bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
						}
					}

					AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

					if ( bNeedSkillLevel )
						AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
				}

				//	3. 요구경험치
				if ( 0 < sLVL.dwSKP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
				}

				//	4. 요구레벨
				if ( 0 < sLVL.dwLEVEL )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) ) );
				}

				//	5. 조건 - 암광
				BOOL bValue = TRUE;
				strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
				if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
				{
					if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
					{
						bValue = FALSE;
					}
				}
				AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


				/*UserNum ItemReq, Juver, 2017/06/27 */
				if ( pItemData->sBasicOp.dwReqUserNum != 0 )
				{
					strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 0 ), pItemData->sBasicOp.dwReqUserNum );
					AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqUserNum == pCharacter->GetUserID() ));
				}

				//	Stats
				//	1. 요구힘
				if ( 0 < sLVL.sSTATS.wPow )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.wPow);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wPow >= sLVL.sSTATS.wPow ) );
				}

				//	2. 요구체력
				if ( 0 < sLVL.sSTATS.wStr )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.wStr);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wStr >= sLVL.sSTATS.wStr ) );
				}

				//	3. 요구정신
				if ( 0 < sLVL.sSTATS.wSpi )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.wSpi);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wSpi >= sLVL.sSTATS.wSpi ));
				}

				//	4. 요구민첩
				if ( 0 < sLVL.sSTATS.wDex )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.wDex);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wDex >= sLVL.sSTATS.wDex ) );
				}

				//	5. 요구지력
				if ( 0 < sLVL.sSTATS.wInt )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.wInt);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wInt >= sLVL.sSTATS.wInt ) );
				}

				//	6. 요구근력
				if ( 0 < sLVL.sSTATS.wSta )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.wSta);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wPow >= sLVL.sSTATS.wSta ) );
				}
			}
		}
	}

	void AddTextAddValue( const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText;
		BOOL bLEAST_HAVE_ONE(TRUE);
		BOOL bADD_HP(FALSE), bADD_MP(FALSE), bADD_SP(FALSE), bADD_MA(FALSE);
		INT arrVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;

			if ( emTYPE != EMADD_NONE )
			{
				//	NOTE
				//		가산효과가 존재할경우에만 타이틀을
				//		출력하기 위해, 적어도 하나가 존재할때
				//		뿌린다는 것을 체크한다.
				if ( bLEAST_HAVE_ONE )
				{
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 10 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bLEAST_HAVE_ONE = FALSE;
				}

				int nVALUE = sSUIT.sADDON[i].nVALUE;

				switch ( emTYPE )
				{
				case EMADD_HP:
					if( !bADD_HP )
					{
						arrVALUE[EMADD_HP] = sItemCustom.GETADDHP();
						bADD_HP = TRUE;
					}
					break;
				case EMADD_MP:
					if( !bADD_MP )
					{
						arrVALUE[EMADD_MP] = sItemCustom.GETADDMP();
						bADD_MP = TRUE;
					}
					break;
				case EMADD_SP:
					if( !bADD_SP )
					{
						arrVALUE[EMADD_SP] = sItemCustom.GETADDSP();
						bADD_SP = TRUE;
					}
					break;
				case EMADD_MA:
					if( !bADD_MA )
					{
						arrVALUE[EMADD_MA] = sItemCustom.GETADDMA();
						bADD_MA = TRUE;
					}
					break;
				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
				case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
				};
			}
		}

		for( int i=1; i<EMADD_SIZE; ++i)
		{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );
				AddTextNoSplit( strText, NS_UITEXTCOLOR::ORANGE );
			}
		}
	}

	/*costume combine stats, Juver, 2017/09/01 */
	void AddTextAddValueCostume( SITEM* pItemDisguiseCombine )
	{
		if ( pItemDisguiseCombine )
		{
			CString strText;
			BOOL bLEAST_HAVE_ONE(TRUE);
			INT arrVALUE[EMADD_SIZE];
			SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

			for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
			{
				EMITEM_ADDON emTYPE = pItemDisguiseCombine->sSuitOp.sADDON[i].emTYPE;

				if ( emTYPE != EMADD_NONE )
				{
					if ( bLEAST_HAVE_ONE )
					{
						AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY_NEWINFO", 0 ),NS_UITEXTCOLOR::LIGHTSKYBLUE );
						bLEAST_HAVE_ONE = FALSE;
					}

					int nVALUE = pItemDisguiseCombine->sSuitOp.sADDON[i].nVALUE;

					switch ( emTYPE )
					{
					case EMADD_HP:			arrVALUE[EMADD_HP] += nVALUE;		break;
					case EMADD_MP:			arrVALUE[EMADD_MP] += nVALUE;		break;
					case EMADD_SP:			arrVALUE[EMADD_SP] += nVALUE;		break;
					case EMADD_MA:			arrVALUE[EMADD_MA] += nVALUE;		break;
					case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
					case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
					case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
					case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
					case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
					case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
					case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
					case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
					case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
					case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
					case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
					case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
					};
				}
			}

			for( int i=1; i<EMADD_SIZE; ++i)
			{
				if( arrVALUE[i] != 0 )
				{
					strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::ORANGE );
				}
			}
		}
	}

	void LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom )
	{
		if ( m_sItemCustomBACK == sItemCustom ) return ;

		m_sItemCustomBACK = sItemCustom;

		RemoveAllInfo();

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( !pItemData ) return ;
		
		DWORD dwLevel = pItemData->sBasicOp.emLevel;
		//	이름
		AddTextNoSplit ( pItemData->GetName(), COMMENT::ITEMCOLOR[dwLevel] );
	}

	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	void LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
		const BOOL bNPCShop /*= FALSE*/, const WORD wNPCShopType /*= 0*/, const BOOL bItemLink /*= FALSE*/, const BOOL bInInventory /*= FALSE*/, BOOL bInCompoundResult /*= FALSE*/, BOOL bCompoundGenerateRandomOption /*= FALSE*/  )  
	{	
		 /*npc shop, Juver, 2017/07/26 */
		/*item link, Juver, 2017/07/31 */
		/*product item, Juver, 2017/10/15 */
		if ( m_sItemCustomBACK == sItemCustom_base && m_bShopOpenBACK == bShopOpen
			&& m_bInMarketBACK == bInMarket && m_bInPrivateMarketBACK == bInPrivateMarket
			&& m_bIsWEAR_ITEMBACK == bIsWEAR_ITEM && m_wPosXBACK == wPosX && m_wPosYBACK == wPosY
			&& m_bNPCShopBACK == bNPCShop && m_wNPCShopTypeBACK == wNPCShopType
			&& m_bItemLinkBACK == bItemLink && m_bInInventoryBACK == bInInventory
			&& m_bInCompoundResultBACK == bInCompoundResult &&  m_bCompoundGenerateRandomOptionBACK == bCompoundGenerateRandomOption )	
			return ;

		m_sItemCustomBACK = sItemCustom_base;
		m_bShopOpenBACK = bShopOpen;
		m_bInMarketBACK = bInMarket;
		m_bInPrivateMarketBACK = bInPrivateMarket;
		m_bIsWEAR_ITEMBACK = bIsWEAR_ITEM;
		m_wPosXBACK = wPosX;
		m_wPosYBACK = wPosY;

		/*npc shop, Juver, 2017/07/26 */
		m_bNPCShopBACK = bNPCShop; 
		m_wNPCShopTypeBACK = wNPCShopType;

		/*item link, Juver, 2017/07/31 */
		m_bItemLinkBACK = bItemLink;
		m_bInInventoryBACK = bInInventory;

		/*product item, Juver, 2017/10/15 */
		m_bInCompoundResultBACK = bInCompoundResult;
		m_bCompoundGenerateRandomOptionBACK = bCompoundGenerateRandomOption;

		RemoveAllInfo();

		/*item wrapper, Juver, 2018/01/12 */
		SITEMCUSTOM sItemCustom = sItemCustom_base;

		/*item link, Juver, 2017/07/31 */
		if ( bItemLink )
		{
			AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
		}

		if ( bIsWEAR_ITEM )
		{
			AddTextNoSplit ( ID2GAMEWORD ("WEAR_ITEM" ), NS_UITEXTCOLOR::RED );
		}

		CString strText, strText2;
		BOOL bValue = FALSE;
		int nExtraValue = 0;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		/*item wrapper, Juver, 2018/01/12 */
		BOOL bWrapped = FALSE;
		std::string strorigname = "";
		if ( pItemData && pItemData->sBasicOp.emItemType == ITEM_WRAPPER_BOX && sItemCustom.nidDISGUISE != NATIVEID_NULL() )
		{
			strorigname = pItemData->GetName();
			bWrapped = TRUE;
		}

		
		if ( bWrapped )
		{
			pItemData = NULL;
			pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
			sItemCustom.sNativeID = sItemCustom.nidDISGUISE;
			sItemCustom.nidDISGUISE = NATIVEID_NULL();
			sItemCustom.tDISGUISE = 0;
		}

		if ( !pItemData )
		{
			bWrapped = FALSE;
			pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		}

		{
			AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 0 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//	BasicInfo
			//	1.아이템 이름		
			BOOL bInsert = FALSE;

			/*item wrapper, Juver, 2018/01/12 */
			if ( bWrapped && strorigname.size() )
				strText.Format ( "%s:%s %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 0 ), strorigname.c_str(), pItemData->GetName() );
			else
				strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 0 ), pItemData->GetName() );

			BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NONE);
			APPEND_ITEM_GRADE ( strText, uGRADE );
			AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			
			DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;
//			AddTextNoSplit ( strText, COMMENT::ITEMCOLOR[dwLevel] );

			//	코스춤
			if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				SITEM* pDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );

				strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ), pDisguiseData->GetName() );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::GREENYELLOW );
			}

			//	코스츔 기간 표시
			if ( sItemCustom.tDISGUISE!=0 )
			{				
				CTime cTime(sItemCustom.tDISGUISE);
				if ( cTime.GetYear()!=1970 )
				{
					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
																				(cTime.GetYear ()%2000), 
																				cTime.GetMonth (), 
																				cTime.GetDay (), 
																				cTime.GetHour (), 
																				cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 7 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			DWORD dwCOMMISSION_MONEY = 0;

			//	2.가격
			if ( pItemData->sBasicOp.dwBuyPrice || bInPrivateMarket )
			{
				if ( bInPrivateMarket )
				{
					bool bOPENER;
					DWORD dwPrivateMarketID;
					CInnerInterface::GetInstance().GetPrivateMarketInfo ( bOPENER, dwPrivateMarketID );
					if ( bOPENER )	//	파는 사람
					{
						GLPrivateMarket &sPMarket = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );			
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}						
					}
					else		//	사는 사람
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwPrivateMarketID );
						if ( !pCLIENT ) return ;

						GLPrivateMarket &sPMarket = pCLIENT->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );			
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}
					}
				}				
				else if ( bShopOpen && bInMarket )	// 살때 가격
				{
					LONGLONG dwNpcSellPrice = 0;

					volatile LONGLONG dwPrice = 0;
					volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
					volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

					if( sNpcNativeID.wMainID != 0 && sNpcNativeID.wSubID != 0 )
					{
						PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNpcNativeID );
						if( pCrowData != NULL )
						{
							LONGLONG dwNpcPrice = pCrowData->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
							if( dwNpcPrice == 0 )
							{								
								dwNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
								dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
							}else{
								dwNpcSellPrice = dwNpcPrice;
								dwPrice = dwNpcSellPrice;								
							}
						}

					}
					
					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

					//	커미션 액수
					dwCOMMISSION_MONEY = dwPrice - dwNpcSellPrice;
					
					D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
					if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
					{
						dwColor = NS_UITEXTCOLOR::PALEGREEN;
					}
					AddTextNoSplit ( strText, dwColor );
				}
				else if ( bShopOpen ) // 팔때 가격
				{
					volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetSaleRate();
					volatile float fSALE_DISCOUNT = fSHOP_RATE * 0.01f;

					volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
					volatile DWORD dwSALE_PRICE = DWORD ( dwPrice * fSALE_DISCOUNT );					

					//	커미션 액수
					volatile DWORD dwDISPRICE = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
					dwCOMMISSION_MONEY = dwDISPRICE - dwSALE_PRICE;

					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwSALE_PRICE, 3, "," );
					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

					AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
				}
				/*npc shop, Juver, 2017/07/26 */
				else if( bNPCShop )
				{
					//from npc shop
					volatile LONGLONG dwPrice = pItemData->sBasicOp.dwBuyPrice;

					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );
					strText.Format( "%s:%s", ID2GAMEWORD("NPC_SHOP_TYPE_PRICE", wNPCShopType ), strMoney );

					D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
					if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
					{
						dwColor = NS_UITEXTCOLOR::PALEGREEN;
					}
					AddTextNoSplit ( strText, dwColor );
				}

			}

			//	수수료 표시
			if ( dwCOMMISSION_MONEY )
			{
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwCOMMISSION_MONEY, 3, "," );
				strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 6), strMoney );

				AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			}

			//	수량표시
			AddItemTurnInfo ( sItemCustom, bInMarket, bInPrivateMarket );

			//	기간표시
			if ( pItemData->IsTIMELMT () )
			{
				CTime cTime(sItemCustom.tBORNTIME);
				if ( cTime.GetYear()!=1970 )
				{
					CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
					cTime += sLMT;

					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString ( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
						(cTime.GetYear ()%2000), cTime.GetMonth (), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 4 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			//	착용조건->요구 레벨
			if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
			{
				bool bReqLevel = true;
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
									
				if ( pItemData->sBasicOp.wReqLevelUP )
				{
					strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
					strText += strText2; 
					bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
				}

				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
			}

			//	재사용 대기시간
			if ( pItemData->sBasicOp.IsCoolTime() )
			{
				CString strTime = "";
				CTimeSpan cCoolTime( pItemData->sBasicOp.dwCoolTime );
				
				if ( cCoolTime.GetHours() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cCoolTime.GetMinutes() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cCoolTime.GetSeconds() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 8 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );	
			}

			//	남은시간


			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if ( pCharacter && pCharacter->IsCoolTime( pItemData->sBasicOp.sNativeID ) )
			{
				CString strTime = "";
				__time64_t tCurTime =  GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
				__time64_t tCoolTime = pCharacter->GetMaxCoolTime( pItemData->sBasicOp.sNativeID );				

				CTimeSpan cReTime( tCoolTime - tCurTime );

                if ( cReTime.GetHours() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cReTime.GetMinutes() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cReTime.GetSeconds() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 9 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//	AdvInfo
			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;
			EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;

			switch ( emItemType )
			{
			case ITEM_SUIT:
			case ITEM_VEHICLE:
			case ITEM_REVIVE:
			case ITEM_ANTI_DISAPPEAR:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	공격력
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.wLow, sDamage.wHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	기력치
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	방어력
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	공격 거리
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	명중률
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	회피율
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	착용위치
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	공격 속성
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP 소모
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}
				
					//	착용조건-------------------------------------------------------------------------------------
					//	착용조건-착용가능 클래스
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							if ( (dwReqCharClass==GLCC_ALL_8CLASS) && (dwReqCharClass_Disguise==GLCC_ALL_8CLASS) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else if ( (dwReqCharClass==GLCC_MAN_8CLASS) && (dwReqCharClass_Disguise==GLCC_MAN_8CLASS) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_MALE_CLASS");
							}
							else if ( (dwReqCharClass==GLCC_WOMAN_8CLASS) && (dwReqCharClass_Disguise==GLCC_WOMAN_8CLASS) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_FEMALE_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_BRAWLER_M) && (dwReqCharClass_Disguise&GLCC_BRAWLER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_BRAWLER_W) && (dwReqCharClass_Disguise&GLCC_BRAWLER_W) )	{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SWORDSMAN_M)  && (dwReqCharClass_Disguise&GLCC_SWORDSMAN_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SWORDSMAN_W)  && (dwReqCharClass_Disguise&GLCC_SWORDSMAN_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SHAMAN_M)  && (dwReqCharClass_Disguise&GLCC_SHAMAN_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SHAMAN_W)  && (dwReqCharClass_Disguise&GLCC_SHAMAN_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_GUNNER_M)  && (dwReqCharClass_Disguise&GLCC_GUNNER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_GUNNER_W)  && (dwReqCharClass_Disguise&GLCC_GUNNER_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ASSASSIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ASSASSIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_TRICKER_M)  && (dwReqCharClass_Disguise&GLCC_TRICKER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_TRICKER_W)  && (dwReqCharClass_Disguise&GLCC_TRICKER_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							if ( dwReqCharClass==GLCC_ALL_8CLASS)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else if ( dwReqCharClass==GLCC_MAN_8CLASS)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_MALE_CLASS");
							}
							else if ( dwReqCharClass==GLCC_WOMAN_8CLASS)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_FEMALE_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_BRAWLER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_BRAWLER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SWORDSMAN_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SWORDSMAN_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARCHER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARCHER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SHAMAN_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SHAMAN_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_EXTREME_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_EXTREME_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_GUNNER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_GUNNER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ASSASSIN_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ASSASSIN_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_TRICKER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_TRICKER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	조건 - 암광
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	착용조건->격투치
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	착용조건->사격치
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	착용조건->힘
					if ( rItemStats.wPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.wPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wPow <= rCharStats.wPow ) );
					}
					//	착용조건->체력
					if ( rItemStats.wStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.wStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wStr <= rCharStats.wStr ) );
					}
					//	착용조건->정신력
					if ( rItemStats.wSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.wSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wSpi <= rCharStats.wSpi ) );
					}
					//	착용조건->민첩성
					if ( rItemStats.wDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.wDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wDex <= rCharStats.wDex ) );
					}
					//	착용조건->지력
					if ( rItemStats.wInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.wInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wInt <= rCharStats.wInt ) );
					}
					//	착용조건->근력
					if ( rItemStats.wSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.wSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wSta <= rCharStats.wSta ) );
					}

					/*UserNum ItemReq, Juver, 2017/06/27 */
					if ( pItemData->sBasicOp.dwReqUserNum != 0 )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 0 ), pItemData->sBasicOp.dwReqUserNum );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqUserNum == pCharacter->GetUserID() ));
					}

					//-----------------------------------------------------------------------------------------------
					//	저항값
					const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					//	상태이상
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		가산 효과
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );
					if( pItemData->sBasicOp.emItemType == ITEM_VEHICLE )
					{
						strText.Format(ID2GAMEWORD ("ITEM_VEHICLE_TYPE_INFO", 0) , COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

						strText.Format( "%s %s", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

						/*vehicle system, Juver, 2017/08/09 */
						if ( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE )
						{
							AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_BIKE[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
						}else{
							AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_HOVER[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
						}

						bool bInfo(true);


						VEHICLEITEMINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.dwVehicleID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.end() )
						{
							iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.dwVehicleID );
							if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.end() )	bInfo = false;
						}


						if ( bInfo ) 
						{
							SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;
							for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i ) 
							{
								SITEM* pItem = GLItemMan::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].sNativeID );
								if ( pItem )
								{
									strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT",i), pItem->GetName () );
									AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
								}
							}

							strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY",0), sVehicleItemInfo.m_nFull/10.0f );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

							if ( sVehicleItemInfo.m_emTYPE != VEHICLE_TYPE_BOARD )
							{
								/*vehicle booster system, Juver, 2017/08/10 */
								strText.Format( "%s", ID2GAMEWORD ("ITEM_VEHICLE_BOOST_LEARN", sVehicleItemInfo.m_bBooster ) );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}
						}
					}

					//	특수기능
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						//	특수기능 ( 변화율 )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						// 경험치 배율
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						//	특수기능 ( 변화량 )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
							
						}
					}

					/*costume combine stats, Juver, 2017/09/01 */
					SITEM*	pItemDisguiseCombine = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
					if ( pItemDisguiseCombine && pItemDisguiseCombine->sBasicOp.IsDISGUISE() )
					{
						AddTextAddValueCostume ( pItemDisguiseCombine );

						EMITEM_VAR emITEM_VAR = pItemDisguiseCombine->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemDisguiseCombine->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE))
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY_NEWINFO", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}

						if ( emITEM_VAR != EMVAR_NONE )
						{
							float  fVAR_SCALE = pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE );
						}

						if ( emITEM_VOL != EMVAR_NONE)
						{
							float fVOLUME = pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
							strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE );	
						}
					}

					// random value
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;
						
						//Add Rate Values Random Option #1 -- Ocram Networkso
						INT nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_DAMAGE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_DEFENSE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HITRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_AVOIDRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HP) ) //HP Add not VIT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_MP) ) //MP Add not INT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_SP) ) //SP Add not STA
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_MP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_SP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HMS_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_GRIND_DAMAGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_GRIND_DEFENSE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_RANGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_DIS_SP) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_RESIST) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_MOVE_SPEED) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Direct Values Random Option #1 -- Ocram Networkso

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_ATTACK_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_DEFENSE_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HIT_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_AVOID_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_POWER) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_VITALITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_SPIRIT) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_DEXTERITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_STAMINA) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_MELEE) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_SHOOTING) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_ENERGY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_HP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_MP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType1 = sItemCustom.GETOptTYPE1();
						if( (nRandOptType1 && nRandOptType1 == EMR_OPT_SP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType1 ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Rate Values Random Option #2 -- Ocram Networkso
						INT nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_DAMAGE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_DEFENSE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HITRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_AVOIDRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HP) ) //HP Add not VIT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_MP) ) //MP Add not INT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_SP) ) //SP Add not STA
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_MP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_SP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HMS_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_GRIND_DAMAGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_GRIND_DEFENSE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_RANGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_DIS_SP) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_RESIST) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_MOVE_SPEED) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Direct Values Random Option #2 -- Ocram Networkso

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_ATTACK_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_DEFENSE_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HIT_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_AVOID_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_POWER) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_VITALITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_SPIRIT) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_DEXTERITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_STAMINA) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_MELEE) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_SHOOTING) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_ENERGY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_HP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_MP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType2 = sItemCustom.GETOptTYPE2();
						if( (nRandOptType2 && nRandOptType2 == EMR_OPT_SP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType2 ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType2 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Rate Values Random Option #3 -- Ocram Networkso
						INT nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_DAMAGE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_DEFENSE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HITRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_AVOIDRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HP) ) //HP Add not VIT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_MP) ) //MP Add not INT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_SP) ) //SP Add not STA
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_MP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_SP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HMS_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_GRIND_DAMAGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_GRIND_DEFENSE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_RANGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_DIS_SP) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_RESIST) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_MOVE_SPEED) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Direct Values Random Option #3 -- Ocram Networkso

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_ATTACK_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_DEFENSE_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HIT_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_AVOID_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_POWER) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_VITALITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_SPIRIT) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_DEXTERITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_STAMINA) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_MELEE) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_SHOOTING) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_ENERGY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_HP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_MP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType3 = sItemCustom.GETOptTYPE3();
						if( (nRandOptType3 && nRandOptType3 == EMR_OPT_SP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType3 ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType3 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Rate Values Random Option #4 -- Ocram Networkso
						INT nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_DAMAGE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_DEFENSE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HITRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_AVOIDRATE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HP) ) //HP Add not VIT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_MP) ) //MP Add not INT
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_SP) ) //SP Add not STA
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_MP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_SP_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HMS_INC) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_GRIND_DAMAGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_GRIND_DEFENSE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_RANGE) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_DIS_SP) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_RESIST) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_MOVE_SPEED) ) //Rate of regeneration
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						//Add Direct Values Random Option #4 -- Ocram Networkso

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_ATTACK_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_DEFENSE_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HIT_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_AVOID_VOL) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_POWER) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_VITALITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_SPIRIT) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_DEXTERITY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_STAMINA) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_MELEE) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_SHOOTING) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_ENERGY) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_HP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_MP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType4 = sItemCustom.GETOptTYPE4();
						if( (nRandOptType4 && nRandOptType4 == EMR_OPT_SP_REC) ) //add direct value - Ocram Networkso
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType4 ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType4 ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;

			case ITEM_CHARM:
			case ITEM_ARROW:
			case ITEM_BULLET: /*gun-bullet logic, Juver, 2017/05/27 */
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					//	공격력
					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.wLow || sDAMAGE.wHigh  )
					{
						strText.Format("%s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.wLow ), 
													GetNumberWithSign ( sDAMAGE.wHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}
 

					//	NOTE
					//		가산 효과
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );					
				}
				break;

			case ITEM_CURE:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 7 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if( pItemData->sDrugOp.wCureVolume == 0 )
					{
						strText.Format("%s",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str());
					}
					else
					{
						strText.Format("%s:%d",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str(), pItemData->sDrugOp.wCureVolume);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT);
				}
				break;

			case ITEM_SKILL:
				{
					AddInfoSkillItem ( sItemCustom );
				}
				break;

			case ITEM_PET_SKILL:
				{
					AddInfoPetSkillItem( sItemCustom );
				}
				break;

			case ITEM_GRINDING:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY_EX", 0 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
				}
				break;

				// 경험치 획득 카드 정보
			case ITEM_VIETNAM_EXPGET:
				{
					if ( pItemData->sDrugOp.bRatio )
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 1) , pItemData->sDrugOp.wCureVolume );						
					}
					else
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 0), pItemData->sDrugOp.wCureVolume );
					}

					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;
				// 아이템 획득 카드 정보
			case ITEM_VIETNAM_ITEMGET:
				{
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_ITEM", 0) , pItemData->sDrugOp.wCureVolume );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;

			//case ITEM_VEHICLE:
			//	{
			//		strText.Format(ID2GAMEWORD ("ITEM_VEHICLE_TYPE_INFO", 0) , COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
			//		AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

			//		strText.Format( "%s %s", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
			//		AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

			//		/*vehicle system, Juver, 2017/08/09 */
			//		if ( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE )
			//		{
			//			AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_BIKE[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
			//		}
			//		/* vehicle car, cart, Juver, 2018/02/13 */
			//		else if( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_CAR )
			//		{
			//			AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_CAR[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
			//		}
			//		/* vehicle car, cart, Juver, 2018/02/13 */
			//		else if( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_CART )
			//		{
			//			AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_CART[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
			//		}
			//		else
			//		{
			//			AddTextNoSplit( COMMENT::VEHICLE_SUBTYPE_HOVER[pItemData->sVehicle.emVehicleSubType].c_str(), NS_UITEXTCOLOR::DEFAULT);
			//		}

			//		
			//		EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
			//		//	이동속도 변화량
			//		//
			//		if ( emITEM_VOL != EMVAR_NONE)
			//		{
			//			if ( emITEM_VOL == EMVAR_MOVE_SPEED )
			//			{
			//				float fVOLUME = sItemCustom.GETMOVESPEED();
			//				strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
			//				AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
			//			}
			//			else
			//			{
			//				float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
			//				strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
			//				AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
			//			}
			//		}


			//		bool bInfo(true);

			//		
			//		VEHICLEITEMINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.dwVehicleID );
			//		if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.end() )
			//		{
			//			iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.dwVehicleID );
			//			if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.end() )	bInfo = false;
			//		}


			//		if ( bInfo ) 
			//		{
			//			SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;
			//			for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i ) 
			//			{
			//				SITEM* pItem = GLItemMan::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].sNativeID );
			//				if ( pItem )
			//				{
			//					strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT",i), pItem->GetName () );
			//					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
			//				}
			//			}

			//			strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY",0), sVehicleItemInfo.m_nFull/10.0f );
			//			AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

			//			if ( sVehicleItemInfo.m_emTYPE != VEHICLE_TYPE_BOARD )
			//			{
			//				/*vehicle booster system, Juver, 2017/08/10 */
			//				strText.Format( "%s", ID2GAMEWORD ("ITEM_VEHICLE_BOOST_LEARN", sVehicleItemInfo.m_bBooster ) );
			//				AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
			//			}
			//		}


			//		AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

			//		DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
			//		CString szClass;
			//		
			//		if( dwReqCharClass )
			//		{
			//			if ( dwReqCharClass==GLCC_ALL_8CLASS)
			//			{
			//				szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
			//			}
			//			else
			//			{
			//				if ( dwReqCharClass & GLCC_BRAWLER_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_BRAWLER_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_SWORDSMAN_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_SWORDSMAN_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_ARCHER_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_ARCHER_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_SHAMAN_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_SHAMAN_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_EXTREME_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_EXTREME_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_GUNNER_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_GUNNER_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_ASSASSIN_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_ASSASSIN_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_TRICKER_M){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();
			//					szClass+=" ";
			//				}

			//				if ( dwReqCharClass & GLCC_TRICKER_W){
			//					szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
			//				}
			//			}

			//			strText.Format( "%s", szClass.GetString() );
			//			AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
			//		}					

			//		// Note : 랜덤 수치
			//		if( sItemCustom.IsSetRandOpt() )
			//		{
			//			CString strDescText;

			//			AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

			//			INT nRandOptType = sItemCustom.GETOptTYPE1();
			//			if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
			//			{
			//				strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

			//				float fVal = sItemCustom.GETOptVALUE1();
			//				if( fVal != 0.0f )
			//				{
			//					if( fVal > 0.0f) strDescText += _T('+');

			//					strDescText.AppendFormat( "%.2f", fVal );
			//					if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			//					AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
			//				}
			//			}

			//			nRandOptType = sItemCustom.GETOptTYPE2();
			//			if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
			//			{
			//				strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

			//				float fVal = sItemCustom.GETOptVALUE2();
			//				if( fVal != 0.0f )
			//				{
			//					if( fVal > 0.0f) strDescText += _T('+');

			//					strDescText.AppendFormat( "%.2f", fVal );
			//					if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			//					AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
			//				}
			//			}

			//			nRandOptType = sItemCustom.GETOptTYPE3();
			//			if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
			//			{
			//				strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

			//				float fVal = sItemCustom.GETOptVALUE3();
			//				if( fVal != 0.0f )
			//				{
			//					if( fVal > 0.0f) strDescText += _T('+');

			//					strDescText.AppendFormat( "%.2f", fVal );
			//					if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			//					AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
			//				}
			//			}

			//			nRandOptType = sItemCustom.GETOptTYPE4();
			//			if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
			//			{
			//				strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

			//				float fVal = sItemCustom.GETOptVALUE4();
			//				if( fVal != 0.0f )
			//				{
			//					if( fVal > 0.0f) strDescText += _T('+');

			//					strDescText.AppendFormat( "%.2f", fVal );
			//					if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

			//					AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
			//				}
			//			}
			//		}

			//	}
			//	break;

			case ITEM_PET_CARD:
				{
					if ( sItemCustom.dwPetID == 0 ) break;
					PETCARDINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.find ( sItemCustom.dwPetID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.end() )
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.find ( sItemCustom.dwPetID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.end() ) break;
					}

					SPETCARDINFO sPetCardInfo = (*iter).second;

					strText.Format(ID2GAMEWORD ("ITEM_PET_INFO", 0 ));
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_NAME",0), sPetCardInfo.m_szName );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_TYPE",0), COMMENT::PET_TYPE[sPetCardInfo.m_emTYPE].c_str() );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_PET_FULL",0), sPetCardInfo.m_nFull/10.0f );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sPetCardInfo.m_PutOnItems[PET_ACCETYPE_A].sNativeID );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD ("ITEM_PET_SLOTA",0), pItem->GetName () );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}

					pItem = GLItemMan::GetInstance().GetItem ( sPetCardInfo.m_PutOnItems[PET_ACCETYPE_B].sNativeID );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD ("ITEM_PET_SLOTB",0), pItem->GetName () );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}

					if ( sItemCustom.tDISGUISE != 0 )
					{
						CTime currentTime = GLGaeaClient::GetInstance().GetCurrentTime();
						CTime startTime   = sItemCustom.tBORNTIME;
						CTimeSpan timeSpan = currentTime - startTime;
						if( timeSpan.GetTotalSeconds() < sItemCustom.tDISGUISE )
						{
							strText.Format("[%s]", ID2GAMEWORD ("ITEM_PET_USE_SKINPACK",0) );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
							strText.Format("%s : %dsec", ID2GAMEWORD ("CLUB_BATTLE_TIME",0), sItemCustom.tDISGUISE - timeSpan.GetTotalSeconds() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
						}
					}

					AddTextNoSplit( ID2GAMEWORD ("ITEM_PET_SKILLS", 0 ), NS_UITEXTCOLOR::DEFAULT );

					PETSKILL_MAP_CITER pos = sPetCardInfo.m_ExpSkills.begin();
					PETSKILL_MAP_CITER pos_end = sPetCardInfo.m_ExpSkills.end();
					for ( ;pos != pos_end; ++pos )
					{
						const PETSKILL& sPetSkill = (*pos).second;
						PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sPetSkill.sNativeID );
						if ( pSKILL )
						{
							strText.Format("%s", pSKILL->GetName() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
						}
					}

					/*dual pet skill, Juver, 2017/12/29 */
					AddTextNoSplit( ID2GAMEWORD ("ITEM_PET_DUALSKILL", sPetCardInfo.m_bDualSkill ), NS_UITEXTCOLOR::DEFAULT );
				}
				break;

				/*rv card, Juver, 2017/11/25 */
			case ITEM_RANDOM_OPTION_CARD:
				{
					AddTextNoSplit( ID2GAMEWORD ("ITEM_RANDOM_OPTION_CARD", 0), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					SITEMCUSTOM sitem_custom_copy = sItemCustom;
					sitem_custom_copy.cOptTYPE1 = (BYTE) pItemData->sRvCard.emOption;
					sitem_custom_copy.nOptVALUE1 = (short)pItemData->sRvCard.wValue;

					INT nRandOptType = sitem_custom_copy.GETOptTYPE1();
					if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
					{
						CString strOptionText;
						if (  pItemData->sRvCard.bReplaceOpt )
							strOptionText.Format( "%s %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 3 ), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
						else 
							strOptionText.Format( "%s %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 2 ), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
						
						float fVal = sitem_custom_copy.GETOptVALUE1();
						if( fVal != 0.0f )
						{
							if( fVal > 0.0f) strOptionText += _T('+');

							strOptionText.AppendFormat( "%.2f", fVal );
							if( sitem_custom_copy.IsPerRandOpt( nRandOptType ) ) strOptionText += _T('%');

							AddTextNoSplit( strOptionText, NS_UITEXTCOLOR::PRIVATE );
						}
					}

					if ( pItemData->sRvCard.bCheckExist )
						AddTextNoSplit( ID2GAMEWORD ("ITEM_RANDOM_OPTION_CARD", 4), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					AddTextNoSplit( ID2GAMEWORD ("ITEM_RANDOM_OPTION_CARD", 1), NS_UITEXTCOLOR::LIGHTSKYBLUE );
					for( DWORD nSuit=0; nSuit<SUIT_NSIZE; ++nSuit )
					{
						if ( pItemData->sRvCard.bUseSuit[nSuit] )
							AddTextNoSplit( COMMENT::ITEMSUIT[nSuit].c_str(), NS_UITEXTCOLOR::PRIVATE );
					}
				}break;



			}
			
			if( emItemType != ITEM_PET_SKILL )
			{
				AddTextNoSplit( ID2GAMEWORD( "ITEM_CATEGORY", 8 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

				// Note : 시귀/직귀 카드에 맵이름 좌표 출력
				if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_REGEN )
				{
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	

					CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sStartMapID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sStartMapID );
					if( pMapNode )
					{
						GLLevelFile cLevelFile;
						BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
						if( bOk )
						{	
							D3DXVECTOR3 vStartPos;
							PDXLANDGATE pGate = cLevelFile.GetLandGateMan().FindLandGate( pCharacter->m_dwStartGate );
							if( pGate )
							{
								vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}
							else
							{
								pGate = cLevelFile.GetLandGateMan().FindLandGate( DWORD(0) );
								if( pGate ) vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}

							int nPosX(0), nPosY(0);
							cLevelFile.GetMapAxisInfo().Convert2MapPos( vStartPos.x, vStartPos.z, nPosX, nPosY );

							strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
							AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
						}
					}
				}
				else if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_LASTCALL )
				{
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

					if( pCharacter->m_sLastCallMapID.IsValidNativeID() )
					{
						CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sLastCallMapID ) );
						if( strName == _T("(null)") ) strName.Empty();

						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
						AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

						SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sLastCallMapID );
						if( pMapNode )
						{
							GLLevelFile cLevelFile;
							BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
							if( bOk )
							{	
								int nPosX(0), nPosY(0);
								cLevelFile.GetMapAxisInfo().Convert2MapPos( pCharacter->m_vLastCallPos.x, pCharacter->m_vLastCallPos.z, nPosX, nPosY );

								strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
								AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
							}
						}
					}
					else
					{
						AddTextNoSplit( ID2GAMEWORD("ITEM_CALL_MAP", 2 ), NS_UITEXTCOLOR::RED);
					}
				}
				else if ( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_TELEPORT )
				{
					CString strName( GLGaeaClient::GetInstance().GetMapName( pItemData->sBasicOp.sSubID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_TELEPORT_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_TELEPORT_MAP", 1 ), pItemData->sBasicOp.wPosX, pItemData->sBasicOp.wPosY );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
				}

				BOOL bAccept(FALSE);

				if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );
				}
				else
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID );
				}

				if ( bAccept )
				{
					strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 0 ) );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::ENABLE );

					WORD wNeedSP = pCharacter->CALC_ACCEPTP ( sItemCustom.sNativeID );
					if ( 0 < wNeedSP )
					{
						strText.Format ( "%s:%d", ID2GAMEWORD("ITEM_NEEDSP" ), wNeedSP );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::PALEGREEN );
					}
				}
				else
				{
					strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 1 ) );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::RED );
				}
			}

			/*contribution point, Juver, 2017/08/23 */
			if( pItemData->sBasicOp.dwReqContributionPoint )
			{
				CString strPoint;
				strPoint.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 4), pItemData->sBasicOp.dwReqContributionPoint );

				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( pItemData->sBasicOp.dwReqContributionPoint <= GLGaeaClient::GetInstance().GetCharacterLogic().m_llContributionPoint )
				{
					dwColor = NS_UITEXTCOLOR::PALEGREEN;
				}

				AddTextNoSplit ( strPoint, dwColor );
			}

			/*activity point, Juver, 2017/08/23 */
			if( pItemData->sBasicOp.dwReqActivityPoint )
			{
				CString strPoint;
				strPoint.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 3), pItemData->sBasicOp.dwReqActivityPoint );

				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( pItemData->sBasicOp.dwReqActivityPoint <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwActivityPoint )
				{
					dwColor = NS_UITEXTCOLOR::PALEGREEN;
				}

				AddTextNoSplit ( strPoint, dwColor );
			}

			{ // 설명
				const TCHAR * pszComment(NULL);

				if ( (emItemType != ITEM_SKILL) || (emItemType != ITEM_PET_SKILL) )
				{
					pszComment = pItemData->GetComment();
				}
				else
				{
					SNATIVEID sNativeID = sItemCustom.sNativeID;
					SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
					SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

					//	Note : 스킬 정보 가져옴.
					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
					if ( pSkill )
					{
						pszComment = pSkill->GetDesc();
					}
				}

				if( pszComment )
				{
					int StrLength = static_cast<int>( strlen( pszComment ) );
					if( StrLength )
					{
						AddTextNoSplit ( ID2GAMEWORD( "ITEM_CATEGORY", 9 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextLongestLineSplit( pszComment, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			/*nondrop card, Juver, 2017/11/26 */
			if ( sItemCustom.IsGM_GENITEM() )
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_GM_GEN"),  NS_UITEXTCOLOR::GOLD);
			}
			else
			{	//itemflags
				DWORD dwFlags = pItemData->sBasicOp.dwFlags;
				CString strFlag;

				bool bSale = pItemData->sBasicOp.IsSALE();
				bool bExchange = pItemData->sBasicOp.IsEXCHANGE();
				bool bLocker = bExchange;
				bool bThrow = pItemData->sBasicOp.IsTHROW();
				bool bCostume = pItemData->sBasicOp.IsDISGUISE();
				bool bTrashCan = pItemData->sBasicOp.IsGarbage();

				AddTextNoSplit( "", NS_UITEXTCOLOR::WHITE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 0 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bSale? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bSale? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 1 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bThrow? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bThrow? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 2 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bLocker? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bLocker? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 3 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bExchange? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bExchange? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 4 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bTrashCan? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bTrashCan? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );
			}

			{ /*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( pCharacter && !pCharacter->ValidWindowOpen() && 
					( emItemType == ITEM_SUIT || emItemType == ITEM_ARROW || emItemType == ITEM_CHARM || emItemType == ITEM_BULLET ) )
				{
					BOOL bUseItem = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID );

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
						bUseItem = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );

					if ( bUseItem )
						AddTextNoSplit(ID2GAMEINTEXT("INVENTOWEAR_ITEM_INFO"), NS_UITEXTCOLOR::GOLD );
				}
			}

			{	/*item preview, Juver, 2017/07/27 */
				bool bCanPreview = CInnerInterface::GetInstance().PreviewItemCheckSimple( pItemData->sBasicOp.sNativeID );
				if ( bCanPreview )
				{
					AddTextNoSplit ( ID2GAMEWORD( "ITEM_USEPREVIEW", 0 ), NS_UITEXTCOLOR::GOLD );
				}
			}

			{	/*box contents, Juver, 2017/08/30 */
				if ( emItemType == ITEM_BOX || emItemType == ITEM_PREMIUMSET )
				{
					AddTextNoSplit ( ID2GAMEINTEXT( "HELP_SHOW_ITEM_TOOLTIP" ), NS_UITEXTCOLOR::GOLD );
				}
			}

			/*item link, Juver, 2017/07/31 */
			if ( bInInventory )
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "HELP_SHOW_ITEM_LINK" ), NS_UITEXTCOLOR::GOLD );
			}

			/*product item, Juver, 2017/10/15 */
			if ( bInCompoundResult )
			{
				if ( bCompoundGenerateRandomOption )
					AddTextNoSplit ( ID2GAMEINTEXT( "COMPOUND_RANDOM_VALUE_POSSIBLE" ), NS_UITEXTCOLOR::DODGERBLUE );
				else
					AddTextNoSplit ( ID2GAMEINTEXT( "COMPOUND_RANDOM_VALUE_NOT_POSSIBLE" ), NS_UITEXTCOLOR::DODGERBLUE );
			}

			/*item color, Juver, 2018/01/10 */
			bool bColoring = pItemData->sBasicOp.bItemColor;
			SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
			if ( pitem_data_disguise && pitem_data_disguise->sBasicOp.bItemColor )
				bColoring = true;
			
			if ( bColoring )
			{
				AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_INFO_COLOR", 0 ), NS_UITEXTCOLOR::WHITE );

				{
					WORD wCOLOR = sItemCustom.wColor1;
					WORD wR, wG, wB;
					wR = wCOLOR >> 10;
					wG = wCOLOR >> 5;
					wG = wG&0x1f;
					wB = wCOLOR&0x1f;

					wR = (WORD)( wR * 8.225806f );
					wG = (WORD)( wG * 8.225806f );
					wB = (WORD)( wB * 8.225806f );

					DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_INFO_COLOR", 1 ), dwColor );
				}
				
				{
					WORD wCOLOR = sItemCustom.wColor2;
					WORD wR, wG, wB;
					wR = wCOLOR >> 10;
					wG = wCOLOR >> 5;
					wG = wG&0x1f;
					wB = wCOLOR&0x1f;

					wR = (WORD)( wR * 8.225806f );
					wG = (WORD)( wG * 8.225806f );
					wB = (WORD)( wB * 8.225806f );

					DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_INFO_COLOR", 2 ), dwColor );
				}
			}

			/*item wrapper, Juver, 2018/01/12 */
			if ( !bWrapped )
			{
				BOOL bWrappable = pItemData->isWrappable();
				if ( sItemCustom.nidDISGUISE != NATIVEID_NULL() )	bWrappable = FALSE;
				if ( !sItemCustom.IsGM_GENITEM() && pItemData->sBasicOp.IsEXCHANGE() )	bWrappable = FALSE;

				if ( bWrappable )
				{
					AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_POSSIBLE_WRAP" ), NS_UITEXTCOLOR::WHITE );
				}
			}
			else
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_POSSIBLE_UNWRAP" ), NS_UITEXTCOLOR::WHITE );
			}

			/*item transfer card, Juver, 2018/01/18 */
			if ( pItemData->sBasicOp.bItemTransfer )
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_ITEM_TRANSFER" ), NS_UITEXTCOLOR::WHITE );
			}


		}
	}
};
