#include <iostream>
#include <string>
#include <map>  
#include <vector>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <math.h>

#include "Fcm.h"

using namespace std;


Fcm::Fcm(int k, double alpha)
{
	this->k = k;
	this->alpha = alpha; 
}

// Obtain all the characters in the file to a vector<char>
vector<char> Fcm::getCharsFromText(string filename)
{
	vector <char> info;

	ifstream file;		// Read from a file 
	file.open(filename);	// opens the file
	if(!file) { 		// file couldn't be opened
      		cerr << "Error: file could not be opened" << endl;
      		exit(1);
  	}

	if (file.is_open()) {
		char mychar;
		while (file) {
			mychar = file.get();
			// descartar todos os caracteres especiais
			//if ( ((mychar >= 'A' && mychar <= 'Z') || (mychar >='a' && mychar <= 'z')))
			//info.push_back(tolower(mychar));	
			if (mychar == '\n')	// eliminate paragraphs
			{
				info.push_back(' ');	
			}else{
				info.push_back(mychar);	
			}
		}
	}
	info.pop_back();	// remove the eof

	return info;
}


// Determine all the contexts+nextChar
vector<string> Fcm::getContextWithNextChar(vector<char> info)
{

	vector<string> contextWithNextChar;
	string contextNextChar {};
	// Determine all the contexts+nextChar
	for (size_t idx = 0; idx < info.size(); idx++)	// percorre todos os caracteres
	{
		//cout << "char: " << info[idx] << endl;
		for (size_t c = idx ; c < (k+1+idx); c++)
		{	
			//cout << "c: " << c << endl;
			contextNextChar += info[c];	
		}
		//cout << contextNextChar << endl;
		contextWithNextChar.push_back(contextNextChar);
		contextNextChar = "";
	}
	return contextWithNextChar;
}

void Fcm::createAlphabet(vector<char> info)
{
	for (char c : info)
	{
		if ( ((c >= 'A' && c <= 'Z') || (c >='a' && c <= 'z')))
			alphabet.insert(c);	// sem repeticoes
	}
}

map<string,map<char, int>> Fcm::createModel(string filename)
{
	map<string,map<char, int>> model;
	vector<char> info = getCharsFromText(filename);
	createAlphabet(info);
	vector<string> contextWithNextChar = getContextWithNextChar(info);
	
	string context {};
	char nextchar {};
	for (size_t s = 0; s < contextWithNextChar.size(); s++)
	{
		//cout << "inicial_contextWithNextChar: " << contextWithNextChar[s] << endl;
		context = contextWithNextChar[s].substr(0,k);
		//cout << "context: " << context << endl;
		nextchar = contextWithNextChar[s].substr(k,k)[0];
		//cout << "nextchar: " << nextchar << endl;
		
		if (model.count(context) > 0)
		{
			//cout << "context already exist" << endl;
			model[context][nextchar]++;
		}else{
			//cout << "NAO EXISTE" << endl;
			map<char, int> empty;
    		model[context] = empty;
    		model[context][nextchar]++;
		}
	}	
	return model;	
}

double Fcm::modelEntropy(map<string,map<char, int>> &model)
{
	double H {0};
	double Pctx {0};
	double Hctx {0};	
	map<string,double> Pctx_tmp;
	map<string,double> Hctx_tmp;
	
	double probTmp {0};
	map<string,int> nctxNextchar;
	int nOccurAllctx {0};
	int tmp {0};
	
	for (auto m : model)	// model -> < context, <{nextchar, nOccurrences}, {...}> >
	{
		tmp = 0;
		for (auto i : model[m.first])	// occurrences ctx nextchar -> {nextchar, nOccurrences}, {...}
			tmp += i.second;
		// Determine the number of times all context nextchar appear
		nctxNextchar[m.first] = tmp;
		// Determine the number of times the context appear
		nOccurAllctx += tmp;
	}
	
	// Calculate the context probability and context Entropy
	for(auto m : model){
		Pctx_tmp[m.first] = (double)nctxNextchar[m.first] / nOccurAllctx;
	 
	    for(auto occurnextchar : model[m.first]){
	        probTmp = (double) occurnextchar.second / nctxNextchar[m.first];
	        Hctx_tmp[m.first] -= probTmp * log2(probTmp);
	      
	    }
	} 
	
	// Calcule model Entropy
	for (auto m : model)
	{
		Pctx = Pctx_tmp[m.first];
		Hctx = Hctx_tmp[m.first];
		H += (Pctx * Hctx);
	}
	return H;
} 


