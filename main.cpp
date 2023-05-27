#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

const int range = 6;

struct Pracownik
{
    string nazwisko;
    unsigned int staz;
    unsigned int pensja;
};

void ArrayFilling (int [][range], int, int);
void ArrayPrint2D (int [][range]);
bool OpenOutputFile (ofstream &, string);
void FileSave2D (ofstream &, int [][range]);
void FileSave1D (ofstream &, Pracownik [], unsigned int);
bool OpenFileRead (ifstream &, string, int &);
void FileRead2D (ifstream &, int [][range]);
void FileRead1D (ifstream &, Pracownik [], unsigned int);
void ArrayPrint1D (Pracownik [], unsigned int);
double AverageSalary (Pracownik [], unsigned int, unsigned int);
void StringSorting (Pracownik [], unsigned int);

int main ()
{
    srand (time(0));
    int A[range][range];
    int p, k;
    char choice;
    int lineCounter = 0;
    
    cout << "Zadanie 2.4.1: podstawowa obsluga strumienia we/wy" << endl << endl;
    cout << "Podaj poczatek przedzialu: ";
    cin >> p;
    cout << "Podaj koniec przedzialu: ";
    cin >> k;

    ArrayFilling(A,p,k);

    cout << "Czy chcesz wyswietlic zawartosc tablicy wylosowanych liczb [t]? ";
    cin >> choice;
    if (choice=='t' || choice=='T')
        ArrayPrint2D(A);

    ofstream fileOut;

    if (OpenOutputFile (fileOut, "plikWynikowy.txt")==false)
    {
        cout << "Otwarcie pliku do zapisu zakonczone niepowodzeniem, koniec programu." << endl;
        return 1;
    }

    FileSave2D (fileOut, A);
    fileOut.close();

    ifstream fileInput;
    if (OpenFileRead(fileInput, "plikWynikowy.txt", lineCounter)==false)
    {
        cout << "Otwarcie pliku do odczytu zakonczone niepowodzeniem, koniec programu." << endl;
        return 1;
    }

    FileRead2D (fileInput, A);
    fileInput.close();

    cout << "Czy chcesz wyswietlic zawartosc tablicy transponowanej [t]? ";
    cin >> choice;
    if (choice=='t' || choice=='T')
        ArrayPrint2D(A);

    int N;
    cout << endl << "Zadanie 2.4.2: struktury + obsluga we/wy" << endl << endl;

    lineCounter = 0;
    if (OpenFileRead(fileInput, "pracownicy.txt", lineCounter)==false)
    {
        cout << "Otwarcie pliku do odczytu zakonczone niepowodzeniem, koniec programu." << endl;
        return 1;
    }

    do
    {
        cout << "Deklaracja tablicy \"Pracownik\". Podaj ile rekordow chcesz odczytac z pliku: ";
        cin >> N;
        if (N<0 || N>lineCounter)
        {
            cout << endl << "Zakres przekracza ilosc rekordow w pliku. Podaj liczbe miedzy 0, a ";
            cout << lineCounter << "." << endl;
        }
    } while (N<0 || N>lineCounter);
    Pracownik *worker = new Pracownik [N];

    FileRead1D (fileInput, worker, N);
    fileInput.close();

    cout << "Czy chcesz wyswietlic zawartosc tablicy pracownikow [t]? ";
    cin >> choice;
    if (choice=='t' || choice=='T')
        ArrayPrint1D (worker, N);
    
    unsigned int seniority;
    cout << "Dla jakiego minimalnego stazu, obliczyc srednia pensje? ";
    cin >> seniority;

    cout << "Dla stazu trwajacego >= " << seniority << ", srednia pensja wynosi: " << AverageSalary (worker, N, seniority) << endl;
    
    StringSorting (worker, N);
    cout << "Czy chcesz wyswietlic zawartosc posortowanej alfabetycznie tablicy pracownikow [t]? ";
    cin >> choice;
    if (choice=='t' || choice=='T')
        ArrayPrint1D (worker, N);

    string newFileName;
    cout << "Podaj nazwe pliku (wraz z rozszerzeniem), do jakiego chcesz zapisac posortowana alfabetycznie tablice pracownikow" << endl;
    cin >> newFileName;

    if (OpenOutputFile (fileOut, newFileName)==false)
    {
        cout << "Otwarcie pliku do zapisu zakonczone niepowodzeniem, koniec programu." << endl;
        return 1;
    }
    FileSave1D (fileOut, worker, N);
    fileOut.close();

    delete [] worker;
    return 0;
}

