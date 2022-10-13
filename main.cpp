#include <fstream>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
static const unsigned char primeNumber =  13;

// otwiera, czyta i zapisuje plik do wektora
bool getFileContent(const string &fileName, vector<char> &container)
{
	ifstream file(fileName, ios::in|ios::binary);
	file.unsetf(ios::skipws);
	 
	if (!file) 
	{
		cout << "File open error" <<  fileName << endl;
		return false;
	}
	file.seekg (0 , ios::end);
	size_t lenOfFile = file.tellg();
	file.seekg (0 , ios::beg);    
	container.reserve(lenOfFile);
	 
	for(;;)
	{  	
		container.push_back(file.get()); 		
		--lenOfFile;
		if (lenOfFile == NULL)
			 break;
	 }
	 file.close();
	 return true;	
}

bool saveFileContent(const string &fileName, const vector<char> &container)
{
	ofstream file(fileName, ios::out|ios::binary);
	if (!file) 
 	{	
 		cout << "File creation error" <<  fileName << endl;
 		return false;
 	}
 	
	for (size_t i = 0; i < container.size(); ++i)
 	{
 		file << container[i]; 
 	}
 
 	file.flush();
 	file.close(); 

 	return true;	
}

// ---------------------------------------- kontrola parzystosci --------------------------------------------
char calculateParity(char c) 
{ 
	/* sumujemy ilosc jedynek w bajcie */
	/* po calej dlugosci znaku (w sensie po bitach w nim) sprawdzamy po kolei czy bit jest wiekszy od zera (czyli rowny 1)
	jesli tak to zwiekszamy counter o 1. potem dzielimy na dwa i wychodzi 1 albo 0*/ 
	int counter = 0; 
 	for (size_t i = 0; i < sizeof(char); ++i)
 	{
        if ( (c >> i) &  (0x01 > 0))
        {
        counter++;
        }
 	}  	
 	return counter%2;
}

bool checkParity(vector<char> &container)
{
	for (size_t i = 0; i < container.size(); i +=2)
 	{
 		char data = container[i];
 		char parity = container[i+1];  					
 		if (parity != calculateParity(data)) 			 	
 			return false;  		
 	}
 	return true;
}

// ---------------------------------------- suma modulo --------------------------------------------

char calculateControlSum(unsigned char c) 
{ 	
	/* po calej dlugosci znaku sprawdzamy po kolei czy bit jest wiekszy od zera (czyli rowny 1)
	jesli tak to zwiekszamy sumę o 2^i. potem dzielimy przez jakas liczbe pierwsza i nasz wynik jest suma kontrolna */ 
	int sum = 0;
	for (size_t i = 0; i < sizeof(char); ++i)
	{
		if ((c >> i) & 0x01)
		{
		sum +=  2^i;
		} else 
		{
		sum +=  0;
		}
	}
	return sum % primeNumber;	
}

bool checkControllSum(vector<char> &container)
{
	for (size_t i = 0; i < container.size(); i += 2)
	{
	 	char data = container[i];
	 	char ctrlSum = container[i+1]; 
	 	if (ctrlSum != calculateControlSum(data))
	 		return false; 
	}
	return true;
}

int main(int argc, char** argv)
{
        string fileName = "//test.bin"; 
	string withParityFileName = "//testParity.bin";
	string withModuloFileName = "//testModulo.bin";
        vector<char> fileData;

// -------------------- Przygotowanie daych, liczenie sum kontrolnych i zapisywanie do pliku ----------------------------------------
        if (!getFileContent(fileName, fileData))
		return -1;	

// kontrola parzystosci
        vector<char> withParityCheck;
        withParityCheck.reserve(fileData.size()*2);
        for(size_t i = 0; i < fileData.size(); ++i)
        {
                withParityCheck.push_back(fileData[i]);
                withParityCheck.push_back(calculateParity(fileData[i])); 
        }  	 
        if (!saveFileContent(withParityFileName, withParityCheck)) 		
                return -1;


        // kontrola modulo
        vector<char> withModuloCheck;
        withModuloCheck.reserve(fileData.size()*2);
        for(size_t i = 0; i < fileData.size(); ++i)
        {
                withModuloCheck.push_back(fileData[i]);
                withModuloCheck.push_back(calculateControlSum(fileData[i])); 
        }  	 
        if (!saveFileContent(withModuloFileName, withModuloCheck))
                return -1;    
                

// -------------------- Odczyt  daych binarnych, kontrola sum -----------------------------------------------

        vector<char> binaryData;	

// kontrola parzystosci
        if (!getFileContent(withParityFileName, binaryData))
                return -1;
                                                
        if (!checkParity(binaryData))	
                cout << "Parity control sum FAIL for " << withParityFileName << endl;	
        else 	
                cout << "Parity control sum OK for " << withParityFileName << endl;	
                
        binaryData.clear();



// kontrola modulo
        if (!getFileContent(withModuloFileName, binaryData))
                return -1;			
	
        if (!checkControllSum(binaryData))
                cout << "Modulo control sum FAIL for " << withModuloFileName << endl;
        else 	
                cout << "Modulo control sum OK for " << withModuloFileName << endl;	

        binaryData.clear();

return 0;
}
