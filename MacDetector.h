#ifndef MAC_DETECTOR_H
#define MAC_DETECTOR_H

#include <string>
#include <vector>

class MacDetector {
public:
    static bool detectVM();
    static std::vector<std::string> getMACAddresses();
    static std::string getResult();
    static std::string getDetailedInfo();

private:
    static bool detectWindows();
    static bool detectLinux();
    static bool isVirtualMAC(const std::string& mac);
    static std::string normalizeMAC(const std::string& mac);
};

#endif // MAC_DETECTOR_H