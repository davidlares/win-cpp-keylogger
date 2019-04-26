#include <iostream>
#include <windows.h>
#include "Helper.h"
#include "Keys.h"
#include "Base64.h"
#include "IO.h"

using namespace std;

int main(int argc, char** argv) {
	
	// disabling render prompt
	MSG Msg;
	// infinite loop for taking messages
	while(GetMessage (&Msg, NULL, 0, 0)) {
		// takes a message intended for a GUI
		TranslateMessage(&Msg);
		// forward the message to the system
		DispatchMessage(&Msg);
	}
	return 0;
}
