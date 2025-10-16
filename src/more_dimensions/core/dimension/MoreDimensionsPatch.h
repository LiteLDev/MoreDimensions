#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <vector>

extern "C" {
__declspec(dllimport) bool pl_notice(void* target);
}
namespace {
struct AddressAndReg {
    DWORD                   func_rva;       // target function rva
    DWORD                   func_rva_start; // patch start rva
    DWORD                   func_rva_end;   // jmp target rva
    const std::vector<BYTE> native_code;    // patch native code
};

// VanillaDimensions::toSerializedInt and VanillaDimensions::fromSerializedInt(int)
std::vector<AddressAndReg> data = {
    {0x0913f00, 0x0914164, 0x091418f, {0x89, 0xc8}                        },
    {0x099c570, 0x099c676, 0x099c699, {0x89, 0xcb}                        },
    {0x099c880, 0x099c992, 0x099c9b5, {0x89, 0xcb}                        },
    {0x099cba0, 0x099ccb0, 0x099ccd3, {0x89, 0xcb}                        },
    {0x09a3280, 0x09a3638, 0x09a365c, {0x89, 0xc7}                        },
    {0x11be120, 0x11be1e1, 0x11be1fd, {0x89, 0xc8}                        },
    {0x11be260, 0x11be2dd, 0x11be307, {0x89, 0xc6}                        },
    {0x14b4dd0, 0x14b5066, 0x14b5090, {0x89, 0x4e, 0x3c}                  },
    {0x154c370, 0x154c5f7, 0x154c613, {0x89, 0xcf}                        },
    {0x154c880, 0x154cde1, 0x154ce0c, {0x89, 0xc3}                        },
    {0x1980180, 0x19802ea, 0x1980306, {0x89, 0xcb}                        },
    {0x19d12b0, 0x19d1304, 0x19d132d, {0x89, 0xc2}                        },
    {0x1c57aa0, 0x1c57b16, 0x1c57b3f, {0x89, 0xc2}                        },
    {0x1d13a20, 0x1d13bda, 0x1d13bfd, {0x89, 0xc3}                        },
    {0x1df6730, 0x1df6780, 0x1df67a9, {0x89, 0xc2}                        },
    {0x1df7de0, 0x1df7e15, 0x1df7e3e, {0x89, 0xc2}                        },
    {0x1ecada0, 0x1ecae71, 0x1ecae9a, {0x89, 0xc2}                        },
    {0x2066f30, 0x2066f8d, 0x2066fb6, {0x89, 0xc2}                        },
    {0x21ea950, 0x21eaa11, 0x21eaa2d, {0x89, 0xc8}                        },
    {0x21eaa90, 0x21eab0d, 0x21eab37, {0x89, 0xc6}                        },
    {0x28b9d30, 0x28ba081, 0x28ba0a9, {0x41, 0x89, 0xc9}                  },
    {0x28ba300, 0x28ba3b1, 0x28ba3d9, {0x89, 0xcf}                        },
    {0x32dc680, 0x32dc705, 0x32dc728, {0x41, 0x89, 0x0a}                  },
    {0x34ae010, 0x34ae2fa, 0x34ae322, {0x41, 0x89, 0xc9}                  },
    {0x38d2ad0, 0x38d32c4, 0x38d32ef, {0x89, 0xcb}                        },
    {0x38d4b90, 0x38d5ac6, 0x38d5ae4, {0x41, 0x89, 0xc8}                  },
    {0x38efae0, 0x38efcbf, 0x38efcdb, {0x89, 0xc8}                        },
    {0x39f5480, 0x39f5579, 0x39f559b, {0x89, 0xc3}                        },
    {0x3bdbf50, 0x3bdc651, 0x3bdc677, {0x41, 0x89, 0xce}                  },
    {0x3bdcf80, 0x3bdd528, 0x3bdd557, {0x89, 0x8e, 0xd0, 0x0a, 0x00, 0x00}},
    {0x3bdec40, 0x3bdf07f, 0x3bdf0aa, {0x89, 0xc3}                        },
    {0x3bdec40, 0x3bdf514, 0x3bdf540, {0x89, 0xdf}                        },
    {0x3bf7710, 0x3bf7790, 0x3bf77c0, {0x89, 0x0b}                        },
    {0x3bf7810, 0x3bf7820, 0x3bf784b, {0x89, 0xd3}                        },
    {0x3bf7b20, 0x3bf7d5a, 0x3bf7d76, {0x89, 0xcb}                        },
    {0x3bf7e10, 0x3bf7f46, 0x3bf7f6f, {0x89, 0xc3}                        },
    {0x4260e30, 0x4260e90, 0x4260eb8, {0x41, 0x89, 0xd9}                  },
    {0x426dcc0, 0x426dda0, 0x426ddc9, {0x89, 0xc2}                        },
    {0x43fa280, 0x43fa3f1, 0x43fa40d, {0x89, 0xc8}                        },
    {0x43fa890, 0x43faad2, 0x43fab00, {0x41, 0x89, 0xc4}                  },
    {0x43fd740, 0x43fd894, 0x43fd8b8, {0x89, 0xc7}                        },
    {0x43fd930, 0x43fdac9, 0x43fdae5, {0x89, 0xc8}                        },
    {0x48926e0, 0x4892801, 0x4892837, {0x89, 0xcf}                        },
    {0x4893360, 0x48933df, 0x4893404, {0x89, 0xdf}                        },
    {0x4899570, 0x4899805, 0x4899821, {0x89, 0xc8}                        },
    {0x4984c20, 0x4984cf8, 0x4984d20, {0x89, 0xc7}                        },
    {0x4986750, 0x49867f5, 0x4986811, {}                                  },
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
    HMODULE hModule = GetModuleHandle(NULL);
    if (!hModule) {
        std::cerr << "Failed to get module handle for BDS" << std::endl;
        return;
    }
    for (auto& item : data) {
        PatchFunction(hModule, item.func_rva_start, item.func_rva_end, item.native_code);
        void* funcp = (void*)((BYTE*)hModule + item.func_rva);
        pl_notice(funcp);
    };
};
