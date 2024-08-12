#include "stdafx.h"

#include "../Lib -- Engine/GUInterface/Cursor.h"
#include "DxInputDevice.h"
#include "editmeshs.h"
#include "DxMethods.h"
#include "DxViewPort.h"
#include "DxEffectMan.h"
#include "DxShadowMap.h"
#include "EditMeshs.h"
#include "GLogicData.h"
#include "GLItemMan.h"

#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "stl_Func.h"
#include "DxEffGroupPlayer.h"
#include "DxEffCharData.h"

#include "GLPartyClient.h"
#include "GLStrikeM.h"
#include "GLCrowData.h"

#include "GLCharacter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 파티 맴버 check
// 자기편에게 스킬 체크 ( 자기 위치에 영향 )
// *****************************************************
BOOL GLCharacter::SKT_SELFZONEOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(GETCROW());
	m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(m_dwGaeaID);
	++m_wTARNUM;

	WORD wTARNUM = sSKILL_DATA.wTARNUM;
	if ( wTARNUM<=m_wTARNUM )	return TRUE;

	//	Note : 파티원들 중에 Skill 영역에 들어오는 멤버 검사.
	//
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand )	return TRUE;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	float fDetectRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DWORD dwIndex = 0;
	GLPARTY_CLIENT* pMember = GLPartyClient::GetInstance().GetMaster ();
	while ( pMember && dwIndex<MAXPARTY )
	{
		PGLCHARCLIENT pChar = pLand->GetChar ( pMember->m_dwGaeaID );

		if ( !pMember->ISONESELF() && pChar )
		{
			// 부활일때만 죽은 사람 체크
			bool bValidBody = TRUE;
			
			for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
			{
				if ( sSKILL.m_sAPPLY.sSpecs[nSpec].emSPEC == EMSPECA_REBIRTH )
					bValidBody = FALSE;
			}

			// 살아있으면
			if ( pChar->IsValidBody() == bValidBody )
			{
				D3DXVECTOR3 vMemPos = pMember->m_vPos;
				vMemPos = pChar->GetPosition();

				float fLength = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-vMemPos) );
				if ( fLength <= fDetectRange )
				{
					STARGETID sFindID ( pChar->GETCROW(), pChar->m_dwGaeaID, vMemPos );
					m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
				}

				// 소환수가 있으면 소환수도 추가한다.
				/*if( bValidBody == TRUE && pChar->m_dwSummonGUID != GAEAID_NULL )
				{
					PGLANYSUMMON pSummon = pLand->GetSummon( pChar->m_dwSummonGUID );
					if( pSummon && pSummon->IsValidBody() )
					{
						vMemPos = pSummon->GetPosition();
						fLength = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-vMemPos) );
						if ( fLength <= fDetectRange )
						{
							STARGETID sFindID ( pSummon->GETCROW(), pSummon->m_dwGUID, vMemPos );
							m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
						}
					}
				}*/
			}
		}

		pMember = GLPartyClient::GetInstance().GetMember ( dwIndex++ );
	}

	//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
	//
	std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
	TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
	for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
	{
		if ( wTARNUM <= m_wTARNUM )		break;

		m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
		++m_wTARNUM;
	}

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 적(PC/Mob) check
// 적들에게 스킬 체크 ( 자기 위치에 영향 )
// *****************************************************
BOOL GLCharacter::SKT_SELFZONEENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	float fAttackRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DETECTMAP_RANGE pairRange;
	DWORD emCrow = ( CROW_EX_MOB | CROW_EX_PC | CROW_EX_SUMMON );
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, GetPosition(), fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	MOB.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	SUMMON.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);

			/*skill summon, Juver, 2017/10/08 */
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM <= m_wTARNUM )		break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;
	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 PC만 check
// 모두에게 스킬 체크 ( 자기 위치에 영향 )
// *****************************************************
BOOL GLCharacter::SKT_SELFZONANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	float fAttackRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DETECTMAP_RANGE pairRange;
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( CROW_EX_PC, GetPosition(), fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( IsReActionable(sFindID,FALSE) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM <= m_wTARNUM )		break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;
	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 파티멤버 check
// 자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECOUR_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	if ( sTargetID.emCrow == CROW_MOB )	return FALSE;

	GLPARTY_CLIENT* pParty = GLPartyClient::GetInstance().FindMember ( sTargetID.dwID );
	if ( !pParty )						return FALSE;

	m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(sTargetID.emCrow);
	m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(sTargetID.dwID);
	++m_wTARNUM;
	return TRUE;
}

