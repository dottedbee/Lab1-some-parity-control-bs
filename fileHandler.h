#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <bitset>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

using namespace std;

//======================================================================================================
//                                   otwieranie i zamykanie pliku
//======================================================================================================
// otwiera, czyta i zapisuje plik do wektora
bool getFileContent(const string &fileName, vector<char> &container)
{
    ifstream file(fileName, ios::in|ios::binary);
    file.unsetf(ios::skipws);

    if (file.fail())
    {
        cout << "Encountered an error while opening file " <<  fileName << endl;
        return false;
    }

// gdzie on sie konczy zeby znalezc dlugosc
    file.seekg (0 , ios::end);
    size_t lenOfFile = file.tellg();
// wracam
    file.seekg (0 , ios::beg);
    container.reserve(lenOfFile);

    for(;;)
    {
        container.push_back(file.get());
        --lenOfFile;
        if (lenOfFile == 0)
             break;
     }

     file.close();
     return true;
}

// zapisuje elementy wektora do pliku
bool saveFileContent(const string &fileName, const vector<char> &container)
{
    ofstream file(fileName, ios::out|ios::binary);
    if (file.fail())
    {
        cout << "Encountered an error while saving file " <<  fileName << endl;
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

#endif // FILEHANDLER_H
