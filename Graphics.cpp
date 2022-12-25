#include "main.h"
#include "Graphics.h"
#pragma comment(lib, "Dwrite")

pGraphics::pGraphics() :
    m_hwnd(NULL),
    m_pDirect2dFactory(NULL),
    m_pRenderTarget(NULL),
    m_pRed(NULL),
    m_pGreen(NULL),
    m_pBlack(NULL),
    m_pText(NULL),
    m_pTextBolt(NULL),
    m_pWriteFactory(NULL),
    m_pYellow(NULL),
    m_pMapGreen1(NULL),
    m_pMapGreen2(NULL)
{}

pGraphics::~pGraphics() {
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pRed);
    SafeRelease(&m_pGreen);
    SafeRelease(&m_pBlack);
    SafeRelease(&m_pYellow);
    SafeRelease(&m_pText);
    SafeRelease(&m_pTextBolt);
    SafeRelease(&m_pWriteFactory);
    SafeRelease(&m_pMapGreen1);
    SafeRelease(&m_pMapGreen2);
}

void pGraphics::RunMessageLoop() {
    MSG msg;

    int A = 0;
    int D = 0;
    int S = 0;
    int W = 0;
    int SPACE = 0;
    int ESC = 0;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) && (ESC == 0)) {
            ++ESC;
            PostQuitMessage(0);
        }
        else if (GetAsyncKeyState(0x20) == 0) ESC = 0;

        if ((GetAsyncKeyState(0x20) & 0x8000) && (SPACE == 0)) {
            ++SPACE;

            rGame.ClearAllData();
            rGame.GlobalScreenData.gameOver = false;

            OnRender();
        }
        else if (GetAsyncKeyState(0x20) == 0) SPACE = 0;
        if (!rGame.GlobalScreenData.gameOver) {
            if ((GetAsyncKeyState(0x41) & 0x8000) && (A == 0)) {
                ++A;

                if (rGame.GlobalPlayerData.pos.first - 1 == rGame.GlobalObjectData.objectId) {
                    rGame.MoveLeft();
                    rGame.Grow();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                }
                else if (rGame.GlobalObjectData.GrowObjectMap[rGame.GlobalPlayerData.pos.first] == 0 or rGame.BinarySearch(rGame.GlobalPlayerData.pos.first - 1, rGame.GlobalPlayerData.pixels)) {
                    rGame.GlobalScreenData.gameOver = true;
                }
                else {
                    rGame.MoveLeft();
                }

                OnRender();
            }
            else if (GetAsyncKeyState(0x41) == 0) A = 0;
            if ((GetAsyncKeyState(0x44) & 0x8000) && (D == 0)) {
                ++D;

                if (rGame.GlobalPlayerData.pos.first + 1 == rGame.GlobalObjectData.objectId) {
                    rGame.MoveRight();
                    rGame.Grow();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                }
                else if (rGame.GlobalObjectData.GrowObjectMap[rGame.GlobalPlayerData.pos.first] == rGame.GlobalScreenData.size.first / 30 - 1 or rGame.BinarySearch(rGame.GlobalPlayerData.pos.first + 1, rGame.GlobalPlayerData.pixels)) {
                    rGame.GlobalScreenData.gameOver = true;
                }
                else {
                    rGame.MoveRight();
                }

                OnRender();
            }
            else if (GetAsyncKeyState(0x44) == 0) D = 0;
            if ((GetAsyncKeyState(0x53) & 0x8000) && (S == 0)) {
                ++S;

                if (rGame.GlobalPlayerData.pos.first + rGame.GlobalScreenData.size.first / 30 == rGame.GlobalObjectData.objectId) {
                    rGame.MoveDown();
                    rGame.Grow();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                }
                else if (rGame.GlobalPlayerData.pos.first >= (rGame.GlobalScreenData.size.second / 30 * (rGame.GlobalScreenData.size.first / 30)) - 1 - rGame.GlobalScreenData.size.first / 30 or rGame.GlobalPlayerData.pos.first >= (rGame.GlobalScreenData.size.second / 30 * rGame.GlobalScreenData.size.first / 30) - 1 or rGame.BinarySearch(rGame.GlobalPlayerData.pos.first + rGame.GlobalScreenData.size.first / 30, rGame.GlobalPlayerData.pixels)) {
                    rGame.GlobalScreenData.gameOver = true;
                }
                else {
                    rGame.MoveDown();
                }

                OnRender();
            }
            else if (GetAsyncKeyState(0x53) == 0) S = 0;
            if ((GetAsyncKeyState(0x57) & 0x8000) && (W == 0)) {
                ++W;

                if (rGame.GlobalPlayerData.pos.first - rGame.GlobalScreenData.size.first / 30 == rGame.GlobalObjectData.objectId) {
                    rGame.MoveUp();
                    rGame.Grow();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                }
                else if (rGame.GlobalPlayerData.pos.first == 0 or rGame.GlobalPlayerData.pos.first <= rGame.GlobalScreenData.size.first / 30 - 1 or rGame.BinarySearch(rGame.GlobalPlayerData.pos.first - rGame.GlobalScreenData.size.first / 30, rGame.GlobalPlayerData.pixels)) {
                    rGame.GlobalScreenData.gameOver = true;
                }
                else {
                    rGame.MoveUp();
                }

                OnRender();
            }
            else if (GetAsyncKeyState(0x57) == 0) W = 0;
        }
    }
}

