#include "includes.h"
#include "phook.h"
#include "game.h"
#include "game_data.h"
#include "putil.h"

HWND window;
void* pDevice[119];
PTR EndSceneAddress;
EndScene oEndScene;
WNDPROC oWndProc;
bool showMenu = false;
Window wrect;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
void DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR color, IDirect3DDevice9* pDevice);
void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 device);
void DrawESP(LPDIRECT3DDEVICE9 device);

void InitImGui(LPDIRECT3DDEVICE9 device)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

bool initialized = false;
long __stdcall PHook::hkEndScene(LPDIRECT3DDEVICE9 device)
{
	if (!initialized)
	{
		InitImGui(device);
		initialized = true;
	}

	wrect.Update(window);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(showMenu)
		CSS::DrawMenu();

	DrawESP(device);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(device);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall PHook::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		key_hook[wParam] = uMsg;
	if (uMsg == WM_KEYDOWN && wParam == KEY_MENU)
		showMenu = !showMenu;
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PHook::Init, hMod, 0, nullptr);
		CreateThread(nullptr, 0, CSS::Hack, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PHook::Shutdown, hMod, 0, nullptr);
		break;
	}
	return TRUE;
}

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h }; //create the rect
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0); //clear = re-draw target area
}

void DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR color, IDirect3DDevice9* pDevice)
{
	DrawFilledRect(x, y, w, thickness, color, pDevice); //Top horiz line
	DrawFilledRect(x, y, thickness, h, color, pDevice); // Left vertical line
	DrawFilledRect((x + w), y, thickness, h, color, pDevice); //right vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness, color, pDevice); //bottom horiz line
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 device)
{
	ID3DXLine* LineL;
	D3DXCreateLine(device, &LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
	LineL->Release();
}

void DrawESP(LPDIRECT3DDEVICE9 device)
{
	if (localPlayerAddr)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (players[i] != nullptr && (PTR)players[i])
			{
				if (updated[i] && players[i]->Dormant == 0 && players[i]->State != 0)
				{
					if (esp_box || esp_line)
					{
						D3DXVECTOR3 pos, out;
						pos = D3DXVECTOR3(players[i]->Pos.x, players[i]->Pos.y, players[i]->Pos.z);
						if (WorldToScreen(device, vmatrix, &pos, &out))
						{
							float dist = Get3dDistance(localPlayer->Pos, players[i]->Pos);
							int width = DEFAULT_BOX_WIDTH / dist;
							int height = DEFAULT_BOX_HEIGHT / dist;
							D3DCOLOR draw_color;
							if (esp_box)
							{
								players[i]->Team == localPlayer->Team
									? draw_color = D3DCOLOR_RGBA((int)(color_team_box[0] * 255), (int)(color_team_box[1] * 255), (int)(color_team_box[2] * 255), (int)255)
									: draw_color = D3DCOLOR_RGBA((int)(color_enemy_box[0] * 255), (int)(color_enemy_box[1] * 255), (int)(color_enemy_box[2] * 255), (int)255);
								DrawBorderBox(out.x - width / 2, out.y - height, width, height, 1, draw_color, device);
							}

							if (esp_line)
							{
								if (players[i]->Team != localPlayer->Team)
								{
									draw_color = D3DCOLOR_RGBA((int)(color_snapline[0] * 255), (int)(color_snapline[1] * 255), (int)(color_snapline[2] * 255), (int)255);
									DrawLine(wrect.GetWidth() / 2, wrect.GetHeight(), out.x, out.y, 1, draw_color, device);
								}
							}
						}
					}
				}
			}

			updated[i] = false;
		}
	}
}