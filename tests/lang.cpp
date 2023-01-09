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

	// Tamanho em bits do ficheiro original
	cout<<"Size of the original file is " << f.getSizeFile(testFile)*8 << " bits" << endl;
	
	// Estimar o número bits para a compressao do ficheiro original (testFile)
	//  em função do modelo criado pelo ficheiro modelFile
	bitEstimationComp = f.nBitsToCompress(modelFile, testFile);
	cout << "Required bits for compression: " << bitEstimationComp << endl; 
	 
	return 0; 
}
