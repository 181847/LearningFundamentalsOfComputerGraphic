#pragma once

#include "Miscellaneous.h"
#include <assert.h>

namespace TestSuit
{

/*!
    \brief one Case instance corresponds to on test case
    each instance will only have one function to run the test code.
*/
class Case
{
protected:
    /*!
        \brief a programable timecounter to be used int Run() function.
    */
    TimeCounter* m_pProgramableTimeCounter;

    /*!
        \brief a helpful macro to count time elapsed inside a field,
        which is stored in m_pDetailTimeCounter.
    */
#define COUNT_DETAIL_TIME assert(nullptr != this->m_pProgramableTimeCounter && "The programable time counter has not been set!");\
TestSuit::TimeGuard LOCAL_DETAIL_TIME_COUNTER_GUARD(*(this->m_pProgramableTimeCounter))

    /*!
        \brief name of this Case.
    */
    std::string m_name;

public:
    Case(const std::string& name): m_name(name) {}
    Case(const Case&) = delete;
    Case& operator=(const Case&) = delete;
    virtual ~Case() {}

    /*!
        \brief set a programable timeCounter into the Case.
        this time counter can be used by the codes to specific which part of your codes
        need time evaluation.
    */
    void SetProgramableTimeCounter(TimeCounter* pDetailTimeCounter)
    {
        m_pProgramableTimeCounter = pDetailTimeCounter;
    }

    /*!
        \brief before each case run, this function will be called to 
        receive an environment, which basically is just additional information
        that depends on the Suit you are using.
        If you will, you can just ignore this function,
        or you can store it and use it later(Run).
        \param pEnvironment a pointer to some info which is depended on which Suit you are using.
    */
    virtual void SetEnvironment(void * pEnvironment) {};

    virtual void Run() = 0;

    /*!
        \brief get the name of this case.        
    */
    std::string GetName()
    {
        return m_name;
    }
};

}// namespace TestSuit

