#ifndef S2C_MESSAGE_H
#define S2C_MESSAGE_H

#define COMMON_TEXT 64
#define FILE_TEXT 1024
#define S2C_MAX_MESSAGE_SIZE 65535

enum s2c_message
{
	s2c_message_begin = 0,
	s2c_login_ack,

	s2c_message_end = S2C_MAX_MESSAGE_SIZE,
};

struct S2C_BASE_HEAD
{
	int _message_id;
};

struct S2C_LOGIN_ACK : S2C_BASE_HEAD
{
	int _result_code;
	int _user_id;
};

#endif // S2C_MESSAGE_H
