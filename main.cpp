#include "injection.h"

const char szDllFile[] = "./aspasia.dll";
const char szProc[] = "cs2.exe";

int main (void) {
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof (PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		std::cout << "Error Creating ToolHelp32Snapshot \n";
		system ("PAUSE");
		return 0;
	}

	DWORD PID = 0;

	if (Process32First (hSnap, &PE32)) {
		do {
			if (!strcmp (PE32.szExeFile, szProc)) {
				PID = PE32.th32ProcessID;
				break;
			}
		} while (Process32Next (hSnap, &PE32));
	}

	CloseHandle (hSnap);

	HANDLE hProc = OpenProcess (PROCESS_ALL_ACCESS, FALSE, PID);

	if (!hProc) {
		std::cout << "Error in OpenProcess with pid " << PID << std::endl;
		system ("PAUSE");
		return 0;
	}


	if (!ManualMap (hProc, szDllFile)) {
		CloseHandle (hProc);
		std::cout << "Error on Manual Mapping Function \n";
		system ("PAUSE");
		return 0;
	}

	CloseHandle (hProc);
	return 0;
}