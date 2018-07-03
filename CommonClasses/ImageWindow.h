#pragma once
#include "Image.h"

namespace CommonClass
{

class ImageWindow
{
public:
    ImageWindow(const Image* img, const std::wstring& title = L"BitImage");
    ~ImageWindow();

    /*!
        \brief show the image window but block the current thread.
    */
    void BlockShow();

    /*!
        \brief paint image data to the window
    */
    void PaintToWindow();

private:

    /*!
        \brief fill the image pixel into the memory of the bit map
    */
    void FillInBMP(void * pBitMapMemory);

public:

    /*!
        \brief the title of the show window
    */
    std::wstring   m_title;

private:
    const Image*   m_pImg;        // the image to be shown

    unsigned long  m_hWnd = 0;    // the handle the image window
};

}// namespace CommonClass
