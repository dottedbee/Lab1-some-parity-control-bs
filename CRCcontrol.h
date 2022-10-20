#ifndef CRCCONTROL_H
#define CRCCONTROL_H
#include <math.h>
#include <string>
#include <vector>
#include <bitset>
#include <cstddef>
#include <insertErrors.h>
#include <fileHandler.h>

using namespace std;

// wielomian: x8+x2+x+1 100000111 0x07

char crc8(char data, int crc)
{
    for (size_t i = 0; i < sizeof(data); ++i)
    {
        crc ^= data;
        for (size_t j = 0; j < 8; ++j)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x07;
            } else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

int calculateCRC8(vector<char> &binaryData)
{
    int crc8val = 0;
    int crc = 0x00;

    for(size_t i = 0; i < binaryData.size(); ++i)
    {
        crc8val = crc8(binaryData[i], crc8val);
    }
    return crc8val;
}

int crcControl (vector<char> &fileData, string withCRCFileName, string fileName, double errorPercentage, int errorRepetition, bool repetitionEnabled)
{
        if (!getFileContent(fileName, fileData))
            {return -1;}

        vector<char> withCRCCheck;
        withCRCCheck.reserve(fileData.size()+1); 	// cztery bajty na sume kontrolna CRC32

        int crc8 = calculateCRC8(fileData);

        fileData = addErrors(errorRepetition, errorPercentage, fileData, repetitionEnabled);

        withCRCCheck.assign(fileData.begin(), fileData.end());
        withCRCCheck.push_back(crc8);

        if (!saveFileContent(withCRCFileName, withCRCCheck))
                {return -1;}

        fileData.clear();

        vector<char> binaryData;
        if (!getFileContent(withCRCFileName, binaryData))
                {return -1;}

        vector<char> extractedData;
        extractedData.assign(binaryData.begin(), binaryData.end()-1);
        int crcChecked = calculateCRC8(extractedData);

        int crcReceived = binaryData[binaryData.size()-1];

// sprawdzam zgodnosc sumy kontrolnej odczytanej z pliku i tej wyliczonej na nowo
        if (crcReceived != crcChecked)
                cout << "CRC control sum FAIL" << endl;
        else
                cout << "CRC control sum PASS" << endl;

        binaryData.clear();
        extractedData.clear();

        return 0;
}

#endif // CRCCONTROL_H
