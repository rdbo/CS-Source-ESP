#include "putil.h"

int Window::GetWidth()
{
	return right - left;
}

int Window::GetHeight()
{
	return bottom - top;
}

void Window::Update(HWND window)
{
	if (window == NULL) return;
	HWND hWndParent = GetParent(window);
	POINT p = { 0 };
	MapWindowPoints(window, hWndParent, &p, 1);

	this->left = p.x;
	this->top = p.y;

	RECT r;
	GetClientRect(window, &r);
	this->right = r.right + this->left;
	this->bottom = r.bottom + this->top;
}

bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, ViewMatrix vmatrix, D3DXVECTOR3* pos, D3DXVECTOR3* out)
{
	D3DXVECTOR2 screenVec;
	screenVec.x = pos->x * vmatrix.matrix[0][0] + pos->y * vmatrix.matrix[0][1] + pos->z * vmatrix.matrix[0][2] + vmatrix.matrix[0][3];
	screenVec.y = pos->x * vmatrix.matrix[1][0] + pos->y * vmatrix.matrix[1][1] + pos->z * vmatrix.matrix[1][2] + vmatrix.matrix[1][3];

	const auto w = pos->x * vmatrix.matrix[3][0] + pos->y * vmatrix.matrix[3][1] + pos->z * vmatrix.matrix[3][2] + vmatrix.matrix[3][3];
	if (w < 0.1f)
		return false;

	screenVec.x /= w;
	screenVec.y /= w;

	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	out->x = (viewPort.Width / 2) + ((screenVec.x / 2) * viewPort.Width + 0.5f);
	out->y = (viewPort.Height / 2) - ((screenVec.y / 2) * viewPort.Height + 0.5f);
	out->z = 0.0f;
}

float Get3dDistance(flVec3 pos, flVec3 enemyPos)
{
	return sqrt(
		pow(double(enemyPos.x - pos.x), 2.0) +
		pow(double(enemyPos.y - pos.y), 2.0) +
		pow(double(enemyPos.z - pos.z), 2.0));
}