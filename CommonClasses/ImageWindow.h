#pragma once
#include "Image.h"

namespace CommonClass
{

class ImageWindow
{
public:
    ImageWindow(const Image* img);
    ~ImageWindow();

    /*!
        \brief show the image window but block the current thread.
    */
    void BlockShow();

private:
    /*!
        \brief fill the image pixel into the memory of the bit map
    */
    void FillInBMP(void * pBitMapMemory);

public:
    const Image* pImg;    // the image to be shown
};

}// namespace CommonClass
