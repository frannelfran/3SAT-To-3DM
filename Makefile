# Makefile para el proyecto 3SAT-To-3DM
# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
DEBUGFLAGS = -g -O0
RELEASEFLAGS = -O3

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
DOC_DIR = doc

# Archivos fuente y objeto
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/Reduccion3SATto3DM.cpp $(SRC_DIR)/Utils.cpp $(SRC_DIR)/UI.cpp $(SRC_DIR)/FormulaHandler.cpp $(SRC_DIR)/JsonUtils.cpp
OBJECTS = $(BIN_DIR)/main.o $(BIN_DIR)/Reduccion3SATto3DM.o $(BIN_DIR)/Utils.o $(BIN_DIR)/UI.o $(BIN_DIR)/FormulaHandler.o $(BIN_DIR)/JsonUtils.o

# Ejecutable
TARGET = $(BIN_DIR)/3sat-to-3dm

# Target por defecto
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando ejecutable..."
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "✓ Compilación completada: $(TARGET)"

# Compilar main.cpp
$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/Reduccion3SATto3DM.h $(INCLUDE_DIR)/Utils.h $(INCLUDE_DIR)/UI.h $(INCLUDE_DIR)/FormulaHandler.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando main.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(BIN_DIR)/main.o

# Compilar Reduccion3SATto3DM.cpp
$(BIN_DIR)/Reduccion3SATto3DM.o: $(SRC_DIR)/Reduccion3SATto3DM.cpp $(INCLUDE_DIR)/Reduccion3SATto3DM.h $(INCLUDE_DIR)/Tripleta.h $(INCLUDE_DIR)/Clausula.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando Reduccion3SATto3DM.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Reduccion3SATto3DM.cpp -o $(BIN_DIR)/Reduccion3SATto3DM.o

# Compilar Utils.cpp
$(BIN_DIR)/Utils.o: $(SRC_DIR)/Utils.cpp $(INCLUDE_DIR)/Utils.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando Utils.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Utils.cpp -o $(BIN_DIR)/Utils.o

# Compilar UI.cpp
$(BIN_DIR)/UI.o: $(SRC_DIR)/UI.cpp $(INCLUDE_DIR)/UI.h $(INCLUDE_DIR)/Utils.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando UI.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/UI.cpp -o $(BIN_DIR)/UI.o

# Compilar FormulaHandler.cpp
$(BIN_DIR)/FormulaHandler.o: $(SRC_DIR)/FormulaHandler.cpp $(INCLUDE_DIR)/FormulaHandler.h $(INCLUDE_DIR)/Reduccion3SATto3DM.h $(INCLUDE_DIR)/Utils.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando FormulaHandler.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/FormulaHandler.cpp -o $(BIN_DIR)/FormulaHandler.o

# Compilar JsonUtils.cpp
$(BIN_DIR)/JsonUtils.o: $(SRC_DIR)/JsonUtils.cpp $(INCLUDE_DIR)/JsonUtils.h
	@mkdir -p $(BIN_DIR)
	@echo "Compilando JsonUtils.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/JsonUtils.cpp -o $(BIN_DIR)/JsonUtils.o

# Compilar con símbolos de depuración
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean $(TARGET)
	@echo "✓ Compilación en modo DEBUG completada"

# Compilar con optimizaciones
release: CXXFLAGS += $(RELEASEFLAGS)
release: clean $(TARGET)
	@echo "✓ Compilación en modo RELEASE completada"

# Ejecutar el programa
run: $(TARGET)
	@echo "Ejecutando programa..."
	@echo "===================="
	@./$(TARGET)

# Ejecutar demo rápido (ejemplo 1 en modo resumen)
demo: $(TARGET)
	@echo "=== DEMO RÁPIDO ==="
	@echo "Ejecutando Ejemplo 1 en modo resumen..."
	@echo ""
	@printf "1\n1\nn\n0\n" | ./$(TARGET)

# Test interactivo con todos los ejemplos
test-interactive: $(TARGET)
	@echo "=== TEST INTERACTIVO ==="
	@echo "Probando carga de archivos..."
	@printf "1\n1\nn\n0\n" | ./$(TARGET) > /dev/null && echo "✓ Carga de archivo OK" || echo "✗ Carga FALLO"
	@echo "Probando ayuda..."
	@printf "4\n\n0\n" | ./$(TARGET) > /dev/null && echo "✓ Ayuda OK" || echo "✗ Ayuda FALLO"
	@echo ""
	@echo "=== TESTS COMPLETADOS ==="

# Limpiar archivos compilados
clean:
	@echo "Limpiando archivos compilados..."
	@rm -rf $(BIN_DIR)/*.o $(TARGET)
	@echo "✓ Limpieza completada"

# Limpiar todo incluyendo directorios
distclean: clean
	@echo "Limpieza profunda..."
	@rm -rf $(BIN_DIR)/*
	@echo "✓ Limpieza profunda completada"

# Generar documentación (requiere Doxygen)
docs:
	@echo "Generando documentación con Doxygen..."
	@mkdir -p $(DOC_DIR)
	@doxygen Doxyfile 2>/dev/null || echo "⚠ Doxygen no encontrado o Doxyfile no configurado"

# Mostrar ayuda
help:
	@echo "Makefile para 3SAT-To-3DM"
	@echo "========================="
	@echo "Targets disponibles:"
	@echo "  make              - Compila el proyecto (modo normal)"
	@echo "  make debug        - Compila con símbolos de depuración"
	@echo "  make release      - Compila con optimizaciones"
	@echo "  make run          - Compila y ejecuta el programa interactivo"
	@echo "  make demo         - Ejecuta demo rápido (Ejemplo 1 automático)"
	@echo "  make test-interactive - Ejecuta tests de todos los ejemplos"
	@echo "  make clean        - Elimina archivos objeto y ejecutable"
	@echo "  make distclean    - Limpieza completa del directorio bin"
	@echo "  make docs         - Genera documentación con Doxygen"
	@echo "  make help         - Muestra esta ayuda"

# Targets que no generan archivos
.PHONY: all debug release run demo test-interactive clean distclean docs help
