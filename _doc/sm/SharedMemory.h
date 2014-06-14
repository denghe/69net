// SharedMemory.h: interface for the CSharedMemory class. 
// 
////////////////////////////////////////////////////////////////////// 
 
#if !defined(AFX_SHAREDMEMORY_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_) 
#define AFX_SHAREDMEMORY_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_ 
 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
 
#include <process.h> 
 
class CSharedMemory 
{ 
private: 
	class CWriteQueue 
	{ 
	// This class is the queue, it contains a pointer to 
	// a data block and a pointer to the next queue item. 
	friend class CSharedMemory; 
	private: 
		CWriteQueue(int nDataSize) 
		{ 
			pData = new BYTE[nDataSize]; 
			pNext = NULL; 
		}; 
		~CWriteQueue() 
		{ 
			delete [] pData; 
		}; 
		void		*pData; 
		CWriteQueue *pNext; 
	}; 
public: 
	enum 
	{ 
		// Return values of the class-functions. 
		MEM_ERROR_UNKNOWN		= -1, 
		MEM_SUCCESS				= 0, 
		MEM_ERROR_CLOSED		= 1, 
		MEM_ERROR_TIMEOUT		= 2, 
		MEM_ERROR_OTHERPARTY	= 3, 
		MEM_ERROR_DATASIZE		= 4 
	}; 
	CSharedMemory() 
	{ 
		m_nOtherInstanceID	= 0; 
		m_nInstanceID		= 0; 
		// Create an event that indicates wether the connection 
		// is open or not. 
		m_hClosed		= CreateEvent(NULL, TRUE, TRUE, NULL); 
		m_hDataWrit[0]	= NULL; 
		m_hDataWrit[1]	= NULL; 
		m_hDataRead[0]	= NULL; 
		m_hDataRead[1]	= NULL; 
		m_hDataInQueue	= NULL; 
		m_hQueueMutex	= NULL; 
	}; 
	virtual ~CSharedMemory() 
	{ 
		Close(); 
		CloseHandle(m_hClosed); 
	}; 
	bool Open(char* sName, int nDataSize, int nTimeOut = INFINITE) 
	{ 
		m_pFirst	= NULL; 
 
		// The connection must be closed before it can be opened. 
		if (WaitForSingleObject(m_hClosed, 0) == WAIT_OBJECT_0) 
		{ 
			// The name may not exceed MAX_PATH, we substract 10 because we 
			// add some strings to the name in some code. 
			if (strlen(sName) != 0 && strlen(sName) < MAX_PATH - 10) 
			{ 
				// The datasize must be larger than 0. 
				if (nDataSize > 0) 
				{ 
					// The following mutexes can indicate 4 things: 
					// - No instance of this shared memory class was created. 
					// - The first instance of this class was created. 
					// - The second instance of this shared memory class was created. 
					// - Both instances were created. 
					char sMutex0	[MAX_PATH]; 
					char sMutex1	[MAX_PATH]; 
					strcpy(sMutex0	, sName); 
					strcpy(sMutex1	, sName); 
					strcat(sMutex0	, "Mutex0"); 
					strcat(sMutex1	, "Mutex1"); 
					m_hSharedMemoryMutex[0] = CreateMutex(NULL, FALSE, sMutex0); 
					m_hSharedMemoryMutex[1] = CreateMutex(NULL, FALSE, sMutex1); 
					if (m_hSharedMemoryMutex[0] && m_hSharedMemoryMutex[1]) 
					{ 
						// Only two instances of this class (with this name) may reside on 
						// one system. These will be referred to as 'm_nInstanceID and m_nOtherInstanceID' 
						HANDLE hWait[2] = {m_hSharedMemoryMutex[0], m_hSharedMemoryMutex[1]}; 
						DWORD dwResult = WaitForMultipleObjects(2, hWait, FALSE, 0); 
						if (dwResult == WAIT_OBJECT_0 || dwResult == (WAIT_OBJECT_0 + 1)) 
						{ 
							if ((m_nInstanceID = dwResult - WAIT_OBJECT_0) == 0) 
								m_nOtherInstanceID = 1; 
							else 
								m_nOtherInstanceID = 0; 
 
							char sName0		[MAX_PATH]; 
							char sName1		[MAX_PATH]; 
							strcpy(sName0	, sName); 
							strcpy(sName1	, sName); 
							strcat(sName0	, "0"); 
							strcat(sName1	, "1"); 
 
							// We will use two shared memory pools to provide duplex 
							// communication. 
							if ((m_hSharedMemory[0]	= CreateFileMapping(	(HANDLE)0xFFFFFFFF, 
																			NULL, 
																			PAGE_READWRITE, 
																			0, 
																			sizeof(int) + nDataSize, 
																			sName0)) != NULL 
																			&&			 
								(m_hSharedMemory[1]	= CreateFileMapping(	(HANDLE)0xFFFFFFFF, 
																			NULL, 
																			PAGE_READWRITE, 
																			0, 
																			sizeof(int) + nDataSize, 
																			sName1)) != NULL) 
							{ 
								bool bFileMappingAlreadyExists = (GetLastError() == ERROR_ALREADY_EXISTS); 
 
								// Now map a pointer to the size tag in the shared memory. 
								m_pSize = (int*)MapViewOfFile(	m_hSharedMemory[0], 
																	FILE_MAP_ALL_ACCESS, 
																	0, 
																	0, 
																	sizeof(int)); 
								if (m_pSize) 
								{ 
									bool bSharedMemorySizeOk = false; 
									if (bFileMappingAlreadyExists) 
									{ 
										// We will check if the size of the memory block is of the 
										// same size as the block that was already allocated by another 
										// instance of the shared memory class. 
										// The size of the memory block is saved in the first integer 
										// at the specified shared memory address. 
										if (*m_pSize == nDataSize) 
											bSharedMemorySizeOk = true; 
									} 
									else 
									{ 
										// The memory was not allocated by another instance so we 
										// have the honors to allocate it. This means also that we should 
										// set the size of the memory that we have allocated in the first 
										// integer of the shared memory space. 
										*m_pSize = nDataSize; 
										bSharedMemorySizeOk = true; 
									} 
									if (bSharedMemorySizeOk) 
									{ 
										m_pSharedMemory[0] = (BYTE*)MapViewOfFile(	m_hSharedMemory[0], 
																						FILE_MAP_ALL_ACCESS, 
																						0, 
																						0, 
																						nDataSize); 
										m_pSharedMemory[1] = (BYTE*)MapViewOfFile(	m_hSharedMemory[1], 
																						FILE_MAP_ALL_ACCESS, 
																						0, 
																						0, 
																						nDataSize); 
										if (m_pSharedMemory[0] && m_pSharedMemory[1]) 
										{ 
											// Move the pointer a little further so that it does not point to 
											// the size tag, but to the address of the data that we want to share. 
											m_pSharedMemory[0] += sizeof(int); 
											m_pSharedMemory[1] += sizeof(int); 
 
											// The following events make sure that data can only 
											// be read when data was written and vise versa. 
											char sDataWrit0		[MAX_PATH]; 
											char sDataWrit1		[MAX_PATH]; 
											char sDataRead0		[MAX_PATH]; 
											char sDataRead1		[MAX_PATH]; 
											strcpy(sDataWrit0	, sName); 
											strcpy(sDataWrit1	, sName); 
											strcpy(sDataRead0	, sName); 
											strcpy(sDataRead1	, sName); 
											strcat(sDataWrit0	, "DataWrit0"); 
											strcat(sDataWrit1	, "DataWrit1"); 
											strcat(sDataRead0	, "DataRead0"); 
											strcat(sDataRead1	, "DataRead1"); 
											m_hDataWrit[0]	= CreateEvent(NULL, FALSE, FALSE,	sDataWrit0); 
											m_hDataWrit[1]	= CreateEvent(NULL, FALSE, FALSE,	sDataWrit1); 
											m_hDataRead[0]	= CreateEvent(NULL, FALSE, TRUE,	sDataRead0); 
											m_hDataRead[1]	= CreateEvent(NULL, FALSE, TRUE,	sDataRead1); 
											if (m_hDataWrit[0] && m_hDataWrit[1] && m_hDataRead[0] && m_hDataRead[1]) 
											{ 
												m_hSecondInstanceAvailable = CreateEvent(NULL, FALSE, FALSE, sName); 
												if (m_hSecondInstanceAvailable) 
												{ 
													if (m_nInstanceID == 0) 
													{ 
														// We are the first instance, wait for the second instance 
														// to come this far, then we can assume that the connection 
														// is fully open. 
														if (WaitForSingleObject(m_hSecondInstanceAvailable, nTimeOut) == WAIT_OBJECT_0) 
														{ 
															CloseHandle(m_hSecondInstanceAvailable); 
															ResetEvent(m_hClosed); 
															m_hQueueMutex	= CreateMutex(NULL, FALSE, NULL); 
															m_hDataInQueue	= CreateEvent(NULL, FALSE, FALSE, NULL); 
															m_hQueueThread	= (HANDLE)_beginthread(QueueThread, 0, this); 
															return true; 
														} 
													} 
													else if (m_nInstanceID == 1) 
													{ 
														// We are the second instance, signal the other instance that 
														// we have come this far. 
														// Immediately wait 0 seconds for the event, if it is still signaled 
														// we know that the other instance was not waiting, the connection 
														// has failed. 
														SetEvent(m_hSecondInstanceAvailable); 
														if (WaitForSingleObject(m_hSecondInstanceAvailable, 0) == WAIT_TIMEOUT) 
														{ 
															CloseHandle(m_hSecondInstanceAvailable); 
															ResetEvent(m_hClosed); 
															m_hQueueMutex	= CreateMutex(NULL, FALSE, NULL); 
															m_hDataInQueue	= CreateEvent(NULL, FALSE, FALSE, NULL); 
															m_hQueueThread	= (HANDLE)_beginthread(QueueThread, 0, this); 
															return true; 
														} 
													} 
													CloseHandle(m_hSecondInstanceAvailable); 
												} 
												else 
												{ 
													// We could not create the required event. 
												} 
											} 
											else 
											{ 
												// We could not create any event handles. 
											} 
											UnmapViewOfFile(m_pSharedMemory[0]); 
											UnmapViewOfFile(m_pSharedMemory[1]); 
										} 
										else 
										{ 
											// We could not get a pointer to the actual data. 
										} 
									} 
									else 
									{ 
										// The datasize of the already allocated memory, and the size of this 
										// instance do not match. 
									} 
									UnmapViewOfFile(m_pSize); 
								} 
								else 
								{ 
									// We could not map to the integer that contains the size of the memory block. 
								} 
								CloseHandle(m_hSharedMemory[0]); 
								CloseHandle(m_hSharedMemory[1]); 
							} 
							else 
							{ 
								// The memory handles could not be created. 
							} 
						} 
						else 
						{ 
							// There was no mutex available, this can mean that there are 
							// already two instances of this object with the same name 
							// in use on this system. 
						} 
						CloseHandle(m_hSharedMemoryMutex[0]); 
						CloseHandle(m_hSharedMemoryMutex[1]); 
					} 
					else 
					{ 
						// The mutexes could not be created. 
					} 
				} 
				else 
				{ 
					// The datasize is not > 0. 
				} 
			} 
			else 
			{ 
				// The name of the shared memory is not valid, or the datasize is not larger than 0. 
			} 
		} 
		else 
		{ 
			// This instance is already open. 
		} 
		return false; 
	}; 
	void Close() 
	{ 
		if (WaitForSingleObject(m_hClosed, 0) == WAIT_TIMEOUT) 
		{ 
			// Indicate that this instance is closed 
			SetEvent(m_hClosed); 
 
			// Release your own mutex. This will auitomatically signal 
			// the other instance of this class that this instance broke 
			// the connection. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nInstanceID]); 
 
			// The writequeue may still contain elements, empty 
			// it. 
			EmptyWriteQueue(); 
 
			WaitForSingleObject(m_hQueueThread, INFINITE); 
			CloseHandle(m_hQueueMutex); 
			CloseHandle(m_hDataInQueue); 
			m_hQueueMutex	= NULL; 
			m_hDataInQueue	= NULL; 
 
			// Cleanup some stuff. 
			CloseHandle(m_hDataWrit[0]); 
			CloseHandle(m_hDataWrit[1]); 
			CloseHandle(m_hDataRead[0]); 
			CloseHandle(m_hDataRead[1]); 
			m_hDataWrit[0] = NULL; 
			m_hDataWrit[1] = NULL; 
			m_hDataRead[0] = NULL; 
			m_hDataRead[1] = NULL; 
 
			UnmapViewOfFile(m_pSize); 
 
			m_pSharedMemory[0] -= sizeof(int); 
			m_pSharedMemory[1] -= sizeof(int); 
			UnmapViewOfFile(m_pSharedMemory[0]); 
			UnmapViewOfFile(m_pSharedMemory[1]); 
			 
			CloseHandle(m_hSharedMemory[0]); 
			CloseHandle(m_hSharedMemory[1]); 
 
			CloseHandle(m_hSharedMemoryMutex[0]); 
			CloseHandle(m_hSharedMemoryMutex[1]); 
 
		} 
	}; 
	int Write(void *pData, int nDataSize, DWORD dwTimeOut) 
	{ 
		// The 'Write' and 'WriteToQueue' functions can be used promiscously. 
 
		// This function writes to the shared memory pool, it can only write 
		// to that pool if existing data in the pool has been read by the 
		// other instance of this class. 
		// If this function returns MEM_ERROR_OTHERPARTY, the calling 
		// process should close the connection and re-open it to create a new 
		// valid connection. 
		HANDLE	hWait[3]; 
		hWait[0]	= m_hClosed; 
		hWait[1]	= m_hSharedMemoryMutex[m_nOtherInstanceID]; 
		hWait[2]	= m_hDataRead[m_nOtherInstanceID]; 
		DWORD	dwWaitResult = WaitForMultipleObjects(3, hWait, FALSE, dwTimeOut); 
		switch(dwWaitResult) 
		{ 
		case WAIT_OBJECT_0 + 2: 
			if (nDataSize > *m_pSize) 
				return MEM_ERROR_DATASIZE; 
			// Data was read from the shared memory pool, write new data 
			// and notify any listener that new data was written. 
			memcpy(m_pSharedMemory[m_nOtherInstanceID], pData, nDataSize); 
			SetEvent(m_hDataWrit[m_nOtherInstanceID]); 
			return MEM_SUCCESS; 
		case WAIT_OBJECT_0: 
			// The close function of this instance was called. 
			return MEM_ERROR_CLOSED; 
		case WAIT_OBJECT_0 + 1: 
			// The other instance closed. 
			// Since we locked the mutex by waiting for it, we have to release 
			// it again. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_ABANDONED_0 + 1: 
			// The other instance left without a trace, this means probably that 
			// it crashed. 
			// Since we locked the mutex by waiting for it, we have to release 
			// it again. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_FAILED: 
			if (!m_hDataRead[m_nOtherInstanceID]) 
				return MEM_ERROR_CLOSED; 
			// I don't know wat happened, you should call 'GetLastError()'. 
			return MEM_ERROR_UNKNOWN; 
		case WAIT_TIMEOUT: 
			// There was a timeout, the other party has not yet read previous data. 
			return MEM_ERROR_TIMEOUT; 
		} 
		return MEM_ERROR_UNKNOWN; 
	} 
	int WriteToQueue(void *pData, int nDataSize) 
	{ 
		// The 'Write' and 'WriteToQueue' functions can be used promiscously. 
 
		// This function is somewhat the same as the previous function, however, 
		// this function is non-blocking. As long as the connection is valid this 
		// function can write new data into a queue. The queue is read by a thread 
		// that calls the previous 'Write' function. 
		HANDLE	hWait[3]; 
		hWait[0]	= m_hClosed; 
		hWait[1]	= m_hSharedMemoryMutex[m_nOtherInstanceID]; 
		hWait[2]	= m_hQueueMutex; 
		switch (WaitForMultipleObjects(3, hWait, FALSE, INFINITE)) 
		{ 
		case WAIT_OBJECT_0: 
			return MEM_ERROR_CLOSED; 
		case WAIT_OBJECT_0 + 2: 
			{ 
				if (nDataSize > *m_pSize) 
					return MEM_ERROR_DATASIZE; 
				CWriteQueue *pNew = new CWriteQueue(*m_pSize); 
				memcpy(pNew->pData, pData, *m_pSize); 
 
				if (!m_pFirst) 
					m_pFirst = pNew; 
				else 
				{ 
					CWriteQueue *pCurrent = m_pFirst; 
					while (pCurrent->pNext) 
						pCurrent = pCurrent->pNext; 
					pCurrent->pNext = pNew; 
				} 
 
				SetEvent(m_hDataInQueue); 
				ReleaseMutex(m_hQueueMutex); 
			} 
			return MEM_SUCCESS; 
		case WAIT_OBJECT_0 + 1: 
			// The other instance closed. 
			// Since we locked the mutex by waiting for it, we have to release 
			// it again. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_ABANDONED_0 + 1: 
			// The other instance left without a trace, this means probably that 
			// it crashed. 
			// Since we locked the mutex by waiting for it, we have to release 
			// it again. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_FAILED: 
			// This can happen when the connection was not opened yet. 
			// It is caused by an invalid or NULL handle. 
			if (!m_hQueueMutex) 
				return MEM_ERROR_CLOSED; 
			// This must never happen. 
			return MEM_ERROR_UNKNOWN; 
		} 
		return MEM_ERROR_UNKNOWN; 
	} 
	int Read(void *pData, int nDataSize, DWORD dwTimeOut) 
	{ 
		// This function reads from the shared memory pool, it can 
		// only read data when data was written to the pool. 
		// It is always a blocking function. 
 
		// It reads data that was written to the shared memory pool by 
		// the 'Write' or 'WriteToQueue' functions. 
		HANDLE	hWait[3]; 
		hWait[0]	= m_hDataWrit[m_nInstanceID]; 
		hWait[1]	= m_hClosed; 
		hWait[2]	= m_hSharedMemoryMutex[m_nOtherInstanceID]; 
		DWORD	dwWaitResult = WaitForMultipleObjects(3, hWait, FALSE, dwTimeOut); 
		switch(dwWaitResult) 
		{ 
		case WAIT_OBJECT_0: 
			// This happens when data is written into the shared memory pool. 
			// It indicates that the data can be copied into a memory 
			// buffer. 
			if (nDataSize > *m_pSize) 
				return MEM_ERROR_DATASIZE; 
			memcpy(pData, m_pSharedMemory[m_nInstanceID], nDataSize); 
			SetEvent(m_hDataRead[m_nInstanceID]); 
			return MEM_SUCCESS; 
		case WAIT_OBJECT_0 + 1: 
			// This happens when no connection was made yet, or this 
			// instance was closed. 
			return MEM_ERROR_CLOSED; 
		case WAIT_OBJECT_0 + 2: 
			// This happens when the other party closes its connection. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_ABANDONED_0 + 2: 
			// This happens when the other party gracefully closes its connection. 
			// This can be caused by an unexpected termination of the host 
			// process of the other instance. 
			ReleaseMutex(m_hSharedMemoryMutex[m_nOtherInstanceID]); 
			return MEM_ERROR_OTHERPARTY; 
		case WAIT_FAILED: 
			// This can happen when the connection was not opened yet. 
			// It is caused by an invalid or NULL handle. 
			if (!m_hDataWrit[m_nInstanceID]) 
				return MEM_ERROR_CLOSED; 
			return MEM_ERROR_UNKNOWN; 
		case WAIT_TIMEOUT: 
			// This indicates that the maximum wait time (dwTimeOut) has passed. 
			return MEM_ERROR_TIMEOUT; 
		} 
		return MEM_ERROR_UNKNOWN; 
	} 
private: 
	void EmptyWriteQueue() 
	{ 
		// This private function avoids memory leaking of items in the send-queue. 
		while (m_pFirst && WaitForSingleObject(m_hQueueMutex, INFINITE) == WAIT_OBJECT_0) 
		{ 
			if (m_pFirst) 
			{ 
				// First get the first element of the queue 
				CWriteQueue *pQueue = m_pFirst; 
				m_pFirst = pQueue->pNext; 
				delete pQueue; 
			} 
			ReleaseMutex(m_hQueueMutex); 
		} 
	} 
	static void QueueThread(void *pArg) 
	{ 
		// This thread writes every packet that enteres the queue to the shared memory pool. 
		// It ensures that the 'WriteToQueue' function is nonblocking. 
		// The queue access is mutexed to avoid simultaneous access to the queue by 
		// this thread and the 'WriteToQueue' function. 
		CSharedMemory	*pThis = (CSharedMemory*)pArg; 
		HANDLE hWait[2] = {pThis->m_hClosed, pThis->m_hDataInQueue}; 
		bool bQuit = false; 
		while (!bQuit) 
		{ 
			switch (WaitForMultipleObjects(2, hWait, FALSE, INFINITE)) 
			{ 
			case WAIT_OBJECT_0 + 1: 
				{ 
					BYTE *pData = NULL; 
					while (pThis->m_pFirst && WaitForSingleObject(pThis->m_hQueueMutex, INFINITE) == WAIT_OBJECT_0) 
					{ 
						if (pThis->m_pFirst) 
						{ 
							// First get the first element of the queue 
							CWriteQueue *pQueue = pThis->m_pFirst; 
							pData = new BYTE[*pThis->m_pSize]; 
							memcpy(pData, pThis->m_pFirst->pData, *pThis->m_pSize); 
							pThis->m_pFirst = pQueue->pNext; 
							delete pQueue; 
							ReleaseMutex(pThis->m_hQueueMutex); 
				 
							pThis->Write(pData, *pThis->m_pSize, INFINITE); 
							delete [] pData; 
						} 
						else 
							ReleaseMutex(pThis->m_hQueueMutex); 
					} 
				} 
				break; 
			case WAIT_OBJECT_0: 
				bQuit = true; 
				break; 
			} 
		} 
	} 
private: 
	// We will use two shared memory pools to create a transparant memory 'pipe'. 
	// One pool will be used as destination for one instance, and source for the other 
	// instance, the other will be used the other way around. 
	// The two mutexes will indicate which instance is already available. 
	HANDLE			m_hSharedMemoryMutex[2]; 
 
	int				m_nInstanceID; 
	int				m_nOtherInstanceID; 
 
	HANDLE			m_hSharedMemory[2]; 
 
	BYTE*			m_pSharedMemory[2]; 
 
	int*			m_pSize; 
	HANDLE			m_hClosed;			// This handle indicates wether this instance 
										// is open or closed. 
	HANDLE			m_hDataWrit[2]; 
	HANDLE			m_hDataRead[2]; 
 
	HANDLE			m_hSecondInstanceAvailable; 
 
	// Queue stuff 
	HANDLE			m_hQueueThread; 
	HANDLE			m_hDataInQueue; 
	CWriteQueue		*m_pFirst; 
	HANDLE			m_hQueueMutex; 
}; 
 
#endif // !defined(AFX_SHAREDMEMORY_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_) 
