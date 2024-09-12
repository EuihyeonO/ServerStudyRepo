#pragma once
#include <d3d11.h>

#include "Resource.h"
#include "Define.h"

class DirectXManager
{
public:
	static DirectXManager* GetInstance()
	{
		static DirectXManager Instance;
		return &Instance;
	}

	void Start(HINSTANCE _hInstance);
	void End();

	bool CreateDeviceD3D();
	void CreateRenderTarget();
	void CleanUpDeviceD3D();
	void CleanUpRenderTarget();

	void ResizeWindow();

	HWND GethWnd()
	{
		return hWnd;
	}

	ID3D11Device* GetDevice()
	{
		return g_pd3dDevice;
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return g_pd3dDeviceContext;
	}

	ID3D11RenderTargetView* GetRTV()
	{
		return g_mainRenderTargetView;
	}

	ID3D11RenderTargetView** GetRTVDoublePtr()
	{
		return &g_mainRenderTargetView;
	}
	IDXGISwapChain* GetSwapChain()
	{
		return g_pSwapChain;
	}

private:
	static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	static HINSTANCE hInst;
	static WCHAR szTitle[MAX_LOADSTRING];
	static WCHAR szWindowClass[MAX_LOADSTRING];
	static WNDCLASSEXW wc;
	static HWND hWnd;

	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
	static UINT g_ResizeWidth;
	static UINT g_ResizeHeight;
	static ID3D11RenderTargetView* g_mainRenderTargetView;

};