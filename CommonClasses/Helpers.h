#pragma once
/*!
    \brief this head file define some help function, such as a function to format std::string as printf
*/

#include <string>
#include <memory>

/*!
    \brief printf for std::string
    Usage:
        std::string s = string_format("%f, %d -- [%f, %f]", 1.0, 2, 3.3f, 4.4f);
        
        s == "1.0, 2 -- [3.3, 4.4]";
    The code is coming from: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
*/
template<typename ... Args>
std::string string_format(const std::string& fmt, Args ... args)
{
    const size_t size = 1 + snprintf(nullptr, 0, fmt.c_str(), args ...);
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, fmt.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // construct with out the last '\0'
}
