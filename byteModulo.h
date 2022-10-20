#ifndef BYTEMODULO_H
#define BYTEMODULO_H

#include <math.h>
#include <string>
#include <vector>
#include <bitset>
#include <insertErrors.h>
#include <fileHandler.h>

using namespace std;

//======================================================================================================
//                                  suma kontrolna modulo wyliczanie
//======================================================================================================
char calculateControlSum(char c)
{
    char primeNumber =  13;
    int sum = 0;
    for (size_t i = 0; i < sizeof(char); ++i)
    {
        if ((c >> i) & (0x01 > 0))
        {
        sum +=  2^i;
        } else
        {
        sum +=  0;
        }
    }
    return sum % primeNumber;
}

//======================================================================================================
//                               suma kontrolna modulo na calym pliku
//======================================================================================================
int moduloControlByByte (vector<char> &fileData, string withModuloFileName, string fileName, double errorPercentage, int errorRepetition, bool repetitionEnabled)
{
        if (!getFileContent(fileName, fileData))
            {return -1;}

// robie miejsce na dane + sume kontrolna
        vector<char> withModuloCheck;
        withModuloCheck.reserve(fileData.size()*2);

// wyliczam sume kontrolna na poprawnych danych
        for(size_t i = 0; i < fileData.size(); ++i)
        {
            withModuloCheck.push_back(fileData[i]);
            withModuloCheck.push_back(calculateControlSum(fileData[i]));
        }

// dodaje bledy do danych i nadpisuje
        withModuloCheck = addErrorsB(errorRepetition, errorPercentage, withModuloCheck, repetitionEnabled);

        if (!saveFileContent(withModuloFileName, withModuloCheck))
                {return -1;}

        fileData.clear();

        vector<char> binaryData;
        if (!getFileContent(withModuloFileName, binaryData))
                {return -1;}

        int check = 0;
        for (size_t i = 0; i < binaryData.size(); i += 2)
        {
            char data = binaryData[i];
            char contSum = binaryData[i+1];
            if (contSum != calculateControlSum(data))
                {++check;}
        }

        if (check != 0)
                cout << "Modulo control sum FAIL" << endl;
        else
                cout << "Modulo control sum PASS" << endl;

        binaryData.clear();

        return 0;
}

#endif // BYTEMODULO_H
