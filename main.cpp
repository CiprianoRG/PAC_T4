#include <iostream>
#include <string>
#include "vm_detector.h"
#include "CpuidDetector.h"
#include "BiosDetector.h"
#include "MacDetector.h"

// MIRA MI CHAVO: Esta es la función principal, el corazón del programa
int main(int argc, char* argv[]) {
    // MIRA MI CHAVO: Configuramos los colores según el sistema operativo
    #ifdef _WIN32
        USE_COLORS = true;
    #else
        USE_COLORS = ISATTY(FILENO(stdout));
    #endif
    
    // MIRA MI CHAVO: Mostramos el banner bien padre
    printBanner();
    
    // MIRA MI CHAVO: Variables para controlar qué hace el programa
    bool verbose = false;
    bool testAll = true;
    int specificTechnique = -1;
    
    // MIRA MI CHAVO: Revisamos los argumentos que nos pasó el usuario
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--verbose" || arg == "-v") {
            verbose = true;
        } 
        else if (arg == "--all" || arg == "-a") {
            testAll = true;
        }
        else if (arg == "--technique" || arg == "-t") {
            if (i + 1 < argc) {
                specificTechnique = std::stoi(argv[++i]);
                testAll = false;
            } else {
                std::cerr << "Error: --technique necesita un numero\n";
                return 1;
            }
        }
        else if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        }
        else {
            std::cerr << "Argumento desconocido: " << arg << "\n";
            printUsage();
            return 1;
        }
    }
    
    // MIRA MI CHAVO: Aquí ejecutamos lo que el usuario pidió
    try {
        if (specificTechnique != -1) {
            testIndividualTechnique(specificTechnique, verbose);
        } 
        else if (testAll) {
            if (verbose) {
                // MIRA MI CHAVO: Modo detallado - mostramos todo con lujo de detalle
                testIndividualTechnique(1, true);
                testIndividualTechnique(2, true); 
                testIndividualTechnique(3, true);
            } else {
                // MIRA MI CHAVO: Modo normal - tabla resumida bien chida
                testAllTechniques();
            }
        }
        
        // MIRA MI CHAVO: Mostramos información final del sistema
        std::cout << "\n";
        printSectionHeader("INFORMACION DEL SISTEMA");

        #ifdef _WIN32
            std::cout << "Plataforma: Windows\n";
        #else
            std::cout << "Plataforma: Linux\n";
        #endif

        // MIRA MI CHAVO: Resumen de lo que se ejecutó
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
        // MIRA MI CHAVO: Si algo sale mal, mostramos el error en rojo
        setConsoleColor(31);
        std::cerr << "Error durante la deteccion: " << e.what() << "\n";
        resetConsoleColor();
        return 1;
    }
    
    // MIRA MI CHAVO: Si llegamos aquí, todo salió bien
    std::cout << "\n";
    setConsoleColor(32);
    std::cout << "Analisis completado\n";
    resetConsoleColor();
    
    return 0;
}