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
#include <limits.h>
#include <typeinfo>

using namespace std;

struct Route {
    string destination;
    string ancestor;
    size_t distance;
    
    Route(string destination, size_t distance) {
        this->distance = distance;
        this->destination = destination;
    }
    
    Route(string destination, string ancestor, size_t distance) {
        this->distance = distance;
        this->destination = destination;
        this->ancestor = ancestor;
    }
    
    Route(){}
    
    bool operator < (const Route &r) const {
        return (distance < r.distance);
    }
};

void showInstruction();
void checkIfStartIsDeclaredInTheFile(const map<string, vector<Route> >& routes, const string& start);
void saveToFile(const string& fileName, const vector<string>& finalPat);
void readDataFromFile(const string& inputPath, string& start, string& finish,  map<string, vector<Route> >& routes);
void calculateRoute(vector<Route>& finished, multiset<Route>& working, map<string, vector<Route> >& routes, const string& start, const string& finish);
bool isInFinished(const vector<Route>& finished, const string& nodeName);
size_t getShortestRoute(const vector<Route> &finished, const string& finish);
void getFinalPathWithDistance(const vector<Route> &finished, const string& start, const string& finish, vector<string>& finalPath);
string argsFromCmd(int argc, const char * argv[], const char operant[]);
bool isLineValid(const string& start, const string& finish, const size_t& distance, const int& lineNUmber);

int main(int argc, const char * argv[]) {
    
    
    if (!(argc == 7)) {
        showInstruction();
    } else {
        
        map<string, vector<Route> > routes;
        vector<string> finalPath;
        string start, finish, input, output;
        
        try {
            input = argsFromCmd(argc, argv, "-i");
        } catch (const char* error) {
            cerr << error << endl << endl;
            showInstruction();
            exit(-1);
        }
        
        
        try {
            readDataFromFile(input, start, finish, routes);
            
        } catch (const int inputLine) {
            cerr << "Nieprawidłowe dane w linijce: " << inputLine << endl;
            exit(-1);
        } catch (const char* fileDoesNotExists) {
            cout << fileDoesNotExists << input << endl;
            exit(-1);
        }
        
        try {
            start = argsFromCmd(argc, argv, "-s");
            output = argsFromCmd(argc, argv, "-o");
        } catch (const char* error) {
            cerr << error << endl << endl;
            showInstruction();
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
            cerr << error << endl;
        }
    }
    return 0;
}

void showInstruction() {
    //TODO: check if it works on windows command line
    cerr <<"Aby skorzystać z programu należy podać paramatery startowe(kolejność nie ma znaczenia): " << endl;
    cerr <<"-i <nazwa pliku wejścia> -o <nazwa pliku wyjścia> -s <nazwa bazy>" << endl;
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
            if (isLineValid(start, finish, distance, lineNumber)) {
                routes[start].push_back(*(new Route(finish, distance)));
                routes[finish].push_back(*(new Route(start, distance)));
            } else {
                //Throw the num of non valid line
                throw lineNumber;
            }
        }
    } else {
        throw "Sprawdź czy plik o podanej nazwie instnieje: ";
    }
    in.close();
}

void calculateRoute(vector<Route>& finished, multiset<Route>& working, map<string, vector<Route> >& routes, const string& start, const string& finish) {
    
    working.insert(*(new Route(start, 0)));
    
    for (auto workRoute : working) {
        
        if (workRoute.destination == finish) {
            finished.push_back(workRoute);
        }
        
        if (isInFinished(finished, workRoute.destination))
            continue;
        
        finished.push_back(workRoute);
        
        for (auto route : routes[workRoute.destination]) {
            string destination = route.destination;
            
            if (!(isInFinished(finished, destination))) {
                size_t distanceCost = route.distance + workRoute.distance;
                working.insert(*(new Route(destination, workRoute.destination, distanceCost)));
            }
        }
    }
}

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
        throw "Zapoznaj się z instrukcją obsługi programu: ";
    }
    return arg;
}

bool isLineValid(const string& start, const string& finish, const size_t& distance, const int& lineNUmber) {
    bool isValid = true;
    if (start.empty() || finish.empty() || distance == 0) {
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
        cout <<  "Brak miasta o nazwie: " << start << endl;
        exit(-1);
    }
}

/**
 * Funkcja sprawdza czy lokalizacja jest dodana do vectora wktórym znajdują się już obliczone odległości ale nie koniecznie najkrótsze
 * @param nodeName przechowywuje nazwę wierzchołka - wierzchołek to miasto
 * @param finished przechowywuje wszystkie do danego miasta(nie tylko najkrótsze)
 * @return funkcja zwraca wartość 'true' jeżeli podane miasto znajduje się już w wektorze
 */

bool isInFinished(const vector<Route> &finished, const string& nodeName) {
    bool isFound = false;
    
    for (auto route : finished) {
        if (route.destination == nodeName) {
            isFound = true;
            break;
        }
    }
    return isFound;
}

/**
 * Funkcja znajduje i zwraca najkrótszą odległość od miasta pocątkowego do końcowego
 * @param finished przechowywuje wszystkie do danego miasta (nie tylko najkrótsze)
 * @param finish miasto docelowe
 */

size_t getShortestRoute(const vector<Route>& finished, const string& finish) {
    
    size_t result = SIZE_MAX;
    size_t new_result;
    
    for (auto route : finished) {
        if (route.destination == finish) {
            new_result = route.distance;
            if (new_result < result)result = new_result;
        }
    }
    
    if (!(result == SIZE_MAX || result == 0)) {
        return result;
    } else {
        throw "Brak połączenia do miasta(ta informacja nie jest zapisywana do pliku): ";
    }
}

Route returnRoute(const vector<Route>& finished, string destination)
{
    Route buffRoute = *new Route();
    
    for (auto route : finished) {
        if (route.destination == destination) {
            buffRoute = route;
        }
    }
    return buffRoute;
}

void getFinalPathWithDistance(const vector<Route> &finished, const string& start, const string& finish, vector<string>& finalPath) {
    
    size_t distance;
    string path;
    string ancestor;
    vector<string> vPath;
    
    try {
        distance = getShortestRoute(finished, finish);
        
        vPath.push_back(finish);
        
        for (auto route : finished) {
            if (route.destination == finish && route.distance == distance) ancestor = route.ancestor;
        }
        
        while (ancestor != "") {
            Route buffor = returnRoute(finished, ancestor);
            vPath.push_back(buffor.destination);
            ancestor = buffor.ancestor;
        }
        
        for (int i = vPath.size() - 1.00; i > -1; i--) {
            path += vPath[i];
            if (vPath[i] != finish)path.append(" -> "); else path.append(": ");
        }
        
        path.append(to_string(distance));
        finalPath.push_back(path);
        
        
    } catch (const char* error) {
        cerr << error << finish << endl;
    }
}

void saveToFile(const string& fileName, const vector<string>& finalPath) {
    
    ofstream out;
    out.open(fileName, ios::out);
    string toSave;
    if (out.is_open()) {
        for (auto path : finalPath) {
            out << path << endl;
        }
    } else {
        throw "Prwdopodobnie brak uprawnien do otwierania plików";
    }
    out.close();
}

