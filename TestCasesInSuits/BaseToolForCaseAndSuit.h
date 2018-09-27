#pragma once
/*!
    \brief this head file define may useful base class for Case and Suit.
*/

#include "CommonHeaders.h"
#include <filesystem>
#include <functional>
#undef RGB

using namespace CommonClass;



/*!
    \brief base case class that have common function for and useful Environment for pipline testing.
    for the Suit class, you should use SuitForPipline.
*/
class CaseForPipline : public TestSuit::Case
{
protected:
    using Super = CaseForPipline;

public:
    using SimplePoint = GraphicToolSet::SimplePoint;
    using CommonRenderingBuffer = GraphicToolSet::CommonRenderingBuffer;

protected:

    /*!
        \brief the random number generator.
    */
    RandomTool::MTRandom mtr;

    /*!
        \brief a common path for output, which will have a back slash at the end of the string.
    */
    const std::wstring OUTPUT_PATH = L"../OutputTestImage/";

    GraphicToolSet graphicToolSet;

public:
    CaseForPipline(const std::string& caseName) : Case(caseName) {}

    void Run() {}

    /*!
        \brief fill in the mesh data with sphere ray line list.
        \param outVetexData return vertex data
        \param outIndices return indices in line list.
    */
    void BuildSphereRayLineData(std::vector<SimplePoint>& outVertexData, std::vector<unsigned int>& outIndices);

    /*!
        \brief clamp the channel to [0.0f, 1.0f]
        \param ch the channel value to be clamped.
    */
    Types::F32 ClampChannel(Types::F32 ch);

    /*!
        \brief get random rgb color.
    */
    CommonClass::RGB GetRandomRGB();

    /*!
        \brief random vector, all component is in [0.0, 1.0], this can be used as a color.
    */
    vector3 GetRandomVector3ForRGB();

    /*!
        \brief get random vector3.
        if you want to set the seed, try mtr.SetRandomSeed(...);
    */
    CommonClass::vector3 GetRandomVector3(bool allowZeroVector = true, const int MAX_INT = 200);

    /*!
        \brief create a random float number between [-MAX_RADIUS, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloat(const Types::F32 MAX_RADIUS = 200.0f);
    
    /*!
        \brief create a float number between [-MAX_RADIUS, 0) ^ (0, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloatNotZero(const Types::F32 MAX_RADIUS = 200.0f);

    /*!
        \brief create an array contain random float  [-200.0f, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloat(MAX_RADIUS);
        }
        return retArr;
    }
    
    /*!
        \brief create a float number between [-200.0f, 0) ^ (0, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatNotZeroArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloatNotZero(MAX_RADIUS);
        }
        return retArr;
    }

    /*!
        \brief show the image and block the thread until user close the window.
        \param pImg the image to be shown
        \param title the title of the window
    */
    void BlockShowImg(const Image * pImg, const std::wstring title) const;

    /*!
        \brief create a quad poly.
        \param p1-4 four point position
        remember the clockwise is front face.
        A-------->
        |
        |   front order
        |
        <--------
    */
    std::unique_ptr<CommonClass::Polygon> CreatQuadPoly(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4);

    /*!
        \brief help to get point position in an array whose size is 8.
    */
    enum PointsOrder
    {
        TOP_LEFT_FRONT = 0,
        TOP_LEFT_BACK,
        TOP_RIGHT_BACK,
        TOP_RIGHT_FRONT,
        BOTTOM_LEFT_FRONT,
        BOTTOM_LEFT_BACK,
        BOTTOM_RIGHT_BACK,
        BOTTOM_RIGHT_FRONT
    };


    /*!
        \brief build six poly for a box
        \param points eight points to consist a box,
        \return 6 polygons which is visible from the outside of the box, polygons are in the order of {top, bottom, left, right, front, back}
        first four points is on the top, ranged in clockwise
        last four points is on the bottom, ranged ALSO in clockwise
        --------->
        A        |  \
        |  top   |   \
        |start   V    \
        *<--------     V
        \       ---------->
         \      A         |
          \     |   bottom|
           \    |start    V
            V   *<---------

    */
    std::array<std::unique_ptr<CommonClass::Polygon>, 6> CreatBox(const std::array<vector3, 8>& points);

    /*!
        \brief get the path for output files.
    */
    virtual std::wstring GetStoragePath() const;

    /*!
        \brief get safe storage path, if the path not exist, create it right now.
    */
    virtual std::wstring GetSafeStoragePath() const;

    /*!
        \brief save the image to the sub folder with specific name, and show the image in a block thread with the title as same as the image.
        \param pipline the pipline to show
        \param nameOfImgAndWnd image name without .png and the window title at the same time
    */
    void SaveAndShowPiplineBackbuffer(const Pipline & pipline, const std::wstring& nameOfImgAndWindow) const;
    
    void SaveAndShow(Image& img, const std::wstring& nameWithOutExt) const;
};

/*!
    \Brief base suit for pipline test, this class will have an special environment, 
    to fully take use of this suit, please inherit CaseForPipline to write you own case.
*/
template<typename ... CASE_TYPE_LIST>
class SuitForPipline: public TestSuit::Suit<CASE_TYPE_LIST...>
{
public:
};

