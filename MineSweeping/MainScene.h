#pragma once

#include "Block.h"

class MainScene : public Scene
{
public:
	MainScene();

	// 开始新游戏
	void startNewGame();

	// 游戏胜利
	void win();

	// 游戏结束
	void gameover();

	// 清除方块（点击方块）
	void sweepBlock(int index);

	// 扩展非雷方块
	void expandBlock(int row, int col);

	// 标记方块
	void markBlock(int index);

	// 标记方块为怀疑
	void markSuspectBlock(int index);

	// 修改游戏难度
	void changeDifficulty();

	// 开始计时
	void startTiming();

	// 结束计时
	void endTiming();

	// 计时函数
	void timing();

	// 获取鼠标位置的方块下标，若鼠标不在方块上则返回 -1
	int getPointInBlock(const Point& point);

	// 设置雷数量文字
	void setBoomLeftNumber(int leftNum);

	// 设置计时秒数文字
	void setTimePastSeconds(int seconds);

	// 处理用户点击
	// 0 为左键点击，1 为右键点击，2为左右键同时点击
	void onClick(int button);

	// 获取用户按键
	void onUpdate() override;

private:
	bool _timingStarted = false;
	bool _gameover = true;

	bool _mouseDoubleClick = false;		// 鼠标左右键同时点击
	char _userInput = 0;				// 储存用户输入

	const float _menuHeight = 40;		// 菜单区高度
	const float _buttonHeight = 100;	// 按钮区高度

	int _blockRow = 9;
	int _blockCol = 9;
	int _boomCount = 10;
	int _boomLeft = 0;
	int _timePast = 0;

	Node* _gameArea = nullptr;
	Sprite* _background = nullptr;
	Sprite* _menuBackground = nullptr;
	Button* _smileBtn = nullptr;
	Text* _boomLeftText = nullptr;
	Text* _timePastText = nullptr;

	std::vector<Block*> _blocks;
};
