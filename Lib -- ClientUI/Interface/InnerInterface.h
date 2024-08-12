#pragma	once

#include "../Lib -- Engine/GUInterface/UIMan.h"
#include "../Lib -- Client/G-Logic/GLCharClient.h"
#include "./InnerInterfaceGuid.h"

/*pvp tyranny, Juver, 2017/08/25 */
#include "GLPVPTyrannyDefine.h" 

//itemmall
class	CItemMallIcon;
class	CItemShopWindow;
class	CAutoSystemWindow;
class	CAutoSkillPageToTray;

class	CAcademyConftDisplay;	
class	CAdminMessageDisplay;	
class	CBasicButton;	
class	CBasicChat;	
class	CBasicGameMenu;	
class   CQBoxButton;
class	CBasicInfoView;	
class	CBasicPotionTray;	
class	CBasicQuickSkillSlot;	
class	CBasicSkillTray;	
class	CBasicVarTextBox;	
class	CBlockProgramAlarm;	
class	CBusWindow;	
class	CCharacterWindow;	
class	CChatMacroWindow;	
class	CClubMake;	
class	CClubStorageWindow;	
class	CClubWindow;	
class	CConftConfirm;	
class	CConftDisplayMan;	
class	CConftModalWindow;	
class	CCountMsg;	
class	CDamageDisplayMan;	
class	CDialogueWindow;	
class	CFriendWindow;	
class	CGambleAgainBox;	
class	CGambleAnimationBox;	
class   CPetSkinMixImage;
class	CGambleBox;	
class	CGambleResultEvenBox;	
class	CGambleResultOddBox;	
class	CGambleSelectBox;	
class	CHeadChatDisplayMan;	
class	CInventoryWindow;
class	CItemBankWindow;	
class	CVNGainSysInventory;
class   CVNGainSysGauge;
class	CItemMove;	
class	CItemRebuild;	// ITEMREBUILD_MARK
class	CKeySettingWindow;	
class	CLargeMapWindow;	
class	CMapMoveDisplay;	
class	CMarketWindow;	
class	CMiniMap;	
class	CMiniPartyWindow;	
class	CModalWindow;	
class	CNameDisplayMan;	
class	CPartyModalWindow;	
class	CPartyWindow;	
class	CPetRebirthDialogue;// Monster7j	
class	CPetWindow;	
class	CPrivateMarketMake;	
class	CPrivateMarketSellWindow;	
class	CPrivateMarketShowMan;	
class	CPrivateMarketWindow;	
class	CPtoPWindow;	
class	CQuestAlarm;	
class	CQuestionItemDisplay;	
class	CQuestWindow;	
class	CRebirthDialogue;	
class	CRebuildInventoryWindow;	
class	CReceiveNoteWindow;	
class	CSimpleHP;	
class	CSimpleMessageMan;	
class	CSkillTrayTab;	
class	CSkillWindowToTray;	
class	CSMSSendWindow;	
class	CStorageChargeCard;	
class	CStorageWindow;	
class	CSubMasterSet;	
class	CSystemMessageWindow;	
class	CTargetInfoDisplay;	
class	CTargetInfoDisplayNpc;
class	CTargetInfoDisplayPlayer;
class	CTradeInventoryWindow;	
class	CTradeWindow;	
class	CUILeftTopGroup;	
class	CWaitServerDialogue;	
class	CWriteNoteWindow;	
struct	GLCHARLOGIC;	
class	GLCLUB;	
class	GLMapAxisInfo;	
struct	GLSKILL;	
struct	SCHARSKILL;	
struct	SITEMCUSTOM;	
struct	SNpcTalk;	
class	CBonusTimeGauge;
class	CBonusTimeDisplay;
class	CQuestHelper;
class	CVehicleWindow;
class	CThaiCCafeMark;
class	CItemGarbage;
class	CGarbageInventoryWindow;
class	CItemShopIconMan;
class	CShopItemSearchWindow;
class	CItemSearchResultWindow;
class	CSummonWindow;
class	CSummonPosionDisplay;
class	CAttendanceBookWindow;
class	CClubBattleModal;
class	CMapRequireCheck;
class	CTaxiWindow;
class	CItemMixWindow;
class	CItemMixInvenWindow;
class	CGatherGauge;
class	CPKComboDisplay;
class	CCdmRankingDisplay;

 /*game stats, Juver, 2017/06/22 */
class	CGameStats;
class	CPKRankNotification;

/*npc shop, Juver, 2017/07/25 */
class	CNPCShopWindow;

/*crow target*/
class	CCrowTargetInfo;
class	CCrowTargetInfoNpc;
class	CCrowTargetInfoPlayer;

/*item preview, Juver, 2017/07/27 */
class	CItemPreviewWindow;

/*pet status, Juver, 2017/07/30 */
class	CPetStatus; 

 /*vehicle status, Juver, 2017/07/30 */
class	CVehicleStatus;

/*item link, Juver, 2017/07/31 */
class	CBasicVarTextBoxItemLink; 

/*vehicle booster system, Juver, 2017/08/11 */
class	CVehicleBoosterDisplay; 

/*box contents, Juver, 2017/08/29 */
class	CInfoBoxContents;			

/* Competition UI, Juver, 2017/08/24 */
class	CCompetitionNotifyButton;
class	CCompetitionWindow;

/* extra notice, Juver, 2017/08/24 */
class	CExtraNotice;

/*pvp tyranny, Juver, 2017/08/25 */
class	CPVPTyrannyTowerCapture;
class	CPVPTyrannyTowerProgress;
class	CPVPTyrannyRebirthDialogue;
class	CPVPTyrannyCaptureNotice;
class	CPVPTyranny2CaptureNotice;
class	CPVPTyrannyRankings;
class	CPVPTyrannyTopRankings;
class	CPVPTyrannyBattleNotice;

/*item exchange, Juver, 2017/10/12 */
class	CNPCItemExchangeWindow;


/*product item, Juver, 2017/10/15 */
class	CProductButton;
class	CProductWindow;

/*student record ui, Juver, 2017/10/30 */
class	CStudentRecordButton;
class	CStudentRecordWindow;

/*activity system, Juver, 2017/11/05 */
class	CActivityItemWindow;

/*quest ui, Juver, 2017/11/06 */
class	CModernQuestWindow;

/*charinfoview , Juver, 2017/11/11 */
class	CCharacterInfoViewWindow;

