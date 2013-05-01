//
//  StringHelper.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#ifndef __Adventures_on_the_farm__StringHelper__
#define __Adventures_on_the_farm__StringHelper__

#include <string>
#include <vector>

namespace StringHelper
{
    void replace(std::string &str, const std::string &from, const std::string &to);
    void trim(std::string &str);
    
    std::vector<std::string> split(const std::string &text, char sep);
}

#endif /* defined(__Adventures_on_the_farm__StringHelper__) */
