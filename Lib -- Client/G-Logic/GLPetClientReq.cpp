#include "StdAfx.h"
#include "GLPetClient.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "GLContrlMsg.h"
#include "GLItemMan.h"
#include "GLTradeClient.h"
#include "RanFilter.h"

#include "../Lib -- Engine/Common/StringUtils.h"

#include "../Lib -- ClientUI/Interface/InnerInterface.h"
#include "../Lib -- ClientUI/Interface/UITextControl.h"
#include "../Lib -- ClientUI/Interface/GameTextControl.h"
#include "../Lib -- ClientUI/Interface/ModalWindow.h"
#include "../Lib -- ClientUI/Interface/ModalCallerID.h"

#include "GLCharDefine.h"
#include "DxEffGroupPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLPetClient::ReqGoto ( D3DXVECTOR3 vTarPos, bool bToPickUp )
{
	GLMSG::SNETPET_REQ_GOTO NetMsg;

	D3DXVECTOR3 vPos(vTarPos);

	// 플래그 리셋
	ReSetAllSTATE ();

	// 만약 아이템을 주우러 간다면
	if ( bToPickUp )
	{
		BOOL bGoto = m_actorMove.GotoLocation ( vTarPos+D3DXVECTOR3(0,+10,0),
							                vTarPos+D3DXVECTOR3(0,-10,0) );

		if ( !bGoto ) 
		{
			m_actorMove.Stop ();
			m_pSkinChar->SELECTANI ( AN_GUARD_N, AN_SUB_NONE );
			SetSTATE ( EM_PETACT_STOP );
			m_sSkillTARGET.RESET ();
			return;
		}

		// 아이템을 주울때는 무조건 뛰어간다.
		SetSTATE ( EM_PETACT_MOVE );
		SetSTATE ( EM_PETACT_RUN_PICKUP_ITEM );
		SetMoveState ( IsSTATE ( EM_PETACT_MOVE ) );
	
		NetMsg.dwGUID  = m_dwGUID;
		NetMsg.vCurPos = m_vPos;
		NetMsg.vTarPos = vTarPos;
		NetMsg.dwFlag  = m_dwActionFlag;

		NETSENDTOFIELD ( &NetMsg );

		return;
	}

	// 주인의 이동타겟
	STARGETID sTarget = m_pOwner->GetTargetID ();

	// 주인보다 앞서있으면
	D3DXVECTOR3 vOwnerPos = m_pOwner->GetPosition ();
	D3DXVECTOR3 vDist = m_vPos - sTarget.vPos;
	float fPetToTar = D3DXVec3Length(&vDist);
	vDist = vOwnerPos - sTarget.vPos;
	float fOwnerToTar = D3DXVec3Length(&vDist);

	vDist = vOwnerPos - m_vPos;
	float fDist = D3DXVec3Length(&vDist);

	// 이동중일때만 기다려준다
	if ( fDist < GLCONST_PET::fWalkArea && m_pOwner->IsACTION ( GLAT_MOVE ) )
	{
		m_actorMove.Stop ();
		m_pSkinChar->SELECTANI ( AN_GUARD_N, AN_SUB_NONE );
		SetSTATE ( EM_PETACT_WAIT );

		GLMSG::SNETPET_REQ_STOP NetMsg;
		NetMsg.dwGUID = m_dwGUID;
		NetMsg.dwFlag = m_dwActionFlag;
		NetMsg.vPos	  = m_vPos;
		NETSENDTOFIELD ( &NetMsg );
		return;
	}

	BOOL bGoto = m_actorMove.GotoLocation ( vTarPos+D3DXVECTOR3(0,+10,0),
							                vTarPos+D3DXVECTOR3(0,-10,0) );

	// 혹시 못가는 곳이면
	if ( !bGoto )
	{
		for ( WORD i = 0; i < 8; ++i )
		{
			D3DXVECTOR3 vRandPos = GLPETDEFINE::RANDPOS[i];

			vTarPos = vPos + vRandPos;
			bGoto = m_actorMove.GotoLocation ( vTarPos+D3DXVECTOR3(0,+10,0),
							                   vTarPos+D3DXVECTOR3(0,-10,0) );
			if ( bGoto ) break;
		}
	}

	if ( !bGoto ) 
	{
		m_actorMove.Stop ();
		m_pSkinChar->SELECTANI ( AN_GUARD_N, AN_SUB_NONE );
		SetSTATE ( EM_PETACT_STOP );
		return;
	}

	// [주의] 
	// 이동메시지 전송시 항상 주인의 Run 상태를 따라야 한다.
	SetSTATE ( EM_PETACT_MOVE );
	SetMoveState ( m_pOwner->IsSTATE ( EM_ACT_RUN ) );

	NetMsg.dwGUID  = m_dwGUID;
	NetMsg.vCurPos = m_vPos;
	NetMsg.vTarPos = vTarPos;
	NetMsg.dwFlag  = m_dwActionFlag;

	NETSENDTOFIELD ( &NetMsg );
}

