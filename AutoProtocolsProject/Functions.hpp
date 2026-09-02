#pragma once
#include <iostream>
#include <io.h>       
#include <fcntl.h>  
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
using namespace std;
namespace fs = std::filesystem;

void pause() {
	cout << "\n\nPress Enter to continue...";
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	cin.get();
}

template<typename T>
T getUI() {
	T value;
	while (true) {
		//cout << prompt;
		if (cin >> value) {
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			return value;
		}
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		cout << "Invalid input. Try again... ";
	}
}

inline string getString() {
	string value;
	while (true) {
		//cout << prompt;
		if (getline(cin, value) && !value.empty()) {
			return value;
		}
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		cout << "Invalid input. Try again...\n";
	}
}

string getStringOptional() {
	string value;
	//cout << prompt;
	getline(cin, value);
	return value;
}



class Protocol {
public:
	class Program {
	private:
		int Number;
		string Name, FilePath;
	public:
		Program() : Number(0), Name("None"), FilePath("None") {}

		Program(int num, string n, string f) : Number(num), Name(n), FilePath(f) {}

		int getNumber() const { return Number; }

		string getName() const { return Name; }

		string getPath() const { return FilePath; }

		void setNumber(int t) { Number = t; }

		void StartPr() {
			string command = " -fullscreen";
			vector<char> buffer(FilePath.begin(), FilePath.end());
			//buffer.insert(buffer.end(), command.begin(), command.end());
			buffer.push_back('\0');
			//buffer.data();

			STARTUPINFOA si = { sizeof(si) };
			PROCESS_INFORMATION pi;

			if (CreateProcessA(
				nullptr,
				buffer.data(),
				nullptr,
				nullptr,
				false,
				0,
				nullptr,
				nullptr,
				&si,
				&pi
			)) {
				CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
			}
			else {
				DWORD err = GetLastError();
				if (ShellExecuteA(NULL, "open", FilePath.c_str(), NULL, NULL, SHOW_FULLSCREEN)) {}
				else { cout << "\nApplication launch error. Err code: " << err; pause(); }
			}
		}

		void ChangeName(string& n) { Name = n; }

		void ChangePath(string& p) { FilePath = p; }

		void Display() {
			cout << "\nProgram number: " << Number << "\nProgram name: " << Name << "\nFile path: " << FilePath;
		}

		~Program() {}
	};

private:
	int Number;
	string Name, Description;
public:
	vector<Program> PrList;
public:
	Protocol() : Number(0), Name("None"), Description("None") {}
	Protocol(int num, string name = "None", string des = "None") : Number(num), Name(name), Description(des) {}

	~Protocol() {}

	int getNumber() const { return Number; }

	string getName() const { return Name; }

	string getDesc() const { return Description; }

	void setPrtNumber(int t) { Number = t; }

	void StartProtocol() {
		for (size_t i = 0; i < PrList.size(); i++) {
			PrList[i].StartPr();
		}
	}

	void ChangeName(string& n) { Name = n; }

	void ChangeDesc(string& d) { Description = d; }

	void Display() {
		cout << "\nProtocol number: " << Number << "\nName: " << Name << "\nDesc: " << Description;
	}
};



void DisplayProtocols(vector<Protocol>& List) {
	//Display Protocol
	if (!(List.empty())) {

		for (size_t i = 0; i < List.size(); i++) {
			//cout << "\n\nProtocol #" << i + 1 << ":" << endl;
			List[i].Display();
			cout << "\n-------------------";
			//Programs from protocol
			for (size_t p = 0; p < List[i].PrList.size(); p++) {
				List[i].PrList[p].Display(); cout << endl;
			}
			cout << "\n===================";
		}
	}
	else {
		cout << "\nProtocol list is empty!";
		return;
	}
}

void CreateProtocol(vector<Protocol>& LPrtList) {
	cout << "\nEnter protocol name: ";
	string LocalName = getStringOptional();
	if (LocalName.empty()) { LocalName = "None"; }
	cout << "\nEnter protocol description: ";
	string LocalDesc = getStringOptional();
	if (LocalDesc.empty()) { LocalDesc = "None"; }


	LPrtList.push_back(Protocol(LPrtList.size() + 1, LocalName, LocalDesc));

	cout << "\nEnter 1 to create a program or 0 to exit: ";
	int LocalUI = getUI<int>();
	while (true) {
		switch (LocalUI) {
		case 1: {
			cout << "\nEnter program name: ";
			string LocalProgN = getStringOptional();
			if (LocalProgN.empty()) { LocalProgN = "None"; }
			cout << "\nEnter the program location or website URL: ";
			string LocalPath = getStringOptional();
			if (LocalPath.empty()) { LocalPath = "None"; }

			LPrtList.back().PrList.push_back(Protocol::Program(LPrtList.back().PrList.size() + 1, LocalProgN, LocalPath));
			break;
		}
		case 0: return;
		}
		cout << "\nEnter 1 to create a program or 0 to exit: ";
		LocalUI = getUI<int>();
	}
}

