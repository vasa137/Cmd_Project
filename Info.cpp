#include "Globals.h"
#include "Command.h"
string Info::FindDriveType(int drv){
	switch (drv){
	case DRIVE_UNKNOWN:
		return " of unknown type"; break;
	case DRIVE_NO_ROOT_DIR:
		return " invalid"; break;
	case DRIVE_REMOVABLE:
		return " removable drive"; break;
	case DRIVE_FIXED:
		return " hard disk"; break;
	case DRIVE_REMOTE:
		return " network drive"; break;
	case DRIVE_CDROM:
		return " CD-ROM drive"; break;
	case DRIVE_RAMDISK:
		return " RAM disk"; break;
	default:
		return " unknown type"; break;
	}
}

/*System functions that print information about drives*/
bool Info::execute() {
	TCHAR tchar[105] = {0};
	GetLogicalDriveStrings(sizeof(tchar) / sizeof(TCHAR),tchar);
	cout << "                                     USED                     FREE                   CAPACITY" << endl;
	for (LPTSTR lpDrive = tchar; *lpDrive != 0; lpDrive += 4) {
		char ch = *lpDrive;
		cout << ch << ":/  ";
		int t = GetDriveType(lpDrive);
		ULARGE_INTEGER p1, p2, p3; 	
		string str;
		GetDiskFreeSpaceEx(lpDrive, &p1, &p2, &p3);
		str = FindDriveType(t); 
		printf("%-20s  %20llu B | %20llu B | %20llu B \n", str.c_str(), p1, p3, p2);
	}
	return true;
}