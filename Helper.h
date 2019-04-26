#ifndef HELPER_H
#define HELPER_H

#include <ctime> // retrieving time functions
#include <string> // string handling
#include <sstream> // string convertions (log files)
#include <fstream>

using namespace std;

namespace Helper {
	// generic types
	template <class T>
	string ToString(const T &);	// function prototypes
	// from ctime
	struct DateTime {
		DateTime() { // constructor for the struct
			time_t ms; // variable
			time(&ms); // system time variable (from ctime library)
			struct tm *info = localtime(&ms); // pointer to tm struct
			D = info->tm_mday;
			m = info->tm_mon + 1;
			y = 1900 + info->tm_year;
			M = info->tm_min;
			H = info->tm_hour;
			S = info->tm_sec;
		}	
		// overriding constructors with initialization
		DateTime(int D, int m, int y, int H, int M, int S) : D(D), m(m), y(y), H(H), M(M), S(S) {}
		// overriding for date
		DateTime(int D, int m, int y) : D(D), y(y), H(0), M(0), S(0) {}
		
		// method for getting current dateTime
		DateTime Now() const {
			return DateTime();
		}
		// formating dates
		int D, m, y, H, M, S;
		string GetDateString() const {
			return string(D < 10 ? "0" : "") + ToString(D) + string(m < 10 ? ".0" : ".") + ToString(m) + "." + ToString(y);
		}
		// formating time
		string GetTimeString(const string &separator = ":") const {
			return string(H < 10 ? "0" : "") + ToString(H) + separator + 
				   string(M < 10 ? "0" : "") + ToString(M) + separator + 
				   string(S < 10 ? separator : "") + ToString(S);
		}
		// putting it all together
		string GetDateTimeString(const string &separator = ":") const {
			return GetDateString() + " " + GetTimeString(separator);
		}
	};

	// generic function
	template <class T>
	// body
	string ToString(const T &e) {
		ostringstream s; 
		s << e; // only admit types that support insertion operators
		return s.str();
	}
	
	// recording strokes for debugging (independent)
	void WriteAppLog(const string &s) {
		ofstream file("log.txt", ios::app);
		file << "[" << Helper::DateTime().GetDateTimeString() << "]" << "\n" << s << endl << "\n";
		file.close();
	}
	
}

#endif
