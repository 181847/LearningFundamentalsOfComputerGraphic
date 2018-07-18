#pragma once
#include <memory>
#include <vector>
#include "Method.h"

namespace TestSuit
{

/*!
    \brief a Suit will create multiple Method instance and run their test code.
    your job is to wrap the desired Method object into the Suit, and called Start();
*/
template<typename ...METHOD_TYPE_LIST>
class Suit
{
public:
    std::vector<std::unique_ptr<Method>> m_methods;

public:
    Suit()
    {
        std::unique_ptr<Method> list[] = { nullptr /* prevent array has size of ZERO */, std::make_unique<METHOD_TYPE_LIST>()... };
        const int num = sizeof...(METHOD_TYPE_LIST);
        
        for (int i = 1; i <= num; ++i)
        {
            m_methods.push_back(std::move(list[i]));
        }
    }
    ~Suit() {}

    /*!
        \brief add a method to run.
        \param method pointer 
    */
    void AddMethod(std::unique_ptr<Method> method)
    {
        m_methods.push_back(std::move(method));
    }

    /*!
        \brief start run the methods
    */
    void Start()
    {
        for (auto & method : m_methods)
        {
            method->Run();
        }
    }
};

} // namespace TestSuit
