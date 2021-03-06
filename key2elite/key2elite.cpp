/*
key2elite v1.0

by @imifos
http://github.com/imifos

Locates the Elite Dangerous client and sends a set of key strokes. It's give no feedback, except in case of error
in which case it pops a message box. 

Usage:
    1) "key2elite.exe [Single hardware key scan code as decimal number]
	   key2elite.exe 33

    2) "key2elite.exe [Multiple hardware key scan code as decimal number separated by dash] 
	   key2elite.exe 33-33-44-55.

Scan code table: 
https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
http://www.philipstorr.id.au/pcbook/book3/scancode.htm
 
 MIT License
*/

#include "stdafx.h"
#include "key2elite.h"

HWND elite;
const wchar_t *eliteWindowName = L"Elite - Dangerous (CLIENT)";
wchar_t title[8000];
int keycode = 0;
const wchar_t sep[] = L"-";

#define NONE  0
#define LCTRL 1
#define RCTRL 2
#define LSHIFT 3
#define RSHIFT 4
#define LALT 5
#define RALT 6

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM param)
{
	::GetWindowText(hwnd, title, sizeof(title));

	if (wcsstr(title, eliteWindowName) != NULL) {
		elite = hwnd;
	}
	return TRUE;
}


void sendKey(int keycode,int modifier) {

	LPINPUT input; 
	int index = 0;
	DWORD addFlags[2];

	if (modifier == NONE) {
		input = new INPUT[1];
	}
	else {
		input = new INPUT[2];
		
		input[index].type = INPUT_KEYBOARD;
		input[index].ki.wScan = 0;
		input[index].ki.time = 100;
		input[index].ki.dwExtraInfo = 0;
		input[index].ki.wVk = 0;

		addFlags[index] = 0;

		switch (modifier) {
			case LSHIFT: 
				input[index].ki.wScan = 0x2A; //42
				break;
			case RSHIFT: 
				input[index].ki.wScan = 0x36; // 54
				break;
			case LCTRL:
				input[index].ki.wScan = 0x1d; 
				break;
			case RCTRL:
				input[index].ki.wScan = 0x1d; 
				addFlags[index] = KEYEVENTF_EXTENDEDKEY;
				break;
			case LALT:
				input[index].ki.wScan = 0x38;
				break;
			case RALT:
				input[index].ki.wScan = 0x38; 
				addFlags[index] = KEYEVENTF_EXTENDEDKEY;
				break;
		}
		index++;
	}

	// main key
	input[index].type = INPUT_KEYBOARD;
	input[index].ki.wScan = keycode; // hardware scan code for key
	input[index].ki.time = 200;
	input[index].ki.dwExtraInfo = 0;
	input[index].ki.wVk = 0;
	addFlags[index] = 0;

	index++;

	for (int i=0;i<index;i++)
		input[i].ki.dwFlags = KEYEVENTF_SCANCODE | !KEYEVENTF_KEYUP | addFlags[i]; // press
	
	SendInput(index, input, sizeof(INPUT));

	for (int i = 0; i<index; i++)
   	    input[i].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP | addFlags[i]; // release

	SendInput(index, input, sizeof(INPUT));

	delete input;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//wchar_t lpCmdLine[100] = L"33C"; //L"33S";

	if (lpCmdLine == NULL || wcslen(lpCmdLine) == 0) {
		MessageBox(NULL, (LPCWSTR)L"key2elite v1.0, by @imifos, github.com/imifos\n\nLocates the Elite Dangerous client and sends a set of key strokes."
			"Command: Single hardware key scan code as decimal number or multiple codes like this: '33-44-55'. "
			"Modifiers can be applied as following: 33s-44C-55, with s being c=LCTRL, C=RCTRL, s=LSHIFT, S=RSHIFT, a=LALT, A=RALT."
			"Example: key2elite.exe 33-44a-55", (LPCWSTR)L"key2elite", MB_ICONINFORMATION);
		return -5;
	}

	// Find Elite Dangerous client window
	elite = NULL;
	EnumWindows(EnumWindowsProc, NULL);
	if (elite == NULL) {
		MessageBox(NULL, (LPCWSTR)L"EliteClient not found! Is the game running?", (LPCWSTR)L"key2elite", MB_ICONWARNING);
		return -2;
	}

	// Bring it to foreground, as key press target
	SetForegroundWindow(elite);

	Sleep(200); // VERY IMPORTANT !!!

	bool multipleKeys = false;
	if (wcschr(lpCmdLine, sep[0]) != NULL)
		multipleKeys = true;
	
	// Tokenize the command line, expecting "xx-xx-xx" with xx as key scan code.
	wchar_t *nextToken = NULL;
	wchar_t *token = wcstok_s(lpCmdLine, sep, &nextToken);
	while (token != NULL) {
		try {
			int modifier = NONE;
			if (wcschr(token, 'c') != NULL)
				modifier = LCTRL;
			if (wcschr(token, 'C') != NULL)
				modifier = RCTRL;
			if (wcschr(token, 's') != NULL)
				modifier = LSHIFT;
			if (wcschr(token, 'S') != NULL)
				modifier = RSHIFT;
			if (wcschr(token, 'a') != NULL)
				modifier = LALT;
			if (wcschr(token, 'A') != NULL)
				modifier = RALT;
			
			// Decode the key (the optional letter at the end gets ignored by stoi())
			keycode = std::stoi(token);

			// Send the key to foreground application
			sendKey(keycode,modifier);
			if (multipleKeys)
				Sleep(400);
		}
		catch (...) {
			std::wstring val1(token);
			std::wstring val2(lpCmdLine);
			std::wstring message = L"Unable to parse the passed value to a number! Value:[" + val1 + L"] Command line:[" + val2 + L"]";
			MessageBox(NULL, (LPCWSTR)message.c_str(), (LPCWSTR)L"key2elite", MB_ICONWARNING);
			return -3;
		}
		token = wcstok_s(NULL, sep, &nextToken);
	}

	return 0;
}
