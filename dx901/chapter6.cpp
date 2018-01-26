/*
*	font
*
*/
#include "common.h"
#ifdef _USING_CHAPTER_6_
#include "d3dUtility.h"
using namespace D3D;
ID3DXFont* g_pFont = NULL;
int g_frames = 0;
float g_timeDelta = 0.0f;
int g_fps = 0;

bool Setup( int width, int height )
{
	
	D3DXFONT_DESC fontDesc;
	::ZeroMemory( &fontDesc, sizeof(D3DXFONT_DESC) );
	fontDesc.Height = 40;
	fontDesc.Width = 0;
	fontDesc.Weight = 400;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.Italic = false;
	wcscpy_s(fontDesc.FaceName, L"Times New Roman");

	HRESULT  hr = D3DXCreateFontIndirect( g_pDevice, &fontDesc, &g_pFont);
	if( FAILED( hr ) )
	{
		return false;
	}
	return true;
}

bool Display( float timeDelta )
{
	g_timeDelta += timeDelta;
	g_frames++;
	if(g_timeDelta > 1.0f)
	{
		g_fps = (float)g_frames/g_timeDelta;
		g_frames = 0;
		g_timeDelta = 0.0f;
	}

	g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0 );
	g_pDevice->BeginScene();
	TCHAR strFps[255];
	memset( strFps, 0, sizeof(strFps));
	wsprintf(strFps, L"FPS:%d", g_fps);
	RECT rect = {0, 0, 130, 400};
	g_pFont->DrawText(NULL, strFps, wcslen(strFps),  &rect, DT_LEFT, RED);
	g_pDevice->EndScene();
	g_pDevice->Present( NULL, NULL, NULL, NULL );
	return true;
}

void CleanUp()
{
	g_pFont->Release();
}
#endif