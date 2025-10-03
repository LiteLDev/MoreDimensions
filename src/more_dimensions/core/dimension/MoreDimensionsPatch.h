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
    {0x0888530, 0x088880f, 0x088883a, {0x89, 0xc8}                        },
    {0x090ccb0, 0x090cdbf, 0x090cde2, {0x89, 0xcb}                        },
    {0x090cfd0, 0x090d0ee, 0x090d111, {0x89, 0xcb}                        },
    {0x090d300, 0x090d41c, 0x090d43f, {0x89, 0xcb}                        },
    {0x0913c50, 0x0913fce, 0x0913ff1, {0x89, 0xc3}                        },
    {0x0d8d360, 0x0d8d610, 0x0d8d63a, {0x89, 0x4e, 0x3c}                  },
    {0x0dc8f90, 0x0dc91f7, 0x0dc9213, {0x89, 0xcf}                        },
    {0x0dc9480, 0x0dc9a72, 0x0dc9a9b, {0x89, 0xc3}                        },
    {0x124a2e0, 0x124a45b, 0x124a494, {0x89, 0xcb}                        },
    {0x125dd40, 0x125df13, 0x125df36, {0x89, 0xc3}                        },
    {0x12c42d0, 0x12c4324, 0x12c434d, {0x89, 0xc2}                        },
    {0x13e8a70, 0x13e8aa7, 0x13e8ad0, {0x89, 0xc2}                        },
    {0x1414050, 0x14140a5, 0x14140ce, {0x89, 0xc2}                        },
    {0x1561160, 0x15611ee, 0x1561217, {0x89, 0xc2}                        },
    {0x1567e00, 0x1567e50, 0x1567e79, {0x89, 0xc2}                        },
    {0x1569160, 0x1569195, 0x15691be, {0x89, 0xc2}                        },
    {0x1845e20, 0x1845ee1, 0x1845efd, {0x89, 0xc8}                        },
    {0x1845f60, 0x1845fdd, 0x1846007, {0x89, 0xc6}                        },
    {0x1e58d20, 0x1e59259, 0x1e59281, {0x41, 0x89, 0xc9}                  },
    {0x1e59450, 0x1e5953f, 0x1e59567, {0x41, 0x89, 0xc9}                  },
    {0x2806f90, 0x2807015, 0x2807038, {0x41, 0x89, 0x0a}                  },
    {0x2b4cd80, 0x2b4d224, 0x2b4d24c, {0x41, 0x89, 0xc9}                  },
    {0x2d3faf0, 0x2d40377, 0x2d403a2, {0x89, 0xc3}                        },
    {0x2d41c50, 0x2d42cfa, 0x2d42d18, {0x41, 0x89, 0xc8}                  },
    {0x2db2420, 0x2db2600, 0x2db261c, {0x89, 0xc8}                        },
    {0x2e11b70, 0x2e11c65, 0x2e11c87, {0x89, 0xc3}                        },
    {0x30c55e0, 0x30c5d91, 0x30c5db7, {0x41, 0x89, 0xc6}                  },
    {0x30c6720, 0x30c6cf7, 0x30c6d26, {0x89, 0x8e, 0xe0, 0x0a, 0x00, 0x00}},
    {0x30c8530, 0x30c896e, 0x30c8999, {0x89, 0xc3}                        },
    {0x30c8530, 0x30c8e1a, 0x30c8e46, {0x89, 0xdf}                        },
    {0x30e0b00, 0x30e0b80, 0x30e0bb0, {0x89, 0x0b}                        },
    {0x30e0c00, 0x30e0c10, 0x30e0c3b, {0x89, 0xd3}                        },
    {0x30e0f30, 0x30e116f, 0x30e118b, {0x89, 0xcb}                        },
    {0x30e1220, 0x30e1350, 0x30e1379, {0x89, 0xc3}                        },
    {0x37344a0, 0x3734511, 0x3734539, {0x41, 0x89, 0xd9}                  },
    {0x3742570, 0x3742633, 0x3742656, {0x89, 0xc3}                        },
    {0x38d0ca0, 0x38d0e11, 0x38d0e2d, {0x89, 0xc8}                        },
    {0x38d12e0, 0x38d1512, 0x38d1540, {0x41, 0x89, 0xc6}                  },
    {0x38d42f0, 0x38d4440, 0x38d4464, {0x89, 0xc7}                        },
    {0x38d44d0, 0x38d466e, 0x38d468a, {0x89, 0xc8}                        },
    {0x38f7670, 0x38f7682, 0x38f76b0, {0x41, 0x89, 0xc3}                  },
    {0x3924920, 0x3924920, 0x3924942, {0x89, 0x11}                        },
    {0x3db11c0, 0x3db1274, 0x3db1290, {0x89, 0xc8}                        },
    {0x3e5e340, 0x3e5e418, 0x3e5e440, {0x89, 0xc7}                        },
    {0x3e5feb0, 0x3e5ff78, 0x3e5ff94, {}                                  },
    {0x3eaf040, 0x3eaf165, 0x3eaf189, {0x89, 0xcb}                        },
    {0x3eb0020, 0x3eb009e, 0x3eb00c3, {0x89, 0xdf}                        },
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
