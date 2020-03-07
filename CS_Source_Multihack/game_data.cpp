#include "game_data.h"
#include "game.h"

PTR client;
PTR engine;
PTR localPlayerAddr;
PTR entityList;
Player* localPlayer;
Player* entity;
Player* players[MAX_PLAYERS];
DWORD* ForceJump;
UINT key_hook[0xFE];
ViewMatrix vmatrix;
bool updated[MAX_PLAYERS];

bool bhop = false;
bool esp_box = false;
bool esp_line = false;
float color_enemy_box[3] = { 1, 0, 0 };
float color_team_box[3] = { 0, 0, 1 };
float color_snapline[3] = { 1, 0.5, 0};