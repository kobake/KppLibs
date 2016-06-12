#include <BaseLib.h>
#include <DebugLib.h>
#include "CMemoryStream.h"
#include <string>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      CMemoryInputStream                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CMemoryInputStream::CMemoryInputStream(const void* p,uint len)
{
	m_pData=(const char*)p;
	m_nDataLen=len;
	m_pCur=m_pData;
}

CMemoryInputStream::CMemoryInputStream(Memory mem)
{
	m_pData=(const char*)mem.data;
	m_nDataLen=mem.length;
	m_pCur=m_pData;
}

bool CMemoryInputStream::good() const
{
	return m_pData!=NULL;
}

bool CMemoryInputStream::Eof() const
{
	return m_pCur-m_pData>=(int)m_nDataLen;
}

int CMemoryInputStream::Read(void* p,int size)
{
	//size§ŒÀ (ƒf[ƒ^”ÍˆÍ‚ğ’´‚¦‚È‚¢‚æ‚¤‚É’²®)
	{
		int rest = (int)size - (int)(m_pCur-m_pData);
		if(rest<0)rest=0;
		if((int)size>rest){
			size=rest;
		}
	}
	if(size>0){
		assert(m_pCur-m_pData+size<=(int)m_nDataLen);
		memcpy(p,m_pCur,size);
		m_pCur+=size;
	}
	return size;
}

void CMemoryInputStream::peek(void* p,int size)
{
	assert(m_pCur-m_pData+size<=(int)m_nDataLen);
	memcpy(p,m_pCur,size);
}

void CMemoryInputStream::skip(uint n)
{
	assert(m_pCur-m_pData+n<=m_nDataLen);
	m_pCur+=n;
}

void CMemoryInputStream::seek(int n,SeekMode mode)
{
	if(mode==ESEEK_CUR){
		m_pCur+=n;
	}else if(mode==ESEEK_BEGIN){
		m_pCur=m_pData+n;
	}else if(mode==ESEEK_END){
		m_pCur=m_pData+m_nDataLen+n;
	}
	assert(m_pCur>=m_pData && m_pCur<(m_pData+m_nDataLen));
}

uint CMemoryInputStream::tell() const
{
	return (uint)(m_pCur-m_pData);
}

uint CMemoryInputStream::Available() const
{
	return (uint)(m_pData+m_nDataLen-m_pCur);
}

void CMemoryInputStream::Close()
{
	m_pData=NULL;
	m_nDataLen=0;
	m_pCur=NULL;
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      CMemoryOutputStream                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CMemoryOutputStream::CMemoryOutputStream(void* p,uint len)
{
	m_pData=(char*)p;
	m_nDataLen=len;
	m_pCur=m_pData;
}

CMemoryOutputStream::CMemoryOutputStream(Memory mem)
{
	m_pData=(char*)mem.data;
	m_nDataLen=mem.length;
	m_pCur=m_pData;
}

bool CMemoryOutputStream::good() const
{
	return m_pData!=NULL;
}

int CMemoryOutputStream::Write(const void* p,int size)
{
	assert(m_pCur-m_pData+size<=(int)m_nDataLen);
	memcpy(m_pCur,p,size);
	m_pCur+=size;
	return size;
}

void CMemoryOutputStream::skip(uint n)
{
	assert(m_pCur-m_pData+n<=m_nDataLen);
	m_pCur+=n;
}

void CMemoryOutputStream::seek(int n,SeekMode mode)
{
	if(mode==ESEEK_CUR){
		assert(m_pCur-m_pData+n<=(int)m_nDataLen);
		m_pCur+=n;
	}else if(mode==ESEEK_BEGIN){
		assert(0); //###–¢À‘•
	}else if(mode==ESEEK_END){
		assert(0); //###–¢À‘•
	}
}

uint CMemoryOutputStream::tell() const
{
	return (uint)(m_pCur-m_pData);
}

void CMemoryOutputStream::Close()
{
	m_pData=NULL;
	m_nDataLen=0;
	m_pCur=NULL;
}
