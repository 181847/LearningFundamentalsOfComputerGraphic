// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Suit.h"

class TestMethod : public TestSuit::Method
{
public:
    TestMethod(){}
    virtual ~TestMethod() {}

    virtual void Run() override
    {
        printf("one method run.\n");
    }
};


int main()
{
    printf("hellow\n");

    using namespace TestSuit;

    Suit<TestMethod> suit;
    Suit<> suit2;

    suit.Start();
    suit2.Start();

    getchar();
    return 0;
}