void ArrayFilling (int array[range][range], int beginning, int ending)
{
	for (int i=0; i<range; i++)
	{
		for (int j=0; j<range; j++)
        {
            array[i][j] = rand ()% (ending - beginning + 1) + beginning;
        }
	}
}

void ArrayPrint2D (int array[range][range])
{
    cout << "Tablica:" << endl;
    for (int i=0; i<range; i++)
    {
        for (int j=0; j<range; j++)
        {
            cout << setw (5) << array[i][j] << "|";
        }
        cout << endl;
    }
}

bool OpenOutputFile (ofstream &myFile, string fileName)
{
    int fileNameLength = fileName.length();
    char *charArray = new char [fileNameLength];
    for (int i=0; i<fileNameLength; i++)
    {
        charArray[i] = fileName[i];
    }
    myFile.open (charArray, ios::out);

    delete []charArray;

    if (myFile.is_open()==false)
        return false;
    else
        return true;
}

void FileSave2D (ofstream &myFile, int array[range][range])
{
    int endline = range-1;
    for (int i=0; i<range; i++)
    {
        for (int j=0; j<range; j++)
        {
            myFile << array[i][j];
            if (j!=endline) myFile << " ";
        }
        if (i!=endline) myFile << endl;
    }
}

bool OpenFileRead (ifstream &myFile, string fileName, int &lineCounter)
{
    int fileNameLength = fileName.length();

    char *charArray = new char [fileNameLength];
    for (int i=0; i<fileNameLength; i++)
    {
        charArray[i] = fileName[i];
    }
    myFile.open (charArray, ios::in);

    if (myFile.is_open()==false)
    {
        delete []charArray;
        return false;
    }
    string tempLine;
    while (myFile.peek()!=EOF)
    {
        getline (myFile, tempLine);
        ++lineCounter;
    }
    myFile.close();

    myFile.open (charArray, ios::in);
    if (myFile.is_open()==false)
    {
        delete []charArray;
        return false;
    }

    delete []charArray;
    return true;
}

void FileRead2D (ifstream &myFile, int array[range][range])
{
    for (int i=0; i<range; i++)
    {
        for (int j=0; j<range; j++)
        {
            myFile >> array[j][i];
        }
    }
}

void FileRead1D (ifstream &myFile, Pracownik array[], unsigned int arrayLength)
{
    for (int i=0; i<arrayLength; i++)
    {
        myFile >> array[i].nazwisko >> array[i].staz >> array[i].pensja;
    }
}

void ArrayPrint1D (Pracownik array[], unsigned int arrayLength)
{
    cout << setw(15) << left << "NAZWISKO";
    cout << setw(4) << "STAZ" << " \t";
    cout << setw(6) << "PENSJA" << endl;
    for (int i=0; i<arrayLength; i++)
    {
        cout << setw(15) << left << array[i].nazwisko;
        cout << setw(4) << right << array[i].staz << " \t";
        cout << setw(6) << array[i].pensja << endl;
    }
}

double AverageSalary (Pracownik array[], unsigned int arrayLength, unsigned int given)
{
    double sum = 0.0;
    int counter = 0;
    for (int i=0; i<arrayLength; i++)
    {
        if (array[i].staz>=given)
        {
            sum += array[i].pensja;
            ++counter;
        }
    }
    return (sum/counter);
}

void StringSorting (Pracownik array[], unsigned int arrayLength)
{
    int i, j;
    Pracownik temp[1];
    do
    {
        for (i=0; i<arrayLength-1; i++)
        {
            j=0;
            while (array[i].nazwisko[j]==array[i+1].nazwisko[j])
            {
                ++j;
                if (j>array[i].nazwisko.length())
                {
                    --j;
                    break;
                }
            }
            if (array[i].nazwisko[j] > array[i+1].nazwisko[j])
            {
                temp[0] = array[i];
                array[i] = array[i+1];
                array[i+1] = temp[0];
            }
        }
        --arrayLength;
    } while (arrayLength>1);
}

void FileSave1D (ofstream &myFile, Pracownik array[], unsigned int arrayLength)
{
    int lastIndex = arrayLength-1;
    for (int i=0; i<arrayLength; i++)
    {
        myFile << setw(15) << left << array[i].nazwisko;
        myFile << setw(4) << right << array[i].staz << " \t";
        myFile << setw(6) << array[i].pensja;
        if (i!=lastIndex) myFile << endl;
    }
}
