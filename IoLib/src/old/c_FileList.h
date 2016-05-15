#if 0

#pragma once

#include "t_Chain.h"
#include "c_FileDetail.h"

class FileListElement : public FileName, public ChainElement<FileListElement>{
protected:
//	int compare(FileListElement *data2){}
public:
	FileListElement(FileName *_filename,FileListElement *_last,FileListElement *_next)
	: FileName(*_filename), ChainElement<FileListElement>(_last,_next)
	{
	}
};

class FileList : public ChainElementList<FileListElement,FileName>{
};


#endif
