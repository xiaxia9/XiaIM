#ifndef CGATESERVER_H
#define CGATESERVER_H

#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include <QMainWindow>
#include <QThread>

#include "src/message/common.h"

#pragma comment(lib, "ws2_32.lib")

class CGateServer: public QThread
{
    Q_OBJECT
public:
    CGateServer();
    ~CGateServer();

    // 连接服务器
    bool ConnectServer();

    // 断开连接
    bool CloseServer();

    // 发送消息至服务器
    bool SendLoginToServer(int message_id, const std::string& username, const std::string& password);

    bool _GetPackage(int uClientFd, int ubody_size, int uhead, char* pPackage);

    SOCKET GetSocket() { return m_socket; }

protected:
    void run();


signals:
    void GateServerRecvLoginAck(char* pPackage);

public slots:

private:

    // 当前连接服务器端创建的socket
    SOCKET m_socket;

    // 聊天信息缓冲区
    char m_szMessage[BUF_SIZE];

    // 用户连接的服务器 IP + port
    struct sockaddr_in m_sServerAddr;

    // 表示客户端是否正常工作
    bool m_bIsClientWork;

    WSADATA m_wsaData;
};

#endif // CGATESERVER_H

