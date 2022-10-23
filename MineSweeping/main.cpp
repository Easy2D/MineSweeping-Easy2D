//---------------------------------------------------------
// �������ƣ�MineSweeping
// ���ߣ�Nomango, Shenyuan Guan
// ���뻷����Visual Studio 2019 / Easy2D v2.1.12
// ��Ŀ���ͣ�Win32 Application
//---------------------------------------------------------

#include "MainScene.h"
#include <easy2d/easy2d.h>

using namespace easy2d;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// ��ʼ��
	if (Game::init())
	{
		// ���ô��ڱ���ʹ�С
		Window::setTitle("Mine Sweeping");

		// ��������������
		auto scene = gcnew MainScene;
		// ��ʼ����Ϸ
		scene->startNewGame();
		SceneManager::enter(scene);

		// ��ʼ��Ϸ
		Game::start();
	}
	// ������Ϸ��Դ
	Game::destroy();
	return 0;
}
