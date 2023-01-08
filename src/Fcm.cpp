#include <iostream>
#include <string>
#include <map>  
#include <vector>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <numeric>

#include "Fcm.h"

#define positiveInfinite 1.5935e250*7.948e110

using namespace std;

Fcm::Fcm(int k, double alpha)
{
	this->k = k;
	this->alpha = alpha; 
}

void Fcm::createAlphabet(vector<char> info)
{
	for (char c : info)
	{
        if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')) //if not blank space
			alphabet.insert(c);	// sem repeticoes
	}
}

vector<char> Fcm::getCharsFromText(string filename)
{
	ifstream file;		// Read from a file 
	file.open(filename);	// opens the file
	if(!file) { 		// file couldn't be opened
      		cout << "Error: file " << filename << " could not be opened" << endl;
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
			if (mychar == '\n' || mychar == '\t' || mychar == '\n' || mychar == '\r' || mychar == '\f' || mychar == '\v')	// eliminate paragraphs
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

void Fcm::statisticsModelCreation(vector<string> totalFilesRi) {
	cout << "\nCreating models statistics (may vary from OS to OS and how busy the system is)"<<endl;

	clock_t start1, end1;
	double elapsed = 0, time;
	int count = 0;
	char* f;
	for (size_t i = 0; i < totalFilesRi.size(); i++)
	{
		cout << "\nProcessing: \n\t" << totalFilesRi[i] << endl;

		//open file
		f = &*totalFilesRi[i].begin(); //convert from string to c*
		FILE* file = fopen(f,"r");
		if(file == NULL) {
      		cout << "Error: file " << totalFilesRi[i] << " could not be opened" << endl;
		}

		//get size of file
		count = 0;
		while(fgetc(file) != EOF) count++;
		cout << "Size of file: " << count << " bytes" << endl;

		//get time to compress
		start1 = clock();
		createModel(totalFilesRi[i]);
		end1 = clock();
		time = double(end1 - start1)/CLOCKS_PER_SEC;
		elapsed += time;
		cout << "Alphabet Size: " << alphabet.size() << " characters" << endl;
		cout << "Elapsed Time: " << time << " seconds" << endl;
		cout << "Processed: " << count/time/pow(10,6) << " megabytes per second" << endl;

		fclose(file);
	}
	cout << "\nTotal of " << totalFilesRi.size() << " models created in " << elapsed << " seconds." << endl;
}

map<int, string> Fcm::locateLang(vector<string> totalFilesRi, string filenameT, int sensitivity, bool verbose)
{
	//declaração e inicialização
	vector<map<string,map<char, int>>> totalModels;	// Cada posicao contem M1, M2, ..., Mn
	vector<int> alphabetSizeM;	// tamanho do alfabeto para cada modelo M1, M2, ..., Mn
	vector<vector<double>> probModels;		// [<prob's M1>, <prob's M2>, ..., <prob's Mn>]
	vector<double> probM;

	vector<char> info = getCharsFromText(filenameT);
	vector<string> contextWithNextChar = getContextWithNextChar(info);
	double curAverage = positiveInfinite;
	int curLang = 0, prevLang = -1;
	map<int, string> langChanges {};

	int ni, nctx;
	map <char, int> occurCtx;
	string context;
	char nextchar;
	double bits;

	clock_t start1, end1;
	double elapsed = 0;

	//criação dos modelos
	if(verbose) cout << "\ncreating models..."<<endl;
	for (size_t i = 0; i < totalFilesRi.size(); i++)
	{
		totalModels.push_back(createModel(totalFilesRi[i]));	// criar o modelo para o texto respetivo
		alphabetSizeM.push_back(alphabet.size());
		if(verbose) cout << "created model for " << totalFilesRi[i] << endl;
	}
	//declaração janela da média
	double averageWindow[totalModels.size()][sensitivity] {}; //modelo X char(sensitivity)
	double average[totalModels.size()] {};
	
	
	//início deteção do idioma
	if(verbose) cout << "\nLocating languages..." << endl;
	start1 = clock();
	//percorrer todos os contextos com caracteres
	for (size_t s = 0; s < contextWithNextChar.size(); s++) { // k a size
		//if(verbose) cout << "|" << contextWithNextChar[s] << "|";
		// Percorrer todos os modelos
		for (size_t m = 0; m < totalModels.size(); m++) {

			//cáculo dos bits (get bits for char)
			ni  = 0;
			nctx = 0;
			context = contextWithNextChar[s].substr(0,k);
			nextchar = contextWithNextChar[s].substr(k,k)[0];
			if (totalModels[m].count(context) > 0) //se encontra o contexto no modelo
			{
				//contagem de vezes que apareceu next char, dado que aconteceu o contexto
				if(totalModels[m][context].count(nextchar)) {
					ni = totalModels[m][context][nextchar];
				}
				//contagem de vezes que apareceu o contexto
				occurCtx = totalModels[m][context];
				for (auto i : occurCtx)	nctx += i.second;
			}
			//numero de bits para codificar o caractere
			bits = -log2(((ni + alpha) / (nctx + (alpha*alphabetSizeM[m]))));
			//cout << "model: " << m << "| bits: " << bits << endl;

			//atualização da janela de sensibilidade
			if(s < sensitivity) {
				average[m] += bits/((float)sensitivity);
				averageWindow[m][s] = bits;
			}else{
				//media - old av bits + new av bits
				average[m] = average[m] + bits/((float)sensitivity) - averageWindow[m][s%sensitivity]/((float)sensitivity);
				averageWindow[m][s%sensitivity] = bits;
			}


			//comparação para ter a menor media
			if(average[m] < curAverage && m != prevLang) {
				curAverage = average[m];
				curLang = m;
			}
		}

		//cout << "curLang: " << curLang << " | curAverage: " << curAverage << endl;
		//cout << "prevLang: " << prevLang;
		//if(prevLang != -1) cout << " | prevAverage: " << average[prevLang];
		//cout << endl;

		//análise do resultado
		if(prevLang != curLang){
			if(prevLang != -1) {
				cout << "\nLanguage changed from: " << totalFilesRi[prevLang] << " to: " << totalFilesRi[curLang] << " at position " <<  s << endl;
				cout << "averages comparison:" << average[curLang] << " < " << average[prevLang] << endl;
				cout << "context of change: " << contextWithNextChar[s] << endl;
				langChanges.insert({ s , totalFilesRi[curLang] });
			}
			else {
				cout << "\nLanguage at start is " << totalFilesRi[curLang] << endl;
				langChanges.insert({ 0 , totalFilesRi[curLang] });
			}
			prevLang = curLang;
		}
	}
	end1 = clock();
	elapsed = double(end1 - start1)/CLOCKS_PER_SEC;
	if(verbose) cout << "\ndone in: "  << elapsed << " seconds \n" <<
	"average context processing time: "<< elapsed/contextWithNextChar.size()*1000 << " miliseconds" << endl;

	return langChanges;
}


int Fcm::getSizeFile(string filename)
{
	ifstream in_file(filename, ios::binary);
   	in_file.seekg(0, ios::end);
   	int file_size = in_file.tellg();

  	return file_size;
}


string Fcm::languageName(string languagePath)
{
	string langName {};
	int count {0};

	for (int c = 0; c < languagePath.size()-1; c++)
	{
		if (languagePath[c] == '/')
		{
			count++;
		}

		if (count == 2)
		{
			langName += languagePath[c];
		}
	}
	return langName.substr(1, langName.size());
}

void Fcm::saveModelToFile(map<string,map<char, int>> &model, string inFilename, string outFilename)
{
	ofstream ofs { outFilename };
	ofs << "Modelo criado para o ficheiro de texto: " << inFilename << "\n" << endl;

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
