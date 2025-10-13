#include "MacDetector.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <fstream>
    #include <dirent.h>
    #include <cstring>
#endif

using namespace std;

string MacDetector::normalizeMAC(const string& mac) {
    string normalized;
    for (char c : mac) {
        if (isxdigit(c)) {
            normalized += toupper(c);
        }
    }
    return normalized;
}

bool MacDetector::isVirtualMAC(const string& mac) {
    string normalized = normalizeMAC(mac);
    
    // OUI de fabricantes de virtualización
    vector<string> virtualOUIs = {
        "000C29", // VMware
        "005056", // VMware
        "000569", // VMware
        "080027", // VirtualBox
        "00155D", // Hyper-V
        "00163E", // Xen
        "001C42", // Parallels
        "0003FF", // Microsoft Hyper-V
        "001C14", // VMware (nuevo)
        "000F4B"  // VirtualBox (alternativo)
    };
    
    if (normalized.length() >= 6) {
        string oui = normalized.substr(0, 6);
        for (const auto& virtualOUI : virtualOUIs) {
            if (oui == virtualOUI) {
                return true;
            }
        }
    }
    
    return false;
}

#ifdef _WIN32
bool MacDetector::detectWindows() {
    PIP_ADAPTER_INFO adapterInfo;
    PIP_ADAPTER_INFO adapter;
    ULONG bufferSize = sizeof(IP_ADAPTER_INFO);
    
    // Primera llamada para obtener el tamaño del buffer
    adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);
    if (adapterInfo == NULL) {
        return false;
    }
    
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
        free(adapterInfo);
        adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);
        if (adapterInfo == NULL) {
            return false;
        }
    }
    
    bool vmDetected = false;
    
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == NO_ERROR) {
        adapter = adapterInfo;
        while (adapter) {
            if (adapter->AddressLength == 6) { // MAC address tiene 6 bytes
                string mac;
                char macBuffer[18];
                snprintf(macBuffer, sizeof(macBuffer), 
                        "%02X:%02X:%02X:%02X:%02X:%02X",
                        adapter->Address[0], adapter->Address[1],
                        adapter->Address[2], adapter->Address[3],
                        adapter->Address[4], adapter->Address[5]);
                mac = macBuffer;
                
                if (isVirtualMAC(mac)) {
                    vmDetected = true;
                    break;
                }
            }
            adapter = adapter->Next;
        }
    }
    
    free(adapterInfo);
    return vmDetected;
}

vector<string> MacDetector::getMACAddresses() {
    vector<string> macAddresses;
    PIP_ADAPTER_INFO adapterInfo;
    PIP_ADAPTER_INFO adapter;
    ULONG bufferSize = sizeof(IP_ADAPTER_INFO);
    
    adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);
    if (adapterInfo == NULL) {
        return macAddresses;
    }
    
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
        free(adapterInfo);
        adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);
        if (adapterInfo == NULL) {
            return macAddresses;
        }
    }
    
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == NO_ERROR) {
        adapter = adapterInfo;
        while (adapter) {
            if (adapter->AddressLength == 6) {
                char macBuffer[18];
                snprintf(macBuffer, sizeof(macBuffer), 
                        "%02X:%02X:%02X:%02X:%02X:%02X",
                        adapter->Address[0], adapter->Address[1],
                        adapter->Address[2], adapter->Address[3],
                        adapter->Address[4], adapter->Address[5]);
                macAddresses.push_back(string(macBuffer));
            }
            adapter = adapter->Next;
        }
    }
    
    free(adapterInfo);
    return macAddresses;
}
#else
bool MacDetector::detectLinux() {
    vector<string> macs = getMACAddresses();
    for (const auto& mac : macs) {
        if (isVirtualMAC(mac)) {
            return true;
        }
    }
    return false;
}

vector<string> MacDetector::getMACAddresses() {
    vector<string> macAddresses;
    
    // Leer archivos de interfaz de red en /sys/class/net/
    string netPath = "/sys/class/net/";
    DIR* dir = opendir(netPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') { // Ignorar . y ..
                string interfacePath = netPath + entry->d_name + "/address";
                ifstream macFile(interfacePath);
                if (macFile.is_open()) {
                    string mac;
                    getline(macFile, mac);
                    if (!mac.empty()) {
                        macAddresses.push_back(mac);
                    }
                    macFile.close();
                }
            }
        }
        closedir(dir);
    }
    
    return macAddresses;
}
#endif

bool MacDetector::detectVM() {
#ifdef _WIN32
    return detectWindows();
#else
    return detectLinux();
#endif
}

string MacDetector::getResult() {
    bool vmDetected = detectVM();
    vector<string> macs = getMACAddresses();
    
    if (vmDetected) {
        // Encontrar cuál MAC es virtual
        for (const auto& mac : macs) {
            if (isVirtualMAC(mac)) {
                return "VM Detectada [MAC: " + mac + "]";
            }
        }
        return "VM Detectada [Virtual MAC found]";
    } else {
        string macList;
        for (size_t i = 0; i < macs.size(); ++i) {
            if (i > 0) macList += ", ";
            macList += macs[i];
        }
        return "No VM Detectada [MACs: " + macList + "]";
    }
}

string MacDetector::getDetailedInfo() {
    string info = "=== MAC Address Detection Details ===\n";
    
    vector<string> macs = getMACAddresses();
    info += "Found " + to_string(macs.size()) + " network interfaces:\n";
    
    bool vmDetected = false;
    for (const auto& mac : macs) {
        bool isVirtual = isVirtualMAC(mac);
        info += "  " + mac + " -> " + (isVirtual ? "VIRTUAL" : "PHYSICAL") + "\n";
        if (isVirtual) {
            vmDetected = true;
        }
    }
    
    info += "VM Detection: " + string(vmDetected ? "POSITIVE" : "NEGATIVE") + "\n";
    
    return info;
}