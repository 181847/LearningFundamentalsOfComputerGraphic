// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Suit.h"
#include <Windows.h>

class TestCase : public TestSuit::Case
{
public:
    TestCase() :Case("TestCase") {}

    virtual void Run() override
    {
        {

            COUNT_DETAIL_TIME;
            Sleep(83);
        }
        Sleep(5);
        printf("one method run.\n");
    }
};

class TestCase2 : public TestSuit::Case
{
public:
    TestCase2() :Case("TestCase2") {}

    virtual void Run() override
    {
        COUNT_DETAIL_TIME;
        Sleep(69);
        printf("second method run.\n");
    }
};

class TestCase3 : public TestSuit::Case
{
public:
    TestCase3() :Case("TestCase3") {}

    virtual void Run() override
    {
        COUNT_DETAIL_TIME;
        Sleep(3);
        printf("third method run.\n");
    }
};

class MySuit : public TestSuit::Suit<TestCase, TestCase2, TestCase3>
{
public:
    virtual void PrepareTotal() override
    {
        printf("MySuit called PrepareTotal\n");
    }

    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
        printf("MySuit called PrepareBeforeEachCase\n");
        return nullptr;
    }
};

template<typename ...CASE_LIST>
class MySecondSuit : public TestSuit::Suit<CASE_LIST...>
{

public:
    virtual void PrepareTotal() override
    {
        printf("**** THIS IS A GENERAL TEST SUIT FOR CAST_LIST.\n");
    }

    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
        printf("****** CASE START WITH: %s\n", pTheCase->GetName().c_str());
        return nullptr;
    }
};


int main()
{
    using namespace TestSuit;

    MySuit mySuit;
    mySuit.Start();


    printf("\n\n\ntemplate test suit\n\n");
    MySecondSuit<TestCase, TestCase2, TestCase3> mySuit2;
    mySuit2.Start();

    getchar();
    return 0;

}

