#pragma once

#include "Block.h"

class MainScene : public Scene
{
public:
	MainScene();

	// ��ʼ����Ϸ
	void startNewGame();

	// ��Ϸʤ��
	void win();

	// ��Ϸ����
	void gameover();

	// ������飨������飩
	void sweepBlock(int index);

	// ��չ���׷���
	void expandBlock(int row, int col);

	// ��Ƿ���
	void markBlock(int index);

	// ��Ƿ���Ϊ����
	void markSuspectBlock(int index);

	// �޸���Ϸ�Ѷ�
	void changeDifficulty();

	// ��ʼ��ʱ
	void startTiming();

	// ������ʱ
	void endTiming();

	// ��ʱ����
	void timing();

	// ��ȡ���λ�õķ����±꣬����겻�ڷ������򷵻� -1
	int getPointInBlock(const Point& point);

	// ��������������
	void setBoomLeftNumber(int leftNum);

	// ���ü�ʱ��������
	void setTimePastSeconds(int seconds);

	// �����û����
	// 0 Ϊ��������1 Ϊ�Ҽ������2Ϊ���Ҽ�ͬʱ���
	void onClick(int button);

	// ��ȡ�û�����
	void onUpdate() override;

private:
	bool _timingStarted = false;
	bool _gameover = true;

	bool _mouseDoubleClick = false;		// ������Ҽ�ͬʱ���
	char _userInput = 0;				// �����û�����

	const float _menuHeight = 40;		// �˵����߶�
	const float _buttonHeight = 100;	// ��ť���߶�

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
