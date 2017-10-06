#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

struct sourceT {
	string type;
	string name;
	string call;
};

/* 
 * Se asume que el archivo va a estar en la carpeta del mismo nombre
 * que la dependencia, ej:
 * 			accordion.js va a estar en "js/accordion.js"
 */
const int CANT_DEP = 4;
const sourceT dep[CANT_DEP] = {
				{"js","accordion", "<script src=\"js/accordion.js\"></script>"},
				{"js","navbar", "<script src=\"js/navbar.js\"></script>"},
				{"css","bootstrap.min", "<link href=\"css/bootstrap.min.css\" rel=\"stylesheet\">"},
				{"css","my-styles", "<link href=\"css/my-styles.css\" rel=\"stylesheet\">"}
				};

const string path = "outputs/";
string name;

inline bool file_exist(string n);
int is_dependence(string line);
inline string blockHtml(string type, bool begin);
inline void add_dependence(ofstream &newFile, sourceT dep);
void import_sources(ofstream &newFile, ifstream &file);

/* 1er nombre del archivo*/
int main(int argc, char* argv[])
{	
	if (argc != 2)
		return 0;
	name = argv[1];

	cout << (path+name) << std::endl;
	ofstream newFile((path+name).c_str());
	ifstream file(name.c_str());
	
	if (newFile.is_open())
	{	
		import_sources(newFile, file);
		newFile.close();
		file.close();
	}
}

inline bool file_exist(string n) {
	ifstream f(n.c_str());
	return f.good();
}

void import_sources(ofstream &newFile, ifstream &file){
	string line;
	bool stop = false;
	int index_dep;

	while (getline(file, line))
	{	
		index_dep = is_dependence(line);
		if (index_dep != -1) // Si hay una dependencia lo agrega
			add_dependence(newFile, dep[index_dep]);
		else
			newFile << line << std::endl;
	}
}

inline void add_dependence(ofstream &newFile, sourceT dep){
	string line;
	ifstream sourceFile((dep.type + "/" + dep.name + "." + dep.type).c_str());
	newFile << blockHtml(dep.type, 1) << std::endl;
	while (getline(sourceFile, line))
		newFile << line << std::endl;
	newFile << blockHtml(dep.type, 0) << std::endl;
	sourceFile.close();
}

int is_dependence(string line){
	for(int i=0; i<CANT_DEP; i++){
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		if (!line.compare(dep[i].call)){
			return i;
		}
	}
	return -1;
}

inline string blockHtml(string type, bool begin){
	if (begin){
		return (type == "js")? "<script>" : "<style>";
	}else{
		return (type == "js")? "</script>" : "</style>";
	}
}