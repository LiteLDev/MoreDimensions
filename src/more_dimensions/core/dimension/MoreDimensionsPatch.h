#pragma once

#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <vector>

extern "C" {
__declspec(dllimport) bool pl_notice(void* target);
}
namespace Patch {
struct AddressAndReg {
    DWORD                   func_rva;       // target function rva
    DWORD                   func_rva_start; // patch start rva
    DWORD                   func_rva_end;   // jmp target rva
    const std::vector<BYTE> native_code;    // patch native code
};

// VanillaDimensions::toSerializedInt
// Remember to remove VanillaDimensions::fromSerializedInt(int) from result
#ifdef LL_PLAT_C
std::vector<AddressAndReg> data = {
    {0x0f9d240, 0x0f9d263, 0x0f9d27f, {0x89, 0xd3}                        },
    {0x0f9d660, 0x0f9d78d, 0x0f9d7ae, {0x89, 0xcb}                        },
    {0x19d8af0, 0x19d8be7, 0x19d8c15, {0x41, 0x89, 0xc0}                  },
    {0x1a1a9e0, 0x1a1ae05, 0x1a1ae33, {0x41, 0x89, 0xd8}                  },
    {0x1a1beb0, 0x1a1c0f8, 0x1a1c116, {0x41, 0x89, 0xc9}                  },
    {0x1a1beb0, 0x1a1c183, 0x1a1c19f, {0x89, 0xc8}                        },
    {0x1a357b0, 0x1a35845, 0x1a35868, {}                                  },
    {0x280d940, 0x280dbca, 0x280dbf4, {0x89, 0x46, 0x3c}                  },
    {0x2c79030, 0x2c79260, 0x2c7927c, {0x89, 0xc7}                        },
    {0x2c79420, 0x2c79999, 0x2c799c4, {0x89, 0xc3}                        },
    {0x2d305c0, 0x2d30716, 0x2d30732, {0x89, 0xc3}                        },
    {0x2d90050, 0x2d900d7, 0x2d90100, {0x89, 0xc2}                        },
    {0x2ee38b0, 0x2ee3a4c, 0x2ee3a75, {0x89, 0xc2}                        },
    {0x2fbe1c0, 0x2fbe214, 0x2fbe23d, {0x89, 0xc2}                        },
    {0x31dfb50, 0x31dfc31, 0x31dfc5a, {0x89, 0xc2}                        },
    {0x31f6e00, 0x31f6e56, 0x31f6e7f, {0x89, 0xc2}                        },
    {0x31f84e0, 0x31f8515, 0x31f853e, {0x89, 0xc2}                        },
    {0x33b62e0, 0x33b634d, 0x33b6376, {0x89, 0xc2}                        },
    {0x33c7fc0, 0x33c7ff5, 0x33c8011, {0x89, 0xc8}                        },
    {0x3722180, 0x3722235, 0x3722251, {0x89, 0xd0}                        },
    {0x37222e0, 0x372231f, 0x3722348, {0x89, 0xc1}                        },
    {0x37222e0, 0x3722391, 0x37223ba, {0x89, 0xc1}                        },
    {0x3bd1440, 0x3bd1798, 0x3bd17c0, {0x41, 0x89, 0xc9}                  },
    {0x3bd1a70, 0x3bd1b1f, 0x3bd1b47, {0x89, 0xcf}                        },
    {0x45ad0b0, 0x45ad135, 0x45ad158, {0x41, 0x89, 0x0b}                  },
    {0x48c92e0, 0x48c95cc, 0x48c95f4, {0x41, 0x89, 0xc9}                  },
    {0x4bae0d0, 0x4bae8d3, 0x4bae8fe, {0x89, 0xc3}                        },
    {0x4bb0480, 0x4bb12f4, 0x4bb1312, {0x41, 0x89, 0xc0}                  },
    {0x4bde900, 0x4bdea9e, 0x4bdeaba, {}                                  },
    {0x4c30de0, 0x4c30ed9, 0x4c30efb, {0x89, 0xc3}                        },
    {0x5039a80, 0x503a209, 0x503a22f, {0x41, 0x89, 0xc6}                  },
    {0x503ab60, 0x503b12c, 0x503b15b, {0x89, 0x86, 0xd0, 0x0a, 0x00, 0x00}},
    {0x503c7d0, 0x503cc10, 0x503cc3b, {0x89, 0xc3}                        },
    {0x503c7d0, 0x503d07c, 0x503d0a8, {0x89, 0xdf}                        },
    {0x5057980, 0x5057a00, 0x5057a30, {0x89, 0x0b}                        },
    {0x5057a80, 0x5057a8a, 0x5057ab3, {0x89, 0xd3}                        },
    {0x5057da0, 0x5057f8e, 0x5057faa, {0x89, 0xc3}                        },
    {0x5058040, 0x5058176, 0x505819f, {0x89, 0xc3}                        },
    {0x56e4bc0, 0x56e4c20, 0x56e4c48, {0x41, 0x89, 0xd9}                  },
    {0x56f29b0, 0x56f2a7a, 0x56f2aa5, {0x89, 0xc2}                        },
    {0x58655a0, 0x58656fd, 0x5865719, {}                                  },
    {0x5865b70, 0x5865d9e, 0x5865dcd, {0x89, 0xcd}                        },
    {0x5889d90, 0x5889eca, 0x5889eee, {0x89, 0xc7}                        },
    {0x5889f50, 0x588a0d6, 0x588a0f2, {}                                  },
    {0x5d6f790, 0x5d6f868, 0x5d6f890, {0x89, 0xc7}                        },
    {0x5d71210, 0x5d71297, 0x5d712b3, {}                                  },
    {0x5dfe530, 0x5dfe635, 0x5dfe66b, {0x89, 0xcf}                        },
    {0x5dff300, 0x5dff373, 0x5dff398, {0x89, 0xdf}                        },
    {0x5e057c0, 0x5e058d3, 0x5e058ef, {}                                  },
    {0x5e90430, 0x5e906b4, 0x5e906da, {0x41, 0x89, 0xc6}                  },
    {0x5ea5e80, 0x5ea5fe4, 0x5ea600f, {0x89, 0xc1}                        },
    {0x5eac670, 0x5eac8e7, 0x5eac912, {0x89, 0xc1}                        },
    {0x5eb7670, 0x5eb77f3, 0x5eb781c, {0x89, 0xc1}                        },
    {0x5eb9de0, 0x5eba042, 0x5eba067, {0x89, 0xc7}                        },
    {0x60c3630, 0x60c374f, 0x60c3773, {0x89, 0xcb}                        },
    {0x60c3990, 0x60c3abb, 0x60c3adf, {0x89, 0xcb}                        },
    {0x60c3d00, 0x60c3e29, 0x60c3e4d, {0x89, 0xcb}                        },
    {0x60cadb0, 0x60cb163, 0x60cb187, {0x89, 0xc7}                        },
    {0x6364680, 0x6364735, 0x6364751, {0x89, 0xd0}                        },
    {0x63647e0, 0x636481f, 0x6364848, {0x89, 0xc1}                        },
    {0x63647e0, 0x6364891, 0x63648ba, {0x89, 0xc1}                        },
};
#else
inline std::vector<AddressAndReg> data = {
    {0x09251f0, 0x0925445, 0x0925470, {0x89, 0xc1}                        },
    {0x092d9c0, 0x092db43, 0x092db6c, {0x89, 0xc1}                        },
    {0x09aea50, 0x09aeb6f, 0x09aeb93, {0x89, 0xcb}                        },
    {0x09aedb0, 0x09aeedb, 0x09aeeff, {0x89, 0xcb}                        },
    {0x09af120, 0x09af249, 0x09af26d, {0x89, 0xcb}                        },
    {0x09b61d0, 0x09b6583, 0x09b65a7, {0x89, 0xc7}                        },
    {0x0fa9380, 0x0fa9435, 0x0fa9451, {0x89, 0xd0}                        },
    {0x0fa94e0, 0x0fa951f, 0x0fa9548, {0x89, 0xc1}                        },
    {0x0fa94e0, 0x0fa9591, 0x0fa95ba, {0x89, 0xc1}                        },
    {0x13d7cd0, 0x13d7f5a, 0x13d7f84, {0x89, 0x46, 0x3c}                  },
    {0x1517d40, 0x1517f70, 0x1517f8c, {0x89, 0xc7}                        },
    {0x1518130, 0x15186a9, 0x15186d4, {0x89, 0xc3}                        },
    {0x199ff70, 0x19a00c6, 0x19a00e2, {0x89, 0xc3}                        },
    {0x19f1550, 0x19f15a4, 0x19f15cd, {0x89, 0xc2}                        },
    {0x1c6f320, 0x1c6f3a7, 0x1c6f3d0, {0x89, 0xc2}                        },
    {0x1d975d0, 0x1d9776c, 0x1d97795, {0x89, 0xc2}                        },
    {0x1e42c50, 0x1e42d31, 0x1e42d5a, {0x89, 0xc2}                        },
    {0x1e59e30, 0x1e59e86, 0x1e59eaf, {0x89, 0xc2}                        },
    {0x1e5b310, 0x1e5b345, 0x1e5b36e, {0x89, 0xc2}                        },
    {0x1ee3060, 0x1ee30cd, 0x1ee30f6, {0x89, 0xc2}                        },
    {0x209e070, 0x209e125, 0x209e141, {0x89, 0xd0}                        },
    {0x209e1d0, 0x209e20f, 0x209e238, {0x89, 0xc1}                        },
    {0x209e1d0, 0x209e281, 0x209e2aa, {0x89, 0xc1}                        },
    {0x2813de0, 0x2814138, 0x2814160, {0x41, 0x89, 0xc9}                  },
    {0x2814410, 0x28144bf, 0x28144e7, {0x89, 0xcf}                        },
    {0x3128c40, 0x3128cc5, 0x3128ce8, {0x41, 0x89, 0x0b}                  },
    {0x32827f0, 0x3282adc, 0x3282b04, {0x41, 0x89, 0xc9}                  },
    {0x363de20, 0x363e623, 0x363e64e, {0x89, 0xc3}                        },
    {0x36401d0, 0x3641044, 0x3641062, {0x41, 0x89, 0xc0}                  },
    {0x3665040, 0x36651de, 0x36651fa, {}                                  },
    {0x36b2e90, 0x36b2f89, 0x36b2fab, {0x89, 0xc3}                        },
    {0x3ae15d0, 0x3ae1d44, 0x3ae1d6a, {0x41, 0x89, 0xc6}                  },
    {0x3ae26a0, 0x3ae2c6c, 0x3ae2c9b, {0x89, 0x86, 0xd0, 0x0a, 0x00, 0x00}},
    {0x3ae4410, 0x3ae4850, 0x3ae487b, {0x89, 0xc3}                        },
    {0x3ae4410, 0x3ae4cbc, 0x3ae4ce8, {0x89, 0xdf}                        },
    {0x3afc670, 0x3afc6f0, 0x3afc720, {0x89, 0x0b}                        },
    {0x3afc770, 0x3afc77a, 0x3afc7a3, {0x89, 0xd3}                        },
    {0x3afca90, 0x3afcc7e, 0x3afcc9a, {0x89, 0xc3}                        },
    {0x3afcd30, 0x3afce66, 0x3afce8f, {0x89, 0xc3}                        },
    {0x40dff60, 0x40dffc0, 0x40dffe8, {0x41, 0x89, 0xd9}                  },
    {0x4105350, 0x410541a, 0x4105445, {0x89, 0xc2}                        },
    {0x423bf30, 0x423c08d, 0x423c0a9, {}                                  },
    {0x423c500, 0x423c72e, 0x423c75d, {0x89, 0xcd}                        },
    {0x423fa20, 0x423fb5a, 0x423fb7e, {0x89, 0xc7}                        },
    {0x423fbe0, 0x423fd66, 0x423fd82, {}                                  },
    {0x4724970, 0x4724a83, 0x4724a9f, {}                                  },
    {0x47da1e0, 0x47da2b7, 0x47da2df, {0x89, 0xc7}                        },
    {0x47dbc50, 0x47dbcd7, 0x47dbcf3, {}                                  },
    {0x4832550, 0x4832655, 0x483268b, {0x89, 0xcf}                        },
    {0x4833320, 0x4833393, 0x48333b8, {0x89, 0xdf}                        },
};
#endif

inline bool PatchFunction(HMODULE hModule, DWORD faddress_start, DWORD faddress_end, const std::vector<BYTE>& native_code) {
    LPVOID patchAddress = reinterpret_cast<LPVOID>(reinterpret_cast<DWORD_PTR>(hModule) + faddress_start);


    // Construct the patch
    std::vector<BYTE> patch;

    patch.insert(patch.end(), native_code.begin(), native_code.end());


    // jmp faddress_end
    // Calculate the relative offset for the jump
    DWORD_PTR offset = reinterpret_cast<DWORD_PTR>(hModule) + faddress_end
                     - (reinterpret_cast<DWORD_PTR>(patchAddress) + patch.size() + 5); // +5 for the size of the jmp instruction

    patch.push_back(0xE9); // jmp opcode
    patch.push_back(static_cast<BYTE>(offset & 0xFF));
    patch.push_back(static_cast<BYTE>((offset >> 8) & 0xFF));
    patch.push_back(static_cast<BYTE>((offset >> 16) & 0xFF));
    patch.push_back(static_cast<BYTE>((offset >> 24) & 0xFF));

    // Use RAII to ensure process handle is always closed
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (hProcess == nullptr || hProcess == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open process handle" << std::endl;
        return false;
    }

    // Auto-close handle using RAII pattern
    struct HandleCloser {
        HANDLE handle;
        ~HandleCloser() {
            if (handle != nullptr && handle != INVALID_HANDLE_VALUE) {
                CloseHandle(handle);
            }
        }
    } handleGuard{hProcess};

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
        // Restore protection before returning
        VirtualProtect(patchAddress, patch.size(), oldProtect, &oldProtect);
        return false;
    }

    if (!VirtualProtect(patchAddress, patch.size(), oldProtect, &oldProtect)) {
        std::cerr << "Failed to restore memory protection" << std::endl;
        return false; // Though the patch is applied, restoring protection failed.
    }

    return true;
}

inline void printHexBytes(const void* address, size_t numBytes) {
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

} // namespace Patch


inline void injectNaticeCode() {
    // Calculate the address in the target process
    HMODULE hModule = GetModuleHandle(nullptr);
    if (!hModule) {
        std::cerr << "Failed to get module handle for BDS" << std::endl;
        return;
    }
    for (auto& item : Patch::data) {
        Patch::PatchFunction(hModule, item.func_rva_start, item.func_rva_end, item.native_code);
        void* funcp = (void*)(reinterpret_cast<BYTE*>(hModule) + item.func_rva);
        pl_notice(funcp);
    };
};
