#include "SystemControl.h"
#include "File_Selection.h"
#include "CommandParser.h"
#include "Command.h"
#include "Configurator.h"
#include "Exceptions.h"
#include "PredicateBuilder.h"
using namespace std;

const string SystemControl::start_path = agetPath();

bool SystemControl::ADMIN = false;
string SystemControl::Password;

bool SystemControl::changePassword() {
	cout << "Enter old password : ";
	string pwd;
	cin >> pwd;
	if (pwd == Password) {
		cout << "Enter new password : ";
		cin >> pwd;
		if (pwd == "") throw new EXCEPTION("Bad password! You must enter at least one character!");
		string pwdcopy;
		cout << "Repeat new password : ";
		cin >> pwdcopy;
		if (pwd == pwdcopy) {
			Password = pwdcopy;
			cout << "You changed password!" << endl;
			string s; getline(cin, s);
			return true;
		}
		else throw new EXCEPTION("Failed to change password. You entered two different passwords.");
	}
	else throw new EXCEPTION("Failed to change password. This isn't valid password for admin mode.");
}

/*initialize map of commands for administrator and normal user*/
void SystemControl::initCommandMaps() {
	Command::admin_commands["cd"] = new Cd();
	Command::admin_commands["dir"] = new Dir();
	Command::admin_commands["delete"] = new Delete();
	Command::admin_commands["makedir"] = new Makedir();
	Command::admin_commands["copy"] = new Copy();
	Command::admin_commands["move"] = new Move();
	Command::admin_commands["assign"] = new Assign();
	Command::admin_commands["alias"] = new Alias();
	Command::admin_commands["select"] = new Select();
	Command::admin_commands["exit"] = new Exit();
	Command::admin_commands["switch"] = new Switch();
	Command::admin_commands["password"] = new Pass();
	Command::admin_commands["info"] = new Info();
	Command::admin_commands["setts"] = new Setts();
	Command::admin_commands["compare"] = new Compare();
	Command::admin_commands["manual"] = new Manual();

	/*if normal user can't execute some command its location points to dummy command which execution prints that we don't have privileges to execute it*/
	Command::basic_commands["cd"] = Command::admin_commands["cd"];
	Command::basic_commands["dir"] = Command::admin_commands["dir"];
	Command::basic_commands["assign"] = Command::admin_commands["assign"];
	Command::basic_commands["alias"] = Command::admin_commands["alias"];
	Command::basic_commands["select"] = Command::admin_commands["select"];
	Command::basic_commands["exit"] = Command::admin_commands["exit"];
	Command::basic_commands["switch"] = Command::admin_commands["switch"];
	Command::basic_commands["info"] = Command::admin_commands["info"];
	Command::basic_commands["password"] = Command::admin_commands["password"];
	Command::basic_commands["compare"] = Command::admin_commands["compare"];
	Command::basic_commands["manual"] = Command::admin_commands["manual"];
	Command::basic_commands["delete"] = Dummy::getInstance();
	Command::basic_commands["makedir"] = Dummy::getInstance();
	Command::basic_commands["copy"] = Dummy::getInstance();
	Command::basic_commands["move"] = Dummy::getInstance();
	Command::basic_commands["setts"] = Dummy::getInstance();

	Command::privileges_map = &Command::basic_commands; // this pointer points to admin_commands or basic_commands, it depends on current mode of application
	//all commands are executed with this pointer!!!
}

/*initialize map of composite and simple predicates with address of static allocator of each predicate*/
void SystemControl::initSelectMaps() {
	PredicateBuilder::getInstance()->complex_predicate_map["AND"] = (Predicate* (*) (Predicate*, Predicate*)) &(AND::allocator);
	PredicateBuilder::getInstance()->complex_predicate_map["OR"] = (Predicate* (*) (Predicate*, Predicate*)) &(OR::allocator);
	PredicateBuilder::getInstance()->complex_predicate_map["XOR"] = (Predicate* (*) (Predicate*, Predicate*)) &(XOR::allocator);
	PredicateBuilder::getInstance()->complex_predicate_map["NOT"] = (Predicate* (*) (Predicate*, Predicate*)) &(NOT::allocator);

	PredicateBuilder::getInstance()->basic_predicate_map["name"] = (Predicate* (*) (string&)) &(FileNamePredicate::allocator);
	PredicateBuilder::getInstance()->basic_predicate_map["size"] = (Predicate* (*) (string&)) &(FileSizePredicate::allocator);
	PredicateBuilder::getInstance()->basic_predicate_map["time"] = (Predicate* (*) (string&)) &(FileTimeStampPredicate::allocator);
}

