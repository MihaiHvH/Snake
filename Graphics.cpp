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
    m_pYellow(NULL)
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
}

void pGraphics::RunMessageLoop() {
    MSG msg;

    int A = 0;
    int D = 0;
    int S = 0;
    int W = 0;
    int SPACE = 0;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!rGame.GlobalScreenData.gameOver) {
            if ((GetAsyncKeyState(0x20) & 0x8000) && (SPACE == 0)) {
                ++SPACE;
                OnRender();
            }
            else if (GetAsyncKeyState(0x20) == 0) SPACE = 0;

            if ((GetAsyncKeyState(0x41) & 0x8000) && (A == 0)) {
                ++A;
                if (rGame.IsPixelPos({ rGame.GlobalPlayerData.pixels.at(0).first - 30, rGame.GlobalPlayerData.pixels.at(0).second }, rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId), true)) {
                    rGame.MoveLeft();
                    OnRender();
                    rGame.Grow();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                    OnRender();
                }
                else if (rGame.GlobalPlayerData.pixels[0].first - 30 < 1) {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }
                else if (!rGame.IsPixelPos(rGame.GlobalPlayerData.pixels, { rGame.GlobalPlayerData.pos.first - 30, rGame.GlobalPlayerData.pos.second }, true)) {
                    rGame.MoveLeft();
                }
                else {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }


                OnRender();
            }
            else if (GetAsyncKeyState(0x41) == 0) A = 0;
            if ((GetAsyncKeyState(0x44) & 0x8000) && (D == 0)) {
                ++D;
                if (rGame.IsPixelPos({ rGame.GlobalPlayerData.pixels.at(0).first + 30, rGame.GlobalPlayerData.pixels.at(0).second }, rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId), true)) {
                    rGame.MoveRight();
                    rGame.Grow();
                    OnRender();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                    OnRender();
                }
                else if (rGame.GlobalPlayerData.pixels[0].first + 30 + (rGame.GlobalScreenData.size.first - rGame.GlobalScreenData.maxCoordMap[0].first) >= rGame.GlobalScreenData.size.first) {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }
                else if (!rGame.IsPixelPos(rGame.GlobalPlayerData.pixels, { rGame.GlobalPlayerData.pos.first + 30, rGame.GlobalPlayerData.pos.second }, true)) {
                    rGame.MoveRight();
                }
                else {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }

                OnRender();
            }
            else if (GetAsyncKeyState(0x44) == 0) D = 0;
            if ((GetAsyncKeyState(0x53) & 0x8000) && (S == 0)) {
                ++S;
                if (rGame.IsPixelPos({ rGame.GlobalPlayerData.pixels.at(0).first, rGame.GlobalPlayerData.pixels.at(0).second + 30 }, rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId), true)) {
                    rGame.MoveDown();
                    rGame.Grow();
                    OnRender();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                    OnRender();
                }
                else if (rGame.GlobalPlayerData.pixels[0].second + 30 + rGame.GlobalScreenData.size.second - rGame.GlobalScreenData.maxCoordMap[0].second >= rGame.GlobalScreenData.size.second) {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }
                else if (!rGame.IsPixelPos(rGame.GlobalPlayerData.pixels, { rGame.GlobalPlayerData.pos.first, rGame.GlobalPlayerData.pos.second + 30 }, true)) {
                    rGame.MoveDown();
                }
                else {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }


                OnRender();
            }
            else if (GetAsyncKeyState(0x53) == 0) S = 0;
            if ((GetAsyncKeyState(0x57) & 0x8000) && (W == 0)) {
                ++W;
                if (rGame.IsPixelPos({ rGame.GlobalPlayerData.pixels.at(0).first, rGame.GlobalPlayerData.pixels.at(0).second - 30 }, rGame.GlobalObjectData.objectMap.at(rGame.GlobalObjectData.objectId), true)) {
                    rGame.MoveUp();
                    rGame.Grow();
                    OnRender();
                    rGame.GlobalObjectData.spawned = false;
                    rGame.Spawn();
                    OnRender();
                }
                else if (rGame.GlobalPlayerData.pixels[0].second - 30 < 1) {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
                }
                else if (!rGame.IsPixelPos(rGame.GlobalPlayerData.pixels, { rGame.GlobalPlayerData.pos.first, rGame.GlobalPlayerData.pos.second - 30 }, true)) {
                    rGame.MoveUp();
                }
                else {
                    rGame.GlobalScreenData.gameOver = true;
                    std::cout << "Game over!";
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
        m_pBlack->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

        /*D2D1_RECT_F area{
            0,
            0,
            rGame.GlobalScreenData.size.first,
            rGame.GlobalScreenData.size.second
        };
        m_pRenderTarget->DrawRectangle(&area, m_pBlack, 10);
        */
        //GlobalObjectMap render
        for (int i = 1; i < rGame.GlobalObjectData.ObjectMapSize; ++i) {
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
                    rGame.GlobalPlayerData.pixels.at(0).first + 10,
                    rGame.GlobalPlayerData.pixels.at(0).second,
                    rGame.GlobalPlayerData.pixels.at(0).first + 30,
                    rGame.GlobalPlayerData.pixels.at(0).second + 20
            };
            m_pRenderTarget->FillRectangle(&rectS, m_pYellow);

            for (int i = 1; i < rGame.GlobalPlayerData.size; ++i) {
                D2D1_RECT_F rect{//left //top //right // bottom
                    rGame.GlobalPlayerData.pixels.at(i).first + 10,
                    rGame.GlobalPlayerData.pixels.at(i).second,
                    rGame.GlobalPlayerData.pixels.at(i).first + 30,
                    rGame.GlobalPlayerData.pixels.at(i).second + 20
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

            m_pTextBolt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

            m_pRenderTarget->DrawTextW(
                result,
                sizeof(result) + 1,
                m_pTextBolt, 
                D2D1::RectF(0, 0, rGame.GlobalScreenData.size.first, rGame.GlobalScreenData.size.second), 
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
    if (m_pRenderTarget) {
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        rGame.GlobalScreenData.size = std::make_pair(m_pRenderTarget->GetSize().width, m_pRenderTarget->GetSize().height);
        rGame.GenerateObjectMap();
    }
}

std::pair<int, int> pGraphics::getScreenSize() {
    if (m_pRenderTarget) {
        return std::make_pair(m_pRenderTarget->GetSize().width, m_pRenderTarget->GetSize().height);
    }
    return std::make_pair(0, 0);
}