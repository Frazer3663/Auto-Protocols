#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <string>
#include <shellapi.h>
#include <shlobj.h>
#include <stdexcept>
using namespace std;
namespace fs = std::filesystem;


string GetDirectory() {
	char appDataPath[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);
	string tsavePath = string(appDataPath) + "\\AutoProtocols\\protocols.txt";

	string folderPath = string(appDataPath) + "\\AutoProtocols";
	CreateDirectoryA(folderPath.c_str(), NULL);

	return tsavePath;
}

void clearFile(const string& fileName) {
	ofstream file(fileName, ios::trunc);
}

bool isFileEmpty(const string& fileName) {
	return fs::file_size(fileName) == 0;
}

void SaveData(const vector<Protocol>& LPrtList, const string& filename) {
	clearFile(filename);
	ofstream file(filename);
	file << "\nATTENTION! CHANGE ANYTHING IN THIS FILE ONLY IF YOU KNOW WHAT YOU ARE DOING.\n\n";

	for (const auto& CurrentPrt : LPrtList) {
		file << "\n[Protocol]" << endl;
		file << "Protocol name: " << CurrentPrt.getName() << endl;
		file << "Protocol desc: " << CurrentPrt.getDesc() << endl;
		file << "Program list size: " << CurrentPrt.PrList.size() << endl;

		for (const auto& CurrentProgram : CurrentPrt.PrList) {
			//file << "[Programs]" << endl;
			file << "Program name: " << CurrentProgram.getName() << endl;
			file << "Program path: " << CurrentProgram.getPath() << endl;
		}
	}
}

void LoadData(vector<Protocol>& LPrtList, const string& filename) {
	ifstream file(filename);

	if (!file) {
		ofstream create_file(filename);
		if (!create_file) { cout << "Cant open/create file, check access rights"; pause(); return; }
		create_file.close();
		file.open(filename);
	}

	string line;

	while (getline(file, line)) {
		if (line == "[Protocol]") {
			string name, desc;
			int progCount;

			//Name
			file.ignore(256, ':');
			file.ignore(256, ' ');
			getline(file, name);

			//Desc
			file.ignore(256, ':');
			file.ignore(256, ' ');
			getline(file, desc);

			//Programs count
			file.ignore(256, ':');
			file.ignore(256, ' ');
			file >> progCount;
			file.ignore(256, '\n');

			Protocol proto(LPrtList.size() + 1, name, desc);

			for (size_t i = 0; i < progCount; i++) {
				string pName, pPath;
				file.ignore(256, ':');
				file.ignore(256, ' ');
				getline(file, pName);
				file.ignore(256, ':');
				file.ignore(256, ' ');
				getline(file, pPath);

				proto.PrList.push_back(Protocol::Program(i + 1, pName, pPath));
			}
			LPrtList.push_back(proto);
		}
	}
}

void OpenDebugFile(string savePath) {
	DWORD err = GetLastError();
	if(ShellExecuteA(NULL, "open", savePath.c_str(), NULL, NULL, SW_SHOWDEFAULT)) {}
	else { cout << "\nError: " << err; pause(); }
}