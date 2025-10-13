# 🔍 Detector de Máquinas Virtuales

Programa en C++ que detecta máquinas virtuales usando 3 técnicas diferentes.

## 🛠️ Técnicas de Detección

- **CPUID**: Busca el bit de hypervisor en el procesador
- **BIOS**: Verifica el fabricante del BIOS  
- **MAC**: Revisa direcciones de red de fabricantes de VMs

## 🚀 Compilación

### Windows
```cmd
cl /EHsc /std:c++17 *.cpp advapi32.lib iphlpapi.lib ws2_32.lib
```

### Linux
```bash
g++ -std=c++17 -o detector_vm *.cpp
```

## 💻 Uso

```bash
# Todas las técnicas
./detector_vm --all

# Técnica específica
./detector_vm --technique 1    # CPUID
./detector_vm --technique 2    # BIOS
./detector_vm --technique 3    # MAC

# Modo detallado
./detector_vm --verbose
```

## 📊 Resultados en Mi Equipo

| Técnica | Mi PC Física | VM VMware |
|---------|--------------|-----------|
| CPUID | ✅ **Sí** detecta | ✅ Sí |
| BIOS | ❌ No detecta | ✅ Sí |
| MAC | ✅ **Sí** detecta | ✅ Sí |

**Nota**: Mi PC física detecta VM porque tiene Hyper-V y adaptadores VMware instalados.

## 📁 Archivos

- `main.cpp` - Programa principal
- `CpuidDetector.*` - Detección por CPUID
- `BiosDetector.*` - Detección por BIOS
- `MacDetector.*` - Detección por MAC
- `Makefile` - Para compilar en Linux

## ⚠️ Nota

Proyecto académico para la materia de ciberseguridad.
