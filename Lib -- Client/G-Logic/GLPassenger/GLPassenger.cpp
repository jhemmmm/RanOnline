#include "stdafx.h"
#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLPassenger.h"
#include "./GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPassenger::GLPassenger(void)
	: m_dwDriverID( GAEAID_NULL )
	, m_dwPassengerID( GAEAID_NULL )
{
}

GLPassenger::~GLPassenger(void)
{
}

void GLPassenger::Reset ()
{
	m_dwPassengerID = GAEAID_NULL;
	m_dwDriverID = GAEAID_NULL;
}

void  GLPassenger::SetData ( DWORD dwDrvID , DWORD dwPasID )
{
	Reset (); 

	SetDriver ( dwDrvID );
	SetPassenger ( dwPasID );
}

HRESULT GLPassenger::FrameMove ( float fTime, float fElapsedTime )
{
	if ( !Valid() )		return S_FALSE;
		
	return S_OK;
}


