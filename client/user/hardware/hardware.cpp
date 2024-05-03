#include "hardware.h"

#include <intrin.h>
#include <sstream>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "../../general/protect/dataencryption/dataencryption.h"

std::string Hardware::getHardwareId() {
    std::string cpuInfo = getCPUInfo();
    if (cpuInfo.empty()) {
        return std::string();
    }

    std::string gpuInfo = getGPUInfo();
    if (gpuInfo.empty()) {
        return std::string();
    }

    return DataEncryption::encryptBase64(cpuInfo + gpuInfo);
}

std::string Hardware::getCPUInfo() {
    std::uint32_t regs[4]{ };
    __cpuid(reinterpret_cast<int*>(regs), 0);

    std::ostringstream oss;
    oss << regs[0]
        << regs[1]
        << regs[2]
        << regs[3];

    return oss.str();
}

std::string Hardware::getGPUInfo() {
    IDirect3D9* directDevice = Direct3DCreate9(D3D_SDK_VERSION);
    if (directDevice == nullptr) {
        return std::string();
    }

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    if (FAILED(directDevice->GetAdapterIdentifier(0, 0, &adapterIdentifier))) {
        return std::string();
    }

    std::ostringstream oss;
    oss << adapterIdentifier.Description;
    oss << adapterIdentifier.DeviceIdentifier.Data1
        << adapterIdentifier.DeviceIdentifier.Data2
        << adapterIdentifier.DeviceIdentifier.Data3;

    return oss.str();
}
