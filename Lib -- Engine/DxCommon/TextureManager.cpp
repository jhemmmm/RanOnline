#include "stdafx.h"
#include <process.h>
//#include <strstream>
#include <algorithm>
#include <set>

#include "DebugSet.h"
#include "NsSMeshSceneGraph.h"

#include "./TextureManager.h"
#include "MemoryTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------------
//
namespace TextureManager
{
	static char g_strTexturePath[MAX_PATH] = "";

	TEXTUREMAP			g_mapLoading;
	TEXTUREMAP			g_mapComplete;
	TEXTUREIDMAP		g_mapTextureID;
	BOOL				g_bTexThread = FALSE;	// 1.텍스쳐 쓰레드가 활성화 인가?  2.삭제를 1번만 하기 위함.

	CRITICAL_SECTION	m_CSLockLoading;
	CRITICAL_SECTION	m_CSLockComplete;
	CRITICAL_SECTION	m_CSLockTextureID;

	CFileFindTree	g_FileTree;

	BOOL			g_bTexTypeDEBUG = FALSE;

	//	CLASS : TextureManager
	TextureContainer::TextureContainer ( const char* szName, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlags, DWORD dwStage ) :
		m_dwWidth(0),
		m_dwHeight(0),
		m_dwBPP(0),
		m_dwStage(0),
		m_dwFlags(D3DFMT_UNKNOWN),
		m_bHasAlpha(FALSE),
		m_bJPG(FALSE),
		m_refCount(0),
		m_pddsTexture(NULL),
		m_pRGBAData(NULL),
		m_emTexType(EMTT_NORMAL)
	{
		m_strName = szName;

		m_dwStage=dwStage; // 파일 자체에 밉맵포함되어 있음
		m_dwFlags=dwFlags;
	}

	TextureContainer::TextureContainer() :
		m_dwNameExtNum(0),
		m_ppNameExt(NULL),
		m_dwWidth(0),
		m_dwHeight(0),
		m_dwBPP(0),
		m_dwStage(0),
		m_dwFlags(D3DFMT_UNKNOWN ),
		m_bHasAlpha(FALSE),
		m_bJPG(FALSE),
		m_refCount(0),
		m_pddsTexture(NULL),
		m_pddsCubeTexture(NULL),
		m_pRGBAData(NULL)
	{

	}

	TextureContainer::~TextureContainer()
	{
		SAFE_RELEASE(m_pddsTexture);
		SAFE_RELEASE(m_pddsCubeTexture);

		NSCHARSG::ReleaseTexture( (DWORD)m_pddsTexture );
		NSCHARSG::ReleaseTexture( (DWORD)m_pddsCubeTexture );

		if ( m_ppNameExt )
		{
			for ( DWORD i=0; i<m_dwNameExtNum; i++ )	SAFE_DELETE_ARRAY(m_ppNameExt[i]);
			SAFE_DELETE_ARRAY(m_ppNameExt);
		}

		DeleteRGBAData ();
	}

	void TextureContainer::DeleteRGBAData ()
	{
 		if ( m_pRGBAData )
		{
			for ( DWORD i=0;i<m_dwStage;i++) 
			{
				delete[] m_pRGBAData[i];
			}
			
			delete[] m_pRGBAData;
			
			m_pRGBAData=NULL;
		}
	}

	HRESULT TextureContainer::LoadTexture ( const char* szName, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlags, DWORD dwStage )
	{
		GASSERT(szName);

		m_strName = szName;

		m_dwStage = dwStage;
		m_dwFlags = dwFlags;

		return LoadImageData( pd3dDevice, FALSE );
	}

	HRESULT TextureContainer::LoadTexture ( LPDIRECT3DDEVICEQ pd3dDevice )
	{
		return LoadImageData( pd3dDevice, FALSE );
	}

	HRESULT TextureContainer::LoadCubeTexture ( const char* szName, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlags, DWORD dwStage )
	{
		GASSERT(szName);

		m_strName = szName;

		m_dwStage = dwStage;
		m_dwFlags = dwFlags;

		return LoadImageData( pd3dDevice, TRUE );
	}

	std::string TextureContainer::GetTextureFilePath( const char* szFileName, BOOL &bEncrypted )
	{
		bEncrypted = FALSE;

		std::string strFileName = szFileName;

		//find encrypted file first
		std::string strFindEnc = strFileName.c_str();
		std::size_t nfound = strFindEnc.rfind('.');
		if ( nfound != std::string::npos )
		{
			strFindEnc.erase( nfound );
			strFindEnc.append(".mtf");
			std::string* pFilePath = TextureManager::g_FileTree.FindPathName ( std::string(strFindEnc.c_str()) );
			if ( pFilePath )
			{
				bEncrypted = TRUE;
				return pFilePath->c_str();
			}
		}

		//encrypted file not found, try to find the original file requested
		std::string* pFilePath = TextureManager::g_FileTree.FindPathName ( std::string(strFileName.c_str()) );
		if ( pFilePath )
		{
			//file found, check if encrypted
			std::string strFileFind = pFilePath->c_str();
			if ( strFileFind.rfind(".mtf") != std::string::npos )	bEncrypted = TRUE;

			return pFilePath->c_str();
		}

		//now try looking for file with different extension
		pFilePath = TextureManager::g_FileTree.FindPathNameNoExtension ( std::string(strFileName.c_str()) );
		if ( pFilePath )
		{
			std::string strFileFind = pFilePath->c_str();
			if ( strFileFind.rfind(".mtf") != std::string::npos )	bEncrypted = TRUE;

			return pFilePath->c_str();
		}

		return "";
	}

