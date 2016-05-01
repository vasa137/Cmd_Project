#include "File_Selection.h"
#include "Command.h"
#include "SystemControl.h"
#include "CommandParser.h"
#include "Exceptions.h"
using namespace std;

int main() {
	 SystemControl::start(); // initialize system, selections,aliases, scripts,password, maps, allocate memory, etc.
	 Command* command;
	 string line="";
	 while (1) {
			 cout << agetPath() << ">";
			 getline(cin, line);
		 try {
			 command = CommandParser::getInstance()->getCommand(line); // parse command
			 if (command == nullptr) cout << endl << "Command is not recognized as an internal or external command" << endl << endl;
			 else command->execute();
		 }
		 catch (EXCEPTION* except) { cout << *except << endl;  delete except;  } // print exception message and delete object that points on it
	 }
	 delete command; // prevent from memory leak
	 return 0;
}
