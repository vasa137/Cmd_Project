#include "Command.h"
#include "CommandParser.h"
#include "Exceptions.h"

using namespace std;

bool Assign::execute()    {
	if ((*privileges_map)[CommandParser::getInstance()->lineGet()] !=  nullptr){
		throw new EXCEPTION("This isn't valid name for command list (reservated name).");
	}

	cout << endl << "List commands (each command in one row, enter for exit ) : " << endl << endl;
	string command_list = "", x = "";

	while (1) {
		getline(cin, x);
		if (x == "") {
			if (command_list == "") {
				throw new EXCEPTION("Error. Empty script!");
			}
			else break;
		}
		else {
			command_list += x + "\n";
		}
	}

	Command::command_map[CommandParser::getInstance()->lineGet()] = command_list; // save name of script with its commands in command_map
	admin_commands[CommandParser::getInstance()->lineGet()] = basic_commands[CommandParser::getInstance()->lineGet()] = MapCommand::getInstance(); // this command can be executed in both regimes, because protecttion for single commands exists
	cout << "Script succesfully created" << endl;
	return true;
}

MapCommand* MapCommand::instance = nullptr; // singleton, all map_commands will point to one MapCommand object on heap

MapCommand* MapCommand::getInstance()    {
	if (instance == nullptr) {
		instance = new MapCommand();
	}
	return instance;
}

bool MapCommand::execute()   {
	stringstream command_list(Command::command_map[CommandParser::getInstance()->wholeLineGet()]); // execution of script, execute each command like single commands
	string cmdline;
	while (getline(command_list, cmdline)) {
		Command* command = CommandParser::getInstance()->getCommand(cmdline);
		if (command != nullptr) command->execute();
	}
	return true;
}