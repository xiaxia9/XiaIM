#include <QMessageBox>

#include "gate_server.h"
#include "src/stdafx.h"
#include "src/login/login.h"
#include "string.h"

Q_DECLARE_METATYPE(char*)

CGateServer::CGateServer()
{
    m_bIsClientWork = false;

    // 初始化要连接的服务器地址和端口
    m_sServerAddr.sin_family = AF_INET;
    m_sServerAddr.sin_port = htons(8888);
    // 公网地址
    m_sServerAddr.sin_addr.s_addr = inet_addr("");
    //ConnectServer();
}


CGateServer::~CGateServer()
{

}

 bool CGateServer::ConnectServer()
 {
     //加载套接字
     if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
     {
          printf("Failed to load Winsock");
          return false;
     }

     //创建套接字
     m_socket = socket(AF_INET, SOCK_STREAM, 0);
     if (SOCKET_ERROR == m_socket){
         printf("Socket() error:%d", WSAGetLastError());
         return false;
     }


     //向服务器发出连接请求
     if (::connect(m_socket, (struct  sockaddr*)&m_sServerAddr, sizeof(m_sServerAddr)) == INVALID_SOCKET){
         printf("Connect failed:%d", WSAGetLastError());
         return false;
     }

     m_bIsClientWork = true;
     g_socket = m_socket;
     return true;
 }

 // 断开连接
 bool CGateServer::CloseServer()
 {
     //关闭套接字
     closesocket(m_socket);
     WSACleanup();
     system("pause");
     return true;
 }

 // 发送消息至服务器
 bool CGateServer::SendLoginToServer(int message_id, const std::string& username, const std::string& password)
 {
     C2S_LOGIN login_info;
     strcpy(login_info._username, username.data());
     strcpy(login_info._password, password.data());

     login_info._message_id = message_id;
     int result_size = send(g_socket, (char*)&login_info, sizeof(login_info) + 1, 0);
     if (result_size <= 0)
     {
         return false;
     }
     printf("**************%s",login_info._username);
     return true;
 }

void CGateServer::run()
 {
     ConnectServer();
     while(true)
     {
         int message_id = s2c_message_begin;
         int size = recv(g_socket, (char*)&message_id, sizeof(message_id), 0);
         if (size > 0)
         {
             int head_size = sizeof(message_id);
             int body_size = 0;
             char *pPackage = nullptr;
             int i = 0;
             switch (message_id)
             {
             case s2c_login_ack:
                body_size = sizeof(S2C_LOGIN_ACK) - head_size;
                pPackage = (char*)malloc(body_size + head_size);
                if (!_GetPackage(g_socket, body_size, message_id, pPackage))
                {
                   break;
                }
                emit GateServerRecvLoginAck(pPackage);
                break;
             default:
                break;
             }
             Sleep(50);
         }
         Sleep(100);
     }
 }

 bool CGateServer::_GetPackage(int uClientFd, int ubody_size, int uhead, char* pPackage)
 {
     void *p = malloc(ubody_size);
     int size = recv(uClientFd, (char*)p, ubody_size, 0);

     if (-1 == size || 0 == size)
     {
         return false;
     }

     memcpy(pPackage, (void*)&uhead, sizeof(uhead));
     memcpy(pPackage + sizeof(uhead), p, ubody_size);

     return true;
 }
