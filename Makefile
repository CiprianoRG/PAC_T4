# Compilador y configuraciones
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = vm_detector
SOURCES = main.cpp CpuidDetector.cpp BiosDetector.cpp MacDetector.cpp
HEADERS = CpuidDetector.h BiosDetector.h MacDetector.h
LIBS = -ladvapi32 -liphlpapi -lws2_32

# Detectar el sistema operativo
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET := $(TARGET).exe
    LIBS = -ladvapi32 -liphlpapi -lws2_32
else
    # Linux
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LIBS =
    endif
endif

# Regla principal
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

# Reglas individuales para cada módulo (opcional)
CpuidDetector.o: CpuidDetector.cpp CpuidDetector.h
	$(CXX) $(CXXFLAGS) -c CpuidDetector.cpp

BiosDetector.o: BiosDetector.cpp BiosDetector.h
	$(CXX) $(CXXFLAGS) -c BiosDetector.cpp

MacDetector.o: MacDetector.cpp MacDetector.h
	$(CXX) $(CXXFLAGS) -c MacDetector.cpp

main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compilación por separado (modular)
modular: main.o CpuidDetector.o BiosDetector.o MacDetector.o
	$(CXX) -o $(TARGET) main.o CpuidDetector.o BiosDetector.o MacDetector.o $(LIBS)

# Debug
debug: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -g -DDEBUG -o $(TARGET)_debug $(SOURCES) $(LIBS)

# Release optimizado
release: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -O3 -DNDEBUG -o $(TARGET)_release $(SOURCES) $(LIBS)

# Limpieza
clean:
	rm -f $(TARGET) $(TARGET)_debug $(TARGET)_release *.o

# Instalar dependencias (Linux)
deps-linux:
	sudo apt-get update
	sudo apt-get install build-essential g++

# Help
help:
	@echo "=== VM Detector Build System ==="
	@echo "Targets disponibles:"
	@echo "  make all        - Compila el proyecto principal"
	@echo "  make debug      - Compila con información de debug"
	@echo "  make release    - Compila optimizado para release"
	@echo "  make modular    - Compila cada módulo por separado"
	@echo "  make clean      - Limpia archivos compilados"
	@echo "  make deps-linux - Instala dependencias (Linux)"
	@echo "  make help       - Muestra esta ayuda"

# Alias
all: $(TARGET)

.PHONY: clean debug release deps-linux help all modular