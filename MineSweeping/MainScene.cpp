#include "MainScene.h"
#include "resource.h"

#define MOUSE_RIGHT (0x1 >> 0)
#define MOUSE_LEFT (0x1 >> 1)

MainScene::MainScene()
{
	// 背景
	_background = gcnew Sprite(PNG_BG, L"PNG");
	_background->movePosY(_menuHeight);
	addChild(_background);

	// 菜单背景
	_menuBackground = gcnew Sprite(PNG_MENU_BG, L"PNG");
	addChild(_menuBackground);

	// 笑脸按钮
	auto smile = gcnew Sprite(PNG_SMILE, L"PNG");
	auto smileDown = gcnew Sprite(PNG_SMILE_OOPS, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	smileDown->setAnchor(0.5f, 0.5f);

	_smileBtn = gcnew Button(smile, smileDown, std::bind(&MainScene::startNewGame, this));
	_smileBtn->setAnchor(0.5f, 0.5f);
	addChild(_smileBtn);

	// 难度按钮
	auto difficulty = gcnew Sprite(PNG_DIFFICULTY, L"PNG");
	auto difficultyMouseOver = gcnew Sprite(PNG_DIFFICULT_MOUSEOVER, L"PNG");
	auto difficultyButton = gcnew Button(difficulty, difficultyMouseOver, nullptr, std::bind(&MainScene::changeDifficulty, this));
	addChild(difficultyButton);

	// 文字样式
	Font font = Font(L"黑体", 22);
	Text::Style style = Text::Style(Color::Black);

	// 剩余炸弹数量文字
	_boomLeftText = gcnew Text(L"雷数 0", font, style);
	_boomLeftText->setAnchor(0.5f, 0.5f);
	addChild(_boomLeftText);

	// 计时文字
	_timePastText = gcnew Text(L"时间 0", font, style);
	_timePastText->setAnchor(0.5f, 0.5f);
	addChild(_timePastText);

	// 游戏区域
	_gameArea = gcnew Node;
	_gameArea->movePosY(_menuHeight + _buttonHeight);
	addChild(_gameArea);
}

void MainScene::startNewGame()
{
	// 修改窗口大小
	Window::setSize(_blockCol * BLOCK_SIZE, _blockRow * BLOCK_SIZE + _menuHeight + _buttonHeight);

	// 调整菜单背景图片大小
	_menuBackground->setWidth(Window::getWidth());

	// 调整按钮区背景图片大小
	_background->setWidth(Window::getWidth());
	_background->setHeight(_buttonHeight - 1);

	// 调整笑脸按钮和文字位置
	_smileBtn->setPos(Window::getWidth() / 2, _buttonHeight / 2 + _menuHeight);
	_boomLeftText->setPos(Window::getWidth() / 4, _buttonHeight / 2 + _menuHeight);
	_timePastText->setPos(Window::getWidth() / 4 * 3, _buttonHeight / 2 + _menuHeight);

	// 重置笑脸图片
	auto smile = gcnew Sprite(PNG_SMILE, L"PNG");
	auto smileDown = gcnew Sprite(PNG_SMILE_OOPS, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	smileDown->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(smile);
	_smileBtn->setSelected(smileDown);

	// 重置时间和雷数量
	setBoomLeftNumber(_boomCount);
	setTimePastSeconds(0);
	endTiming();

	// 重新生成所有方块
	_gameArea->removeAllChildren();
	_blocks.clear();
	for (int r = 0; r < _blockRow; r++)
	{
		for (int c = 0; c < _blockCol; c++)
		{
			auto block = gcnew Block;
			block->setType(Block::Type::Empty);
			block->setPos(c * BLOCK_SIZE, r * BLOCK_SIZE);

			_gameArea->addChild(block);
			_blocks.push_back(block);
		}
	}

	// 布雷
	int blockCount = _blockRow * _blockCol;
	for (int i = 0; i < _boomCount;)
	{
		// 随机生成下标
		int index = Random::range(0, blockCount - 1);
		// 如果不是雷就埋雷，如果是雷就跳过
		if (_blocks[index]->isSafeType())
		{
			_blocks[index]->setType(Block::Type::Boom);
			i++;
		}
	}

	// 计算方块周围的地雷数量
	for (int row = 0; row < _blockRow; row++)
	{
		for (int col = 0; col < _blockCol; col++)
		{
			// 对雷四周的方块数字+1
			auto block = _blocks[row * _blockCol + col];
			if (!block->isSafeType())
			{
				auto collectAroundBlock = [=](int row, int col)
				{
					if (row >= 0 && row < _blockRow && col >= 0 && col < _blockCol)
					{
						int oldNum = _blocks[row * _blockCol + col]->getNumber();
						_blocks[row * _blockCol + col]->setNumber(oldNum + 1);
					}
				};

				collectAroundBlock(row + 1, col);
				collectAroundBlock(row - 1, col);
				collectAroundBlock(row, col + 1);
				collectAroundBlock(row, col - 1);
				collectAroundBlock(row + 1, col + 1);
				collectAroundBlock(row + 1, col - 1);
				collectAroundBlock(row - 1, col + 1);
				collectAroundBlock(row - 1, col - 1);
			}
		}
	}

	// 重载方块图片
	for (auto block : _blocks)
	{
		block->reloadImage();
	}

	// 游戏结束标记
	_gameover = false;
}

void MainScene::win()
{
	// 游戏结束标记
	_gameover = true;

	// 所有雷做标记
	for (auto block : _blocks)
	{
		if (!block->isSafeType())
			block->setState(Block::State::Marked);
	}

	// 暂停计时
	endTiming();

	// 笑脸戴墨镜
	auto smile = gcnew Sprite(PNG_SMILE_WITH_GLASSES, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(smile);
}

void MainScene::gameover()
{
	// 游戏结束标记
	_gameover = true;

	// 显示所有方块
	for (auto block : _blocks)
	{
		if (block->getState() != Block::State::Boom)
			block->setState(Block::State::Show);
	}

	// 暂停计时
	endTiming();

	// 笑脸换哭脸
	auto cry = gcnew Sprite(PNG_CRY, L"PNG");
	cry->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(cry);
}

void MainScene::sweepBlock(int index)
{
	Block* block = _blocks[index];
	Block::State oldState = block->getState();

	// 忽略已经显示的方块
	if (oldState != Block::State::Show)
	{
		if (block->isSafeType())
		{
			// 隐藏当前方块（清除旗子标记）
			block->setState(Block::State::Hidden);

			// 展开所有非雷方块
			int row = index / _blockCol;
			int col = index % _blockCol;
			expandBlock(row, col);

			// 统计未显示的方块数量
			int unshow = 0;
			for (auto block : _blocks)
			{
				if (block->getState() != Block::State::Show)
					unshow++;
			}

			// 判断游戏是否胜利
			if (unshow == _boomCount)
			{
				win();
				return;
			}
		}
		else
		{
			block->setState(Block::State::Boom);
			gameover();
			return;
		}
	}

	// 开始计时
	startTiming();
}

void MainScene::expandBlock(int row, int col)
{
	if (row < 0 || row >= _blockRow || col < 0 || col >= _blockCol)
		return;

	Block* block = _blocks[row * _blockCol + col];
	Block::State oldState = block->getState();

	if (oldState != Block::State::Hidden || !block->isSafeType())
	{
		// 方块已经显示或方块是雷，则不再扩展
		return;
	}

	// 显示方块
	block->setState(Block::State::Show);

	// 扩展上下左右方块
	auto expandEmptyBlocks = [=](bool expandUnempty, int row, int col)
	{
		// 当前方块数字为 0 时，扩展所有四周方块
		// 当前方块数字不为 0 时，只扩展四周数字为 0 的方块
		if (row >= 0 && row < _blockRow && col >= 0 && col < _blockCol)
		{
			if (expandUnempty || _blocks[row * _blockCol + col]->getNumber() == 0)
				expandBlock(row, col);
		}
	};

	bool expandUnempty = (block->getNumber() == 0);
	expandEmptyBlocks(expandUnempty, row + 1, col);
	expandEmptyBlocks(expandUnempty, row - 1, col);
	expandEmptyBlocks(expandUnempty, row, col + 1);
	expandEmptyBlocks(expandUnempty, row, col - 1);
	expandEmptyBlocks(expandUnempty, row + 1, col + 1);
	expandEmptyBlocks(expandUnempty, row + 1, col - 1);
	expandEmptyBlocks(expandUnempty, row - 1, col + 1);
	expandEmptyBlocks(expandUnempty, row - 1, col - 1);
}

void MainScene::markBlock(int index)
{
	Block* block = _blocks[index];
	Block::State oldState = block->getState();
	if (oldState == Block::State::Hidden || oldState == Block::State::Suspect)
	{
		block->setState(Block::State::Marked);
		setBoomLeftNumber(_boomLeft - 1);
	}
	else if (oldState == Block::State::Marked)
	{
		block->setState(Block::State::Hidden);
		setBoomLeftNumber(_boomLeft + 1);
	}

	// 开始计时
	startTiming();
}

void MainScene::markSuspectBlock(int index)
{
	Block* block = _blocks[index];
	Block::State oldState = block->getState();
	if (oldState == Block::State::Hidden)
	{
		block->setState(Block::State::Suspect);
	}
	else if (oldState == Block::State::Suspect)
	{
		block->setState(Block::State::Hidden);
	}
	else if (oldState == Block::State::Marked)
	{
		block->setState(Block::State::Suspect);
		setBoomLeftNumber(_boomLeft + 1);
	}

	// 开始计时
	startTiming();
}

void MainScene::changeDifficulty()
{
	auto difficultySettingScene = gcnew Scene;

	Font font = Font(L"黑体", 22);
	Text::Style style = Text::Style(Color::Black);

	auto level1 = gcnew Text(L"9x9 布局含 10 颗雷", font, style);
	auto level2 = gcnew Text(L"16x16 布局含 25 颗雷", font, style);
	auto level3 = gcnew Text(L"16x16 布局含 40 颗雷", font, style);

	auto level1Btn = gcnew Button(level1, [=]()
		{
			_blockRow = _blockCol = 9;
			_boomCount = 10;
			startNewGame();
			SceneManager::back();
		});
	auto level2Btn = gcnew Button(level2, [=]()
		{
			_blockRow = _blockCol = 16;
			_boomCount = 25;
			startNewGame();
			SceneManager::back();
		});
	auto level3Btn = gcnew Button(level3, [=]()
		{
			_blockRow = _blockCol = 16;
			_boomCount = 40;
			startNewGame();
			SceneManager::back();
		});

	// 背景
	auto background = gcnew Sprite(PNG_BG, L"PNG");
	background->setSize(400, 300);
	difficultySettingScene->addChild(background);
	difficultySettingScene->addChild(level1Btn);
	difficultySettingScene->addChild(level2Btn);
	difficultySettingScene->addChild(level3Btn);

	level1Btn->setAnchor(0.5f, 0.5f);
	level2Btn->setAnchor(0.5f, 0.5f);
	level3Btn->setAnchor(0.5f, 0.5f);
	level1Btn->setPos(200, 70);
	level2Btn->setPos(200, 140);
	level3Btn->setPos(200, 210);

	Window::setSize(400, 280);
	SceneManager::enter(difficultySettingScene);
}

void MainScene::startTiming()
{
	if (!_timingStarted)
	{
		_timingStarted = true;
		// 开始计时
		Timer::add(std::bind(&MainScene::timing, this), 1.0f, -1);
	}
}

void MainScene::endTiming()
{
	if (_timingStarted)
	{
		_timingStarted = false;
		// 结束计时
		Timer::removeAll();
	}
}

void MainScene::timing()
{
	setTimePastSeconds(_timePast + 1);
}

int MainScene::getPointInBlock(const Point& point)
{
	for (int i = 0; i < _blocks.size(); i++)
	{
		if (_blocks[i]->containsPoint(point))
		{
			return i;
		}
	}
	return -1;
}

void MainScene::setBoomLeftNumber(int leftNum)
{
	_boomLeft = leftNum;
	if (_boomLeft > 0)
	{
		_boomLeftText->setText(FormatString(L"雷数 %d", _boomLeft));
	}
	else
	{
		_boomLeftText->setText(L"雷数 0");
	}
}

void MainScene::setTimePastSeconds(int seconds)
{
	_timePast = seconds;
	_timePastText->setText(FormatString(L"时间 %d", _timePast));
}

void MainScene::onClick(int button)
{
	int index = getPointInBlock(Input::getMousePos());
	if (index == -1)
		return;

	switch (button)
	{
	case 0:
		sweepBlock(index);
		break;
	case 1:
		markBlock(index);
		break;
	case 2:
		markSuspectBlock(index);
		break;
	}
}

void MainScene::onUpdate()
{
	if (_gameover)
		return;

	if (Input::isPress(MouseCode::Left))
		_userInput |= MOUSE_LEFT;

	if (Input::isPress(MouseCode::Right))
		_userInput |= MOUSE_RIGHT;

	if (Input::isRelease(MouseCode::Left))
	{
		if (_userInput & MOUSE_RIGHT)
		{
			// 如果右键按下了
			_mouseDoubleClick = true;
		}
		else if (_mouseDoubleClick)
		{
			_mouseDoubleClick = false;
			onClick(2);
		}
		else
		{
			// 左键点击了方块
			onClick(0);
		}
		_userInput = 0;
	}

	if (Input::isRelease(MouseCode::Right))
	{
		if (_userInput & MOUSE_LEFT)
		{
			// 如果左键按下了
			_mouseDoubleClick = true;
		}
		else if (_mouseDoubleClick)
		{
			_mouseDoubleClick = false;
			onClick(2);
		}
		else
		{
			// 右键点击了方块
			onClick(1);
		}
		_userInput = 0;
	}
}
