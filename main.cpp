// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <vector>
#include "gszauer/Vec3.h"

const gszauer::vec4 white(1.f, 1.f, 1.f, 1.f);
const gszauer::vec4 pink(1.00f, 0.00f, 0.75f, 1.0f);
const gszauer::vec4 cyan(0.00f, 0.75f, 1.00f, 1.0f);

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct ImGuiGrid
{
    enum { LINE_WIDTH = 1 }; // handlers: small lines width
    enum { GRAB_RADIUS = 8 }; // handlers: circle radius
    enum { GRAB_BORDER = 2 }; // handlers: circle border width
    enum { AREA_CONSTRAINED = true }; // should grabbers be constrained to grid area?
    enum { AREA_WIDTH = 256 }; // area width in pixels. 0 for adaptive size (will use max avail width)

    bool drawGrid();
    bool drawLine(gszauer::vec2 a, gszauer::vec2 b, gszauer::vec4 c);
    bool drawPoint(gszauer::vec2 p, gszauer::vec4 c);
    bool drawLine(gszauer::vec3 a, gszauer::vec3 b, gszauer::vec4 c);
    bool drawPoint(gszauer::vec3 p, gszauer::vec4 c);

    ImVec2 scalePosition(gszauer::vec2 p);

    gszauer::vec2 mgmin = gszauer::vec2(0.f);
    gszauer::vec2 mgmax = gszauer::vec2(1.f);

    ImVec2 mCanvas;
    ImRect mbb;
};

bool ImGuiGrid::drawGrid()
{
    using namespace ImGui;

    // prepare canvas
    const float avail = GetContentRegionAvailWidth();
    const float dim = AREA_WIDTH > 0 ? AREA_WIDTH : avail;

    ImVec2 Canvas(dim, dim);
    const ImGuiStyle& Style = GetStyle();

    ImDrawList* DrawList = GetWindowDrawList();
    ImGuiWindow* Window = GetCurrentWindow();
    if (Window->SkipItems)
        return false;

    ImRect bb = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
    ItemSize(bb);
    if (!ItemAdd(bb, NULL))
        return false;

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

    // background grid
    for (int i = 0; i <= Canvas.x; i += (Canvas.x / 8)) {
        DrawList->AddLine(
            ImVec2(bb.Min.x + i, bb.Min.y),
            ImVec2(bb.Min.x + i, bb.Max.y),
            GetColorU32(ImGuiCol_TextDisabled));
    }
    for (int i = 0; i <= Canvas.y; i += (Canvas.y / 8)) {
        DrawList->AddLine(
            ImVec2(bb.Min.x, bb.Min.y + i),
            ImVec2(bb.Max.x, bb.Min.y + i),
            GetColorU32(ImGuiCol_TextDisabled));
    }

    mbb = bb;
    mCanvas = Canvas;

    return true;
}

bool ImGuiGrid::drawLine(gszauer::vec2 a, gszauer::vec2 b, gszauer::vec4 c)
{
    using namespace ImGui;

    ImDrawList* DrawList = GetWindowDrawList();

    float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;

    ImVec2 p1 = scalePosition(a);
    ImVec2 p2 = scalePosition(b);
    ImColor color(c.r, c.g, c.b, c.a);

    DrawList->AddLine(p1, p2, color, LINE_WIDTH);

    return true;
}

bool ImGuiGrid::drawPoint(gszauer::vec2 p, gszauer::vec4 c)
{
    using namespace ImGui;

    ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);

    ImDrawList* DrawList = GetWindowDrawList();

    ImVec2 pos = scalePosition(p);
    ImColor color(c.r, c.g, c.b, c.a);

    DrawList->AddCircleFilled(pos, GRAB_RADIUS, ImColor(white));
    DrawList->AddCircleFilled(pos, GRAB_RADIUS - GRAB_BORDER, color);

    return true;
}

bool ImGuiGrid::drawLine(gszauer::vec3 a, gszauer::vec3 b, gszauer::vec4 c)
{
    return drawLine(gszauer::vec2(a), gszauer::vec2(b), c);
}

bool ImGuiGrid::drawPoint(gszauer::vec3 p, gszauer::vec4 c)
{
    return drawPoint(gszauer::vec2(p), c);
}

ImVec2 ImGuiGrid::scalePosition(gszauer::vec2 p)
{
    gszauer::vec2 npos = (p - mgmin) / (mgmax - mgmin);
    return ImVec2(npos.x, 1 - npos.y) * (mbb.Max - mbb.Min) + mbb.Min;
}

void ShowLinePlot()
{
    ImGui::Begin("Debug");
    ImGui::Text("Bezier curve");

    auto p1 = gszauer::vec3(-5.f, 0.f, 0.f);
    auto p2 = gszauer::vec3(+5.f, 0.f, 0.f);
    auto c1 = gszauer::vec3(-2.f, 1.f, 0.f);
    auto c2 = gszauer::vec3(+2.f, 1.f, 0.f);

    auto red = gszauer::vec4(1.f, 0.f, 0.f, 1.f);
    auto green = gszauer::vec4(0.f, 1.f, 0.f, 1.f);
    auto blue = gszauer::vec4(0.f, 0.f, 1.f, 1.f);
    auto magenta = gszauer::vec4(1.f, 0.f, 1.f, 1.f);

    ImGuiGrid grid;
    grid.mgmin = gszauer::vec2(-5.f);
    grid.mgmax = gszauer::vec2(+5.f);
    grid.drawGrid();

    gszauer::Bezier<gszauer::vec3> curve;
    curve.P1 = p1;
    curve.P2 = p2;
    curve.C1 = c1;
    curve.C2 = c2;

    static const int count = 200;
    for (int i = 0; i < count; i++) {
        float t0 = (float)(i + 0) / count;
        float t1 = (float)(i + 1) / count;
        auto k0 = gszauer::interpolate(curve, t0);
        auto k1 = gszauer::interpolate(curve, t1);
        grid.drawLine(k0, k1, magenta);
    }

    grid.drawLine(p1, c1, white);
    grid.drawLine(p2, c2, white);
    grid.drawPoint(p1, red);
    grid.drawPoint(c1, green);
    grid.drawPoint(p2, red);
    grid.drawPoint(c2, green);

    ImGui::End();
}

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ShowLinePlot();

        // Rendering
        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
