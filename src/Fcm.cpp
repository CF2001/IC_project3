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

vector<char> Fcm::getCharsFromText(string filename)
{
	ifstream file;		// Read from a file 
	file.open(filename);	// opens the file
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
	return info;
}

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
        if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')) //if not blank space
			alphabet.insert(c);	// sem repeticoes
	}
}


map<string,map<char, int>> Fcm::createModel(string filename)
{
	map<string,map<char, int>> model;
	vector<string> contextWithNextChar;

	vector<char> info = getCharsFromText(filename);
	createAlphabet(info);
	contextWithNextChar = getContextWithNextChar(info);
	
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

	vector<char> info = getCharsFromText(filenameT);
	contextWithNextChar = getContextWithNextChar(info);

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

//return estimated bits for given char after given context in the given model
double Fcm::getBitsForChar(int alphabetSIZE, string context, char nextchar, map<string,map<char, int>> modelFCM) {
	int ni  = 0;
	int nctx = 0;
	if (modelFCM.count(context) > 0) //se encontra o contexto no modelo
	{
		//contagem de vezes que apareceu next char, dado que aconteceu o contexto
		if(modelFCM[context].count(nextchar)) {
			ni = modelFCM[context][nextchar];
		}
		//contagem de vezes que apareceu o contexto
		map <char, int> occurCtx = modelFCM[context];
		for (auto i : occurCtx)
		{
			nctx += i.second;
		}

	}
	//numero de bits para codificar o caractere
	return -log2(((ni + alpha) / (nctx + (alpha*alphabetSIZE))));
}

void Fcm::locateLang(vector<string> totalFilesRi, string filenameT)
{
	cout << "creating models..."<<endl;

	vector<map<string,map<char, int>>> totalModels;	// Cada posicao contem M1, M2, ..., Mn
	vector<int> alphabetSizeM;	// tamanho do alfabeto para cada modelo M1, M2, ..., Mn
	vector<vector<double>> probModels;		// [<prob's M1>, <prob's M2>, ..., <prob's Mn>]
	vector<double> probM;

	for (size_t i = 0; i < totalFilesRi.size(); i++)
	{
		//cout << "Languages: " << totalFilesRi[i] << endl;
		totalModels.push_back(createModel(totalFilesRi[i]));	// criar o modelo para o texto respetivo
		alphabetSizeM.push_back(alphabet.size());
	}
	cout << "Created Models !" << endl;
	cout << "nModels: " << totalModels.size() << endl;
	
	//inicialization
	vector<char> info = getCharsFromText(filenameT);
	vector<string> contextWithNextChar = getContextWithNextChar(info);
	double bits {};
	int sensit = 3; //fator determinante do quão sensível o algoritmo é a mudanças de língua
	double curMedia = 10000; //big number
	int curLang = 0, prevLang = 0;

	double values[totalModels.size()][sensit] {}; //modelo X char(sensitivity)
	double media[totalModels.size()] {};
	
	cout << "Searching..." << endl;
	//percorrer os caracteres
	for (size_t s = 0; s < contextWithNextChar.size(); s++) { // k a size
		cerr << contextWithNextChar[s] << " | ";
		// Percorrer todos os modelos
		for (size_t m = 0; m < totalModels.size(); m++) {
			bits = getBitsForChar(alphabetSizeM[m], contextWithNextChar[s].substr(0,k), contextWithNextChar[s].substr(k,k)[0], totalModels[m]);
			
			//determinação do valor médio de bits
			//cout << "s: " << s << endl;
			if(s < sensit) {
				values[m][s] = bits;
				media[m] += bits/sensit;
			}
			else{
				//media - old av bits + new av bits
				media[m] = media[m] + bits/sensit - values[m][s%sensit]/sensit;
				values[m][s%sensit] = bits;
			}
			//getchar();

			//comparação para ter a menor média
			if(media[m] < curMedia && m != prevLang) {
				cout << endl;
				for(int i = 0; i<totalModels.size(); i++)
					cout << i <<": " << media[i] << " | ";
				cout << "\n" << media[m] << " < " << curMedia << endl;
				cout << m << " < " << curLang << endl;
				getchar();
				curMedia = media[m];
				curLang = m;
			}
			//cout << "curMedia: " << curMedia << endl;
			//getchar();
		}
		//analizemos o resultado
		if(prevLang != curLang){
			cout << "Language changed from: " << totalFilesRi[prevLang] << " to: " << totalFilesRi[curLang] << endl;
			getchar();
			prevLang = curLang;
		}

	}
}

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
