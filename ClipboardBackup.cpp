// Adapted from https://www.codeproject.com/Articles/11372/Clipboard-backup-Visual-C by sswater shi

// ClipboardBackup.cpp: implementation of the CClipboardBackup class.
//
//////////////////////////////////////////////////////////////////////

#include "ClipboardBackup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClipboardBackup::CClipboardBackup()
{
	Backup();
}

CClipboardBackup::~CClipboardBackup()
{
	ClearBackupDatas();
}

BOOL CClipboardBackup::Backup()
{
	if( ! ::OpenClipboard(NULL) )
		return FALSE;

	ClearBackupDatas();

	UINT format = 0;
	while( (format = ::EnumClipboardFormats(format)) != 0 )
	{
		ClipboardData data;
		data.m_nFormat = format;

		if( format <= 14 )
			data.m_szFormatName[0] = 0;
		else if( GetClipboardFormatName(format, data.m_szFormatName, 100) == 0 )
			data.m_szFormatName[0] = 0;

		HANDLE hMem = ::GetClipboardData( format );
		if( hMem == NULL )
			continue;

		data.m_nLength = ::GlobalSize(hMem);

		LPVOID pMem = ::GlobalLock( hMem );
		data.m_pData = new byte[data.m_nLength];

		memcpy(data.m_pData, pMem, data.m_nLength);

		m_lstData.push_back(data);
	}

	::CloseClipboard();
	return TRUE;
}

BOOL CClipboardBackup::Restore()
{
	if( ! ::OpenClipboard(NULL) )
		return FALSE;

	::EmptyClipboard();

	for (auto it = m_lstData.begin(); it != m_lstData.end(); ++it)
	{
		ClipboardData& data = *it;

		UINT format = data.m_nFormat;

		if( data.m_szFormatName[0] != 0 )
		{
			UINT u = RegisterClipboardFormat( data.m_szFormatName );
			if( u > 0 ) format = u;
		}

		HANDLE hMem = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, data.m_nLength );
		LPVOID pMem = ::GlobalLock( hMem );

		memcpy( pMem, data.m_pData, data.m_nLength );

		::GlobalUnlock( hMem );

		::SetClipboardData( format, hMem );
	}

	::CloseClipboard();
	return TRUE;
}

VOID CClipboardBackup::ClearBackupDatas()
{
	for (auto it = m_lstData.begin(); it != m_lstData.end(); ++it)
	{
		ClipboardData& data = *it;

		delete [] data.m_pData;
	}

	m_lstData.clear();
}
