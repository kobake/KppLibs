#include "include_sys.h"
#include "c_JavaFileStream.h"
#include "f_java.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- --  [class] JavaFileStream -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

JavaFileStream::JavaFileStream(FILE *_fp) : FileStream(_fp)
{
}
JavaFileStream::JavaFileStream() : FileStream()
{
}
JavaFileStream::JavaFileStream(JavaFileStream &left) : FileStream(left)
{
}
JavaFileStream::~JavaFileStream()
{
}
void JavaFileStream::writeInt(int n)
{
	java_writeInt(fp,n);
}
int JavaFileStream::readInt()
{
	return java_readInt(fp);
}
void JavaFileStream::writeByte(int n)
{
	putc(n,fp);
}
int JavaFileStream::readByte()
{
	return getc(fp);
}

