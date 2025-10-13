#ifndef CPUID_DETECTOR_H
#define CPUID_DETECTOR_H

#include <string>

class CpuidDetector {
public:
    // Detecta si estamos en una VM usando el bit hypervisor de CPUID
    static bool detectVM();
    
    // Obtiene el vendor del hypervisor (si está disponible)
    static std::string getHypervisorVendor();
    
    // Resultado formateado para mostrar
    static std::string getResult();
    
    // Versión detallada con toda la información
    static std::string getDetailedInfo();

private:
    // Función wrapper portable para CPUID
    static void cpuid(int cpuInfo[4], int functionId);
    
    // Verifica si la función CPUID está disponible
    static bool isCpuidSupported();
};

#endif // CPUID_DETECTOR_H