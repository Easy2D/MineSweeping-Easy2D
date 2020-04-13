#pragma once

#include <easy2d/easy2d.h>
using namespace easy2d;

const int BLOCK_SIZE = 41; // 单个方块的像素尺寸

class Block : public Sprite
{
public:
	enum class Type
	{
		Empty,		// 空地
		Boom,		// 地雷
	};

	enum class State
	{
		Hidden,		// 隐藏
		Show,		// 已显示
		Marked,		// 标记旗子
		Suspect,	// 标记怀疑
		Boom,		// 踩中地雷
	};

	// 方块状态
	State getState() const;
	void setState(State state);

	// 方块类型
	Type getType() const;
	bool isSafeType() const;
	void setType(Type type);

	// 数字标记
	int getNumber() const;
	void setNumber(int num);

	void reloadImage();

private:
	Type _type = Block::Type::Empty;
	State _state = Block::State::Hidden;
	int _num = 0;
};
