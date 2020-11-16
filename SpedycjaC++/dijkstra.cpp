//
//  dijkstra.cpp
//  SpedycjaC++
//
//  Created by Jan Piaskowy on 15/11/2020.
//  Copyright © 2020 Jan Piaskowy. All rights reserved.
//

#include "dijkstra.hpp"


/**
 *Funkcja przeszukuje kontener finished i zwraca obiekt Route gdzie miasto docelowe jest takie jak przekazujemy w parametrze
 *@param finished kontener który przechowywuje połączenia
 *@param destination miasto końcowe według którego chcemy przeszukać kontener
 *@returns zwraca obiekt Route
 */
Route returnRoute(const std::vector<Route>& finished, std::string destination);

/**
 * Funkcja sprawdza czy lokalizacja jest dodana do vectora wktórym znajdują się już obliczone odległości ale nie koniecznie najkrótsze
 * @param nodeName przechowywuje nazwę wierzchołka - wierzchołek to miasto
 * @param finished kontener który przechowywuje połączenia
 * @return funkcja zwraca wartość 'true' jeżeli podane miasto znajduje się już w wektorze
 */
bool isInFinished(const std::vector<Route> &finished, const std::string& nodeName);

/**
 *Funkcja wyliczająca trasę oraz zapisująca połączenia do konteneru finished
 *@param finished kontener który przechowywuje połączenia
 *@param working TODO
 *@param routes mapa zawierająca wszystkie połącznenia pomiędzy miastami razem z kilometrami
 *@param start miejscowość z której szukamy połączenia
 *@param finish miejscowość do której szukamu połączenia
 */
void calculateRoute(std::vector<Route>& finished, std::multiset<Route>& working, std::map<std::string, std::vector<Route> >& routes, const std::string& start, const std::string& finish);

/**
 * Funkcja znajduje i zwraca najkrótszą odległość od miasta pocątkowego do końcowego
 * @param finished przechowywuje wszystkie do danego miasta (nie tylko najkrótsze)
 * @param finish miasto docelowe
 */
size_t getShortestRoute(const std::vector<Route>& finished, const std::string& finish);

/**
 *Funkcja wyznacza scieżkę formatuje ją i zapisuje do kontenera z gotowymi ścieżkami
 *@param finished kontener który przechowywuje połączenia
 *@param start miejscowość z której szukamy połączenia
 *@param finish miejscowość do której szukamu połączenia
 *@param finalPath ścieżki poprawnie sformatowane gotowe do zapisania w pliku
 */
void getFinalPathWithDistance(const std::vector<Route> &finished, const std::string& start, const std::string& finish, std::vector<std::string>& finalPath);
