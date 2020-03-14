#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <list>
#include <string>

#define EPOLL_SIZE 5000

class CGateServer
{
public:
	CGateServer();
	~CGateServer();
	bool InitServer();
	bool CloseServer();

	bool Run();

	//接收用户发来的消息
	bool RecvUserInfo(int uClientFd);

private:
	bool _AddFd(int epollfd, int fd, bool enable_et);

	bool _GetPackage(int uClientFd, int ubody_size, int uhead, char* pPackage);

private:
	struct sockaddr_in m_ServerAddr;

	// 监听socket
	int m_uListener;

	// epoll_create创建后的返回值
	int m_uEpollFd;

	std::string m_strIPAddress;
	std::string m_strPort;

	// 客户端列表
	std::list<int> m_lClientList;
};

static CGateServer g_GateServer;
