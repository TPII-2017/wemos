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
 * Se asume que el archivo (js, css) va a estar en la carpeta con nombre
 * de la constante PATH y la extensión, ej:
 * 			accordion.js va a estar en "{const PATH}/js/accordion.js"
 */
const int CANT_DEP = 4;
const sourceT dep[CANT_DEP] = {
		{"js","accordion", "<script src=\"js/accordion.js\"></script>"},
		{"js","navbar", "<script src=\"js/navbar.js\"></script>"},
		{"css","bootstrap.min", "<link href=\"css/bootstrap.min.css\" rel=\"stylesheet\">"},
		{"css","my-styles", "<link href=\"css/my-styles.css\" rel=\"stylesheet\">"}
	};

string PATH;
string NAME_FILE;

int is_dependence(string line);
inline string block_html(string type, bool begin);
inline void add_dependence(ofstream &newFile, sourceT dep);
void import_sources(ofstream &newFile, ifstream &file);

/*
 * 1er param es el path donde estara las dependencias y
 * 2do param es el nombre del archivo con extensión.
 */
int main(int argc, char* argv[])
{	
	if (argc != 3)
	{
		cout << " - Solo un parametro:";
		cout << " nombre del archivo con su extensión." << std::endl;
		return 0;
	}
	PATH = argv[1];
	NAME_FILE = argv[2];

	ofstream newFile((PATH+"/"+NAME_FILE).c_str());
	ifstream file(NAME_FILE.c_str());
	
	if (newFile.is_open())
	{	
		import_sources(newFile, file);
		newFile.close();
		file.close();
	}
}

void import_sources(ofstream &newFile, ifstream &file){
	string line;
	bool stop = false;
	int indexDep;

	while (getline(file, line))
	{	
		indexDep = is_dependence(line);
		if (indexDep != -1) // Si hay una dependencia lo agrega
			add_dependence(newFile, dep[indexDep]);
		else
			newFile << line << std::endl;
	}
}

inline void add_dependence(ofstream &newFile, sourceT dep){
	string line;
	ifstream sourceFile((PATH+"/"+dep.type+"/"+dep.name+"."+dep.type).c_str());
	newFile << block_html(dep.type, 1) << std::endl;
	while (getline(sourceFile, line))
		newFile << line << std::endl;
	newFile << block_html(dep.type, 0) << std::endl;
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

inline string block_html(string type, bool begin){
	if (begin)
		return (type == "js")? "<script>" : "<style>";
	else
		return (type == "js")? "</script>" : "</style>";
}