// *****************************************************
// Desc: 타겟만 추가
// 적들에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECENERMY_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(sTargetID.emCrow);
	m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(sTargetID.dwID);
	++m_wTARNUM;
	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 PC check
// 모두에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECANYBODY_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	if ( sTargetID.emCrow == CROW_MOB )	return FALSE;

	m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(sTargetID.emCrow);
	m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(sTargetID.dwID);
	++m_wTARNUM;
	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 파티멤버들 check
// 자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(sTargetID.emCrow);
	m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(sTargetID.dwID);
	++m_wTARNUM;

	WORD wTARNUM = sSKILL_DATA.wTARNUM;
	if ( wTARNUM<=m_wTARNUM )	return TRUE;

	if ( !GLGaeaClient::GetInstance().ValidCheckTarget(sTargetID) )		return FALSE;
	D3DXVECTOR3 vFindPos = GLGaeaClient::GetInstance().GetTargetPos(sTargetID);

	//	Note : 파티원들 중에 Skill 영역에 들어오는 맴버들 검사.(자기자신은 제외)
	//
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand )	return TRUE;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	float fDetectRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DWORD dwIndex = 0;
	GLPARTY_CLIENT* pMember = GLPartyClient::GetInstance().GetMaster ();
	while ( pMember && dwIndex<MAXPARTY )
	{
		PGLCHARCLIENT pChar = pLand->GetChar ( pMember->m_dwGaeaID );

		if ( !pMember->ISONESELF() && pChar )
		{
			D3DXVECTOR3 vMemPos = pMember->m_vPos;
			vMemPos = vFindPos;

			float fLength = D3DXVec3Length ( &D3DXVECTOR3(vFindPos-vMemPos) );
			if ( fLength <= fDetectRange )
			{
				STARGETID sFindID ( pChar->GETCROW(), pChar->m_dwGaeaID, vMemPos );
				m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
			}

			// 소환수가 있으면 소환수도 추가한다.
			/*if( pChar->m_dwSummonGUID != GAEAID_NULL )
			{
				PGLANYSUMMON pSummon = pLand->GetSummon( pChar->m_dwSummonGUID );
				if( pSummon && pSummon->IsValidBody() )
				{
					vMemPos = pSummon->GetPosition();
					fLength = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-vMemPos) );
					if ( fLength <= fDetectRange )
					{
						STARGETID sFindID ( pSummon->GETCROW(), pSummon->m_dwGUID, vMemPos );
						m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
					}
				}
			}*/
		}

		pMember = GLPartyClient::GetInstance().GetMember ( dwIndex++ );
	}

	//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
	//
	std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
	TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
	for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
	{
		if ( wTARNUM<=m_wTARNUM )	break;

		m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
		++m_wTARNUM;
	}

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 적들 Check (무조건 타겟이 있어야 발동함)
// 적들에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	// 타겟이 유효성 검사
	if ( !GLGaeaClient::GetInstance().ValidCheckTarget(sTargetID) )		return FALSE;
	D3DXVECTOR3 vFindPos = GLGaeaClient::GetInstance().GetTargetPos(sTargetID);

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	float fAttackRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DETECTMAP_RANGE pairRange;
	DWORD emCrow = ( CROW_EX_MOB | CROW_EX_PC | CROW_EX_SUMMON );
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, vFindPos, fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	MOB.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	SUMMON.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);

			/*skill summon, Juver, 2017/10/08 */
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM<=m_wTARNUM )	break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 PC check (무조건 타겟이 있어야 발동함)
// 모두에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
// *****************************************************
BOOL GLCharacter::SKT_TARSPECANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	if ( !GLGaeaClient::GetInstance().ValidCheckTarget(sTargetID) )		return FALSE;
	D3DXVECTOR3 vFindPos = GLGaeaClient::GetInstance().GetTargetPos(sTargetID);

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	float fAttackRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DETECTMAP_RANGE pairRange;
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( CROW_EX_PC, vFindPos, fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( IsReActionable(sFindID,FALSE) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM<=m_wTARNUM )	break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 적 check (첫 타겟의 반경내의 적들)
// *****************************************************
BOOL GLCharacter::SKT_TARSPECFANWIZE ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	if ( !GLGaeaClient::GetInstance().ValidCheckTarget(sTargetID) )		return FALSE;
	D3DXVECTOR3 vTarPos = GLGaeaClient::GetInstance().GetTargetPos(sTargetID);
	D3DXVECTOR3 vTarOrgDir = vTarPos - GetPosition();

	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID.emCrow, sTargetID.dwID );
	if ( !pTarget )		return FALSE;

	//	첫 표적 넣음.
	m_wTARNUM = 0;
	m_sTARIDS[m_wTARNUM].SET ( sTargetID );
	m_wTARNUM++;

	D3DXVECTOR3 vFindPos = GetPosition();

	float fApplyAngle = D3DXToRadian(sSKILL_DATA.wAPPLYANGLE) / 2.0f;

	//	Note : 장거리형 스킬일 경우 GETSUM_TARRANGE() 가산.
	float fAttackRange = (float) ( pTarget->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_APPLY(sSKILL,wLEVEL) + 2 );

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	DETECTMAP_RANGE pairRange;
	DWORD emCrow = ( CROW_EX_MOB | CROW_EX_PC | CROW_EX_SUMMON );
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, vFindPos, fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( sFindID==sTargetID )			continue;
			if ( !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	MOB.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);
			if ( sFindID==sTargetID )		continue;

			D3DXVECTOR3 vTarDir = sFindID.vPos - GetPosition();

			float fDir = DXGetThetaYFromDirection ( vTarDir, vTarOrgDir );
			if ( sSKILL_DATA.wAPPLYANGLE==0 || fabs(fDir) <= fApplyAngle )
			{
				float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
				m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
			}
		}

		//	SUMMON.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);
			if ( sFindID==sTargetID )		continue;

			/*skill summon, Juver, 2017/10/08 */
			if ( !IsReActionable(sFindID) )		continue;

			D3DXVECTOR3 vTarDir = sFindID.vPos - GetPosition();

			float fDir = DXGetThetaYFromDirection ( vTarDir, vTarOrgDir );
			if ( sSKILL_DATA.wAPPLYANGLE==0 || fabs(fDir) <= fApplyAngle )
			{
				float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
				m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
			}
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( sSKILL_DATA.wTARNUM <= m_wTARNUM )	break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 적 check (관통스킬을 위한)
// *****************************************************
BOOL GLCharacter::SKT_TARSELFTOSPEC ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	//	자신에서 목표까지 적이 존제할 경우 관통 사격.
	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID.emCrow, sTargetID.dwID );
	if ( !pTarget )		return FALSE;

	D3DXVECTOR3 vTarget = sTargetID.vPos;	vTarget.y += pTarget->GetBodyHeight()*0.75f;
	D3DXVECTOR3 vFrom = m_vPos;				vFrom.y += m_fHeight*0.75f;

	D3DXVECTOR3 vDir = vTarget - vFrom;
	D3DXVec3Normalize ( &vDir, &vDir );

	//	Note : 장거리형 스킬일 경우 GETSUM_TARRANGE() 가산.
	float fAttackRange = (float) ( pTarget->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_APPLY(sSKILL,wLEVEL) + 2 );
	vTarget = vFrom + vDir * fAttackRange;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	DETECTMAP_RANGE pairRange;
	DWORD emCrow = ( CROW_EX_MOB | CROW_EX_PC | CROW_EX_SUMMON );
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, vFrom, vTarget );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	MOB.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; iter!=pairRange.second; ++iter )
		{
			STARGETID sFindID = (*iter);

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFrom-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	SUMMON.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; iter!=pairRange.second; ++iter )
		{
			STARGETID sFindID = (*iter);

			/*skill summon, Juver, 2017/10/08 */
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFrom-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 관통 갯수많큼 뽑아내기.
		//
		{
			WORD wPIERCENUM = sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE();

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<wPIERCENUM); ++iter, ++i )
			{
				if ( wPIERCENUM <= m_wTARNUM )		break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 파티원 check
// *****************************************************
BOOL GLCharacter::SKT_TARZONEOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	m_vTARPOS = *pvTarPos;
	D3DXVECTOR3 vFindPos = *pvTarPos;

	WORD wTARNUM = sSKILL_DATA.wTARNUM;
	if ( wTARNUM<=m_wTARNUM )	return TRUE;

	//	Note : 파티원들 중에 Skill 영역에 들어오는 멤버 검사.
	//
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand )	return TRUE;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	float fDetectRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DWORD dwIndex = 0;
	GLPARTY_CLIENT* pMember = GLPartyClient::GetInstance().GetMaster ();
	while ( pMember && dwIndex<MAXPARTY )
	{
		PGLCHARCLIENT pChar = pLand->GetChar ( pMember->m_dwGaeaID );

		if ( !pMember->ISONESELF() && pChar )
		{
			D3DXVECTOR3 vMemPos = pMember->m_vPos;
			vMemPos = vFindPos;

			float fLength = D3DXVec3Length ( &D3DXVECTOR3(vFindPos-vMemPos) );
			if ( fLength <= fDetectRange )
			{
				STARGETID sFindID ( pChar->GETCROW(), pChar->m_dwGaeaID, vMemPos );
				m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
			}

			// 소환수가 있으면 소환수도 추가한다.
			/*if( pChar->m_dwSummonGUID != GAEAID_NULL )
			{
				PGLANYSUMMON pSummon = pLand->GetSummon( pChar->m_dwSummonGUID );
				if( pSummon && pSummon->IsValidBody() )
				{
					vMemPos = pSummon->GetPosition();
					fLength = D3DXVec3Length ( &D3DXVECTOR3(GetPosition()-vMemPos) );
					if ( fLength <= fDetectRange )
					{
						STARGETID sFindID ( pSummon->GETCROW(), pSummon->m_dwGUID, vMemPos );
						m_vecTAR_TEMP.push_back ( STARDIST(fLength,sFindID) );
					}
				}
			}*/
		}

		pMember = GLPartyClient::GetInstance().GetMember ( dwIndex++ );
	}

	//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
	//
	std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
	TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
	TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
	for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
	{
		if ( wTARNUM<=m_wTARNUM )	break;

		m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
		++m_wTARNUM;
	}

	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 적(PC/MOB) check (타겟 검사 안함)
