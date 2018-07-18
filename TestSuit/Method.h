#pragma once

namespace TestSuit
{

/*!
    \brief one Method instance corresponds to on test case
    each instance will only have one function to run the test code.
*/
class Method
{
public:
    Method() {}
    virtual ~Method() {}

    virtual void Run() = 0;
};

}// namespace TestSuit


