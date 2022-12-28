#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <filesystem>

#include "../src/Fcm.cpp"

#define usage "./locateLang [options] <modelFile1/folder1> ... <modelFileN/folderWithModelsN> <FileToTest> \nfor more see ./locateLang -h"

int main(int argc, char *argv[])
{
	cout << endl;
	int opt;
	int order = 7, window = 3;
	double smoothing = 0.1;
	char* path;
	bool verb = false, stats = false;
	string languageToDiscover;
	vector<string> modelLanguages;

	//opções order (-o), smothing (-s) e window (-w) precisam de argumentos
    while ((opt = getopt(argc, argv, "iw:vo:s:h")) != -1)
    {
        switch (opt)
        {
			case 'i': { //chama statistics
				stats = true;
				cout << "statistics ON" << endl;
				break;
			}
			case 'w': { //janela da média para suavização da curva
				int w = atoi(optarg);
				if(w > 0) {
					window = w;
				} else {
					cout << "window parameter must be grater than 0"<< endl;
					cout << usage << endl;
					return -1;
				}
				break;
			}
            case 'v': { //verbose
				verb = true;
				cout << "verbose ON" << endl;
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
				float s = atof(optarg);
				if(s > 0) {
					smoothing = s;
				} else {
					cout << "smoothing parameter must be grater than 0"<< endl;
					cout << usage << endl;
					return -1;
				}
				break;
			}
			case 'h':
			{
				cout << "\tLOCATELANG HELP\n " <<
						"./locateLang [options] <modelFile1> ... <modelFileN> <FileToTest>\n" <<
						"alternative usage: ./locateLang [options] <folderWithModels> <FileToTest>\n" << 
						"\n" <<
						"options:\n" <<
						"-h Show help message\n" <<
						"-i Generate statistics report for generation of the models (i.e. time taken and bytes processed).\n" <<
						"-v Turns verbose on, the program will display each step of the process and all the context processed.\n" <<
						"-o Takes as required parameter an <integer> number. Sets the order in which the models will be produced (i.e. the length of the Markov's chain). Default value is 7.\n" <<
						"-s Takes as required parameter a <double> precision number. Sets the smoothing parameter so it can have a generic probability even if the context and character never shows on the training data. Default value is 0.1.\n" <<
						"-w Takes as required parameter <integer> number. Sets the size of the floating window used to compare probabilities between models. It will have an influence on the sensitivity of the algorithm. Default value is 3.\n" << 
						"\n" <<
						"note 1: It's required at least one model file and one test file (always the last argument)\n" <<
						"note 2: A model file can be a path to a folder containing text files, each representing one model/language" << endl;
				return 0;
				break;
			}
			default: {
				cout << "wrong option" << endl;
				cout << usage << endl;
				return -1;
			}
		}
	}
	if (argc - optind < 2) { //verifica se tem pelo menos 1 diretório/modelo e 1 nome para o ficheiro de teste
		cout << "missing obrigatory arguments" << endl;
		cout << usage << endl;
		return -1;
	}
	
	//processa argumentos obrigatórios
	if(verb) {
		cout << "order: " << order << endl;
		cout << "smoothing: " << smoothing << endl;
		cout << "sensitivity window: " << window << endl;
		cout << "\ngot files to model: " << endl;
		}
	for (int i = optind; i < argc-1; i++) {
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

	//innitialize class
	Fcm f {order, smoothing};
	//get statistics
	if(stats) f.statisticsModelCreation(modelLanguages);
	//locate languages
	map<int,string> langChange = f.locateLang(modelLanguages, languageToDiscover, window, verb);

	return 0;
}