	HRESULT TextureContainer::LoadImageData ( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bCube )
	{
		HRESULT hr = S_OK;
		const char*	strExtension = NULL;
		const char*	strPathName = NULL;

		BOOL bEncrypted = FALSE;
		std::string strFileFind = GetTextureFilePath( m_strName.c_str(), bEncrypted );

		if ( strFileFind.size() <= 0 )	
		{
			CDebugSet::ErrorFile( "TextureContainer::LoadImageData", m_strName.c_str() );
			return E_INVALIDARG;
		}

		strPathName = strFileFind.c_str ();

		//static float s_fAddTime = 0.f;
		//static float s_fStartTime = 0.f;
		//static float s_fEndTime = 0.f;
		//s_fStartTime = DXUtil_Timer( TIMER_GETAPPTIME );

		D3DXIMAGE_INFO ImageInfo;
		if( bCube )
		{
			// 암호화 되어 있을경우
			if ( bEncrypted )
			{
				CMemoryTexture cTexture;
				if ( !cTexture.LoadFileDec( strPathName ) ) return E_FAIL;

				BYTE* pData = cTexture.GetData();
				int nSize = cTexture.GetSize();

				
				hr = D3DXCreateCubeTextureFromFileInMemoryEx( pd3dDevice, pData, nSize, D3DX_DEFAULT, m_dwStage, 0,
										(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
										D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsCubeTexture );
				if ( FAILED(hr) )
				{
					hr = D3DXCreateCubeTextureFromFileInMemoryEx ( pd3dDevice, pData, nSize, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
											D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsCubeTexture );
				}
				if ( FAILED(hr) )	return hr;
			}
			else
			{
				hr = D3DXCreateCubeTextureFromFileEx( pd3dDevice, strPathName, D3DX_DEFAULT, m_dwStage, 0,
										(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
										D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsCubeTexture );
				if ( FAILED(hr) )
				{
					hr = D3DXCreateCubeTextureFromFileEx ( pd3dDevice, strPathName, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
											D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsCubeTexture );
				}
				if ( FAILED(hr) )
				{
					CDebugSet::ErrorFile( "TextureContainer::LoadImageData", strPathName );
					return hr;
				}
			}			
		}
		else
		{
			// 암호화 되어 있을경우
			if ( bEncrypted )
			{

				CMemoryTexture cTexture;
				if ( !cTexture.LoadFileDec( strPathName ) ) return E_FAIL;

				BYTE* pData = cTexture.GetData();
				int nSize = cTexture.GetSize();

				hr = D3DXCreateTextureFromFileInMemoryEx ( pd3dDevice, pData, nSize, D3DX_DEFAULT, D3DX_DEFAULT, m_dwStage, 0,
										(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
										D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsTexture );
				if ( FAILED(hr) )
				{
					hr = D3DXCreateTextureFromFileInMemoryEx ( pd3dDevice, pData, nSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
											D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsTexture );
				}
				if ( FAILED(hr) )	return hr;				

			}
			else
			{
				hr = D3DXCreateTextureFromFileEx ( pd3dDevice, strPathName, D3DX_DEFAULT, D3DX_DEFAULT, m_dwStage, 0,
										(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
										D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsTexture );
				if ( FAILED(hr) )
				{
					hr = D3DXCreateTextureFromFileEx ( pd3dDevice, strPathName, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											(D3DFORMAT)m_dwFlags, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
											D3DX_FILTER_BOX|D3DX_FILTER_MIRROR, 0, &ImageInfo, NULL, &m_pddsTexture );
				}
				if ( FAILED(hr) )
				{
					CDebugSet::ErrorFile( "TextureContainer::LoadImageData", strPathName );
					return hr;
				}
			}			
		}

		// 알파 체크
		D3DSURFACE_DESC sDesc;
		sDesc.Format = D3DFMT_UNKNOWN;
		if( m_pddsTexture )				m_pddsTexture->GetLevelDesc( 0, &sDesc );
		else if( m_pddsCubeTexture )	m_pddsCubeTexture->GetLevelDesc( 0, &sDesc );

		std::string::size_type idx;
		switch ( sDesc.Format )
		{
		case D3DFMT_DXT1:
			idx = m_strName.find( "_a." );
			if( idx!=std::string::npos )	
			{
				m_emTexType = EMTT_ALPHA_HARD;
				break;
			}
			idx = m_strName.find( "_a1." );
			if( idx!=std::string::npos )
			{
				m_emTexType = EMTT_ALPHA_HARD;
				break;
			}
			idx = m_strName.find( "_a2." );
			if( idx!=std::string::npos )
			{
				m_emTexType = EMTT_ALPHA_HARD;
				break;
			}

			m_emTexType = EMTT_NORMAL;
			break;

		case D3DFMT_DXT5:
			m_emTexType = EMTT_ALPHA_SOFT;

			idx = m_strName.find( "_a." );
			if( idx!=std::string::npos )	
			{
				m_emTexType = EMTT_ALPHA_SOFT;
				break;
			}
			idx = m_strName.find( "_a1." );
			if( idx!=std::string::npos )
			{
				m_emTexType = EMTT_ALPHA_SOFT01;
				break;
			}
			idx = m_strName.find( "_a2." );
			if( idx!=std::string::npos )
			{
				m_emTexType = EMTT_ALPHA_SOFT02;
				break;
			}
			break;

		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_A8R8G8B8:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
		case D3DFMT_A8R3G3B2:
		case D3DFMT_A8:
			m_emTexType = EMTT_ALPHA_HARD;
			break;

		case D3DFMT_X8R8G8B8:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_X4R4G4B4:
		case D3DFMT_R8G8B8:
		case D3DFMT_R5G6B5:
		case D3DFMT_R3G3B2:
		default:
			m_emTexType = EMTT_NORMAL;
			break;
		};

		if( g_bTexTypeDEBUG )
		{
			switch ( sDesc.Format )
			{
				// 데이터 정리를 위한 것
			case D3DFMT_DXT1:
			case D3DFMT_DXT5:
				break;
			case D3DFMT_DXT2:
				CDebugSet::ToLogFile( "DXT1, DXT5 정해주세요. || Format : DXT2 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_DXT3:
				CDebugSet::ToLogFile( "DXT1, DXT5 정해주세요. || Format : DXT3 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_DXT4:
				CDebugSet::ToLogFile( "DXT1, DXT5 정해주세요. || Format : DXT4 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_A8R8G8B8:
				CDebugSet::ToLogFile( "압축해주세요 || Format : A8R8G8B8 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_A1R5G5B5:
				CDebugSet::ToLogFile( "압축해주세요 || Format : A1R5G5B5 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_A4R4G4B4:
				CDebugSet::ToLogFile( "압축해주세요 || Format : A4R4G4B4 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_A8R3G3B2:
				CDebugSet::ToLogFile( "압축해주세요 || Format : A8R3G3B2 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_A8:
				CDebugSet::ToLogFile( "압축해주세요 || Format : A8 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_X8R8G8B8:
				CDebugSet::ToLogFile( "압축해주세요 || Format : X8R8G8B8 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_X1R5G5B5:
				CDebugSet::ToLogFile( "압축해주세요 || Format : X1R5G5B5 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_X4R4G4B4:
				CDebugSet::ToLogFile( "압축해주세요 || Format : X4R4G4B4 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_R8G8B8:
				CDebugSet::ToLogFile( "압축해주세요 || Format : R8G8B8 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_R5G6B5:
				CDebugSet::ToLogFile( "압축해주세요 || Format : R5G6B5 || 파일이름 : %s", m_strName.c_str() );
				break;
			case D3DFMT_R3G3B2:
				CDebugSet::ToLogFile( "압축해주세요 || Format : R3G3B2 || 파일이름 : %s", m_strName.c_str() );
				break;
			default:
				if( m_strName.size() )
				{
					CDebugSet::ToLogFile( "압축해주세요 || Format : etc... || 파일이름 : %s", m_strName.c_str() );
				}
				break;
			};
		}
		

		#ifdef _DEBUG
		D3DSURFACE_DESC SurfaceDesc;
		if( m_pddsTexture )				m_pddsTexture->GetLevelDesc( 0, &SurfaceDesc );
		else if( m_pddsCubeTexture )	m_pddsCubeTexture->GetLevelDesc( 0, &SurfaceDesc );
		if ( (SurfaceDesc.Height != ImageInfo.Height) || (SurfaceDesc.Width != ImageInfo.Width) )
		{
			//CString StrMsg;
			//StrMsg.Format ( "(주의) 이미지의 크기가 상반됩니다.\r\n원치 않게 이미지가 왜곡 됩니다.\r\n(%d,%d) --> (%d,%d)", ImageInfo.Height, ImageInfo.Width, SurfaceDesc.Height, SurfaceDesc.Width );
			//MessageBox ( NULL, StrMsg.GetString(), pFileNode->GetPathName(), MB_OK );
		} // if ( (SurfaceDesc.Height != ImageInfo.Height) || (SurfaceDesc.Width != ImageInfo.Width) )
		#endif

		
		//// Thread 도 돌기 때문에 하지 말자.
		//s_fEndTime = DXUtil_Timer( TIMER_GETAPPTIME );
		//s_fEndTime = s_fEndTime - s_fStartTime;
		//s_fAddTime += s_fEndTime;
		//CDebugSet::ToListView( "load texture : %f, %f, %s ", s_fEndTime, s_fAddTime, m_strName.c_str() );

		return S_OK;
	}

	HRESULT TextureContainer::LoadSBGFile(const char* strPathname)
	{
		FILE* file=fopen(strPathname,"rb");
		if( !file )        return E_FAIL;

		struct	SBGFILEHEADER
		{
			DWORD dwSize;			//파일 크기
			DWORD dwOffBits;		//실제데이터가 시작되는 부분까지의 거리
			DWORD dwStage;			//밉맵체인의 개수
		}fileHeader;

		fread(&fileHeader,sizeof(SBGFILEHEADER),1,file);
		DWORD dwBufferSize = fileHeader.dwSize-fileHeader.dwOffBits;

		m_dwStage=fileHeader.dwStage;
		
		BITMAPINFO* pbmi=(BITMAPINFO*)new BYTE[fileHeader.dwOffBits-sizeof(SBGFILEHEADER)];
		fread(pbmi,fileHeader.dwOffBits-sizeof(SBGFILEHEADER),1,file);


		m_dwWidth = pbmi->bmiHeader.biWidth;
		m_dwHeight= pbmi->bmiHeader.biHeight;
		m_dwBPP	  = pbmi->bmiHeader.biBitCount;

		DWORD	tempWidth = m_dwWidth;
		DWORD	tempHeight=m_dwHeight;
		
	//
		m_pRGBAData = new DWORD*[m_dwStage];
	//
		if( m_dwBPP==24)
		{
			for ( DWORD i=0;i<m_dwStage;i++)
			{

				//dwBufferSize = pbmi->bmiHeader.biHeight * pbmi->bmiHeader.biWidth * pbmi->bmiHeader.biBitCount/8;
				m_pRGBAData[i]=new DWORD[tempWidth*tempHeight];
				for ( DWORD y=0;y<tempHeight;y++)
				{
					DWORD dwOffset =(tempHeight-1-y)*tempWidth;
					for ( DWORD x=0;x<tempWidth;x++)
					{
						DWORD b=getc(file);
						DWORD g=getc(file);
						DWORD r=getc(file);
						DWORD a=0xff;
						m_pRGBAData[i][dwOffset+x]=(r<<24L)+(g<<16L)+(b<<8L)+(a);
					}
				}
				tempWidth =tempWidth/2;
				tempHeight=tempHeight/2;
			}
			fclose(file);
		}
		else	
		{
			fclose(file);
			return D3DERR_INVALIDCALL;
		}
		return S_OK;
	}

	#ifdef _DEBUG
	HRESULT	TextureContainer::LoadBMPFile(const char* strPathname)
	{
		FILE* file=fopen(strPathname,"rb");
		if( !file )        return E_FAIL;

		BITMAPFILEHEADER	bfHeader;
		fread(&bfHeader,sizeof(BITMAPFILEHEADER),1,file);

		DWORD dwBufferSize = bfHeader.bfSize-bfHeader.bfOffBits;

		m_dwStage=1;
		
		BITMAPINFO* pbmi=(BITMAPINFO*)new BYTE[bfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)];
		fread(pbmi,bfHeader.bfOffBits-sizeof(BITMAPFILEHEADER),1,file);


		m_dwWidth = pbmi->bmiHeader.biWidth;
		m_dwHeight= pbmi->bmiHeader.biHeight;
		m_dwBPP	  = pbmi->bmiHeader.biBitCount;

		DWORD	tempWidth = m_dwWidth;
		DWORD	tempHeight=m_dwHeight;
		
		m_pRGBAData=new DWORD*[m_dwStage];
		if( m_dwBPP==24)
		{
			for ( DWORD i=0;i<m_dwStage;i++)
			{
				//dwBufferSize = pbmi->bmiHeader.biHeight * pbmi->bmiHeader.biWidth * pbmi->bmiHeader.biBitCount/8;
				m_pRGBAData[i]=new DWORD[tempWidth*tempHeight];
				for ( DWORD y=0;y<tempHeight;y++)
				{
					DWORD dwOffset = (tempHeight-y-1)*tempWidth;
					for ( DWORD x=0;x<tempWidth;x++)
					{
						DWORD b=getc(file);
						DWORD g=getc(file);
						DWORD r=getc(file);
						DWORD a=0xff;
						m_pRGBAData[i][dwOffset+x]=(r<<24L)+(g<<16L)+(b<<8L)+(a);
					}
				}
				tempWidth =tempWidth/2;
				tempHeight=tempHeight/2;
			}
			fclose(file);
		}
		else	
		{
			fclose(file);
			return D3DERR_INVALIDCALL;
		}
		return S_OK;
	}
	#endif

	HRESULT TextureContainer::LoadTargaFile(const char* strPathname )
	{
		FILE* file = fopen( strPathname, "rb" );
		if( NULL == file )
			return E_FAIL;

		struct TargaHeader
		{
			DWORD MipmapStage;
			BYTE ImageType;
			WORD XOrigin;
			WORD YOrigin;
			WORD ImageWidth;
			WORD ImageHeight;
			BYTE PixelDepth;
			BYTE ImageDescriptor;
		} tga;

		fread( &tga, sizeof(TargaHeader), 1, file );

		m_dwStage	= tga.MipmapStage;
		m_dwWidth   = tga.ImageWidth;
		m_dwHeight  = tga.ImageHeight;
		m_dwBPP     = tga.PixelDepth;

		DWORD tempWidth=m_dwWidth;
		DWORD tempHeight=m_dwHeight;
		
		m_pRGBAData = new DWORD* [m_dwStage];

		for ( DWORD i=0;i<m_dwStage;i++)
		{
			m_pRGBAData[i] = new DWORD[tempWidth*tempHeight];

			if( m_pRGBAData[i] == NULL )
			{
				fclose(file);
				return E_FAIL;
			}
			for( DWORD y=0; y<tempHeight; y++ )
			{
				DWORD dwOffset = y*tempWidth;				//Origin in lower left-hand corner

				if( 0 == ( tga.ImageDescriptor & 0x0010 ) )	//Origin in upper left-hand corner
					dwOffset = (tempHeight-y-1)*tempWidth;

				for( DWORD x=0; x<tempWidth; x )
				{
					//이미지 타입이 10인 경우
					if( tga.ImageType == 10 )
					{
						BYTE PacketInfo = getc( file );
						WORD PacketType = 0x80 & PacketInfo;
						WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

						if( PacketType )				//for the run length packet
						{
							DWORD b = getc( file );
							DWORD g = getc( file );
							DWORD r = getc( file );
							DWORD a = 0xff;
							if( m_dwBPP == 32 )
								a = getc( file );

							while( PixelCount-- )		//반복된 칼라 정보
							{
								m_pRGBAData[i][dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
								x++;
							}
						}
						else							//for the raw packet
						{
							while( PixelCount-- )		
							{
								BYTE b = getc( file );
								BYTE g = getc( file );
								BYTE r = getc( file );
								BYTE a = 0xff;
								if( m_dwBPP == 32 )
									a = getc( file );

								m_pRGBAData[i][dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
								x++;
							}
						}
					}
					//이미지 타입이 2인 경우
					else
					{
						BYTE b = getc( file );
						BYTE g = getc( file );
						BYTE r = getc( file );
						BYTE a = 0xff;
						if( m_dwBPP == 32 )
							a = getc( file );

						m_pRGBAData[i][dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
						x++;
					}
				}
			}
			tempWidth =tempWidth/2;
			tempHeight=tempHeight/2;
		}
		fclose( file );

		// Check for alpha content
		for( DWORD i=0; i<(m_dwWidth*m_dwHeight); i++ )
		{
			if( (m_pRGBAData[0][i]&0x000000ff) != 0xff )
			{
				m_bHasAlpha = TRUE;
				break;
			}
		}
		return S_OK;
	}

	HRESULT TextureContainer::Restore ( LPDIRECT3DDEVICEQ pd3dDevice )
	{
		SAFE_RELEASE(m_pddsTexture);

		HRESULT	hr;
		//Check params
		if(NULL==pd3dDevice)	return	DDERR_INVALIDPARAMS;

		D3DCAPSQ	ddDesc;
		if( FAILED(pd3dDevice->GetDeviceCaps(&ddDesc)))	return E_FAIL;

		//Setup the new surface desc
		D3DSURFACE_DESC ddsd;
		//surface 초기화
		SecureZeroMemory(&ddsd,sizeof(D3DSURFACE_DESC));

		ddsd.Type=D3DRTYPE_TEXTURE;						//Texture 와 Surface차이점은 먼가??
		ddsd.Usage=0;									//D3DUSAGE_RENDERTARGET;
		ddsd.Pool=D3DPOOL_MANAGED;

		ddsd.Width=m_dwWidth;
		ddsd.Height=m_dwHeight;

		//텍스처 배수 체크
		if( ddDesc.TextureCaps & D3DPTEXTURECAPS_POW2 )	
		{
			for(ddsd.Width =1;m_dwWidth >ddsd.Width ;ddsd.Width<<=1);
			for(ddsd.Height=1;m_dwHeight>ddsd.Height;ddsd.Height<<=1);
		}
		//텍스처 최대 사이즈 체크
		DWORD	dwMaxWidth=ddDesc.MaxTextureWidth;
		DWORD	dwMaxHeight=ddDesc.MaxTextureHeight;
		ddsd.Width =min(ddsd.Width , (dwMaxWidth  ? dwMaxWidth  : 256));
		ddsd.Height=min(ddsd.Height, (dwMaxHeight ? dwMaxHeight : 256));


		//정방형 텍스처 체크
		if ( ddDesc.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY)
		{
			if( ddsd.Width>ddsd.Height)	ddsd.Height= ddsd.Width;
			else						ddsd.Width = ddsd.Height;
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////  	NOTE : m_dwFlags 만들고자 하는 텍스처가 디바이스에서 지원하는지 체크 코드 추가 필요
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//디바이스에서 지원하는 텍스처 형식 결정
		//LPDIRECT3DQ	pD3D8;
		//pd3dDevice->GetDirect3D(&pD3D8);


		//원하는 형태의 텍스처를 만듬	

	#ifdef _DEBUG	
		D3DDISPLAYMODE Mode;
		pd3dDevice->GetDisplayMode( 0, &Mode );
		m_dwFlags=Mode.Format;
	#endif
		
		if ( m_bJPG)
		{
			std::string strTempName;
			strTempName = g_strTexturePath;
			strTempName += m_strName;

			return D3DXCreateTextureFromFileEx(pd3dDevice,strTempName.c_str(),D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,
												(D3DFORMAT)m_dwFlags,D3DPOOL_MANAGED,D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
												D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,0,NULL,NULL,&m_pddsTexture);
		}


		hr=NULL;
		hr = pd3dDevice->CreateTexture(ddsd.Width,ddsd.Height,m_dwStage/*밉맵개수*/,
									0/*D3DUSAGE_RENDERTARGETddsd.Usage*/,
									(D3DFORMAT)m_dwFlags/*D3DFMT_X8R8G8B8*/,ddsd.Pool,&m_pddsTexture, NULL );

		m_pddsTexture->GetLevelDesc(0,&ddsd);
		if ( hr==D3DERR_INVALIDCALL )
		{
			return hr;
		}
		else if ( hr==D3DERR_OUTOFVIDEOMEMORY)
		{
			return hr;
		}
		else if ( hr==E_OUTOFMEMORY)
		{
			return hr;
		}
		if(FAILED(hr))	return hr;
		//surface에 픽셀 정보를 넣음
		if(m_pRGBAData)	
		{
			hr=CopyRGBADataToSurface();
			return hr;
		}
		return S_OK;
	}

	HRESULT TextureContainer::CopyRGBADataToSurface()
	{
		//Get Device to create a tempory surface
		LPDIRECT3DDEVICEQ	pd3dDevice;
		m_pddsTexture->GetDevice(&pd3dDevice);

		//Setup the new surface desc
		D3DSURFACE_DESC	ddsd;
		m_pddsTexture->GetLevelDesc(0,&ddsd);
		
		DWORD dwRMask,dwGMask,dwBMask,dwAMask;
		DWORD dwRShiftL=8,dwRShiftR=0;
		DWORD dwGShiftL=8,dwGShiftR=0;
		DWORD dwBShiftL=8,dwBShiftR=0;
		DWORD dwAShiftL=8,dwAShiftR=0;

		DWORD dwMask;
		DWORD dwBPP;
		if ( ddsd.Format==D3DFMT_A8R8G8B8)
		{
			dwRMask=0x00ff0000;
			dwGMask=0x0000ff00;
			dwBMask=0x000000ff;
			dwAMask=0xff000000;
			dwBPP=32;
		}
		else if ( ddsd.Format==D3DFMT_X8R8G8B8)
		{
			dwRMask=0x00ff0000;
			dwGMask=0x0000ff00;
			dwBMask=0x000000ff;
			dwAMask=0xff000000;
			dwBPP=32;
		}
		else if ( ddsd.Format==D3DFMT_R5G6B5)
		{
			dwRMask=0x0000f800;
			dwGMask=0x000007e0;
			dwBMask=0x0000001f;
			dwAMask=0x00000000;
			dwBPP=16;
		}
		else if ( ddsd.Format==D3DFMT_A4R4G4B4)
		{
			dwRMask=0x0000f000;
			dwGMask=0x000000f0;
			dwBMask=0x0000000f;
			dwAMask=0x0000f000;
			dwBPP=16;
		}

		for (dwMask=dwRMask ; dwMask && !(dwMask&0x01) ; dwMask>>=1)	dwRShiftR++;
		for ( ; dwMask ; dwMask>>=1)									dwRShiftL--;

		for (dwMask=dwGMask ; dwMask && !(dwMask&0x01) ; dwMask>>=1)	dwGShiftR++;
		for ( ; dwMask ; dwMask>>=1)									dwGShiftL--;

		for (dwMask=dwBMask ; dwMask && !(dwMask&0x01) ; dwMask>>=1)	dwBShiftR++;
		for ( ; dwMask ; dwMask>>=1)									dwBShiftL--;

		for (dwMask=dwAMask ; dwMask && !(dwMask&0x01) ; dwMask>>=1)	dwAShiftR++;
		for ( ; dwMask ; dwMask>>=1)									dwAShiftL--;

		D3DLOCKED_RECT	d3dr;
		DWORD	tempWidth=m_dwWidth;
		DWORD	tempHeight=m_dwHeight;
		

		RECT	rt;

		for ( DWORD i=0;i<m_dwStage;i++)
		{
			SetRect(&rt, 0, 0, tempWidth, tempHeight);

			m_pddsTexture->LockRect(i/*mipmap level*/,&d3dr/*locked region*/,&rt/*0rect to lock*/,0 );
			DWORD*	pDstData32=(DWORD*)d3dr.pBits;
			WORD*	pDstData16=(WORD*)d3dr.pBits;
			
			for ( DWORD y=0;y<tempHeight;y++)
			{
				for( DWORD x=0;x<tempWidth;x++)
				{
					DWORD dwPixel=m_pRGBAData[i][y*tempWidth+x];

					BYTE r=(BYTE)((dwPixel>>24)&0x000000ff);
					BYTE g=(BYTE)((dwPixel>>16)&0x000000ff);
					BYTE b=(BYTE)((dwPixel>> 8)&0x000000ff);
					BYTE a=(BYTE)((dwPixel>> 0)&0x000000ff);

					DWORD dr=((r>>dwRShiftL)<<dwRShiftR)&dwRMask;
					DWORD dg=((g>>dwGShiftL)<<dwGShiftR)&dwGMask;
					DWORD db=((b>>dwBShiftL)<<dwBShiftR)&dwBMask;
					DWORD da=((a>>dwAShiftL)<<dwAShiftR)&dwAMask;
				
					if(dwBPP==32)
					{
						pDstData32[x]=(DWORD)(dr+dg+db+da);
					}
					else
					{
						pDstData16[x]=(WORD)(dr+dg+db+da);
					}
				}
				
				if(dwBPP==32)
					pDstData32 = pDstData32+d3dr.Pitch/(dwBPP/8);//(DWORD*)d3dr.pBits+d3dr.Pitch/(dwBPP/8);
				else
					pDstData16 = pDstData16+d3dr.Pitch/(dwBPP/8);//(WORD*)d3dr.pBits+d3dr.Pitch/(dwBPP/8);

				//d3dr.pBits=(BYTE*)d3dr.pBits+d3dr.Pitch/(dwBPP/8);
			}
			m_pddsTexture->UnlockRect(i);

			tempWidth=tempWidth/2;
			tempHeight=tempHeight/2;
		}

		return S_OK;
	}

	void SetTexturePath ( const char* strTexturePath )
	{
		if ( NULL==strTexturePath )		strTexturePath = "";

		StringCchCopy( g_strTexturePath, MAX_PATH, strTexturePath );

		g_FileTree.CreateTree ( std::string(g_strTexturePath) );
	}

	char* GetTexturePath ()
	{
		return g_strTexturePath;
	}

	void ReScanTexture ()
	{
		g_FileTree.CreateTree ( std::string(g_strTexturePath) );
	}

	BOOL IsTexture( const char* pName )
	{
		if( !pName )	return FALSE;

		size_t nStrLen = strlen(pName)+1;
		char* strTextureNameLwr = new char[ nStrLen ];
		StringCchCopy( strTextureNameLwr, nStrLen, pName );
		_strlwr ( strTextureNameLwr );

		std::string* pFilePath = g_FileTree.FindPathName( std::string(strTextureNameLwr) );
		SAFE_DELETE_ARRAY( strTextureNameLwr );

		if( pFilePath )		return TRUE;
		else				return FALSE;
	}

	// Note : g_mapLoading에서 Search 후 있다면 RefCount++ 한다.
	BOOL IsLoading_AddRef( const char* strName )
	{
		BOOL bUse = FALSE;
		EnterCriticalSection(&m_CSLockLoading);
		{
			TEXTUREMAP_ITER iter = g_mapLoading.find( strName );
			if ( iter!=g_mapLoading.end() )	
			{
				bUse = TRUE;
				(*iter).second->m_refCount++;
			}
		}
		LeaveCriticalSection(&m_CSLockLoading);

		return bUse;
	}

	// Note : g_mapComplete에서 Search 후 있다면 RefCount++와 Texture를 돌려준다.
	LPDIRECT3DTEXTUREQ GetCompleteTex_AddRef( const char* strName )
	{
		LPDIRECT3DTEXTUREQ pTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			TEXTUREMAP_ITER iter = g_mapComplete.find( strName );
			if ( iter!=g_mapComplete.end() )
			{
				pTexture = (*iter).second->m_pddsTexture;
				(*iter).second->m_refCount++;
			}
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return pTexture;
	}

	// Note : g_mapComplete에서 Search 후 있다면 RefCount++와 Texture를 돌려준다.
	LPDIRECT3DCUBETEXTUREQ	GetCompleteTex_AddRefCUBE( const char* strName )
	{
		LPDIRECT3DCUBETEXTUREQ pTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			TEXTUREMAP_ITER iter = g_mapComplete.find( strName );
			if ( iter!=g_mapComplete.end() )
			{
				pTexture = (*iter).second->m_pddsCubeTexture;
				(*iter).second->m_refCount++;
			}
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return pTexture;
	}

	// Note : g_mapLoading에서 Search 후 있다면 pTextureContainer를
	//			g_mapComplete로 옮기고 RefCount++한 후 Texture를 돌려준다.
	LPDIRECT3DTEXTUREQ	GetLoading2CompleteTex_AddRef( const char* strName, TextureContainer* pTextureContainer )
	{
		LPDIRECT3DTEXTUREQ pTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		EnterCriticalSection(&m_CSLockLoading);
		{
			TEXTUREMAP_ITER iter = g_mapLoading.find( strName );
			if ( iter!=g_mapLoading.end() )	
			{
				(*iter).second->m_refCount++;
				pTextureContainer->m_refCount = (*iter).second->m_refCount;

				pTexture = pTextureContainer->m_pddsTexture;

				g_mapComplete[strName] = pTextureContainer;	// 삽입

				SAFE_DELETE( (*iter).second );	// 삭제
				g_mapLoading.erase( iter );		// 삭제
			}
		}
		LeaveCriticalSection(&m_CSLockLoading);
		LeaveCriticalSection(&m_CSLockComplete);

		return pTexture;
	}

	// Note : g_mapComplete에서 Search 후 있다면 Texture를 돌려준다.
	LPDIRECT3DTEXTUREQ	GetCompleteMapTEX( const char* strName )
	{
		LPDIRECT3DTEXTUREQ pTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			TEXTUREMAP_ITER iter = g_mapComplete.find( strName );
			if ( iter!=g_mapComplete.end() )
			{
				pTexture = (*iter).second->m_pddsTexture;
			}
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return pTexture;
	}

	// Note : g_mapLoading에 추가한다.
	void AddLoadingMap( TextureContainer* pTexture, const char* strName )
	{
		EnterCriticalSection(&m_CSLockLoading);
		{
			pTexture->m_refCount++;
			g_mapLoading[strName] = pTexture;
		}
		LeaveCriticalSection(&m_CSLockLoading);
	}

	// Note : g_mapComplete에 추가한다.
	LPDIRECT3DTEXTUREQ AddCompleteMap( TextureContainer* pTexture, const char* strName )
	{
		LPDIRECT3DTEXTUREQ pddsTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			pTexture->m_refCount++;
			pddsTexture = pTexture->m_pddsTexture;
			g_mapComplete[strName] = pTexture;
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return pddsTexture;
	}

	// Note : g_mapComplete에 추가한다.
	LPDIRECT3DCUBETEXTUREQ AddCompleteMapCUBE( TextureContainer* pTexture, const char* strName )
	{
		LPDIRECT3DCUBETEXTUREQ pddsTexture = NULL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			pTexture->m_refCount++;
			pddsTexture = pTexture->m_pddsCubeTexture;
			g_mapComplete[strName] = pTexture;
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return pddsTexture;
	}

	// Note : g_mapTextureID에 추가한다.
	void AddTextureID( const char* strName )
	{
		EnterCriticalSection(&TextureManager::m_CSLockTextureID);
		{
			TextureManager::TEXTUREIDMAP_ITER iterID = TextureManager::g_mapTextureID.find( strName );
			if( iterID == TextureManager::g_mapTextureID.end() )	// 같은 값이 없다면 작업한다.
			{
				DWORD dwSize = (DWORD)TextureManager::g_mapTextureID.size();
				TextureManager::g_mapTextureID[ strName ] = dwSize+1;	// 숫자는 1부터 시작한다.
			}
		}
		LeaveCriticalSection(&TextureManager::m_CSLockTextureID);
	}

	// Note : g_mapLoading에서 Search후 있다면 RefCount-- 한다.
	BOOL SubLoadingMap( const char* strName )
	{
		BOOL bUSE = FALSE;
		EnterCriticalSection(&m_CSLockLoading);
		{
			TEXTUREMAP_ITER iter = g_mapLoading.find ( strName );
			if( iter!=g_mapLoading.end() )
			{
				bUSE = TRUE;
				iter->second->m_refCount--;
				if ( iter->second->m_refCount <= 0 )
				{
					CDebugSet::ToListView( "release texture (Loading) : <%s>", strName );

					delete iter->second;
					g_mapLoading.erase ( iter );
				}
			}
		}
		LeaveCriticalSection(&m_CSLockLoading);

		return bUSE;
	}

	// Note : g_mapComplete에서 Search후 있다면 RefCount-- 한다.
	BOOL SubCompleteMap( const char* strName )
	{
		BOOL bUSE = FALSE;
		EnterCriticalSection(&m_CSLockComplete);
		{
			TEXTUREMAP_ITER iter = g_mapComplete.find ( strName );
			if( iter!=g_mapComplete.end() )
			{
				bUSE = TRUE;
				iter->second->m_refCount--;
				if ( iter->second->m_refCount <= 0 )
				{
					CDebugSet::ToListView( "release texture (Complete) : <%s>", strName );

					delete iter->second;
					g_mapComplete.erase ( iter );
				}
			}
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return bUSE;
	}

	// Note : 함수 GetCompleteMapTEX()에서 Search후 있다면 Texture를 넘긴다.
	void GetTexture( const char* strName, LPDIRECT3DTEXTUREQ &pddsTexture )
	{
		if( !g_bTexThread )		return;
		if ( !strName )			return;
		if ( strName[0]=='\0' )	return;

		size_t nStrLen = strlen(strName)+1;
		char* strTextureNameLwr = new char[nStrLen];
		StringCchCopy( strTextureNameLwr, nStrLen, strName );
		_strlwr ( strTextureNameLwr );

		EnterCriticalSection(&m_CSLockComplete);
		{
			// CompleteMap 확인 후 있다면 Tex를 넘긴다.
			pddsTexture = GetCompleteMapTEX( strTextureNameLwr );
		}
		LeaveCriticalSection(&m_CSLockComplete);

		SAFE_DELETE_ARRAY(strTextureNameLwr);
	}

	// Note : 만약 로딩이 되었다면 당연히 ID도 있어야 한다.
	DWORD GetTextureID( const char* strName )
	{
		TEXTUREIDMAP_ITER iter= g_mapTextureID.find( strName );
		if( iter != g_mapTextureID.end() )
		{
			return (*iter).second;
		}
		return 0;
	}

	HRESULT LoadTexture( const char* strTextureName,
							LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DTEXTUREQ &pddsTexture,
							DWORD dwFlags, DWORD dwStage, BOOL bThread )
	{
		HRESULT hr=S_OK;

		// 에디터에서는 Thread를 사용하지 않는다.
		if( g_bTexTypeDEBUG )	bThread = FALSE;
	//	bThread = FALSE;

		// Init Need
		pddsTexture = NULL;

		if( !g_bTexThread )		return S_OK;

		if ( !strTextureName )			return E_INVALIDARG;
		if ( strTextureName[0]=='\0' )	return E_INVALIDARG;

		size_t nStrLen = strlen(strTextureName)+1;
		char* strTextureNameLwr = new char[nStrLen];
		StringCchCopy( strTextureNameLwr, nStrLen, strTextureName );
		_strlwr ( strTextureNameLwr );
		
		// Note : CompleteMap 확인후 있다면 RefCount++ 한다.
		if( pddsTexture = GetCompleteTex_AddRef(strTextureNameLwr) )	goto _RETURN;

		// Note : 쓰레드 옵션일 경우 나중에 로딩하도록 한다.
		if( bThread )
		{
			// Note : LoadingMap 확인후 있다면 RefCount++ 한다.
			if( IsLoading_AddRef(strTextureNameLwr))	goto _RETURN;

			// Note : 텍스쳐 콘테이너 생성.
			TextureContainer* pTexture = new TextureContainer;
			if( !pTexture )
			{
				hr = E_OUTOFMEMORY;
				goto _RETURN;
			}
			pTexture->m_dwFlags = dwFlags;
			pTexture->m_dwStage = dwStage;			

			// LoadingMap 에 추가한다.
			AddLoadingMap( pTexture, strTextureNameLwr );
			goto _RETURN;
		}
		else
		{
			// Note : 텍스쳐 콘테이너 생성.
			TextureContainer* pTexture = new TextureContainer;
			if( !pTexture )
			{
				hr = E_OUTOFMEMORY;
				goto _RETURN;
			}

			hr = pTexture->LoadTexture( strTextureNameLwr, pd3dDevice, dwFlags, dwStage );
			if ( FAILED(hr) )
			{
				SAFE_DELETE(pTexture);
				goto _RETURN;
			}
			pTexture->m_dwFlags = dwFlags;
			pTexture->m_dwStage = dwStage;

			// Note : g_mapLoading에서 Search 후 있다면 pTextureContainer를
			//			g_mapComplete로 옮기고 RefCount++한 후 Texture를 돌려준다.
			if( pddsTexture = GetLoading2CompleteTex_AddRef( strTextureNameLwr, pTexture ) )	goto _RETURN;

			// Note : CompleteMap 확인후 있다면 RefCount++ 한다.
			if( pddsTexture = GetCompleteTex_AddRef(strTextureNameLwr) )
			{
				SAFE_DELETE( pTexture );
				goto _RETURN;
			}

			// Note : CompleteMap 에 추가한 후 Texture 데이터를 얻는다.
			pddsTexture = AddCompleteMap( pTexture, strTextureNameLwr );

			// Note : Texture ID 작업.
			AddTextureID( strTextureNameLwr );

			goto _RETURN;
		}

_RETURN:
		SAFE_DELETE_ARRAY(strTextureNameLwr);

		if ( FAILED(hr) )
		{
			CDebugSet::ToLogFile ( "ERROR : TextureManager::LoadTexture() %s, %s", strTextureName, DXGetErrorString9(hr) );
		}

		return hr;
	}

	HRESULT LoadCubeTexture ( const char* strTextureName,
							LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DCUBETEXTUREQ &pddsCubeTexture,
							DWORD dwFlags, DWORD dwStage )
	{
		HRESULT hr=S_OK;

		//// 에디터에서는 Thread를 사용하지 않는다.
		//if( g_bTexTypeDEBUG )	bThread = FALSE;

		// Init Need
		pddsCubeTexture = NULL;

		if( !g_bTexThread )		return S_OK;

		if ( !strTextureName )			return E_INVALIDARG;
		if ( strTextureName[0]=='\0' )	return E_INVALIDARG;

		size_t nStrLen = strlen(strTextureName)+1;
		char* strTextureNameLwr = new char[nStrLen];
		StringCchCopy( strTextureNameLwr, nStrLen, strTextureName );
		_strlwr ( strTextureNameLwr );

		// Note : CompleteMap 확인후 있다면 RefCount++ 한다.
		if( pddsCubeTexture = GetCompleteTex_AddRefCUBE(strTextureNameLwr) )	goto _RETURN;

		//	Note : 텍스쳐 콘테이너 생성.
		//
		TextureContainer* pTexture = new TextureContainer;
		if( !pTexture )
		{
			hr = E_OUTOFMEMORY;
			goto _RETURN;
		}
		
		hr = pTexture->LoadCubeTexture( strTextureNameLwr, pd3dDevice, dwFlags, dwStage );
		if ( FAILED(hr) )
		{
			SAFE_DELETE(pTexture);
			goto _RETURN;
		}

		// CompleteMap 에 추가한 후 Texture 데이터를 얻는다.
		pddsCubeTexture = AddCompleteMapCUBE( pTexture, strTextureNameLwr );

		// Note : Texture ID 작업.
		AddTextureID( strTextureNameLwr );

_RETURN:
		SAFE_DELETE_ARRAY(strTextureNameLwr);

		if ( FAILED(hr) )
		{
			D3DCAPSQ pCaps;
			pd3dDevice->GetDeviceCaps( &pCaps );
			if( pCaps.CubeTextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR )
			{
				CDebugSet::ToLogFile ( "ERROR : TextureManager::LoadTexture() %s, %s", strTextureName, DXGetErrorString9(hr) );
			}
		}

		return hr;
	}

	HRESULT ReleaseTexture( const char* strName, LPDIRECT3DTEXTUREQ &pddsTexture )
	{
		// 없다면 삭제 안한다.
		if( !pddsTexture )	return S_OK;

		// 무조건 초기화는 해줘야 한다. 초기화 안 해 줄꺼 같아서 해 놓음.
		pddsTexture = NULL;

		if( !g_bTexThread )		return S_OK;

		if( !strName || (strName[0]=='\0') )			
		{
			pddsTexture = NULL;
			return E_INVALIDARG;
		}

		size_t nStrLen = strlen(strName)+1;
		char* _szNameLwr = new char[nStrLen];
		StringCchCopy( _szNameLwr, nStrLen, strName );
		_strlwr ( _szNameLwr );

		// Note : 로딩 할 것에서 먼저 찾는다.
		if( SubLoadingMap( _szNameLwr ) )		goto _RETURN;

		// Note : 완료 된 것에서 찾는다.
		if( SubCompleteMap( _szNameLwr ) )		goto _RETURN;

_RETURN:
		SAFE_DELETE_ARRAY( _szNameLwr );
		return S_OK;
	}

	HRESULT ReleaseTexture( const char* strName, LPDIRECT3DCUBETEXTUREQ &pddsTexture )
	{
		// 없다면 삭제 안한다.
		if( !pddsTexture )	return S_OK;

		// 무조건 초기화는 해줘야 한다. 초기화 안 해 줄꺼 같아서 해 놓음.
		pddsTexture = NULL;

		if( !g_bTexThread )		return S_OK;

		if( !strName || (strName[0]=='\0') )			
		{
			pddsTexture = NULL;
			return E_INVALIDARG;
		}

		size_t nStrLen = strlen(strName)+1;
		char* _szNameLwr = new char[nStrLen];
		StringCchCopy( _szNameLwr, nStrLen, strName );
		_strlwr ( _szNameLwr );

		// Note : 완료 된 것에서 찾는다.
		if( SubCompleteMap( _szNameLwr ) )		goto _RETURN;

_RETURN:
		SAFE_DELETE_ARRAY( _szNameLwr );
		return S_OK;
	}

	HRESULT DeleteTexture ( char* strName )		// 로딩 할 것에 있는 것도 해야 한다.. 현재 쓰이는 곳은 없다.
	{
		//TEXTUREMAP_ITER iter = g_mapComplete.find( strName );
		//if ( iter==g_mapComplete.end() )	return E_FAIL;

		////CDebugSet::ToListView ( "delete texture : %s", iter->second->m_strName.c_str() );

		//delete iter->second;
		//g_mapComplete.erase ( iter );

		//iter = g_mapLoading.find( strName );
		//if( iter!=g_mapLoading.end() )
		//{
		//	(*iter).second->m_refCount = 0;
		//}

		//return S_FALSE;
		return S_OK;
	}


	LOADING_THREAD	m_sLoadingThread;
	void OneTimeSceneInit()
	{
		InitializeCriticalSection(&m_CSLockLoading);
		InitializeCriticalSection(&m_CSLockComplete);
		InitializeCriticalSection(&m_CSLockTextureID);
		
		g_bTexThread = TRUE;
	}

	void StartThread( LPDIRECT3DDEVICEQ pd3dDevice )
	{
		m_sLoadingThread.StartThread( pd3dDevice );
	}

	void DeleteAllTexture()
	{
		m_sLoadingThread.EndThread();

		std::for_each ( g_mapLoading.begin(), g_mapLoading.end(), std_afunc::DeleteMapObject() );
		std::for_each ( g_mapComplete.begin(), g_mapComplete.end(), std_afunc::DeleteMapObject() );
		g_mapLoading.clear ();
		g_mapComplete.clear ();
	}

	void CleanUp()
	{
		if( g_bTexThread )
		{
			g_bTexThread = FALSE;
			DeleteCriticalSection(&m_CSLockLoading);
			DeleteCriticalSection(&m_CSLockComplete);
			DeleteCriticalSection(&m_CSLockTextureID);
		}
	}

	EM_TEXTYPE	GetCompleteTex_TexType( const char* strName )
	{
		EM_TEXTYPE emTexType = EMTT_NORMAL;
		EnterCriticalSection(&m_CSLockComplete);
		{
			TEXTUREMAP_ITER iter = g_mapComplete.find( strName );
			if ( iter!=g_mapComplete.end() )
			{
				emTexType = (*iter).second->m_emTexType;
			}
		}
		LeaveCriticalSection(&m_CSLockComplete);

		return emTexType;
	}

	EM_TEXTYPE	GetTexType ( const char* strName )
	{
		if ( !strName )			return EMTT_NORMAL;
		if ( strName[0]=='\0' )	return EMTT_NORMAL;

		size_t nStrLen = strlen(strName)+1;
		char* strTextureNameLwr = new char[nStrLen];
		StringCchCopy( strTextureNameLwr, nStrLen, strName );
		_strlwr ( strTextureNameLwr );

		// TexType을 얻어온다.
		EM_TEXTYPE emTexType = GetCompleteTex_TexType( strTextureNameLwr );
		SAFE_DELETE_ARRAY( strTextureNameLwr );

		return emTexType;
	}

	void EnableTexTypeDEBUG()	
	{
		g_bTexTypeDEBUG=TRUE; 
	}

	HRESULT ReportUsingTexture ()
	{
		//DWORD dwAllSum = 0;
		//CDebugSet::ClearFile ( "textureman.txt" );

		//TEXTUREMAP_ITER iter = g_mapTexture.begin();
		//TEXTUREMAP_ITER iter_end = g_mapTexture.end();
		//for ( ; iter!=iter_end; ++iter )
		//{
		//	TextureContainer *pContainer = (*iter).second;
		//	DWORD dwLod = pContainer->m_pddsTexture->GetLOD();
	
		//	std::strstream strStream;
		//	strStream << "[" << pContainer->m_strName << "]";
		//	strStream << "  LOD:" << dwLod;
		//	
		//	DWORD dwSum = 0;
		//	if ( dwLod>0 )
		//	{
		//		for ( DWORD i=0; i<dwLod; ++i )
		//		{
		//			D3DSURFACE_DESC sDesc;
		//			pContainer->m_pddsTexture->GetLevelDesc ( i, &sDesc );
		//			
		//			dwSum += sDesc.Size;
		//			strStream << "  " << sDesc.Size;
		//		}
		//	}
		//	else
		//	{
		//		D3DSURFACE_DESC sDesc;
		//		pContainer->m_pddsTexture->GetLevelDesc ( 0, &sDesc );

		//		dwSum += sDesc.Size;
		//	}

		//	dwAllSum += dwSum;
		//	strStream << "  sum byte = " << dwSum << std::ends;
		//	CDebugSet::ToFile ( "textureman.txt", strStream.str() );
		//	strStream.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		//}

		//CDebugSet::ToFile ( "textureman.txt", "All Sum Byte : %d", dwAllSum );

		return S_OK;
	}
};

// -----------------------------------------------------------------------------------------------------------------------------------------
//															LOADING_THREAD
// -----------------------------------------------------------------------------------------------------------------------------------------
LOADING_THREAD::~LOADING_THREAD()
{	
}

BOOL LOADING_THREAD::StartThread( LPDIRECT3DDEVICEQ pd3dDevice )
{
	// 쓰레드가 켜져 있다면 또 생성하지 않음
	if( m_sData.bEnable )	return TRUE;

	// 초기화
	m_sData.pd3dDevice = pd3dDevice;
	m_sData.bEnable = TRUE;
	m_sData.bDelete = FALSE;

	// 생성
	//hThread = CreateThread ( NULL, 0, LoadThread, &m_sData, 0, &dwThreadID );
	hThread = (HANDLE) ::_beginthreadex(
							NULL,
							0,
							LoadThread,
							&m_sData,
							0,
							(unsigned int*) &dwThreadID );
	if ( !hThread ) return FALSE;

	return TRUE;
}

void LOADING_THREAD::EndThread()
{
	// 쓰레드가 꺼져 있다면 또 지우지 않음.
	if( !m_sData.bEnable )	return;

	// 쓰레드 끄는것을 예약
	m_sData.bEnable = FALSE;

	// Note : Data 로딩이 Thread 에서 이루어짐으로 Thread 가 종료 후에 삭제를 한다.
	while(1)
	{
		Sleep( 1 );

		if ( m_sData.bDelete )	break;
	}

	CloseHandle( hThread );
	hThread = NULL;

	return;
}

unsigned int WINAPI LOADING_THREAD::LoadThread( LPVOID pData )
{
	HRESULT hr=S_OK;
	DATA* _pData = (DATA*)pData;

	while (1)
	{
		Sleep( 1 );

		// Note : 텍스쳐 이름을 얻는다.
		DWORD dwFlags = 0L;
		DWORD dwStage = 0L;
		std::string strName ="";
		EnterCriticalSection(&TextureManager::m_CSLockLoading);
		{
			TextureManager::TEXTUREMAP_ITER iter = TextureManager::g_mapLoading.begin();
			if( iter!=TextureManager::g_mapLoading.end() )
			{
				strName = (*iter).first.c_str();
				dwFlags = (*iter).second->m_dwFlags;
				dwStage = (*iter).second->m_dwStage;
			}
		}
		LeaveCriticalSection(&TextureManager::m_CSLockLoading);

		// Note : Loading Map에 없다.
		if( !strName.size() )	goto _RETURN;

		// Note : 텍스쳐 이름으로만 로딩 작업을 한다.
		TextureManager::TextureContainer* pTexture = new TextureManager::TextureContainer;

		hr = pTexture->LoadTexture( strName.c_str(), _pData->pd3dDevice, dwFlags, dwStage );

		if ( FAILED(hr) )	// Note : 로딩이 안 되었을 시 작업.
		{
			EnterCriticalSection(&TextureManager::m_CSLockLoading);
			{
				TextureManager::TEXTUREMAP_ITER iter = TextureManager::g_mapLoading.find( strName.c_str() );
				if( iter!=TextureManager::g_mapLoading.end() )
				{
					SAFE_DELETE( (*iter).second );
					TextureManager::g_mapLoading.erase( iter );		
				}
				SAFE_DELETE( pTexture );
			}
			LeaveCriticalSection(&TextureManager::m_CSLockLoading);
		}
		else				// Note : 로딩완료 했을 경우.
		{
			// Note : Texture ID 작업.
			TextureManager::AddTextureID( strName.c_str() );

			// Note : 실제 로딩 작업.
			EnterCriticalSection(&TextureManager::m_CSLockComplete);
			EnterCriticalSection(&TextureManager::m_CSLockLoading);
			{
				TextureManager::TEXTUREMAP_ITER iter = TextureManager::g_mapLoading.find( strName.c_str() );
				if( iter!=TextureManager::g_mapLoading.end() )		// Note : 데이터 존재함.
				{
					pTexture->m_refCount = (*iter).second->m_refCount;			// RefCount를 동일하게 맞춰준다.
					TextureManager::g_mapComplete[ (*iter).first ] = pTexture;	// 완성 리스트에 넣는다.

					SAFE_DELETE( (*iter).second );								// 빈 껍데기 지워준다.
					TextureManager::g_mapLoading.erase( iter );					// 로딩 리스트에서 지워준다.
				}
				else												// Note : 텍스쳐 로딩 중에 Release가 되어서 필요가 없게 되었다.
				{
					SAFE_DELETE( pTexture );
				}
			}
			LeaveCriticalSection(&TextureManager::m_CSLockLoading);
			LeaveCriticalSection(&TextureManager::m_CSLockComplete);
		}

_RETURN:
		if ( !_pData->bEnable )
		{
			_pData->bDelete = TRUE;
			return 0;
		}		
	}

	return 0;
}