// *****************************************************
BOOL GLCharacter::SKT_TARZONEENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	m_vTARPOS = *pvTarPos;
	D3DXVECTOR3 vFindPos = *pvTarPos;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	DETECTMAP_RANGE pairRange;
	DWORD emCrow = ( CROW_EX_MOB | CROW_EX_PC | CROW_EX_SUMMON );
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, vFindPos, static_cast<float>(sSKILL_DATA.wAPPLYRANGE) );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(GetPosition()-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	MOB.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);
			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	SUMMON.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			STARGETID sFindID = (*iter);

			/*skill summon, Juver, 2017/10/08 */
			if ( sTargetID!=sFindID && !IsReActionable(sFindID) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM <= m_wTARNUM )		break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;
	return TRUE;
}

// *****************************************************
// Desc: 스킬의 적용 범위에 들어오는 PC check (타겟 검사 안함)
// *****************************************************
BOOL GLCharacter::SKT_TARZONEANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL )
{
	const SKILL::CDATA_LVL &sSKILL_DATA = sSKILL.m_sAPPLY.sDATA_LVL[wLEVEL];

	m_vTARPOS = *pvTarPos;
	D3DXVECTOR3 vFindPos = *pvTarPos;

	if ( !m_vecTAR_TEMP.empty() )	m_vecTAR_TEMP.erase ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );

	//	Crow 검사.
	float fAttackRange = GETBODYRADIUS() + static_cast<float>(sSKILL_DATA.wAPPLYRANGE) + 2;

	DETECTMAP_RANGE pairRange;	
	DETECTMAP *pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( CROW_PC, vFindPos, fAttackRange );
	if ( pDetectMap )
	{
		DETECTMAP_ITER iter;

		//	PC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		iter = pairRange.first;
		for ( ; (iter!=pairRange.second); ++iter )
		{
			const STARGETID &sFindID = (*iter);
			if ( IsReActionable(sFindID,FALSE) )		continue;

			float fLengthSq = D3DXVec3LengthSq ( &D3DXVECTOR3(vFindPos-sFindID.vPos) );
			m_vecTAR_TEMP.push_back ( STARDIST(fLengthSq,sFindID) );
		}

		//	Note : 가까운것부터 가능한 갯수많큼 뽑아내기.
		//
		{
			WORD wTARNUM = sSKILL_DATA.wTARNUM;

			std::sort ( m_vecTAR_TEMP.begin(), m_vecTAR_TEMP.end() );
			TARGETMAP_ITER iter = m_vecTAR_TEMP.begin();
			TARGETMAP_ITER iter_end = m_vecTAR_TEMP.end();
			for ( WORD i=m_wTARNUM; (iter!=iter_end) && (i<EMTARGET_NET); ++iter, ++i )
			{
				if ( wTARNUM <= m_wTARNUM )		break;

				m_sTARIDS[m_wTARNUM].SET ( (*iter).m_sTARGETID );
				++m_wTARNUM;
			}
		}
	}

	if ( m_wTARNUM==0 )		return FALSE;

	return TRUE;
}


