#pragma once

struct NET_MSG_GENERIC;

class GLPassenger
{
protected:
	DWORD		m_dwPassengerID;
	DWORD		m_dwDriverID;
public:
	void Reset ();
	BOOL Valid ()						
	{	
		return ( m_dwDriverID != GAEAID_NULL && m_dwDriverID != GAEAID_NULL );
	}

public:

	void SetData ( DWORD dwDrvID , DWORD dwPasID );

	void SetDriver ( DWORD dwDrvID )	{ m_dwDriverID = dwDrvID; }
	DWORD GetDriver ()					{ return m_dwDriverID; }

	void SetPassenger ( DWORD dwPasID )	{ m_dwPassengerID = dwPasID; }
	DWORD GetPassenger()					{ return m_dwPassengerID; }

public:
	HRESULT FrameMove ( float fTime, float fElapsedTime );

public:
	GLPassenger(void);
	~GLPassenger(void);
};
