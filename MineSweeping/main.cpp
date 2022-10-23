//---------------------------------------------------------
// 程序名称：MineSweeping
// 作者：Nomango, Shenyuan Guan
// 编译环境：Visual Studio 2019 / Easy2D v2.1.12
// 项目类型：Win32 Application
//---------------------------------------------------------

#include "MainScene.h"
#include <easy2d/easy2d.h>

using namespace easy2d;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// 初始化
	if (Game::init())
	{
		// 设置窗口标题和大小
		Window::setTitle("Mine Sweeping");

		// 创建场景并进入
		auto scene = gcnew MainScene;
		// 开始新游戏
		scene->startNewGame();
		SceneManager::enter(scene);

		// 开始游戏
		Game::start();
	}
	// 销毁游戏资源
	Game::destroy();
	return 0;
}