/*bike color , Juver, 2017/11/12 */
class	CBikeColorWindow;

/*pet skill info display, Juver, 2017/12/01 */
/*dual pet skill, Juver, 2017/12/29 */
class	CPetSkillInfoDisplay;
class	CPetSkillInfoDisplay2;

/*change scale card, Juver, 2018/01/03 */
class	CChangeScaleWindow;

/*item color, Juver, 2018/01/10 */
class	CItemColorWindow;

/*change school card, Juver, 2018/01/12 */
class	CChangeSchoolWindow;

/*equipment lock, Juver, 2018/01/13 */
class	CEquipmentLockEnableWindow;
class	CEquipmentLockInputWindow;
class	CEquipmentLockRecoverWindow;

/*inventory,equipment,storage lock, DevArt22, 2020/02/11 */
class	CPersonalLockWindow;

/*inventory lock, DevArt22, 2020/02/11 */
class	CInventoryLockEnableWindow;
class	CInventoryLockInputWindow;

/*storage lock, DevArt22, 2020/02/11 */
class	CStorageLockEnableWindow;
class	CStorageLockInputWindow;

/*item transfer card, Juver, 2018/01/18 */
class	CItemTransferWindow;
class	CItemTransferInvenWindow;

/*school wars, Juver, 2018/01/19 */
class	CPVPSchoolWarsScore;
class	CPVPSchoolWarsRebirthDialogue;
class	CPVPSchoolWarsTopRankings;
class	CPVPSchoolWarsBattleNotice;
class	CPVPSchoolWarsRankings;

/*pvp capture the flag, Juver, 2018/02/01 */
class	CPVPCaptureTheFlagRebirthDialogue;
class	CPVPCaptureTheFlagTopRankings;
class	CPVPCaptureTheFlagBattleNotice;
class	CPVPCaptureTheFlagScore;
class	CPVPCaptureTheFlagRankings;
class	CPVPCaptureTheFlagHoldIcon;

/* car, cart color, Juver, 2018/02/14 */
class	CCarColorWindow;
class	CProcessListDisplay; /*get process command, Juver, 2017/06/08 */

//dmk14 | 11-4-16 | pk ranking
class	PlayerRankingDisplay;

/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
class	CGlobalRankingWindow;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// LG-7 CharView
class	CCharViewWindow;
/////////////////////////////////////////////////////////////////////////////
//	NOTE
//		대련시에 키워드
enum
{
	CONFT_WIN_INDEX = 0,
	CONFT_LOSE_INDEX,
	CONFT_DRAW_INDEX
};

//	NOTE
//		학교
enum
{	
	SUNGMOON = 0,
	HYUNAM,
	BONGHWANG,
	INIT_SCHOOL = 100,
};

const float fDEFAULT_WAITTIME_LEFT = 5.0f;

class	CInnerInterface : public CUIMan
{
private:
	//	정보 표시 타입
	enum ET_INFOTYPE
	{		
		ET_ITEM_INFO,
		ET_SKILL_INFO,
		ET_GENERAL_INFO,
		ET_ITEM_INFO_LINK, /*item link, Juver, 2017/07/31 */
	};

public:
	static const int nOUTOFRANGE;
	static const float fMENU_LIFTUP;

private: //	등록되는 모든 컨트롤
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	CGlobalRankingWindow*		m_pGlobalRankingWindow;
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	CCharViewWindow*			m_pCharViewWindow;
	/////////////////////////////////////////////////////////////////////////////

