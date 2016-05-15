#pragma once

class Internet{
protected:
	wchar agent[64];
	HINTERNET hInternet;
public:
	Internet();
	virtual ~Internet();
	bool internetConnect();
	bool internetDisconnect();
	bool internetIsConnect();
	void internetSetAgent(wchar *_agent);
};

