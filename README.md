# Detector de Máquinas Virtuales - Multi-Técnica

Este es un detector avanzado de virtualización que usa tres técnicas diferentes para identificar si estás ejecutándote en una máquina virtual o en hardware físico.

## Integrantes del Equipo
- DIEGO AGUAYO FRIAS
- VALERIA ABIGAIL NAVARRO CASAREZ
- ASHLEY KARINA RIOS RODRIGUEZ
- LUIS CIPRIANO RODRIGUEZ GONZALEZ

## ¿Qué hace este programa?

Este chulada de programa analiza tu sistema usando tres métodos distintos para detectar si estás en una VM:

- **🔍 Nivel Hardware** - CPUID y bits de hypervisor
- **🔍 Nivel Firmware** - Vendor del BIOS  
- **🔍 Nivel Red** - Direcciones MAC de virtualización

## Arquitectura del Proyecto

### **Módulos Principales:**

| Módulo | ¿Qué hace? | Archivos |
|--------|-------------|----------|
| **CpuidDetector** | Este revisa el procesador directamente usando instrucciones CPUID para detectar bits de hypervisor | `CpuidDetector.h/cpp` |
| **BiosDetector** | Este analiza el vendor del BIOS buscando nombres de fabricantes de VMs como VMware, VirtualBox, etc. | `BiosDetector.h/cpp` |
| **MacDetector** |  Este chequea las direcciones MAC de tus adaptadores de red contra OUI de virtualización | `MacDetector.h/cpp` |

### **Módulos de Utilidad:**

| Módulo | ¿Qué hace? | Archivos |
|--------|-------------|----------|
| **Color Utils** |  Maneja todos los colores bonitos en la terminal para Windows y Linux | `color_utils.cpp` |
| **Display Utils** |  Controla la interfaz visual, tablas, banners y formato de salida | `display_utils.cpp` |
| **Main Program** | Es el cerebro que coordina todo el flujo del programa | `main.cpp` |

##  Como Compilar Esta Maravilla

### **En Windows:**
```cmd
cl /EHsc /std:c++17 /Fevm_detector.exe main.cpp color_utils.cpp display_utils.cpp CpuidDetector.cpp BiosDetector.cpp MacDetector.cpp advapi32.lib iphlpapi.lib ws2_32.lib
```

### **En Linux:**
```bash
g++ -std=c++17 -o vm_detector main.cpp color_utils.cpp display_utils.cpp CpuidDetector.cpp BiosDetector.cpp MacDetector.cpp
```

## Como Usar el Detector

### **Modo Normal (Recomendado):**
```bash
./vm_detector
```
**MIRA MI CHAVO:** Esto te muestra una tabla bien padre con todos los resultados.

### **Modo Detallado:**
```bash
./vm_detector --verbose
```
**MIRA MI CHAVO:** Para cuando quieres ver toda la información técnica de cada detección.

### **Probar Técnica Específica:**
```bash
./vm_detector --technique 1    # Solo CPUID
./vm_detector --technique 2    # Solo BIOS  
./vm_detector --technique 3    # Solo MAC
```

### **Ayuda:**
```bash
./vm_detector --help
```

##  Ejemplo de Salida

```
=========================================
    DETECTOR DE VM - MULTI-TECNICA
    Ciberseguridad Anti-Virtualizacion
=========================================


==================================================
  TABLA DE RESULTADOS DE DETECCION
==================================================
+--------------+------------------------------+-----------------+
|   Tecnica    |          Resultado           |     Estado      |
+--------------+------------------------------+-----------------+
| CPUID        | VM Detectada [Microsoft Hv]  | VIRTUAL         |
| BIOS Vendor  | No VM Detectada [BIOS: LE... | FISICA          |
| MAC Address  | VM Detectada [MAC: 00:50:... | VIRTUAL         |
+--------------+------------------------------+-----------------+

RESULTADOS COMPLETOS:
CPUID: VM Detectada [Microsoft Hv]
BIOS Vendor: No VM Detectada [BIOS: LENOVO]
MAC Address: VM Detectada [MAC: 00:50:56:C0:00:01]


==================================================
  ANALISIS FINAL
==================================================
RESUMEN: ALTA PROBABILIDAD DE VM (2/3 tecnicas positivas)
--------------------------------------------------


==================================================
  INFORMACION DEL SISTEMA
==================================================
Plataforma: Windows
Tecnicas: Todas | Modo: Resumen

Analisis completado
```

##  Interpretación de Resultados

### **Estados Posibles:**
- **🟢 FISICA** - Probablemente hardware real
- **🔴 VIRTUAL** - Detectado como máquina virtual  

### **Niveles de Confianza:**
- **EVIDENCIA FUERTE** - 3/3 técnicas detectan VM
- **ALTA PROBABILIDAD** - 2/3 técnicas detectan VM  
- **POSIBLE VM** - 1/3 técnicas detecta VM
- **PROBABLE FÍSICA** - 0/3 técnicas detectan VM

##  Técnicas de Detección Implementadas

### **1. CPUID Hypervisor Bit**
- **Qué hace:** Revisa el bit 31 del registro CPUID
- **Efectividad:** Alta para hipervisores reales
- **Limitaciones:** Puede dar falsos positivos en algunos hosts

### **2. BIOS Vendor Strings**  
- **Qué hace:** Analiza cadenas del vendor del BIOS
- **Efectividad:** Muy buena para VMs puras
- **Limitaciones:** No detecta virtualización anidada

### **3. MAC Address OUI**
- **Qué hace:** Verifica prefixes de direcciones MAC
- **Efectividad:** Buena para VMs con red NAT
- **Limitaciones:** Fácil de evadir cambiando MAC

##  Características Técnicas

- **✅ Multiplataforma** - Funciona en Windows y Linux
- **✅ Colores Automáticos** - Se adapta a cada terminal
- **✅ Sin Dependencias** - Solo C++ estándar
- **✅ Salida Limpia** - Formato profesional y legible
- **✅ Modular** - Fácil de extender y mantener


## 💻 Desarrollo

¿Quieres mejorar esta chulada? ¡Bienvenido!
- Agrega nuevas técnicas de detección
- Mejora la interfaz de usuario  
- Optimiza el rendimiento
- Reporta bugs o ideas

---
