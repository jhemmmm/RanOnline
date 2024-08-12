#include "stdafx.h"
#include "./DbActionLogic.h"
#include "./G-Logic/GLGaeaServer.h"
#include "./G-Logic/GLChar.h"

#include "../Lib -- Network/s_CSessionServer.h"
#include "../Lib -- Network/s_CFieldServer.h"
#include "../Lib -- Network/s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInventoryLockEnable::CInventoryLockEnable( DWORD dwClientID, DWORD dwChaNum, const char* szChaName, const char* szEPin )
{	
	m_dwClient = dwClientID;
	m_dwChaNum = dwChaNum;

	memset(m_szChaName, 0, sizeof(char) * CHR_ID_LENGTH+1);
	memset(m_szEPin, 0, sizeof(char) * CHAR_INVENTORY_LOCK_PASS_SIZE+1);

	if (szChaName != NULL)
		StringCchCopy(m_szChaName, CHR_ID_LENGTH+1, szChaName);

	if (szEPin != NULL)
		StringCchCopy(m_szEPin, CHAR_INVENTORY_LOCK_PASS_SIZE+1, szEPin);
}

int CInventoryLockEnable::Execute (CServer* pServer)
{
	if (pServer == NULL)
	{
		return DB_ERROR;
	}
	else
	{
		int nResult = m_pDbManager->InventoryLockEnable(m_dwChaNum, m_szChaName, m_szEPin );

		GLMSG::SNETPC_INVEN_INVENTORY_LOCK_ENABLE_FROM_DB NetMsgDB2Fld;
		StringCchCopy ( NetMsgDB2Fld.szName, CHAR_SZNAME+1, m_szChaName );
		StringCchCopy ( NetMsgDB2Fld.szPin, CHAR_INVENTORY_LOCK_PASS_SIZE+1, m_szEPin );

		if ( nResult == DB_OK ) 
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_ENABLE_DONE;
		}
		else if( nResult == 1 )
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_ENABLE_ALREADY_ENABLED_DB;
		}
		else if( nResult == 2 )
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_ENABLE_CHAR_NOT_EXIST;
		}
		else 
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_ENABLE_FAILED;
		}

		CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
		pTemp->InsertMsg(m_dwClient, (char*) &NetMsgDB2Fld );
		return nResult;
	}
}

CInventoryLockInput::CInventoryLockInput( DWORD dwClientID, DWORD dwChaNum, const char* szChaName, const char* szEPin )
{	
	m_dwClient = dwClientID;
	m_dwChaNum = dwChaNum;

	memset(m_szChaName, 0, sizeof(char) * CHR_ID_LENGTH+1);
	memset(m_szEPin, 0, sizeof(char) * CHAR_INVENTORY_LOCK_PASS_SIZE+1);

	if (szChaName != NULL)
		StringCchCopy(m_szChaName, CHR_ID_LENGTH+1, szChaName);

	if (szEPin != NULL)
		StringCchCopy(m_szEPin, CHAR_INVENTORY_LOCK_PASS_SIZE+1, szEPin);
}

int CInventoryLockInput::Execute (CServer* pServer)
{
	if (pServer == NULL)
	{
		return DB_ERROR;
	}
	else
	{
		int nResult = m_pDbManager->InventoryLockInput(m_dwChaNum, m_szChaName, m_szEPin );

		GLMSG::SNETPC_INVEN_INVENTORY_LOCK_INPUT_FROM_DB NetMsgDB2Fld;
		StringCchCopy ( NetMsgDB2Fld.szName, CHAR_SZNAME+1, m_szChaName );
		StringCchCopy ( NetMsgDB2Fld.szPin, CHAR_INVENTORY_LOCK_PASS_SIZE+1, m_szEPin );

		if ( nResult == DB_OK ) 
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_INPUT_DONE;
		}
		else if( nResult == 1 )
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_INPUT_DATA_NOT_EXIST;
		}
		else 
		{
			NetMsgDB2Fld.emFB = EMFB_INVENTORY_LOCK_INPUT_FAILED;
		}

		CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
		pTemp->InsertMsg(m_dwClient, (char*) &NetMsgDB2Fld );
		return nResult;
	}
}