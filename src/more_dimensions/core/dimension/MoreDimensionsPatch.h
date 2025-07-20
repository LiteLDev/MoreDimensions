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
    {0x08ecf50, 0x08ecf75, 0x08ecf91, {0x89, 0xd3}                        },
    {0x08ed390, 0x08ed4b9, 0x08ed4da, {0x89, 0xcb}                        },
    {0x0ca5a70, 0x0ca5c7e, 0x0ca5c9a, {0x89, 0xd1}                        },
    {0x0ca6d70, 0x0ca7058, 0x0ca7076, {0x41, 0x89, 0xc9}                  },
    {0x0ca6d70, 0x0ca70e0, 0x0ca70fc, {0x89, 0xc8}                        },
    {0x0cc2410, 0x0cc24ab, 0x0cc24ce, {}                                  },
    {0x0fd9f40, 0x0fda069, 0x0fda097, {0x41, 0x89, 0xc0}                  },
    {0x1faca20, 0x1facb99, 0x1facbb5, {0x89, 0xcb}                        },
    {0x1feb4c0, 0x1feb54e, 0x1feb577, {0x89, 0xc2}                        },
    {0x2071b90, 0x2071bc7, 0x2071bf0, {0x89, 0xc2}                        },
    {0x213c5b0, 0x213c600, 0x213c629, {0x89, 0xc2}                        },
    {0x213d6d0, 0x213d705, 0x213d72e, {0x89, 0xc2}                        },
    {0x215b100, 0x215b154, 0x215b17d, {0x89, 0xc2}                        },
    {0x217a980, 0x217a99f, 0x217a9c8, {0x89, 0xc2}                        },
    {0x21806d0, 0x2180705, 0x2180721, {0x89, 0xc8}                        },
    {0x2197360, 0x2197533, 0x2197556, {0x89, 0xc3}                        },
    {0x21fd1d0, 0x21fd437, 0x21fd453, {0x89, 0xcf}                        },
    {0x21fd6c0, 0x21fdcb2, 0x21fdcdb, {0x89, 0xc3}                        },
    {0x23f0640, 0x23f06c5, 0x23f06e8, {0x41, 0x89, 0x0a}                  },
    {0x28e84a0, 0x28e885b, 0x28e8883, {0x41, 0x89, 0xc9}                  },
    {0x28e8a20, 0x28e8b1b, 0x28e8b3e, {0x89, 0xc8}                        },
    {0x2c0d340, 0x2c0d667, 0x2c0d68f, {0x41, 0x89, 0xc9}                  },
    {0x3792360, 0x3792b1f, 0x3792b45, {0x41, 0x89, 0xc6}                  },
    {0x37934b0, 0x3793b3f, 0x3793b6e, {0x89, 0x8e, 0xe0, 0x0a, 0x00, 0x00}},
    {0x37953c0, 0x3795828, 0x3795853, {0x89, 0xc3}                        },
    {0x37953c0, 0x3795cd4, 0x3795d00, {0x89, 0xdf}                        },
    {0x37b0800, 0x37b0880, 0x37b08b0, {0x89, 0x0b}                        },
    {0x37b0900, 0x37b0910, 0x37b093b, {0x89, 0xd3}                        },
    {0x37b0c30, 0x37b0fb4, 0x37b0fd0, {0x89, 0xcb}                        },
    {0x37b1070, 0x37b11a0, 0x37b11c9, {0x89, 0xc3}                        },
    {0x383c690, 0x383cf17, 0x383cf42, {0x89, 0xc3}                        },
    {0x383e7f0, 0x383f92d, 0x383f94b, {0x41, 0x89, 0xc8}                  },
    {0x38bf8f0, 0x38bfabc, 0x38bfad8, {0x89, 0xc8}                        },
    {0x3918cc0, 0x3918db5, 0x3918dd7, {0x89, 0xc3}                        },
    {0x3d56fb0, 0x3d57073, 0x3d57096, {0x89, 0xc3}                        },
    {0x3ea50a0, 0x3ea5211, 0x3ea522d, {0x89, 0xc8}                        },
    {0x3ea56e0, 0x3ea5902, 0x3ea5930, {0x41, 0x89, 0xc6}                  },
    {0x3ea8740, 0x3ea8890, 0x3ea88b4, {0x89, 0xc7}                        },
    {0x3ea8920, 0x3ea8abe, 0x3ea8ada, {0x89, 0xc8}                        },
    {0x42486f0, 0x424874e, 0x4248771, {0x89, 0xd8}                        },
    {0x45eb110, 0x45eb122, 0x45eb150, {0x41, 0x89, 0xc3}                  },
    {0x4642650, 0x4642775, 0x4642799, {0x89, 0xcb}                        },
    {0x4643750, 0x46437ce, 0x46437f3, {0x89, 0xdf}                        },
    {0x4649970, 0x4649a24, 0x4649a40, {0x89, 0xc8}                        },
    {0x4689e80, 0x4689e80, 0x4689ea2, {0x89, 0x11}                        },
    {0x4825800, 0x482594d, 0x4825975, {0x89, 0xc7}                        },
    {0x4827560, 0x482761b, 0x4827637, {}                                  },
    {0x49b7640, 0x49b7900, 0x49b7926, {0x41, 0x89, 0xc6}                  },
    {0x49caf90, 0x49cb16f, 0x49cb19a, {0x89, 0xc8}                        },
    {0x49d0c20, 0x49d1219, 0x49d123d, {0x41, 0x89, 0xcf}                  },
    {0x49dc820, 0x49dca85, 0x49dcaad, {0x89, 0xc8}                        },
    {0x49e0180, 0x49e041b, 0x49e043f, {0x89, 0xc3}                        },
    {0x4a986d0, 0x4a98ad6, 0x4a98afb, {0x89, 0xc6}                        },
    {0x4a9a000, 0x4a9a04a, 0x4a9a06f, {0x89, 0xcf}                        },
    {0x51008a0, 0x5100b50, 0x5100b7a, {0x89, 0x4e, 0x3c}                  },
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
