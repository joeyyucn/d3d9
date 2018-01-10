#include "d3dUtility.h"
namespace D3D
{
	const DWORD vertex::FVF = D3DFVF_XYZ;
	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
	{
		LPCWSTR className = L"D3DParentWindow"; 
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = className;
		wcex.lpszMenuName = NULL;
		wcex.hIconSm = NULL;
		wcex.lpfnWndProc = WndProc;
		if(!RegisterClassEx(&wcex))
		{
			return false;
		}

		HWND hWnd;
		hWnd = CreateWindow(className, L"myD3DWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 
			NULL, NULL, hInstance, NULL);

		if(!hWnd)
		{
			return false;
		}

		IDirect3D9* d3d9;
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		
		if(d3d9 == NULL)
		{
			return false;
		}

		D3DPRESENT_PARAMETERS params;
		D3DCAPS9 caps;
		bool supportsHardwareVertexProcessing = false;
		int vertexProcess = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		
		if( D3D_OK != d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps))
		{
			d3d9->Release();
			return false;
		}

		if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			vertexProcess = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}

		params.BackBufferHeight = height;
		params.BackBufferWidth = width;
		params.BackBufferFormat = D3DFMT_A8R8G8B8;
		params.BackBufferCount = 1;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		params.MultiSampleQuality = 0;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.Windowed = windowed;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D24S8;
		params.Flags = 0;
		params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		params.hDeviceWindow = hWnd;

		HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, vertexProcess, &params, device);

		if(D3D_OK != hr)
		{
			d3d9->Release();
			return false;
		}

		ShowWindow(hWnd, true);
		UpdateWindow(hWnd);
		d3d9->Release();
		return true;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
			}
			break;
		default:
			break;
		}
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	int EnterMessageLoop(bool (*ptr_display)(float timeDelta))
	{
		MSG msg;
		::ZeroMemory(&msg, sizeof(MSG));

		static float lastTime = (float)timeGetTime();

		while(msg.message != WM_QUIT)
		{
			if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				float currTime = (float)timeGetTime();
				float timeDelta = (currTime - lastTime);
				ptr_display(timeDelta);
				lastTime = currTime;
			}
		}

		return msg.wParam;
	}
}