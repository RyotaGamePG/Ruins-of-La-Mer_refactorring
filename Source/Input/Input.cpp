#include "Input/Input.h"

Input* Input::instance = nullptr;

// �R���X�g���N�^
Input::Input(HWND hWnd)
	: mouse(hWnd)
{
	instance = this;
}

// �X�V����
void Input::Update()
{
	gamePad.Update();
	mouse.Update();
}
