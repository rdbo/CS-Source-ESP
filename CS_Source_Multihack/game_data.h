#pragma once
#include "includes.h"
#include "game.h"
//#include "putil.h"
#define MAX_PLAYERS 32
#define ACTION_VALUE 6
#define KEY_MENU VK_INSERT
#define KEY_BHOP VK_SPACE
#define STATE_IS_ALIVE 2

#define ESP_BOX_ENEMY D3DCOLOR_RGBA(255, 0, 0, 255)
#define ESP_BOX_TEAM D3DCOLOR_RGBA(0, 0, 255, 255)
#define DEFAULT_BOX_WIDTH 18100/2
#define DEFAULT_BOX_HEIGHT 36000/2
//#define DEFAULT_BOX_WIDTH 15838
//#define DEFAULT_BOX_HEIGHT 24000

typedef struct { float matrix[4][4]; } ViewMatrix;

struct iVec2
{
	int x, y;
};

struct flVec3
{
	float x, y, z;
};

struct flVec4
{
	float x, y, z, w;
};

using namespace CSS::Offsets::Player;
class Player
{
public:
	char pad[dwHealth];
	DWORD Health;
	char pad2[dwTeam - (dwHealth + sizeof(Health))];
	DWORD Team;
	char pad3[dwState - (dwTeam + sizeof(Team))];
	DWORD State;
	char pad4[bDormant - (dwState + sizeof(State))];
	byte Dormant;
	char pad5[vflPos - (bDormant + sizeof(Dormant))];
	flVec3 Pos;
	char pad6[dwFlags - (vflPos + sizeof(Pos))];
	DWORD Flags;
};

//Variables

extern UINT key_hook[0xFE];
extern PTR client;
extern PTR engine;
extern PTR localPlayerAddr;
extern PTR entityList;
extern Player* localPlayer;
extern DWORD* ForceJump;
extern Player* players[MAX_PLAYERS];
extern ViewMatrix vmatrix;
extern bool updated[MAX_PLAYERS];

extern bool bhop;
extern bool esp_box;
extern bool esp_line;
extern float color_enemy_box[3];
extern float color_team_box[3];
extern float color_snapline[3];