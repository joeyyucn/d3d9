#include "d3dUtility.h"
namespace D3D
{
	const DWORD vertex::FVF = D3DFVF_XYZ;
	const DWORD colorvertex::FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE;
	const DWORD normalvertex::FVF = D3DFVF_XYZ|D3DFVF_NORMAL;
	const DWORD normalTexVertex::FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
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
		params.EnableAutoDepthStencil = true;
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
				float timeDelta = (currTime - lastTime) * 0.001f;
				ptr_display(timeDelta);
				lastTime = currTime;
			}
		}

		return msg.wParam;
	}

	D3DMATERIAL9 InitMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, float power)
	{
		D3DMATERIAL9 material;
		material.Ambient = ambient;
		material.Diffuse = diffuse;
		material.Specular = specular;
		material.Emissive = emissive;
		material.Power = power;
		return material;
	}

	D3DLIGHT9 InitDirectionLight(const D3DXCOLOR* color, const D3DVECTOR* direction)
	{
		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Direction = *direction;
		light.Ambient = *color * 0.0f;
		light.Diffuse = *color;
		light.Specular = *color * 0.8f;
		return light;
	}
}