	CAutoSystemWindow*			m_pAutoSystemWindow;
	CAutoSkillPageToTray*		m_pAutoSkillPageToTray;
	CAcademyConftDisplay*		m_pAcademyConftDisplay;				
	CAdminMessageDisplay*		m_pAdminMessageDisplay;				
	CBasicChat*					m_pChat;				
	CBasicGameMenu*				m_pGameMenu;		
	CQBoxButton*				m_pQBoxButton;
	CBasicInfoView*				m_pBasicInfoView;	
	CBasicQuickSkillSlot*		m_pBasicQuickSkillSlot;				
	CBasicVarTextBox*			m_pInfoDisplay;			
	CBasicVarTextBox*			m_pInfoDisplayEx;
	CBlockProgramAlarm*			m_pBlockProgramAlarm;				
	CBusWindow*					m_pBusWindow;				
	CCharacterWindow*			m_pCharacterWindow;			
	CChatMacroWindow*			m_pChatMacroWindow;			
	CClubMake*					m_pClubMake;	
	CClubStorageWindow*			m_pClubStorageWindow;			
	CClubWindow*				m_pClubWindow;		
	CConftConfirm*				m_pConftConfirm;		
	CConftDisplayMan*			m_pConftDisplayMan;			
	CConftModalWindow*			m_pConftModalWindow;			
	CCountMsg*					m_pCountMsgWindow;	
	CDamageDisplayMan*			m_pDamageDisplayMan;				
	CDialogueWindow*			m_pDialogueWindow;				
	CFriendWindow*				m_pFriendWindow;				
	CGambleAgainBox*			m_pGambleAgainBox;			
	CGambleAnimationBox*		m_pGambleAnimationBox;				
	CPetSkinMixImage*			m_pPetSkinMixImage;
	CGambleBox*					m_pGambleBox;	
	CGambleResultEvenBox*		m_pGambleResultEvenBox;				
	CGambleResultOddBox*		m_pGambleResultOddBox;				
	CGambleSelectBox*			m_pGambleSelectBox;			
	CHeadChatDisplayMan*		m_pHeadChatDisplayMan;				
	CInventoryWindow*			m_pInventoryWindow;				
	CItemBankWindow*			m_pItemBankWindow;
	CItemShopWindow*			m_pItemShopWindow;	//itemmall
	CVNGainSysInventory*		m_pVNGainSysInventory;				
	CVNGainSysGauge*			m_pVNGainSysGauge;
	CItemMove*					m_pItemMove;		
	CItemRebuild*				m_pItemRebuildWindow;	// ITEMREBUILD_MARK	
	CKeySettingWindow*			m_pKeySettingWindow;			
	CLargeMapWindow*			m_pLargeMapWindow;			
	CMapMoveDisplay*			m_pMapMoveDisplay;			
	CMarketWindow*				m_pMarketWindow;			
	CMiniMap*					m_pMiniMap;				
	CModalWindow*				m_pModalWindow;		
	CNameDisplayMan*			m_pNameDisplayMan;				
	CPartyModalWindow*			m_pPartyModalWindow;			
	CPartyWindow*				m_pPartyWindow;			
	CPetRebirthDialogue*		m_pPetRebirthDialogue;// Monster7j				
	CPetWindow*					m_pPetWindow;	
	CPrivateMarketMake*			m_pPrivateMarketMake;			
	CPrivateMarketSellWindow*	m_pPrivateMarketSellWindow;				
	CPrivateMarketShowMan*		m_pPrivateMarketShowMan;				
	CPrivateMarketWindow*		m_pPrivateMarketWindow;				
	CPtoPWindow*				m_pPtoPWindow;			
	CQuestAlarm*				m_pQuestAlarm;				
	CQuestionItemDisplay*		m_pQuestionItemDisplay;				
	CQuestWindow*				m_pQuestWindow;				
	CRebirthDialogue*			m_pRebirthDialogue;			
	CRebuildInventoryWindow*	m_pRebuildInventoryWindow;				
	CSimpleHP*					m_pSimpleHP;				
	CSimpleHP*					m_pSummonHP;				
	CSkillTrayTab*				m_pSkillTrayTab;		
	CSkillWindowToTray*			m_pSkillWindowToTray;				
	CStorageChargeCard*			m_pStorageChargeCard;			
	CStorageWindow*				m_pStorageWindow;			
	CSubMasterSet*				m_pSubMasterSet;		// 부마스터 설정 창
	CSystemMessageWindow*		m_pSystemMessageWindow;				
	CTargetInfoDisplay*			m_pTargetInfoDisplay;			
	CTargetInfoDisplayNpc*		m_pTargetInfoDisplayNpc;
	CTargetInfoDisplayPlayer*	m_pTargetInfoDisplayPlayer;	
	CTradeInventoryWindow*		m_pTradeInventoryWindow;				
	CTradeWindow*				m_pTradeWindow;			
	CUILeftTopGroup*			m_pUILeftTopGroup;			
	CWaitServerDialogue*		m_pWaitServerDisplay;
	CBonusTimeGauge*			m_pBonusTimeGauge;
	CBonusTimeDisplay*			m_pBonusTimeDisplay;
	CQuestHelper*				m_pQuestHelper;
	CVehicleWindow*				m_pVehicleWindow;
	CThaiCCafeMark*				m_pThaiCCafeMark;
	CItemGarbage*				m_pItemGarbageWindow;
	CGarbageInventoryWindow*	m_pGarbageInventoryWindow;
	CItemShopIconMan*			m_pItemShopIconMan;
	CShopItemSearchWindow*		m_pShopItemSearchWindow;
	CItemSearchResultWindow*	m_pItemSearchResultWindow;
	CSummonWindow*				m_pSummonWindow;
	CAttendanceBookWindow*		m_pAttendanceBookWindow;
	CClubBattleModal*			m_pClubBattleModalWindow;
	CTaxiWindow*				m_pTaxiWindow;
	CMapRequireCheck*			m_pMapRequireCheckWindow;
	CItemMixWindow*				m_pItemMixWindow;
	CItemMixInvenWindow*		m_pItemMixInvenWindow;
	CGatherGauge*				m_pGatherGauge;
	CPKComboDisplay*			m_pPKComboDisplay;
	CCdmRankingDisplay*			m_pCdmRankingDisplay;
	CProcessListDisplay*		m_pProcessListDisplay; /*get process command, Juver, 2017/06/08 */

	CItemMallIcon*				m_pItemMallIcon; //itemmall
		CBasicButton*			pButton;

	/*game stats, Juver, 2017/06/22 */
	CGameStats*					m_pGameStats; 
	CPKRankNotification*		m_pPKRankNotification;

	/*npc shop, Juver, 2017/07/25 */
	CNPCShopWindow*				m_pNPCShopWindow;

	/*item preview, Juver, 2017/07/27 */
	CItemPreviewWindow*			m_pItemPreviewWindow;
	
	/*crow target*/
	CCrowTargetInfo*			m_pCrowTargetInfo; //Add new Interface
	CCrowTargetInfoNpc*			m_pCrowTargetInfoNpc; //Add new Interface
	CCrowTargetInfoPlayer*		m_pCrowTargetInfoPlayer; //Add new Interface


	/*pet status, Juver, 2017/07/30 */
	CPetStatus*					m_pPetStatus; 

	/*vehicle status, Juver, 2017/07/30 */
	CVehicleStatus*				m_pVehicleStatus; 

	 /*item link, Juver, 2017/07/31 */
	CBasicVarTextBoxItemLink*	m_pInfoDisplayItemLink;	

	/*vehicle booster system, Juver, 2017/08/11 */
	CVehicleBoosterDisplay*		m_pVehicleBoosterDisplay; 

	/*box contents, Juver, 2017/08/29 */
	CInfoBoxContents*			m_pInfoBoxContents;		

	/* Competition UI, Juver, 2017/08/24 */
	CCompetitionNotifyButton*	m_pCompetitionNotifyButton;
	CCompetitionWindow*			m_pCompetitionWindow;

	/* extra notice, Juver, 2017/08/24 */
	CExtraNotice*				m_pExtraNotice; 

	/*pvp tyranny, Juver, 2017/08/25 */
	CPVPTyrannyTowerCapture*	m_pPVPTyrannyTowerCapture;
	CUIControl*					m_pPVPTyrannyTowerCaptureDummy;
	CPVPTyrannyTowerProgress*	m_pPVPTyrannyTowerProgress;
	CPVPTyrannyRebirthDialogue*	m_pRebirthDialogueTyranny;
	CPVPTyrannyCaptureNotice*	m_pPVPTyrannyNoticeCapture[TYRANNY_SCHOOL_SIZE];
	CPVPTyranny2CaptureNotice*	m_pPVPTyranny2Capture[TYRANNY_SCHOOL_SIZE];
	CPVPTyranny2CaptureNotice*	m_pPVPTyranny2CaptureOwn;			
	CPVPTyrannyRankings*		m_pPVPTyrannyRankings;
	CPVPTyrannyTopRankings*		m_pPVPTyrannyTopRankings;
	CPVPTyrannyBattleNotice*	m_pPVPTyrannyBattleNoticeStart;
	CPVPTyrannyBattleNotice*	m_pPVPTyrannyBattleNoticeEnd;

