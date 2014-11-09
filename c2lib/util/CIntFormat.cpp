#include "common/_required.h"
#include "CIntFormat.h"
#include <vector>
#include <StringLib.h>
using namespace std;

//__int64�̍ő�l�F9,223,372,036,854,775,808 = 9 EBytes (�R���}�����25���A�R���}���ꂸ��19��)
//K(�L��)��M(���K)��G(�M�K)��T(�e��)��P(�y�^)��E(�G�N�T)

//�i�}�̒l���R���}��؂�ŏo�� (��: 123,456 Bytes)
std::wstring CIntFormat::PlainBytes() const
{
	//�߂�l
	wchar_t ret[64] = L"";
	wchar_t* p = ret;

	//���[�N�ϐ�
	__int64 n = m_nValue;

	//�}�C�i�X�L��
	if(n<0){
		*p++ = L'-';
		n*=-1;
	}

	//1000���ɕ������� -> buf
	int buf[32];
	int i=-1;
	do{
		i++;
		buf[i] = (int)(n % 1000);
		n/=1000;
	}
	while(n!=0);
	int end = i;

	//�R���}��؂�Ō�������
	for(i=end;i>=0;i--){
		if(i==end){
			p += swprintf(p, ret+64-p, L"%d", buf[i]);
		}
		else{
			p += swprintf(p, ret+64-p, L",%03d", buf[i]);
		}
	}
	wcscpy(p, L" Bytes");
	return ret;
}

//KB, MB, GB ���̒P�ʂɕϊ����ďo�� (��: 123.46 KBytes)
std::wstring CIntFormat::EasyBytes() const
{
	//�߂�l
	wchar_t buf[64] = L"";
	wchar_t* p = buf;

	//���[�N�ϐ�
	__int64 n = m_nValue;

	//�}�C�i�X�L��
	if(n<0){
		*p++ = L'-';
		n*=-1;
	}

	if(0){
	}
	else if(n<1024){
		swprintf_s(p, _countof(buf)-1, L"%d Bytes", n);
	}
	else if(n<1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f KB", (double)n/1024);
	}
	else if(n<1024*1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f MB", (double)n/1024/1024);
	}
	else if(n<1024LL*1024*1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f GB", (double)n/1024/1024/1024);
	}
	else if(n<1024LL*1024*1024*1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f TB", (double)n/1024/1024/1024/1024);
	}
	else if(n<1024LL*1024*1024*1024*1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f PB", (double)n/1024/1024/1024/1024/1024);
	}
	else{// if(n<1024*1024*1024*1024*1024*1024*1024){
		swprintf_s(p, _countof(buf)-1, L"%.2f EB", (double)n/1024/1024/1024/1024/1024/1024);
	}
	return buf;
}

//�J�����ɕ\�����镗
std::wstring CIntFormat::EasyBytes_LikeColumn() const
{
	// -- -- ����� KB �\�� (�����_�����A�R���}��؂�) -- -- //

	// KB�P�ʂɂ���
	__int64 n = m_nValue / 1024;
	if(n==0 && m_nValue!=0){
		if(m_nValue>0){
			n=1;
		}
		else{
			n=-1;
		}
	}

	// �R���}��؂�
	wchar_t buf[128];
	itow_comma(buf, n, 3);

	// �P��
	wcscat(buf, L" KB");

	// ����
	return buf;
}

//n��L������3���ɑ����ďo��
static void _MakeEasy(wchar_t* buf, double n, const wchar_t* unit)
{
	swprintf(buf, L"%.3g", n);
	int len = (int)wcslen(buf);
	if(wcschr(buf,L'.')==NULL){
		if(len==1){
			//X �� X.00
			wcscat(buf,L".00");
		}
		else if(len==2){
			//XX �� XX.0
			wcscat(buf,L".0");
		}
	}
	else{
		if(wcslen(buf)==3){
			//X.X �� X.X0
			wcscat(buf,L"0");
		}
	}
	wcscat(buf, L" ");
	wcscat(buf, unit);
}

//�X�e�[�^�X�o�[��
std::wstring CIntFormat::EasyBytes_LikeStatusBar() const
{
	// -- -- �L������:3�� �����_:�L�� �P��:�C��{bytes, KB, MB, GB, TB, PT, EB} -- -- //
	//0.00
	//00.0
	//000
	wchar_t buf[64];
	__int64 n = m_nValue;
	__int64 a = _abs64(n);
	if(a<1024){
		swprintf_s(buf, _countof(buf), L"%I64d bytes", n);
	}
	else if(a<1024*1024){
		_MakeEasy(buf, (double)n/1024, L"KB");
	}
	else if(a<1024*1024*1024){
		_MakeEasy(buf, (double)n/1024/1024, L"MB");
	}
	else if(a<1024LL*1024*1024*1024){
		_MakeEasy(buf, (double)n/1024/1024/1024, L"GB");
	}
	else if(a<1024LL*1024*1024*1024*1024){
		_MakeEasy(buf, (double)n/1024/1024/1024/1024, L"TB");
	}
	else if(a<1024LL*1024*1024*1024*1024*1024){
		_MakeEasy(buf, (double)n/1024/1024/1024/1024/1024, L"PB");
	}
	else{// if(a<1024*1024*1024*1024*1024*1024*1024){
		_MakeEasy(buf, (double)n/1024/1024/1024/1024/1024/1024, L"EB");
	}
	return buf;
}
