#include <Windows.h>
#include <WinIoCtl.h>
#include <stdio.h>
#include <strsafe.h>

void GetError(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	LPVOID lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


#define BUF_LEN 4096
void main()
{
	HANDLE hVol;
	CHAR Buffer[BUF_LEN];

	USN_JOURNAL_DATA JournalData;
	
	PUSN_RECORD UsnRecord;

	DWORD dwBytes;
	DWORD dwRetBytes;
	int I;

	hVol = CreateFile(TEXT("\\\\.\\c:"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hVol == INVALID_HANDLE_VALUE)
	{
		GetError(TEXT("CreateFile failed"));
		return;
	}

	if (!DeviceIoControl(hVol,
		FSCTL_QUERY_USN_JOURNAL,
		NULL,
		0,
		&JournalData,
		sizeof(JournalData),
		&dwBytes,
		NULL))
	{
		GetError(TEXT("Query journal failed"));
		return;
	}

	printf("Journal ID: %I64x\n", JournalData.UsnJournalID);
	printf("FirstUsn: %I64x\n\n", JournalData.FirstUsn);

	READ_USN_JOURNAL_DATA_V0 ReadData = { 0, 0xFFFFFFFF, FALSE, 0, 0 };
	ReadData.UsnJournalID = JournalData.UsnJournalID;
	//ReadData.StartUsn = *startUSN;
	ReadData.ReasonMask = USN_REASON_FILE_CREATE | USN_REASON_FILE_DELETE | USN_REASON_RENAME_NEW_NAME | USN_REASON_CLOSE;
	ReadData.ReturnOnlyOnClose = 1;
	ReadData.Timeout = (DWORDLONG)-20000000;
	ReadData.BytesToWaitFor = 1024 * 16;
	for (;;)
	{
		memset(Buffer, 0, BUF_LEN);

		if (!DeviceIoControl(hVol,
			FSCTL_READ_USN_JOURNAL,
			&ReadData,
			sizeof(ReadData),
			&Buffer,
			BUF_LEN,
			&dwBytes,
			NULL))
		{
			GetError(TEXT("Read journal failed"));
			return;
		}

		dwRetBytes = dwBytes - sizeof(USN);
		// Find the first record
		UsnRecord = (PUSN_RECORD)(((PUCHAR)Buffer) + sizeof(USN));

		printf("****************************************\n");

		// This loop could go on for a long time, given the current buffer size.
		while (dwRetBytes > 0)
		{
			strfind
			printf("USN: %I64x\n", UsnRecord->Usn);
			printf("File name: %.*S\n",
				UsnRecord->FileNameLength / 2,
				UsnRecord->FileName);
			UsnRecord->TimeStamp;
			printf("Reason: %x\n", UsnRecord->Reason);
			printf("\n");

			dwRetBytes -= UsnRecord->RecordLength;

			// Find the next record
			UsnRecord = (PUSN_RECORD)(((PCHAR)UsnRecord) +
				UsnRecord->RecordLength);
		}
		// Update starting USN for next call
		ReadData.StartUsn = *(USN *)&Buffer;
	}

	CloseHandle(hVol);

}