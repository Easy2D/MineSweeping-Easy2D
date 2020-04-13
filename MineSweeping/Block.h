#pragma once

#include <easy2d/easy2d.h>
using namespace easy2d;

const int BLOCK_SIZE = 41; // ������������سߴ�

class Block : public Sprite
{
public:
	enum class Type
	{
		Empty,		// �յ�
		Boom,		// ����
	};

	enum class State
	{
		Hidden,		// ����
		Show,		// ����ʾ
		Marked,		// �������
		Suspect,	// ��ǻ���
		Boom,		// ���е���
	};

	// ����״̬
	State getState() const;
	void setState(State state);

	// ��������
	Type getType() const;
	bool isSafeType() const;
	void setType(Type type);

	// ���ֱ��
	int getNumber() const;
	void setNumber(int num);

	void reloadImage();

private:
	Type _type = Block::Type::Empty;
	State _state = Block::State::Hidden;
	int _num = 0;
};
