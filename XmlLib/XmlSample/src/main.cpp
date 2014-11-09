#include <c2lib.h>
#include <XmlLib.h>
#include <string>

bool test( std::string filename );


int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	printf("hello\n");
	test("test.xml");

	return 0;
}

class CXmlNode;

bool test( std::string filename )
{
	// ドキュメント構築 //
	CXmlDocument doc;
	{
		// ファイルから構築するパターン //
		if(0){
			doc.LoadFromFile(filename.c_str());
		}
		// 文字列から構築するパターン //
		else{
			FILE* fp = fopen(filename.c_str(), "rb");
			if(!fp)return false;
			fseek(fp, 0, SEEK_END);
			std::vector<char> buf(ftell(fp) + 1);
			fseek(fp, 0, SEEK_SET);
			fread(&buf[0], 1, buf.size() - 1, fp);
			buf[buf.size() - 1] = 0;
			fclose(fp);
			doc.LoadFromString(&buf[0]);
		}
	}
	if(!doc.IsValid())return false;
 
	// statusノード //
	CXmlNode nodeRoot = doc.GetRootElement();
	if(!nodeRoot.IsValid() || strcmp(nodeRoot.GetName(), "status") != 0)return false;

	// targetノード //
	CXmlNode nodeTarget = nodeRoot.GetChild(0);
	if(!nodeTarget.IsValid() || strcmp(nodeTarget.GetName(), "target") != 0)return false;

	// entryノード群 //
	for(int i = 0; i < nodeTarget.GetChildCount(); i++){
		CXmlNode nodeEntry = nodeTarget.GetChild(i);
		if(!nodeEntry.IsValid() || strcmp(nodeEntry.GetName(), "entry") != 0)continue; // entryノードのみ //
		// ファイルパス //
		printf("Path: %ls\n", nodeEntry.GetProperty("path").c_str());
		// 状態 //
		CXmlNode nodeStatus = nodeEntry.FindFirstChild("wc-status");
		printf("Status: %ls\n", nodeStatus.GetProperty("item").c_str());
		printf("Revision: %ls\n", nodeStatus.GetProperty("revision").c_str());
		// リビジョン情報 //
		CXmlNode nodeCommit = nodeStatus.FindFirstChild("commit");
		CXmlNode nodeAuthor = nodeCommit.FindFirstChild("author");
		printf("Author: %ls\n", nodeAuthor.GetText().c_str());
		printf("\n");
	}
//	if (xmlStrcmp(cur->name, (const xmlChar *) "rss")) {
//		fprintf(stderr,"document of the wrong type, root node != rss");
//		xmlFreeDoc(doc);
//		return 0;
//	}
 
//	std::string hoge = cur.GetProperty("target");

//	printf("children:%d\n", cur.GetChildCount());
//	CXmlNode child = cur.GetChild(0);
//	CXmlNode child1 = cur.GetChild(0);
//	CXmlNode child2 = cur.GetChild(0);

//	xmlChar *version = xmlGetProp(cur, (const xmlChar *)"version");
//	printf("version: %s\n", version);   // "2.0"
//	xmlFree(version);
   
	return 1;
}
