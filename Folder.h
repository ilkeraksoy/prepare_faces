#ifndef FOLDER_H
#define FOLDER_H

#include <iostream>
#include <string>
#include <locale.h>

#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

class Folder {

private:

	string name;
	path p;

public:

	Folder();
	Folder(const string &name);
	~Folder();

	bool create();
	bool createSubfolder(const string &name) const;
	bool check() const;
	bool checkSubfolder(const string &name) const;
	bool rename(const string &name);
	bool renameSubfolder(const string &oldName, const string &newName) const;
	bool remove();
	bool removeSubfolder(const string &name) const;
	void changeFileNamesNumerically() const;
	void getName(string &name) const;
	string getName() const;
	void setPath(const string &path);
	void getPath(string &path) const;
	string getPath() const;
	void getParentFolderPath(string &path) const;
	string getParentFolderPath() const;
	void getParentFolderName(string &path) const;
	string getParentFolderName() const;
	int getNumberOfFiles() const;
	int getNumberOfSubfolders() const;
	void getFileNames(vector<string> &fileNames) const;
	void getSubfolderNames(vector<string> &folderNames) const;
	void getFilePathNames(vector<string> &filePathNames) const;
	void getSubfolderPathNames(vector<string> &folderPathNames) const;
	void printFileNames() const;
	void printSubfolderNames() const;
	void printFilePathNames() const;
	void printSubfolderPathNames() const;
};

#endif //FOLDER_H