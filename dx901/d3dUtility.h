#pragma once
#include <d3d9.h>
#include <d3dx9.h>

namespace D3D
{
	const D3DCOLOR	RED = D3DCOLOR_XRGB(255, 0, 0);
	const D3DCOLOR	GREEN = D3DCOLOR_XRGB(0, 255, 0);
	const D3DCOLOR	BLUE = D3DCOLOR_XRGB(0, 0, 255);
	const D3DCOLOR	WHITE = D3DCOLOR_XRGB(255, 255, 255);
	const D3DCOLOR	BLACK = D3DCOLOR_XRGB(0, 0, 0);

	const D3DXCOLOR XRED = D3DXCOLOR(RED);
	const D3DXCOLOR XGREEN = D3DXCOLOR(GREEN);
	const D3DXCOLOR XBLUE = D3DXCOLOR(BLUE);
	const D3DXCOLOR XWHITE = D3DXCOLOR(WHITE);
	const D3DXCOLOR XBLACK = D3DXCOLOR(BLACK);

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
	};

	struct normalTexVertex
	{
		normalTexVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		{
			_x = x;
			_y = y;
			_z = z;
			_nx = nx;
			_ny = ny;
			_nz = nz;
			_u = u;
			_v = v;
		}

		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;
		static const DWORD FVF;
	};

	
	D3DMATERIAL9 InitMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emmisive, float power);

	const D3DMATERIAL9 WHITEMATERIAL = InitMaterial(XWHITE, XWHITE, XWHITE, XBLACK, 8.0f);
	const D3DMATERIAL9 BLACKMATERIAL = InitMaterial(XBLACK, XBLACK, XBLACK, XBLACK, 8.0f);
	const D3DMATERIAL9 REDMATERIAL = InitMaterial(XRED, XRED, XRED, XBLACK, 8.0f);
	
	D3DLIGHT9 InitDirectionLight(const D3DXCOLOR* color, const D3DVECTOR* direction);
}