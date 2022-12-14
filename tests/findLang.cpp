#include <iostream>
#include <string>
#include <vector>

#include "../src/Fcm.cpp"

/*
./findLang ../languages/Bulgarian/BULG1.txt ../languages/English/english1.txt ../languages/Estonian/estonian1.txt ../languages/French/FR1.txt ../languages/German/german1.txt ../languages/Greek/greek1.txt ../languages/Italian/italian1.txt ../languages/Polish/polish1.txt ../languages/Portuguese/lusiadasCanto1.txt ../languages/Spanish/spanish1.txt ../languages/French/FR2.txt
*/
int main(int argc, char *argv[])
{
	
	// A minimum of 10 model texts is required
	if (argc < 12)
	{
		cerr << "Usage: ./lang <modelFile1> <...> <modelFile10> <languageToDiscover>\n";
		return 1;
	}
	
	string languageToDiscover = argv[argc-1];
	vector<string> modelLanguages;
	for (int l = 1; l < argc-1; l++)
	{
		modelLanguages.push_back(argv[l]);
	}

	int order = 0;	
	double smoothing = 0; 

	cout << endl;
	while (order <= 0)
	{
		cout << "Insert the order of the model: ";
		cin >> order;
	}

	while (smoothing <= 0)
	{
		cout << "Insert the smoothing (alpha) of the model: ";
		cin >> smoothing;
	}
	
	Fcm f {order, smoothing}; 
	double bitsEstimationComp {0};
	double bestEstimation {0};
	string bestLanguage {};
	int init = 0;

	cout << "----------- Starting Language Recognition System ------------- " << endl;	

	// 1º - Determine the model for each modelLanguage 
	// 2º - Estimating the bits needed for compression of the language 
	//		to be found through the respective modelLanguage
	for (string modelLang : modelLanguages)
	{
			cout << "\nCalculating the model and estimated bits for text compression..." << endl;
			cout << "Model Language: " << modelLang << endl;
			bitsEstimationComp = f.nBitsToCompress(modelLang, languageToDiscover);
			cout << "Estimated Bits -> languageToDiscover: " << languageToDiscover << " -> " << bitsEstimationComp << endl;
			if (init == 0)	// Initialization
			{
				bestLanguage = modelLang;
				bestEstimation = bitsEstimationComp;
				cout << "bestLanguage: " << bestLanguage << endl;
				cout << "bestEstimation: " << bestEstimation << endl;
			}else{
				if (bestEstimation > bitsEstimationComp)
				{
					bestLanguage = modelLang;
					bestEstimation = bitsEstimationComp;
					cout << "bestLanguage: " << bestLanguage << endl;
					cout << "bestEstimation: " << bestEstimation << endl;
				}
			}
			init++;
			cout << "________________________________________________________________________________________\n";
	}
	
	cout << "Recognized text language: " << bestLanguage << endl;
	 
	return 0; 
}
