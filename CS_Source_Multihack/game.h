#pragma once
#include "includes.h"

//v = vector/array
//dw = DWORD
//fl = float
//b = bool

namespace CSS
{
	namespace Offsets
	{
		namespace Client
		{
			const PTR dwLocalPlayer = 0x4C6708;
			const PTR dwEntityList = 0x4D3904;
			const PTR dwForceJump = 0x4F3B3C;
		}

		namespace Engine
		{
			const PTR dwViewMatrix = 0x5ADBF8;
		}

		namespace Player
		{
			const PTR dwHealth = 0x94;
			const PTR dwTeam = 0x9C;
			const PTR dwState = 0x178;
			const PTR bDormant = 0x17E;
			const PTR vflPos = 0x260;
			const PTR dwFlags = 0x350;
			const PTR LoopDistance = 0x10;
		}
	}

	DWORD WINAPI Hack(LPVOID lpReserved);
	void DrawMenu();
}