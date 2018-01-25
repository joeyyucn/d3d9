/*
*	this chapter is about light and material
*	including:
*	use predefined materials
*	creating a position light
*	using the light and material draw a primitive
*	NOTE THAT: when light is disabled, the color of vertices is 
*	white, we would better to use BLACK as target buffer clear color
*/
#include "common.h"

#ifdef _USING_CHAPTER_2_
#include <stdio.h>
#include "d3dUtility.h"

using namespace D3D;
IDirect3DVertexBuffer9* g_pVertexBuffer = NULL;
bool Display(float timeDelta)
{
	static int frames = 0;
	static float timeCost = 0 ;
	static int fps;
	timeCost += timeDelta;
	frames++;
	if(timeCost > 1.0f)
	{
		fps = (int)((float)frames / timeCost);
		frames = 0;
		timeCost = 0;
		char buff[255];
		::ZeroMemory(buff, 255);
		sprintf_s(buff, "FPS: %d\r\n", fps);
		OutputDebugStringA(buff);
	}

	static float y = 0;
	D3DXMATRIX yRot;
	D3DXMatrixRotationY(&yRot, y);
	y += timeDelta;
	if(y > 2.0f * D3DX_PI)
	{
		y = 0;
	}

	D3DXVECTOR3 direction = D3DXVECTOR3(sin(y), 0, cos(y));
	D3DLIGHT9 light = InitDirectionLight(&XWHITE, &direction); 
	g_pDevice->SetLight(0, &light);

	//g_pDevice->SetTransform(D3DTS_WORLD, &yRot);
	g_pDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	g_pDevice->BeginScene();
	g_pDevice->SetStreamSource(NULL, g_pVertexBuffer, 0, sizeof(normalvertex));
	g_pDevice->SetFVF(normalvertex::FVF);
	g_pDevice->LightEnable(0, true);
	g_pDevice->SetMaterial(&REDMATERIAL);
	//g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	
	
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffer = NULL;
	D3DXCreateTeapot(g_pDevice, &pMesh, &pBuffer);
	pMesh->DrawSubset(0);

	pMesh->Release();
	pBuffer->Release();
	g_pDevice->EndScene();
	g_pDevice->Present(0, 0, 0, 0);
	return true;
}

bool Setup(int width, int height)
{
	HRESULT hr = g_pDevice->CreateVertexBuffer(6*sizeof(normalvertex), D3DUSAGE_WRITEONLY, normalvertex::FVF, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	if(FAILED(hr))
	{
		return false;
	}

	//normalvertex* pVertices = NULL;
	normalvertex* pVertices = NULL;
	hr = g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	if(FAILED(hr))
	{
		return false;
	}

	pVertices[0] = normalvertex(-1.0f, 0, 0, -1.0f, 1.0f, -1.0f);
	pVertices[1] = normalvertex(0, 1.0f, 0, -1.0f, 1.0f, -1.0f);
	pVertices[2] = normalvertex(0, 0, -3.0f, -1.0f, 1.0f, -1.0f);
	pVertices[3] = normalvertex(0, 0, -3.0f, 1.0f, 1.0f, -1.0f);
	pVertices[4] = normalvertex(0, 1.0f, 0, 1.0f, 1.0f, -1.0f);
	pVertices[5] = normalvertex(1.0f, 0, 0, 1.0f, 1.0f, -1.0f);
	


	g_pVertexBuffer->Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -4.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	g_pDevice->SetTransform(D3DTS_VIEW, &v);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5F, (float)width/(float)height, 1.0f, 1000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj); 

	/*
	D3DXVECTOR3 direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DLIGHT9 light = InitDirectionLight(&XWHITE, &direction); 
	
	g_pDevice->SetLight(0, &light);
	*/
	//g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	//g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	return true;
}

void CleanUp()
{
	g_pVertexBuffer->Release();
}
#endif