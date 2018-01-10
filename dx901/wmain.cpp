#include <stdio.h>
#include <Windows.h>
#include <d3dx9math.h>
#include "d3dUtility.h"
using namespace D3D;
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3dx9.lib")

IDirect3DDevice9* g_pDevice = NULL;
IDirect3DVertexBuffer9* g_pVertexBuffer = NULL;
IDirect3DIndexBuffer9* g_pIndexBuffer = NULL;

int g_windowWidth = 800;
int g_windowHeight = 600;

bool Display(float deltaTime)
{
	g_pDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	g_pDevice->BeginScene();
	g_pDevice->SetStreamSource(NULL, g_pVertexBuffer, 0, sizeof(vertex));
	g_pDevice->SetIndices(g_pIndexBuffer);
	g_pDevice->SetFVF(vertex::FVF);
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	g_pDevice->EndScene();
	g_pDevice->Present(0, 0, 0, 0);
	return true;
}

bool Setup()
{
	HRESULT hr = g_pDevice->CreateVertexBuffer(8*sizeof(vertex), D3DUSAGE_WRITEONLY, vertex::FVF, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	if(D3D_OK != hr)
	{
		return false;
	}

	hr = g_pDevice->CreateIndexBuffer(36*sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	if(D3D_OK != hr)
	{
		return false;
	}

	vertex* vertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void **)&vertices, 0);
	vertices[0] = vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = vertex(1.0f, -1.0f, 1.0f);
	g_pVertexBuffer->Unlock();

	DWORD* indices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	// front
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 2; indices[4] = 3; indices[5] = 0;

	// back
	indices[6] = 4; indices[7] = 7; indices[8] = 6;
	indices[9] = 6; indices[10] = 5; indices[11] = 4;

	// top
	indices[12] = 1; indices[13] = 5; indices[14] = 6;
	indices[15] = 6; indices[16] = 2; indices[17] = 1;

	// bottom
	indices[18] = 0; indices[19] = 3; indices[20] = 7;
	indices[21] = 7; indices[22] = 4; indices[23] = 0;

	// left
	indices[24] = 0; indices[25] = 1; indices[26] = 5;
	indices[27] = 5; indices[28] = 4; indices[29] = 0;

	// right
	indices[30] = 3; indices[31] = 2; indices[32] = 6;
	indices[33] = 6; indices[34] = 7; indices[35] = 3;
	g_pIndexBuffer->Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 1.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	g_pDevice->SetTransform(D3DTS_VIEW, &v);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5F, (float)g_windowWidth/(float)g_windowHeight, 1.0F, 1000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj); 

	g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}

void CleanUp()
{
	g_pVertexBuffer->Release();
	g_pIndexBuffer->Release();
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	if(!InitD3D(hinstance, g_windowWidth, g_windowHeight, true, D3DDEVTYPE_REF, &g_pDevice))
	{
		MessageBox(NULL, L"Failed init D3D", L"Error", MB_OK);
		return 0;
	}

	if(!Setup())
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

