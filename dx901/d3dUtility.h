#pragma once
#include <d3d9.h>

namespace D3D
{
	const D3DCOLOR	RED = D3DCOLOR_XRGB(255, 0, 0);
	const D3DCOLOR	GREEN = D3DCOLOR_XRGB(0, 255, 0);
	const D3DCOLOR	BLUE = D3DCOLOR_XRGB(0, 0, 255);

	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMessageLoop(bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	template<class T> void Release(T t)
	{
		if(t)
		{
			t->Release();
			t = NULL;
		}
	}

	template<class T> void Delete(T t)
	{
		if(t)
		{
			delete t;
			t = NULL;
		}
	}

	struct vertex
	{
		vertex(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}

		float _x, _y, _z;
		static const DWORD FVF;
	};

	struct colorvertex
	{
		colorvertex(float x, float y, float z, D3DCOLOR color)
		{
			_x = x;
			_y = y;
			_z = z;
			_color = color;
		}

		float _x, _y, _z;
		D3DCOLOR _color;
		static const DWORD FVF;
	};

	struct normalvertex
	{
		normalvertex(float x, float y, float z, float nx, float ny, float nz)
		{
			_x = x;
			_y = y;
			_z = z;
			_nx = nx;
			_ny = ny;
			_nz = nz;
		}

		float _x, _y, _z;
		float _nx, _ny, _nz;
		static const DWORD FVF;
	}
}