	/*item exchange, Juver, 2017/10/12 */
	CNPCItemExchangeWindow*		m_pNPCItemExchangeWindow;

	/*product item, Juver, 2017/10/15 */
	CProductButton*				m_pProductButton;
	CProductWindow*				m_pProductWindow;

	/*student record ui, Juver, 2017/10/30 */
	CStudentRecordButton*		m_pStudentRecordButton;
	CStudentRecordWindow*		m_pStudentRecordWindow;

	/*activity system, Juver, 2017/11/05 */
	CActivityItemWindow*		m_pActivityItemWindow;

	/*quest ui, Juver, 2017/11/06 */
	CModernQuestWindow*			m_pModernQuestWindow;

	/*charinfoview , Juver, 2017/11/11 */
	CCharacterInfoViewWindow*	m_pCharacterInfoViewWindow;

	/*bike color , Juver, 2017/11/12 */
	CBikeColorWindow*			m_pBikeColorWindow;

	/*pet skill info display, Juver, 2017/12/01 */
	/*dual pet skill, Juver, 2017/12/29 */
	CPetSkillInfoDisplay*		m_pPetSkillInfoDisplay;
	CPetSkillInfoDisplay2*		m_pPetSkillInfoDisplay2;

	/*change scale card, Juver, 2018/01/03 */
	CChangeScaleWindow*			m_pChangeScaleWindow;

	/*item color, Juver, 2018/01/10 */
	CItemColorWindow*			m_pItemColorWindow;

	/*change school card, Juver, 2018/01/12 */
	CChangeSchoolWindow*		m_pChangeSchoolWindow;

	/*equipment lock, Juver, 2018/01/13 */
	CEquipmentLockEnableWindow*		m_pEquipmentLockEnableWindow;
	CEquipmentLockInputWindow*		m_pEquipmentLockInputWindow;
	CEquipmentLockRecoverWindow*	m_pEquipmentLockRecoverWindow;

	/*inventory,equipment,storage lock, DevArt22, 2020/02/11 */
	CPersonalLockWindow*			m_pPersonalLockWindow;
	CUIControl*						m_pPersonalLockWindowDummy;

	/*inventory lock, DevArt22, 2020/02/11 */
	CInventoryLockEnableWindow*		m_pInventoryLockEnableWindow;
	CInventoryLockInputWindow*		m_pInventoryLockInputWindow;

	/*storage lock, DevArt22, 2020/02/11 */
	CStorageLockEnableWindow*		m_pStorageLockEnableWindow;
	CStorageLockInputWindow*		m_pStorageLockInputWindow;

	/*item transfer card, Juver, 2018/01/18 */
	CItemTransferWindow*		m_pItemTransferWindow;
	CItemTransferInvenWindow*	m_pItemTransferInvenWindow;

	/*school wars, Juver, 2018/01/19 */
	CPVPSchoolWarsScore*				m_pPVPSchoolWarsScore;
	CUIControl*							m_pPVPSchoolWarsScoreDummy;
	CPVPSchoolWarsRebirthDialogue*		m_pRebirthDialogueSchoolWars;
	CPVPSchoolWarsTopRankings*			m_pPVPSchoolWarsTopRankings;
	CPVPSchoolWarsBattleNotice*			m_pPVPSchoolWarsBattleNoticeStart;
	CPVPSchoolWarsBattleNotice*			m_pPVPSchoolWarsBattleNoticeEnd;
	CPVPSchoolWarsRankings*				m_pPVPSchoolWarsRankings;

	/*pvp capture the flag, Juver, 2018/02/01 */
	CPVPCaptureTheFlagRebirthDialogue*	m_pRebirthDialogueCaptureTheFlag;
	CPVPCaptureTheFlagTopRankings*		m_pPVPCaptureTheFlagTopRankings;
	CPVPCaptureTheFlagBattleNotice*		m_pPVPCaptureTheFlagBattleNoticeStart;
	CPVPCaptureTheFlagBattleNotice*		m_pPVPCaptureTheFlagBattleNoticeEnd;
	CPVPCaptureTheFlagScore*			m_pPVPCaptureTheFlagScore;
	CUIControl*							m_pPVPCaptureTheFlagScoreDummy;
	CPVPCaptureTheFlagRankings*			m_pPVPCaptureTheFlagRankings;
	CPVPCaptureTheFlagHoldIcon*			m_pPVPCaptureTheFlagHoldIcon;

	/* car, cart color, Juver, 2018/02/14 */
	CCarColorWindow*					m_pCarColorWindow;

	//dmk14 | 11-4-16 | pk ranking
	PlayerRankingDisplay*		m_pPlayerRankingDisplay;

private:
	CUIControl*	m_pSystemMessageWindowDummy;
	CUIControl*	m_pMapMoveDisplayDummy;
	CUIControl*	m_pBlockProgramAlarmDummy;
	CUIControl*	m_pLeftTopGroupDummy;
	CUIControl*	m_pBasicInfoViewDummy;
	CUIControl* m_pBasicPotionTrayDummy;
	CUIControl* m_pQuestAlarmDummy;

	void ResetControl();

private:
	bool m_bFirstVNGainSysCall;

public:
	CInnerInterface ();
	virtual	~CInnerInterface ();

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT OneTimeSceneInit ();
	virtual	HRESULT FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT FinalCleanup ();

public:
	void SetMiniMapInfo ( GLMapAxisInfo &sInfo, CString strMapName );
	void SetFirstVNGainSysCall ( bool bFirstCall ) { m_bFirstVNGainSysCall = bFirstCall; }

public:
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	CGlobalRankingWindow*	GetGlobalRankingWindow()		{ return m_pGlobalRankingWindow; }
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	CCharViewWindow*		GetCharViewWindow()				{ return m_pCharViewWindow; }
	/////////////////////////////////////////////////////////////////////////////

