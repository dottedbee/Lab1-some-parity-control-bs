#ifndef FULLFILEMODULO_H
#define FULLFILEMODULO_H

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
char calculateControlSum(vector<char> &container)
{
    /* po calej dlugosci pliku sprawdzamy po kolei czy bit jest wiekszy od zera (czyli rowny 1)
    jesli tak to zwiekszamy sumę o 2^i. potem dzielimy przez jakas liczbe pierwsza i nasz wynik jest suma kontrolna */
    int sum = 0;
    char primeNumber =  13;
    for (size_t i = 0; i < container.size(); ++i)
    {
        char c = container[i];
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
int moduloControlFullFile (vector<char> &fileData, vector<char> &temp, string withModuloFileName, string fileName, double errorPercentage, int errorRepetition, bool repetitionEnabled)
{
        if (!getFileContent(fileName, fileData))
            {return -1;}

// robie miejsce na dane + sume kontrolna
        vector<char> withModuloCheck;
        withModuloCheck.reserve(fileData.size()+sizeof(char));

// wyliczam sume kontrolna na poprawnych danych
        char sumConMod = calculateControlSum(fileData);

// dodaje bledy do danych i nadpisuje
        fileData = addErrors(errorRepetition, errorPercentage, fileData, repetitionEnabled);

// zapisuje najpierw bledny wektor a potem na koncu sume, wiec jak otworze potem plik i odejme i wylicze sume ponownie
// to wynik bedzie inny bo operacja bedzie na bloku innych danych.
        withModuloCheck.assign(fileData.begin(), fileData.end());
        withModuloCheck.push_back(sumConMod);

        if (!saveFileContent(withModuloFileName, withModuloCheck))
                {return -1;}

        fileData.clear();

// 'rozdzielam' wektor na czesci: przeslane dane i suma kontrolna

        vector<char> binaryDataModulo;
        if (!getFileContent(withModuloFileName, binaryDataModulo))
                {return -1;}

        char moduloValue = binaryDataModulo[binaryDataModulo.size()-1];
        temp.assign(binaryDataModulo.begin(), binaryDataModulo.end()-1);

// sprawdzam zgodnosc sumy kontrolnej odczytanej z pliku i tej wyliczonej na nowo
        if (moduloValue != calculateControlSum(temp))

                cout << "Modulo control sum FAIL" << endl;
        else
                cout << "Modulo control sum PASS" << endl;

        binaryDataModulo.clear();
        temp.clear();

        return 0;
}

#endif // FULLFILEMODULO_H
