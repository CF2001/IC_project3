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
	*	@brief	get the order of the model
	*/
	int getOrder();
	

	/**
	*	@brief	set the order of the model
	*	@param 	k ordem of the model
	*/
	void setOrder(int k);


	/**
	*	@brief	Returns the total number of different letters in a given language
	*	@param	info	all characters of the text
	*/
	void createAlphabet(vector<char> info);


	/**
	*	@brief 	Reads all the characters from the text file and puts them in a vector
	*	@param	filename text file to read
	*/
	vector<char> getCharsFromText(string filename);


	/**
	*	@brief	Returns all contexts concatenated with their next characters
	*	@param	info vector with all characters read from the text
	*/
	vector<string> getContextWithNextChar(vector<char> info);
	
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
	*	@brief	Estimates the number of bits required for compression of a text file
	*	@param	filenameRi	text file for creating the fcm model
	*	@param	filenameT	text file for analysis
	*/
	double nBitsToCompress(string filenameRi, string filenameT);

	
	/**
	*	@brief	Locates the position of different languages in a text file
	*	@param	totalFilesRi
	* 	@param	filenameT		text file for analysis
	* 	@param	sensit
	* 	@param	verb	
	*/
	map<int, string> locateLang(vector<string> totalFilesRi, string filenameT, int sensit, bool verb);

	/**
	*	@brief	Gets the number of bits needed to compress a given character
	*	@param	alphabetSIZE	number of different characters of a language
	*	@param	context			respective context of the text to be analyzed
	*	@param	nextchar		next character to the context of the text to analyze
	*	@param	modelFCM		fcm model of the other text
	*/
	double getBitsForChar(int alphabetSIZE, string context, char nextchar, map<string,map<char, int>> modelFCM);
	

	/**
	*	@brief	
	*	@param 	totalFilesRi
	*/
	void statisticsModelCreation(vector<string> totalFilesRi);


	/**
	*	@brief	File size in bits
	*	@param 	filename	text file 
	*/
	int getSizeFile(string filename);


	/**
	*	@brief	Name of the language in which the text is written
	*	@param 	languagePath	path of the text file 
	*/
	string languageName(string languagePath);

	/**
	*	@brief	Stores a language model in a text file
	*	@param	model		data structure of the model fcm
	*	@param	inFilename	text file from which the model is built
	*	@param	outFilename	text file where you write the model
	*/
	void saveModelToFile(map<string,map<char, int>> &model, string inFilename, string outFilename);
	
};
