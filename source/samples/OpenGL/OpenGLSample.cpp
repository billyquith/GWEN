/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Windows.h>

#ifdef USE_DEBUG_FONT
#include <Gwork/Renderers/OpenGL_DebugFont.h>
#else
#include <Gwork/Renderers/OpenGL.h>
#endif
#include "gl/glew.h"

HWND CreateGameWindow(void)
{
    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style            = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc      = DefWindowProc;
    wc.hInstance        = GetModuleHandle(nullptr);
    wc.lpszClassName    = L"GworkWindow";
    wc.hCursor          = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);
#ifdef USE_DEBUG_FONT
    HWND hWindow = CreateWindowExW((WS_EX_APPWINDOW|WS_EX_WINDOWEDGE), wc.lpszClassName,
                                   L"Gwork - OpenGL Sample (Using embedded debug font renderer)",
                                   (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                    WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME),
                                   -1, -1, 1004, 650, nullptr, nullptr, GetModuleHandle(
                                       nullptr), nullptr);
#else
    HWND hWindow = CreateWindowExW((WS_EX_APPWINDOW|WS_EX_WINDOWEDGE), wc.lpszClassName,
                                   L"Gwork - OpenGL Sample (No cross platform way to render fonts in OpenGL)",
                                   (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                    WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME), -1, -1, 1004, 650, nullptr, nullptr,
                                   GetModuleHandle(nullptr), nullptr);
#endif // ifdef USE_DEBUG_FONT
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    return hWindow;
}

HWND g_hWND = nullptr;

HGLRC CreateOpenGLDeviceContext()
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);      // just its size
    pfd.nVersion = 1;   // always 1
    pfd.dwFlags = PFD_SUPPORT_OPENGL|   // OpenGL support - not DirectDraw
                  PFD_DOUBLEBUFFER|     // double buffering support
                  PFD_DRAW_TO_WINDOW;   // draw to the app window, not to a
                                        // bitmap image
    pfd.iPixelType = PFD_TYPE_RGBA;     // red, green, blue, alpha for each
                                        // pixel
    pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for
                                        // green, 8 for blue.
    // This count of color bits EXCLUDES alpha.
    pfd.cDepthBits = 32;                // 32 bits to measure pixel depth.
    int pixelFormat = ChoosePixelFormat(GetDC(g_hWND), &pfd);

    if (pixelFormat == 0)
        FatalAppExit(nullptr, TEXT("ChoosePixelFormat() failed!"));

    SetPixelFormat(GetDC(g_hWND), pixelFormat, &pfd);
    HGLRC OpenGLContext = wglCreateContext(GetDC(g_hWND));
    wglMakeCurrent(GetDC(g_hWND), OpenGLContext);
    RECT r;

    if (GetClientRect(g_hWND, &r))
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, r.right-r.left, r.bottom-r.top);
    }

    return OpenGLContext;
}

int main()
{
    //
    // Create a new window
    //
    g_hWND = CreateGameWindow();
    //
    // Create OpenGL Device
    //
    HGLRC OpenGLContext = CreateOpenGLDeviceContext();
    //
    // Create a Gwork OpenGL Renderer
    //
#ifdef USE_DEBUG_FONT
    Gwk::Renderer::OpenGL* renderer = new Gwk::Renderer::OpenGL_DebugFont();
#else
    Gwk::Renderer::OpenGL* renderer = new Gwk::Renderer::OpenGL();
#endif
    renderer->Init();
    //
    // Create a Gwork skin
    //
    Gwk::Skin::TexturedBase* skin = new Gwk::Skin::TexturedBase(renderer);
    skin->Init("DefaultSkin.png");
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(998, 650-24);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));
    //
    // Create our unittest control (which is a Window with controls in it)
    //
    UnitTest* unit = new UnitTest(canvas);
    unit->SetPos(10, 10);
    //
    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    //
    Gwk::Input::Windows GworkInput;
    GworkInput.Initialize(canvas);
    //
    // Begin the main game loop
    //
    MSG msg;

    while (true)
    {
        // Skip out if the window is closed
        if (!IsWindowVisible(g_hWND))
            break;

        // If we have a message from windows..
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // .. give it to the input handler to process
            GworkInput.ProcessMessage(msg);

            // if it's QUIT then quit..
            if (msg.message == WM_QUIT)
                break;

            // Handle the regular window stuff..
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Main OpenGL Render Loop
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            canvas->RenderCanvas();
            SwapBuffers(GetDC(g_hWND));
        }
    }

    // Clean up OpenGL
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(OpenGLContext);
    delete canvas;
    delete skin;
    delete renderer;
}
