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
    {0x09e1ab0, 0x09e1ad5, 0x09e1af1, {0x89, 0xd3}                        },
    {0x09e1ee0, 0x09e2009, 0x09e202a, {0x89, 0xcb}                        },
    {0x0ce5440, 0x0ce564e, 0x0ce566a, {0x89, 0xd1}                        },
    {0x0ce5440, 0x0ce57b5, 0x0ce57e3, {0x41, 0x89, 0xd8}                  },
    {0x0ce6920, 0x0ce6c18, 0x0ce6c36, {0x41, 0x89, 0xc9}                  },
    {0x0ce6920, 0x0ce6ca2, 0x0ce6cbe, {0x89, 0xc8}                        },
    {0x0d01b80, 0x0d01c1b, 0x0d01c3e, {}                                  },
    {0x0f96e10, 0x0f96f39, 0x0f96f67, {0x41, 0x89, 0xc0}                  },
    {0x2151e40, 0x21520f0, 0x215211a, {0x89, 0x4e, 0x3c}                  },
    {0x22a14f0, 0x22a1757, 0x22a1773, {0x89, 0xcf}                        },
    {0x22a19e0, 0x22a1fd2, 0x22a1ffb, {0x89, 0xc3}                        },
    {0x2510070, 0x25101fb, 0x2510234, {0x89, 0xcb}                        },
    {0x257cc70, 0x257cca7, 0x257ccd0, {0x89, 0xc2}                        },
    {0x2592e10, 0x2592e60, 0x2592e89, {0x89, 0xc2}                        },
    {0x2594390, 0x25943c5, 0x25943ee, {0x89, 0xc2}                        },
    {0x25d00d0, 0x25d015e, 0x25d0187, {0x89, 0xc2}                        },
    {0x25f71c0, 0x25f7214, 0x25f723d, {0x89, 0xc2}                        },
    {0x260fc40, 0x260fc75, 0x260fc91, {0x89, 0xc8}                        },
    {0x26344e0, 0x26346b3, 0x26346d6, {0x89, 0xc3}                        },
    {0x26412f0, 0x264130f, 0x2641338, {0x89, 0xc2}                        },
    {0x2bc2c20, 0x2bc3159, 0x2bc3181, {0x41, 0x89, 0xc9}                  },
    {0x2bc3350, 0x2bc343f, 0x2bc3467, {0x41, 0x89, 0xc9}                  },
    {0x35c8470, 0x35c84f5, 0x35c8518, {0x41, 0x89, 0x0a}                  },
    {0x3857570, 0x3857a14, 0x3857a3c, {0x41, 0x89, 0xc9}                  },
    {0x3a95bb0, 0x3a96437, 0x3a96462, {0x89, 0xc3}                        },
    {0x3a97d10, 0x3a98e62, 0x3a98e80, {0x41, 0x89, 0xc8}                  },
    {0x3af9e80, 0x3afa060, 0x3afa07c, {0x89, 0xc8}                        },
    {0x3b55ff0, 0x3b560e5, 0x3b56107, {0x89, 0xc3}                        },
    {0x3df6b80, 0x3df733f, 0x3df7365, {0x41, 0x89, 0xc6}                  },
    {0x3df7cd0, 0x3df835a, 0x3df8389, {0x89, 0x8e, 0xe0, 0x0a, 0x00, 0x00}},
    {0x3df9c20, 0x3dfa05e, 0x3dfa089, {0x89, 0xc3}                        },
    {0x3df9c20, 0x3dfa50a, 0x3dfa536, {0x89, 0xdf}                        },
    {0x3e14f40, 0x3e14fc0, 0x3e14ff0, {0x89, 0x0b}                        },
    {0x3e15040, 0x3e15050, 0x3e1507b, {0x89, 0xd3}                        },
    {0x3e15370, 0x3e156f4, 0x3e15710, {0x89, 0xcb}                        },
    {0x3e157b0, 0x3e158e0, 0x3e15909, {0x89, 0xc3}                        },
    {0x44fe480, 0x44fe4f1, 0x44fe519, {0x41, 0x89, 0xd9}                  },
    {0x4524710, 0x45247d3, 0x45247f6, {0x89, 0xc3}                        },
    {0x4597bf0, 0x4597d61, 0x4597d7d, {0x89, 0xc8}                        },
    {0x4598230, 0x4598462, 0x4598490, {0x41, 0x89, 0xc6}                  },
    {0x459b230, 0x459b380, 0x459b3a4, {0x89, 0xc7}                        },
    {0x459b410, 0x459b5ae, 0x459b5ca, {0x89, 0xc8}                        },
    {0x471bd70, 0x471bd70, 0x471bd92, {0x89, 0x11}                        },
    {0x47fa410, 0x47fa422, 0x47fa450, {0x41, 0x89, 0xc3}                  },
    {0x4bece70, 0x4becf48, 0x4becf70, {0x89, 0xc7}                        },
    {0x4bee9e0, 0x4beeaa8, 0x4beeac4, {}                                  },
    {0x4c2adb0, 0x4c2aed5, 0x4c2aef9, {0x89, 0xcb}                        },
    {0x4c2beb0, 0x4c2bf2e, 0x4c2bf53, {0x89, 0xdf}                        },
    {0x4c31fc0, 0x4c32074, 0x4c32090, {0x89, 0xc8}                        },
    {0x4caf9f0, 0x4cafcb0, 0x4cafcd6, {0x41, 0x89, 0xc6}                  },
    {0x4cc2d20, 0x4cc2eff, 0x4cc2f2a, {0x89, 0xc8}                        },
    {0x4cc8a70, 0x4cc9069, 0x4cc908d, {0x41, 0x89, 0xcf}                  },
    {0x4cd4510, 0x4cd478b, 0x4cd47b4, {0x89, 0xc8}                        },
    {0x4cd7a20, 0x4cd7cca, 0x4cd7cee, {0x89, 0xc3}                        },
    {0x4d83e60, 0x4d83f6f, 0x4d83f92, {0x89, 0xcb}                        },
    {0x4d84180, 0x4d8429e, 0x4d842c1, {0x89, 0xcb}                        },
    {0x4d844b0, 0x4d845cc, 0x4d845ef, {0x89, 0xcb}                        },
    {0x4d8ae10, 0x4d8b18e, 0x4d8b1b1, {0x89, 0xc3}                        },
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
    HMODULE hModule = GetModuleHandle(L"Minecraft.Windows.exe");
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
