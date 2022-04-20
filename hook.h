#pragma once
#include "structure.h"

namespace pkodev
{
	namespace hook
	{
		// void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
		void __fastcall ToClient__CM_LOGIN(void* This, void* NotUsed, void* datasock, void* recvbuf);
	}
}