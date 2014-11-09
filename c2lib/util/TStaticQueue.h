#pragma once

/*
	�L���[�N���X�B

	�y�����z
	�z��Ŏ������ꂽ���K�V�[�ȃL���[�\�����J�v�Z��������B
	std::queue ���g���ꍇ�Ɣ�ׁA
	���K�V�[�Ȏ����̃p�t�H�[�}���X��ύX���Ȃ����Ƃ������i�ǂ����������j�B
	�L���[�ő咷�͌Œ�l�Ƃ��Ďw�肷��K�v����B

	�y�f�[�^�\���ɂ��āz
	m_nBegin==-1, m_nEnd==-1 �̎��A�����
	m_nBegin==m_nEnd �̎��A����
*/
template <class _DATA_TYPE, int _MAX_QUEUE_SIZE>
class TStaticQueue{
public:
	//�^
	typedef _DATA_TYPE DataType;
public:
	//�萔
	static const int MAX_SIZE = _MAX_QUEUE_SIZE; //�ő�i�[��
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

	//���(m_nEnd����)�ɒǉ�
	void PushBack(const DataType& data)
	{
		if(Size()>=MAX_SIZE){
			//full
			assert(0);
			return;
		}

		//����̊i�[
		if(m_nBegin==-1){
			m_nBegin = 0;
			m_nEnd = 1;
			m_array[0] = data;
		}
		//����ȍ~�̊i�[
		else{
			//�i�[
			m_array[m_nEnd] = data;

			//�g��
			m_nEnd++;
			if(m_nEnd >= MAX_SIZE){
				m_nEnd -= MAX_SIZE;
			}
		}
	}

	//�O(m_nBegin)���폜
	void PopFront()
	{
		if(Size()<=0){
			//empty
			assert(0);
			return;
		}

		//�k��
		m_nBegin++;
		if(m_nBegin >= MAX_SIZE){
			m_nBegin -= MAX_SIZE;
		}

		//��ɂȂ����ꍇ
		if(m_nBegin == m_nEnd){
			Clear();
		}
	}

	//�O(m_nBegin)���擾
	const DataType& GetFront() const
	{
		assert(Size() > 0);
		return m_array[m_nBegin];
	}

	//�T�C�Y�m�F�̃��b�p�֐�
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
