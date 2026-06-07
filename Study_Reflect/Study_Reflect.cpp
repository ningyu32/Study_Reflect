// Study_Reflect.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Study_Reflect.h"
#include "Object.h"
#include "Reflect/Reflection.h"

#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "MyReflect/Registry.h"
#include "Actor.h"

#pragma comment(lib, "d3d11.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

//class BasicReflectSample : public Object
//{
//    GENERATED_REFLECT_BODY(BasicReflectSample)
//public:
//    bool bEnabled = true;
//    REGISTER_CLASS_PROPERTY(BasicReflectSample, bool, bEnabled);
//
//public:
//    std::int32_t Health = 100;
//    REGISTER_CLASS_PROPERTY(BasicReflectSample, std::int32_t, Health);
//
//public:
//    std::uint32_t Level = 3;
//    REGISTER_CLASS_PROPERTY(BasicReflectSample, std::uint32_t, Level);
//
//protected:
//    float MoveSpeed = 320.0f;
//    REGISTER_CLASS_PROPERTY(BasicReflectSample, float, MoveSpeed);
//
//private:
//    double Energy = 75.0;
//    REGISTER_CLASS_PROPERTY(BasicReflectSample, double, Energy);
//};

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool                CreateDeviceD3D(HWND hWnd);
void                CleanupDeviceD3D();
void                CreateRenderTarget();
void                CleanupRenderTarget();
void                RenderReflectDemo();
bool                DrawBasicProperty(const Reflect::PropertyInfo& Property, void* Instance);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STUDYREFLECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STUDYREFLECT));

    MSG msg;

    // 主消息循环:
    bool bDone = false;
    while (!bDone)
    {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (msg.message == WM_QUIT)
            {
                bDone = true;
            }
        }

        if (bDone)
        {
            break;
        }

        RenderReflectDemo();
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STUDYREFLECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STUDYREFLECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   if (!CreateDeviceD3D(hWnd))
   {
      CleanupDeviceD3D();
      return FALSE;
   }

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

   ImGui::StyleColorsDark();
   ImGui_ImplWin32_Init(hWnd);
   ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }

    switch (message)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED && g_pd3dDevice != nullptr)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
        {
            return 0;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
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
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice,
        &featureLevel,
        &g_pd3dDeviceContext);

    if (result == DXGI_ERROR_UNSUPPORTED)
    {
        result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            createDeviceFlags,
            featureLevelArray,
            2,
            D3D11_SDK_VERSION,
            &sd,
            &g_pSwapChain,
            &g_pd3dDevice,
            &featureLevel,
            &g_pd3dDeviceContext);
    }

    if (result != S_OK)
    {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

void RenderReflectDemo()
{
    static bool bShowReflectDemo = true;
    //static BasicReflectSample Sample;
    //const Reflect::TypeInfo* Type = Sample.GetReflectType();

    if (!bShowReflectDemo)
    {
        PostQuitMessage(0);
        return;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(520, 420), ImGuiCond_FirstUseEver);
    ImGui::Begin("Basic Reflection Demo", &bShowReflectDemo);
    //ImGui::Text("Class: %s", Type->Name.data());
    //ImGui::Text("Version: %u", Type->Version);
    ImGui::Separator();

    //for (const Reflect::PropertyInfo& Property : Type->Properties)
    //{
    //    //DrawBasicProperty(Property, &Sample);
    //}
    for (auto pair : Reflection::Register::Get()->GetAllClasses())
    {
        ImGui::Text(pair.first.c_str());
    }
    for (auto pair : Actor::GetStaticClassInfo()->GetAllProperties())
    {
        ImGui::Text(pair.first.c_str());
    }
    ImGui::BeginChild("test");
    //测试界面
    Actor* actor = Reflection::Register::Get()->CreateObject<Actor>();
    
    ImGui::EndChild();

    ImGui::Separator();
    if (ImGui::BeginTable("Schema", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Field");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Offset");
        ImGui::TableSetupColumn("Version");
        ImGui::TableHeadersRow();

        /*for (const Reflect::PropertyInfo& Property : Type->Properties)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(Property.Name.data());
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(Property.Type ? Property.Type->Name.data() : "unregistered");
            ImGui::TableSetColumnIndex(2);
            if (Property.Offset == static_cast<std::size_t>(-1))
            {
                ImGui::TextUnformatted("n/a");
            }
            else
            {
                ImGui::Text("%zu", Property.Offset);
            }
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%u", Property.Version);
        }*/
        ImGui::EndTable();
    }

    ImGui::End();
    ImGui::Render();

    const float clearColor[4] = { 0.08f, 0.09f, 0.10f, 1.00f };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0);
}

//bool DrawBasicProperty(const Reflect::PropertyInfo& Property, void* Instance)
//{
//    if (Property.Type == nullptr)
//    {
//        ImGui::TextDisabled("%s: unregistered type", Property.Name.data());
//        return false;
//    }
//
//    void* Value = Property.GetValuePtr(Instance);
//    switch (Property.Type->Kind)
//    {
//    case Reflect::TypeKind::Bool:
//        return ImGui::Checkbox(Property.Name.data(), static_cast<bool*>(Value));
//    case Reflect::TypeKind::Int8:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_S8, Value);
//    case Reflect::TypeKind::UInt8:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_U8, Value);
//    case Reflect::TypeKind::Int16:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_S16, Value);
//    case Reflect::TypeKind::UInt16:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_U16, Value);
//    case Reflect::TypeKind::Int32:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_S32, Value);
//    case Reflect::TypeKind::UInt32:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_U32, Value);
//    case Reflect::TypeKind::Int64:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_S64, Value);
//    case Reflect::TypeKind::UInt64:
//        return ImGui::InputScalar(Property.Name.data(), ImGuiDataType_U64, Value);
//    case Reflect::TypeKind::Float:
//        return ImGui::DragFloat(Property.Name.data(), static_cast<float*>(Value), 0.1f);
//    case Reflect::TypeKind::Double:
//        return ImGui::InputDouble(Property.Name.data(), static_cast<double*>(Value));
//    default:
//        ImGui::TextDisabled("%s: unsupported type", Property.Name.data());
//        return false;
//    }
//}
