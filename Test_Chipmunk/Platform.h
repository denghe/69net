#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "Lib/All.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "OpenGl32.lib")
#pragma comment(lib, "glu32.lib")


namespace Platform
{
    LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam );

    class GLWindow
    {
    public:
        GLWindow();
        ~GLWindow();
        bool init( wchar_t* n, int w, int h, int x = 0, int y = 0, wchar_t* t = L"", WNDPROC proc = &defaultProc );
        bool setVsync( bool enable = true );
        void setAutoSwapBuffer( bool enable = true );
        void swapBuffer();
        void run( std::function<void()> updater );
    private:
        HINSTANCE _app;
        HWND _wnd;
        WNDCLASS _wc;
        wchar_t* _cn;
        WNDPROC _proc;
        HDC _dc;
        HGLRC _rc;
        bool _autoSwap;
    };

}

#endif
