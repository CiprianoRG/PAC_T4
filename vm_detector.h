#ifndef VM_DETECTOR_H
#define VM_DETECTOR_H
// MIRA MI CHAVO: Declaramos USE_COLORS como extern para que todos la vean
extern bool USE_COLORS;
// Aquí definimos todas las funciones que vamos a usar
void printBanner();
void printSectionHeader(const std::string& title);
void printColoredText(const std::string& text, int color);
void setConsoleColor(int color);
void resetConsoleColor();
void testAllTechniques();
void testIndividualTechnique(int technique, bool verbose = false);
void printUsage();
void printDetectionTable();

#endif