	CAutoSystemWindow*			GetAutoSystemWindow()			{ return m_pAutoSystemWindow; }
	CAutoSkillPageToTray*		GetAutoSkillPageToTray()		{ return m_pAutoSkillPageToTray; }
	CItemShopWindow*		GetItemShop()					{ return m_pItemShopWindow; } //itemmall
	CMiniMap*				GetMiniMap()					{ return m_pMiniMap; }
	CItemRebuild* 			GetItemRebuild() 				{ return m_pItemRebuildWindow; 	} //sealed card NaJDeV
	CSkillWindowToTray*		GetSkillWindowToTray()			{ return m_pSkillWindowToTray; }
	CPartyWindow*			GetPartyWindow()				{ return m_pPartyWindow; }
	CBasicChat*				GetChat()						{ return m_pChat; }
	CNameDisplayMan*		GetDispName()					{ return m_pNameDisplayMan; }
	CPrivateMarketShowMan*	GetPMarketShow()				{ return m_pPrivateMarketShowMan; }
	CTradeWindow*			GetTradeWindow()				{ return m_pTradeWindow; }
	CTradeInventoryWindow*	GetTradeInventoryWindow()		{ return m_pTradeInventoryWindow; }
	CPtoPWindow*			GetPtoPWindow()					{ return m_pPtoPWindow; }
	CInventoryWindow*		GetInventoryWindow()			{ return m_pInventoryWindow; }
	CItemMove*				GetItemMove()					{ return m_pItemMove; }
	CMarketWindow*			GetMarketWindow()				{ return m_pMarketWindow; }
	CHeadChatDisplayMan*	GetHeadChatDisplayMan()			{ return m_pHeadChatDisplayMan; }
	CConftModalWindow*		GetConftModalWindow()			{ return m_pConftModalWindow; }
	CPartyModalWindow*		GetPartyModalWindow()			{ return m_pPartyModalWindow; }
	CConftDisplayMan*		GetConftDisplayMan()			{ return m_pConftDisplayMan; }
	CQuestWindow*			GetQuestWindow()				{ return m_pQuestWindow; }
	CFriendWindow*			GetFriendWindow()				{ return m_pFriendWindow; }
	CBasicGameMenu*			GetGameMenu()					{ return m_pGameMenu; }
	CQBoxButton*			GetQBoxButton()					{ return m_pQBoxButton; }
	CPetWindow*				GetPetWindow()					{ return m_pPetWindow; }
	CCharacterWindow*		GetCharacterWindow()			{ return m_pCharacterWindow; }
	CGambleBox*				GetGambleBox()					{ return m_pGambleBox; }
	CGambleSelectBox*		GetGambleSelectBox()			{ return m_pGambleSelectBox; }
	CGambleAgainBox*		GetGambleAgainBox()				{ return m_pGambleAgainBox; }
	CGambleAnimationBox*	GetGambleAnimationBox()			{ return m_pGambleAnimationBox; }
	CPetSkinMixImage*		GetPetSkinMixImage()			{ return m_pPetSkinMixImage; }
	CGambleResultEvenBox*	GetGambleResultEvenBox()		{ return m_pGambleResultEvenBox; }
	CGambleResultOddBox*	GetGambleResultOddBox()			{ return m_pGambleResultOddBox; }
	CModalWindow*			GetModalWindow()				{ return m_pModalWindow; }
	CKeySettingWindow*		GetKeySettingWindow()			{ return m_pKeySettingWindow; }
	CChatMacroWindow*		GetChatMacroWindow()			{ return m_pChatMacroWindow; }
	CItemShopIconMan*		GetItemShopIconMan()			{ return m_pItemShopIconMan; }
	CShopItemSearchWindow*	GetShopItemSearchWindow()		{ return m_pShopItemSearchWindow; }
	CItemSearchResultWindow* GetItemSearchResultWindow()	{ return m_pItemSearchResultWindow; }
	CSummonWindow*			GetSummonWindow()				{ return m_pSummonWindow; }
	CMapRequireCheck*		GetRequireCheck()				{ return m_pMapRequireCheckWindow; }
	CGameStats*				GetGameStatsDisplay()			{ return m_pGameStats; }				/*game stats, Juver, 2017/06/22 */
	CNPCShopWindow*			GetNPCShopWindow()				{ return m_pNPCShopWindow; }			/*npc shop, Juver, 2017/07/25 */
	CNPCItemExchangeWindow*	GetNPCItemExchangeWindow()		{ return m_pNPCItemExchangeWindow; }	/*item exchange, Juver, 2017/10/12 */
	CModernQuestWindow*		GetModernQuestWindow()			{ return m_pModernQuestWindow; }		/*quest ui, Juver, 2017/11/06 */
	CQuestHelper*			GetQuestHelper()				{ return m_pQuestHelper;	}			/*quest ui, Juver, 2017/11/11 */
	CCharacterInfoViewWindow*	GetCharInfoViewWindow()		{ return m_pCharacterInfoViewWindow; }	/*charinfoview , Juver, 2017/11/11 */
	CPVPSchoolWarsScore*	GetSchoolWarsScoreWindow()		{ return m_pPVPSchoolWarsScore; }		/*school wars, Juver, 2018/01/20 */
	/*inventory,equipment,storage lock, DevArt22, 2020/02/11 */
	CPersonalLockWindow*	GetPersonalLockWindow()			{ return m_pPersonalLockWindow; }
	
public:
	void	CloseAllWindow ();

public:
	BOOL	PrintMsgText ( D3DCOLOR dwColor, const char* szFormat, ... );
	BOOL	PrintConsoleText ( const char* szFormat, ... );

	BOOL	PrintMsgTextDlg ( D3DCOLOR dwColor, const char* szFormat, ... );
	BOOL	PrintConsoleTextDlg ( const char* szFormat, ... );

	BOOL	PrintMsgDlg ( D3DCOLOR dwColor, const char* szFormat, ... );

public:
	const char* MakeString ( const char* szFormat, ... );

	

public:
	BOOL IsGateOpen()							{ return m_bGateOpen; }
	void SetGateOpen( BOOL bGateOpen )			{ m_bGateOpen = bGateOpen; }
	void SetDamage( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack );


	//SetTarget Info
	void SetTargetInfo( STARGETID sTargetID );
	void SetTargetInfoNpc( STARGETID sTargetID );		//add target info
	void SetTargetInfoPlayer( STARGETID sTargetID );	//add target info
	
	void ResetTargetInfo()						{ HideGroup ( TARGETINFO_DISPLAY ); }
	void ResetTargetInfoNpc()					{ HideGroup ( TARGETINFO_DISPLAY_NPC ); }		//add target info
	void ResetTargetInfoPlayer()				{ HideGroup ( TARGETINFO_DISPLAY_PLAYER ); }	//add target info

