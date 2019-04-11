/*
	Name: Burak Bozdað
	ID: 150170110
*/

#include <fstream>
#include <iostream>
#include <vector>

#define MAX(a, b) (((a) > (b)) ? a : b)
#define MIN(a, b) (((a) < (b)) ? a : b)

using namespace std;

#include "Polynomial.h"
#include "Vector.h"

void actions()
{
    cout << "Possible Actions:" << endl;
    cout << "1. Print Polynomial and Vector lists" << endl;
    cout << "2. Do a polynomial operation" << endl;
    cout << "3. Do a vector operation" << endl;
    cout << "4. Help: Print possible actions" << endl;
    cout << "0. Exit the program" << endl
         << endl
         << endl;
    cout << "Enter an option: ";
}

int main()
{
    cout << "Polynomial and Vector List Program!" << endl;
    cout << "Polynomials and Vectors are read from text files!" << endl
         << endl;

    ifstream readp("Polynomial.txt");
    ifstream readv("Vector.txt");
    int pcount, vcount;
    readp >> pcount;
    readv >> vcount;

    //Arrays are created and initialized to 0
    Polynomial polys[pcount];
    Vector vects[vcount];

    //Reading files and saving data
    int sd;
    for (int i = 0; i < pcount; i++) {
        readp >> sd;
        int temp[sd + 1];
        for (int j = 0; j <= sd; j++)
            readp >> temp[j];
        polys[i].setP(sd, temp);
    }
    for (int i = 0; i < vcount; i++) {
        readv >> sd;
        int temp[sd];
        for (int j = 0; j < sd; j++)
            readv >> temp[j];
        vects[i].setV(sd, temp);
    }

    //The main part of the program
    int opt = 4, a, b;
    char op;
    while (opt != 0) {
        switch (opt) {
        case 4: //HELP
            actions();
            cin >> opt;
            break;
        case 0: //Close program
            readp.close();
            readv.close();
            return 0;
            break;
        case 1: //Show current vectors and polynomials
            cout << "Vectors:" << endl;
            for (int i = 0; i < vcount; i++)
                cout << i + 1 << ". " << vects[i] << endl;
            cout << endl
                 << "Polynomials:" << endl;
            for (int i = 0; i < pcount; i++)
                cout << i + 1 << ". " << polys[i] << endl;
            cout << endl
                 << "Enter an option: ";
            cin >> opt;
            break;
        case 2: //Do polynomial operations
            cout << "Which polynomial operation would you like to do?(+:addition, *:multiplication):" << endl;
            cin >> a >> op >> b;
            cout << "(" << polys[a - 1] << ") " << op << " (" << polys[b - 1] << ") = " << ((op == '+') ? (polys[a - 1] + polys[b - 1]) : (polys[a - 1] * polys[b - 1])) << endl;
            cout << endl
                 << "Enter an option: ";
            cin >> opt;
            break;
        case 3: //Do vector operations
            cout << "Which vector operation would you like to do?(+:addition, *:scalar multiplication, .:dot product):" << endl;
            cin >> a >> op >> b;
            if (op == '.') {
                if (vects[a - 1].getSize() != vects[b - 1].getSize())
                    cerr << "ERROR: Sizes are not equal." << endl;
                else
                    cout << vects[a - 1] << " . " << vects[b - 1] << " = " << vects[a - 1] * vects[b - 1] << endl;
            } else if (op == '*')
                cout << vects[a - 1] << " * " << b << " = " << vects[a - 1] * b << endl;
            else {
                if (vects[a - 1].getSize() != vects[b - 1].getSize())
                    cerr << "ERROR: Sizes are not equal." << endl;
                else
                    cout << vects[a - 1] << " + " << vects[b - 1] << " = " << vects[a - 1] + vects[b - 1] << endl;
            }
            cout << endl
                 << "Enter an option: ";
            cin >> opt;
            break;
        }
    }

    return 0;
}
