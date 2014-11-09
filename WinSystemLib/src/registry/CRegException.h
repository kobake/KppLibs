#pragma once

#include <string>
#include "std/myexception.h"
using namespace std;


GEN_MYEXCEPTION(CRegException);

GEN_MYEXCEPTION2(CRegException_KeyNotFound, CRegException);
GEN_MYEXCEPTION2(CRegException_BadPath, CRegException);

