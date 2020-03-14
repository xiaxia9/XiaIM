#ifndef STDAFX_H
#define STDAFX_H

#include "src/message/c2s_message.h"
#include "src/message/s2c_message.h"
#include "src/gate_server/gate_server.h"
#include "src/message/result_code.h"

#define STR_PATH "../IMClient/resource/pictures/images.png"
// ./images.png
static int g_socket = 0;

extern CGateServer *g_GateServer;

#endif // STDAFX_H
