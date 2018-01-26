/*
*	MESH2
*
*/
#include "common.h"
#ifdef _USING_CHAPTER_8_
#include "d3dUtility.h"
#include <vector>
using namespace D3D;

ID3DXMesh* g_pMesh = NULL;
std::vector<D3DMATERIAL9> g_materials;
std::vector<IDirect3DTexture9*> g_textures;


bool Setup( int width, int height )
{
	ID3DXBuffer* pAdjacencyBuffer = NULL;
	ID3DXBuffer* pMaterialsBuffer = NULL;
	DWORD numMaterials = 0;
	D3DXLoadMeshFromX( L".\\xmodal\\airplane 2.x", D3DXMESH_MANAGED, g_pDevice,  &pAdjacencyBuffer, &pMaterialsBuffer, NULL, &numMaterials, &g_pMesh );

	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialsBuffer->GetBufferPointer();
	IDirect3DTexture9* pTexture = NULL;
	char path[255];
	for( unsigned int i = 0; i < numMaterials; i++)
	{
		pMaterials[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;
		g_materials.push_back(pMaterials[i].MatD3D);

		if( pMaterials[i].pTextureFilename != 0 )
		{
			ZeroMemory( path, 255);
			sprintf_s( path, ".\\xmodal\\%s", pMaterials[i].pTextureFilename );
			D3DXCreateTextureFromFileA( g_pDevice, path, &pTexture );
			g_textures.push_back( pTexture );
		}
		else
		{
			g_textures.push_back( 0 );
		}
	}

	pAdjacencyBuffer->Release();
	pMaterialsBuffer->Release();

	if( !(g_pMesh->GetFVF() & D3DFVF_NORMAL) )
	{
		ID3DXMesh* pMesh = NULL;
		g_pMesh->CloneMeshFVF( D3DXMESH_MANAGED, g_pMesh->GetFVF() | D3DFVF_NORMAL, g_pDevice, &pMesh ); 
		D3DXComputeNormals( pMesh, NULL );
		g_pMesh->Release();
		g_pMesh = pMesh;
	}

	D3DXMATRIX view;
	D3DXVECTOR3 pos(0, 0, -5);
	D3DXVECTOR3 target(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH( &view, &pos, &target, &up );
	g_pDevice->SetTransform( D3DTS_VIEW, &view );

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI * 0.5f, (float)width/(float)height, 1.0f, 1000.0f );
	g_pDevice->SetTransform( D3DTS_PROJECTION, &proj );
	
	D3DLIGHT9 light = InitDirectionLight( &XWHITE, &D3DXVECTOR3( 0, 0, 1.0f ) );
	g_pDevice->SetLight( 0, &light );
	g_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	return true;
}

bool Display( float timeDelta )
{
	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0 );
	g_pDevice->BeginScene();
	g_pDevice->SetMaterial(&WHITEMATERIAL);
	g_pDevice->LightEnable(0, true);
	for( unsigned int i = 0; i < g_materials.size(); i++ )
	{
		g_pDevice->SetMaterial( &g_materials[i] );
		g_pDevice->SetTexture( 0, g_textures[i] );
		g_pMesh->DrawSubset( i );
	}
	g_pDevice->LightEnable(0, false);
	g_pDevice->EndScene();
	g_pDevice->Present( NULL, NULL, NULL, NULL );
	return true;
}

void CleanUp()
{
	g_pMesh->Release();
}
#endif