void SystemControl::start() {
	cout << "Do you want to load your aliases? (Y-yes, other-no) : ";
	char yn;
	cin >> yn;
	if (yn == 'Y' || yn == 'y') {
		Configurator::getInstance()->aliasLoader();
	}
	File::changeCurrPar(); // initialize embedded aliases
	cout << "Do you want to load your selections? (Y-yes, other-no) : ";
	cin >> yn;
	if (yn == 'Y' || yn == 'y') {
		Configurator::getInstance()->selectionLoader();
	}
	cout << "Do you want to load your scripts? (Y-yes, other-no) : ";
	cin >> yn;
	if (yn == 'Y' || yn == 'y') {	
		Configurator::getInstance()->scriptLoader();
	}

	initCommandMaps();
	initSelectMaps();

	Password = Configurator::getInstance()->loadPassword();

	cout << "Do you want to log as administrator? (Y-yes, other-NO) " << endl << "Your choice : ";
	cin >> yn;
	if (yn == 'Y' || yn == 'y') {
		changeMode();
	}

	cout << endl;
	string x;
	getline(cin, x);
}

void SystemControl::terminate_system() {


	if (CommandParser::getInstance()->lineGet() != "") {
		Configurator::getInstance()->saveAliases();
		Configurator::getInstance()->saveSelections();
		Configurator::getInstance()->saveScripts();
		cout << "Configuration saved." << endl;
	}
	else {
		cout << endl << "Do you want to save your aliases? (Y-yes, other-no) : ";
		char yn;
		cin >> yn;
		if (yn == 'Y' || yn == 'y') {
			Configurator::getInstance()->saveAliases();
			cout << "Aliases saved." << endl;
		}
		cout << endl << "Do you want to save your selections? (Y-yes, other-no) : ";
		cin >> yn;
		if (yn == 'Y' || yn == 'y') {
			Configurator::getInstance()->saveSelections();
			cout << "Selections saved." << endl;
		}
		cout << endl << "Do you want to save your scripts? (Y-yes, other-no) : ";
		cin >> yn;
		if (yn == 'Y' || yn == 'y') {
			Configurator::getInstance()->saveScripts();
			cout << "Scripts saved." << endl;
		}
	}

	Configurator::getInstance()->savePassword(Password);

	File::removeAliasMap();
	Selection::removeSelectionLists();

	/*delete all allocated memory*/
	delete Command::admin_commands["cd"];
	delete Command::admin_commands["dir"];
	delete Command::admin_commands["delete"];
	delete Command::admin_commands["copy"];
	delete Command::admin_commands["move"];
	delete Command::admin_commands["makedir"];
	delete Command::admin_commands["assign"];
	delete Command::admin_commands["alias"];
	delete Command::admin_commands["select"];
	delete Command::admin_commands["exit"];
	delete Command::admin_commands["switch"];
	delete Command::admin_commands["password"];
	delete Command::admin_commands["info"];
	delete Command::admin_commands["setts"];

	delete MapCommand::getInstance();
	delete Dummy::getInstance();
	exit(0);
}

bool SystemControl::changeMode() {
	if (ADMIN == 1) { /*Ako smo u admin modu direktno prelazimo u user mod*/
		ADMIN = 0; 
		Command::privileges_map = &Command::basic_commands;
		cout << "You changed the mode to normal user." << endl;
		return true;
	}
	else { /*Ako smo u user modu neophodno nam je da pogodimo password kako bi usli u admin mod*/
		cout << "Enter password for admin mode : ";
		string pass;
		cin >> pass;
		if (pass == Password) {
			cout << "Correct password, welcome to admin mode! " << endl << endl;
			Command::privileges_map = &Command::admin_commands;
			ADMIN = 1;
			return true;
		}
		else {
			cout << "Incorrect password! User mode continues. " << endl << endl;
			return false;
		}
	}
}

bool Exit::execute()  {
	SystemControl::terminate_system();
	return true;
}

bool Switch::execute() {
	return SystemControl::changeMode();
}

bool Pass::execute() {
	return SystemControl::changePassword();
}

Dummy* Dummy::instance = nullptr; // singleton dummy command