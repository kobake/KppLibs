#pragma once

#include <exception>

class SingleCursor{
public:
	//�R���X�g���N�^
	SingleCursor();
	SingleCursor(int _range_min,int _range_max,bool _loop);
	virtual ~SingleCursor();
	//����
	void setRange(int _range_min,int _range_max);
	void setLoop(bool _loop);
	void setPos(int _pos);
	void move(int offset);
	int getPos(){ return pos; }
	//���b�p�[
	void setRange(int _range_min,int _range_max,bool _loop);
private:
	int pos;
	int range_min;
	int range_max;
	bool loop;
public:
	//��O
	class invalid_range : public std::exception{
	public:
		const char* what() const{ return "SingleCursor::invalid_exception"; }
	};
	class invalid_pos : public std::exception{
	public:
		const char* what() const{ return "SingleCursor::invalid_pos"; }
	};
};
