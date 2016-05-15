#pragma once

class FileStream{
protected:
	bool to_close;
	FILE *fp;
	wchar *tmpname;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FileStream(FILE *_fp);
	FileStream();
	FileStream(FileStream &left);
	virtual ~FileStream();
	//�I�[�v���E�N���[�Y
	bool open(const wchar *fpath,const wchar *mode);
	void close();
	bool tmp_open(const wchar *mode);
	bool tmp_close(const wchar *replacefile=NULL);
	//���o��(int,byte)
	void writeInt(int n);
	int readInt();
	void writeByte(int n);
	int readByte();
	void writeShort(short n);
	short readShort();
	//array
	int read(void *buf,int size);
	int write(const void *buf,int size);
	wchar *readTextFormatAlloc();
	bool writeTextFormat(const wchar *text);
	wchar *readAllAlloc(long *size=NULL);
	//�V�[�N
	void seekSet(long n);
	void seekCur(long n);
	void seekEnd(long n);
	long getLength();
	//
	template <class T> void ex_write(const T& t)
	{
		t.write(this);
	}
	template <class T> void ex_read(T& t)
	{
		t.read(this);
	}
	//ex_io
	void ex_write(const int& t);
	void ex_read(int& t);
	void ex_write(const wstring& t);
	void ex_read(wstring& t);
};



