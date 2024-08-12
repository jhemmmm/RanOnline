#pragma once

#include <string>

#include "./GLItem.h"

//#include "../../Lib -- Engine/Common/CList.h"
//#include "../../Lib -- Engine/G-Logic/GLDefine.h"

struct DxSkinPiece;
class DxSimMesh;

class CItemNode
{
public:
	SITEM			m_sItem;

	SRANDOM_GEN*	m_pRANDOM_SET;
	DxSimMesh*		m_pInvenMesh;	//	Inventory Mesh.
	DxSimMesh*		m_pFieldMesh;	//	Field Mesh.

public:
	CItemNode () :
		m_pRANDOM_SET(NULL),
		m_pInvenMesh(NULL),
		m_pFieldMesh(NULL)
	{
	}

	~CItemNode ()
	{
	}
};
typedef CItemNode* PITEMNODE;

class GLItemMan
{
public:
	enum
	{
		ENCODE_VER_OLD	= 0x0100,
		ENCODE_VER		= 0x0200,

		VERSION			= 0x0200,
		MAX_MID			= 1024,
		MAX_SID			= 1024,

		MONEYMESH_NUM	= 3,
		SPECID_NUM		= 999,
		SHUFFLE_NUM		= 5,
		MAX_NUM			= 8 // ?아이템 해킹대처용 렌덤 배열 수
	};

	static const char* _FILEHEAD;
	static const char* _LOGFILE;
	static const char* _STRINGTABLE;

	typedef std::vector<SNATIVEID>		NID_VEC;
	typedef NID_VEC::iterator			NID_VEC_ITER;
	typedef std::vector<WORD>			WRD_VEC;
	typedef std::vector<SRANDOM_DATA>	VEC_RANDOM;

	struct SGENITEMS
	{
		std::vector<SNATIVEID>	m_vecItems;
		WRD_VEC					m_sRendom;
		WORD					m_wNowCur;

		SGENITEMS () :
			m_wNowCur(0)
		{
		}
	};

protected:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	char				m_szFileName[MAX_PATH];

	bool				m_bModify;
	PITEMNODE**			m_ppItem;
	SGENITEMS			m_sGenItems[SPECID_NUM];
	VEC_RANDOM			m_sRandom;

	std::string			m_strMoneyMesh[MONEYMESH_NUM];
	DxSimMesh*			m_pMoneyMesh[MONEYMESH_NUM];

	// ?아이템 해킹 대처용
	SNATIVEID			m_sQITEMID[MAX_NUM];

protected:
	PITEMNODE GetItemNode ( WORD wMID, WORD wSID );

public:
	BOOL ValidTable ()									{ return m_ppItem!=NULL; }

	bool	ValidData ();
	bool	IsModify ()									{ return m_bModify; }

public:
	SITEM* GetItem ( WORD wMID, WORD wSID );
	SITEM* GetItem ( SNATIVEID sNativeID )				{ return GetItem ( sNativeID.wMainID, sNativeID.wSubID ); }
	SRANDOM_GEN* GetItemRandomOpt ( SNATIVEID sNativeID );

public:
	SNATIVEID GetRandomQItemID ( SNATIVEID sNativeID ); // 랜덤 ?아이템 발생 (헤킹 대처!)

public:
	EMITEM_QUESTION GetRandQItem();

public:
	BOOL ItemGenerateSet ();
	SNATIVEID RendomGenSpecID ( DWORD dwSpecID );

	DxSimMesh* GetFieldMesh ( SNATIVEID sNativeID )			{ return GetFieldMesh ( sNativeID.wMainID, sNativeID.wSubID ); }
	DxSimMesh* GetFieldMesh ( WORD wMID, WORD wSID );

	DxSimMesh* GetInvenMesh ( SNATIVEID sNativeID )			{ return GetInvenMesh ( sNativeID.wMainID, sNativeID.wSubID ); }
	DxSimMesh* GetInvenMesh ( WORD wMID, WORD wSID );

	DxSimMesh* GetMoneyMesh ( DWORD i )						{ return m_pMoneyMesh[i]; }

public:
	SRANDOM_GEN* LoadRandomGenData ( const char* szName );

public:
	WORD FindFreeMID ();
	WORD FindFreeSID ( WORD wMainID );
	
public:
	BOOL InsertItem ( SNATIVEID sNativeID, PITEMNODE pItem, bool binner=false )		{ return InsertItem ( sNativeID.wMainID, sNativeID.wSubID, pItem, binner ); }
	BOOL InsertItem ( WORD wMID, WORD wSID, PITEMNODE pItem, bool binner=false );

public:
	BOOL DeleteItem ( WORD wMID, WORD wSID );
	BOOL DeleteItem ( SNATIVEID sNativeID );

public:
	HRESULT OneTimeSceneInit ();
	HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT DeleteDeviceObjects ();
	HRESULT FinalCleanup ();

	HRESULT CleanUp ()
	{
		HRESULT hr;
		hr = DeleteDeviceObjects ();
		if ( FAILED(hr) )	return hr;
		
		hr = FinalCleanup ();
		if ( FAILED(hr) )	return hr;

		return S_OK;
	}

public:
	HRESULT LoadFile ( const char* szFile, BOOL bServer, bool bPastLoad );
	HRESULT SaveFile ( const char* szFile );

public:
	HRESULT	SyncStringTable();
	HRESULT SyncUpdateData ();
	HRESULT SaveCsvFile( CWnd* pWnd );
	HRESULT LoadCsvFile( CWnd* pWnd );

	//for randomoption fixup
	HRESULT SaveRandomOptCsvFile( CWnd* pWnd );
	HRESULT LoadRandomOptCsvFile( CWnd* pWnd );

public:
	GLItemMan ();

public:
	~GLItemMan ();

public:
	static GLItemMan& GetInstance();
};
