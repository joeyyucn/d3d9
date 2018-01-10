#pragma once
#include <d3d9.h>

namespace D3D
{
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
}