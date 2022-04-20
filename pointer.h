#pragma once
#include "address.h"
#include "structure.h"

namespace pkodev
{
	namespace pointer
	{
		// void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
		typedef void(__thiscall* ToClient__CM_LOGIN__Ptr)(void*, void*, void*);
		ToClient__CM_LOGIN__Ptr ToClient__CM_LOGIN = (ToClient__CM_LOGIN__Ptr)(void*)(address::MOD_EXE_VERSION::ToClient__CM_LOGIN);

		// uShort RPacket::ReverseReadLong()
		typedef unsigned int(__thiscall* RPacket__ReverseReadLong__Ptr)(void*);
		RPacket__ReverseReadLong__Ptr RPacket__ReverseReadLong = (RPacket__ReverseReadLong__Ptr)(void*)(address::MOD_EXE_VERSION::RPacket__ReverseReadLong);
	}
}