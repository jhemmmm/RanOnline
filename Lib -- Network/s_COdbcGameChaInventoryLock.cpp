#include "stdafx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*inventory lock, DevArt22, 2020/02/11 */
int COdbcManager::InventoryLockEnable (DWORD dwChaNum, char* szChaName, char* szEPin )
{
	SQLRETURN	sReturn = 0;	

	CString strName (szChaName);
	strName.Trim(_T(" ")); 
	strName.Replace(_T("'"), _T("''"));

    if (strName.GetLength() >= (CHR_ID_LENGTH+1))
	{
		return DB_ERROR;
	}

	CString strEPin (szEPin);
	strEPin.Trim(_T(" ")); 
	strEPin.Replace(_T("'"), _T("''"));

	if (strEPin.GetLength() >= (CHAR_INVENTORY_LOCK_PASS_SIZE+1))
	{
		return DB_ERROR;
	}


	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_InsertInventoryPass(%d,'%s','%s',?)}", dwChaNum, strName.GetString(), strEPin.GetString() ); 

	int nReturn = m_pGameDB->ExecuteSpInt(szTemp);
	return nReturn;
}

/*inventory lock, DevArt22, 2020/02/11 */
int COdbcManager::InventoryLockInput (DWORD dwChaNum, char* szChaName, char* szEPin )
{
	SQLRETURN	sReturn = 0;	

	CString strName (szChaName);
	strName.Trim(_T(" ")); 
	strName.Replace(_T("'"), _T("''"));

	if (strName.GetLength() >= (CHR_ID_LENGTH+1))
	{
		return DB_ERROR;
	}

	CString strEPin (szEPin);
	strEPin.Trim(_T(" ")); 
	strEPin.Replace(_T("'"), _T("''"));

	if (strEPin.GetLength() >= (CHAR_INVENTORY_LOCK_PASS_SIZE+1))
	{
		return DB_ERROR;
	}

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_CheckInventoryPass(%d,'%s','%s',?)}", dwChaNum, strName.GetString(), strEPin.GetString() ); 

	int nReturn = m_pGameDB->ExecuteSpInt(szTemp);
	return nReturn;
}