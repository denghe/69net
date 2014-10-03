#include "Precompile.h"

namespace xxx
{

    // x, y
    std::pair<LONG, LONG> getTaskbarOffset()
    {
        auto rtv = std::make_pair( 0, 0 );
        RECT rect;
        HWND taskBar = FindWindow( L"Shell_traywnd", NULL );
        if( taskBar && GetWindowRect( taskBar, &rect ) )
        {
            if( rect.left <= 0 && rect.top <= 0 )
            {
                if( rect.right > rect.bottom )
                {
                    //printf( "top" );
                    rtv.second = rect.bottom;
                }
                else
                {
                    //printf( "left" );
                    rtv.first = rect.right;
                }
            }
            //else if( rect.left == 0 ) printf( "bottom" );
            //else printf( "right" );
        }
        return rtv;
    }



    LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
    {
        switch( uiMsg )
        {
        case WM_ACTIVATE:
        case WM_KEYDOWN:
        case WM_KEYUP:
            return 0;
        case WM_SIZE:
            G::window->width = LOWORD( lParam );
            G::window->height = HIWORD( lParam );
            if( G::window->resizeCallback ) G::window->resizeCallback();
            //if( G::glwindow->update ) G::glwindow->update();
            return 0;

        case WM_CLOSE:
            PostQuitMessage( 0 );
            return 0;

        case WM_TIMER:
            G::window->update();
            return 0;

            // 这样效果其实也不大好。鼠标于标题栏按下不动的 0.5 秒左右时长内会定住
        case WM_ENTERMENULOOP:
        case WM_ENTERSIZEMOVE:
            // http://stackoverflow.com/questions/3102074/win32-my-application-freezes-while-the-user-resizes-the-window
            SetTimer( hWnd,                 // handle to main window 
                      1,                    // timer identifier 
                      1,                    // 1 ms interval 
                      (TIMERPROC)NULL ); // no timer callback 
            break;

        case WM_EXITMENULOOP:
        case WM_EXITSIZEMOVE:
            KillTimer( hWnd, 1 );
            break;
        }

        return DefWindowProc( hWnd, uiMsg, wParam, lParam );
    }

    Window::Window()
    {
        G::window = this;
    }

    Window::~Window()
    {
        if( wnd )
        {
            wglMakeCurrent( nullptr, nullptr );
            ReleaseDC( wnd, dc );
            wglDeleteContext( rc );
            DestroyWindow( wnd );
            UnregisterClass( className, app );
        }
        G::window = nullptr;
    }

    bool Window::Init( wchar_t* _className, int _width, int _height, int _x /*= 0*/, int _y /*= 0*/, bool _doubleBuffer /*= true*/, wchar_t* _title /*= L""*/, WNDPROC _wndProc /*= &defaultProc*/ )
    {
        assert( !wnd );    // 防重复调用

        className = _className;
        wndProc = _wndProc;
        autoSwap = doubleBuffer = _doubleBuffer;
        if( !_title || !*_title ) _title = _className;

        RECT rect;
        rect.left = 0L;
        rect.top = 0L;
        rect.right = width = _width;
        rect.bottom = height = _height;

        wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpfnWndProc = _wndProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = app;
        wndClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
        wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
        wndClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );  // nullptr
        wndClass.lpszMenuName = nullptr;
        wndClass.lpszClassName = _className;
        if( !RegisterClass( &wndClass ) )
        {
            printf( "GetLastError() = %d", GetLastError() );
            return false;
        }

        AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
        auto offset = getTaskbarOffset();

        wnd = CreateWindowEx(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
            _className, _title,
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            _x + offset.first, _y + offset.second,
            rect.right - rect.left, rect.bottom - rect.top,
            NULL, NULL,
            app,
            NULL
            );

        if( !wnd )
        {
            printf( "GetLastError() = %d", GetLastError() );
            UnregisterClass( className, app );
            return false;
        }


        dc = GetDC( wnd );

        PIXELFORMATDESCRIPTOR pfd;
        memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
        pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        if( _doubleBuffer ) pfd.dwFlags |= PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = (BYTE)GetDeviceCaps( dc, BITSPIXEL );

        pfd.cRedBits = 8;
        pfd.cGreenBits = 8;
        pfd.cBlueBits = 8;
        pfd.cAlphaBits = 8;

        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        pfd.iLayerType = PFD_MAIN_PLANE;

        int iFormat = ChoosePixelFormat( dc, &pfd );
        SetPixelFormat( dc, iFormat, &pfd );

        rc = wglCreateContext( dc );
        wglMakeCurrent( dc, rc );

        ShowWindow( wnd, SW_SHOW );
        SetForegroundWindow( wnd );
        SetFocus( wnd );


        resizeCallback = []
        {
            if( G::scene )
            {
                G::scene->size = { (float)G::window->width, (float)G::window->height };
                G::scene->dirty = true;
            }
        };

        return true;
    }

    typedef BOOL( APIENTRY *PFNWGLSWAPINTERVALEXTPROC )( int );
    bool Window::SetVsync( bool _enable /*= true */ )
    {
        if( !wnd ) return false;
        char* extensions = (char*)glGetString( GL_EXTENSIONS );
        if( strstr( extensions, "WGL_EXT_swap_control" ) )
        {
            auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
            wglSwapIntervalEXT( _enable ? 1 : 0 );
            return true;
        }
        return false;
    }

    void Window::SwapBuffer()
    {
        assert( doubleBuffer );
        SwapBuffers( dc );
    }

    void Window::Loop( std::function<void()> _updater )
    {
        if( wndProc == &defaultProc )
        {
            if( autoSwap )
            {
                update = [ = ]
                {
                    _updater();
                    SwapBuffer();
                };
            }
            else
            {
                update = _updater;
            }
        }

        if( resizeCallback )resizeCallback();
        if( G::game )
        {
            G::game->Loaded();
        }

        MSG msg;
        bool running = true;
        while( running )
        {
            if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                if( msg.message == WM_QUIT )
                {
                    running = false;
                }
                else
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
            }
            update();
        }
    }

    void Window::SetAutoSwapBuffer( bool _enable /*= true */ )
    {
        autoSwap = _enable;
    }

}
