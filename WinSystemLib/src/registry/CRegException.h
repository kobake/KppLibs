#pragma once

#include <string>
#include <BaseLib.h>
using namespace std;


GEN_MYEXCEPTION(CRegException);

GEN_MYEXCEPTION2(CRegException_KeyNotFound, CRegException);
GEN_MYEXCEPTION2(CRegException_BadPath, CRegException);

