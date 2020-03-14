#ifndef C2S_MESSAGE_H
#define C2S_MESSAGE_H

#include <string>

#define COMMON_TEXT 64
#define FILE_TEXT 1024
#define C2S_MAX_MESSAGE_SIZE 65535

enum c2s_message
{
	c2s_message_begin = 0,
	c2s_login,                       //µÇÂ¼

	c2s_message_end = C2S_MAX_MESSAGE_SIZE,
};

struct C2S_BASE_HEAD
{
	int _message_id;
};

struct C2S_LOGIN : C2S_BASE_HEAD
{
	char _username[COMMON_TEXT];
	char _password[COMMON_TEXT];
};

#endif // C2S_MESSAGE_H
