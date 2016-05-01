#ifndef CPARSER_H_
#define CPARSER_H_

#include <string>

using namespace std;

class Command;
class Selection;
class CommandParser {
	string whole_line;
	string line;
public:
	static CommandParser* getInstance() {
		if (instance == nullptr) {
			instance = new CommandParser();
		}
		return instance;
	}
	string parse_and_removespace(string& line);
	void get_source_and_dest(string& source, string& dest, char sep);
	bool isAlias(string* arg = nullptr);
	string getAlias(string * arg = nullptr);
	bool isSelection(string * arg = nullptr);
	string getSelectionName(string* arg = nullptr);
	bool isStar(string * arg = nullptr);
	Command* getCommand(string& line);
	void setLine(string& s) { line = s; }
	string lineGet() const { return line; }
	string wholeLineGet() const { return whole_line; }
	bool checkTimeFormat(string& time);
	bool checkDateFormat(string& date);
	bool isPredicate(string& s);
	void findCompareSelections(Selection*&, Selection*&, bool& star1, bool& star2, string& selname1, string& selname2);
	void checkCompareFormatAndGetArgs(string& selname1, string& selname2, string& condition, string& resultName, bool& intersection);
private:
	static CommandParser * instance;
	CommandParser(){}
};

#endif
