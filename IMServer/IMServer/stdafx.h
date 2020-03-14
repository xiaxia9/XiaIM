#pragma once
#include <map>
#include <string>

#include "c2s_message.h"
#include "result_code.h"
#include "s2c_message.h"

extern std::map<int, int> g_Client;  // client 唯一, 与 userid 对照

//数据库名
std::string g_strDBName = "TestIM";
