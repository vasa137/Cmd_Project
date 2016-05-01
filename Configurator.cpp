#include "File_Selection.h"
#include "Command.h"
#include "Configurator.h"
#include "SystemControl.h"

Configurator* Configurator::instance = nullptr; // singleton

void Configurator::loadSelections(FILE* selectionsFile) {
	int number_of_selections;
	fread(&number_of_selections, sizeof(int), 1, selectionsFile);

	if (number_of_selections == 0) return;

	SelectionF selectionStruct;
	string abspath;
	char absbuff[MAX_LEN];

	while (number_of_selections) {
		fread(&selectionStruct, sizeof(SelectionF), 1, selectionsFile);
		Selection* s = new Selection(selectionStruct.name);
		while (selectionStruct.num_of_files) {
			fread(&absbuff, sizeof(char)*MAX_LEN, 1, selectionsFile);
			abspath = absbuff;
			s->addFile(new File(abspath));
			selectionStruct.num_of_files--;
		}
		Selection::addSelection(s);
		number_of_selections--;
	}
}

void Configurator::saveSelections() {
	FILE* selectionsFile;
	string start_path = SystemControl::get_start_path() + "\\selections.sc";
	if ((selectionsFile = fopen(start_path.c_str(), "wb")) == NULL) return;
	int number_of_selections = Selection::selection_list.size();
	fwrite(&number_of_selections, sizeof(int), 1, selectionsFile);

	SelectionF selectionStruct;
	string abspath;
	char absbuff[MAX_LEN];

	list<Selection*>::iterator it = Selection::selection_list.begin();
	for (; it != Selection::selection_list.end(); ++it) {
		strcpy(selectionStruct.name, (*it)->getSelName().c_str());
		selectionStruct.num_of_files = (*it)->getFileList().size();
		fwrite(&selectionStruct, sizeof(SelectionF), 1, selectionsFile);

		list<File*>::iterator ft = (*it)->file_list.begin();
		for (; ft != (*it)->file_list.end(); ++ft) {
			abspath = (*ft)->getFullPath();
			strcpy(absbuff, abspath.c_str());
			fwrite(&absbuff, sizeof(char)* MAX_LEN, 1, selectionsFile);
		}
	}

	fclose(selectionsFile);
}

void Configurator::loadAliases(FILE* aliasFile) {
	AliasF aliasStruct;

	while (fread(&aliasStruct, sizeof(AliasF), 1, aliasFile) == 1) {
		File::alias_map[aliasStruct.name] = new File(string(aliasStruct.abspath));
	}

}

void Configurator::saveAliases() {
	FILE* aliasFile;
	string start_path = SystemControl::get_start_path() + "\\aliases.sc";
	if ((aliasFile = fopen(start_path.c_str(), "wb")) == NULL) return;

	AliasF aliasStruct;

	map<string, File*>::iterator it = File::alias_map.begin();



	for (; it != File::alias_map.end(); ++it) {
		strcpy(aliasStruct.name, it->first.c_str());
		strcpy(aliasStruct.abspath, it->second->getFullPath().c_str());

		if (strcmp(aliasStruct.name, "Par") && strcmp(aliasStruct.name, "Curr")) { // ne cuvamo par i cur
			fwrite(&aliasStruct, sizeof(AliasF), 1, aliasFile);
		}
	}

	fclose(aliasFile);
}

void Configurator::loadScripts(FILE* scriptFile) {
	AssignF assignStruct;
	while (fread(&assignStruct, sizeof(AssignF), 1, scriptFile) == 1) {
		Command::command_map[assignStruct.name] =string(assignStruct.script);
		Command::admin_commands[assignStruct.name] = Command::basic_commands[assignStruct.name] = MapCommand::getInstance();
	}
}

void Configurator::saveScripts() {
	FILE* scriptFile;
	string start_path = SystemControl::get_start_path() + "\\scripts.sc";
	if ((scriptFile = fopen(start_path.c_str(), "wb")) == NULL) return;

	AssignF assignStruct;

	map<string, string>::iterator it = Command::command_map.begin();

	for (; it != Command::command_map.end(); ++it) {
		strcpy(assignStruct.name, it->first.c_str());
		strcpy(assignStruct.script, it->second.c_str());
		fwrite(&assignStruct, sizeof(AssignF), 1, scriptFile);
	}

	fclose(scriptFile);
}

/*Load saved selections*/
void Configurator::selectionLoader() {
	FILE* selectionsFile;
	if ((selectionsFile = fopen(".\\selections.sc", "rb")) != NULL) {
		Configurator::getInstance()->loadSelections(selectionsFile);
		cout << "Selections succesfully loaded." << endl;
		fclose(selectionsFile);
	}
}

/*Load aliases*/
void Configurator::aliasLoader() {
	FILE* aliasFile;
	if ((aliasFile = fopen(".\\aliases.sc", "rb")) != NULL) {
		loadAliases(aliasFile);
		cout << "Aliases succesfully loaded." << endl;
		fclose(aliasFile);
	}
}

/*Load all assigns*/
void Configurator::scriptLoader() {
	FILE* scriptFile;
	if ((scriptFile = fopen(".\\scripts.sc", "rb")) != NULL) {
		Configurator::getInstance()->loadScripts(scriptFile);
		cout << "Scripts succesfully loaded." << endl;
		fclose(scriptFile);
	}
}

string Configurator::loadPassword() {
	ifstream pwfile(".\\pass.sc", ios::binary);
	string crypted_password;
	pwfile >> crypted_password;
	pwfile.close();
	if (crypted_password == "") return "admin"; /*default password for this application*/
	string password = crypted_password;
	for (int i = 0; i < crypted_password.length(); i++) {
		password[i] = crypted_password[i] - 'I'; // crypt password like Caeser in old Rome, sub each letter with another
	}
	return password;
}

void Configurator::savePassword(string password) {
	ofstream pwfile(SystemControl::get_start_path() + "\\pass.sc", ios::binary);
	string new_pass = password;
	for (int i = 0; i < password.length(); i++) {
		new_pass[i] = password[i] + 'I'; // read and transform crypted password
 	}
	pwfile << new_pass;
	pwfile.close();
}