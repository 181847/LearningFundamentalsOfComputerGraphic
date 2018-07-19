// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Suit.h"

class TestCase : public TestSuit::Case
{
    virtual void Run() override
    {
        printf("one method run.\n");
    }
};

class TestCase2 : public TestSuit::Case
{
    virtual void Run() override
    {
        printf("second method run.\n");
    }
};

class TestCase3 : public TestSuit::Case
{
    virtual void Run() override
    {
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

    virtual void PrepareBeforeEachCase() override
    {
        printf("MySuit called PrepareBeforeEachCase\n");
    }
};


int main()
{
    using namespace TestSuit;

    MySuit mySuit;
    mySuit.Start();

    getchar();
    return 0;
}

