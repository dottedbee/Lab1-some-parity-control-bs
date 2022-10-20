#ifndef INSERTERRORS_H
#define INSERTERRORS_H

#include <random>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<char> addErrors(int errorRepetition, double errorPercentage, vector<char> &container, bool repetitionEnabled)
{

    int errorNumber;
    if (errorPercentage > 0 && errorRepetition == 0 && errorPercentage <= 100)
    {
        errorNumber = container.size()*errorPercentage/100;
    } else if (errorRepetition > 0 && errorPercentage == 0 && errorRepetition < container.size())
    {
        errorNumber = errorRepetition;
    } else if (errorRepetition == 0 && errorPercentage == 0)
    {
        errorNumber = 0;
    } else
    {
        cout << "An error has occured, please make sure you defined only either errorRepetition or errorPercentage value.\n"
                "errorRepetition value CANNOT be greater than the length of the file: "<< container.size() <<
                ".\n errorPercentage value CANNOT be greater than 100."<< endl;
        exit(1);
    }

        if(repetitionEnabled == true)
        {
            for(size_t ir = 0; ir < errorNumber; ++ir)
            {
                int randomContNumber = rand()%container.size();
                std::bitset<8> randByte(container[randomContNumber]); // wybieram przypadkowy element z wektora charow i robie z niego ciag bitow

                int randomBitNumber = rand()%8; // wybieram jeden losowy bit z bajtu
                string randBytestr = randByte.to_string();

                container[randomContNumber] = (randBytestr[randomBitNumber]+1)%2;
            }
        } else
        {
            for(size_t io = 0; io < errorNumber; ++io)
            {
                std::bitset<8> orderByte(container[io]); /*wybieram po kolei bajt i robie z niego ciag bitow (errorow zawsze jest mniej niz
                                                            bledow wiec nie musze sie martwic ze sie skonczy miejsce w kontenerze)*/
                int randomBitNumber = rand()%8; // wybieram jeden losowy bit z bajtu
                string randBytestr = orderByte.to_string();

                container[io] = (orderByte[randomBitNumber]+1)%2;
            }
        }

    return container;
}

vector<char> addErrorsB(int errorRepetition, double errorPercentage, vector<char> &container, bool repetitionEnabled)
{

    int errorNumber;
    if (errorPercentage > 0 && errorRepetition == 0 && errorPercentage <= 100)
    {
        errorNumber = container.size()*errorPercentage/100;
    } else if (errorRepetition > 0 && errorPercentage == 0 && errorRepetition < container.size())
    {
        errorNumber = errorRepetition;
    } else if (errorRepetition == 0 && errorPercentage == 0)
    {
        errorNumber = 0;
    } else
    {
        cout << "An error has occured, please make sure you defined only either errorRepetition or errorPercentage value.\n"
                "errorRepetition value CANNOT be greater than the length of the file: "<< container.size() <<
                ".\n errorPercentage value CANNOT be greater than 100."<< endl;
        exit(1);
    }

        if(repetitionEnabled == true)
        {
            for(size_t ir = 0; ir < errorNumber; ++ir)
            {
                int randomContNumber = (rand()*2)%container.size();
                std::bitset<8> randByte(container[randomContNumber]); // wybieram przypadkowy element z parzystym indeksem z wektora charow i robie z niego ciag bitow

                int randomBitNumber = rand()%8; // wybieram jeden losowy bit z bajtu
                string randBytestr = randByte.to_string();

                container[randomContNumber] = (randBytestr[randomBitNumber]+1)%2;
            }
        } else
        {
            for(size_t io = 0; io < errorNumber; ++io)
            {
                std::bitset<8> orderByte(container[io*2]); /*wybieram po kolei bajt i robie z niego ciag bitow (errorow zawsze jest mniej niz
                                                            bledow wiec nie musze sie martwic ze sie skonczy miejsce w kontenerze)*/
                int randomBitNumber = rand()%8; // wybieram jeden losowy bit z bajtu
                string randBytestr = orderByte.to_string();

                container[io*2] = (orderByte[randomBitNumber]+1)%2;
            }
        }

    return container;
}

#endif // INSERTERRORS_H
