#pragma once
#include "includes.h"
#include "game_data.h"

struct Window : public RECT
{
	void SetRect(RECT rect);
	int GetWidth();
	int GetHeight();
};

bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, ViewMatrix vmatrix, D3DXVECTOR3* pos, D3DXVECTOR3* out);
float Get3dDistance(flVec3 pos, flVec3 enemyPos);