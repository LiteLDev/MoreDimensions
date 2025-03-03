#include <Windows.h>
#include <iostream>
#include <vector>

namespace {
struct AddressAndReg {
    DWORD                   func_rva_start;
    DWORD                   func_rva_end;
    const std::vector<BYTE> native_code;
};

std::vector<AddressAndReg> data = {
    {0x082BA6F, 0x082BA9A, {0x89, 0xC8}      },
    {0x26D1CFB, 0x26D1D1F, {0x89, 0xC3}      },
    {0x10F0CD,  0x10F0D00, {0x89, 0xC2}      },
    {0x10FFF2E, 0x10FFF57, {0x89, 0xC2}      },
    {0x1107480, 0x11074A9, {0x89, 0xC2}      },
    {0x1108365, 0x110838E, {0x89, 0xC2}      },
    {0x121CE22, 0x121CE4B, {0x89, 0xC3}      },
    {0x123B4A3, 0x123B4C6, {0x89, 0xC3}      },
    {0x124926F, 0x1249298, {0x89, 0xC2}      },
    {0x1262064, 0x126208D, {0x89, 0xC2}      },
    {0x1B0D610, 0x1B0D638, {0x41, 0x88, 0xC9}},
    {0x1B0D8DB, 0x1B0D8FE, {0x88, 0xC8}      },
    {0x1D8677C, 0x1D867A4, {0x41, 0x88, 0xC9}},
    {0x27A6E57, 0x27A6E82, {0x89, 0xC3}      },
    {0x2803614, 0x280363F, {0x89, 0xC3}      },
    {0x2803ABD, 0x2803AE9, {0x89, 0xDF}      },
    {0x280089F, 0x28008C5, {0x41, 0x89, 0xC6}},
    {0x281BE40, 0x281BE69, {0x89, 0xC3}      },
    {0x281B5B0, 0x281B5DB, {0x89, 0xD3}      },
    {0x2B45EA2, 0x2B45ED0, {0x41, 0x89, 0xC6}},
    {0x2B48EA0, 0x2B48EC4, {0x89, 0xC7}      },
    {0x2B502EE, 0x2B50311, {0x88, 0xD8}      },
    {0x2AA89D3, 0x2AA89F6, {0x89, 0xC3}      },
    {0x31078C2, 0x31078F0, {0x41, 0x89, 0xC3}},
    {0x31D6A15, 0x31D6A39, {0x89, 0xCB}      },
    {0x31D7A6E, 0x31D7A93, {0x89, 0xDF}      },
    {0x337435D, 0x3374385, {0x40, 0x88, 0xC7}},
};

bool PatchFunction(
    HMODULE hModule,
    DWORD              faddress_start,
    DWORD              faddress_end,
    const std::vector<BYTE>& native_code
) {
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
    if (!WriteProcessMemory(hProcess, patchAddress, patch.data(), patch.size(), &bytesWritten) || bytesWritten != patch.size()) {
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

} // namespace


void injectNaticeCode(){
    // Calculate the address in the target process
    HMODULE hModule = GetModuleHandle(L"bedrock_server_mod.exe");
    if (!hModule) {
        std::cerr << "Failed to get module handle for bedrock_server_mod.exe" << std::endl;
        return;
    }
    for(auto& item: data) {
        PatchFunction(hModule, item.func_rva_start, item.func_rva_end, item.native_code);
    };
};