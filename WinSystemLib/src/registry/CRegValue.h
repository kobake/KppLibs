#pragma once

#include <vector>

class CRegType;

class CRegValue{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRegValue();
	CRegValue(const wchar_t* source,const CRegType& _type);
	CRegValue(const void* _data,int _ndata,const CRegType& _type);

	//���
	bool IsValid() const{ return m_dwType != REG_NONE; }

	//����
	DWORD getType() const{ return m_dwType; }
	const void* getData() const{ return &m_vData[0]; }
	size_t GetSize() const{ return m_vData.size(); }

	//���
	std::wstring ToString() const;

private:
	DWORD				m_dwType;
	std::vector<byte>	m_vData;
};
