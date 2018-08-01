#include <MyTools/TestTool/Suit.h>  // include test framework
#include "CommonHeaders.h"
// include required suits here
#include "TestBasicTypesSuit.h"
#include "CaseAndSuitForCommonClasses.h"
#include "CaseAndSuitForRayRender.h"
#include "CaseAndSuitForRasterisation.h"
#include "CaseAndSuitForRasterizeTriangle.h"

#pragma comment(lib, "MyTools/MyTool.lib")
#pragma comment(lib, "CommonClasses.lib")

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

int main()
{
    SuitQueueRunner<
        BasicTypesTestSuit_Runable, 
        SuitForCommonClasses,
        SuitForRayRender,
        SuitForRasterisation, 
        SuitForRasterizeTriangle
    > suitQueueRunner;

    suitQueueRunner.StartBatch();

    getchar();
    return 0;
}