void GLPetClient::ReqStop ( bool bStopAttack )
{
	ReSetAllSTATE ();
	SetSTATE ( EM_PETACT_STOP );

	m_actorMove.Stop ();
	m_pSkinChar->SELECTANI ( AN_GUARD_N, AN_SUB_NONE );
	m_sSkillTARGET.RESET ();

	GLMSG::SNETPET_REQ_STOP NetMsg;
	NetMsg.dwGUID      = m_dwGUID;
	NetMsg.dwFlag      = m_dwActionFlag;
	NetMsg.vPos	       = m_vPos;
	NetMsg.bStopAttack = bStopAttack;
	NETSENDTOFIELD ( &NetMsg );
}

void GLPetClient::ReqGiveFood ( WORD wPosX, WORD wPosY )
{
	// 비활성 상태에서도 사료를 줄 수 있다.
	//if ( !IsVALID() )							return;
	if ( GLTradeClient::GetInstance().Valid() )	return;

	GLCharacter* pOwner = GLGaeaClient::GetInstance().GetCharacter();
	SINVENITEM* pInvenItem = pOwner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem ) return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) return;

	// 팻카드 여부 체크
	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )	return;

	SITEM* pHold = pOwner->GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) return;

	// 사료 여부 체크
	if ( pHold->sBasicOp.emItemType != ITEM_PET_FOOD )	return;

	SPETCARDINFO sPet;

	if ( !DxGlobalStage::GetInstance().IsEmulator() )
	{
		// 팻카드의 정보가 없으면 없는 팻이다.
		PETCARDINFO_MAP_ITER iter = pOwner->m_mapPETCardInfo.find ( pInvenItem->sItemCustom.dwPetID );
		if ( iter==pOwner->m_mapPETCardInfo.end() ) return;
		sPet = (*iter).second;
	}
	else
	{
		sPet.m_nFull  = m_nFull;
		sPet.m_emTYPE = m_emTYPE;
	}

	// 포만감이 가득찼으면
	if ( sPet.m_nFull >= 1000 ) return;

	GLMSG::SNETPET_REQ_GIVEFOOD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.emPetTYPE = sPet.m_emTYPE;

	NETSENDTOFIELD ( &NetMsg );
}

void GLPetClient::ReqInputName ( WORD wPosX, WORD wPosY )
{
	if ( !IsVALID() )							return; // 활성상태에서만 이름변경 가능
	if ( GLTradeClient::GetInstance().Valid() )	return;

	SINVENITEM* pInvenItem = m_pOwner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_RENAME_FB_FB_NOITEM") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_RENAME )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_RENAME_FB_FB_BADITEM") );
		return;
	}

	DoModal( ID2GAMEINTEXT("INVEN_CHAR_RENAME"), MODAL_INPUT, EDITBOX_PETNAME, MODAL_PET_RENAME );

	m_pOwner->m_wInvenPosX1 = wPosX;
	m_pOwner->m_wInvenPosY1 = wPosY;
}

void GLPetClient::ReqRename ( const char* szCharName )
{
	if( !szCharName ) return;

	CString strTEMP( szCharName );

#ifdef TH_PARAM	
	// 태국어 문자 조합 체크

	if ( !m_pCheckString ) return;
	
	if ( !m_pCheckString(strTEMP) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_THAICHAR_ERROR"));
		return;
	}
#endif

#ifdef VN_PARAM
	// 베트남 문자 조합 체크 
	if( STRUTIL::CheckVietnamString( strTEMP ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_VNCHAR_ERROR"));
		return;
	}

#endif 



	BOOL bFILTER0 = STRUTIL::CheckString( strTEMP );
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter( strTEMP );
	if ( bFILTER0 || bFILTER1 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEEXTEXT("CHARACTER_BADNAME") );
		return;
	}

	if ( strlen ( szCharName ) > PETNAMESIZE )
	{
		// 너무 길다.
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_RENAME_FB_TOOLONG") );
		return;
	}

	GLMSG::SNETPET_REQ_RENAME NetMsg;
	NetMsg.wPosX = m_pOwner->m_wInvenPosX1;
	NetMsg.wPosY = m_pOwner->m_wInvenPosY1;
	StringCchCopy ( NetMsg.szName, PETNAMESIZE+1, szCharName );
	NETSENDTOFIELD ( &NetMsg );

	m_pOwner->m_wInvenPosX1 = 0;
	m_pOwner->m_wInvenPosY1 = 0;
}