//double Fcm::nBitsToCompress(map<string,map<char, int>> &model, string filename)
double Fcm::nBitsToCompress(string filenameRi, string filenameT)
{
	map<string,map<char, int>> modelFCM = createModel(filenameRi);
	
	vector<string> contextWithNextChar;
	int alphabetSIZE = alphabet.size();
	double nbitsToCompress {0};	

	//Fcm::printModel(modelFCM);

	ifstream file;		// Read from a file 
	file.open(filenameT);	// opens the file
	if(!file) { 		// file couldn't be opened
      		cerr << "Error: file could not be opened" << endl;
      		exit(1);
  	}

	vector <char> info;
  	// Obtain all the characters in the file to a vector<char>
  	if (file.is_open()) {
		char mychar;
		while (file) {
			mychar = file.get();
			// descartar todos os caracteres especiais
			//if ( ((mychar >= 'A' && mychar <= 'Z') || (mychar >='a' && mychar <= 'z')))
			//info.push_back(tolower(mychar));	
			if (mychar == '\n')	// eliminate paragraphs
			{
				info.push_back(' ');	
			}else{
				info.push_back(mychar);	
			}
		}
	}
	info.pop_back();	// remove the eof

	
	string contextNextChar {};
	// Determine all the contexts+nextChar
	for (size_t idx = 0; idx < info.size(); idx++)	// percorre todos os caracteres
	{
		//cout << "char: " << info[idx] << endl;
		for (size_t c = idx ; c < (k+1+idx); c++)
		{	
			//cout << "c: " << c << endl;
			contextNextChar += info[c];	
		}
		//cout << contextNextChar << endl;
		contextWithNextChar.push_back(contextNextChar);
		contextNextChar = "";
	}

	//cout << "alphabetSIZE: " << alphabetSIZE << endl;
	
	string context {};
	char nextchar {};
	double prob {0};
	int ni {0};
	int nctx {0};

	
	for (size_t s = 0; s < contextWithNextChar.size(); s++)
	{
		//cout << "inicial_contextWithNextChar: " << contextWithNextChar[s] << endl;
		context = contextWithNextChar[s].substr(0,k);
		//cout << "contextINIT: " << context << endl;
		nextchar = contextWithNextChar[s].substr(k,k)[0];
		//cout << "nextchar: " << nextchar << endl;
		
		ni  = 0;
		nctx = 0;
		if (modelFCM.count(context) > 0)
		{
			//cout << "_____contextENTROU______" << endl;
			ni = modelFCM[context][nextchar];
			//cout << "ni: " << ni << endl;
			
			map <char, int> occurCtx = modelFCM[context];
			for (auto i : occurCtx)
			{
				nctx += i.second;
			}
			//cout << "nctx: " << nctx << endl;
		}

		prob += -log2(((ni + alpha) / (nctx + (alpha*alphabetSIZE))));
		//cout << "prob" << prob << endl;
	}
		
	nbitsToCompress = prob;
	//cout << "n bits para compressao: " << nbitsToCompress << endl; 
	
	return nbitsToCompress;
}

/*
double Fcm::locateLang(vector<string> totalFilesRi, string filenameT)
{

	vector<map<string,map<char, int>>> totalModels;
	vector<int> alphabetSize;
	for (size_t i = 0; i < totalFilesRi.size(); i++)
	{
		totalModels.push_back(createModel(totalFilesRi[i]));	// criar o modelo para o texto respetivo
		alphabetSize.push_back(alphabet.size());
	}
	//map<string,map<char, int>> modelFCM = createModel(filenameRi);
	
	vector<string> contextWithNextChar;
	int alphabetSIZE = alphabet.size();
	double nbitsToCompress {0};	

	//Fcm::printModel(modelFCM);

	ifstream file;		// Read from a file 
	file.open(filenameT);	// opens the file
	if(!file) { 		// file couldn't be opened
      		cerr << "Error: file could not be opened" << endl;
      		exit(1);
  	}

	vector <char> info;
  	// Obtain all the characters in the file to a vector<char>
  	if (file.is_open()) {
		char mychar;
		while (file) {
			mychar = file.get();
			// descartar todos os caracteres especiais
			//if ( ((mychar >= 'A' && mychar <= 'Z') || (mychar >='a' && mychar <= 'z')))
			//info.push_back(tolower(mychar));	
			if (mychar == '\n')	// eliminate paragraphs
			{
				info.push_back(' ');	
			}else{
				info.push_back(mychar);	
			}
		}
	}
	info.pop_back();	// remove the eof

	
	string contextNextChar {};
	// Determine all the contexts+nextChar
	for (size_t idx = 0; idx < info.size(); idx++)	// percorre todos os caracteres
	{
		//cout << "char: " << info[idx] << endl;
		for (size_t c = idx ; c < (k+1+idx); c++)
		{	
			//cout << "c: " << c << endl;
			contextNextChar += info[c];	
		}
		//cout << contextNextChar << endl;
		contextWithNextChar.push_back(contextNextChar);
		contextNextChar = "";
	}

	//cout << "alphabetSIZE: " << alphabetSIZE << endl;
	
	string context {};
	char nextchar {};
	double probModel {0};
	//int ni {0};
	//int nctx {0};
	for (size_t s = 0; s < contextWithNextChar.size(); s++)
	{
		//cout << "inicial_contextWithNextChar: " << contextWithNextChar[s] << endl;
		context = contextWithNextChar[s].substr(0,k);
		//cout << "contextINIT: " << context << endl;
		nextchar = contextWithNextChar[s].substr(k,k)[0];
		//cout << "nextchar: " << nextchar << endl;

		probBits()
		
	}
	return nbitsToCompress;
}
*/

/*
double probBits(int alphabetSIZE, string context, char nextchar, map<string,map<char, int> modelFCM)
{
	double prob {0};
	int ni {0};
	int nctx {0};
	if (modelFCM.count(context) > 0)
	{
		//cout << "_____contextENTROU______" << endl;
		ni = modelFCM[context][nextchar];
		//cout << "ni: " << ni << endl;
		
		map <char, int> occurCtx = modelFCM[context];
		for (auto i : occurCtx)
		{
			nctx += i.second;
		}
		//cout << "nctx: " << nctx << endl;
	}

	prob = -log2(((ni + alpha) / (nctx + (alpha*alphabetSIZE))));
	//cout << "prob" << prob << endl;

	return prob; 
}
*/

void Fcm::saveModelToFile(map<string,map<char, int>> &model, string filename)
{
	ofstream ofs { filename };

	for (auto [ctx, counter]  : model)
	{
		ofs << "{\"" << ctx << "\" : ";
		for (auto [newchar, noccur] : counter)
		{
			ofs << " { '" << newchar << "' : " << noccur << " }";
			//ofs << "{ '" << (int)noccur << " }";
		}	
		ofs << "}" << endl; 
	}
	
}
