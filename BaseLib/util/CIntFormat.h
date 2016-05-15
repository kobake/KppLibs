#pragma once

#include <string>

class CIntFormat{
public:
	CIntFormat(__int64 nValue) : m_nValue(nValue) { }
	std::wstring PlainBytes() const; //�i�}�̒l���R���}��؂�ŏo�� (��: 123,456 Bytes)
	std::wstring EasyBytes() const;  //KB, MB, GB ���̒P�ʂɕϊ����ďo�� (��: 123.46 KBytes)
	std::wstring EasyBytes_LikeColumn() const; //�J�����ɕ\�����镗
	std::wstring EasyBytes_LikeStatusBar() const; //�X�e�[�^�X�o�[��
private:
	__int64	m_nValue;
};
