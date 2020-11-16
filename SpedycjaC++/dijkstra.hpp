//
//  dijkstra.hpp
//  SpedycjaC++
//
//  Created by Jan Piaskowy on 15/11/2020.
//  Copyright © 2020 Jan Piaskowy. All rights reserved.
//

#ifndef dijkstra_hpp
#define dijkstra_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>

struct Route {
    std::string destination;
    std::string ancestor;
    size_t distance;
    
    Route(std::string destination, size_t distance) {
        this->distance = distance;
        this->destination = destination;
    }
    
    Route(std::string destination, std::string ancestor, size_t distance) {
        this->distance = distance;
        this->destination = destination;
        this->ancestor = ancestor;
    }
    
    Route(){}
    
    bool operator < (const Route &r) const {
        return (distance < r.distance);
    }
};

Route returnRoute(const std::vector<Route>& finished, std::string destination) {
    Route buffRoute = *new Route();
    
    for (auto route : finished) {
        if (route.destination == destination) {
            buffRoute = route;
        }
    }
    return buffRoute;
}

bool isInFinished(const std::vector<Route> &finished, const std::string& nodeName) {
    bool isFound = false;

    for (auto route : finished) {
        if (route.destination == nodeName) {
            isFound = true;
            break;
        }
    }
    return isFound;
}


void calculateRoute(std::vector<Route>& finished, std::multiset<Route>& working, std::map<std::string, std::vector<Route> >& routes, const std::string& start, const std::string& finish) {
    
    working.insert(*(new Route(start, 0)));
    
    for (auto workRoute : working) {
        
        if (workRoute.destination == finish) {
            finished.push_back(workRoute);
        }
        
        if (isInFinished(finished, workRoute.destination))
            continue;
        
        finished.push_back(workRoute);
        
        for (auto route : routes[workRoute.destination]) {
            std::string destination = route.destination;
            
            if (!(isInFinished(finished, destination))) {
                size_t distanceCost = route.distance + workRoute.distance;
                working.insert(*(new Route(destination, workRoute.destination, distanceCost)));
            }
        }
    }
}

size_t getShortestRoute(const std::vector<Route>& finished, const std::string& finish) {
    
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


void getFinalPathWithDistance(const std::vector<Route> &finished, const std::string& start, const std::string& finish, std::vector<std::string>& finalPath) {
    
    size_t distance;
    std::string path;
    std::string ancestor;
    std::vector<std::string> vPath;
    
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
        
        path.append(std::to_string(distance));
        finalPath.push_back(path);
        
        
    } catch (const char* error) {
        std::cerr << error << finish << std::endl;
    }
}

#endif /* dijkstra_hpp */
