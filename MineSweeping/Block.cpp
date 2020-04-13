#include "Block.h"
#include "resource.h"
#include <cassert>

Block::State Block::getState() const
{
	return _state;
}

void Block::setState(State state)
{
	_state = state;
	reloadImage();
}

Block::Type Block::getType() const
{
	return _type;
}

void Block::setType(Type type)
{
	_type = type;
}

int Block::getNumber() const
{
	return _num;
}

void Block::setNumber(int num)
{
	assert(num < 9);
	_num = num;
}

bool Block::isSafeType() const
{
	return _type != Block::Type::Boom;
}

void Block::reloadImage()
{
	if (_state == Block::State::Hidden)
	{
		this->open(PNG_HIDDEN, L"PNG");
	}
	else if (_state == Block::State::Show)
	{
		if (!isSafeType())
		{
			this->open(PNG_BOOM, L"PNG");
		}
		else
		{
			switch (getNumber())
			{
			case 0:
				this->open(PNG_0, L"PNG");
				break;
			case 1:
				this->open(PNG_1, L"PNG");
				break;
			case 2:
				this->open(PNG_2, L"PNG");
				break;
			case 3:
				this->open(PNG_3, L"PNG");
				break;
			case 4:
				this->open(PNG_4, L"PNG");
				break;
			case 5:
				this->open(PNG_5, L"PNG");
				break;
			case 6:
				this->open(PNG_6, L"PNG");
				break;
			case 7:
				this->open(PNG_7, L"PNG");
				break;
			case 8:
				this->open(PNG_8, L"PNG");
				break;
			}
		}
	}
	else if (_state == Block::State::Suspect)
	{
		this->open(PNG_SUSPECT, L"PNG");
	}
	else if (_state == Block::State::Marked)
	{
		this->open(PNG_FLAG, L"PNG");
	}
	else if (_state == Block::State::Boom)
	{
		this->open(PNG_RED_BOOM, L"PNG");
	}
}
