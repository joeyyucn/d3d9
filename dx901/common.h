#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define _USING_CHAPTER_8_
bool Setup(int width, int height);
bool Display( float timeDelta );
void CleanUp();

extern IDirect3DDevice9* g_pDevice;