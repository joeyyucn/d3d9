#include <stdio.h>
#include <Windows.h>
#include <d3dx9math.h>
#include "d3dUtility.h"
#include "common.h"

using namespace D3D;
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3dx9.lib")

IDirect3DDevice9* g_pDevice = NULL;


int g_windowWidth = 800;
int g_windowHeight = 600;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	if(!InitD3D(hinstance, g_windowWidth, g_windowHeight, true, D3DDEVTYPE_REF, &g_pDevice))
	{
		MessageBox(NULL, L"Failed init D3D", L"Error", MB_OK);
		return 0;
	}

	if(!Setup(g_windowWidth, g_windowHeight))
	{
		MessageBox(NULL, L"Failed setup", L"Error", MB_OK);
		g_pDevice->Release();
		return 0;
	}
	EnterMessageLoop(Display);
	CleanUp();
	g_pDevice->Release();
	return 0;
}

