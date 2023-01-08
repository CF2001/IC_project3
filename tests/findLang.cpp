#include <iostream>
#include <string>
#include <vector>

#include "../src/Fcm.cpp"


int main(int argc, char *argv[])
{
	
	// A minimum of 10 model texts is required
	if (argc < 12)
	{
		cerr << "\nUsage: ./findLang <modelFile1> <...> <modelFile10> <languageToDiscover>\n";
		cerr << " A minimum of 10 text models is required\n";
		return 1;
	}
	
	string languageToDiscover = argv[argc-1];	// last argument 
	vector<string> modelLanguages;				// all model files
	for (int l = 1; l < argc-1; l++)
	{
		modelLanguages.push_back(argv[l]);
	}

	int order {0};	
	double smoothing {0}; 

	cout << endl;
	while (order <= 0)
	{
		cout << "\033[1;34mInsert the order of the model: \033[0m";
		cin >> order;
	}

	while (smoothing <= 0)
	{
		cout << "\033[1;34mInsert the smoothing (alpha) of the model: \033[0m";
		cin >> smoothing;
	}
	cout << endl;
	
	Fcm f {order, smoothing}; 
	double bitsEstimationComp {0};
	double bestEstimation {0};
	string bestLanguage {};
	
	int start = 0;

	cout << "\033[1;34m ----------------------- Starting Language Recognition System ----------------------- \033[0m" << endl;	

	// 1º - Determine the model for each modelLanguage 
	// 2º - Estimating the bits needed for compression of the language 
	//		to be found through the respective modelLanguage
	for (string modelLang : modelLanguages)
	{
			cout << "\nCalculating the model and estimated bits for text compression...\n" << endl;

			cout << "+-------------------------------+" << endl;
			cout << "|    Model Language:       \t|    ";
			cout << 		modelLang			<< endl;
			cout << "---------------------------------"	<< endl;
			cout << "|    Language To Discover:  \t|   ";
			cout << 	  languageToDiscover 	<< endl;
			cout << "---------------------------------" << endl;


			bitsEstimationComp = f.nBitsToCompress(modelLang, languageToDiscover);

			cout << "|    Estimated Bits:       \t|    ";
			cout <<       bitsEstimationComp  << endl;
			cout << "---------------------------------" << endl;
			
			/* Initialization - it is initially considered that the language with the closest 
				and best bit estimate is the initial one in the modelLanguages list
			*/
			if (start == 0)	
			{
				bestLanguage = modelLang;
				bestEstimation = bitsEstimationComp;

				cout << "\nInitialization of the best estimate value: \n" << endl;
				cout << "\033[1;32m Best Language: \033[0m" << bestLanguage << endl;
				cout << "\033[1;32m Best Estimation: \033[0m" << bestEstimation << endl;
			}else{
				if (bestEstimation > bitsEstimationComp)
				{
					bestLanguage = modelLang;
					bestEstimation = bitsEstimationComp;
	
					cout << "\n\033[1;32m Best Language: \033[0m" << bestLanguage << endl;
					cout << "\033[1;32m Best Estimation: \033[0m" << bestEstimation << endl;
				}
			}
			start++;
			cout << "______________________________________________________________________________\n";
	}
	
	cout << "\n Text " << languageToDiscover << " is written in " << "\033[1;32m " << f.languageName(bestLanguage) << "\033[0m\n"<< endl;
	 
	return 0; 
}
