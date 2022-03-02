#include <Windows.h>
#include <iostream>
#include "window.hpp"
extern bool isRunning;


Window::Window(int width, int height, std::string title)
    :   m_hInstance(GetModuleHandle(NULL)),
        m_hwnd(0),
        m_width(width),
        m_height(height), 
        m_title(title)
{
    WindowCreate();

}
HWND Window::getHWND()
{
    return m_hwnd;
}

HINSTANCE Window::getHINSTANCE()
{
    return m_hInstance;
}


LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;


        case WM_KEYDOWN:
            if(wParam == VK_SPACE){
                DestroyWindow(hwnd);
                break;
            }

        case WM_DESTROY:
            PostQuitMessage(0);
            isRunning = false;
            break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void Window::WindowCreate()
{

    const char *CLASS_NAME = "myClass";
    
    WNDCLASS wc = {0};
    wc.hInstance = m_hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(0, CLASS_NAME, m_title.c_str() , WS_OVERLAPPEDWINDOW, 0, 0, m_width, m_height, NULL, NULL, m_hInstance, NULL);

    if(!m_hwnd){

        std::cout << "CreateWindowEX() failed \n";

    }
    ShowWindow(m_hwnd, 1);

}

bool Window::GetMessages()
{
    MSG msg;

    while(PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)){

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    return true;
}


Window::~Window()
{

}
