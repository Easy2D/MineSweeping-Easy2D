#include "MainScene.h"
#include "resource.h"

#define MOUSE_RIGHT (0x1 >> 0)
#define MOUSE_LEFT (0x1 >> 1)

MainScene::MainScene()
{
	// ����
	_background = gcnew Sprite(PNG_BG, L"PNG");
	_background->movePosY(_menuHeight);
	addChild(_background);

	// �˵�����
	_menuBackground = gcnew Sprite(PNG_MENU_BG, L"PNG");
	addChild(_menuBackground);

	// Ц����ť
	auto smile = gcnew Sprite(PNG_SMILE, L"PNG");
	auto smileDown = gcnew Sprite(PNG_SMILE_OOPS, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	smileDown->setAnchor(0.5f, 0.5f);

	_smileBtn = gcnew Button(smile, smileDown, std::bind(&MainScene::startNewGame, this));
	_smileBtn->setAnchor(0.5f, 0.5f);
	addChild(_smileBtn);

	// �ѶȰ�ť
	auto difficulty = gcnew Sprite(PNG_DIFFICULTY, L"PNG");
	auto difficultyMouseOver = gcnew Sprite(PNG_DIFFICULT_MOUSEOVER, L"PNG");
	auto difficultyButton = gcnew Button(difficulty, difficultyMouseOver, nullptr, std::bind(&MainScene::changeDifficulty, this));
	addChild(difficultyButton);

	// ������ʽ
	Font font = Font(L"����", 22);
	Text::Style style = Text::Style(Color::Black);

	// ʣ��ը����������
	_boomLeftText = gcnew Text(L"���� 0", font, style);
	_boomLeftText->setAnchor(0.5f, 0.5f);
	addChild(_boomLeftText);

	// ��ʱ����
	_timePastText = gcnew Text(L"ʱ�� 0", font, style);
	_timePastText->setAnchor(0.5f, 0.5f);
	addChild(_timePastText);

	// ��Ϸ����
	_gameArea = gcnew Node;
	_gameArea->movePosY(_menuHeight + _buttonHeight);
	addChild(_gameArea);
}

void MainScene::startNewGame()
{
	// �޸Ĵ��ڴ�С
	Window::setSize(_blockCol * BLOCK_SIZE, _blockRow * BLOCK_SIZE + _menuHeight + _buttonHeight);

	// �����˵�����ͼƬ��С
	_menuBackground->setWidth(Window::getWidth());

	// ������ť������ͼƬ��С
	_background->setWidth(Window::getWidth());
	_background->setHeight(_buttonHeight - 1);

	// ����Ц����ť������λ��
	_smileBtn->setPos(Window::getWidth() / 2, _buttonHeight / 2 + _menuHeight);
	_boomLeftText->setPos(Window::getWidth() / 4, _buttonHeight / 2 + _menuHeight);
	_timePastText->setPos(Window::getWidth() / 4 * 3, _buttonHeight / 2 + _menuHeight);

	// ����Ц��ͼƬ
	auto smile = gcnew Sprite(PNG_SMILE, L"PNG");
	auto smileDown = gcnew Sprite(PNG_SMILE_OOPS, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	smileDown->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(smile);
	_smileBtn->setSelected(smileDown);

	// ����ʱ���������
	setBoomLeftNumber(_boomCount);
	setTimePastSeconds(0);
	endTiming();

	// �����������з���
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

	// ����
	int blockCount = _blockRow * _blockCol;
	for (int i = 0; i < _boomCount;)
	{
		// ��������±�
		int index = Random::range(0, blockCount - 1);
		// ��������׾����ף�������׾�����
		if (_blocks[index]->isSafeType())
		{
			_blocks[index]->setType(Block::Type::Boom);
			i++;
		}
	}

	// ���㷽����Χ�ĵ�������
	for (int row = 0; row < _blockRow; row++)
	{
		for (int col = 0; col < _blockCol; col++)
		{
			// �������ܵķ�������+1
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

	// ���ط���ͼƬ
	for (auto block : _blocks)
	{
		block->reloadImage();
	}

	// ��Ϸ�������
	_gameover = false;
}

void MainScene::win()
{
	// ��Ϸ�������
	_gameover = true;

	// �����������
	for (auto block : _blocks)
	{
		if (!block->isSafeType())
			block->setState(Block::State::Marked);
	}

	// ��ͣ��ʱ
	endTiming();

	// Ц����ī��
	auto smile = gcnew Sprite(PNG_SMILE_WITH_GLASSES, L"PNG");
	smile->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(smile);
}

void MainScene::gameover()
{
	// ��Ϸ�������
	_gameover = true;

	// ��ʾ���з���
	for (auto block : _blocks)
	{
		if (block->getState() != Block::State::Boom)
			block->setState(Block::State::Show);
	}

	// ��ͣ��ʱ
	endTiming();

	// Ц��������
	auto cry = gcnew Sprite(PNG_CRY, L"PNG");
	cry->setAnchor(0.5f, 0.5f);
	_smileBtn->setNormal(cry);
}

void MainScene::sweepBlock(int index)
{
	Block* block = _blocks[index];
	Block::State oldState = block->getState();

	// �����Ѿ���ʾ�ķ���
	if (oldState != Block::State::Show)
	{
		if (block->isSafeType())
		{
			// ���ص�ǰ���飨������ӱ�ǣ�
			block->setState(Block::State::Hidden);

			// չ�����з��׷���
			int row = index / _blockCol;
			int col = index % _blockCol;
			expandBlock(row, col);

			// ͳ��δ��ʾ�ķ�������
			int unshow = 0;
			for (auto block : _blocks)
			{
				if (block->getState() != Block::State::Show)
					unshow++;
			}

			// �ж���Ϸ�Ƿ�ʤ��
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

	// ��ʼ��ʱ
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
		// �����Ѿ���ʾ�򷽿����ף�������չ
		return;
	}

	// ��ʾ����
	block->setState(Block::State::Show);

	// ��չ�������ҷ���
	auto expandEmptyBlocks = [=](bool expandUnempty, int row, int col)
	{
		// ��ǰ��������Ϊ 0 ʱ����չ�������ܷ���
		// ��ǰ�������ֲ�Ϊ 0 ʱ��ֻ��չ��������Ϊ 0 �ķ���
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

	// ��ʼ��ʱ
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

	// ��ʼ��ʱ
	startTiming();
}

void MainScene::changeDifficulty()
{
	auto difficultySettingScene = gcnew Scene;

	Font font = Font(L"����", 22);
	Text::Style style = Text::Style(Color::Black);

	auto level1 = gcnew Text(L"9x9 ���ֺ� 10 ����", font, style);
	auto level2 = gcnew Text(L"16x16 ���ֺ� 25 ����", font, style);
	auto level3 = gcnew Text(L"16x16 ���ֺ� 40 ����", font, style);

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

	// ����
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
		// ��ʼ��ʱ
		Timer::add(std::bind(&MainScene::timing, this), 1.0f, -1);
	}
}

void MainScene::endTiming()
{
	if (_timingStarted)
	{
		_timingStarted = false;
		// ������ʱ
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
		_boomLeftText->setText(FormatString(L"���� %d", _boomLeft));
	}
	else
	{
		_boomLeftText->setText(L"���� 0");
	}
}

void MainScene::setTimePastSeconds(int seconds)
{
	_timePast = seconds;
	_timePastText->setText(FormatString(L"ʱ�� %d", _timePast));
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
			// ����Ҽ�������
			_mouseDoubleClick = true;
		}
		else if (_mouseDoubleClick)
		{
			_mouseDoubleClick = false;
			onClick(2);
		}
		else
		{
			// �������˷���
			onClick(0);
		}
		_userInput = 0;
	}

	if (Input::isRelease(MouseCode::Right))
	{
		if (_userInput & MOUSE_LEFT)
		{
			// ������������
			_mouseDoubleClick = true;
		}
		else if (_mouseDoubleClick)
		{
			_mouseDoubleClick = false;
			onClick(2);
		}
		else
		{
			// �Ҽ�����˷���
			onClick(1);
		}
		_userInput = 0;
	}
}