void GLPetClient::ReqInputColor ( WORD wPosX, WORD wPosY )
{
	if ( !IsVALID() )							return; // 활성상태에서만 변경 가능
	if ( GLTradeClient::GetInstance().Valid() )	return;

	if ( !GLCONST_PET::IsChangeablePetColor ( m_emTYPE ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_UNCHANGEABLE") );
		return;
	}

	SINVENITEM* pInvenItem = m_pOwner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_NOITEM") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_COLOR )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_BADITEM") );
		return;
	}

	if ( IsUsePetSkinPack() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_SKINPACK") );
		return;
	}

	DoModal( ID2GAMEINTEXT("MODAL_PETCOLOR_INFO"), MODAL_INFOMATION, OK, MODAL_PETCOLOR_INFO );

	m_pOwner->m_wInvenPosX1 = wPosX;
	m_pOwner->m_wInvenPosY1 = wPosY;
}
void GLPetClient::ReqChangeColor ( WORD wColor )
{
	GLMSG::SNETPET_REQ_CHANGE_COLOR NetMsg;
	NetMsg.wColor = wColor;
	NetMsg.wPosX  = m_pOwner->m_wInvenPosX1;
	NetMsg.wPosY  = m_pOwner->m_wInvenPosY1;
	NETSENDTOFIELD ( &NetMsg );

	m_pOwner->m_wInvenPosX1 = 0;
	m_pOwner->m_wInvenPosY1 = 0;
}
void GLPetClient::ReqInputStyle ( WORD wPosX, WORD wPosY )
{
	if ( !IsVALID() )							return; // 활성상태에서만 변경 가능
	if ( GLTradeClient::GetInstance().Valid() )	return;

	PETSTYLE sPetStyle = GLCONST_PET::GetPetStyle ( m_emTYPE );
	if ( sPetStyle.wSTYLENum == 0 )				return; // chf 파일이 없다!!

	SINVENITEM* pInvenItem = m_pOwner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_NOITEM") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_STYLE )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_BADITEM") );
		return;
	}

	if ( IsUsePetSkinPack() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_STYLECOLOR_CHANGE_FB_SKINPACK") );
		return;
	}

	CInnerInterface::GetInstance().ShowGroupFocus( PETSTYLECARD_WINDOW );

	m_pOwner->m_wInvenPosX2 = wPosX;
	m_pOwner->m_wInvenPosY2 = wPosY;
}
void GLPetClient::ReqChangeStyle ( WORD wStyle )
{
	GLMSG::SNETPET_REQ_CHANGE_STYLE NetMsg;
	NetMsg.wStyle = wStyle;
	NetMsg.wPosX  = m_pOwner->m_wInvenPosX2;
	NetMsg.wPosY  = m_pOwner->m_wInvenPosY2;
	NETSENDTOFIELD ( &NetMsg );

	m_pOwner->m_wInvenPosX2 = 0;
	m_pOwner->m_wInvenPosY2 = 0;
}

// 슬롯을 클릭하기만 하면 호출되어져야 한다.
void GLPetClient::ReqChangeAccessory ( EMSUIT emSUIT )
{
	if ( GLTradeClient::GetInstance().Valid() )		return;
	//if ( emSUIT == ACCETYPESIZE )					return;

	//	Note : 공격중이나 스킬 시전 중에 슬롯 변경을 수행 할 수 없다고 봄.
	//
	if ( m_pOwner->IsACTION(GLAT_ATTACK) || m_pOwner->IsACTION(GLAT_SKILL) ) return;

	SITEMCUSTOM sHoldItemCustom = m_pOwner->GET_HOLD_ITEM ();
	SITEMCUSTOM	sSlotItemCustom = GetSlotitembySuittype ( emSUIT );

	//	SLOT <-> HOLD
	if ( sHoldItemCustom.sNativeID != NATIVEID_NULL() && sSlotItemCustom.sNativeID != NATIVEID_NULL() )
	{
		SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHoldItemCustom.sNativeID );
		if ( !pHoldItem ) 
		{
			// 일반 오류
			return;
		}

		if ( !CheckSlotItem( pHoldItem->sBasicOp.sNativeID, emSUIT ) ) return;
		
		SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( sSlotItemCustom.sNativeID );
		if ( !pSlotItem ) 
		{
			// 일반 오류
			return;
		}

		// 타입이 다르면
		if ( pHoldItem->sSuitOp.emSuit != pSlotItem->sSuitOp.emSuit ) return;

		GLMSG::SNETPET_REQ_SLOT_EX_HOLD NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}
	// HOLD -> SLOT
	else if ( sHoldItemCustom.sNativeID != NATIVEID_NULL() )
	{
		SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHoldItemCustom.sNativeID );
		if ( !pHoldItem ) 
		{
			// 일반 오류
			return;
		}

		if ( !CheckSlotItem( pHoldItem->sBasicOp.sNativeID, emSUIT ) ) return;

		// 타입이 다르면
		if ( pHoldItem->sSuitOp.emSuit != emSUIT ) return;

		GLMSG::SNETPET_REQ_HOLD_TO_SLOT NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}
	// SLOT -> HOLD
	else if ( sSlotItemCustom.sNativeID != NATIVEID_NULL() )
	{
		GLMSG::SNETPET_REQ_SLOT_TO_HOLD NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}
}

