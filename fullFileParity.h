#ifndef FULLFILEPARITY_H
#define FULLFILEPARITY_H

#include <math.h>
#include <string>
#include <vector>
#include <bitset>
#include <insertErrors.h>
#include <fileHandler.h>

using namespace std;

//======================================================================================================
//                                    bit parzystosci wyliczanie
//======================================================================================================
char calculateParity(vector<char> &container)
{
     /*sprawdzamy czy bit w charze jest wiekszy od zera (czyli rowny 1)
    jesli tak to zwiekszamy counter o 1. potem dzielimy na dwa i wychodzi reszta 1 albo 0 */

    int counter = 0;
    for (size_t i = 0; i < container.size(); ++i)
    {
        char c = container[i];
        if ((c >> i) & (0x01 > 0))
        {
        counter++;
        }
    }
    return counter%2;
}

//======================================================================================================
//                                bit parzystosci na calym ciagu pliku
//======================================================================================================
int parityControlFullFile (vector<char> &fileData, vector<char> &temp, string withParityFileName, string fileName, double errorPercentage, int errorRepetition, bool repetitionEnabled)
{
        if (!getFileContent(fileName, fileData))
            {return -1;}

// robie miejsce na dane + sume kontrolna
        vector<char> withParityCheck;
        withParityCheck.reserve(fileData.size()+sizeof(char));

// wyliczam sume kontrolna na poprawnych danych
        char sumPar = calculateParity(fileData);

// dodaje bledy do danych i nadpisuje
        fileData = addErrors(errorRepetition, errorPercentage, fileData, repetitionEnabled);

// zapisuje najpierw bledny wektor a potem na koncu sume, wiec jak otworze potem plik i odejme i wylicze sume ponownie
// to wynik bedzie inny bo operacja bedzie na bloku innych danych.
        withParityCheck.assign(fileData.begin(), fileData.end());
        withParityCheck.push_back(sumPar);

        if (!saveFileContent(withParityFileName, withParityCheck))
                {return -1;}

        fileData.clear();

// 'rozdzielam' wektor na czesci: przeslane dane i suma kontrolna

        vector<char> binaryData;
        if (!getFileContent(withParityFileName, binaryData))
                {return -1;}

        char parityValue = binaryData[binaryData.size()-1];
        temp.assign(binaryData.begin(), binaryData.end()-1);

// sprawdzam zgodnosc sumy kontrolnej odczytanej z pliku i tej wyliczonej na nowo
        if (parityValue != calculateParity(temp))
                cout << "Parity control sum FAIL" << endl;
        else
                cout << "Parity control sum PASS" << endl;

        binaryData.clear();
        temp.clear();

        return 0;
}

#endif // FULLFILEPARITY_H
