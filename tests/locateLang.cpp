#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <filesystem>

#include "../src/Fcm.cpp"

#define usage "./locateLang [options] <modelFile1> ... <modelFile2> <FileToTest> \nfor more see ./locateLang -h"

int main(int argc, char *argv[])
{
	cout << endl;
	int opt;
	int order = 7, sensitivity = 3;
	double smoothing = 0.1;
	char* path;
	bool verb = false, stats = false;
	string languageToDiscover;
	vector<string> modelLanguages;

	//opções order (-o), smothing (-s) e sensitivity (-t) precisam de argumentos
    while ((opt = getopt(argc, argv, "t:vo:s:h")) != -1)
    {
        switch (opt)
        {
			case 'i': { //chama statistics
				stats = true;
				cout << "statistics ON" << endl;
				break;
			}
			case 't': { //janela da média para suavização da curva
				int t = atoi(optarg);
				if(t > 0) {
					sensitivity = t;
				} else {
					cout << "sensitivity parameter must be grater than 0"<< endl;
					cout << usage << endl;
					return -1;
				}
				break;
			}
            case 'v': { //verbose
				verb = true;
				cout << "verbose ON\n" << endl;
				break;
			}
			case 'o': { //order parameter
				int o = atoi(optarg);
				if(o > 0) {
					order = o;
				} else {
					cout << "order parameter must be grater than 0"<< endl;
					cout << usage << endl;
					return -1;
				}
				break;
			}
			case 's': { //smoothing parameter
				int s = atof(optarg);
				if(s > 0) {
					order = s;
				} else {
					cout << "smoothing parameter must be grater than 0"<< endl;
					cout << usage << endl;
					return -1;
				}
				break;
			}
			case 'h': //Elaborate
			{
				cout << "you asked for help........"<< endl;
				cout << usage << endl;
				return 0;
				break;
			}
			default: {
				cout << "wrong option" << endl;
				cout << usage << endl;
			}
		}
	}
	if (argc - optind < 2) { //verifica se tem pelo menos 1 diretório/modelo e 1 nome para o ficheiro de teste
		cout << "missing obrigatory arguments" << endl;
		cout << usage << endl;
		return -1;
	}
	
	//processa argumentos obrigatórios
	
	if(verb) cout << "got files: " << endl;
	for (int i = optind; i < argc-1; i ++) {
		path = argv[i];
		if (filesystem::is_directory(path)) {
			for (const auto & entry : filesystem::directory_iterator(path)) {
				modelLanguages.push_back(entry.path()); //insere no modelo
				
				if(verb) cout << entry.path() << endl;
			}
		}
		else if (filesystem::is_regular_file(path)) {
			modelLanguages.push_back(path); //insere no modelo

			if(verb) cout << path << endl;
		}
		else {
			cout << "can't find file or folder " << path << endl;
			cout << usage << endl; 
			return -1;
		}
    }
	if (filesystem::is_regular_file(argv[argc-1])) {
		languageToDiscover = argv[argc-1];
		if(verb) cout << "file to test:\n" << languageToDiscover << endl;
	}
	else {
			cout << "can't find file " << path << endl;
		cout << usage << endl;
		return -1;
	}

	Fcm f {order, smoothing}; 

	
	if(stats) f.statisticsModelCreation(modelLanguages);
	f.locateLang(modelLanguages, languageToDiscover, sensitivity, verb);
	return 0;
}
