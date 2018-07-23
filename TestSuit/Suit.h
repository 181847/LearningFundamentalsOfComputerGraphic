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

    the routine of Suit is running like this:
    Start()
    {
        PrepareTotal();
            foreach case:
                environment = PrepareBeforeEachCase(case);
                case.SetEnvironment(environment);
                // .... running ...
                FinishEachCase(case, environment);
        FinishAllCases();
    }
*/
template<typename ...CASE_TYPE_LIST>
class Suit
{
public:
    std::vector<std::unique_ptr<Case>> m_cases;

public:
    /*!
        \brief construct the case list with all the types listed in the template parameters.
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

            const std::string SUCCESS_MARK = "\\/\\/\\/\\/";
            const std::string FAILURE_MARK = "XXXXXX";

            std::string resultMark;

            if (theCase->GetErrorCount() > 0)
            {
                resultMark = FAILURE_MARK;
            }
            else
            {
                resultMark = SUCCESS_MARK;
            }// end theCase->GetErrorCoount()

            // print running time.
            printf("case: %32s <%8lld %s <-- %8lld %s --> >.\n Result: %s\n\n", 
                theCase->GetName().c_str(),
                outer.m_sumDuration.count(), outer.DURATION_TYPE_NAME.c_str(),
                inner.m_sumDuration.count(), inner.DURATION_TYPE_NAME.c_str(),
                resultMark.c_str());
            
            FinishEachCase(theCase.get(), pEnvironment);
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
        \brief finish codes when one Case::Run() finished (Even a exception happened.).        
        \param pTheCase the latest Case that had run.
        \param pEnvironment the environment pointer, PLZ DONT leak any memory of this pointer.
               You can clean up the environment here of memory safety.
    */
    virtual void FinishEachCase(Case * pTheCase, void * pEnvironment) {}

    /*!
        \brief finish codes when all cases run over.        
    */
    virtual void FinishAllCases() { printf("Test suit stop!"); }
};

} // namespace TestSuit
