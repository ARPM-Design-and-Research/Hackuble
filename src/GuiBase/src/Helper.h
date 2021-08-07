#pragma once

#include <sstream>
#include <iomanip>

/* Some helper function that are useful throughout the application
* but don't have any specific context that they need to be tied to
*/

//Converts float to string to 2 decimal places
std::string floatToString(float v);