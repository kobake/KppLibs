#pragma once

#include <DebugLib_Assert.h>

//!サイズ自由のビットセット
template <size_t BITS>
class BitSet{
public:
	typedef BitSet<BITS> Me;
public:
	static const size_t BYTES=(BITS+7)/8;
public:
	//!初期状態ではすべてのビットが0です
	BitSet()
	{
		clear_all();
	}

	//!全ビットをクリアする
	void clear_all()
	{
		for(size_t i=0;i<BYTES;i++)data[i]=0;
	}

	//!指定ビットを取得する
	bool get(size_t index) const
	{
		assert(index<BITS);
		return ( data[index/8] & (1<<(index%8)) )!=0;
	}

	//!指定ビットを設定する
	void set(size_t index,bool value)
	{
		if(value)
			store(index);
		else
			clear(index);
	}

	//!指定ビットを1にする
	void store(size_t index)
	{
		assert(index<BITS);
		data[index/8] |= (1<<(index%8));
	}

	//!指定ビットを0にする
	void clear(size_t index)
	{
		assert(index<BITS);
		data[index/8] &= ~(1<<(index%8));
	}

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	// 判定
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	// 1ビットでも一致してたらtrue //
	bool And(const Me& rhs) const
	{
		for(int i = 0; i < BYTES; i++){
			if(data[i] & rhs.data[i])return true;
		}
		return false;
	}

private:
	unsigned char data[BYTES];
};
