#pragma once
#include <memory>
#include <vector>
#include "Case.h"

namespace TestSuit
{

/*!
    \brief a Suit will create multiple Case instance and run their test code.
    your job is to wrap the desired Case object into the Suit, and called Start();
*/
template<typename ...CASE_TYPE_LIST>
class Suit
{
public:
    std::vector<std::unique_ptr<Case>> m_cases;

public:
	/*!
		\brief construct the cast list with all the types listed in the template parameters.
	*/
    Suit()
    {
        InitDefaultCases();
    }
    virtual ~Suit() {}

private:
    /*!
        \brief initialize all the cases in the template parameters.
    */
    void InitDefaultCases()
    {
        std::unique_ptr<Case> list[] = { nullptr /* prevent array has size of ZERO */, std::make_unique<CASE_TYPE_LIST>()... };
        const int num = sizeof...(CASE_TYPE_LIST);

        for (int i = 1; i <= num; ++i)
        {
            m_cases.push_back(std::move(list[i]));
        }
    }

public:
    /*!
        \brief add a theCase to run.
        \param case pointer 
    */
    void AddCase(std::unique_ptr<Case> theCase)
    {
		m_cases.push_back(std::move(theCase));
    }

    /*!
        \brief start run the theCases
    */
    void Start()
    {
        PrepareTotal();
        for (auto & theCase : m_cases)
        {
            PrepareBeforeEachCase();
            theCase->Run();
        }
    }

    /*!
        \brief prepare options for entire suit.Start()
    */
    virtual void PrepareTotal() {}

    /*!
        \brief preparations before each case start.
    */
    virtual void PrepareBeforeEachCase() {}
};

} // namespace TestSuit
