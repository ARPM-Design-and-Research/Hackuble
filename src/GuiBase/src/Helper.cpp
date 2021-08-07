#pragma once

#include "Helper.h"


//Converts float to string with 2 decimal place precision. Otherwise, the decimal places are large
std::string floatToString(float v) {
    std::ostringstream streamObj;
    streamObj << std::fixed;
    streamObj << std::setprecision(2);
    streamObj << v;

    return streamObj.str();
}