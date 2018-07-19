#pragma once
#include <memory>
#include <vector>
#include <stdio.h>
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
            // preparations
            void * pEnvironment = PrepareBeforeEachCase(theCase.get());
            theCase->SetEnvironment(pEnvironment);

            // set up time counter
            TimeCounter outer;
            TimeCounter inner;
            theCase->SetProgramableTimeCounter(&inner);

            // run test case
            try
            {// counting time for one case
                TimeGuard COUNT_THIS_CASE(outer);
                theCase->Run();
            }
            catch (std::exception excep)
            {
                printf("exception!! Message: %s\n", excep.what());
            }

            // print running time.
            printf("Case terminate: %s \nCost %lld %s for total.\nCost %lld %s for detail.\n", 
                theCase->GetName().c_str(),
                outer.m_sumDuration.count(), outer.DURATION_TYPE_NAME.c_str(),
                inner.m_sumDuration.count(), inner.DURATION_TYPE_NAME.c_str());
            
            FinishEachCase(theCase.get());
        }// end for each case
        FinishAllCases();
    }// end Start()

    /*!
        \brief prepare options for entire suit.Start()
    */
    virtual void PrepareTotal() { printf("Test Suit Start!"); }

    /*!
        \brief preparations before each case start.
        \param pTheCase point to the Case
        \return return a pointer which will be passed to the Case before it Run().
    */
    virtual void * PrepareBeforeEachCase(Case * pTheCase) { return nullptr; }

    /*!
        \brief finish codes when one Case::Run() finished.        
        \param pTheCase the latest Case that had run.
    */
    virtual void FinishEachCase(Case * pTheCase) {}

    /*!
        \brief finish codes when all cases run over.        
    */
    virtual void FinishAllCases() { printf("Test suit stop!"); }
};

} // namespace TestSuit
