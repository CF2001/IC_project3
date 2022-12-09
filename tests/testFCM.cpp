#include <iostream>

#include "../src/Fcm.cpp"

int main(int argc, char *argv[])
{

	Fcm f {4, 0.1};
	
	map<string,map<char, int>> model;
	double modelEntropy {0};
	
	model = f.createModel(argv[1]);
	cout << "Model Entropy: " << f.modelEntropy(model) << endl;
	//f.printModel(model);
	
	
	
	return 0;
}
