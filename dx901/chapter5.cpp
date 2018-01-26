/*
*	stencil
*
*/
#include "common.h"
#ifdef _USING_CHAPTER_5_
#include "d3dUtility.h"
using namespace D3D;
IDirect3DVertexBuffer9* g_pVertex;
ID3DXMesh* g_pMesh;

bool Setup( int width, int height )
{
	HRESULT hr = S_OK;
	hr = g_pDevice->CreateVertexBuffer( sizeof(normalvertex)*6, D3DUSAGE_WRITEONLY, normalvertex::FVF, D3DPOOL_MANAGED, &g_pVertex, NULL );
	if( FAILED( hr ) )
	{
		return false;
	}

	normalvertex* pVertices = NULL;
	g_pVertex->Lock( 0, 0, (void**)&pVertices, 0 );
	float halfTrangleWidth = 2.0f;
	pVertices[0] = normalvertex( -halfTrangleWidth, halfTrangleWidth, 3, 0, 0, -1 );
	pVertices[1] = normalvertex( halfTrangleWidth, halfTrangleWidth, 3, 0, 0, -1 );
	pVertices[2] = normalvertex( -halfTrangleWidth, -halfTrangleWidth, 3, 0, 0, -1 );

	pVertices[3] = normalvertex( halfTrangleWidth, halfTrangleWidth, 3, 0, 0, -1 );
	pVertices[4] = normalvertex( halfTrangleWidth, -halfTrangleWidth, 3, 0, 0, -1 );
	pVertices[5] = normalvertex( -halfTrangleWidth, -halfTrangleWidth, 3, 0, 0, -1 );
	g_pVertex->Unlock();
	
	// teapot
	D3DXCreateTeapot( g_pDevice, &g_pMesh, NULL );

	// view and projection matrix
	D3DXMATRIX view;
	D3DXVECTOR3 pos( -2, 0, -3 );
	D3DXVECTOR3 up( 0, 1, 0 );
	D3DXVECTOR3 target( 0, 0, 0);
	D3DXMatrixLookAtLH( &view, &pos, &target, &up );
	g_pDevice->SetTransform( D3DTS_VIEW, &view );

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI*0.5F, (float)width/(float)height, 1, 1000 );
	g_pDevice->SetTransform( D3DTS_PROJECTION, &proj );

	g_pDevice->SetRenderState( D3DRS_LIGHTING, true );

	// light 
	D3DLIGHT9 light = InitDirectionLight( &XWHITE, &D3DXVECTOR3( 0, 0, 1 ) );
	g_pDevice->SetLight(0, &light );

	//
	return true;
}

void RenderMirror()
{
	g_pDevice->SetRenderState( D3DRS_STENCILENABLE, true );
	g_pDevice->SetRenderState( D3DRS_STENCILREF, 0x1 );
	g_pDevice->SetRenderState( D3DRS_STENCILMASK, 0xffffffff );
	g_pDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
	g_pDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
	g_pDevice->SetRenderState( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP );
	g_pDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	g_pDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE );

	// disable zbuffer wirte
	g_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, false );
	// disable target buffer write
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
	g_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	g_pDevice->SetStreamSource( 0, g_pVertex, 0, sizeof(normalvertex) );
	g_pDevice->SetFVF( normalvertex::FVF );
	D3DXMATRIX identify;
	D3DXMatrixIdentity( &identify );
	g_pDevice->SetTransform( D3DTS_WORLD, &identify );
	g_pDevice->SetMaterial( &GRAYMATERIAL );
	g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	
	g_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
	
	g_pDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_EQUAL );
	g_pDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

	D3DXMATRIX W, T, R;
	D3DXMatrixTranslation( &T, 0, 0, -2 );
	D3DXPLANE plane ( 0, 0, 3, -1);
	D3DXMatrixReflect( &R, &plane );
	D3DXMatrixTranslation( &T, 0, 0, 0 );
	W = T*R;

	g_pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0, 0 );
	//g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	g_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
	g_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
	g_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
	g_pDevice->SetTransform( D3DTS_WORLD, &W );
	g_pDevice->SetMaterial( &REDMATERIAL );
	g_pMesh->DrawSubset( 0 );
	g_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	g_pDevice->SetRenderState( D3DRS_STENCILENABLE, false );
	g_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
}

bool Display( float timeDelta )
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );
	g_pDevice->SetTransform( D3DTS_WORLD, &identity );
	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0x0, 1, 0 );
	g_pDevice->BeginScene();
	g_pDevice->LightEnable( 0, true );
	g_pDevice->SetStreamSource( 0, g_pVertex, 0, sizeof(normalvertex) );
	g_pDevice->SetFVF( normalvertex::FVF );
	g_pDevice->SetMaterial( &GRAYMATERIAL );
	g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	
	RenderMirror();

	D3DXMatrixIdentity( &identity );

	g_pDevice->SetMaterial( &REDMATERIAL );
	g_pMesh->DrawSubset( 0 );
	g_pDevice->LightEnable( 0, false );
	g_pDevice->EndScene();
	g_pDevice->Present( NULL, NULL, NULL, NULL );
	return true;
}

void CleanUp()
{
	g_pVertex->Release();
	g_pMesh->Release();
}
#endif