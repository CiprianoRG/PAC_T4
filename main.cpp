//cl /EHsc /std:c++17 main.cpp CpuidDetector.cpp BiosDetector.cpp MacDetector.cpp advapi32.lib iphlpapi.lib ws2_32.lib
#include <iostream>
#include <string>
#include "CpuidDetector.h"    // Detección por CPUID
#include "BiosDetector.h"     // Detección por BIOS Vendor
#include "MacDetector.h"      // Detección por Dirección MAC

#ifdef _WIN32
    #include <windows.h>
    
    /**
     * @brief Establece el color del texto en la consola de Windows
     * @param color Código de color a aplicar
     */
    void setConsoleColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    
    /**
     * @brief Restablece el color de la consola al valor por defecto
     */
    void resetConsoleColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
#else
    /**
     * @brief Establece el color del texto en terminales Linux/Unix
     * @param color Código de color ANSI
     */
    void setConsoleColor(int color) {
        // Códigos de escape ANSI para colores en Linux
        std::cout << "\033[1;" << color << "m";
    }
    
    /**
     * @brief Restablece el color del texto al valor por defecto
     */
    void resetConsoleColor() {
        std::cout << "\033[0m";
    }
#endif

/**
 * @brief Imprime el banner de presentación del programa
 * 
 * Muestra el nombre del proyecto, propósito y información básica
 * en color cyan para una presentación visual atractiva.
 */
void printBanner() {
    setConsoleColor(36); // Cyan para el banner
    std::cout << "=========================================\n";
    std::cout << "    DETECTOR DE VM - Multi-Tecnica\n";
    std::cout << "    Ciberseguridad - Deteccion Anti-VM\n";
    std::cout << "=========================================\n";
    resetConsoleColor();
}

/**
 * @brief Imprime el resultado de la detección con colores
 * @param isVM true si se detectó máquina virtual, false si no
 * 
 * - Rojo: Máquina virtual detectada (alerta)
 * - Verde: No se detectó máquina virtual (seguro)
 */
void printResult(bool isVM) {
    if (isVM) {
        setConsoleColor(31); // Rojo para alerta de VM detectada
        std::cout << "MAQUINA VIRTUAL DETECTADA!\n";
        resetConsoleColor();
    } else {
        setConsoleColor(32); // Verde para estado seguro
        std::cout << "No se detecto maquina virtual\n";
        resetConsoleColor();
    }
}

/**
 * @brief Imprime un encabezado de sección con formato
 * @param title Título de la sección a mostrar
 * 
 * Crea una sección visualmente separada con el título centrado
 * entre líneas de separación, usando color amarillo.
 */
void printSectionHeader(const std::string& title) {
    setConsoleColor(33); // Amarillo para encabezados de sección
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(40, '=') << "\n";
    resetConsoleColor();
}

/**
 * @brief Ejecuta y muestra resultados de las 3 técnicas de detección
 * 
 * Esta función:
 * 1. Ejecuta cada técnica de detección (CPUID, BIOS, MAC)
 * 2. Muestra los resultados individuales de cada técnica
 * 3. Calcula y muestra un resumen general basado en los resultados
 * 4. Evalúa la confiabilidad basada en la consistencia entre técnicas
 */
