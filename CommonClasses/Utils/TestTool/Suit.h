#pragma once
#include <memory>
#include <vector>
#include <stdio.h>
#include <future>
#include "Case.h"

namespace TestSuit
{

/*!
    \brief a basic Suit implementation, define some basic code, and the framework,
    this will ensure all the Suit instance can be stored by this kind of pointer.
*/
class SuitInterface
{
protected:
    using Super = SuitInterface;

public:
    std::vector<std::unique_ptr<Case>> m_cases;

public:
    /*!
        \brief construct the case list with all the types listed in the template parameters.
    */
    SuitInterface() {}
    virtual ~SuitInterface() {}

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
            PrepareBeforeEachCase(theCase.get());

            // set up time counter
            TimeCounter outer;

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
                theCase->m_ProgramableTimeCounter.m_sumDuration.count(), theCase->m_ProgramableTimeCounter.DURATION_TYPE_NAME.c_str(),
                resultMark.c_str());
            
            FinishEachCase(theCase.get());
        }// end for each case
        FinishAllCases();
    }// end Start()

    /*!
        \brief prepare options for entire SuitInterface.Start()
    */
    virtual void PrepareTotal() { printf("Test Suit Start!\n"); }

    /*!
        \brief preparations before each case start.
        \param pTheCase point to the Case
        \return return a pointer which will be passed to the Case before it Run().
    */
    virtual void PrepareBeforeEachCase(Case * pTheCase) {}

    /*!
        \brief finish codes when one Case::Run() finished (Even a exception happened.).        
        \param pTheCase the latest Case that had run.
        \param pEnvironment the environment pointer, PLZ DONT leak any memory of this pointer.
               You can clean up the environment here of memory safety.
    */
    virtual void FinishEachCase(Case * pTheCase) {}

    /*!
        \brief finish codes when all cases run over.        
    */
    virtual void FinishAllCases() { printf("Test Suit stop!\n"); }
};

/*!
    \brief a Suit will create multiple Case instance and run their test code.
    your job is to wrap the desired Case object into the Suit, whether by passing the Case type to the template paramters,
    or call AddCase() later.
    And the final work is to call Suit.Start() to fire up all the codes in those cases.

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
class Suit: public SuitInterface
{
protected:
    /*!
        \brief skip the template class, reference to the 
    */
    using Super = SuitInterface;
public:
    Suit()
    {
        InitDefaultCases();
    }
    
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
};

/*!
    \brief a helper class to Run multiple Suit at once.
*/
template<typename ... SUIT_TYPE_LIST>
class SuitQueueRunner
{
protected:
    std::vector<std::unique_ptr<TestSuit::SuitInterface>> m_suitQueue;

public:
    SuitQueueRunner()
    {
        std::unique_ptr<TestSuit::SuitInterface> list[] = { nullptr /* prevent array has size of ZERO */, std::make_unique<SUIT_TYPE_LIST>()... };
        const int num = sizeof...(SUIT_TYPE_LIST);

        for (int i = 1; i <= num; ++i)
        {
            m_suitQueue.push_back(std::move(list[i]));
        }
    }

    /*!
        \brief add on additional suit.
        \param suit the suit you want to run.
    */
    void AddSuit(std::unique_ptr<TestSuit::SuitInterface> suit)
    {
        m_suitQueue.push_back(std::move(suit));
    }

    /*!
        \brief fire up Suit::Start() for each suit instance.
    */
    void StartBatch()
    {
        for (auto& suit : m_suitQueue)
        {
            suit->Start();
        }
    }
};

} // namespace TestSuit