HRESULT pGraphics::Initialize() {
    HRESULT hr;
    hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr)) {
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = pGraphics::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = HINST_THISCOMPONENT;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"Snake";
        
        std::wstring stemp = std::wstring(rGame.GlobalScreenData.windowName.begin(), rGame.GlobalScreenData.windowName.end());
        LPCWSTR sw = stemp.c_str();

        RegisterClassEx(&wcex);
        m_hwnd = CreateWindow(
            L"Snake",
            sw,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this);

        if (m_hwnd) {
            float dpi = GetDpiForWindow(m_hwnd);

            SetWindowPos(
                m_hwnd,
                NULL,
                NULL,
                NULL,
                static_cast<int>(ceil(640.f * dpi / 96.f)),
                static_cast<int>(ceil(480.f * dpi / 96.f)),
                SWP_NOMOVE);
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }
    }

    return hr;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL))) { {
            pGraphics app;

            if (SUCCEEDED(app.Initialize())) {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}

HRESULT pGraphics::CreateDeviceIndependentResources() {
    static const WCHAR msc_fontName[] = L"Comic Sans MS";
    static const FLOAT msc_fontSize = 16;

    static const FLOAT msc_fontSizeBolt = 32;

    HRESULT hr;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    if (SUCCEEDED(hr)) {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pWriteFactory)
        );
    }
    if (SUCCEEDED(hr)) {
        hr = m_pWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"",
            &m_pText
        );
    }
    if (SUCCEEDED(hr)) {
        hr = m_pWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSizeBolt,
            L"",
            &m_pTextBolt
        );
    }
    m_pTextBolt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    return hr;
}

HRESULT pGraphics::CreateDeviceResources() {
    HRESULT hr = S_OK;

    if (!m_pRenderTarget) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget
        );

        if (SUCCEEDED(hr)) {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Red),
                &m_pRed
            );
        }
        if (SUCCEEDED(hr)) {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Green),
                &m_pGreen
            );
        }
        if (SUCCEEDED(hr)) {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBlack    
            );
        }
        if (SUCCEEDED(hr)) {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Yellow),
                &m_pYellow
            );
        }
        if (SUCCEEDED(hr)) {
            D2D1_COLOR_F color;
            color.a = 255;
            color.r = 0;
            color.g = 156;
            color.b = 18;

            hr = m_pRenderTarget->CreateSolidColorBrush(
                color,
                &m_pMapGreen1
            );
        }
        if (SUCCEEDED(hr)) {
            D2D1_COLOR_F color;
            color.a = 255 / 255.f;
            color.r = 0 / 255.f;
            color.g = 156 / 255.f;
            color.b = 18 / 255.f;

            hr = m_pRenderTarget->CreateSolidColorBrush(
                color,
                &m_pMapGreen1
            );
        }
        if (SUCCEEDED(hr)) {
            D2D1_COLOR_F color;
            color.a = 255 / 255.f;
            color.r = 36 / 255.f;
            color.g = 201 / 255.f;
            color.b = 56 / 255.f;

            hr = m_pRenderTarget->CreateSolidColorBrush(
                color,
                &m_pMapGreen2
            );
        }
    }

    return hr;
}

void pGraphics::DiscardDeviceResources() {
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pRed);
    SafeRelease(&m_pGreen);
}

