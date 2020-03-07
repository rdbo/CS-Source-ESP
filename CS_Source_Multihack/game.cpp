#include "game.h"
#include "game_data.h"
#include "phook.h"
#include "putil.h"
using namespace CSS;

bool CheckModules();
void Bunnyhop();

DWORD WINAPI CSS::Hack(LPVOID lpReserved)
{
	do
	{
		client = (PTR)GetModuleHandle("client.dll");
		engine = (PTR)GetModuleHandle("engine.dll");
	} while (!CheckModules());

	ForceJump = (DWORD*)(client + Offsets::Client::dwForceJump);

	while (true)
	{
		localPlayerAddr = *(PTR*)(client + Offsets::Client::dwLocalPlayer);
		if (localPlayerAddr)
		{
			localPlayer = (Player*)(localPlayerAddr);
			entityList = (PTR)(client + Offsets::Client::dwEntityList);
			vmatrix = *(ViewMatrix*)(engine + Offsets::Engine::dwViewMatrix);
			Bunnyhop();
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				PTR entityAddr = *(PTR*)(entityList + i * Offsets::Player::LoopDistance);
				if (entityAddr && entityAddr != localPlayerAddr)
				{
					players[i] = (Player*)(entityAddr);
					updated[i] = players[i]->Dormant == 0 && players[i]->State == STATE_IS_ALIVE;
				}

				else
				{
					updated[i] = false;
				}
			}
		}
	}

	return TRUE;
}

void CSS::DrawMenu()
{
	ImGui::Begin("CS:Source Multihack by rdbo");
	ImGui::BeginTabBar("navbar");
	if (ImGui::BeginTabItem("ESP"))
	{
		ImGui::Checkbox("ESP SnapLines", &esp_line);
		ImGui::ColorPicker3("Snapline Color", color_snapline);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("ESP Box"))
	{
		ImGui::Checkbox("Enable", &esp_box);
		ImGui::ColorPicker3("Enemy Color", color_enemy_box);
		ImGui::ColorPicker3("Team Color", color_team_box);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Misc"))
	{
		ImGui::Checkbox("Bunnyhop", &bhop);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Debug"))
	{
		ImGui::Text("Window Rect: { %i, %i, %i, %i }", wrect.top, wrect.left, wrect.right, wrect.bottom);
		ImGui::Text("Width: %i", wrect.GetWidth());
		ImGui::Text("Height: %i", wrect.GetHeight());
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	ImGui::End();
}

bool CheckModules()
{
	return client && client != (PTR)INVALID_HANDLE_VALUE && engine && engine != (PTR)INVALID_HANDLE_VALUE;
}

void Bunnyhop()
{
	if (bhop && localPlayer->Flags & (1 << 0) && key_hook[KEY_BHOP] == WM_KEYDOWN)
	{
		*ForceJump = ACTION_VALUE;
	}
}