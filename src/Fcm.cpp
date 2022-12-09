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

map<string,map<char, int>> Fcm::createModel(string inputfname)
{
	map<string,map<char, int>> model;
	vector<string> contextWithNextChar;

	ifstream file;		// Read from a file 
	file.open(inputfname);	// opens the file
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
	
	string context {};
	int nOccurrences {0};
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


void Fcm::printModel(map<string,map<char, int>> &model)
{
	for (auto it : model) {
	cout << "{\"" << it.first << "\" :";
	map<char, int> &internal_map = it.second;
	    for (auto it2: internal_map) {
	        cout << " {'" << it2.first << "' : " << it2.second << "}";
	    }
 	cout << "}" << endl;

 	}
	/*
	//cout << "Context" << " next Char " << " nOcurrences " << endl;	
	for (auto m : model)
	{
		    cout << m.first << "-> ";
			for (auto occurNextChar : model[m.first])
			{
				cout << " {" << occurNextChar.first << ", " << occurNextChar.second << "}";	
			}
			cout << endl;
	}	*/
}

