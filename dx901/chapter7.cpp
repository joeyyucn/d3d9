/*
*	MESH 1
*
*/
#include "common.h"
#ifdef _USING_CHAPTER_7_
#include "d3dUtility.h"
#include <fstream>
using namespace D3D;

ID3DXMesh* g_pMesh = NULL;


void DumpAttributeTable( ID3DXMesh* pMesh, std::ofstream& output )
{
	output << "Attribute table"<< std::endl;
	output << "-------------------" << std::endl;
	DWORD tableSize = 0;
	pMesh->GetAttributeTable( NULL, &tableSize );
	
	D3DXATTRIBUTERANGE* tables = new D3DXATTRIBUTERANGE[ tableSize ];
	pMesh->GetAttributeTable( tables, &tableSize );

	for( unsigned int i = 0; i < tableSize; i++ )
	{
		output << "Entry:" << tables[i].AttribId << std::endl;
		output << "-------------------" << std::endl;
		output << "face count:" << tables[i].FaceCount << std::endl;
		output << "face start:" << tables[i].FaceStart << std::endl;
		output << "vertex start:" << tables[i].VertexCount << std::endl;
		output << "vertex count:" << tables[i].VertexStart << std::endl;
		output << std::endl;
	}

	delete [] tables;
}

bool Setup( int width, int height )
{
	HRESULT hr = S_OK;

	hr = D3DXCreateMeshFVF( 4, 12, D3DXMESH_MANAGED, normalTexVertex::FVF, g_pDevice, &g_pMesh );
	if( FAILED( hr ) )
	{
		return false;
	}

	normalTexVertex* pVertices = NULL;
	g_pMesh->LockVertexBuffer( 0, (void **)&pVertices );
	pVertices[0] = normalTexVertex( -1, 1, 0, 0, 0, -1, 0, 0 );
	pVertices[1] = normalTexVertex( 1, 1, 0, 0, 0, -1, 1, 0 );
	pVertices[2] = normalTexVertex( -1, -1, 0, 0, 0, -1, 0, 1 );

	pVertices[3] = normalTexVertex( 1, 1, 0, 0, 0, -1, 1, 0 );
	pVertices[4] = normalTexVertex( 1, -1, 0, 0, 0, -1, 0, 1 );
	pVertices[5] = normalTexVertex( -1, -1, 0, 0, 0, -1, 0, 1 );
	
	pVertices[6] = normalTexVertex( -1, -1, 0, 0, 1, 0, 0, 1 );
	pVertices[7] = normalTexVertex( 1, -1, 0, 0, 1, 0, 0, 1 );
	pVertices[8] = normalTexVertex( -1, -1, -1, 0, 1, 0, 0, 1 );

	pVertices[9] = normalTexVertex( 1, -1, 0, 0, 1, 0, 0, 1 );
	pVertices[10] = normalTexVertex( 1, -1, -1, 0, 1, 0, 0, 1 );
	pVertices[11] = normalTexVertex( -1, -1, -1, 0, 1, 0, 0, 1 );
	g_pMesh->UnlockVertexBuffer();

	WORD* indices = NULL;
	g_pMesh->LockIndexBuffer( 0, (void**)&indices );
	for( int i  = 0; i < 12; i++ )
	{
		indices[i] = i;
	}
	g_pMesh->UnlockIndexBuffer();

	DWORD* attributes = NULL;
	g_pMesh->LockAttributeBuffer( 0, &attributes );
	attributes[0] = 0; attributes[1] = 0;
	attributes[2] = 1; attributes[3] = 1;
	g_pMesh->UnlockAttributeBuffer();
	
	int size = g_pMesh->GetNumFaces() * 3;
	DWORD* adjacency = new DWORD[size];
	g_pMesh->GenerateAdjacency( 0.0f, adjacency);
	g_pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, adjacency, NULL, NULL, NULL );
	delete [] adjacency;

	std::ofstream file;
	file.open("Dump.txt");
	DumpAttributeTable( g_pMesh, file );
	file.close();
	D3DXMATRIX view;
	D3DXVECTOR3 pos( 0, 4, -5.0f );
	D3DXVECTOR3 target( 0, 0, 0 );
	D3DXVECTOR3 up( 0, 1, 0 );
	D3DXMatrixLookAtLH( &view, &pos, &target, &up );
	g_pDevice->SetTransform( D3DTS_VIEW, &view );
	
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI * 0.5f, (float)width/(float)height, 1.0f, 1000.0f );
	g_pDevice->SetTransform( D3DTS_PROJECTION, &proj );

	D3DLIGHT9 light = InitDirectionLight( &XWHITE, &D3DXVECTOR3(0, -1, 1.0f) );
	g_pDevice->SetLight( 0, &light );
	
	g_pDevice->SetRenderState( D3DRS_LIGHTING, true );

	ID3DXBuffer* pBuffer = NULL;
	return true;
}

bool Display( float timeDelta )
{
	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x0, 1.0f, 0 );
	g_pDevice->BeginScene();
	g_pDevice->LightEnable( 0, true );

	g_pDevice->SetMaterial( &WHITEMATERIAL );
	g_pMesh->DrawSubset( 0 );

	g_pDevice->SetMaterial( &REDMATERIAL );
	g_pMesh->DrawSubset( 1 );

	g_pDevice->LightEnable( 0, false );
	g_pDevice->EndScene();
	g_pDevice->Present( NULL, NULL, NULL, NULL );
	return true;
}

void CleanUp()
{
	g_pMesh->Release();
}
#endif