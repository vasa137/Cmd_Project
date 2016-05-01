#include "TimeStamp.h"
#include "CommandParser.h"
#include "Exceptions.h"
#include <sstream>
#include <ctime>

// get Date and time from this format : xx.xx.xx|xx:xx:xx:xxx
TimeStamp* TimeStamp::DateTimeParse(string& tstamp) {
	stringstream cstream(tstamp);
	string date = "", time = "";
	getline(cstream, date, '|');
	getline(cstream, time);
	if (!CommandParser::getInstance()->checkDateFormat(date)) throw new EXCEPTION("Time format for date isn't correct. Valid format : xx.xx.xxxx");
	if (time != "" && !CommandParser::getInstance()->checkTimeFormat(time)) throw new EXCEPTION("Time format for time isn't correct. Valid format : xx:xx:xx:xxx");
	stringstream dstream(date), tstream(time);
	string day, month, year, hr , min, sec, ms;
	getline(dstream, day, '.');
	getline(dstream, month, '.');
	getline(dstream, year);

	/*podrazumevano vreme*/
	if (time == "") {
		hr = min = sec = ms = "0";
	}
	else {
		getline(tstream, hr, ':');
		getline(tstream, min, ':');
		getline(tstream, sec, ':');
		getline(tstream, ms);
	}
	int Day = atoi(day.c_str());
	int Month = atoi(month.c_str());
	int Year = atoi(year.c_str());
	int Hour = atoi(hr.c_str());
	int Min = atoi(min.c_str());
	int Sec = atoi(sec.c_str());
	int Ms = atoi(ms.c_str());
	return new TimeStamp(Hour, Min, Sec, Ms, Day, Month, Year);
}

//get timestamp of last modification for file from absolute file_path
TimeStamp* TimeStamp::getTimeStamp(string& file_path) {
	struct stat t_stat;
	stat(file_path.c_str(), &t_stat);
	struct tm * timeinfo = localtime(&t_stat.st_mtime);
	return new TimeStamp(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 0 , timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
}