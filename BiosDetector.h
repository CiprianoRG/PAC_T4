#ifndef BIOS_DETECTOR_H
#define BIOS_DETECTOR_H

#include <string>

class BiosDetector {
public:
    static bool detectVM();
    static std::string getBiosVendor();
    static std::string getProductName();
    static std::string getResult();
    static std::string getDetailedInfo();

private:
    static bool detectWindows();
    static bool detectLinux();
    static bool containsVirtualizationKeywords(const std::string& text);
    static std::string toLower(const std::string& str);
};

#endif