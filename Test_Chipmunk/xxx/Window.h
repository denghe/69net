#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

namespace xxx
{
    LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam );

    class Window
    {
    public:
        Window();
        ~Window();
        bool Init( wchar_t* _className, int _width, int _height, int _x = 0, int _y = 0, bool _doubleBuffer = true, wchar_t* _title = L"", WNDPROC _wndProc = &defaultProc );
        bool SetVsync( bool _enable = true );
        void SetAutoSwapBuffer( bool _enable = true );
        void SwapBuffer();
        void Loop( std::function<void()> _updater );
        std::function<void()> resizeCallback;
        //protected:
        HINSTANCE app = GetModuleHandle( NULL );
        HWND wnd = nullptr;
        WNDCLASS wndClass;
        wchar_t* className = nullptr;
        WNDPROC wndProc = nullptr;
        HDC dc = nullptr;
        HGLRC rc = nullptr;
        bool autoSwap = true;
        bool doubleBuffer = true;
        int width = 0, height = 0;
        std::function<void()> update;
    };
}

#endif
