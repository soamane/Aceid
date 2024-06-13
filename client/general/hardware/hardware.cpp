#include "hardware.h"

#include <d3d9.h>
#include <sstream>
#include <intrin.h>

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/protect/xorstring/xorstring.h"

const std::string Hardware::GetHardwareId() {
    const std::string cpuInfo = GetCPUInfo();
    if (cpuInfo.empty()) {
        throw std::runtime_error(xorstr_("Failed to initalize hardware: 1"));
    }

    const std::string gpuInfo = GetGPUInfo();
    if (gpuInfo.empty()) {
        throw std::runtime_error(xorstr_("Failed to initalize hardware: 2"));
    }

    return DataEncryption::EncryptBase64(cpuInfo + gpuInfo);
}

const std::string Hardware::GetCPUInfo() {
    std::uint32_t regs[4]{ };
    __cpuid(reinterpret_cast<int*>(regs), 0);

    std::ostringstream oss;
    oss << regs[0]
        << regs[1]
        << regs[2]
        << regs[3];

    return oss.str();
}

const std::string Hardware::GetGPUInfo() {
    IDirect3D9* directDevice = Direct3DCreate9(D3D_SDK_VERSION);
    if (directDevice == nullptr) {
        throw std::runtime_error(xorstr_("Failed to initalize hardware: 3"));
    }

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    if (FAILED(directDevice->GetAdapterIdentifier(0, 0, &adapterIdentifier))) {
        throw std::runtime_error(xorstr_("Failed to initalize hardware: 4"));
    }

    std::ostringstream oss;
    oss << adapterIdentifier.Description;
    oss << adapterIdentifier.DeviceIdentifier.Data1
        << adapterIdentifier.DeviceIdentifier.Data2
        << adapterIdentifier.DeviceIdentifier.Data3;

    return oss.str();
}
