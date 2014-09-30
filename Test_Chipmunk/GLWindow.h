#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam );

class GLWindow
{
public:
    GLWindow();
    ~GLWindow();
    bool init( wchar_t* n, int w, int h, int x = 0, int y = 0, bool doubleBuffer = true, wchar_t* t = L"", WNDPROC proc = &defaultProc );
    bool setVsync( bool enable = true );
    void setAutoSwapBuffer( bool enable = true );
    void swapBuffer();
    void loop( std::function<void()> updater );
//private:
    HINSTANCE _app = GetModuleHandle( NULL );
    HWND _wnd = nullptr;
    WNDCLASS _wc;
    wchar_t* _cn = nullptr;
    WNDPROC _proc = nullptr;
    HDC _dc = nullptr;
    HGLRC _rc = nullptr;
    bool _autoSwap = true;
    bool _doubleBuffer = true;
    int _w = 0, _h = 0;
};

#endif
