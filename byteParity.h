#ifndef BYTEPARITY_H
#define BYTEPARITY_H

#include <math.h>
#include <string>
#include <vector>
#include <bitset>
#include <stdio.h>
#include <insertErrors.h>
#include <fileHandler.h>

using namespace std;

//======================================================================================================
//                                    bit parzystosci wyliczanie
//======================================================================================================
char calculateParity(char c)
{
     /*sprawdzamy czy bit w charze jest wiekszy od zera (czyli rowny 1)
    jesli tak to zwiekszamy counter o 1. potem dzielimy na dwa i wychodzi reszta 1 albo 0 */

    int counter = 0;
    for (size_t i = 0; i < sizeof(c); ++i)
    {
        if ((c >> i) & (0x01 > 0))
        {
        counter++;
        }
    }
    return counter%2;
}

//======================================================================================================
//                                bit parzystosci po kazdym bajcie
//======================================================================================================
int parityControlByByte(vector<char> &fileData, string withParityFileName, string fileName, double errorPercentage, int errorRepetition, bool repetitionEnabled)
{
        if (!getFileContent(fileName, fileData))
            {return -1;}

// robie miejsce na dane + sume kontrolna
        vector<char> withParityCheck;
        withParityCheck.reserve(fileData.size()*2);

// wyliczam sume kontrolna na poprawnych danych

        for(size_t i = 0; i < fileData.size(); ++i)
        {
            withParityCheck.push_back(fileData[i]);
            withParityCheck.push_back(calculateParity(fileData[i]));
        }

// dodaje bledy do danych i nadpisuje
        withParityCheck = addErrorsB(errorRepetition, errorPercentage, withParityCheck, repetitionEnabled);

        if (!saveFileContent(withParityFileName, withParityCheck))
                {return -1;}

        fileData.clear();

        vector<char> binaryData;
        if (!getFileContent(withParityFileName, binaryData))
                {return -1;}

// sprawdzam zgodnosc sumy kontrolnej odczytanej z co drugiego elementu wektora i tej wyliczonej na nowo
        int check = 0;
        for (size_t i = 0; i < binaryData.size(); i += 2)
        {
            char data = binaryData[i];
            char parity = binaryData[i+1];
            if (parity != calculateParity(data))
                {++check;}
        }

        if (check != 0)
                cout << "Parity control sum FAIL" << endl;
        else
                cout << "Parity control sum PASS" << endl;

        binaryData.clear();

        return 0;
}

#endif // BYTEPARITY_H
