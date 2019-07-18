# 项目简介

TestSuit是一个简单的测试架构，包含两个概念：
* Suit
* Case

## Case
一个简单的运行代码封装，有一个 **Run()** 函数用于执行测试代码。

## Suit
封装多个Method对象实例，可以运行多个 **Case** 对象，本身可以进行一些测试代码的准备工作，比如构建测试测试对象等。

## 使用方式
包含头文件Suit.h

### 创建Case
```C++
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
```

### 创建Suit
```C++
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
```

### 使用并运行
```C++
// assembly the cases to be run.
ExampleSuit<ExampleCase1, ExampleCase1, ExampleCase2, ExampleCase3> suit;
suit.Start();
```
或者手动添加Case
```C++
auto uniqueCase = std::make_unique<ExampleCase2>();
suit.AddCase(std::move(uniqueCase));
```

#### 注意
* 如果你的Case没有无参构造函数，则必须使用[手动添加](#user-content-使用并运行)的方式添加Case。
