#pragma once

#include <exception>

class SingleCursor{
public:
	//コンストラクタ
	SingleCursor();
	SingleCursor(int _range_min,int _range_max,bool _loop);
	virtual ~SingleCursor();
	//動作
	void setRange(int _range_min,int _range_max);
	void setLoop(bool _loop);
	void setPos(int _pos);
	void move(int offset);
	int getPos(){ return pos; }
	//ラッパー
	void setRange(int _range_min,int _range_max,bool _loop);
private:
	int pos;
	int range_min;
	int range_max;
	bool loop;
public:
	//例外
	class invalid_range : public std::exception{
	public:
		const char* what() const{ return "SingleCursor::invalid_exception"; }
	};
	class invalid_pos : public std::exception{
	public:
		const char* what() const{ return "SingleCursor::invalid_pos"; }
	};
};
