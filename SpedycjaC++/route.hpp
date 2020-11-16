//
//  route.hpp
//  SpedycjaC++
//
//  Created by Jan Piaskowy on 15/11/2020.
//  Copyright © 2020 Jan Piaskowy. All rights reserved.
//

#ifndef route_hpp
#define route_hpp

#include <stdio.h>
#include <string>

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

#endif /* route_hpp */
