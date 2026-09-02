#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <malloc.h>
#include <memory.h>
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <stdexcept>
#include "Functions.hpp"
#include "FileSystem.hpp"

using namespace std;
namespace fs = std::filesystem;


int main() {
	string savePath = GetDirectory();
	
	vector<Protocol> PrtList;

	LoadData(PrtList, savePath);

	while (true) {
		system("cls");
		cout << "   ======Menu======  ";
		cout << "\n1) Start the protocol";
		cout << "\n2) Create a new protocol";
		cout << "\n3) Edit protocol";
		cout << "\n4) Display all protocols";
		cout << "\n5) Open Debug file";
		cout << "\n6) Exit";
		cout << "\n\n Enter command: ";

		int UI = getUI<int>();

		switch (UI) {

		case 1: Start(PrtList); break;																		//Start protocol

		case 2: CreateProtocol(PrtList); AutoChangeNumber(PrtList); SaveData(PrtList, savePath); break;		//Create new protocol

		case 3: EditProtocol(PrtList); AutoChangeNumber(PrtList); SaveData(PrtList, savePath); break;		//Edit Protocol

		case 4: DisplayProtocols(PrtList); pause(); break;													//Display all protocols

		case 5: OpenDebugFile(savePath); break;																//Open file AutoProtocols.txt

		case 6: AutoChangeNumber(PrtList); SaveData(PrtList, savePath); exit(0);							//Exit

		default: cout << "\nInvalid input. Try again."; pause(); continue;
		}
	}
}
