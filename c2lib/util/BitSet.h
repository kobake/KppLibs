#pragma once

#include <DebugLib_Assert.h>

//!�T�C�Y���R�̃r�b�g�Z�b�g
template <size_t BITS>
class BitSet{
public:
	typedef BitSet<BITS> Me;
public:
	static const size_t BYTES=(BITS+7)/8;
public:
	//!������Ԃł͂��ׂẴr�b�g��0�ł�
	BitSet()
	{
		clear_all();
	}

	//!�S�r�b�g���N���A����
	void clear_all()
	{
		for(size_t i=0;i<BYTES;i++)data[i]=0;
	}

	//!�w��r�b�g���擾����
	bool get(size_t index) const
	{
		assert(index<BITS);
		return ( data[index/8] & (1<<(index%8)) )!=0;
	}

	//!�w��r�b�g��ݒ肷��
	void set(size_t index,bool value)
	{
		if(value)
			store(index);
		else
			clear(index);
	}

	//!�w��r�b�g��1�ɂ���
	void store(size_t index)
	{
		assert(index<BITS);
		data[index/8] |= (1<<(index%8));
	}

	//!�w��r�b�g��0�ɂ���
	void clear(size_t index)
	{
		assert(index<BITS);
		data[index/8] &= ~(1<<(index%8));
	}

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	// ����
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	// 1�r�b�g�ł���v���Ă���true //
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
