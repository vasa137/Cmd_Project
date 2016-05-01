#ifndef COMMAND_H_
#define COMMAND_H_
#include "Globals.h"

class Selection;
class File;
using namespace std;

class Command {
protected:
	static map<string, string> command_map;
	static map<string, Command*> basic_commands, admin_commands;
	static map<string, Command*>* privileges_map;
public:
	virtual bool execute() = 0;
	friend class CommandParser;
	friend class SystemControl;
	friend class Configurator;
};

class Assign : public Command {
	bool execute() override;
};

class Alias : public Command {
	bool execute() override;
};

class Select : public Command {
	void selectLoop(Selection *s);
	bool execute() override;
};

class MapCommand : public Command {
public:
	static MapCommand* getInstance();
private:
	bool execute() override;
	static MapCommand* instance;
};

class Cd : public Command {
public:
	bool execute() override;
};

class Dir : public Command {
	void writeDir(DIR * dir, string new_dir);
	void dirLoop(Selection *s, bool selection, string new_dir);
public:
	bool execute() override;
};

class Makedir : public Command {
	bool execute() override;
};

class Delete : public Command {
	bool move_flag;
	char dirNotEmpty();
	void delLoop(Selection*, bool, bool);
public:
	Delete(bool flag = false) : move_flag(flag) {}
	bool execute() override;
	bool deletion(string);
};

class Copy : public Command {
private:
	bool move_flag;
	string move_source = "";
	string source = "", dest = "";
	bool DoThisForAll = false, SkipAll = false;
	void copydir_to_Dir(string source, string dest);
	void copyfile_to_Dir(string& source, string& dest, bool);
	void copyLoop(Selection* s, bool selection, string old_dir);
	bool chooseOption(string);
	bool isoverwriteDir(string dest);
	bool isoverwriteFile(string path);
public:
	Copy(bool move = false) : move_flag(move) {}
	bool execute() override;
	string getSource() { return source; }
	string getMoveSource() { return move_source; }
};

class Move : public Command {
	bool execute() override;
};

class Exit : public Command  {
	bool execute() override;
};

class Switch : public Command {
	bool execute() override;

};

class Pass : public Command {
	bool execute()override;
};

class Info : public Command {

	string FindDriveType(int drvtype);
public:
	bool execute() override;
};

class Dummy : public Command {
	bool execute() override {
		cout << "You don't have privileges to execute this command. You must log as admin, type \"switch\" to log as admin. " << endl;
		return true;
	}
	static Dummy* instance;
public:
	static Dummy* getInstance() {
		if (instance == nullptr) {
			instance = new Dummy();
		}
		return instance;
	}
};

class Setts : public Command {
	bool execute() override;
	void setTimeStamp(Selection* s, SYSTEMTIME* st);
	void input_time(SYSTEMTIME* systime, string time);
	void input_date(SYSTEMTIME* systime, string date);
};

class Compare : public Command {
	bool execute() override;
	Selection* intersect(Selection*, Selection*, string& condition, string& resultName);
	Selection* distinct(Selection*, Selection*, string& condition, string& resultName);
};

class Manual : public Command {
	bool execute() override;
};
#endif