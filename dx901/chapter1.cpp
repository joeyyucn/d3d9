/*
*	this chapter is about index buffer and vertex buffer
*	including:
*	create a vertex buffer and fill vertices into the buffer
*	create index buffer and fill indces into the buffer
*	use the vertex and index buffer to draw a primitives
*/
#include "common.h"
#include "d3dUtility.h"
using namespace D3D;

#ifdef _USING_CHAPTER_1_
IDirect3DVertexBuffer9* g_pVertexBuffer = NULL;
IDirect3DIndexBuffer9* g_pIndexBuffer = NULL;

bool Display(float deltaTime)
{
	static float y = 0;
	D3DXMATRIX yRot;
	D3DXMatrixRotationY(&yRot, y);
	y+=deltaTime;
	g_pDevice->SetTransform(D3DTS_WORLD, &yRot);
	g_pDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	g_pDevice->BeginScene();
	g_pDevice->SetStreamSource(NULL, g_pVertexBuffer, 0, sizeof(colorvertex));
	g_pDevice->SetIndices(g_pIndexBuffer);
	g_pDevice->SetFVF(colorvertex::FVF);
	g_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	g_pDevice->EndScene();
	g_pDevice->Present(0, 0, 0, 0);
	return true;
}

bool Setup(int width, int height)
{
	HRESULT hr = g_pDevice->CreateVertexBuffer(8*sizeof(colorvertex), D3DUSAGE_WRITEONLY, colorvertex::FVF, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	if(D3D_OK != hr)
	{
		return false;
	}

	hr = g_pDevice->CreateIndexBuffer(36*sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	if(D3D_OK != hr)
	{
		return false;
	}

	colorvertex* vertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void **)&vertices, 0);
	vertices[0] = colorvertex(-1.0f, -1.0f, -1.0f, RED);
	vertices[1] = colorvertex(-1.0f, 1.0f, -1.0f, GREEN);
	vertices[2] = colorvertex(1.0f, 1.0f, -1.0f, BLUE);
	vertices[3] = colorvertex(1.0f, -1.0f, -1.0f, RED);
	vertices[4] = colorvertex(-1.0f, -1.0f, 1.0f, GREEN);
	vertices[5] = colorvertex(-1.0f, 1.0f, 1.0f, BLUE);
	vertices[6] = colorvertex(1.0f, 1.0f, 1.0f, RED);
	vertices[7] = colorvertex(1.0f, -1.0f, 1.0f, GREEN);
	g_pVertexBuffer->Unlock();

	DWORD* indices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	// front
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back
	indices[6] = 7; indices[7] = 6; indices[8] = 5;
	indices[9] = 5; indices[10] = 4; indices[11] = 7;

	// top
	indices[12] = 1; indices[13] = 5; indices[14] = 6;
	indices[15] = 1; indices[16] = 6; indices[17] = 2;

	// bottom
	indices[18] = 4; indices[19] = 0; indices[20] = 3;
	indices[21] = 4; indices[22] = 3; indices[23] = 7;

	// left
	indices[24] = 4; indices[25] = 5; indices[26] = 1;
	indices[27] = 4; indices[28] = 1; indices[29] = 0;

	// right
	indices[30] = 3; indices[31] = 2; indices[32] = 6;
	indices[33] = 3; indices[34] = 6; indices[35] = 7;
	g_pIndexBuffer->Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	g_pDevice->SetTransform(D3DTS_VIEW, &v);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5F, (float)width/(float)height, 1.0f, 1000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj); 

	//g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void CleanUp()
{
	g_pVertexBuffer->Release();
	g_pIndexBuffer->Release();
}

#endif