#include <iostream>
#include <string>
#include <fstream>

#include "../src/Fcm.cpp"


int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cerr << "Usage: ./testFCM <inputFile> <outputFile> <order> \n";
		return 1;
	}
	
	string inputFile = argv[argc-3];
	string outputFile = argv[argc-2];
	int order = atoi(argv[argc-1]);	
	double smoothing = 0.1; 	// just initialization

	Fcm f {order, smoothing}; 
	map<string,map<char, int>> model;
	double modelEntropy {0};

	// Criar modelo FCM 	
	model = f.createModel(inputFile);
	// Guardar modelo FCM num ficheiro de texto
	f.saveModelToFile(model, inputFile, outputFile);
	// Determinar entropia do modelo 
	modelEntropy = f.modelEntropy(model);
	cout << "Model Entropy: " <<  modelEntropy << endl;

	
	// Criar o grafico da evolução da entropia em função da ordem k
	/*
	ofstream ofs { (char*)"entropyStudy.txt" };
	for (size_t k = 1; k < 21; k++)
	{
		f.setOrder(k);
		model = f.createModel(inputFile);
		ofs << k << "  " << f.modelEntropy(model) << endl;
	}
	*/
	 
	return 0; 
}