	void SetTargetInfoCrow ( STARGETID sTargetID );			//add target info
	void SetTargetInfoCrowNpc ( STARGETID sTargetID );		//add target info
	void SetTargetInfoCrowPlayer ( STARGETID sTargetID );	//add target info

	void ResetTargetInfoCrow()					{ HideGroup ( CROW_TARGET_INFO ); }			//add target info
	void ResetTargetInfoCrowNpc()				{ HideGroup ( CROW_TARGET_INFO_NPC ); }		//add target info
	void ResetTargetInfoCrowPlayer()			{ HideGroup ( CROW_TARGET_INFO_PLAYER ); }	//add target info

	void SetFightBegin ( const int nIndex );
	void SetFightEnd ( const int nResult );

	void SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER );
	void SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER );
	void SetAcademyFightEnd ();

	void SetBlockProgramFound( bool bFOUND )	{ m_bBlockProgramFound = bFOUND; }
	bool IsBlockProgramFound()					{ return m_bBlockProgramFound; }

	void SetBlockProgramAlarm( bool bAlarm )	{ m_bBlockProgramAlarm = bAlarm; }
	bool IsBlockProgramAlarm()					{ return m_bBlockProgramAlarm; }

public:	
	void SetAutoSystemWindowOpen();
	void SetAutoSystemWindowClose();

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	void	SetGlobalRankingWindowOpen(BOOL bOpen);
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 CharView
	void	SetCharViewWindowOpen(BOOL bOpen);
	/////////////////////////////////////////////////////////////////////////////

	void	SetDialogueWindowOpen ( STARGETID sCrowID, GLCHARLOGIC* pCHAR );
	void	SetPtoPWindowOpen ( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub );
	void	SetQuestWindowOpen ( DWORD dwQuestID );
	void	SetBusWindowOpen ( const DWORD dwGaeaID, SNpcTalk* pNpcTalk );

	void	SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID );
	void	SetTradeWindowClose ();
	void	SetStorageWindowOpen ( DWORD dwNPCID );
	void	SetStorageWindowClose ();
	void	SetClubStorageWindowOpen ();
	void	SetClubStorageWindowClose ();
	void	SetMarketWindowOpen ( SNATIVEID sNativeID );
	void	SetMarketWindowClose ();
	void	SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY );
	void	SetItemBankWindowOpen ();
	void	SetItemBankInfo ();
	//itemmall
	void	SetItemShopInfo ();
	void	SetItemShopWindowOpen ();
	void	SetVNGainSysWindowOpen ();
	void	SetVNGainSysInfo ();
	void	SetDefaultPosInterface(UIGUID ControlID);

	void	OpenItemRebuildWindow();	// ITEMREBUILD_MARK
	void	CloseItemRebuildWindow();

	void	OpenItemGarbageWindow();	// 휴지통
	void	CloseItemGarbageWindow();

	void	OpenItemMixWindow( DWORD dwNpcID );		// 아이템 조합
	void	CloseItemMixWindow();
	void	SetItemMixResult( CString strMsg, bool bSuccess = false, bool bFail = false );

	void	OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY );			// 택시 카드

	void	SetPrivateMarketMake ();
	void	SetPrivateMarketOpen ( const bool& bOPENER, const DWORD& dwGaeaID );
	void	GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID );
	void	SetPrivateMarketClose ();	

	void	MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY );

	void	SetClubMake ( const DWORD& dwNpcID );

	void	SetShotcutText ( DWORD nID, CString& strTemp );
	void	GetShotCutKey();
	void	GetChatMacro();
	void	AddChatMacro(int nIndex);
	void	ChatLog( bool bChatLogt, int nChatLogType );
	CString GET_RECORD_CHAT();
	CString GetdwKeyToString(int dwKey);

	void	SetArmSwap( BOOL bSwap );

	/*npc shop, Juver, 2017/07/25 */
	void	SetNPCShopWindowOpen ( SNATIVEID sNativeID );

public:
	void	SetLottoOpen ( const DWORD& dwGaeaID );

public:
	void	DOMODAL_ADD_FRIEND_REQ( const CString& strName );
	void	DOMODAL_CLUB_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster );
	void	DOMODAL_UNION_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubMaster );
	void	DOMODAL_CLUB_BATTLE_ASK( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance = false );
	void	DOMODAL_CLUB_BATTLE_ARMISTICE_ASK( const DWORD& dwClubID, const CString& strClubName, bool bAlliance = false );
	void	DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName );

	void	OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg );

public:
	DWORD GetEventQuestID()							{ return m_dwEventQuestID; }
	void SetEventQuestID( DWORD dwQuestID )			{ m_dwEventQuestID = dwQuestID; }

public:
	void	WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer = fDEFAULT_WAITTIME_LEFT );
	void	WAITSERVER_DIALOGUE_CLOSE();

public:
	void	ClearNameList ();

public:
	void	SetTradeWindowCloseReq ();

private:
	HRESULT	TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT	TEXTURE_PRE_UNLOAD ();

public:
	BOOL IsInventoryWindowOpen()			{ return IsVisibleGroup( INVENTORY_WINDOW ); }
	BOOL IsStorageWindowOpen()				{ return IsVisibleGroup( STORAGE_WINDOW ); }
	BOOL IsClubStorageWindowOpen()			{ return IsVisibleGroup( CLUB_STORAGE_WINDOW ); }
	BOOL IsMarketWindowOpen()				{ return IsVisibleGroup( MARKET_WINDOW ); }

public:
	// 인터페이스 사용시 캐릭터의 움직임을 제어
	BOOL IsCharMoveBlock()							{ return m_bCharMoveBlock; }
	void SetCharMoveBlock()							{ m_bCharMoveBlock = TRUE; }
	void ResetCharMoveBlock()						{ m_bCharMoveBlock = FALSE; }
	BOOL IsOpenWindowToMoveBlock();

	BOOL IsSnapItem()								{ return m_bSnapItem; }
	void SetSnapItem()								{ m_bSnapItem = TRUE; }
	void ResetSnapItem()							{ m_bSnapItem = FALSE; }

	void SetFirstItemSlot()							{ m_bFirstItemSlot = false;	}
	void ResetFirstItemSlot()						{ m_bFirstItemSlot = true; }
	bool IsFirstItemSlot()							{ return m_bFirstItemSlot; }

	void SetFriendName( const CString& strName )	{ m_strMoveFriendName = strName; }
	const CString& GetFriendName() const			{ return m_strMoveFriendName; }

	void SetThaiCCafeClass( DWORD dwClass );
	void SetMyCCafeClass( int nClass );				// 말레이시아 PC방 이벤트