void AutoChangeNumber(vector<Protocol>& PrtList) {
	for (size_t i = 0; i < PrtList.size(); i++) {
		PrtList[i].setPrtNumber(i + 1);
		for (size_t p = 0; p < PrtList[i].PrList.size(); p++) {
			PrtList[i].PrList[p].setNumber(p + 1);
		}
	}
}

void EditProtocol(vector<Protocol>& LPrtList) {
	if ((LPrtList.empty())) { cout << "\nProtocol list is empty, try again!"; pause(); return; }
	while (true) {
		AutoChangeNumber(LPrtList);
		string LName;
		string LDesc;
		string LPName;
		string LPPath;
		system("cls");

		DisplayProtocols(LPrtList);

		cout << "\nEnter the number of the protocol you want to work with or press 0 to exit: ";
		int PrtNumber = getUI<int>();
		int ProgNumber;
		if (PrtNumber == 0) { return; }
		else if (PrtNumber > LPrtList.size() || PrtNumber < 0) { cout << "\nOut of range!"; pause(); return; }
		Protocol* SelProtocol = &LPrtList[PrtNumber - 1];
		//Protocol::Program* SelProgram = nullptr;

		cout << "   ===Select an action===  ";
		cout << "\n1) Delete the protocol";
		cout << "\n2) Change protocol name";
		cout << "\n3) Change protocol description";
		cout << "\n4) Create the program";
		cout << "\n5) Delete the program";
		cout << "\n6) Change program name";
		cout << "\n7) Change program path/url";
		cout << "\n8) Exit";
		cout << "\n\n Enter command: ";
		int UI = getUI<int>();

		switch (UI) {
		case 1: LPrtList.erase(LPrtList.begin() + PrtNumber - 1); continue;

		case 2: cout << "\nEnter new protocol name: ";  LName = getString();
			if (LName.empty()) { LName = "None"; } SelProtocol->ChangeName(LName); continue;

		case 3: cout << "\nEnter new protocol description: ";  LDesc = getString();
			if (LDesc.empty()) { LDesc = "None"; }SelProtocol->ChangeDesc(LDesc); continue;

		case 4: cout << "\nEnter program name: "; LPName = getStringOptional();
			cout << "\nEnter program path: "; LPPath = getStringOptional();
			SelProtocol->PrList.push_back(Protocol::Program(LPrtList[PrtNumber].PrList.size() + 1, LPName, LPPath)); continue;

		case 5: cout << "\nEnter the number of the program you want to delete: "; ProgNumber = getUI<int>();
			if (ProgNumber < 1 || ProgNumber > SelProtocol->PrList.size()) {
				cout << "\nInvalid program number."; pause(); continue;
			}
			SelProtocol->PrList.erase(SelProtocol->PrList.begin() + ProgNumber - 1); continue;


		case 6: cout << "\nEnter the number of the program you want to change name: "; ProgNumber = getUI<int>();
			if (ProgNumber < 1 || ProgNumber > SelProtocol->PrList.size()) {
				cout << "\nInvalid program number."; pause(); continue;
			}
			cout << "\nEnter new program name: "; LPName = getString(); if (LPName.empty()) { LPName = "None"; }
			SelProtocol->PrList[ProgNumber - 1].ChangeName(LPName); continue;


		case 7: cout << "\nEnter the number of the program you want to change path/url: "; ProgNumber = getUI<int>();
			if (ProgNumber < 1 || ProgNumber > SelProtocol->PrList.size()) {
				cout << "\nInvalid program number."; pause(); continue;
			}
			cout << "\nEnter new program path/url: "; LPPath = getString(); if (LPPath.empty()) { LPPath = "None"; }
			SelProtocol->PrList[ProgNumber - 1].ChangePath(LPPath); continue;


		case 8: return;
		default: cout << "\nInvalid input. Try again."; pause(); continue;
		}
	}
}

void Start(vector<Protocol>& LPrtList) {
	if (!(LPrtList.empty())) {
		DisplayProtocols(LPrtList);
		int PrtNumber;
		cout << "\nEnter the number of the protocol you want to start or 0 to exit: ";
		PrtNumber = getUI<int>();
		if (PrtNumber == 0) { return; }
		else if (PrtNumber < 0 or PrtNumber > LPrtList.size()) { cout << "Out of range!"; pause(); return; }
		Protocol* SelProtocol = &LPrtList[PrtNumber - 1];
		SelProtocol->StartProtocol();
	}
	else { cout << "\nProtocol list is empty, try again!"; pause(); }
}