void testAllTechniques() {
    std::cout << "\n";
    printSectionHeader("RESULTADOS COMPLETOS DE DETECCION");
    
    // ================================
    // 1. TÉCNICA CPUID - Nivel Hardware
    // ================================
    std::cout << "\n1. TECNICA CPUID:\n";
    std::cout << "   Resultado: " << CpuidDetector::getResult() << "\n";
    // Esta técnica verifica el bit de hypervisor en el registro CPUID
    // Es muy confiable pero puede detectar hipervisores en máquinas físicas
    
    // ================================
    // 2. TÉCNICA BIOS - Nivel Firmware
    // ================================
    std::cout << "\n2. TECNICA BIOS VENDOR:\n";
    std::cout << "   Resultado: " << BiosDetector::getResult() << "\n";
    // Verifica las cadenas del vendor del BIOS en busca de fabricantes de VMs
    // Muy efectiva para VMs puras pero no detecta virtualización parcial
    
    // ================================
    // 3. TÉCNICA MAC - Nivel Red
    // ================================
    std::cout << "\n3. TECNICA DIRECCION MAC:\n";
    std::cout << "   Resultado: " << MacDetector::getResult() << "\n";
    // Busca direcciones MAC de fabricantes de virtualización
    // Rápida pero fácil de evadir cambiando la dirección MAC
    
    // ================================
    // RESUMEN Y EVALUACIÓN DE CONFIANZA
    // ================================
    std::cout << "\n" << std::string(50, '=') << "\n";
    setConsoleColor(35); // Magenta para el resumen
    
    std::cout << "RESUMEN: ";
    
    // Obtener resultados individuales de cada técnica
    bool cpuidVM = CpuidDetector::detectVM();
    bool biosVM = BiosDetector::detectVM();
    bool macVM = MacDetector::detectVM();
    
    // Contar cuántas técnicas detectaron VM
    int positiveCount = (cpuidVM ? 1 : 0) + (biosVM ? 1 : 0) + (macVM ? 1 : 0);
    
    // Evaluar la confiabilidad basada en la consistencia de resultados
    if (positiveCount == 3) {
        // Las 3 técnicas coinciden - evidencia muy fuerte
        std::cout << "EVIDENCIA FUERTE DE VM (3/3 tecnicas positivas)";
    } else if (positiveCount >= 2) {
        // Mayoría de técnicas detecta VM - alta probabilidad
        std::cout << "ALTA PROBABILIDAD DE VM (" << positiveCount << "/3 tecnicas positivas)";
    } else if (positiveCount == 1) {
        // Solo una técnica detecta - posible pero necesita verificación
        std::cout << "POSIBLE VM (1/3 tecnicas positiva)";
    } else {
        // Ninguna técnica detecta - probable máquina física
        std::cout << "PROBABLE MAQUINA FISICA";
    }
    
    resetConsoleColor();
    std::cout << "\n" << std::string(50, '=') << "\n";
}

/**
 * @brief Prueba una técnica específica de detección
 * @param technique Número de la técnica a probar (1=CPUID, 2=BIOS, 3=MAC)
 * @param verbose Si es true, muestra información detallada de la técnica
 * 
 * Permite probar técnicas individualmente para debugging o análisis específico
 */
void testIndividualTechnique(int technique, bool verbose = false) {
    switch(technique) {
        case 1: // Técnica CPUID - Detección a nivel de procesador
            printSectionHeader("PRUEBA DE TECNICA CPUID");
            std::cout << "Resultado: " << CpuidDetector::getResult() << "\n";
            if (verbose) {
                // Información detallada: vendor, bits de hypervisor, etc.
                std::cout << "\n" << CpuidDetector::getDetailedInfo() << "\n";
            }
            break;
            
        case 2: // Técnica BIOS - Detección por vendor del firmware
            printSectionHeader("PRUEBA DE TECNICA BIOS VENDOR");
            std::cout << "Resultado: " << BiosDetector::getResult() << "\n";
            if (verbose) {
                // Información detallada: vendor, producto, palabras clave encontradas
                std::cout << "\n" << BiosDetector::getDetailedInfo() << "\n";
            }
            break;
            
        case 3: // Técnica MAC - Detección por direcciones de red
            printSectionHeader("PRUEBA DE TECNICA DIRECCION MAC");
            std::cout << "Resultado: " << MacDetector::getResult() << "\n";
            if (verbose) {
                // Información detallada: todas las MACs encontradas y su tipo
                std::cout << "\n" << MacDetector::getDetailedInfo() << "\n";
            }
            break;
            
        default:
            // Manejo de error para técnica no válida
            std::cout << "ERROR: Tecnica desconocida: " << technique << "\n";
            std::cout << "Tecnicas disponibles: 1=CPUID, 2=BIOS, 3=MAC\n";
    }
}

/**
 * @brief Muestra la ayuda de uso del programa
 * 
 * Explica los parámetros disponibles y cómo usar el detector
 */
