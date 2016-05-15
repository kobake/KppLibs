class JavaFileStream;

#ifndef _JavaFileStream_
#define _JavaFileStream_

#include "c_FileStream.h"

class JavaFileStream : public FileStream{
public:
	JavaFileStream(FILE *_fp);
	JavaFileStream();
	JavaFileStream(JavaFileStream &left);
	virtual ~JavaFileStream();
	void writeInt(int n);
	int readInt();
	void writeByte(int n);
	int readByte();
};

#endif
