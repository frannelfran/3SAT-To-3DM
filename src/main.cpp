/**
 * @file main.cpp
 * @brief Programa interactivo para demostrar la reducción de 3SAT a 3DM
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "Reduccion3SATto3DM.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <filesystem>
#include <thread>
#include <chrono>
#include <algorithm>
#include <unistd.h>

namespace fs = std::filesystem;

// Prototipos de funciones
void limpiarPantalla();
void pausar(int ms = 500);
void mostrarMenu();
void mostrarBanner();
void animarTexto(const std::string& texto, int delay = 1);
std::vector<std::string> obtenerArchivosData();
void mostrarFormula(int numVars, const std::vector<Clausula>& formula);
bool cargarDesdeArchivo(const std::string& filepath, int& numVars, std::vector<Clausula>& formula);
std::vector<Clausula> leerFormulaManual(int& numVars);
void ejecutarReduccion(int numVars, const std::vector<Clausula>& formula, bool detalles);
void guardarResultados(const std::string& filename, int numVars, const std::vector<Clausula>& formula);
void mostrarAyuda();
std::string clausulaToString(const Clausula& c);
void limpiarBuffer();

int main() {
    limpiarPantalla();
    mostrarBanner();
    pausar(800);
    
    bool continuar = true;
    
    while (continuar) {
        limpiarPantalla();
        mostrarMenu();
        
        int opcion;
        std::cout << "\n🎯 Selecciona una opción: ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            limpiarBuffer();
            std::cout << "\n❌ Opción inválida. Intenta de nuevo.\n";
            pausar(1500);
            continue;
        }
        limpiarBuffer();
        
        std::vector<Clausula> formula;
        int numVars = 0;
        bool mostrarDetalles = true;
        
        switch (opcion) {
            case 1: {
                // Cargar desde archivo
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        CARGAR MODELO PREDEFINIDO                 ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                auto archivos = obtenerArchivosData();
                
                if (archivos.empty()) {
                    std::cout << "❌ No hay archivos en la carpeta data/\n";
                    pausar(2000);
                    break;
                }
                
                std::cout << "Archivos disponibles:\n\n";
                for (size_t i = 0; i < archivos.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << archivos[i] << "\n";
                }
                
                std::cout << "\n💡 Selecciona un archivo (1-" << archivos.size() << "): ";
                int seleccion;
                
                if (!(std::cin >> seleccion) || seleccion < 1 || seleccion > (int)archivos.size()) {
                    std::cin.clear();
                    limpiarBuffer();
                    std::cout << "\n❌ Selección inválida.\n";
                    pausar(1500);
                    break;
                }
                limpiarBuffer();
                
                std::string filepath = "data/" + archivos[seleccion - 1];
                
                if (cargarDesdeArchivo(filepath, numVars, formula)) {
                    std::cout << "\n✓ Archivo cargado exitosamente\n\n";
                    mostrarFormula(numVars, formula);
                    
                    std::cout << "\n¿Mostrar detalles completos? (s/n): ";
                    char resp;
                    std::cin >> resp;
                    limpiarBuffer();
                    mostrarDetalles = (resp == 's' || resp == 'S');
                    
                    limpiarPantalla();
                    ejecutarReduccion(numVars, formula, mostrarDetalles);
                    
                    if (isatty(fileno(stdin))) {
                        std::cout << "\n\nPresiona Enter para continuar...";
                        std::cin.get();
                    }
                } else {
                    std::cout << "\n❌ Error al cargar el archivo.\n";
                    pausar(2000);
                }
                break;
            }
            
            case 2: {
                // Entrada manual
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        ENTRADA MANUAL DE FÓRMULA                 ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                formula = leerFormulaManual(numVars);
                
                if (!formula.empty()) {
                    std::cout << "\n¿Mostrar detalles completos? (s/n): ";
                    char resp;
                    std::cin >> resp;
                    limpiarBuffer();
                    mostrarDetalles = (resp == 's' || resp == 'S');
                    
                    limpiarPantalla();
                    ejecutarReduccion(numVars, formula, mostrarDetalles);
                    
                    if (isatty(fileno(stdin))) {
                        std::cout << "\n\nPresiona Enter para continuar...";
                        std::cin.get();
                    }
                }
                break;
            }
            
            case 3: {
                // Guardar ejemplo
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        GUARDAR RESULTADOS                        ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                auto archivos = obtenerArchivosData();
                
                if (archivos.empty()) {
                    std::cout << "❌ No hay archivos en la carpeta data/\n";
                    pausar(2000);
                    break;
                }
                
                std::cout << "Selecciona el modelo a guardar:\n\n";
                for (size_t i = 0; i < archivos.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << archivos[i] << "\n";
                }
                
                std::cout << "\n💡 Selecciona (1-" << archivos.size() << "): ";
                int sel;
                
                if (!(std::cin >> sel) || sel < 1 || sel > (int)archivos.size()) {
                    std::cin.clear();
                    limpiarBuffer();
                    std::cout << "\n❌ Selección inválida.\n";
                    pausar(1500);
                    break;
                }
                limpiarBuffer();
                
                std::string filepath = "data/" + archivos[sel - 1];
                
                if (cargarDesdeArchivo(filepath, numVars, formula)) {
                    std::cout << "\nNombre del archivo de salida (sin extensión): ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    filename += ".txt";
                    
                    guardarResultados(filename, numVars, formula);
                    pausar(1500);
                } else {
                    std::cout << "\n❌ Error al cargar el archivo.\n";
                    pausar(2000);
                }
                break;
            }
            
            case 4: {
                // Ayuda
                limpiarPantalla();
                mostrarAyuda();
                if (isatty(fileno(stdin))) {
                    std::cout << "\nPresiona Enter para continuar...";
                    std::cin.get();
                }
                break;
            }
            
            case 0: {
                // Salir
                limpiarPantalla();
                std::cout << "\n\n";
                animarTexto("    ╔══════════════════════════════════════════╗\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ║         ¡Hasta luego!                    ║\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ║    Gracias por usar 3SAT-To-3DM          ║\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ╚══════════════════════════════════════════╝\n", 1);
                std::cout << "\n\n";
                pausar(800);
                limpiarPantalla();
                continuar = false;
                break;
            }
            
            default:
                std::cout << "\n❌ Opción no válida. Intenta de nuevo.\n";
                pausar(1500);
        }
    }
    
    return 0;
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void animarTexto(const std::string& texto, int delay) {
    // En modo no-interactivo (cuando stdin no es un terminal), no animar
    if (!isatty(fileno(stdin))) {
        std::cout << texto;
        return;
    }
    
    for (char c : texto) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void mostrarBanner() {
    std::cout << "\n\n";
    animarTexto("    ╔════════════════════════════════════════════════╗\n", 1);
    animarTexto("    ║                                                ║\n", 1);
    animarTexto("    ║         REDUCCIÓN 3SAT → 3DM                   ║\n", 1);
    animarTexto("    ║                                                ║\n", 1);
    animarTexto("    ║   NP-Completitud y Complejidad Computacional   ║\n", 1);
    animarTexto("    ║                                                ║\n", 1);
    animarTexto("    ║   3-Satisfiability → 3-Dimensional Matching    ║\n", 1);
    animarTexto("    ║                                                ║\n", 1);
    animarTexto("    ╚════════════════════════════════════════════════╝\n", 1);
    std::cout << "\n";
}

void mostrarMenu() {
    std::cout << "\n";
    std::cout << "    ╔══════════════════════════════════════════════════╗\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║              MENÚ PRINCIPAL                      ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ╠══════════════════════════════════════════════════╣\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║    1. Cargar modelo predefinido                  ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║    2. Introducir fórmula manualmente             ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║    3. Guardar resultados en archivo              ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║    4. Ayuda y explicación                        ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ║    0. Salir                                      ║\n";
    std::cout << "    ║                                                  ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════╝\n";
}

std::vector<std::string> obtenerArchivosData() {
    std::vector<std::string> archivos;
    
    if (!fs::exists("data") || !fs::is_directory("data")) {
        return archivos;
    }
    
    for (const auto& entry : fs::directory_iterator("data")) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            archivos.push_back(entry.path().filename().string());
        }
    }
    
    std::sort(archivos.begin(), archivos.end());
    return archivos;
}

void mostrarFormula(int numVars, const std::vector<Clausula>& formula) {
    std::cout << "📋 Fórmula cargada:\n\n";
    std::cout << "   Variables: " << numVars << "\n";
    std::cout << "   Cláusulas: " << formula.size() << "\n\n";
    
    for (size_t i = 0; i < formula.size(); ++i) {
        std::cout << "   C" << (i + 1) << ": " << clausulaToString(formula[i]);
        if (i < formula.size() - 1) {
            std::cout << " ∧";
        }
        std::cout << "\n";
    }
}

bool cargarDesdeArchivo(const std::string& filepath, int& numVars, std::vector<Clausula>& formula) {
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        return false;
    }
    
    formula.clear();
    std::string linea;
    
    // Leer hasta encontrar la línea con numVars y numClausulas
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') {
            continue;
        }
        
        std::istringstream iss(linea);
        int numClausulas;
        if (iss >> numVars >> numClausulas) {
            // Leer las cláusulas
            for (int i = 0; i < numClausulas; ++i) {
                if (!std::getline(file, linea)) {
                    return false;
                }
                
                if (linea.empty() || linea[0] == '#') {
                    --i;
                    continue;
                }
                
                std::istringstream clausula_stream(linea);
                int l1, l2, l3;
                if (clausula_stream >> l1 >> l2 >> l3) {
                    formula.push_back({l1, l2, l3});
                } else {
                    return false;
                }
            }
            return true;
        }
    }
    
    return false;
}

std::vector<Clausula> leerFormulaManual(int& numVars) {
    std::vector<Clausula> formula;
    
    std::cout << "Número de variables: ";
    std::cin >> numVars;
    
    if (numVars <= 0 || numVars > 26) {
        std::cout << "❌ Número de variables inválido (debe estar entre 1 y 26).\n";
        limpiarBuffer();
        return formula;
    }
    
    int numClausulas;
    std::cout << "Número de cláusulas: ";
    std::cin >> numClausulas;
    
    if (numClausulas <= 0) {
        std::cout << "❌ Número de cláusulas inválido.\n";
        limpiarBuffer();
        return formula;
    }
    
    limpiarBuffer();
    
    std::cout << "\nIntroduce cada cláusula (3 literales separados por espacios)\n";
    std::cout << "Usa números positivos para variables (1=" << (char)('p') << ", 2=" << (char)('q') << ", etc.)\n";
    std::cout << "Usa números negativos para negación (-1=¬p, -2=¬q, etc.)\n";
    std::cout << "Ejemplo: 1 -2 3 representa (p ∨ ¬q ∨ r)\n\n";
    
    for (int i = 0; i < numClausulas; ++i) {
        std::cout << "Cláusula " << (i + 1) << ": ";
        
        int l1, l2, l3;
        if (!(std::cin >> l1 >> l2 >> l3)) {
            std::cout << "❌ Error en la entrada. Intenta de nuevo.\n";
            std::cin.clear();
            limpiarBuffer();
            --i;
            continue;
        }
        
        if (std::abs(l1) > numVars || std::abs(l2) > numVars || std::abs(l3) > numVars) {
            std::cout << "❌ Los literales deben estar en el rango [-" << numVars << ", " << numVars << "].\n";
            --i;
            continue;
        }
        
        if (l1 == 0 || l2 == 0 || l3 == 0) {
            std::cout << "❌ Los literales no pueden ser 0.\n";
            --i;
            continue;
        }
        
        formula.push_back({l1, l2, l3});
    }
    
    limpiarBuffer();
    
    std::cout << "\n✓ Fórmula ingresada correctamente:\n";
    for (size_t i = 0; i < formula.size(); ++i) {
        std::cout << "  C" << (i + 1) << ": " << clausulaToString(formula[i]) << "\n";
    }
    std::cout << "\n";
    
    return formula;
}

void ejecutarReduccion(int numVars, const std::vector<Clausula>& formula, bool detalles) {
    std::cout << "\n";
    animarTexto("╔══════════════════════════════════════════════════╗\n", 1);
    animarTexto("║        EJECUTANDO REDUCCIÓN                      ║\n", 1);
    animarTexto("╚══════════════════════════════════════════════════╝\n", 1);
    std::cout << "\n";
    
    std::cout << "⚙️  Generando componentes..." << std::flush;
    pausar(500);
    std::cout << " ✓\n";
    
    std::cout << "⚙️  Construyendo tripletas..." << std::flush;
    
    Reduccion3SATto3DM reduccion(numVars, formula);
    reduccion.generar();
    
    pausar(500);
    std::cout << " ✓\n";
    pausar(300);
    
    std::cout << "\n";
    std::cout << std::string(60, '-') << "\n\n";
    
    if (detalles) {
        reduccion.imprimirResultados();
    } else {
        std::cout << "✅ Reducción completada exitosamente\n\n";
        std::cout << "   📊 Resumen:\n";
        std::cout << "   ├─ Variables: " << numVars << "\n";
        std::cout << "   ├─ Cláusulas: " << formula.size() << "\n";
        std::cout << "   └─ Matching objetivo: " << (numVars * (int)formula.size()) << " tripletas\n";
    }
}

void guardarResultados(const std::string& filename, int numVars, const std::vector<Clausula>& formula) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "❌ Error al abrir el archivo " << filename << "\n";
        return;
    }
    
    file << "========================================\n";
    file << "  REDUCCIÓN 3SAT → 3DM\n";
    file << "========================================\n\n";
    
    file << "FÓRMULA 3SAT:\n";
    file << "Variables: " << numVars << "\n";
    file << "Cláusulas: " << formula.size() << "\n\n";
    
    for (size_t i = 0; i < formula.size(); ++i) {
        file << "C" << (i + 1) << ": " << clausulaToString(formula[i]) << "\n";
    }
    
    file << "\n" << std::string(40, '-') << "\n\n";
    
    // Redirigir cout al archivo temporalmente
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());
    
    Reduccion3SATto3DM reduccion(numVars, formula);
    reduccion.generar();
    reduccion.imprimirResultados();
    
    // Restaurar cout
    std::cout.rdbuf(coutBuf);
    
    file.close();
    std::cout << "✓ Resultados guardados en: " << filename << "\n";
}

void mostrarAyuda() {
    std::cout << "\n";
    animarTexto("╔══════════════════════════════════════════════════╗\n", 1);
    animarTexto("║                    AYUDA                         ║\n", 1);
    animarTexto("╚══════════════════════════════════════════════════╝\n", 1);
    std::cout << "\n";
    
    std::cout << "📚 ¿Qué es 3SAT?\n";
    std::cout << "   El problema 3-Satisfiability (3SAT) consiste en determinar\n";
    std::cout << "   si existe una asignación de valores booleanos a variables\n";
    std::cout << "   que satisfaga una fórmula con cláusulas de 3 literales.\n\n";
    
    std::cout << "🎯 ¿Qué es 3DM?\n";
    std::cout << "   El problema 3-Dimensional Matching (3DM) busca un matching\n";
    std::cout << "   perfecto en un conjunto de tripletas (w, x, y).\n\n";
    
    std::cout << "🔄 ¿Qué hace este programa?\n";
    std::cout << "   Realiza la reducción polinomial de 3SAT a 3DM:\n\n";
    
    std::cout << "   1️⃣  Truth-Setting: Asignar valores a variables\n";
    std::cout << "   2️⃣  Satisfaction Testing: Verificar cláusulas\n";
    std::cout << "   3️⃣  Garbage Collection: Completar el matching\n\n";
    
    std::cout << "💡 Notación de literales:\n";
    std::cout << "   • Positivos: 1=p, 2=q, 3=r, 4=s, etc.\n";
    std::cout << "   • Negativos: -1=¬p, -2=¬q, -3=¬r, -4=¬s, etc.\n";
    std::cout << "   • Ejemplo: (p ∨ ¬q ∨ r) → 1 -2 3\n\n";
    
    std::cout << "📁 Formato de archivos data/*.txt:\n";
    std::cout << "   # Comentario (opcional)\n";
    std::cout << "   <num_variables> <num_clausulas>\n";
    std::cout << "   <lit1> <lit2> <lit3>\n";
    std::cout << "   ...\n";
}

std::string clausulaToString(const Clausula& c) {
    auto literalToString = [](int lit) -> std::string {
        char var = 'a' + std::abs(lit) - 1;
        if (lit < 0) {
            return "¬" + std::string(1, var);
        }
        return std::string(1, var);
    };
    
    return "(" + literalToString(c.l1) + " ∨ " + 
           literalToString(c.l2) + " ∨ " + 
           literalToString(c.l3) + ")";
}

void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
