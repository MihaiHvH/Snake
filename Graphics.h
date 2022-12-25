#pragma once

class pGraphics {
public:
	pGraphics();
	~pGraphics();

	HRESULT Initialize();
	void RunMessageLoop();


    pGame rGame;
    std::pair<int, int> getScreenSize();
    HRESULT OnRender();
private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
    void OnResize(
        UINT width,
        UINT height
    );
    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );
    HWND m_hwnd;
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pRed;
    ID2D1SolidColorBrush* m_pGreen;
    ID2D1SolidColorBrush* m_pMapGreen1;
    ID2D1SolidColorBrush* m_pMapGreen2;
    ID2D1SolidColorBrush* m_pBlack;
    ID2D1SolidColorBrush* m_pYellow;
    IDWriteTextFormat* m_pText;
    IDWriteTextFormat* m_pTextBolt;
    IDWriteFactory* m_pWriteFactory;
};

