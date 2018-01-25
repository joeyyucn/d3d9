/*
*	blending
*
*/
#include "common.h"
#ifdef _USING_CHAPTER_4_
#include "d3dUtility.h"
#include "d3dUtility.h"
using namespace D3D;

D3DMATERIAL9 opaqueMaterial = InitMaterial(XRED, D3DXCOLOR(0, 1.0f, 0, 0.5f), XRED, XBLACK, 8.0f);
IDirect3DTexture9* g_pTexture;
IDirect3DVertexBuffer9* g_pVertexBuffer;

bool Setup( int width, int height )
{
	HRESULT hr = S_OK;
	hr = D3DXCreateTextureFromFile( g_pDevice, L"texture.dds", &g_pTexture );
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
	
	pVertices[0] = normalTexVertex( -1, 1, 1, 0, 0, -1, 0, 0 );
	pVertices[1] = normalTexVertex( 1, 1, 1, 0, 0, -1, 1, 0 );
	pVertices[2] = normalTexVertex( -1, -1, 1, 0, 0, -1, 0, 1 );

	pVertices[3] = normalTexVertex( -1, -1, 1, 0, 0, -1, 0, 1 );
	pVertices[4] = normalTexVertex( 1, 1, 1, 0, 0, -1, 1, 0 );
	pVertices[5] = normalTexVertex( 1, -1, 1, 0, 0, -1, 1, 1 );

	g_pVertexBuffer->Unlock();

	if( FAILED( hr ) )
	{
		g_pVertexBuffer->Release();
		g_pTexture->Release();
		return false;
	}

	D3DXMATRIX v;
	D3DXVECTOR3 pos(0, 0, -3);
	D3DXVECTOR3 target(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH( &v, &pos, &target, &up );
	g_pDevice->SetTransform( D3DTS_VIEW, &v );

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI * 0.5f, (float)width/(float)height, 1, 1000);
	g_pDevice->SetTransform( D3DTS_PROJECTION, &proj );
	g_pDevice->SetRenderState( D3DRS_LIGHTING, true );
	
	g_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

	g_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	

	opaqueMaterial.Diffuse.a = 0.5f;
	return true;
}

bool Display( float timeDeta )
{
	if( GetAsyncKeyState( 'A' ) & 0x8000f )
	{
		opaqueMaterial.Diffuse.a += 0.01f;
	}

	if( GetAsyncKeyState( 'S' ) & 0x8000f )
	{
		opaqueMaterial.Diffuse.a -= 0.01f;
	}

	if( opaqueMaterial.Diffuse.a > 1.0f )
	{
		opaqueMaterial.Diffuse.a = 1.0f;
	}
	
	if( opaqueMaterial.Diffuse.a < 0.0f )
	{
		opaqueMaterial.Diffuse.a = 0.0f;
	}

	D3DXVECTOR3 direction( 0, 0, 1.0f );
	D3DLIGHT9 light = InitDirectionLight(&XWHITE, &direction); 
	g_pDevice->SetLight(0, &light);
	g_pDevice->LightEnable(0, true);

	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x0, 1, 0 );
	g_pDevice->BeginScene();
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pDevice->SetMaterial( &WHITEMATERIAL );
	g_pDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(normalTexVertex) );
	g_pDevice->SetFVF( normalTexVertex::FVF );
	g_pDevice->SetTexture( 0, g_pTexture );
	g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ); 
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	
	
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );;
	LPD3DXMESH pTeapot = NULL;
	D3DXCreateTeapot( g_pDevice, &pTeapot, NULL );
	g_pDevice->SetMaterial( &opaqueMaterial );
	pTeapot->DrawSubset(0);
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	g_pDevice->LightEnable( 0, false );
	
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