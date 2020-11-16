//
//  main.cpp
//  SpedycjaC++
//
//  Created by Jan Piaskowy on 27/10/2020.
//  Copyright © 2020 Jan Piaskowy. All rights reserved.
//

#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "dijkstra.hpp"

using namespace std;

void showInstruction();
void checkIfStartIsDeclaredInTheFile(const map<string, vector<Route> >& routes, const string& start);
void saveToFile(const string& fileName, const vector<string>& finalPat);
void readDataFromFile(const string& inputPath, string& start, string& finish,  map<string, vector<Route> >& routes);
size_t getShortestRoute(const vector<Route> &finished, const string& finish);
string argsFromCmd(int argc, const char * argv[], const char operant[]);
bool isLineValid(const string& start, const string& finish, const size_t& distance);

int main(int argc, const char * argv[]) {
    
    
    if (!(argc == 7)) {
        showInstruction();
    } else {
        
        map<string, vector<Route> > routes;
        vector<string> finalPath;
        string start, finish, input, output;
        
        try {
            input = argsFromCmd(argc, argv, "-i");
            start = argsFromCmd(argc, argv, "-s");
            output = argsFromCmd(argc, argv, "-o");
        } catch (const char* error) {
            cerr << error << endl << endl;
            showInstruction();
            exit(-1);
        }
        
        try {
            readDataFromFile(input, start, finish, routes);
        } catch (const int inputLine) {
            cerr << "[error] Nieprawidłowe dane w linijce: " << inputLine << endl;
            exit(-1);
        }
        
        //TODO: delete it before final version
        //For debugging
        //finish = argv[7];
        checkIfStartIsDeclaredInTheFile(routes, start);
        
        for(auto route : routes){
            
            if (route.first != start) finish = route.first; else continue;
            
            vector<Route> finished;
            multiset<Route> working;
            
            calculateRoute(finished, working, routes, start, finish);
            
            getFinalPathWithDistance(finished, start, finish, finalPath);
            
        }
        try {
            saveToFile(output, finalPath);
        } catch (const char* error) {
            cerr << "[error] Problem z utworzniem pliku: " << error << endl;
        }
    }
    return 0;
}

void showInstruction() {
    //TODO: check if it works on windows command line
    cerr <<"[info] Aby skorzystać z programu należy podać paramatery startowe(kolejność nie ma znaczenia): " << endl;
    cerr <<"[info] -i <nazwa pliku wejścia> -o <nazwa pliku wyjścia> -s <nazwa bazy>" << endl;
}

void readDataFromFile(const string& inputPath, string& start, string& finish,  map<string, vector<Route> >& routes) {
    
    ifstream in(inputPath);
    if (in.is_open()) {
        stringstream ss;
        string line;
        size_t distance;
        int lineNumber = 0;
        while (getline(in, line)) {
            lineNumber++;
            ss.clear();
            ss.str("");
            start.clear();
            finish.clear();
            distance = 0;
            ss << line;
            ss >> start >> finish >> distance;
            if (isLineValid(start, finish, distance)) {
                routes[start].push_back(*(new Route(finish, distance)));
                routes[finish].push_back(*(new Route(start, distance)));
            } else {
                //Throws the num of non valid line
                throw lineNumber;
            }
        }
    } else {
        cerr << "[error] Sprawdź czy plik o podanej nazwie instnieje: " << inputPath << endl;
        exit(-1);
    }
    in.close();
}

/**
 *Funkcja pobierająca parametry startowe programu
 *@param operant przełącznik którego szukamy
 *@returns arg zwraca wartość następnego argumentu po przełączniku którego poszukujemy
 */
string argsFromCmd(int argc, const char * argv[], const char operant[]) {
    string arg;
    bool error = true;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], operant) == 0) {
            error = false;
            arg = argv[i + 1];
        }
    }
    if (error) {
        throw "[error] Zapoznaj się z instrukcją obsługi programu: ";
    }
    return arg;
}

/**
 *Funkcja waliduje dane wporwadzone w pliku wejściiwym
 *@param start miasto początkowe
 *@param finish miasto docelowe
 *@param distance dystans pomiędzy dwoma miastami
 */
bool isLineValid(const string& start, const string& finish, const size_t& distance) {
    bool isValid = true;
    if (start.empty() || finish.empty() || distance == 0 || distance == SIZE_MAX) {
        isValid = false;
    }
    return isValid;
}

/**
 *Funkcja sprawdza czy miasto od którego chcemy zacząć zostało zdefiniowane w pliku
 *@param routes wszystkie możliwe połaczenia
 *@param start miasto początkowe
 */
void checkIfStartIsDeclaredInTheFile(const map<string, vector<Route> >& routes, const string& start) {
    try {
        routes.at(start);
    } catch (out_of_range&) {
        cout <<  "[error] Brak miasta o nazwie: " << start << endl;
        exit(-1);
    }
}

/**
 *Funkcja zapisuje wyznaczone ścieżki do plikui
 *@param fileName ściażka pliku do zapisu
 *@param finalPath kontener zawiecający utworzone ścieżki
 */
void saveToFile(const string& fileName, const vector<string>& finalPath) {
    
    ofstream out;
    out.open(fileName, ios::out);
    if (out.is_open()) {
        for (auto path : finalPath) {
            out << path << endl;
        }
        cout << "[sucess] Zapisano do pliku: " << fileName << endl;
    } else {
        throw fileName;
    }
    out.close();
}

