#include "Folder.h"

Folder::Folder() {

	setlocale(LC_ALL, "Turkish");
}


Folder::Folder(const string &path) {

	setlocale(LC_ALL, "Turkish");

	p = path;
	name = p.filename().string();
}


Folder::~Folder(){}

bool Folder::create() {

	if (check()) {

		string choice;

		while (!iequals(choice, "q")) {

			cout << name << " klasörü mevcut, yeniden oluþturulsun mu? (E/H/Q)";

			cin >> choice;

			if (iequals(choice, "e")) {

				remove();

				try {

					create_directory(getPath());
				}
				catch (filesystem_error &e) {

					std::cerr << e.what() << '\n';
				}

				cout << name << " klasörü yeniden oluþturuldu..." << endl;

				return true;
			} else if (iequals(choice, "h")) {

				return false;
			}
		}

		cout << "Çýkýþ yapýlýyor..!" << endl;

		std::system("PAUSE");

		exit(1);
	} else {

		try {

			create_directory(getPath());
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}

		cout << name << " klasörü oluþturuldu..." << endl;

		return true;
	}
}

bool Folder::createSubfolder(const string &name) const {

	if (checkSubfolder(name)) {

		string choice;

		while (!iequals(choice, "q")) {

			cout << name << " klasörü mevcut, yeniden oluþturulsun mu? (E/H/Q)";

			cin >> choice;

			if (iequals(choice, "e")) {

				removeSubfolder(name);

				try {

					create_directory(getPath() + name);
				}
				catch (filesystem_error &e) {

					std::cerr << e.what() << '\n';
				}

				cout << name << " klasörü yeniden oluþturuldu..." << endl;

				return true;
			}
			else if (iequals(choice, "h")) {

				return false;
			}
		}

		cout << "Çýkýþ yapýlýyor..!" << endl;

		std::system("PAUSE");

		exit(1);
	}
	else {

		try {

			create_directory(getPath() + name);
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}

		cout << name << " klasörü oluþturuldu..." << endl;

		return true;
	}
}



bool Folder::check() const {

	if (is_directory(getPath())) {

		//cout << name << " klasörü mevcut..." << endl;

		return true;
	}
	else {

		//cout << name << " klasörü mevcut deðil..!" << endl;

		return false;
	}
}

bool Folder::checkSubfolder(const string &name) const {

	if (is_directory(getPath() + name)) {

		//cout << name << " klasörü mevcut..." << endl;

		return true;
	}
	else {

		//cout << name << " klasörü mevcut deðil..!" << endl;

		return false;
	}
}


bool Folder::rename(const string &name) {

	if (check()) {

		try {

			boost::filesystem::rename(getPath(), getParentFolderPath() + name);
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}		

		//cout << this->name << " klasörü" << name << " olarak yeniden adlandýrýldý..." << endl;

		p = getParentFolderPath() + name;
		this->name = name;

		return true;
	}
	else {

		//cout << this->name << " klasörü mevcut deðil, adlandýrýlamadý..!" << endl;

		return false;
	}
}

bool Folder::renameSubfolder(const string &oldName, const string &newName) const {

	if (checkSubfolder(oldName)) {

		try {

			boost::filesystem::rename(getPath() + oldName, getPath() + newName);
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}

		return true;
	} else {

		return false;
	}
}


bool Folder::remove() {

	if (check()) {		

		try {

			boost::filesystem::remove_all(getPath());
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}

		//cout << name << " dosyasý silindi..." << endl;

		return true;
	}
	else {

		//cout << name << " dosyasý mevcut deðil, silinemedi..!" << endl;

		return false;
	}
}

bool Folder::removeSubfolder(const string &name) const {

	if (checkSubfolder(name)) {

		try {

			boost::filesystem::remove_all(getPath() + name);
		}
		catch (filesystem_error &e) {

			std::cerr << e.what() << '\n';
		}

		//cout << name << " dosyasý silindi..." << endl;

		return true;
	}
	else {

		//cout << name << " dosyasý mevcut deðil, silinemedi..!" << endl;

		return false;
	}
}

void Folder::changeFileNamesNumerically() const {

	int i = 0;

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			string newFileName = getPath() + to_string(i) + extension(x);

			boost::filesystem::rename(getPath() + "/" + x.path().filename().string(), newFileName);

			i++;
		}
	}
}

void Folder::getName(string &name) const {

	name = this->name;
}

string Folder::getName() const {

	return name;
}

void Folder::setPath(const string &path) {

	if (name.empty() || p.string().empty()) {

		p = path;
		name = p.filename().string();
	} else {

		cout << "Path zaten setlenmiþ..!" << endl;
	}	
}

void Folder::getPath(string &path) const {

	path = p.string() + "/";
}

string Folder::getPath() const {

	return p.string() + "/";
}

void Folder::getParentFolderPath(string &path) const {

	path = p.parent_path().string() + "/";
}
string Folder::getParentFolderPath() const {

	return p.parent_path().string() + "/";
}

void Folder::getParentFolderName(string &path) const {

	path = p.parent_path().filename().string();
}

string Folder::getParentFolderName() const {

	return p.parent_path().filename().string();
}

int Folder::getNumberOfFiles() const {

	int i = 0;

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			i++;
		}
	}

	return i;
}
int Folder::getNumberOfSubfolders() const {

	int i = 0;

	for (auto &&x : directory_iterator(p)) {

		if (is_directory(getPath() + x.path().filename().string())) {

			i++;
		}
	}

	return i;
}



void Folder::getFileNames(vector<string> &fileNames) const {

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			fileNames.push_back(x.path().filename().string());
		}		
	}

	//sort(fileNames.begin(), fileNames.end());
}

void Folder::getSubfolderNames(vector<string> &folderNames) const {

	for (auto &&x : directory_iterator(p)) {

		if (is_directory(getPath() + x.path().filename().string())) {

			folderNames.push_back(x.path().filename().string());
		}
	}

	//sort(folderNames.begin(), folderNames.end());
}

void Folder::getFilePathNames(vector<string> &filePathNames) const {

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			filePathNames.push_back(p.string() + "/" + x.path().filename().string());
		}
	}

	sort(filePathNames.begin(), filePathNames.end());
}


void Folder::getSubfolderPathNames(vector<string> &folderPathNames) const {

	for (auto &&x : directory_iterator(p)) {

		if (is_directory(getPath() + x.path().filename().string())) {

			folderPathNames.push_back(p.string() + "/" + x.path().filename().string());
		}
	}

	sort(folderPathNames.begin(), folderPathNames.end());
}


void Folder::printFileNames() const {

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			cout << x.path().filename().string() << endl;
		}		
	}
}

void Folder::printSubfolderNames() const {

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			cout << getPath() + x.path().filename().string() << endl;
		}
	}
}

void Folder::printFilePathNames() const {

	for (auto &&x : directory_iterator(p)) {

		if (is_regular_file(getPath() + x.path().filename().string())) {

			cout << x.path().filename().string() << endl;
		}
	}
}


void Folder::printSubfolderPathNames() const {

	for (auto &&x : directory_iterator(p)) {

		if (is_directory(getPath() + x.path().filename().string())) {

			cout << getPath() + x.path().filename().string() << endl;
		}
	}
}