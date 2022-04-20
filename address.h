#pragma once

namespace pkodev
{
	namespace address
	{
        // GateServer.exe 1.38
        namespace GATESERVER_138
        {
            // void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
            const unsigned int ToClient__CM_LOGIN = 0x00409BC0;

            // Call address of void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
            const unsigned int call_patch = 0x0040D3FD;

            // Patch 2 address
            const unsigned int push_patch = 0x00409D0C;

            // uShort RPacket::ReverseReadLong()
            const unsigned int RPacket__ReverseReadLong = 0x0041D6A0;
        }
	}
}