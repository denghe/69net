#include "Platform.h"



std::function<void()> defaultTimerProc;

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



LRESULT CALLBACK Platform::defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uiMsg )
    {
    case WM_ACTIVATE:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SIZE:
        return 0;

    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;

    case WM_TIMER:
        if( defaultTimerProc ) defaultTimerProc();
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

Platform::GLWindow::GLWindow()
    : _app( GetModuleHandle( NULL ) )
    , _wnd( nullptr )
    , _cn( nullptr )
    , _proc( nullptr )
    , _dc( nullptr )
    , _rc( nullptr )
    , _autoSwap( true )
{
}

Platform::GLWindow::~GLWindow()
{
    if( _wnd )
    {
        wglMakeCurrent( nullptr, nullptr );
        ReleaseDC( _wnd, _dc );
        wglDeleteContext( _rc );
        DestroyWindow( _wnd );
        UnregisterClass( _cn, _app );
    }
}

bool Platform::GLWindow::init( wchar_t* n, int w, int h, int x /*= 0*/, int y /*= 0*/, wchar_t* t /*= L""*/, WNDPROC proc /*= &defaultProc*/ )
{
    _cn = n;
    _proc = proc;

    RECT rect;
    rect.left = 0L;
    rect.right = w;
    rect.top = 0L;
    rect.bottom = h;

    _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    _wc.lpfnWndProc = proc;
    _wc.cbClsExtra = 0;
    _wc.cbWndExtra = 0;
    _wc.hInstance = _app;
    _wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
    _wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    _wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );  // nullptr
    _wc.lpszMenuName = nullptr;
    _wc.lpszClassName = n;
    if( !RegisterClass( &_wc ) )
    {
        printf( "GetLastError() = %d", GetLastError() );
        return false;
    }

    AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
    auto offset = getTaskbarOffset();

    _wnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        n, t,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        x + offset.first, y + offset.second,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL,
        _app,
        NULL
        );

    if( !_wnd )
    {
        printf( "GetLastError() = %d", GetLastError() );
        UnregisterClass( _cn, _app );
        return false;
    }


    _dc = GetDC( _wnd );

    PIXELFORMATDESCRIPTOR pfd;
    memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = (BYTE)GetDeviceCaps( _dc, BITSPIXEL );

    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    pfd.iLayerType = PFD_MAIN_PLANE;

    int iFormat = ChoosePixelFormat( _dc, &pfd );
    SetPixelFormat( _dc, iFormat, &pfd );

    auto _rc = wglCreateContext( _dc );
    wglMakeCurrent( _dc, _rc );

    ShowWindow( _wnd, SW_SHOW );
    SetForegroundWindow( _wnd );
    SetFocus( _wnd );

    return true;
}

typedef BOOL( APIENTRY *PFNWGLSWAPINTERVALEXTPROC )( int );
bool Platform::GLWindow::setVsync( bool enable /*= true */ )
{
    if( !_wnd ) return false;
    char* extensions = (char*)glGetString( GL_EXTENSIONS );
    if( strstr( extensions, "WGL_EXT_swap_control" ) )
    {
        auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
        wglSwapIntervalEXT( enable ? 1 : 0 );
        return true;
    }
    return false;
}

void Platform::GLWindow::swapBuffer()
{
    SwapBuffers( _dc );
}

void Platform::GLWindow::run( std::function<void()> updater )
{
    if( _proc == &defaultProc )
    {
        if( _autoSwap )
        {
            defaultTimerProc = [=]
            {
                this->swapBuffer();
                updater();
            };
        }
        else
        {
            defaultTimerProc = updater;
        }
    }

    MSG msg;
    bool running = true;
    while( running )
    {
        /* Do Windows stuff */
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
        updater();
        if( _autoSwap ) this->swapBuffer();
    }
}

void Platform::GLWindow::setAutoSwapBuffer( bool enable /*= true */ )
{
    _autoSwap = enable;
}
