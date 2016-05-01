#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_


class Configurator {
public:
	static Configurator* getInstance() {
		if (instance == nullptr) {
			instance = new Configurator();
		}
		return instance;
	}
	void loadSelections(FILE* selectionsFile);
	void saveSelections();
	void loadAliases(FILE* aliasFile);
	void saveAliases();
	void loadScripts(FILE* scriptFile);
	void saveScripts();
	void selectionLoader();
	void aliasLoader();
	void scriptLoader();
	string loadPassword();
	void savePassword(string);
private:
	static Configurator* instance;
	Configurator(){}
};

#endif
