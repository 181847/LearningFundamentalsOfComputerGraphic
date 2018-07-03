#include "ImageWindow.h"
#include <Windows.h>

namespace CommonClass
{

ImageWindow::ImageWindow(const Image * img)
    :pImg(img)
{
}

ImageWindow::~ImageWindow()
{
}


// the message proccesing function of the ImageWindow
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static unsigned char oneChannelValue = 0x01;

    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        printf("you have pressed left button.\n");
        return 0;

    case WM_DESTROY:            // window is destoried
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:              // user click close button of right upper corner of the window.
        DestroyWindow(hwnd);
        return 0;

    case WM_PAINT:              //repaint
    {
        PAINTSTRUCT ps = { 0 };
        HDC hdc = BeginPaint(hwnd, &ps);


        EndPaint(hwnd, &ps);
        return 0;
    }
    }// end switch
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}// end WndProc

/*!
    \brief helper class to set pixel value in the memory of HBITMAP
*/
class BMPSetterAgent
{
public:
    enum Ch
    {
        B = 0,
        G,
        R
    };

public:
    BMPSetterAgent(void * const pBMPMemory, const int width, const int height)
        :pMem(reinterpret_cast<unsigned char * const>(pBMPMemory)), WIDTH(width), HEIGHT(height)
    {
        // empty body
    }

    ~BMPSetterAgent() {}

    /*!
        \brief set one channel value
        \templateParameter _CH which channel to set（Ch::R, Ch::G, Ch::B）
        \param x horizontal position, from left to right
        \param y vertical position, from top to bottom
        \param value fill into the channel of one pixel
    */
    template<Ch _CH>
    void SetChannel(const int x, const int y, unsigned char value)
    {
        assert(0 <= x && x < WIDTH);
        assert(0 <= y && y < HEIGHT);

        pMem[PIXEL_SIZE * (y * WIDTH + x) + _CH] = value;

        // pMem layout:
        // as the address of pMem increase
        //  |------------------------|
        // 4|                        |
        // 3|       A BIT MAP        |
        // 2|                        |
        // 1|                        |
        // 0|P >=======[ B G R ]===>>|
        //  |A-----------------------|
        //   |
        //   |
        //   |--pMem starts from here
    }


private:
    unsigned char * const   pMem;
    const int               WIDTH, HEIGHT;
    const int               PIXEL_SIZE = 3; // each pixel have three bytes [R, G, B]
};

void ImageWindow::BlockShow()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc = { 0 };
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(wc);
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);             // window background
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));         // cursor type
    wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));       // cursor appearance
    wc.hIconSm = NULL;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;                                          // processing message function
    wc.lpszClassName = L"my_windowClass";
    wc.lpszMenuName = NULL;                                            // have no menu
    wc.style = CS_HREDRAW | CS_VREDRAW;                                // class style

    if (!RegisterClassEx(&wc))    //register window class
    {
        MessageBox(NULL, L"Register Class Failed!", NULL, MB_OK);
        return;
    }

    const int WIDTH = pImg->m_width, HEIGHT = pImg->m_height;
    RECT rect;
    rect.left = 0;
    rect.right = WIDTH;     // desired window width
    rect.top = 0;
    rect.bottom = HEIGHT;   // desired window height

    // you can find some parameter in next codes of CreateWindowEx
    AdjustWindowRectEx(
        &rect,
        WS_OVERLAPPEDWINDOW,// window style
        false,              // no menu
        0);                 // extend style

    HWND hWnd = CreateWindowEx(
        0,                      // extend style
        L"my_windowClass",
        L"这是窗口标题",
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // init position x(to be default)
        CW_USEDEFAULT,          // init position y(to be default)
        rect.right - rect.left, // the width after adjusting.
        rect.bottom - rect.top, // the height after adjusting.
        NULL,                   // parent HWND
        NULL,                   // no menu
        hInstance,
        NULL                    // additional parameter when MSG is WM_CREATE, usually to be ZERO
    );

    if (!hWnd)
    {
        MessageBox(NULL, L"CreateWindowEx Failed!", NULL, MB_OK);
        return;
    }


    UpdateWindow(hWnd);
    ShowWindow(hWnd, true);
    {
        HDC hdc = GetDC(hWnd);
        const int PIXEL_BYTES = 3;

        BITMAPINFO bmpInfo;                                     // bmp infomation, define the pixel bits/RGB/width/hegith/...

        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);    // size of the information, here we just set it to be default.
        bmpInfo.bmiHeader.biWidth = WIDTH;                      // width
        bmpInfo.bmiHeader.biHeight = HEIGHT;                    // height
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = PIXEL_BYTES * 8;         // bits per pixel
        bmpInfo.bmiHeader.biCompression = BI_RGB;               // compression as RGB, Warning, the real channel data is arranged as BGR.

        const int numBytes = WIDTH * HEIGHT * PIXEL_BYTES;

        // create BitMap and get its memory address
        void * pMem;// memory address of the BitMap
        HBITMAP hbmp = CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, &pMem, NULL, 0);
        assert(hbmp != NULL);

        FillInBMP(pMem);

        // copy BitMap to window.
        HDC bmpHdc = CreateCompatibleDC(hdc);
        SelectObject(bmpHdc, hbmp);
        BitBlt      (hdc, 0, 0, WIDTH, HEIGHT, bmpHdc, 0, 0, SRCCOPY);
        DeleteDC    (bmpHdc);

        printf("memset done.\n");

        DeleteObject(hbmp);
        DeleteObject(bmpHdc);
    }

    MSG msg;
    BOOL bRet;
    //进入消息循环
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != -1)
    {
        if (bRet == 0)
        {
            break;
        }

        TranslateMessage(&msg); //翻译消息
        DispatchMessage(&msg);  //分发消息
    }
    //return msg.wParam;          //WinMain函数结束, 整个程序退出
}

void ImageWindow::FillInBMP(void * pBitMapMemory)
{
    const int WIDTH = pImg->m_width, HEIGHT = pImg->m_height;
    BMPSetterAgent bmp(pBitMapMemory, WIDTH, HEIGHT);

    //unsigned char * pMem = reinterpret_cast<unsigned char *>(pBitMapMemory);
    //for (int index = 0; index < WIDTH * HEIGHT; ++index)
    //{
    //    auto pixel = pImg->m_canvas[index];
    //    // remember the rgb channel in the bit map is arranged by the order of [Blue -> Green -> Red]
    //    pMem[index * 3 + BMPSetterAgent::B] = pixel.m_b;
    //    pMem[index * 3 + BMPSetterAgent::G] = pixel.m_g;
    //    pMem[index * 3 + BMPSetterAgent::R] = pixel.m_r;
    //}

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            auto const & pixel = pImg->GetRawPixel(x, y);
            bmp.SetChannel<BMPSetterAgent::B>(x, y, pixel.m_b);
            bmp.SetChannel<BMPSetterAgent::G>(x, y, pixel.m_g);
            bmp.SetChannel<BMPSetterAgent::R>(x, y, pixel.m_r);
        }
    }
}

}// namespace CommonClass
