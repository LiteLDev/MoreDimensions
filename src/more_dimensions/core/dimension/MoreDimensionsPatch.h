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
    {0x0933b40, 0x0933da4, 0x0933dcf, {0x89, 0xc8}                        },
    {0x09bda50, 0x09bdb56, 0x09bdb79, {0x89, 0xcb}                        },
    {0x09bdd60, 0x09bde72, 0x09bde95, {0x89, 0xcb}                        },
    {0x09be080, 0x09be190, 0x09be1b3, {0x89, 0xcb}                        },
    {0x09c4760, 0x09c4b18, 0x09c4b3c, {0x89, 0xc7}                        },
    {0x0e98e60, 0x0e990f6, 0x0e99120, {0x89, 0x4e, 0x3c}                  },
    {0x0f53180, 0x0f53407, 0x0f53423, {0x89, 0xcf}                        },
    {0x0f53690, 0x0f53bf1, 0x0f53c1c, {0x89, 0xc3}                        },
    {0x13ae3f0, 0x13ae55a, 0x13ae576, {0x89, 0xcb}                        },
    {0x13c84f0, 0x13c86aa, 0x13c86cd, {0x89, 0xc3}                        },
    {0x14ab810, 0x14ab864, 0x14ab88d, {0x89, 0xc2}                        },
    {0x1783630, 0x17836a6, 0x17836cf, {0x89, 0xc2}                        },
    {0x1911e90, 0x1911eed, 0x1911f16, {0x89, 0xc2}                        },
    {0x1bb2e30, 0x1bb2e65, 0x1bb2e8e, {0x89, 0xc2}                        },
    {0x1c80c20, 0x1c80cf1, 0x1c80d1a, {0x89, 0xc2}                        },
    {0x1c96990, 0x1c969e0, 0x1c96a09, {0x89, 0xc2}                        },
    {0x1e51e20, 0x1e51ee1, 0x1e51efd, {0x89, 0xc8}                        },
    {0x1e51f60, 0x1e51fdd, 0x1e52007, {0x89, 0xc6}                        },
    {0x20df720, 0x20dfa71, 0x20dfa99, {0x41, 0x89, 0xc9}                  },
    {0x20dfcf0, 0x20dfda1, 0x20dfdc9, {0x89, 0xcf}                        },
    {0x2ec8380, 0x2ec8405, 0x2ec8428, {0x41, 0x89, 0x0a}                  },
    {0x322fb70, 0x322fe5a, 0x322fe82, {0x41, 0x89, 0xc9}                  },
    {0x3474f10, 0x3475704, 0x347572f, {0x89, 0xcb}                        },
    {0x3476fd0, 0x3477f06, 0x3477f24, {0x41, 0x89, 0xc8}                  },
    {0x34df220, 0x34df3ff, 0x34df41b, {0x89, 0xc8}                        },
    {0x3529580, 0x3529679, 0x352969b, {0x89, 0xc3}                        },
    {0x3909800, 0x3909f01, 0x3909f27, {0x41, 0x89, 0xce}                  },
    {0x390a830, 0x390add8, 0x390ae07, {0x89, 0x8e, 0xd0, 0x0a, 0x00, 0x00}},
    {0x390c4f0, 0x390c92f, 0x390c95a, {0x89, 0xc3}                        },
    {0x390c4f0, 0x390cdc4, 0x390cdf0, {0x89, 0xdf}                        },
    {0x3924fa0, 0x3925020, 0x3925050, {0x89, 0x0b}                        },
    {0x39250a0, 0x39250b0, 0x39250db, {0x89, 0xd3}                        },
    {0x39253b0, 0x39255ea, 0x3925606, {0x89, 0xcb}                        },
    {0x39256a0, 0x39257d6, 0x39257ff, {0x89, 0xc3}                        },
    {0x3fdcc30, 0x3fdcc90, 0x3fdccb8, {0x41, 0x89, 0xd9}                  },
    {0x4002790, 0x4002870, 0x4002899, {0x89, 0xc2}                        },
    {0x4192920, 0x4192a91, 0x4192aad, {0x89, 0xc8}                        },
    {0x4192f30, 0x4193172, 0x41931a0, {0x41, 0x89, 0xc4}                  },
    {0x4195b80, 0x4195cd4, 0x4195cf8, {0x89, 0xc7}                        },
    {0x4195d70, 0x4195f09, 0x4195f25, {0x89, 0xc8}                        },
    {0x466aa10, 0x466ab31, 0x466ab67, {0x89, 0xcf}                        },
    {0x466b690, 0x466b70f, 0x466b734, {0x89, 0xdf}                        },
    {0x46717c0, 0x4671a55, 0x4671a71, {0x89, 0xc8}                        },
    {0x471dee0, 0x471dfb8, 0x471dfe0, {0x89, 0xc7}                        },
    {0x471fa10, 0x471fab5, 0x471fad1, {}                                  },
    {0x57b1660, 0x57b1721, 0x57b173d, {0x89, 0xc8}                        },
    {0x57b17a0, 0x57b181d, 0x57b1847, {0x89, 0xc6}                        },
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