BOOL GLCharacter::SelectSkillTarget ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos )
{
	m_wTARNUM = 0;

	/*skill validity check, Juver, 2017/11/26 */
	for ( int i=0; i<EMTARGET_NET; ++i )
	{
		m_sTARIDS[i].wCrow = 0;
		m_sTARIDS[i].wID = USHRT_MAX;
	}

	//	Note : 스킬 정보 가져옴.
	//

	SCHARSKILL sSkill;
	
    if ( IsDefenseSkill() ) 
	{
		if ( m_sActiveSkill != m_sDefenseSkill.m_dwSkillID ) return FALSE;
		sSkill.sNativeID = m_sActiveSkill;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( m_sActiveSkill.dwID );
		if ( learniter==m_ExpSkills.end() )		return FALSE;
		sSkill = (*learniter).second;
	}

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sActiveSkill.wMainID, m_sActiveSkill.wSubID );
	if ( !pSkill )							return FALSE;

	switch ( pSkill->m_sBASIC.emIMPACT_TAR )
	{
	case TAR_SELF:
		switch ( pSkill->m_sBASIC.emIMPACT_REALM )
		{
		case REALM_KEEP_ZONE:	return FALSE;

		case REALM_SELF:
			m_sTARIDS[m_wTARNUM].wCrow = static_cast<WORD>(GETCROW());
			m_sTARIDS[m_wTARNUM].wID = static_cast<WORD>(m_dwGaeaID);
			++m_wTARNUM;
			return TRUE;

		case REALM_ZONE:
			switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
			{
			case SIDE_OUR:		return SKT_SELFZONEOUR ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
			case SIDE_ENEMY:	return SKT_SELFZONEENERMY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
			case SIDE_ANYBODY:	return SKT_SELFZONANYBODY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
			};
			return FALSE;
		};
		return FALSE;

	case TAR_SPEC:
		{
			if ( sTargetID.dwID == EMTARGET_NULL )	return FALSE;

			switch ( pSkill->m_sBASIC.emIMPACT_REALM )
			{
			case REALM_KEEP_ZONE:	return FALSE;
			
			case REALM_SELF:
				switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
				{
				case SIDE_OUR:		return SKT_TARSPECOUR_SELF ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				case SIDE_ENEMY:	return SKT_TARSPECENERMY_SELF ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				case SIDE_ANYBODY:	return SKT_TARSPECANYBODY_SELF ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				};
				return TRUE;

			case REALM_ZONE:
				switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
				{
				case SIDE_OUR:		return SKT_TARSPECOUR ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				case SIDE_ENEMY:	return SKT_TARSPECENERMY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				case SIDE_ANYBODY:	return SKT_TARSPECANYBODY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				};
				return FALSE;

			case REALM_FANWIZE:
				switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
				{
				case SIDE_OUR:		return FALSE;
				case SIDE_ENEMY:	return SKT_TARSPECFANWIZE ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
				case SIDE_ANYBODY:	return FALSE;
				};
				return FALSE;
			};
		}
		break;

	case TAR_SELF_TOSPEC:
		switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
		{
		case SIDE_OUR:		return FALSE;
		case SIDE_ENEMY:	return SKT_TARSELFTOSPEC ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
		case SIDE_ANYBODY:	return FALSE;
		};
		return FALSE;

	case TAR_ZONE:
		if ( !pvTarPos )		return FALSE;
		switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
		{
		case SIDE_OUR:		return SKT_TARZONEOUR ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
		case SIDE_ENEMY:	return SKT_TARZONEENERMY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
		case SIDE_ANYBODY:	return SKT_TARZONEANYBODY ( sTargetID, pvTarPos, *pSkill, sSkill.wLevel );
		};
		return FALSE;
	};

	return FALSE;
}
