#include "stdafx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/**
* 새로운 캐릭터를 생성한다.
*/
int COdbcManager::CreateNewCharacter(SCHARDATA2* pCharData2)
{
	SQLRETURN	sReturn = 0;

	DWORD dwUserNum = pCharData2->GetUserID(); // 사용자번호
	DWORD dwSvrNum  = pCharData2->m_dwServerID; // 서버그룹번호	

	TCHAR szTemp[1024] = {0};
	_snprintf( szTemp, 1024, "INSERT INTO ChaInfo(UserNum, SGNum, ChaName, ChaTribe, ChaClass, "
		"ChaSchool, ChaHair, ChaFace, ChaLiving, "
		"ChaBright, ChaLevel, ChaMoney, ChaDex, ChaIntel, "
		"ChaStrong, ChaPower, ChaSpirit, ChaStrength, ChaStRemain, "
		"ChaAttackP, ChaDefenseP, ChaFightA, ChaShootA, ChaExp, "
		"ChaSkillPoint, ChaHP, ChaMP, ChaSP, ChaPK, "
		"ChaStartMap, ChaStartGate, ChaPosX, ChaPosY, ChaPosZ, "
		"ChaHairColor, ChaSex, ChaReExp, ChaScaleRange, ChaCP, "
		"ChaBadge, ChaEquipmentLockEnable, ChaEquipmentLockStatus, ChaInventoryLockEnable, ChaInventoryLockStatus, ChaStorageLockEnable, ChaStorageLockStatus,"
		"ChaSkills, ChaSkillSlot, ChaActionSlot, ChaPutOnItems, ChaInven, ChaQuest, ChaItemFood,"
		"ChaActivity, ChaPKScore, ChaPKDeath ) Values ( "
		"%u,%u,'%s',%d,%d,%u,%u,%u,%d,"
		"%d,%u,%I64d,%u,%u,%u,%u,%u,%u,%u,"
		"%u,%u,%u,%u,%I64d,%u,%u,%u,%u,%u,"
		"%u,%u,%f,%f,%f,%u,%u,%I64d,%f,%u,"
		"'%s',%u,%u,%u,%u,%u,%u,"
		"'','','','','','','',"
		"'',%u,%u )", //pk score
		dwUserNum,
		dwSvrNum,
		pCharData2->m_szName,
		pCharData2->m_emTribe,
		pCharData2->m_emClass,

		pCharData2->m_wSchool,
		pCharData2->m_wHair,
		pCharData2->m_wFace,
		pCharData2->m_nLiving,

		pCharData2->m_nBright,
		pCharData2->m_wLevel,
		pCharData2->m_lnMoney,
		pCharData2->m_sStats.wDex,
		pCharData2->m_sStats.wInt,

		pCharData2->m_sStats.wStr,
		pCharData2->m_sStats.wPow,
		pCharData2->m_sStats.wSpi,
		pCharData2->m_sStats.wSta,
		pCharData2->m_wStatsPoint,

		pCharData2->m_wAP,
		pCharData2->m_wDP,
		pCharData2->m_wPA,
		pCharData2->m_wSA,
		pCharData2->m_sExperience.lnNow,

		pCharData2->m_dwSkillPoint,
		pCharData2->m_sHP.dwData,
		pCharData2->m_sMP.dwData,
		pCharData2->m_sSP.dwData,
		pCharData2->m_wPK,

		pCharData2->m_sStartMapID.dwID,
		pCharData2->m_dwStartGate,
		pCharData2->m_vStartPos.x,
		pCharData2->m_vStartPos.y,
		pCharData2->m_vStartPos.z,

		pCharData2->m_wHairColor,
		pCharData2->m_wSex,
		pCharData2->m_lnReExp,
		pCharData2->m_fScaleRange,
		pCharData2->m_sCombatPoint.wNow,	/*combatpoint logic, Juver, 2017/05/27 */
		pCharData2->m_szBadge,				/*activity system, Juver, 2017/11/04 */

		pCharData2->m_bEnableEquipmentLock,	/*equipment lock, Juver, 2018/01/13 */
		pCharData2->m_bEquipmentLockStatus,	/*equipment lock, Juver, 2018/01/13 */

		pCharData2->m_bEnableInventoryLock,	/*inventory lock, DevArt22, 2020/02/11 */
		pCharData2->m_bInventoryLockStatus,	/*inventory lock, DevArt22, 2020/02/11 */

		pCharData2->m_bEnableStorageLock,	/*storage lock, DevArt22, 2020/02/11 */
		pCharData2->m_bStorageLockStatus,	/*storage lock, DevArt22, 2020/02/11 */
		
		pCharData2->m_dwPKScore,
		pCharData2->m_dwPKDeath
		
		
		); 

	ODBC_STMT* pConn2 = m_pGameDB->GetConnection();
	if (!pConn2) return DB_ERROR;

	sReturn = ::SQLExecDirect(pConn2->hStmt,
		(SQLCHAR*)szTemp, 
		SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
//		Print(szTemp);		
//		Print(GetErrorString(pConn2->hStmt));
		m_pGameDB->FreeConnection(pConn2);

		//strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		return DB_ERROR;
	}

	// 중복캐릭터가 있음
	if (sReturn == SQL_ERROR) 
	{
		// Print(GetErrorString(pConn2->hStmt));
		m_pGameDB->FreeConnection(pConn2);

		//strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		return DB_CHA_DUF;
	}
	// m_pGameDB->FreeStmt(pStmt2);

	// 캐릭터 생성성공
	// 생성한 캐릭터의 고유번호를 가져온다.
	// ChaNameInfo Table의 트리거로 인해서 SELECT cast(SCOPE_IDENTITY() as int) 구문 사용
	/*
	sReturn = ::SQLExecDirect(pConn2->hStmt,
		(SQLCHAR*) "SELECT @@IDENTITY", 
		SQL_NTS);
	*/
		sReturn = ::SQLExecDirect(pConn2->hStmt,
		(SQLCHAR*) "SELECT cast(SCOPE_IDENTITY() as int)", 
		SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(GetErrorString(pConn2->hStmt));
		m_pGameDB->FreeConnection(pConn2);

		//strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		return DB_ERROR;
	}

	SQLINTEGER nChaNewNum=0, cbChaNewNum=SQL_NTS;

	while(true)
	{
		sReturn = ::SQLFetch(pConn2->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp);		
			Print(GetErrorString(pConn2->hStmt));
			m_pGameDB->FreeConnection(pConn2);

			//strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
			return DB_ERROR;
		}
		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{				
			::SQLGetData(pConn2->hStmt, 1, SQL_C_LONG, &nChaNewNum, 0, &cbChaNewNum);
			pCharData2->m_dwCharID = nChaNewNum;			
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	//strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

	m_pGameDB->FreeConnection(pConn2);

	CByteStream ByteStream;
	LPBYTE pBuffer = NULL;
	DWORD dwSize = 0;

	// Character Skill 
	pCharData2->GETEXPSKILLS_BYBUF(ByteStream);		
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{	
		//std::strstream strSkillls;
		//strSkillls << "UPDATE ChaInfo SET ChaInfo.ChaSkills=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strSkillls << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaSkills=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strSkillls.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}	

	// Character Skill Quick Slot
	pCharData2->GETSKILL_QUICKSLOT(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strSkillSlot;
		//strSkillSlot << "UPDATE ChaInfo SET ChaInfo.ChaSkillSlot=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strSkillSlot << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaSkillSlot=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strSkillSlot.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}

	// Character Action Quick Slot
	pCharData2->GETACTION_QUICKSLOT(ByteStream);		
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strActionSlot;
		//strActionSlot << "UPDATE ChaInfo SET ChaInfo.ChaActionSlot=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strActionSlot << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaActionSlot=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strActionSlot.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}

	// Character Put on item
	pCharData2->GETPUTONITEMS_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strPutOnItems;
		//strPutOnItems << "UPDATE ChaInfo SET ChaInfo.ChaPutOnItems=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strPutOnItems << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaPutOnItems=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strPutOnItems.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}

	// Character Inventory
	ByteStream.ClearBuffer();
	pCharData2->GETINVENTORYE_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);	
	if (pBuffer != NULL)
	{
		//std::strstream strInven;
		//strInven << "UPDATE ChaInfo SET ChaInfo.ChaInven=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strInven << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaInven=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strInven.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}

	// Character Quest information
	pCharData2->GETQUESTPLAY(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strQuest;
		//strQuest << "UPDATE ChaInfo SET ChaInfo.ChaQuest=? WHERE (ChaNum=" <<  nChaNewNum << ")";
		//strQuest << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaQuest=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage(szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize);

		//strQuest.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		pBuffer = NULL;
	}

	/*itemfood system, Juver, 2017/05/26 */
	pCharData2->GETITEMFOOD_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaItemFood=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage( szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize );
		pBuffer = NULL;
	}	

	/*activity system, Juver, 2017/10/23 */
	pCharData2->GETACTIVITY_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaActivity=? WHERE (ChaNum=%d)", nChaNewNum );
		m_pGameDB->WriteImage( szTemp, nChaNewNum, (BYTE *) pBuffer, dwSize );
		pBuffer = NULL;
	}	

	// User Inventory
	// 1. Check User Inven		
	// 2. If exist skip
	// 3. not exist, write iventory image
	bool bInven = CheckInven(CCfg::GetInstance()->GetServerGroup(), dwUserNum);
	if (!bInven)
	{
		MakeUserInven(CCfg::GetInstance()->GetServerGroup(), dwUserNum);
	}

#if defined ( TW_PARAM ) || defined ( HK_PARAM ) 
	// 대만,홍콩
	// 캐릭터 생성시 Temp Table에 캐릭터 레벨과 금액 정보를 암호화 해서 저장해 두는 부분
	int nReturn = InsertCharInfoTemp( nChaNewNum );
#endif

	return nChaNewNum;
}


// 대만,홍콩
// 캐릭터 생성시 Temp Table에 캐릭터 레벨과 금액 정보를 암호화 해서 저장해 둔다.
int	COdbcManager::InsertCharInfoTemp( int nChaNum )
{
	ODBC_STMT* pConn2 = m_pGameDB->GetConnection();
	if (!pConn2) return DB_ERROR;
	SQLRETURN	sReturn = 0;

	char szTempMoney[CHAR_TEMPMONEY];
	LONGLONG lnTempMoney = 0;
	lnTempMoney <<= 0x04;
	sprintf( szTempMoney, "%I64d", lnTempMoney );
	size_t i;
	for( i = 0; i < strlen(szTempMoney); i++ )
	{
		szTempMoney[i] += 20;
	}

	int ChaNum = nChaNum;
	int nChaLevel = 1;	

	TCHAR szTemp[256] = {0};

	_snprintf( szTemp, 256, "INSERT INTO ChaLastInfo(ChaNum, ChaLevel, ChaMoney )"
		" Values( %d,%d,'%s' )", ChaNum, nChaLevel, szTempMoney );

	sReturn = ::SQLExecDirect(pConn2->hStmt,
		(SQLCHAR*)szTemp, 
		SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(GetErrorString(pConn2->hStmt));
		m_pGameDB->FreeConnection(pConn2);

		return DB_ERROR;
	}

	return DB_OK;
}