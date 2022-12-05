#include <iostream>
#include <map>  
#include <fstream>

class fcm {

	private:
		int k;
		double alpha;
		
	public: 

	fcm(int modelK, double smoothingAlpha)
	{
		k = modelK;
		alpha = smoothingAlpha;
	}
	
	map<string,map<char, int>> createModel(string fname)
	{
		// Read from a file 
		fstream file = fstream(fname, ios::in);
		
	}
	
};
