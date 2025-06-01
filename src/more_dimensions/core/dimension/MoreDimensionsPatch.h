#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <vector>

extern "C" {
__declspec(dllimport) bool pl_notice(void* target);
}
namespace {
struct AddressAndReg {
    DWORD                   func_rva_start;
    DWORD                   func_rva_end;
    const std::vector<BYTE> native_code;
};

// 0~26 VanillaDimensions::toSerializedInt
// 27~39 VanillaDimensions::fromSerializedInt(int)
std::vector<AddressAndReg> data = {
    {0x07BB92F, 0x07BB95A, {0x89, 0xC8}                        },
    {0x292609B, 0x29260BF, {0x89, 0xC3}                        },
    {0x1162597, 0x11625C0, {0x89, 0xC2}                        },
    {0x117148E, 0x11714B7, {0x89, 0xC2}                        },
    {0x11789E0, 0x1178A09, {0x89, 0xC2}                        },
    {0x11798C5, 0x11798EE, {0x89, 0xC2}                        },
    {0x127D792, 0x127D7BB, {0x89, 0xC3}                        },
    {0x129BBB3, 0x129BBD6, {0x89, 0xC3}                        },
    {0x12A9A9F, 0x12A9AC8, {0x89, 0xC2}                        },
    {0x12C29A4, 0x12C29CD, {0x89, 0xC2}                        },
    {0x1C17D7B, 0x1C17DA3, {0x41, 0x88, 0xC9}                  },
    {0x1C1803B, 0x1C1805E, {0x88, 0xC8}                        },
    {0x1FE4127, 0x1FE414F, {0x41, 0x88, 0xC9}                  },
    {0x29FC7E7, 0x29FC812, {0x89, 0xC3}                        },
    {0x2A596EC, 0x2A59717, {0x89, 0xC3}                        },
    {0x2A59B95, 0x2A59BC1, {0x89, 0xDF}                        },
    {0x2A5694F, 0x2A56975, {0x41, 0x89, 0xC6}                  },
    {0x2A71DC0, 0x2A71DE9, {0x89, 0xC3}                        },
    {0x2A71530, 0x2A7155B, {0x89, 0xD3}                        },
    {0x2D960E2, 0x2D96110, {0x41, 0x89, 0xC6}                  },
    {0x2D99030, 0x2D99054, {0x89, 0xC7}                        },
    {0x2DA043E, 0x2DA0461, {0x88, 0xD8}                        },
    {0x2CF8393, 0x2CF83B6, {0x89, 0xC3}                        },
    {0x33C3822, 0x33C3850, {0x41, 0x89, 0xC3}                  },
    {0x347BE35, 0x347BE59, {0x89, 0xCB}                        },
    {0x347CE8E, 0x347CEB3, {0x89, 0xDF}                        },
    {0x3616FAD, 0x3616FD5, {0x40, 0x88, 0xC7}                  },
    {0x29FF291, 0x29FF2C1, {0x41, 0x89, 0xC8}                  },
    {0x2A1679C, 0x2A167C8, {0x89, 0xC8}                        },
    {0x0C354B0, 0x0C35500, {0x89, 0x8E, 0x3C, 0x00, 0x00, 0x00}},
    {0x1252BF9, 0x1252C25, {0x89, 0xCB}                        },
    {0x127CF17, 0x127CF43, {0x89, 0xCF}                        },
    {0x17809E5, 0x1780A1D, {0x89, 0xC8}                        },
    {0x2A714A0, 0x2A71502, {0x89, 0xC8}                        },
    {0x2A57969, 0x2A579A3, {0x89, 0x8E, 0x30, 0x0B, 0x00, 0x00}},
    {0x2D959B1, 0x2D959DD, {0x89, 0xC8}                        },
    {0x2D9925E, 0x2D9928A, {0x89, 0xC8}                        },
    {0x3482FD4, 0x3483000, {0x89, 0xC8}                        },
    {0x3618CD8, 0x3618D07, {}                                  }
};

bool PatchFunction(HMODULE hModule, DWORD faddress_start, DWORD faddress_end, const std::vector<BYTE>& native_code) {
    LPVOID patchAddress = (LPVOID)((DWORD_PTR)hModule + faddress_start);


    // Construct the patch
    std::vector<BYTE> patch;

    patch.insert(patch.end(), native_code.begin(), native_code.end());


    // jmp faddress_end
    // Calculate the relative offset for the jump
    DWORD_PTR offset = (DWORD_PTR)hModule + faddress_end
                     - ((DWORD_PTR)patchAddress + patch.size() + 5); // +5 for the size of the jmp instruction

    patch.push_back(0xE9); // jmp opcode
    patch.push_back((BYTE)(offset & 0xFF));
    patch.push_back((BYTE)((offset >> 8) & 0xFF));
    patch.push_back((BYTE)((offset >> 16) & 0xFF));
    patch.push_back((BYTE)((offset >> 24) & 0xFF));

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    // Write the patch to memory
    DWORD oldProtect;
    if (!VirtualProtect(patchAddress, patch.size(), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        std::cerr << "Failed to change memory protection" << std::endl;
        return false;
    }

    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, patchAddress, patch.data(), patch.size(), &bytesWritten)
        || bytesWritten != patch.size()) {
        std::cerr << "Failed to write to process memory" << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    if (!VirtualProtect(patchAddress, patch.size(), oldProtect, &oldProtect)) {
        std::cerr << "Failed to restore memory protection" << std::endl;
        return false; // Though the patch is applied, restoring protection failed.
    }

    return true;
}

void printHexBytes(const void* address, size_t numBytes) {
    const unsigned char* start = static_cast<const unsigned char*>(address);

    for (size_t i = 0; i < numBytes; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(start[i]) << " ";
        // Optionally add a newline after every 16 bytes for better readability
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::dec << std::endl; // Reset to decimal output
}

} // namespace


void injectNaticeCode() {
    // Calculate the address in the target process
    HMODULE hModule = GetModuleHandle(L"bedrock_server_mod.exe");
    if (!hModule) {
        std::cerr << "Failed to get module handle for bedrock_server_mod.exe" << std::endl;
        return;
    }
    for (auto& item : data) {
        PatchFunction(hModule, item.func_rva_start, item.func_rva_end, item.native_code);
        if(item.func_rva_start == 0x12A9A9F){
            void* funcp = (void*)((BYTE*)hModule + 0x12A9A80);
            pl_notice(funcp);
        }
    };
};

// BOOL APIENTRY DllMain(HMODULE hModule,
//     DWORD  ul_reason_for_call,
//     LPVOID lpReserved
// )
// {
//     switch (ul_reason_for_call)
//     {
//     case DLL_PROCESS_ATTACH:
//         injectNaticeCode();
//         break;
//     case DLL_THREAD_ATTACH:
//     case DLL_THREAD_DETACH:
//     case DLL_PROCESS_DETACH:
//         break;
//     }
//     return TRUE;
// }