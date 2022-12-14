#include <iostream>
#include <string>

#include "../src/Fcm.cpp"

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cerr << "Usage: ./testFCM <inputFile> <order> <outputFile>\n";
		return 1;
	}
	
	string inputFile = argv[argc-3];
	int order = atoi(argv[argc-2]);	
	string outputFile = argv[argc-1];
	double smoothing = 0.1; 	// just initialization

	Fcm f {order, smoothing}; 
	map<string,map<char, int>> model;
	double modelEntropy {0};

	// Criar modelo FCM 	
	model = f.createModel(inputFile);

	// Guardar modelo FCM num ficheiro de texto
	f.saveModelToFile(model, outputFile);
	
	// Determinar entropia do modelo 
	modelEntropy = f.modelEntropy(model);
	cout << "Model Entropy: " <<  modelEntropy << endl;
	 
	return 0; 
}
