#ifndef FILE_SELECTION_H_
#define FILE_SELECTION_H_

#include "Globals.h"
using namespace std;


class File {
	string abspath;
	static map<string, File*> alias_map;
public:
	static map<string, File*>& getAliasMap(){return alias_map; }
	File(string& path){ 
		abspath = path;
	}
	string getFullPath(){
		return abspath;
	}

	string getName();

	bool isDirect();

	static void removeAliasMap() {
		alias_map.clear();
	}

	static bool deleteAlias(string& idx);

	static void changeCurrPar();
	friend class Configurator;

	void setPath(string& new_path) { abspath = new_path; }
};

class Selection {
	string selname;
	static list<Selection*> selection_list;
	list<File*> file_list;
	friend class Configurator;
	
public:

	string getSelName() {
		return selname;
	}
	Selection(string im) : selname(im) {}

	bool addFile(File *f);

	static void addSelection(Selection* selection) {
		selection_list.push_back(selection);
	}

	static bool star(string& entry, string& rel_path_with_star);

	static Selection* selectionFromStar(string& star_path);

	static list<Selection*>& getSelList() { return selection_list; }

	list<File*>& getFileList() { return file_list; }

	void concatenate_with(Selection *s);

	void removeFileList() { file_list.clear(); }

	static void removeSelectionLists();

	static bool removeSelection(string& sel_name);

	~Selection() { removeFileList(); }

};



#endif