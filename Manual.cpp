#include "Command.h"

bool Manual::execute() {
	cout << " ===========================================================    MANUAL     ===========================================================================" << endl << endl;


	cout << " alias      Creating alias for file or directory -> alias Vasa|C:\\Users" << endl;
	cout << "            Deleting alias -> alias Vasa|delete" << endl;
	cout << "            Delete all aliases -> alias deleteall" << endl;
	cout << "            To use alias use ? in front of Alias name, example : cd ?AliasName" << endl;
	cout << "            2 Embedded aliases, ?Par - parent directory, ?Curr - current directory" << endl << endl;

	cout << " assign     Generate script of commands -> format : assign Name" << endl;
	cout << "            To execute script you only need to execute Name as a command" << endl << endl;

	cout << " cd         Change Directory - move to a specific folder, you can use ?AliasName" << endl << endl;

	cout << " compare    Compare the two selections on some criteria: name, time or size. Result is new Selection." << endl;
	cout << "            To find intersection use: compare FirstSel^SecondSel criteria ResultSelectionName" << endl;
	cout << "            To find distinction use: compare FirstSel\\SecondSel criteria ResultSelectionName" << endl << endl;

	cout << " copy       Copy files, folders, selections and aliases from one folder to another" << endl;
	cout << "            Use this options with separator > like: copy source > destination" << endl;
	cout << "            If collision of name happened, you must enter mode of solving this problem:" << endl;
	cout << "            o Y - Yes: accept overwriting existing files and continue working" << endl;
	cout << "            o A - All : accept overwriting existing files and do so for all future cases" << endl;
	cout << "            o P - Skip : Do not copy the file and continue working" << endl;
	cout << "            o K - Skip All : do not copy any file that causes the collision, or continue working" << endl;
	cout << "            o B - Abort : terminate the further process of copying" << endl ;
	cout << "            You can regularly use aliases with ? and selections with /(slash) in front of alias/selection name, example : copy /Sel > C:\\" << endl << endl;

	cout << " delete     Delete files and folders with asking if you are sure that you want to execute deletion" << endl;
	cout << "            You can regularly use aliases, named and unnamed selections(delete *.cpp)" << endl << endl;

	cout << " dir        Listing files and folders from directory, that is given by argument ( with information about date and size )" << endl;
	cout << "            Without argument -just list current directory" << endl;
	cout << "            You can regularly use aliases with ? and selections with /(slash) in front of alias/selection name, example : dir ?AliasName, dir /Sel" << endl << endl;

	cout << " exit       Quit, with questions about saving selections, aliases, scripts and crypted administrator password" << endl;
	cout << "            If you put any value as an argument, configuration will be automatically saved." << endl << endl;

	cout << " info       Informations about drives" << endl << endl;

	cout << " makedir    Create a new folder" << endl << endl;

	cout << " manual     Manual" << endl << endl;

	cout << " move       Move files,folders,selections from one folder to another" << endl;
	cout << "            YOu can regularly move folders and files without using its real name(Aliases), example : move ?AliasName > ?Alias2Name" << endl;
	cout << "            Use this options with separator > like: source > destination" << endl;
	cout << "            Can handle regular selections with /(slash) in fornt of them /Sel, and unnamed selection are supported to move *.cpp > C:\\" << endl << endl;

	cout << " password   Change the password for privileged regime" << endl << endl;

	cout << " select     Select one or more files or folders and give them a name" << endl;
	cout << "            and stating files and folders in separate rows, later find selection" << endl;
	cout << "            Use with  /(slash) in front of selection name" << endl;
	cout << "            You select also selections, aliases in selection with ?" << endl;
	cout << "            You can use composite or simple criteria to make selection" << endl;
	cout << "            Simple criteria: name , timestamp and size , you can say size>criteria<criteria or exact value(use =)" << endl;
	cout << "            When you use time you can use only date. If you use date and time then you must put separator | between date and time" << endl;
	cout << "            Use simple criterias and operators: AND, OR, XOR, NOT to make composite criteria" << endl;
	cout << "            Unnamed selections are supported to, use * to make them, example: *.cpp" << endl;
	cout << "            You must use @ in front of composite predicates" << endl;
	cout << "            Example of composite criteria -> @AND(NOT(name=C*),name=*h); @time>18.07.2015|00:00:00:000<19.07.2015|13:00:00:000; @size=2500" << endl << endl;

	cout << " setts      Change date and time of files in selection" << endl;
	cout << "            Use this command like: setts SelectionName date|time" << endl;
	cout << "            Date and Time format: xx.xx.xxxx|xx:xx:xx:xxx; now; just date " << endl;
	cout << "            Can deal with unnamed selections too(setts *.cpp now)" << endl << endl;

	cout << " switch     Change the regime access to the standard and privileged, requiring password" << endl << endl;

	return true;
}
