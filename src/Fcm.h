#include <iostream>
#include <string>
#include <map>  
#include <vector>
#include<set>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <math.h>

using namespace std;

class Fcm {

	private:
		int k;
		double alpha;
		set <char> alphabet;
		
	public: 

	/**
	*	FCM class constructor
	* 	@param k is the order of the model
	*	@param alpha is the smoothing parameter
	*/
	Fcm(int k, double alpha);
	
	/**
	*	@bried Creates the FCM model
	*	@param inputfname text file from which the FCM is created
	*/
	map<string,map<char, int>> createModel(string inputfname);
	
	/**
	*	@brief Determines th entropu model 
	*	@param model data structure of the model for determining entropy
	*/
	double modelEntropy(map<string,map<char, int>> &model); 

	/**
	*	@brief	
	*	@param	
	*/
	double nBitsToCompress(string filenameRi, string filenameT);


	void saveModelToFile(map<string,map<char, int>> &model, string filename);
	
	/**
	*	@brief	
	*/
	void printModel(map<string,map<char, int>> &model);
};