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
    {0x0934170, 0x09343d4, 0x09343ff, {0x89, 0xc8}                        },
    {0x09be090, 0x09be196, 0x09be1b9, {0x89, 0xcb}                        },
    {0x09be3a0, 0x09be4b2, 0x09be4d5, {0x89, 0xcb}                        },
    {0x09be6c0, 0x09be7d0, 0x09be7f3, {0x89, 0xcb}                        },
    {0x09c4da0, 0x09c5158, 0x09c517c, {0x89, 0xc7}                        },
    {0x0e994a0, 0x0e99736, 0x0e99760, {0x89, 0x4e, 0x3c}                  },
    {0x0f537c0, 0x0f53a47, 0x0f53a63, {0x89, 0xcf}                        },
    {0x0f53cd0, 0x0f54231, 0x0f5425c, {0x89, 0xc3}                        },
    {0x13aea30, 0x13aeb9a, 0x13aebb6, {0x89, 0xcb}                        },
    {0x13c8b30, 0x13c8cea, 0x13c8d0d, {0x89, 0xc3}                        },
    {0x14abe50, 0x14abea4, 0x14abecd, {0x89, 0xc2}                        },
    {0x1783c70, 0x1783ce6, 0x1783d0f, {0x89, 0xc2}                        },
    {0x19124d0, 0x191252d, 0x1912556, {0x89, 0xc2}                        },
    {0x1bb3470, 0x1bb34a5, 0x1bb34ce, {0x89, 0xc2}                        },
    {0x1c81260, 0x1c81331, 0x1c8135a, {0x89, 0xc2}                        },
    {0x1c96fd0, 0x1c97020, 0x1c97049, {0x89, 0xc2}                        },
    {0x1e52460, 0x1e52521, 0x1e5253d, {0x89, 0xc8}                        },
    {0x1e525a0, 0x1e5261d, 0x1e52647, {0x89, 0xc6}                        },
    {0x20dfc70, 0x20dffc1, 0x20dffe9, {0x41, 0x89, 0xc9}                  },
    {0x20e0240, 0x20e02f1, 0x20e0319, {0x89, 0xcf}                        },
    {0x2ec88d0, 0x2ec8955, 0x2ec8978, {0x41, 0x89, 0x0a}                  },
    {0x32300c0, 0x32303aa, 0x32303d2, {0x41, 0x89, 0xc9}                  },
    {0x3475460, 0x3475c54, 0x3475c7f, {0x89, 0xcb}                        },
    {0x3477520, 0x3478456, 0x3478474, {0x41, 0x89, 0xc8}                  },
    {0x34df770, 0x34df94f, 0x34df96b, {0x89, 0xc8}                        },
    {0x3529ad0, 0x3529bc9, 0x3529beb, {0x89, 0xc3}                        },
    {0x3909d50, 0x390a451, 0x390a477, {0x41, 0x89, 0xce}                  },
    {0x390ad80, 0x390b328, 0x390b357, {0x89, 0x8e, 0xd0, 0x0a, 0x00, 0x00}},
    {0x390ca40, 0x390ce7f, 0x390ceaa, {0x89, 0xc3}                        },
    {0x390ca40, 0x390d314, 0x390d340, {0x89, 0xdf}                        },
    {0x39254f0, 0x3925570, 0x39255a0, {0x89, 0x0b}                        },
    {0x39255f0, 0x3925600, 0x392562b, {0x89, 0xd3}                        },
    {0x3925900, 0x3925b3a, 0x3925b56, {0x89, 0xcb}                        },
    {0x3925bf0, 0x3925d26, 0x3925d4f, {0x89, 0xc3}                        },
    {0x3fdd180, 0x3fdd1e0, 0x3fdd208, {0x41, 0x89, 0xd9}                  },
    {0x4002ce0, 0x4002dc0, 0x4002de9, {0x89, 0xc2}                        },
    {0x4192e70, 0x4192fe1, 0x4192ffd, {0x89, 0xc8}                        },
    {0x4193480, 0x41936c2, 0x41936f0, {0x41, 0x89, 0xc4}                  },
    {0x41960d0, 0x4196224, 0x4196248, {0x89, 0xc7}                        },
    {0x41962c0, 0x4196459, 0x4196475, {0x89, 0xc8}                        },
    {0x466afb0, 0x466b0d1, 0x466b107, {0x89, 0xcf}                        },
    {0x466bc30, 0x466bcaf, 0x466bcd4, {0x89, 0xdf}                        },
    {0x4671d60, 0x4671ff5, 0x4672011, {0x89, 0xc8}                        },
    {0x471e480, 0x471e558, 0x471e580, {0x89, 0xc7}                        },
    {0x471ffb0, 0x4720055, 0x4720071, {}                                  },
    {0x57b1c30, 0x57b1cf1, 0x57b1d0d, {0x89, 0xc8}                        },
    {0x57b1d70, 0x57b1ded, 0x57b1e17, {0x89, 0xc6}                        },
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
