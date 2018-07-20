// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Suit.h"
#include <Windows.h>

/*!
    \brief example case
*/
class ExampleCase1 : public TestSuit::Case
{
private:
    int * pInt; // store temp environment pointer.

public:
    /*!
        \brife called the constructor of the Case, to set it's Name
    */
    ExampleCase1() :Case("Kratos") {}

    /*!
        \brief store environtment value passed by the host suit.
        There is no need to worry about the memory of the suit.
    */
    virtual void SetEnvironment(void * pEnvironment) override
    {
        pInt = reinterpret_cast<int*>(pEnvironment);
    }

    /*!
        \brief main code to be run.
    */
    virtual void Run() override
    {
        {
            COUNT_DETAIL_TIME;  // this macro will count the time between its construction and deconstruction.
            printf("The content value is %d.\n", *pInt);
            Sleep(83);
        }
        Sleep(5);
        printf("one method run.\n");
    }
};

class ExampleCase2 : public TestSuit::Case
{
public:
    ExampleCase2() :Case("Rocky") {}

    virtual void Run() override
    {
        COUNT_DETAIL_TIME;
        Sleep(69);
        printf("second method run.\n");
    }
};

class ExampleCase3 : public TestSuit::Case
{
public:
    ExampleCase3() :Case("Boy") {}

    virtual void Run() override
    {
        COUNT_DETAIL_TIME;
        Sleep(3);
        printf("third method run.\n");
    }
};

/*!
    \brief you can fix the Cases that you will run,
    or just treat them as another template parameters pack.
*/
template<typename ...CASE_TYPE_LIST>
class ExampleSuit : public TestSuit::Suit<CASE_TYPE_LIST...>
{
public:
    /*!
        \brief run before all start.
    */
    virtual void PrepareTotal() override
    {
        printf("MySuit called PrepareTotal\n");
    }

    /*!
        \brief run before each case run.
        \param pTheCase the case that will be run, don't worry about the memory of pTheCase right now.
        \return a pointer can point to any thing, this pointer will be passed to the case.
    */
    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
        printf("MySuit called PrepareBeforeEachCase\n");
        int * pInt = new int(8);
        return pInt;
    }

    /*!
        \brief run after each case
        \param pTheCase the case stops
        \param pEnvironment the pointer that be returned by PrepareBeforeEachCase(...), 
               Please take care of the memory where the pointer points to by yourself.
    */
    virtual void FinishEachCase(TestSuit::Case * pTheCase, void * pEnvironment) override
    {
        printf("Finish the case, the number is %d\n", *reinterpret_cast<int*>(pEnvironment));

        // clear environment
        delete reinterpret_cast<int*>(pEnvironment);
    }

    /*!
        \brief run when all case finished.
    */
    virtual void FinishAllCases() override
    {
        printf("Example suit finished\n");
    }
};


int main()
{
    using namespace TestSuit;

    // assembly the cases to be run.
    ExampleSuit<ExampleCase1, ExampleCase1, ExampleCase2, ExampleCase3> suit;
    
    auto uniqueCase = std::make_unique<ExampleCase2>();
    suit.AddCase(std::move(uniqueCase));

    suit.Start();

    getchar();
    return 0;

}

