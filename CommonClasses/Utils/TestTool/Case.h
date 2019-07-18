#pragma once

#include "Miscellaneous.h"
#include <assert.h>
#include <iostream>

#ifdef WIN32
#include <intrin.h>
#endif // end ifdef WIN32

namespace TestSuit
{

class SuitInterface;

/*!
    \brief one Case instance corresponds to on test case
    each instance will only have one function to run the test code.
*/
class Case
{
protected:
    friend class SuitInterface;
    /*!
        \brief time counter that can be controlled.
    */
    TimeCounter m_ProgramableTimeCounter;

private:
    /*!
        \brief count the errors.
    */
    int m_errorCount = 0;

    /*!
        \brief a helpful macro to count time elapsed inside a field,
        which is stored in m_pDetailTimeCounter.
    */
#define COUNT_DETAIL_TIME TestSuit::TimeGuard LOCAL_DETAIL_TIME_COUNTER_GUARD((this->m_ProgramableTimeCounter))

    /*!
        \brief name of this Case.
    */
    std::string m_name;

public:
    Case(const std::string& name): m_name(name) {}
    Case(const Case&) = delete;
    Case& operator=(const Case&) = delete;
    virtual ~Case() {}

    virtual void Run() = 0;

    /*!
        \brief get the name of this case.        
    */
    std::string GetName()
    {
        return m_name;
    }
	
	/*!
		\brief print a progress percentage into the screen(same line).
		\param progress a number between 0.0f and 1.0f which will be interpret to 0% to 100%.
		this is for some long term test to show some information to user.
	*/
	void ShowProgress(float progress) const
	{
		std::printf("Progress: %.4f%%\r", progress * 100.0f);
	}

protected:
    /*!
        \brief count one error.
    */
    void CountOneError()
    {
        ++m_errorCount;
    }

#ifdef WIN32
    // a help macro for assertion
#define TEST_ASSERT(expr) do {\
        if (!(expr)){\
            this->CountOneError();\
            __debugbreak();\
        }\
    } while(0)
#endif // end ifdef WIN32

public:
    /*!
        \brief return the number of errors.
    */
    int GetErrorCount()
    {
        return m_errorCount;
    }

};

}// namespace TestSuit