public:
	bool	SET_QUESTION_ITEM_ID ( int nID );

	bool	SET_KEEP_QUESTION_ITEM_ID ( int nID );
	void	RESET_KEEP_QUESTION_ITEM ();

	bool	SET_PK_COMBO ( int nID );
	void	RESET_PK_COMBO ();

	void	BONUS_TIME_EVENT_START( bool bCharging ); // 이벤트 시작
	void	BONUS_TIME_EVENT_END(); // 이벤트 종료
	void	BONUS_TIME_BUSTER_START(); // 경험치 시작
	void	BONUS_TIME_BUSTER_END(); // 경험치 종료

	// 베트남 탐닉 방지 시스템 게이지
	void	SET_VNGAINTYPE_GAUGE( int nPos, int nLimit );

	void	SET_QUEST_HELPER( DWORD dwQuestID );
	void	RESET_QUEST_HELPER( DWORD dwQuestID );

public:
	void	SET_CONFT_CONFIRM_VISIBLE ( bool bVisible );
	void	SET_CONFT_CONFIRM_PERCENT ( float fPercent );

public:
	void SetSkillUpID( const DWORD& dwID )			{ m_dwSkillUpID = dwID; }
	DWORD GetSkillUpID()							{ return m_dwSkillUpID; }

	void SetItemInfoTemp( const SITEMCUSTOM& sID )		{ m_sItemInfoTemp = sID; }
	SITEMCUSTOM GetItemInfoTemp()						{ return m_sItemInfoTemp; }

private:
	void MoveBasicInfoWindow ();
	void BasicInfoViewDoubleClick();

private:
	BOOL	IsPartyMember ( const char* szName );

public:
	void	REFRESH_QUEST_WINDOW ();

	void	REFRESH_FRIEND_LIST ();
	void	REFRESH_FRIEND_STATE();

	void	REFRESH_CLUB_LIST ();
	void	REFRESH_CLUB_STATE ();
	void	REFRESH_CLUB_NOTICE();
	void	REFRESH_CLUB_ALLIANCE_LIST();
	void	REFRESH_CLUB_BATTLE_LIST();

	void	REFRESH_ITEMBANK ();
	void	REFRESH_VNGAINSYSTEM ();

	void	REFRESH_ATTENDBOOK();

	void	SET_GATHER_GAUGE( DWORD dwGaeaID, float fTime );

public:
	void	SetInventorySlotViewSize ( const int& nSLOT_VIEW_SIZE );
	int GetInventorySlotViewSize () const;

public:
	void ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName );
	void ADD_FRIEND( const CString& strName );
	void FRIEND_LIST( CONST CString & strName, bool bOnline );

	const CString& GetPrivateMarketTitle ();

	void DoBattleModal( const CString strClubName, bool bAlliance = false );
	BOOL ReqClubBattle( DWORD dwTime, bool bAlliance );

private:
	ET_INFOTYPE		m_etInfoType;

public:
	void	RESET_INFO ();

	void	SHOW_ITEM_INFO_SIMPLE ( SITEMCUSTOM &sItemCustom );

	/*npc shop, Juver, 2017/07/26 */
	/*product item, Juver, 2017/10/15 */
	void	SHOW_ITEM_INFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID = SNATIVEID(),
			BOOL bNPCShop = FALSE, WORD wNPCShopType = 0, BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE );	

	void	SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel );
	void	SHOW_COMMON_LINEINFO ( const CString& strText, D3DCOLOR dwColor );
	void	SHOW_COMMON_LINEINFO_SPLIT ( const CString& strText, D3DCOLOR dwColor );

	bool	BEGIN_COMMON_LINEINFO_MULTI ();
	bool	ADD_COMMON_LINEINFO_MULTI ( const CString& strText, D3DCOLOR dwColor );
	void	END_COMMON_LINEINFO_MULTI ();

	void	WARNING_MSG_ON()			{ ShowGroupBottom( WARNING_MSG_WINDOW ); }
	void	WARNING_MSG_OFF()			{ HideGroup( WARNING_MSG_WINDOW ); }

	void	SET_COUNT_MSG( INT nCount );
	void	RESET_INFO_ITEM_LINK ();
	void	CLEAR_INFO_ITEM_LINK ();
	void	SET_INFO_ITEM_LINK ( SITEMCUSTOM sITEM );
	void	SHOW_INFO_ITEM_LINK ( SITEMCUSTOM &sItemCustom );	

public:
	void	CLEAR_TEXT ();	
	void	ADDTEXT_NOSPLIT ( const CString& strText, const D3DCOLOR& dwColor );
	void	ADDTEXT_LONGESTLINE_SPLIT ( const CString& strText, const D3DCOLOR& dwColor );

public:
	void	ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg );	
	void	MsgProcess ( LPVOID msgBuffer );

public:
	const	D3DXVECTOR3&	GetCharDir () const				{ return m_vCharDir; }

public:
	void	SetAniPic(int nPic);
	void	SetOddEven(BOOL bOdd);

public:
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg );
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg, SITEMLINK sLINK ); /*item link, Juver, 2017/07/31 */

	void	UpdateClubBattleTime( float fClubBattleTime );
	void	ClearItemBank();
	void	ClearItemShop (); //itemmall
	void	ClearVNGainSys();
	void	DisableMinimapTarget();

private:
	void	UpdateStatus ();
	void	UpdateShortcutBefore ();
	void	UpdateShortcutAfter ();

private:
	void	UpdateStateQuestAlarm ();
	void	UpdateStateSimpleHP ();
	void	UpdateSimpleMessage ();
public:
	void	ReqToggleRun ();
	bool	ItemShopAuth ();		// 일본 커스텀 브라우저 인증 모듈 //	ItemShopAuth
	void	ItemShopVisible();		// 일본 커스텀 브라우저 로딩 대기 //	ItemShopAuth
	DWORD	wPosX;
	DWORD	wPosY;

	void	VisibleCDMRanking( bool bVisible );
	void	RefreshCDMRanking();

