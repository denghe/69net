#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam );

class GLWindow
{
public:
    GLWindow();
    ~GLWindow();
    bool Init( wchar_t* className, int width, int height, int x = 0, int y = 0, bool doubleBuffer = true, wchar_t* title = L"", WNDPROC wndProc = &defaultProc );
    bool SetVsync( bool enable = true );
    void SetAutoSwapBuffer( bool enable = true );
    void SwapBuffer();
    void Loop( std::function<void()> updater );
//private:
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
};

#endif
