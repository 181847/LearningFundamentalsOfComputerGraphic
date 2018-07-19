#pragma once

namespace TestSuit
{

/*!
    \brief one Case instance corresponds to on test case
    each instance will only have one function to run the test code.
*/
class Case
{
public:
    Case() {}
    virtual ~Case() {}

    virtual void Run() = 0;
};

}// namespace TestSuit


