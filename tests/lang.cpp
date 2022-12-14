#include <iostream>
#include <string>

#include "../src/Fcm.cpp"

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		cerr << "Usage: ./lang <modelFile> <testFile> <order> <smoothing>\n";
		return 1;
	}
	
	string modelFile = argv[argc-4];
	string testFile = argv[argc-3];
	int order = atoi(argv[argc-2]);	
	double smoothing = atof(argv[argc-1]); 

	
	Fcm f {order, smoothing}; 
	double bitEstimationComp {0};

	// Estimar o número bits para a compressao do testFile em funcao do modelo modelFile
	bitEstimationComp = f.nBitsToCompress(modelFile, testFile);
	cout << "Required bits for compression: " << bitEstimationComp << endl; 
	//f.printModel(model);
	 
	return 0; 
}
