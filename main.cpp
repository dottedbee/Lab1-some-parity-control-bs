#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <ctime>

#include <fullFileParity.h>
#include <fullFileModulo.h>
#include <byteParity.h>
#include <byteModulo.h>
#include <CRCcontrol.h>

using namespace std;

int main(int argc, char** argv)
{
//======================================================================================================
//                                      deklaracje  zmiennych
//======================================================================================================
    srand(time(NULL)); //seed

// Potworne oszustwo windowsowa funkcja, nie chce mi sie zawsze wpisywac sciezki do pliku >:O
    TCHAR result[ MAX_PATH ];
    GetCurrentDirectory(MAX_PATH, result);
    wstring idk(&result[0]);
    string pathToFile(idk.begin(), idk.end());

    //string fileName = pathToFile + "\\testFile\\test.txt";
    //string fileName = pathToFile + "\\testFile\\test.png";
    string fileName = pathToFile + "\\testFile\\test.bin";
    string withParityFileName = pathToFile + "\\testFile\\testParity.bin";
    string withModuloFileName =  pathToFile + "\\testFile\\testModulo.bin";
    string withCRCFileName =  pathToFile + "\\testFile\\testCRC.bin";

    vector<char> fileData;
    vector<char> temp;

// wspolczynniki bledow
    double errorPercentage = 0;
    int errorRepetition = 0;
    bool repetitionEnabled = false;

// Nie pamietalam czy to mialo byc dla calego pliku czy tylko kazdego bajtu wiec moja paranoja kazala mi zrobic dwie wersje ;--;
    bool controlSumForFullFile = false;

    if (controlSumForFullFile)
    {
        parityControlFullFile (fileData, temp, withParityFileName, fileName, errorPercentage, errorRepetition, repetitionEnabled);
        moduloControlFullFile (fileData, temp, withModuloFileName, fileName, errorPercentage, errorRepetition, repetitionEnabled);
    } else
    {
        parityControlByByte (fileData, withParityFileName, fileName, errorPercentage, errorRepetition, repetitionEnabled);
        moduloControlByByte (fileData, withModuloFileName, fileName, errorPercentage, errorRepetition, repetitionEnabled);
    }

    crcControl (fileData, withCRCFileName, fileName, errorPercentage, errorRepetition, repetitionEnabled);

    return 0;
}