LRESULT CALLBACK pGraphics::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE) {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        pGraphics* Graphics = (pGraphics*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(Graphics)
        );

        result = 1;
    }
    else {
        pGraphics* Graphics = reinterpret_cast<pGraphics*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )));

        bool wasHandled = false;

        if (Graphics) {
            switch (message) {
            case WM_SIZE: {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                Graphics->OnResize(width, height);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DISPLAYCHANGE: {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_PAINT: {
                Graphics->OnRender();
                ValidateRect(hwnd, NULL);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY: {
                PostQuitMessage(0);
            }
            result = 1;
            wasHandled = true;
            break;
            }
        }

        if (!wasHandled) {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

HRESULT pGraphics::OnRender() {
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr) && rGame.canRender) {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        //GlobalObjectMap render

        for (int i = 0; i < rGame.GlobalObjectData.objectMap.size(); ++i) {
            D2D1_RECT_F rect{//left //top //right // bottom
                rGame.GlobalObjectData.objectMap.at(i).first + 10,
                rGame.GlobalObjectData.objectMap.at(i).second,
                rGame.GlobalObjectData.objectMap.at(i).first + 30,
                rGame.GlobalObjectData.objectMap.at(i).second + 20
            };
            m_pRenderTarget->FillRectangle(&rect, m_pBlack);
        }
        if (!rGame.GlobalScreenData.gameOver) {
            D2D1_RECT_F rectObject{
                rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId).first + 10,
                rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId).second,
                rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId).first + 30,
                rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId).second + 20
            };
            m_pRenderTarget->FillRectangle(&rectObject, m_pRed);

            D2D1_RECT_F rectS{//left //top //right // bottom
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(0)).first + 10,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(0)).second,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(0)).first + 30,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(0)).second + 20
            };
            m_pRenderTarget->FillRectangle(&rectS, m_pYellow);

            for (int i = 1; i < rGame.GlobalPlayerData.size; ++i) {
                D2D1_RECT_F rect{//left //top //right // bottom
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(i)).first + 10,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(i)).second,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(i)).first + 30,
                    rGame.GlobalObjectData.objectMap.at(rGame.GlobalPlayerData.pixels.at(i)).second + 20
                };
                m_pRenderTarget->FillRectangle(&rect, m_pGreen);
            }

            std::string ss = "Size: ";
            ss.append(std::to_string(rGame.GlobalPlayerData.size));
            std::wstring ssr = std::wstring(ss.begin(), ss.end());
            const wchar_t* result = ssr.c_str();

            m_pRenderTarget->DrawTextW(
                result,
                sizeof(result) + 1,
                m_pText,
                D2D1::RectF(10, 0, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second),
                m_pBlack
            );
        }
        else {
            m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            std::string str = "Game Over!";
            std::wstring wstr = std::wstring(str.begin(), str.end());
            const wchar_t* result = wstr.c_str();

            m_pRenderTarget->DrawTextW(
                result,
                sizeof(result) + 1,
                m_pTextBolt, 
                D2D1::RectF(0, 0, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second), 
                m_pBlack
            );

            m_pRenderTarget->DrawTextW(
                L"Press [SPACE] to restart",
                25,
                m_pText,
                D2D1::RectF(0, 60, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second),
                m_pBlack
            );

            m_pRenderTarget->DrawTextW(
                L"Press [ESC] to exit",
                19,
                m_pText,
                D2D1::RectF(0, 80, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second),
                m_pBlack
            );

            std::string ss = "Size: ";
            ss.append(std::to_string(rGame.GlobalPlayerData.size));
            std::wstring ssr = std::wstring(ss.begin(), ss.end());
            const wchar_t* res = ssr.c_str();

            m_pRenderTarget->DrawTextW(
                res,
                sizeof(res) + 1,
                m_pText,
                D2D1::RectF(0, 40, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second),
                m_pBlack
            );
        }
        hr = m_pRenderTarget->EndDraw();
    }
    if (hr == D2DERR_RECREATE_TARGET) {
        hr = S_OK;
        DiscardDeviceResources();
    }
    return hr;
}

void pGraphics::OnResize(UINT width, UINT height) {
    if (m_pRenderTarget && width >= 290 && height >= 160) {
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        rGame.GlobalScreenData.size = std::make_pair(m_pRenderTarget->GetSize().width, m_pRenderTarget->GetSize().height);
        rGame.GenerateObjectMap();
        rGame.ClearAllData();
#ifdef _DEBUG
        std::cout << "Resizeing window: X: " << width << " Y: " << height << '\n';
#endif // _DEBUG

    }
}

std::pair<int, int> pGraphics::getScreenSize() {
    if (m_pRenderTarget) {
        return std::make_pair(m_pRenderTarget->GetSize().width, m_pRenderTarget->GetSize().height);
    }
    return std::make_pair(0, 0);
}