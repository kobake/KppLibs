#pragma once

/*
	キュークラス。

	【特徴】
	配列で実装されたレガシーなキュー構造をカプセル化する。
	std::queue を使う場合と比べ、
	レガシーな実装のパフォーマンスを変更しないことが特徴（良くも悪くも）。
	キュー最大長は固定値として指定する必要あり。

	【データ構造について】
	m_nBegin==-1, m_nEnd==-1 の時、空っぽ
	m_nBegin==m_nEnd の時、満腹
*/
template <class _DATA_TYPE, int _MAX_QUEUE_SIZE>
class TStaticQueue{
public:
	//型
	typedef _DATA_TYPE DataType;
public:
	//定数
	static const int MAX_SIZE = _MAX_QUEUE_SIZE; //最大格納数
public:
	TStaticQueue()
	{
		Clear();
	}
	void Clear()
	{
		m_nBegin = -1;
		m_nEnd = -1;
	}
	int Size() const
	{
		if(m_nBegin==-1)return 0;

		int nEnd = m_nEnd;
		if(nEnd <= m_nBegin)nEnd += MAX_SIZE;
		return nEnd - m_nBegin;
	}

	//後ろ(m_nEnd部分)に追加
	void PushBack(const DataType& data)
	{
		if(Size()>=MAX_SIZE){
			//full
			assert(0);
			return;
		}

		//初回の格納
		if(m_nBegin==-1){
			m_nBegin = 0;
			m_nEnd = 1;
			m_array[0] = data;
		}
		//初回以降の格納
		else{
			//格納
			m_array[m_nEnd] = data;

			//拡張
			m_nEnd++;
			if(m_nEnd >= MAX_SIZE){
				m_nEnd -= MAX_SIZE;
			}
		}
	}

	//前(m_nBegin)を削除
	void PopFront()
	{
		if(Size()<=0){
			//empty
			assert(0);
			return;
		}

		//縮小
		m_nBegin++;
		if(m_nBegin >= MAX_SIZE){
			m_nBegin -= MAX_SIZE;
		}

		//空になった場合
		if(m_nBegin == m_nEnd){
			Clear();
		}
	}

	//前(m_nBegin)を取得
	const DataType& GetFront() const
	{
		assert(Size() > 0);
		return m_array[m_nBegin];
	}

	//サイズ確認のラッパ関数
	bool IsFull() const
	{
		return Size() == MAX_SIZE;
	}
	bool IsExists() const
	{
		return Size() > 0;
	}
	bool IsEmpty() const
	{
		return Size() == 0;
	}

private:
	DataType	m_array[MAX_SIZE];
	int		m_nBegin;
	int		m_nEnd;
};
