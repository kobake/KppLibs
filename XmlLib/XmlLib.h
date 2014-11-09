#pragma once

#pragma comment(lib,"shlwapi.lib")

// microsoft
#include "src/XmlDocument.h"
#include "src/XmlElement.h"
#include "src/XmlElementList.h"

// libxml2
#include "src/CXmlDocument.h"
#include "src/CXmlNode.h"

#pragma comment(lib,"comsupp.lib")
#pragma comment(lib,"comsuppw.lib") //自宅でリンクエラーになったが、これを追加したら解決。
