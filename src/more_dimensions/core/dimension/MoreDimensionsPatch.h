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
    {0x122beb0, 0x122bed3, 0x122beef, {0x89, 0xd3}                        },
    {0x122c2c0, 0x122c3ed, 0x122c40e, {0x89, 0xcb}                        },
    {0x1c17f00, 0x1c17ff7, 0x1c18025, {0x41, 0x89, 0xc0}                  },
    {0x1c625e0, 0x1c62a07, 0x1c62a35, {0x41, 0x89, 0xd8}                  },
    {0x1c63ac0, 0x1c63d08, 0x1c63d26, {0x41, 0x89, 0xc9}                  },
    {0x1c63ac0, 0x1c63d93, 0x1c63daf, {0x89, 0xc8}                        },
    {0x1c7dbb0, 0x1c7dc45, 0x1c7dc68, {}                                  },
    {0x2937060, 0x29372e4, 0x293730a, {0x41, 0x89, 0xc6}                  },
    {0x294e040, 0x294e1ae, 0x294e1d9, {0x89, 0xc1}                        },
    {0x2954b40, 0x2954db8, 0x2954de3, {0x89, 0xc1}                        },
    {0x295f9a0, 0x295fb2a, 0x295fb53, {0x89, 0xc1}                        },
    {0x2962300, 0x2962566, 0x296258b, {0x89, 0xc7}                        },
    {0x2a69840, 0x2a6995f, 0x2a69983, {0x89, 0xcb}                        },
    {0x2a69ba0, 0x2a69ccb, 0x2a69cef, {0x89, 0xcb}                        },
    {0x2a69f10, 0x2a6a039, 0x2a6a05d, {0x89, 0xcb}                        },
    {0x2a70fc0, 0x2a71373, 0x2a71397, {0x89, 0xc7}                        },
    {0x2b71d70, 0x2b71e77, 0x2b71eb0, {}                                  },
    {0x2b71ef0, 0x2b71f35, 0x2b71f5e, {0x89, 0xc1}                        },
    {0x2b71ef0, 0x2b71fae, 0x2b71fd7, {0x89, 0xc1}                        },
    {0x35715a0, 0x357182a, 0x3571854, {0x89, 0x46, 0x3c}                  },
    {0x393adf0, 0x393b020, 0x393b03c, {0x89, 0xc7}                        },
    {0x393b1e0, 0x393b759, 0x393b784, {0x89, 0xc3}                        },
    {0x3a66670, 0x3a667c6, 0x3a667e2, {0x89, 0xc3}                        },
    {0x3b58010, 0x3b58097, 0x3b580c0, {0x89, 0xc2}                        },
    {0x3c11390, 0x3c113e4, 0x3c1140d, {0x89, 0xc2}                        },
    {0x3df0e70, 0x3df100c, 0x3df1035, {0x89, 0xc2}                        },
    {0x3faf6e0, 0x3faf736, 0x3faf75f, {0x89, 0xc2}                        },
    {0x3fb0da0, 0x3fb0dd5, 0x3fb0dfe, {0x89, 0xc2}                        },
    {0x412c890, 0x412c971, 0x412c99a, {0x89, 0xc2}                        },
    {0x41dc6c0, 0x41dc72d, 0x41dc756, {0x89, 0xc2}                        },
    {0x41f20b0, 0x41f20e5, 0x41f2101, {0x89, 0xc8}                        },
    {0x4529520, 0x4529627, 0x4529660, {}                                  },
    {0x45296a0, 0x45296e5, 0x452970e, {0x89, 0xc1}                        },
    {0x45296a0, 0x452975e, 0x4529787, {0x89, 0xc1}                        },
    {0x4b9ed60, 0x4b9f0b8, 0x4b9f0e0, {0x41, 0x89, 0xc9}                  },
    {0x4b9f3a0, 0x4b9f44f, 0x4b9f477, {0x89, 0xcf}                        },
    {0x53db110, 0x53db195, 0x53db1b8, {0x41, 0x89, 0x0b}                  },
    {0x54659a0, 0x5465ce0, 0x5465d08, {0x41, 0x89, 0xc9}                  },
    {0x5ab3660, 0x5ab3f8b, 0x5ab3fb6, {0x89, 0xc3}                        },
    {0x5ab5b40, 0x5ab6b04, 0x5ab6b22, {0x41, 0x89, 0xc0}                  },
    {0x5acf2d0, 0x5acf46e, 0x5acf48a, {}                                  },
    {0x5cbfbd0, 0x5cbfcc9, 0x5cbfceb, {0x89, 0xc3}                        },
    {0x5f86150, 0x5f868d9, 0x5f868ff, {0x41, 0x89, 0xc6}                  },
    {0x5f87190, 0x5f8775c, 0x5f8778b, {0x89, 0x86, 0xf8, 0x0a, 0x00, 0x00}},
    {0x5f891c0, 0x5f89600, 0x5f8962b, {0x89, 0xc3}                        },
    {0x5f891c0, 0x5f89a6c, 0x5f89a98, {0x89, 0xdf}                        },
    {0x5fa2f60, 0x5fa2fe0, 0x5fa3010, {0x89, 0x0b}                        },
    {0x5fa3060, 0x5fa306a, 0x5fa3093, {0x89, 0xd3}                        },
    {0x5fa3380, 0x5fa356e, 0x5fa358a, {0x89, 0xc3}                        },
    {0x5fa3620, 0x5fa3756, 0x5fa377f, {0x89, 0xc3}                        },
    {0x666e7a0, 0x666e800, 0x666e828, {0x41, 0x89, 0xd9}                  },
    {0x6792910, 0x6792a6d, 0x6792a89, {}                                  },
    {0x6792ee0, 0x679310e, 0x679313d, {0x89, 0xcd}                        },
    {0x6796310, 0x679644a, 0x679646e, {0x89, 0xc7}                        },
    {0x67964d0, 0x6796656, 0x6796672, {}                                  },
    {0x67f77a0, 0x67f7863, 0x67f788c, {0x89, 0xc2}                        },
    {0x6e34340, 0x6e34418, 0x6e34440, {0x89, 0xc7}                        },
    {0x6e35dc0, 0x6e35e47, 0x6e35e63, {}                                  },
    {0x6e548e0, 0x6e549e5, 0x6e54a1b, {0x89, 0xcf}                        },
    {0x6e556e0, 0x6e55753, 0x6e55778, {0x89, 0xdf}                        },
    {0x6e5bb80, 0x6e5bc93, 0x6e5bcaf, {}                                  },
};
#else
inline std::vector<AddressAndReg> data = {
    {0x0988980, 0x0988bd6, 0x0988c01, {0x89, 0xc1}                        },
    {0x0990dc0, 0x0990f48, 0x0990f71, {0x89, 0xc1}                        },
    {0x0a0ef20, 0x0a0f03f, 0x0a0f063, {0x89, 0xcb}                        },
    {0x0a0f280, 0x0a0f3ab, 0x0a0f3cf, {0x89, 0xcb}                        },
    {0x0a0f5f0, 0x0a0f719, 0x0a0f73d, {0x89, 0xcb}                        },
    {0x0a166a0, 0x0a16a53, 0x0a16a77, {0x89, 0xc7}                        },
    {0x0cfdf70, 0x0cfe1fa, 0x0cfe224, {0x89, 0x46, 0x3c}                  },
    {0x0de0540, 0x0de0770, 0x0de078c, {0x89, 0xc7}                        },
    {0x0de0930, 0x0de0ea9, 0x0de0ed4, {0x89, 0xc3}                        },
    {0x1289120, 0x1289276, 0x1289292, {0x89, 0xc3}                        },
    {0x12caad0, 0x12cab24, 0x12cab4d, {0x89, 0xc2}                        },
    {0x158e490, 0x158e517, 0x158e540, {0x89, 0xc2}                        },
    {0x16e2970, 0x16e2b0c, 0x16e2b35, {0x89, 0xc2}                        },
    {0x17cc390, 0x17cc3e6, 0x17cc40f, {0x89, 0xc2}                        },
    {0x17cd850, 0x17cd885, 0x17cd8ae, {0x89, 0xc2}                        },
    {0x18aefe0, 0x18af04d, 0x18af076, {0x89, 0xc2}                        },
    {0x1a56e40, 0x1a56f21, 0x1a56f4a, {0x89, 0xc2}                        },
    {0x1b647b0, 0x1b648b7, 0x1b648f0, {}                                  },
    {0x1b64930, 0x1b64975, 0x1b6499e, {0x89, 0xc1}                        },
    {0x1b64930, 0x1b649ee, 0x1b64a17, {0x89, 0xc1}                        },
    {0x1e22070, 0x1e223c8, 0x1e223f0, {0x41, 0x89, 0xc9}                  },
    {0x1e226b0, 0x1e2275f, 0x1e22787, {0x89, 0xcf}                        },
    {0x2a24fc0, 0x2a25045, 0x2a25068, {0x41, 0x89, 0x0b}                  },
    {0x2ce7bd0, 0x2ce7ebc, 0x2ce7ee4, {0x41, 0x89, 0xc9}                  },
    {0x326d8b0, 0x326e1db, 0x326e206, {0x89, 0xc3}                        },
    {0x326fd90, 0x3270d53, 0x3270d71, {0x41, 0x89, 0xc0}                  },
    {0x32961a0, 0x329633e, 0x329635a, {}                                  },
    {0x32e1d30, 0x32e1e29, 0x32e1e4b, {0x89, 0xc3}                        },
    {0x37ce920, 0x37cf094, 0x37cf0ba, {0x41, 0x89, 0xc6}                  },
    {0x37cf950, 0x37cff1c, 0x37cff4b, {0x89, 0x86, 0xf8, 0x0a, 0x00, 0x00}},
    {0x37d1a90, 0x37d1ed0, 0x37d1efb, {0x89, 0xc3}                        },
    {0x37d1a90, 0x37d233c, 0x37d2368, {0x89, 0xdf}                        },
    {0x37e8f70, 0x37e8ff0, 0x37e9020, {0x89, 0x0b}                        },
    {0x37e9070, 0x37e907a, 0x37e90a3, {0x89, 0xd3}                        },
    {0x37e9390, 0x37e957e, 0x37e959a, {0x89, 0xc3}                        },
    {0x37e9630, 0x37e9766, 0x37e978f, {0x89, 0xc3}                        },
    {0x3e2e860, 0x3e2e8c0, 0x3e2e8e8, {0x41, 0x89, 0xd9}                  },
    {0x3e53ff0, 0x3e540b3, 0x3e540dc, {0x89, 0xc2}                        },
    {0x3fe7300, 0x3fe745d, 0x3fe7479, {}                                  },
    {0x3fe78d0, 0x3fe7afe, 0x3fe7b2d, {0x89, 0xcd}                        },
    {0x3fead00, 0x3feae3a, 0x3feae5e, {0x89, 0xc7}                        },
    {0x3feaec0, 0x3feb046, 0x3feb062, {}                                  },
    {0x44ddf50, 0x44de055, 0x44de08b, {0x89, 0xcf}                        },
    {0x44ded50, 0x44dedc3, 0x44dede8, {0x89, 0xdf}                        },
    {0x44e5210, 0x44e5323, 0x44e533f, {}                                  },
    {0x45f0f70, 0x45f1047, 0x45f106f, {0x89, 0xc7}                        },
    {0x45f29e0, 0x45f2a67, 0x45f2a83, {}                                  },
    {0x561bde0, 0x561bee7, 0x561bf20, {}                                  },
    {0x561bf60, 0x561bfa5, 0x561bfce, {0x89, 0xc1}                        },
    {0x561bf60, 0x561c01e, 0x561c047, {0x89, 0xc1}                        },
};
#endif

inline bool
PatchFunction(HMODULE hModule, DWORD faddress_start, DWORD faddress_end, const std::vector<BYTE>& native_code) {
    LPVOID patchAddress = reinterpret_cast<LPVOID>(reinterpret_cast<DWORD_PTR>(hModule) + faddress_start);


    // Construct the patch
    std::vector<BYTE> patch;

    patch.insert(patch.end(), native_code.begin(), native_code.end());


    // jmp faddress_end
    // Calculate the relative offset for the jump
    DWORD_PTR offset =
        reinterpret_cast<DWORD_PTR>(hModule) + faddress_end
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
