#include <iostream>
#include <string.h>

#include "stdafx.h"
#include "gate_server.h"
#include "login_server.h"

CGateServer::CGateServer()
{
	// 设置内网地址
	m_strIPAddress = "";
	m_strPort = "8888";

	// 初始化服务器地址和端口
	m_ServerAddr.sin_family = PF_INET;
	m_ServerAddr.sin_port = htons(std::stoi(m_strPort));
	m_ServerAddr.sin_addr.s_addr = inet_addr(m_strIPAddress.c_str());

	// 初始化socket
	m_uListener = 0;

	m_uEpollFd = 0;
}


CGateServer::~CGateServer()
{
}

bool CGateServer::InitServer()
{
	// 创建监听socket
	m_uListener = socket(PF_INET, SOCK_STREAM, 0);
	if (m_uListener < 0)
	{
		perror("listener");
		exit(-1);
	}

	// 绑定地址
	if (bind(m_uListener, (struct sockaddr *)&m_ServerAddr, sizeof(m_ServerAddr)) < 0)
	{
		perror("bind error");
		exit(-1);
	}

	// 监听
	int ret = listen(m_uListener, 5);
	if (ret < 0)
	{
		perror("listen error");
		exit(-1);
	}

	// 在内核中创建事件表
	m_uEpollFd = epoll_create(EPOLL_SIZE);

	if (m_uEpollFd < 0)
	{
		perror("epoll fd error");
		exit(-1);
	}

	// 往事件表里面添加监听事件
	if (!_AddFd(m_uEpollFd, m_uListener, true))
	{
		return false;
	}
	return true;
}

bool CGateServer::CloseServer()
{
	// 关闭socket
	close(m_uListener);

	// 关闭epoll监听
	close(m_uEpollFd);
	return true;
}

bool CGateServer::_AddFd(int epollfd, int fd, bool enable_et)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if (enable_et)
	{
		ev.events = EPOLLIN | EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
	return true;
}

bool CGateServer::Run()
{
	// epoll 事件队列
	static struct epoll_event events[EPOLL_SIZE];

	while (true)
	{
		// epoll_events_count表示就绪事件的数目
		int epoll_events_count = epoll_wait(m_uEpollFd, events, EPOLL_SIZE, -1);
		if (epoll_events_count < 0)
		{
			perror("epoll failure");
			break;
		}

		// 处理这epoll_events_count个就绪事件
		for (int i = 0; i < epoll_events_count; ++i)
		{
			int sockfd = events[i].data.fd;
			// 新用户连接
			if (sockfd == m_uListener)
			{
				struct sockaddr_in client_address;
				socklen_t client_addrLength = sizeof(struct sockaddr_in);
				int clientfd = accept(m_uListener, (struct sockaddr*)&client_address, &client_addrLength);
				std::cout << "client connection from: " << inet_ntoa(client_address.sin_addr) << ":"
					<< ntohs(client_address.sin_port) << ", clientfd = " << clientfd << std::endl;

				_AddFd(m_uEpollFd, clientfd, true);

				// 服务端用list保存用户连接
				m_lClientList.push_back(clientfd);
			}
			// 处理用户发来的消息
			else
			{
				RecvUserInfo(sockfd);
			}
		}

	}
	return true;
}

bool CGateServer::RecvUserInfo(int uClientFd)
{
	int message_id = c2s_message_begin;
	ssize_t size = recv(uClientFd, (void*)&message_id, sizeof(message_id), 0);

	int head_size = sizeof(message_id);
	int body_size = 0;
	char *pPackage = nullptr;

	switch (message_id)
	{
	case c2s_login:
		body_size = sizeof(C2S_LOGIN);
		pPackage = (char*)malloc(sizeof(message_id) + body_size);
		if (!_GetPackage(uClientFd, body_size, message_id, pPackage))
		{
			return false;
		}
		if (!g_LoginServer.RecvLoginInfo(uClientFd, pPackage))
		{
			return false;
		}
		break;
	default:
		break;
	}

}

bool CGateServer::_GetPackage(int uClientFd, int ubody_size, int uhead, char* pPackage)
{
	void *p = malloc(ubody_size);
	ssize_t size = recv(uClientFd, p, ubody_size, 0);

	if (-1 == size || 0 == size)
	{
		return false;
	}

	memcpy(pPackage, (void*)&uhead, sizeof(uhead));
	memcpy(pPackage + sizeof(uhead), p, ubody_size);

	return true;
}