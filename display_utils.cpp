#include <iostream>
#include <string>
#include <iomanip>
#include "vm_detector.h"
#include "CpuidDetector.h"
#include "BiosDetector.h"
#include "MacDetector.h"

// MIRA MI CHAVO: Esta función muestra el banner principal bien padre
void printBanner() {
    setConsoleColor(36);
    std::cout << "=========================================\n";
    std::cout << "    DETECTOR DE VM - MULTI-TECNICA\n";
    std::cout << "    Ciberseguridad Anti-Virtualizacion\n";
    std::cout << "=========================================\n";
    resetConsoleColor();
}

// MIRA MI CHAVO: Crea secciones con títulos bien marcados
void printSectionHeader(const std::string& title) {
    setConsoleColor(33);
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(50, '=') << "\n";
    resetConsoleColor();
}

// MIRA MI CHAVO: Función para recortar texto muy largo y poner puntos suspensivos
std::string truncateText(const std::string& text, int maxLength) {
    if (text.length() <= maxLength) {
        return text;
    }
    return text.substr(0, maxLength - 3) + "...";
}

// MIRA MI CHAVO: Esta función hace una tabla bien chida con los resultados
void printDetectionTable() {
    std::cout << "\n";
    printSectionHeader("TABLA DE RESULTADOS DE DETECCION");
    
    // MIRA MI CHAVO: Aquí creamos la cabecera de la tabla más ancha
    setConsoleColor(36);
    std::cout << "+--------------+------------------------------+-----------------+\n";
    std::cout << "|   Tecnica    |          Resultado           |     Estado      |\n";
    std::cout << "+--------------+------------------------------+-----------------+\n";
    resetConsoleColor();
    
    // MIRA MI CHAVO: Llenamos la tabla con cada técnica
    std::string techniques[] = {"CPUID", "BIOS Vendor", "MAC Address"};
    std::string results[3];
    std::string statuses[3];
    
    // CPUID
    results[0] = CpuidDetector::getResult();
    statuses[0] = CpuidDetector::detectVM() ? "VIRTUAL" : "FISICA";
    
    // BIOS
    results[1] = BiosDetector::getResult();
    statuses[1] = BiosDetector::detectVM() ? "VIRTUAL" : "FISICA";

    // MAC
    results[2] = MacDetector::getResult();
    statuses[2] = MacDetector::detectVM() ? "VIRTUAL" : "FISICA";

    // MIRA MI CHAVO: Imprimimos cada fila de la tabla con texto recortado si es necesario
    for (int i = 0; i < 3; i++) {
        std::string resultTruncated = truncateText(results[i], 28);
        
        std::cout << "| " << std::setw(12) << std::left << techniques[i] << " | "
                  << std::setw(28) << std::left << resultTruncated << " | "
                  << std::setw(15) << std::left << statuses[i] << " |\n";
    }
    
    setConsoleColor(36);
    std::cout << "+--------------+------------------------------+-----------------+\n";
    resetConsoleColor();
    
    // MIRA MI CHAVO: Mostramos los resultados completos debajo de la tabla
    std::cout << "\nRESULTADOS COMPLETOS:\n";
    for (int i = 0; i < 3; i++) {
        std::cout << techniques[i] << ": " << results[i] << "\n";
    }
}

// MIRA MI CHAVO: Esta función muestra el resumen final bien resumido
void testAllTechniques() {
    printDetectionTable();
    
    std::cout << "\n";
    printSectionHeader("ANALISIS FINAL");
    
    // MIRA MI CHAVO: Contamos cuántas técnicas detectaron VM
    bool cpuidVM = CpuidDetector::detectVM();
    bool biosVM = BiosDetector::detectVM();
    bool macVM = MacDetector::detectVM();
    
    int positiveCount = (cpuidVM ? 1 : 0) + (biosVM ? 1 : 0) + (macVM ? 1 : 0);
    
    setConsoleColor(35);
    std::cout << "RESUMEN: ";
    
    if (positiveCount == 3) {
        std::cout << "EVIDENCIA FUERTE DE VM (3/3 tecnicas positivas)";
    } else if (positiveCount >= 2) {
        std::cout << "ALTA PROBABILIDAD DE VM (" << positiveCount << "/3 tecnicas positivas)";
    } else if (positiveCount == 1) {
        std::cout << "POSIBLE VM (1/3 tecnicas positiva) - Verificar manualmente";
    } else {
        std::cout << "PROBABLE MAQUINA FISICA";
    }
    
    resetConsoleColor();
    std::cout << "\n" << std::string(50, '-') << "\n";
}

// MIRA MI CHAVO: Para probar una técnica individual si el usuario quiere
void testIndividualTechnique(int technique, bool verbose) {
    switch(technique) {
        case 1:
            printSectionHeader("PRUEBA INDIVIDUAL: CPUID");
            std::cout << "Resultado: " << CpuidDetector::getResult() << "\n";
            if (verbose) {
                std::cout << "\n" << CpuidDetector::getDetailedInfo() << "\n";
            }
            break;
            
        case 2:
            printSectionHeader("PRUEBA INDIVIDUAL: BIOS VENDOR");
            std::cout << "Resultado: " << BiosDetector::getResult() << "\n";
            if (verbose) {
                std::cout << "\n" << BiosDetector::getDetailedInfo() << "\n";
            }
            break;
            
        case 3:
            printSectionHeader("PRUEBA INDIVIDUAL: DIRECCION MAC");
            std::cout << "Resultado: " << MacDetector::getResult() << "\n";
            if (verbose) {
                std::cout << "\n" << MacDetector::getDetailedInfo() << "\n";
            }
            break;
            
        default:
            std::cout << "ERROR: Tecnica desconocida: " << technique << "\n";
    }
}

// MIRA MI CHAVO: Esta función le explica al usuario cómo usar el programa
void printUsage() {
    setConsoleColor(36);
    std::cout << "\nCOMO USAR EL DETECTOR:\n";
    std::cout << "  vm_detector                    - Probar todas las tecnicas (modo tabla)\n";
    std::cout << "  vm_detector --all             - Igual que arriba\n";
    std::cout << "  vm_detector --verbose         - Modo detallado con toda la info\n";
    std::cout << "  vm_detector --technique N     - Probar tecnica especifica (1-3)\n";
    std::cout << "  vm_detector --help            - Mostrar esta ayuda\n";
    
    std::cout << "\nTECNICAS DE DETECCION:\n";
    std::cout << "  1 - CPUID (Hypervisor bit) - Nivel hardware\n";
    std::cout << "  2 - BIOS Vendor - Nivel firmware\n";  
    std::cout << "  3 - MAC Address OUI - Nivel red\n";
    
    std::cout << "\nTIP: Usa --verbose para ver informacion tecnica detallada\n";
    resetConsoleColor();
}