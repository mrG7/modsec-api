#ifndef __MY_MODULE_H__
#define __MY_MODULE_H__

//  The module implementation.
//  This class is responsible for implementing the 
//  module functionality for each of the server events
//  that it registers for.
class CMyHttpModule : public CHttpModule
{
public:
	HANDLE	m_hEventLog;
	BYTE *	m_pIoBuffer;
	HANDLE	m_hIoEvent;
	DWORD	m_dwPageSize;

	//  Implementation of the AcquireRequestState event handler method.
    REQUEST_NOTIFICATION_STATUS
    OnAcquireRequestState(
        IN IHttpContext *                       pHttpContext,
        IN OUT IHttpEventProvider *             pProvider
    );

	REQUEST_NOTIFICATION_STATUS
	OnBeginRequest(
		IN IHttpContext * pHttpContext,
		IN IHttpEventProvider * pProvider
	);

	REQUEST_NOTIFICATION_STATUS
	OnSendResponse(
		IN IHttpContext * pHttpContext,
	    IN ISendResponseProvider * pProvider
	);

	REQUEST_NOTIFICATION_STATUS
	OnPostEndRequest(
		IN IHttpContext * pHttpContext,
		IN IHttpEventProvider * pProvider
	);

	HRESULT ReadFileChunk(HTTP_DATA_CHUNK *chunk, char *buf);
	
	CMyHttpModule();
	~CMyHttpModule();

	void Dispose();

	BOOL WriteEventViewerLog(LPCSTR szNotification, WORD category = EVENTLOG_INFORMATION_TYPE);
};

#endif
