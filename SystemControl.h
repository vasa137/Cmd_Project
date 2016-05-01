#ifndef SCONTROL_H_
#define SCONTROL_H_

#include "Globals.h"

/*Service class for system jobs, I didn't want to make singleton*/

class SystemControl {
public:	
	static void start();
	static const string get_start_path(){
		return start_path;
	}
	static void terminate_system();
	static bool changeMode();
	static bool changePassword();
	SystemControl() = delete;
private:
	static const string start_path;
	static bool ADMIN;
	static string Password;
	static void initCommandMaps();
	static void initSelectMaps();
};

#endif
