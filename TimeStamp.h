#ifndef TSTAMP_H_
#define TSTAMP_H_
#include <string>
using namespace std;
struct TimeStamp {
	int hour, mins, sec, ms;
	int day, month, year;
	TimeStamp(int Hour, int Mins, int Sec, int Ms, int Day, int Month, int Year) : hour(Hour), mins(Mins), sec(Sec), ms(Ms), day(Day), month(Month), year(Year) {}
	friend bool operator==(const TimeStamp& first, const TimeStamp& second) {
		return (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.mins == second.mins && first.sec == second.sec && first.ms == second.ms);
	}
	friend bool operator<(const TimeStamp& first, const TimeStamp& second) {
		long date1 = (long)first.year * 10000 + (long)first.month * 100 + (long)first.day;
		long date2 = (long)second.year * 10000 + (long)second.month * 100 + (long)second.day;
		if (date1 < date2) return true;
		else if (date1 == date2) {
			long time1 = first.hour * 10000000 + first.mins * 100000 + first.sec * 1000 + first.ms;
			long time2 = (long)second.hour * 10000000 + second.mins * 100000 + second.sec * 1000 + second.ms;
			if (time1 < time2) return true;
		}
		return false;
	}
	friend bool operator>(const TimeStamp& first, const TimeStamp& second) {
		long date1 = (long)first.year * 10000 + (long)first.month * 100 + (long)first.day;
		long date2 = (long)second.year * 10000 + (long)second.month * 100 + (long)second.day;
		if (date1 > date2) return true;
		else if (date1 == date2) {
			long time1 = first.hour * 10000000 + first.mins * 100000 + first.sec * 1000 + first.ms;
			long time2 = (long)second.hour * 10000000 + second.mins * 100000 + second.sec * 1000 + second.ms;
			if (time1 > time2) return true;
		}
		return false;
	}
	static TimeStamp* DateTimeParse(string& date_and_time);
	static TimeStamp* getTimeStamp(string& file_path);
};

#endif