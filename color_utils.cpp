#include <iostream>
#include <string>
#include "vm_detector.h"

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define ISATTY _isatty
    #define FILENO _fileno
#else
    #include <unistd.h>
    #define ISATTY isatty
    #define FILENO fileno
#endif

// MIRA MI CHAVO: Aquí definimos USE_COLORS para que todos la puedan usar
bool USE_COLORS = true;

// MIRA MI CHAVO: Esta función le dice a Windows o Linux qué color usar
void setConsoleColor(int color) {
    if (!USE_COLORS) return;
    
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD winColor = 7;
        
        switch(color) {
            case 31: winColor = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case 32: winColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case 33: winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case 34: winColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case 35: winColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case 36: winColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case 37: winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            default: winColor = 7;
        }
        SetConsoleTextAttribute(hConsole, winColor);
    #else
        std::cout << "\033[1;" << color << "m";
    #endif
}

// MIRA MI CHAVO: Esta función regresa los colores a lo normal
void resetConsoleColor() {
    if (!USE_COLORS) return;
    
    #ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    #else
        std::cout << "\033[0m";
    #endif
}

// MIRA MI CHAVO: Imprime texto con color y luego lo regresa a normal
void printColoredText(const std::string& text, int color) {
    setConsoleColor(color);
    std::cout << text;
    resetConsoleColor();
}