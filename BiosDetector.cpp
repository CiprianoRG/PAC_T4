#include "BiosDetector.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #pragma comment(lib, "advapi32.lib")
#endif

using namespace std;

// Convertir a minúsculas para comparación case-insensitive
string BiosDetector::toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

bool BiosDetector::containsVirtualizationKeywords(const string& text) {
    string lowerText = toLower(text);
    
    vector<string> keywords = {
        "vmware", "virtualbox", "vbox", 
        "qemu", "bochs", "xen",
        "virtual", "hyper-v", "microsoft hv",
        "innotek", "parallels", "kvm"
    };
    
    for (const auto& keyword : keywords) {
        if (lowerText.find(keyword) != string::npos) {
            return true;
        }
    }
    
    return false;
}

#ifdef _WIN32
bool BiosDetector::detectWindows() {
    HKEY hKey;
    LONG result;
    char biosData[255];
    DWORD bufferSize = sizeof(biosData);
    
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                         "HARDWARE\\DESCRIPTION\\System\\BIOS", 
                         0, KEY_READ, &hKey);
    
    if (result == ERROR_SUCCESS) {
        // Leer BIOS Vendor
        bufferSize = sizeof(biosData);
        result = RegQueryValueExA(hKey, "BIOSVendor", NULL, NULL, 
                                (LPBYTE)biosData, &bufferSize);
        
        if (result == ERROR_SUCCESS) {
            string vendor(biosData);
            if (containsVirtualizationKeywords(vendor)) {
                RegCloseKey(hKey);
                return true;
            }
        }
        
        // Leer Product Name
        bufferSize = sizeof(biosData);
        result = RegQueryValueExA(hKey, "SystemProductName", NULL, NULL, 
                                (LPBYTE)biosData, &bufferSize);
        
        if (result == ERROR_SUCCESS) {
            string product(biosData);
            if (containsVirtualizationKeywords(product)) {
                RegCloseKey(hKey);
                return true;
            }
        }
        
        RegCloseKey(hKey);
    }
    
    return false;
}
#else
bool BiosDetector::detectLinux() {
    vector<string> dmiFiles = {
        "/sys/class/dmi/id/bios_vendor",
        "/sys/class/dmi/id/product_name", 
        "/sys/class/dmi/id/sys_vendor"
    };
    
    for (const auto& filePath : dmiFiles) {
        ifstream file(filePath);
        if (file.is_open()) {
            string content;
            getline(file, content);
            file.close();
            
            if (!content.empty() && containsVirtualizationKeywords(content)) {
                return true;
            }
        }
    }
    
    return false;
}
#endif

bool BiosDetector::detectVM() {
#ifdef _WIN32
    return detectWindows();
#else
    return detectLinux();
#endif
}

string BiosDetector::getBiosVendor() {
#ifdef _WIN32
    HKEY hKey;
    char vendorData[255];
    DWORD bufferSize = sizeof(vendorData);
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                    "HARDWARE\\DESCRIPTION\\System\\BIOS", 
                    0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        if (RegQueryValueExA(hKey, "BIOSVendor", NULL, NULL, 
                           (LPBYTE)vendorData, &bufferSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return string(vendorData);
        }
        RegCloseKey(hKey);
    }
    return "Unknown";
#else
    ifstream vendorFile("/sys/class/dmi/id/bios_vendor");
    if (vendorFile.is_open()) {
        string vendor;
        getline(vendorFile, vendor);
        vendorFile.close();
        return vendor.empty() ? "Unknown" : vendor;
    }
    return "Unknown";
#endif
}

string BiosDetector::getProductName() {
#ifdef _WIN32
    HKEY hKey;
    char productData[255];
    DWORD bufferSize = sizeof(productData);
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                    "HARDWARE\\DESCRIPTION\\System\\BIOS", 
                    0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        if (RegQueryValueExA(hKey, "SystemProductName", NULL, NULL, 
                           (LPBYTE)productData, &bufferSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return string(productData);
        }
        RegCloseKey(hKey);
    }
    return "Unknown";
#else
    ifstream productFile("/sys/class/dmi/id/product_name");
    if (productFile.is_open()) {
        string product;
        getline(productFile, product);
        productFile.close();
        return product.empty() ? "Unknown" : product;
    }
    return "Unknown";
#endif
}

string BiosDetector::getResult() {
    bool vmDetected = detectVM();
    string vendor = getBiosVendor();
    string product = getProductName();
    
    if (vmDetected) {
        return "VM Detectada [BIOS: " + vendor + ", Product: " + product + "]";
    } else {
        return "No VM Detectada [BIOS: " + vendor + "]";
    }
}

string BiosDetector::getDetailedInfo() {
    string info = "=== BIOS Detection Details ===\n";
    
    info += "BIOS Vendor: " + getBiosVendor() + "\n";
    info += "Product Name: " + getProductName() + "\n";
    info += "VM Detection: " + string(detectVM() ? "POSITIVE" : "NEGATIVE") + "\n";
    
    return info;
}