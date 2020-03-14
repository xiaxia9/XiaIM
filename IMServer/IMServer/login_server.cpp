#include <sys/socket.h>
#include <iostream>
#include <mysql/mysql.h>
#include "stdafx.h"
#include "login_server.h"

CLoginServer::CLoginServer()
{

}


CLoginServer::~CLoginServer()
{
}

bool CLoginServer::RecvLoginInfo(int uClientFd, char* pPackage)
{
	C2S_LOGIN _login_info = *(C2S_LOGIN*)pPackage;
	std::string _user_name = _login_info._username;
	std::string _password = _login_info._password;
	int result_code = error_code_begin;

	int user_id = 0;

	// 查询数据库
	std::string sqlstr = "select * from user where Nickname = \"" + _user_name + "\";";

	std::cout << _user_name << std::endl;

	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	
	mysql_init(&myCont);

	// 参数2：外网地址；参数3：数据库用户名；参数4：数据库密码；参数5：数据库名字
	if (mysql_real_connect(&myCont, "", "", "", "", 3306, NULL, 0))
	{
		if (!mysql_query(&myCont, sqlstr.c_str()))
		{
			result = mysql_store_result(&myCont);
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))//获取具体的数据
				{
					if (sql_row != nullptr && sql_row[2] == _password)
					{
						user_id = std::stoi(sql_row[0]);
						result_code = success_login;
					}
					else
					{
						result_code = error_password;
					}
				}
			}
			else
			{
				// 未查询到相关信息
				result_code = error_username;
			}
		}
	}
	
	else
	{
		return false;
	}

	if (!SendLoginAck(uClientFd, result_code, user_id))
	{
		return false;
	}
	return true;
}

bool CLoginServer::SendLoginAck(int uClientFd, int result_code, int user_id)
{
	S2C_LOGIN_ACK _login_ack;
	_login_ack._message_id = s2c_login_ack;
	_login_ack._result_code = result_code;
	_login_ack._user_id = user_id;
	send(uClientFd, (void*)&_login_ack, sizeof(_login_ack), 0);

	std::cout << _login_ack._user_id << std::endl;
	auto search = g_Client.find(uClientFd);
	if (search == g_Client.end())
	{
		g_Client.insert(std::make_pair(uClientFd, user_id));
	}

	for (auto it : g_Client)
	{
		std::cout << "login: " << it.first << "&&" << it.second << std::endl;
	}

	return true;
}
