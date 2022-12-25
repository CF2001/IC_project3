#include <iostream>
#include <string>
#include <vector>

#include "../src/Fcm.cpp"

int main(int argc, char *argv[])
{
	/*
	// A minimum of 10 model texts is required
	if (argc < 12)
	{
		cerr << "Usage: ./lang <modelFile1> <...> <modelFile10> <languageToDiscover>\n";
		return 1;
	}	
	*/
	
	string languageToDiscover = argv[argc-1];	// texto com varias linguas
	vector<string> modelLanguages;
	for (int l = 1; l < argc-1; l++)
	{
		modelLanguages.push_back(argv[l]);
	}

	int order = 6;	
	double smoothing = 0.1; 

	/*
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
	*/

	Fcm f {order, smoothing}; 
	//f.statisticsModelCreation(modelLanguages);
	f.locateLang(modelLanguages, languageToDiscover);

}
