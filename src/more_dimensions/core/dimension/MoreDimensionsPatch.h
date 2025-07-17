#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <vector>

extern "C" {
__declspec(dllimport) bool pl_notice(void* target);
}
namespace {
struct AddressAndReg {
    DWORD                   func_rva;
    DWORD                   func_rva_start;
    DWORD                   func_rva_end;
    const std::vector<BYTE> native_code;
};

// 0~26 VanillaDimensions::toSerializedInt
// 27~39 VanillaDimensions::fromSerializedInt(int)
std::vector<AddressAndReg> data = {
    {0x08544d0, 0x08547af, 0x08547da, {0x89, 0xc8}                        },
    {0x08d7c20, 0x08d7d2f, 0x08d7d52, {0x89, 0xcb}                        },
    {0x08d7f40, 0x08d805e, 0x08d8081, {0x89, 0xcb}                        },
    {0x08d8270, 0x08d838c, 0x08d83af, {0x89, 0xcb}                        },
    {0x08debc0, 0x08def3e, 0x08def61, {0x89, 0xc3}                        },
    {0x0c3b740, 0x0c3b9f0, 0x0c3ba1a, {0x89, 0x4e, 0x3c}                  },
    {0x0c83ad0, 0x0c83d37, 0x0c83d53, {0x89, 0xcf}                        },
    {0x0c83fc0, 0x0c845b2, 0x0c845db, {0x89, 0xc3}                        },
    {0x1046ae0, 0x1046c6b, 0x1046ca4, {0x89, 0xcb}                        },
    {0x1087090, 0x10870e4, 0x108710d, {0x89, 0xc2}                        },
    {0x10a9b20, 0x10a9b3f, 0x10a9b68, {0x89, 0xc2}                        },
    {0x10ec410, 0x10ec447, 0x10ec470, {0x89, 0xc2}                        },
    {0x1104c80, 0x1104e53, 0x1104e76, {0x89, 0xc3}                        },
    {0x112d310, 0x112d360, 0x112d389, {0x89, 0xc2}                        },
    {0x112e670, 0x112e6a5, 0x112e6ce, {0x89, 0xc2}                        },
    {0x1149d90, 0x1149e1e, 0x1149e47, {0x89, 0xc2}                        },
    {0x17880d0, 0x1788609, 0x1788631, {0x41, 0x89, 0xc9}                  },
    {0x1788800, 0x17888ef, 0x1788917, {0x41, 0x89, 0xc9}                  },
    {0x20df6c0, 0x20df745, 0x20df768, {0x41, 0x89, 0x0a}                  },
    {0x2391820, 0x2391cc4, 0x2391cec, {0x41, 0x89, 0xc9}                  },
    {0x25a4b80, 0x25a5407, 0x25a5432, {0x89, 0xc3}                        },
    {0x25a6ce0, 0x25a7e32, 0x25a7e50, {0x41, 0x89, 0xc8}                  },
    {0x260b9c0, 0x260bba0, 0x260bbbc, {0x89, 0xc8}                        },
    {0x26657d0, 0x26658c5, 0x26658e7, {0x89, 0xc3}                        },
    {0x28da340, 0x28daaf1, 0x28dab17, {0x41, 0x89, 0xc6}                  },
    {0x28db480, 0x28dba57, 0x28dba86, {0x89, 0x8e, 0xe0, 0x0a, 0x00, 0x00}},
    {0x28dd290, 0x28dd6ce, 0x28dd6f9, {0x89, 0xc3}                        },
    {0x28dd290, 0x28ddb7a, 0x28ddba6, {0x89, 0xdf}                        },
    {0x28f5690, 0x28f5710, 0x28f5740, {0x89, 0x0b}                        },
    {0x28f5790, 0x28f57a0, 0x28f57cb, {0x89, 0xd3}                        },
    {0x28f5ac0, 0x28f5e44, 0x28f5e60, {0x89, 0xcb}                        },
    {0x28f5f00, 0x28f6030, 0x28f6059, {0x89, 0xc3}                        },
    {0x2f554e0, 0x2f55551, 0x2f55579, {0x41, 0x89, 0xd9}                  },
    {0x2f63520, 0x2f635e3, 0x2f63606, {0x89, 0xc3}                        },
    {0x30e3ca0, 0x30e3e11, 0x30e3e2d, {0x89, 0xc8}                        },
    {0x30e42e0, 0x30e4512, 0x30e4540, {0x41, 0x89, 0xc6}                  },
    {0x30e72e0, 0x30e7430, 0x30e7454, {0x89, 0xc7}                        },
    {0x30e74c0, 0x30e765e, 0x30e767a, {0x89, 0xc8}                        },
    {0x3159430, 0x3159430, 0x3159452, {0x89, 0x11}                        },
    {0x3254240, 0x3254252, 0x3254280, {0x41, 0x89, 0xc3}                  },
    {0x35967d0, 0x3596884, 0x35968a0, {0x89, 0xc8}                        },
    {0x363c0c0, 0x363c198, 0x363c1c0, {0x89, 0xc7}                        },
    {0x363dc30, 0x363dcf8, 0x363dd14, {}                                  },
    {0x36965d0, 0x36966f5, 0x3696719, {0x89, 0xcb}                        },
    {0x36976d0, 0x369774e, 0x3697773, {0x89, 0xdf}                        },
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
        void* funcp = (void*)((BYTE*)hModule + item.func_rva);
        pl_notice(funcp);
    };
};
