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
    {0x08440f0, 0x08443cf, 0x08443fa, {0x89, 0xc8}                     },
    {0x262a500, 0x262a5f5, 0x262a617, {0x89, 0xc3}                     },
    {0x104de80, 0x104deb7, 0x104dee0, {0x89, 0xc2}                     },
    {0x10a4f60, 0x10a4fee, 0x10a5017, {0x89, 0xc2}                     },
    {0x1089720, 0x1089770, 0x1089799, {0x89, 0xc2}                     },
    {0x108a620, 0x108a655, 0x108a67e, {0x89, 0xc2}                     },
    {0x0d018e0, 0x0d01ed2, 0x0d01efb, {0x89, 0xc3}                     },
    {0x1062410, 0x10625e3, 0x1062606, {0x89, 0xc3}                     },
    {0x0d93460, 0x0d9347f, 0x0d934a8, {0x89, 0xc2}                     },
    {0x0d71150, 0x0d711a4, 0x0d711cd, {0x89, 0xc2}                     },
    {0x197e0f0, 0x197e4ab, 0x197e4d3, {0x41, 0x88, 0xc9}               },
    {0x197e670, 0x197e76b, 0x197e78e, {0x88, 0xc8}                     },
    {0x171f790, 0x171fab7, 0x171fadf, {0x41, 0x88, 0xc9}               },
    {0x25c0140, 0x25c09c7, 0x25c09f2, {0x89, 0xc3}                     },
    {0x2569f20, 0x256a388, 0x256a3b3, {0x89, 0xc3}                     },
    {0x2569f20, 0x256a834, 0x256a860, {0x89, 0xdf}                     },
    {0x2567090, 0x2567841, 0x2567867, {0x41, 0x89, 0xc6}               },
    {0x2582d50, 0x2582e80, 0x2582ea9, {0x89, 0xc3}                     },
    {0x25825e0, 0x25825f0, 0x258261b, {0x89, 0xd3}                     },
    {0x2d2ca90, 0x2d2ccb2, 0x2d2cce0, {0x41, 0x89, 0xc6}               },
    {0x2d2faf0, 0x2d2fc40, 0x2d2fc64, {0x89, 0xc7}                     },
    {0x2d3e700, 0x2d3e75e, 0x2d3e781, {0x88, 0xd8}                     },
    {0x2984bf0, 0x2984cb3, 0x2984cd6, {0x89, 0xc3}                     },
    {0x30e4420, 0x30e4432, 0x30e4460, {0x41, 0x89, 0xc3}               },
    {0x31f7d10, 0x31f7e35, 0x31f7e59, {0x89, 0xcb}                     },
    {0x31f8e10, 0x31f8e8e, 0x31f8eb3, {0x89, 0xdf}                     },
    {0x336cb80, 0x336cccd, 0x336ccf5, {0x40, 0x88, 0xc7}               },
    {0x25c22a0, 0x25c33dd, 0x25c340d, {0x41, 0x89, 0xc8}               },
    {0x25e02f0, 0x25e04bc, 0x25e04e8, {0x89, 0xc8}                     },
    {0x0b92a60, 0x0b92d10, 0x0b92d60, {0x89, 0x8e, 0x3c, 0x0, 0x0, 0x0}},
    {0x0cbde50, 0x0cbdfc9, 0x0cbdff5, {0x89, 0xcb}                     },
    {0x0d013f0, 0x0d01657, 0x0d01683, {0x89, 0xcf}                     },
    {0x10fef90, 0x10ff015, 0x10ff04d, {0x89, 0xc8}                     },
    {0x25824e0, 0x2582560, 0x25825c2, {0x89, 0xc8}                     },
    {0x25681d0, 0x25687a5, 0x25687ea, {0x89, 0x8e, 0xe0, 0xa, 0x0, 0x0}},
    {0x2d2c450, 0x2d2c5c1, 0x2d2c5ed, {0x89, 0xc8}                     },
    {0x2d2fcd0, 0x2d2fe6e, 0x2d2fe9a, {0x89, 0xc8}                     },
    {0x31ff0b0, 0x31ff164, 0x31ff190, {0x89, 0xc8}                     },
    {0x336e8e0, 0x336e998, 0x336e9c7, {}                               },
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
