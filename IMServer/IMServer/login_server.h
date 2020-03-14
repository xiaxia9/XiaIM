#pragma once

class CLoginServer
{
public:
	CLoginServer();
	~CLoginServer();

public:
	bool RecvLoginInfo(int uClientFd, char* pPackage);

	bool SendLoginAck(int uClientFd, int result_code, int user_id);

private:

};

static CLoginServer g_LoginServer;
