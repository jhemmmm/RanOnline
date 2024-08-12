#pragma once

#include "GLPassenger.h"

class GLPassengerClient
{
protected:
	BOOL			m_bPassenger;
	GLPassenger			m_sMyPassenger;
	GLPassenger			m_sTarPassenger;
	DWORD				m_dwTempDriver;

public:
	BOOL Start ( DWORD dwDriverID , DWORD dwPassengerID );
	BOOL Reset ();

	BOOL Valid ();

	void SetTempDriver( DWORD dwID ) { m_dwTempDriver = dwID; }
	DWORD GetTempDriver() { return m_dwTempDriver; }
	void ResetTempDriver() { m_dwTempDriver = GAEAID_NULL; }

public:
	GLPassenger& GetMyPassengerDriver()	{ return m_sMyPassenger; }
	GLPassenger& GetTarDriverPassenger()	{ return m_sTarPassenger; }

protected:
	BOOL MsgPassengerFB ( NET_MSG_GENERIC* nmg );
	BOOL MsgPassengerCancelTar ( NET_MSG_GENERIC* nmg );

public:
	void MsgProcess ( NET_MSG_GENERIC* nmg );
	BOOL IsPassenger() { return m_bPassenger; }

public:
	HRESULT FrameMove ( float fTime, float fElapsedTime );

public:
	static GLPassengerClient& GetInstance();

protected:
	GLPassengerClient(void);

public:
	~GLPassengerClient(void);
};
