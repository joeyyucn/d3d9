/*
*	this chapter is about texturing
*	include:
*	create texture from file
*	create a primitive with texture coordinate
*/

#include "common.h"

#ifdef _USING_CHAPTER_3_
#include "d3dUtility.h"
using namespace D3D;

IDirect3DTexture9* g_pTexture;
IDirect3DVertexBuffer9* g_pVertexBuffer;
IDirect3DVertexBuffer9* g_pVertexBuffer2;

bool Setup( int width, int height )
{
	HRESULT hr = S_OK;
	hr = D3DXCreateTextureFromFile( g_pDevice, L"texture.bmp", &g_pTexture );
	if( FAILED( hr ) )
	{
		return false;
	}

	hr = g_pDevice->CreateVertexBuffer( sizeof(normalTexVertex) * 6, D3DUSAGE_WRITEONLY, normalTexVertex::FVF, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	if( FAILED( hr ) )
	{
		g_pTexture->Release();
		return false;
	}

	normalTexVertex* pVertices = NULL;
	hr = g_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
	if( FAILED( hr ) )
	{
		g_pTexture->Release();
		g_pVertexBuffer->Release();
		return false;
	}
	
	pVertices[0] = normalTexVertex( -1, 1, -1, 0, 0, -1, 0, 0 );
	pVertices[1] = normalTexVertex( 1, 1, -1, 0, 0, -1, 1, 0 );
	pVertices[2] = normalTexVertex( -1, -1, -1, 0, 0, -1, 0, 1 );

	pVertices[3] = normalTexVertex( -1, -1, -1, 0, 0, -1, 0, 1 );
	pVertices[4] = normalTexVertex( 1, 1, -1, 0, 0, -1, 1, 0 );
	pVertices[5] = normalTexVertex( 1, -1, -1, 0, 0, -1, 1, 1 );

	g_pVertexBuffer->Unlock();

	hr = g_pDevice->CreateVertexBuffer( sizeof(vertex)*3, D3DUSAGE_WRITEONLY, vertex::FVF, D3DPOOL_MANAGED, &g_pVertexBuffer2, NULL );
	if( FAILED( hr ) )
	{
		g_pVertexBuffer->Release();
		g_pTexture->Release();
		return false;
	}

	vertex* pVertices2;
	g_pVertexBuffer2->Lock( 0, 0, (void**)&pVertices2, NULL );
	pVertices2[0] = vertex( -1, 1, -1 );
	pVertices2[1] = vertex( 1, 1, -1 );
	pVertices2[2] = vertex( 1, -1, -1 );
	g_pVertexBuffer2->Unlock();

	D3DXMATRIX v;
	D3DXVECTOR3 pos(0, 0, -3);
	D3DXVECTOR3 target(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH( &v, &pos, &target, &up );
	g_pDevice->SetTransform( D3DTS_VIEW, &v );

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI * 0.5f, (float)width/(float)height, 1, 1000);
	g_pDevice->SetTransform( D3DTS_PROJECTION, &proj );
	
	g_pDevice->SetRenderState( D3DRS_LIGHTING, false );
	//g_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	//g_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	return true;
}

bool Display( float timeDeta )
{
	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x0, 1, 0 );
	g_pDevice->BeginScene();

	g_pDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(normalTexVertex) );
	g_pDevice->SetFVF( normalTexVertex::FVF );
	g_pDevice->SetTexture( 0, g_pTexture );
	g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ); 
	
	
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DXMATRIX trans;
	D3DXMatrixTranslation( &trans, 1, 0, 0);
	g_pDevice->SetTransform( D3DTS_WORLD, &trans );
	g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	D3DXMatrixTranslation( &trans, -1, 0, 0);
	g_pDevice->SetTransform( D3DTS_WORLD, &trans );
	
	g_pDevice->EndScene();
	g_pDevice->Present( NULL, NULL, NULL, NULL );
	return true;
}

void CleanUp()
{
	g_pTexture->Release();
	g_pVertexBuffer->Release();
}

#endif