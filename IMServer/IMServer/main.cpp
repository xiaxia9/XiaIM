#include "stdafx.h"
#include "gate_server.h"

int main()
{
	g_GateServer.InitServer();
	g_GateServer.Run();

	return 0;
}
