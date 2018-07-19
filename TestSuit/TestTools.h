#pragma once
#include <chrono>
#include <string>

/*!
	\brief useful tool for test, for example the time counter, OR the assertion function.
*/

namespace TestSuit
{



/*!
    \brief TimeCounter used to record time for the test.
    each unit test will have one TimeCounter inside it,
    user can reuse it in the code to accumulate the expected time to be recored (NOT the time the whole testUnit costs).
    For example, in one test, you may do some calculations, then write the result into the local file.
    What you concen about is how many time it costs to calculate the result,
    rather than writing result to the local file(writing will be slow if the result is large such as 2GB).
    Below is the case for using the timeCounter:
    {
        TimeGuard _dummy(timeCounter);

        // the codes you want to count time on.
    }// stop on counting

    WARNING! If you use TimeCounter in cascaded, the overlaped duration time will be acummulated.
*/
class TimeCounter
{
public:
	/*!
		\brief which unit to measure time elapsed.
	*/
	using DurationType = std::chrono::microseconds;

	/*!
		\brief give the time unit a name to be printed in the output, e.g. "ms" and the out put may look like '... 45654 ms ...'
	*/
	const std::string DURATION_TYPE_NAME = "us";

    /*!
        \brief the sum of the time that have been recorded.
    */
    DurationType m_sumDuration = DurationType::zero();

public:
	TimeCounter(const TimeCounter&) = delete;
	TimeCounter& operator = (const TimeCounter&) = delete;
};

/*!
    \brief use RAII to record a period of time and add them to TimeCounter(argument of the constructor).
*/
struct TimeGuard
{
private:
    TimeCounter& m_tarteTimeCounter;
    std::chrono::system_clock::time_point m_startTimePoint;

public:
	/*!
		\brief start counting and the time elapsed will store in 'addTo'
	*/
    TimeGuard(TimeCounter& addTo)
        :m_tarteTimeCounter(addTo)
    {
        m_startTimePoint = std::chrono::system_clock::now();
    }

    ~TimeGuard()
    {
        auto endTimePoint = std::chrono::system_clock::now();

        m_tarteTimeCounter.m_sumDuration += std::chrono::duration_cast<TimeCounter::DurationType>(endTimePoint - m_startTimePoint);
    }

	TimeGuard(const TimeGuard&) = delete;
	TimeGuard& operator = (const TimeGuard&) = delete;
};

}// namespace TestSuit