void printUsage() {
    setConsoleColor(36); // Cyan para la ayuda
    std::cout << "\nUso:\n";
    std::cout << "  " << "vm_detector                    - Probar todas las tecnicas\n";
    std::cout << "  " << "vm_detector --all             - Probar todas las tecnicas\n";
    std::cout << "  " << "vm_detector --verbose         - Salida detallada\n";
    std::cout << "  " << "vm_detector --technique N     - Probar tecnica especifica\n";
    std::cout << "  " << "vm_detector --help            - Mostrar esta ayuda\n";
    std::cout << "\nTecnicas disponibles:\n";
    std::cout << "  1 - Bit Hypervisor de CPUID (deteccion a nivel hardware)\n";
    std::cout << "  2 - Cadena Vendor del BIOS (deteccion a nivel firmware)\n";
    std::cout << "  3 - OUI de Direccion MAC (deteccion a nivel red)\n";  
    resetConsoleColor();
}

/**
 * @brief Función principal del programa
 * @param argc Número de argumentos de línea de comandos
 * @param argv Array de argumentos de línea de comandos
 * @return int Código de salida (0=éxito, 1=error)
 * 
 * Flujo principal:
 * 1. Parsear argumentos de línea de comandos
 * 2. Ejecutar técnicas según los parámetros
 * 3. Mostrar resultados
 * 4. Manejar errores
 */
int main(int argc, char* argv[]) {
    // Mostrar banner de presentación
    printBanner();
    
    // Variables para controlar el comportamiento del programa
    bool verbose = false;        // Modo detallado
    bool testAll = true;         // Probar todas las técnicas por defecto
    int specificTechnique = -1;  // Técnica específica a probar (-1 = todas)
    
    // ================================
    // PARSEO DE ARGUMENTOS DE LÍNEA DE COMANDOS
    // ================================
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--verbose" || arg == "-v") {
            verbose = true;  // Activar salida detallada
        } 
        else if (arg == "--all" || arg == "-a") {
            testAll = true;  // Probar todas las técnicas
        }
        else if (arg == "--technique" || arg == "-t") {
            // Probar técnica específica: --technique 1 (CPUID)
            if (i + 1 < argc) {
                specificTechnique = std::stoi(argv[++i]);
                testAll = false;  // No probar todas, solo la específica
            } else {
                std::cerr << "Error: --technique requiere un numero\n";
                return 1;
            }
        }
        else if (arg == "--help" || arg == "-h") {
            // Mostrar ayuda y salir
            printUsage();
            return 0;
        }
        else {
            // Argumento no reconocido
            std::cerr << "Argumento desconocido: " << arg << "\n";
            printUsage();
            return 1;
        }
    }
    
    // ================================
    // EJECUCIÓN DE LAS TÉCNICAS DE DETECCIÓN
    // ================================
    try {
        if (specificTechnique != -1) {
            // Modo: Técnica específica
            testIndividualTechnique(specificTechnique, verbose);
        } 
        else if (testAll) {
            // Modo: Todas las técnicas
            if (verbose) {
                // Con salida detallada: mostrar info completa de cada técnica
                testIndividualTechnique(1, true);  // CPUID con verbose
                testIndividualTechnique(2, true);  // BIOS con verbose
                testIndividualTechnique(3, true);  // MAC con verbose
            } else {
                // Sin verbose: solo resumen general
                testAllTechniques();
            }
        }
        
       // ================================
        // INFORMACIÓN FINAL DEL SISTEMA
        // ================================
        std::cout << "\n";
        printSectionHeader("INFORMACION DEL SISTEMA");

        // Información básica del sistema
        #ifdef _WIN32
            std::cout << "Plataforma: Windows\n";
        #else
            std::cout << "Plataforma: Linux\n";
        #endif

        // Resumen de ejecución
        if (specificTechnique != -1) {
            std::cout << "Tecnica probada: ";
            switch(specificTechnique) {
                case 1: std::cout << "CPUID"; break;
                case 2: std::cout << "BIOS"; break;
                case 3: std::cout << "MAC"; break;
            }
            std::cout << " | Modo: " << (verbose ? "Detallado" : "Normal") << "\n";
        } else {
            std::cout << "Tecnicas: Todas | Modo: " << (verbose ? "Detallado" : "Resumen") << "\n";
        }
    } catch (const std::exception& e) {
        // Manejo de errores durante la detección
        setConsoleColor(31); // Rojo para errores
        std::cerr << "Error durante la deteccion: " << e.what() << "\n";
        resetConsoleColor();
        return 1; // Código de error
    }
    
    // Éxito - programa terminó correctamente
    return 0;
}