void GLPetClient::ReqRemoveSlotItem ( EMSUIT emSuit )
{
	WORD wPosX(0), wPosY(0);
	SITEMCUSTOM	sSlotItemCustom = GetSlotitembySuittype ( emSuit );

	SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( sSlotItemCustom.sNativeID );
	if ( !pSlotItem ) 
	{
		// 일반 오류
		return;
	}

	BOOL bOk = m_pOwner->m_cInventory.FindInsrtable ( pSlotItem->sBasicOp.wInvenSizeX, pSlotItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
	if ( !bOk )
	{
		//	인밴이 가득찻음.
		return;
	}

	GLMSG::SNETPET_REQ_REMOVE_SLOTITEM NetMsg;
	NetMsg.emSuit = emSuit;

	NETSENDTOFIELD ( &NetMsg );
}

/*dual pet skill, Juver, 2017/12/27 */
void GLPetClient::ReqChangeActiveSkill_A ( SNATIVEID sSkillID, BOOL bFromUI /*= FALSE*/ )
{
	// 스킬 취소를 위한
	if ( sSkillID == NATIVEID_NULL() )
	{
		GLMSG::SNETPET_REQ_SKILLCHANGE_A NetMsg;
		NetMsg.sSkillID = NATIVEID_NULL();
		NetMsg.bFromUI = bFromUI; /* pet skill bug Juver 2017/06/27 */
		NETSENDTOFIELD ( &NetMsg );
		return;
	}
	
	if ( !CheckSkill ( sSkillID ) )
	{
		// 배우지 않았거나 쿨타임 불충분
		return;
	}

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
	if ( !pSkill )	return;

	STARGETID sTARG( CROW_PET,m_dwGUID, m_vPos );

	//	Note : 스킬 시작과 동시에 나타나는 이팩트. 발동.
	//
	EMELEMENT emELMT = EMELEMENT_SPIRIT;

	if ( pSkill->m_sEXT_DATA.emSELFBODY==SKILL::EMTIME_FIRST )
		DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETSELFBODY(emELMT), &sTARG, &m_vDir );

	SANIMSTRIKE sStrike;
	sStrike.m_emPiece = PIECE_HAIR;
	sStrike.m_emEffect = EMSF_TARGET;
	sStrike.m_dwFrame = 0;

	if ( pSkill->m_sEXT_DATA.emSELFZONE01==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE01_POS, pSkill->m_sEXT_DATA.GETSELFZONE01(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE02==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE02_POS, pSkill->m_sEXT_DATA.GETSELFZONE02(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE03==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE03_POS, pSkill->m_sEXT_DATA.GETSELFZONE03(emELMT), &sTARG );

	GLMSG::SNETPET_REQ_SKILLCHANGE_A NetMsg;
	NetMsg.sSkillID = sSkillID;
	NetMsg.bFromUI = bFromUI; /* pet skill bug Juver 2017/06/27 */
	NETSENDTOFIELD ( &NetMsg );
}

void GLPetClient::ReqChangeActiveSkill_B ( SNATIVEID sSkillID, BOOL bFromUI /*= FALSE*/ )
{
	// 스킬 취소를 위한
	if ( sSkillID == NATIVEID_NULL() )
	{
		GLMSG::SNETPET_REQ_SKILLCHANGE_B NetMsg;
		NetMsg.sSkillID = NATIVEID_NULL();
		NetMsg.bFromUI = bFromUI; /* pet skill bug Juver 2017/06/27 */
		NETSENDTOFIELD ( &NetMsg );
		return;
	}

	if ( !CheckSkill ( sSkillID ) )
	{
		// 배우지 않았거나 쿨타임 불충분
		return;
	}

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
	if ( !pSkill )	return;

	STARGETID sTARG( CROW_PET,m_dwGUID, m_vPos );

	//	Note : 스킬 시작과 동시에 나타나는 이팩트. 발동.
	//
	EMELEMENT emELMT = EMELEMENT_SPIRIT;

	if ( pSkill->m_sEXT_DATA.emSELFBODY==SKILL::EMTIME_FIRST )
		DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETSELFBODY(emELMT), &sTARG, &m_vDir );

	SANIMSTRIKE sStrike;
	sStrike.m_emPiece = PIECE_HAIR;
	sStrike.m_emEffect = EMSF_TARGET;
	sStrike.m_dwFrame = 0;

	if ( pSkill->m_sEXT_DATA.emSELFZONE01==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE01_POS, pSkill->m_sEXT_DATA.GETSELFZONE01(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE02==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE02_POS, pSkill->m_sEXT_DATA.GETSELFZONE02(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE03==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE03_POS, pSkill->m_sEXT_DATA.GETSELFZONE03(emELMT), &sTARG );

	GLMSG::SNETPET_REQ_SKILLCHANGE_B NetMsg;
	NetMsg.sSkillID = sSkillID;
	NetMsg.bFromUI = bFromUI; /* pet skill bug Juver 2017/06/27 */
	NETSENDTOFIELD ( &NetMsg );
}

void GLPetClient::ReqLearnSkill ( WORD wPosX, WORD wPosY )
{
	if ( !IsVALID () )							return;
	if ( GLTradeClient::GetInstance().Valid() )	return;
	//if( m_pOwner->ValidRebuildOpen() )			return;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_pOwner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );

	// 팻용 스킬인지 유무????
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_PET_SKILL )	return;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL( sSKILL_ID ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( 
			NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_LEARNSKILL_FB_ALREADY") );
		return;
	}

	//	Note : 스킬 습득을 요청합니다.
	//
	GLMSG::SNETPET_REQ_LEARNSKILL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	
	NETSENDTOFIELD ( &NetMsg );

	return;
}

void GLPetClient::ReqLearnSkill ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	if ( !IsVALID () )								return;
	if ( !m_pOwner->IsVALID_STORAGE(dwChannel) )	return;
	if ( GLTradeClient::GetInstance().Valid() )		return;
	//if( m_pOwner->ValidRebuildOpen() )			return;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_pOwner->m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )							return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	
	// 팻용 스킬인지 유무????
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_PET_SKILL )	return;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL( sSKILL_ID ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( 
			NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_LEARNSKILL_FB_ALREADY") );
		return;
	}

	//	Note : 스킬 습득을 요청합니다.
	//
	GLMSG::SNETPET_REQ_LEARNSKILL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.bStorage = true;
	NetMsg.dwChannel = dwChannel;
	
	NETSENDTOFIELD ( &NetMsg );

	return;
}

/*dual pet skill, Juver, 2017/12/29 */
void GLPetClient::ReqEnableDualSkill ( WORD wPosX, WORD wPosY )
{
	if ( GLTradeClient::GetInstance().Valid() )	return;

	GLCharacter* ppet_owner = GLGaeaClient::GetInstance().GetCharacter();
	if ( !ppet_owner )	return;

	SINVENITEM* pinven_item = ppet_owner->m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pinven_item ) return;

	wPosX = pinven_item->wPosX;
	wPosY = pinven_item->wPosY;

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem ( pinven_item->sItemCustom.sNativeID );
	if ( !pitem_data ) return;

	if ( pitem_data->sBasicOp.emItemType != ITEM_PET_CARD )	return;

	SITEM* phold_item_data = ppet_owner->GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !phold_item_data ) return;

	if ( phold_item_data->sBasicOp.emItemType != ITEM_CARD_DUALPETSKILL )	return;

	SPETCARDINFO sPet;

	if ( !DxGlobalStage::GetInstance().IsEmulator() )
	{
		PETCARDINFO_MAP_ITER iter = ppet_owner->m_mapPETCardInfo.find ( pinven_item->sItemCustom.dwPetID );
		if ( iter==ppet_owner->m_mapPETCardInfo.end() ) return;
		sPet = (*iter).second;
	}
	else
	{
		sPet.m_nFull  = m_nFull;
		sPet.m_emTYPE = m_emTYPE;
		sPet.m_bDualSkill = m_bDualSkill;
	}

	if ( sPet.m_bDualSkill )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_ALREADY_ENABLED") );
		return;
	}

	GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.emPetTYPE = sPet.m_emTYPE;

	NETSENDTOFIELD ( &NetMsg );
}