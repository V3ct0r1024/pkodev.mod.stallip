#include <ws2tcpip.h>
#include <Windows.h>

#include <iostream>
#include <string>

#include "loader.h"

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"

// Patch 1 backup bytes
char call_patch_backup[sizeof(pkodev::asm_call)]{ 0x00 };

// Patch 2 backup bytes
char push_patch_backup[sizeof(pkodev::asm_push)]{ 0x00 };


// Patch GateServer.exe
void patch(unsigned int address, const char* bytes, unsigned int length, char* backup);

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Make changes
    {
        // Hook for void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
        {
            // Opcode and offset
            pkodev::asm_call call_patch(
                reinterpret_cast<unsigned int>(&pkodev::hook::ToClient__CM_LOGIN)
                - pkodev::address::MOD_EXE_VERSION::call_patch - 0x05
            );

            // Set hook
            patch(
                pkodev::address::MOD_EXE_VERSION::call_patch,
                reinterpret_cast<char*>(&call_patch),
                sizeof(call_patch),
                call_patch_backup
            );
        }

        // Remove 4 bytes of client IP address from login packet
        {
            // Bytes
            pkodev::asm_push push_patch(6);

            // Patch
            patch(
                pkodev::address::MOD_EXE_VERSION::push_patch,
                reinterpret_cast<char*>(&push_patch),
                sizeof(push_patch),
                push_patch_backup
            );
        }
    }
}

// Stop the mod
void Stop()
{
    // Rollback changes
    {
        // Hook for void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
        patch(
            pkodev::address::MOD_EXE_VERSION::call_patch,
            call_patch_backup,
            sizeof(call_patch_backup),
            nullptr
        );

        // Remove 4 bytes of client IP address from login packet
        patch(
            pkodev::address::MOD_EXE_VERSION::push_patch,
            push_patch_backup,
            sizeof(push_patch_backup),
            nullptr
        );
    }
}

// void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
void __fastcall pkodev::hook::ToClient__CM_LOGIN(void* This, void* NotUsed,
    void* datasock, void* recvbuf)
{
    // Buffer for IP address
    char ip_address[16]{ 0x00 };

    // Get RPacket::m_revrpos pointer
    unsigned int * const m_revrpos = reinterpret_cast<unsigned int*>(
        reinterpret_cast<unsigned int>(This) + 0x38
    );

    // Set read position + 4
    *m_revrpos += 4;

    // Read 4 bytes (IP address)
    const unsigned int raw = pkodev::pointer::RPacket__ReverseReadLong(recvbuf);

    // Roll back the read position
    *m_revrpos -= 8;

    // Convert the IP address to the string form
    if (inet_ntop(AF_INET, &raw, ip_address, sizeof(ip_address)) == nullptr)
    {
        // Incorrect IP address
        strncpy_s(ip_address, sizeof(ip_address), "0.0.0.0", _TRUNCATE);
    }

    // Write the IP address to datasocket
    strncpy_s(reinterpret_cast<char *>(datasock) + 0xB4, 16, ip_address, _TRUNCATE);

    // Debug
    std::cout << "ip: " << ip_address << std::endl;

    // Call the original method void ToClient::CM_LOGIN(DataSocket* datasock, RPacket& recvbuf)
    pkodev::pointer::ToClient__CM_LOGIN(This, datasock, recvbuf);
}

// Patch GateServer.exe
void patch(unsigned int address, const char* bytes, unsigned int length, char* backup)
{
    // Access type
    DWORD access = PAGE_EXECUTE_READWRITE;

    // Change the access protection of the proccess
    VirtualProtect(reinterpret_cast<LPVOID>(address), length, access, &access);

    // Save backup bytes
    if (backup != nullptr)
    {
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(address),
            reinterpret_cast<LPVOID>(backup), length, nullptr);
    }

    // Write patch bytes
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(address),
        reinterpret_cast<LPCVOID>(bytes), length, nullptr);

    // Rollback the access protection of the proccess
    VirtualProtect(reinterpret_cast<LPVOID>(address), length, access, &access);
}