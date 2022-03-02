#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Window{

    private:
        HINSTANCE m_hInstance;
        HWND m_hwnd;
        int m_width;
        int m_height;
        std::string m_title;

        void WindowCreate();

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    public:
        Window(int width, int height, std::string title);
        ~Window();

        bool GetMessages();
        HWND getHWND();
        HINSTANCE getHINSTANCE();


};