public:
	/*item preview, Juver, 2017/07/27 */
	bool	PreviewItemCheckSimple( SNATIVEID sID );
	bool	PreviewItemCheckWear( SNATIVEID sID );
	void	PreviewItem( SITEMCUSTOM sItem );

	/*pet status, Juver, 2017/07/30 */
	void	ShowPetStatus( bool bVisible );

	/*vehicle status, Juver, 2017/07/30 */
	void	ShowVehicleStatus( bool bVisible );

	/*item link, Juver, 2017/07/31 */
	void	LinkItem( SITEMCUSTOM* pItem );

	/*vehicle booster system, Juver, 2017/08/11 */
	void	VehicleBoosterVisible( bool bVisible );

	/*box contents, Juver, 2017/08/29 */
	void	ShowBoxContents( SNATIVEID sidItem );

	/* Competition UI, Juver, 2017/08/24 */
	void	SetCompetitionButtonAlarm( BOOL bAlarm );
	void	ShowCompetitionWindow();

	/* extra notice, Juver, 2017/08/24 */
	void	ShowExtraNotice( DWORD dwTextColor, const char* szFormat, ... );

	/*pvp tyranny, Juver, 2017/08/25 */
	void	PVPTyrannyTowerCaptureResetPos();
	void	PVPTyrannyShowCaptureNotice( WORD wSchool );
	void	PVPTyrannyShow2CaptureNotice( WORD wSchool );
	void	PVPTyrannyShow2CaptureOwnNotice();
	void	PVPTyrannyShowRanking();
	bool	PVPTyrannyToShowRanking()	{ return m_bPVPTyrannyShowRanking; }
	void	PVPTyrannyShowTopRanking();
	void	PVPTyrannyShowStartNotice();
	void	PVPTyrannyShowEndNotice();

	/*item exchange, Juver, 2017/10/12 */
	void	SetNPCItemExchangeWindowOpen( SNATIVEID sid_npc );

	/*product item, Juver, 2017/10/15 */
	void	OpenProductWindow();

	/*student record ui, Juver, 2017/10/30 */
	void	OpenStudentRecord();

	/*activity system, Juver, 2017/11/05 */
	void	OpenActivityItemWindow();

	/*equipment lock, Juver, 2018/01/16 */
	void	OpenEquipmentLockInput();
	void	OpenEquipmentLockRecover( std::string strName, std::string strEPass );

	/*inventory,equipment,storage lock, DevArt22, 2020/02/11 */
	void	PersonalLockWindowResetPos();

	/*inventory lock, DevArt22, 2020/02/11 */
	void	OpenInventoryLockInput();

	/*inventory lock, DevArt22, 2020/02/11 */
	void	OpenStorageLockInput();

	/*item transfer card, Juver, 2018/01/18 */
	void	OpenItemTransferWindow();
	void	CloseItemTransferWindow();

	/*school wars, Juver, 2018/01/20 */
	void	PVPSchoolWarsScoreResetPos();
	void	PVPSchoolWarsShowTopRanking();
	void	PVPSchoolWarsShowStartNotice();
	void	PVPSchoolWarsShowEndNotice();
	void	PVPSchoolWarsShowRanking();
	bool	PVPSchoolWarsToShowRanking()	{ return m_bPVPSchoolWarsShowRanking; }

	/*pvp capture the flag, Juver, 2018/02/01 */
	void	PVPCaptureTheFlagShowTopRanking();
	void	PVPCaptureTheFlagShowStartNotice();
	void	PVPCaptureTheFlagShowEndNotice();
	void	PVPCaptureTheFlagScoreResetPos();
	void	PVPCaptureTheFlagShowRanking();
	bool	PVPCaptureTheFlagToShowRanking()	{ return m_bPVPCaptureTheFlagShowRanking; }

	//dmk14 | 11-4-16 | pk ranking
	void	RefreshPlayerRanking();

//#ifdef CH_PARAM // 중국 인터페이스 변경
//public:
//	void UpdatePotionTrayPosition();
//#endif

public:	//	ETC Func.
	SCONFT_OPTION*	GetConftOption ()			{ return &m_sReqConflictOption; }
	SPARTY_OPT*		GetPartyOption ()			{ return &m_sReqPartyOption; }

public:
	bool IsCHANNEL() { return m_bCHANNEL; }
	bool IsCHAT_BEGIN();

private:
	BOOL			m_bGateOpen;
	BOOL			m_bCharMoveBlock;
	bool			m_bBlockProgramFound;
	bool			m_bBlockProgramAlarm;
	bool			m_bITEM_INFO_EX_DISPLAY_MODE;
	DWORD			m_dwEventQuestStep;
	DWORD			m_dwEventQuestID;
	EMCONFT_TYPE	m_emConflictReqType;
	DWORD			m_ConflictReqID;
	SCONFT_OPTION	m_sReqConflictOption;
	SPARTY_OPT		m_sReqPartyOption;
	BOOL			m_bSnapItem;
	BOOL			m_bPartyStateBack;
	CString			m_strFriendName;
	D3DXVECTOR3		m_vCharDir;
	DWORD			m_dwSkillUpID;
	SITEMCUSTOM		m_sItemInfoTemp;
	bool			m_bUSING_INFO_DISPLAY;
	bool			m_bACADEMY_FIGHT_READY;
	CString			m_strGeneralInfoBack;
	float			m_fMoveWindowDistBack;
	bool			m_bFirstItemSlot;
	DWORD			m_dwClubMasterID;
	CString			m_strMoveFriendName;
	bool			m_bCHANNEL;
	bool			m_bTabReserve;
	float			m_fVehicleDelay;
	float			m_fItemBankDelay;
	float			m_fItemShopDelay;
	bool			m_bItemShopLoad;	//	ItemShopAuth

	/*item link, Juver, 2017/07/31 */
	bool			m_bITEM_INFO_ITEM_LINK_DISPLAY_MODE;
	SITEMCUSTOM		m_sItemInfoLinkTemp;

	/*pvp tyranny, Juver, 2017/08/25 */
	bool			m_bPVPTyrannyShowRanking;	

	/*school wars, Juver, 2018/01/20 */
	bool			m_bPVPSchoolWarsShowRanking;	

	/*pvp capture the flag, Juver, 2018/02/02 */
	bool			m_bPVPCaptureTheFlagShowRanking;	

public:
	static CInnerInterface& GetInstance();
public:
	void	OpenProcessListDisplay(); /*get process command, Juver, 2017/06/08 */
};
