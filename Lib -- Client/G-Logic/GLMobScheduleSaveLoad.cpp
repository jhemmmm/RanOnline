#include "stdafx.h"
#include "./GLLandMan.h"
#include "./GLMobSchedule.h"

#include "../../Lib -- Engine/Common/SerialFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD	SMOBACTION::VERSION = 0x0100;
const DWORD GLMobScheduleMan::VERSION = 0x0100;

BOOL GLMobSchedule::Load ( basestream &SFile )
{
	DWORD dwVer, dwSize;
	SFile >> dwVer;
	dwSize = SFile.ReadBlockSize ();

	switch ( dwVer )
	{
	case 0x0100:	Load_0100( SFile );	break;
	case 0x0101:	Load_0101( SFile );	break;
	case 0x0102:	Load_0102( SFile );	break;
	case 0x0103:	Load_0103( SFile );	break;
	case 0x0104:	Load_0104( SFile );	break;
	case 0x0105:	Load_0105( SFile );	break;
	case 0x0106:	Load_0106( SFile );	break;
	case 0x0107:	Load_0107( SFile );	break;
	case 0x0108:	Load_0108( SFile );	break;
	case 0x0109:	Load_0109( SFile );	break;
	case 0x0200:	Load_0200( SFile );	break;
	case VERSION:	Load_0201( SFile );	break;
	default:
		{
			CDebugSet::ErrorVersion( "GLMobSchedule::Load", dwVer );	
		}break;
	};	

	//	박스의 크기를 계산.
	CalculateBox ();

	if ( m_bRendGenPos )
		std::random_shuffle ( m_vectorRendGenPos.begin(), m_vectorRendGenPos.end() );

	//action stay type check
	//check for invalid stay time
	SGLNODE<SMOBACTION>* pCur = m_sMobActList.m_pHead;
	while ( pCur )
	{
		SMOBACTION &sMobAction = pCur->Data;
		if ( sMobAction.emAction == EMACTION_ACTSTAY )
		{
			if ( sMobAction.fLife <= 0.0f )
				sMobAction.fLife = 1.0f;
		}

		pCur = pCur->pNext;
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0100( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	return TRUE;
}
BOOL GLMobSchedule::Load_0101( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0102( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0103( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0104( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0105( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0106( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_nExData1; //unknown data
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0107( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_nExData1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	return TRUE;
}

BOOL GLMobSchedule::Load_0108( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_nExData1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dwExData1; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0109( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_nExData2; //unknown data
	SFile >> m_nExData1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dwExData1; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0200( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	return TRUE;
}

BOOL GLMobSchedule::Load_0201( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	//unknown data
	SFile >> m_nExData1;
	SFile >> m_nExData2;
	SFile >> m_dwExData1;

	return TRUE;
}


BOOL GLMobSchedule::Save ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;

	SFile.BeginBlock(EMBLOCK_02);

	SFile.WriteBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.WriteBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.WriteBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile << m_fReGenTime;
	SFile << m_nRegenHour;
	SFile << m_nRegenMin;

	SFile.WriteBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );

	SGLNODE<SMOBACTION>* pCur = m_sMobActList.m_pHead;
	if ( pCur )	SFile << (BOOL) TRUE;
	else		SFile << (BOOL) FALSE;

	while ( pCur )
	{
		SMOBACTION &sMobAction = pCur->Data;

		SFile << (DWORD) SMOBACTION::VERSION;
		SFile << (DWORD) sizeof(SMOBACTION);
		SFile.WriteBuffer ( &sMobAction, sizeof(SMOBACTION) );

		if ( pCur->pNext )	SFile << (BOOL) TRUE;
		else				SFile << (BOOL) FALSE;

		pCur = pCur->pNext;
	}

	SFile << m_bLeaderMonster;
	SFile << m_strGroupName;
	SFile << m_bRendGenPos;
	SFile << m_wRendGenPosNum;
	SFile << m_wRendGenPosDist;

	SFile << static_cast<DWORD> ( m_vectorRendGenPos.size() );
	VEC3ARRAY_ITER iter = m_vectorRendGenPos.begin ();
	VEC3ARRAY_ITER iter_end = m_vectorRendGenPos.end ();
	for ( ; iter!=iter_end; ++iter )
	{
		SFile <<  (*iter);
	}

	SFile << m_dwPC_REGEN_GATEID;
	SFile << m_strBUSLOCATION;

	//unknown data
	SFile << m_nExData1;
	SFile << m_nExData2;
	SFile << m_dwExData1;

	SFile.EndBlock (EMBLOCK_02);

	return TRUE;
}

BOOL GLMobScheduleMan::LoadFile ( basestream &SFile )
{
	m_setMobDesc.clear ();

	BOOL bExist;
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	dwSize = SFile.ReadBlockSize ();

	SFile >> bExist;
	while ( bExist )
	{
		GLMobSchedule* pMobSch = new GLMobSchedule;

		pMobSch->Load ( SFile );
		m_GLMobSchList.ADDTAIL ( pMobSch );

		AddMobSchGroub ( pMobSch );
		m_setMobDesc.insert ( pMobSch->m_CrowID.dwID );

		SFile >> bExist;
	}

	return TRUE;
}

BOOL GLMobScheduleMan::SaveFile ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;
	SFile.BeginBlock (EMBLOCK_01);

	SGLNODE<GLMobSchedule*>* pCur = m_GLMobSchList.m_pHead;
	SFile << BOOL ( pCur!=NULL );

	while ( pCur )
	{
		GLMobSchedule* pMobSch = pCur->Data;

		pMobSch->Save ( SFile );

		pCur = pCur->pNext;
		SFile << BOOL ( pCur!=NULL );
	}

	SFile.EndBlock (EMBLOCK_01);

	return TRUE;
}