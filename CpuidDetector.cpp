#include "CpuidDetector.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <intrin.h>
    #include <windows.h>
#else
    #include <cpuid.h>
    #include <cstdio>
#endif

void CpuidDetector::cpuid(int cpuInfo[4], int functionId) {
#ifdef _WIN32
    __cpuid(cpuInfo, functionId);
#else
    __cpuid(functionId, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#endif
}

bool CpuidDetector::isCpuidSupported() {
    // En sistemas modernos, CPUID siempre está disponible
    return true;
}

bool CpuidDetector::detectVM() {
    if (!isCpuidSupported()) {
        return false;
    }
    
    int cpuInfo[4] = {0, 0, 0, 0};
    
    try {
        // CPUID con EAX=1 - Processor Info and Features
        cpuid(cpuInfo, 1);
        
        // Bit 31 del registro ECX indica presencia de hypervisor
        // ECX está en cpuInfo[2]
        bool isVirtualized = (cpuInfo[2] & (1 << 31)) != 0;
        
        return isVirtualized;
        
    } catch (...) {
        // En caso de error en CPUID, asumimos no virtualizado
        return false;
    }
}

std::string CpuidDetector::getHypervisorVendor() {
    int cpuInfo[4] = {0, 0, 0, 0};
    
    // Intentar obtener vendor del hypervisor (Leaf 0x40000000)
    cpuid(cpuInfo, 0x40000000);
    
    // Los vendors conocidos usan este leaf
    if (cpuInfo[0] >= 0x40000000) {
        // cpuInfo[1], cpuInfo[2], cpuInfo[3] contienen la cadena del vendor
        char vendor[13] = {0};
        memcpy(vendor, &cpuInfo[1], 4);
        memcpy(vendor + 4, &cpuInfo[2], 4);
        memcpy(vendor + 8, &cpuInfo[3], 4);
        vendor[12] = '\0';
        
        return std::string(vendor);
    }
    
    return "Unknown";
}

std::string CpuidDetector::getResult() {
    bool vmDetected = detectVM();
    std::string vendor = getHypervisorVendor();
    
    if (vmDetected) {
        if (vendor != "Unknown") {
            return "VM Detectada [" + vendor + "]";
        } else {
            return "VM Detectada [Hypervisor Present]";
        }
    } else {
        return "No VM Detectada";
    }
}

std::string CpuidDetector::getDetailedInfo() {
    int cpuInfo[4] = {0, 0, 0, 0};
    std::string info = "=== CPUID Detection Details ===\n";
    
    // Información básica del CPU
    cpuid(cpuInfo, 0);
    int maxLeaf = cpuInfo[0];
    
    char vendor[13] = {0};
    memcpy(vendor, &cpuInfo[1], 4);
    memcpy(vendor + 4, &cpuInfo[2], 4);
    memcpy(vendor + 8, &cpuInfo[3], 4);
    
    info += "CPU Vendor: " + std::string(vendor) + "\n";
    info += "Max CPUID Leaf: 0x" + std::to_string(maxLeaf) + "\n";
    
    // Detección de hypervisor
    bool vmDetected = detectVM();
    info += "Hypervisor Bit (ECX[31]): " + std::string(vmDetected ? "1" : "0") + "\n";
    info += "VM Detection: " + std::string(vmDetected ? "POSITIVE" : "NEGATIVE") + "\n";
    
    // Información del hypervisor si está disponible
    if (vmDetected) {
        std::string hvVendor = getHypervisorVendor();
        info += "Hypervisor Vendor: " + hvVendor + "\n";
        
        // Información adicional del hypervisor
        if (hvVendor != "Unknown") {
            cpuid(cpuInfo, 0x40000001);
            info += "Hypervisor Version: 0x" + std::to_string(cpuInfo[0]) + "\n";
        }
    }
    
    return info;
}