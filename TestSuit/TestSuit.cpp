// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Suit.h"

class TestCase : public TestSuit::Case
{
public:
	TestCase(){}
    virtual ~TestCase() {}

    virtual void Run() override
    {
        printf("one method run.\n");
    }
};


int main()
{
    using namespace TestSuit;

    Suit<TestCase> suit;
    Suit<> suit2;

    suit.Start();
    suit2.Start();

    getchar();
    return 0;
}

