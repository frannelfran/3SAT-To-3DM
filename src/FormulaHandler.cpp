/**
 * @file FormulaHandler.cpp
 * @brief Implementación del manejo de fórmulas 3SAT
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "FormulaHandler.h"
#include "Reduccion3SATto3DM.h"
#include "Utils.h"
#include "JsonUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<std::string> obtenerArchivosData() {
    std::vector<std::string> archivos;
    
    if (!fs::exists("data") || !fs::is_directory("data")) {
        return archivos;
    }
    
    for (const auto& entry : fs::directory_iterator("data")) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".txt" || ext == ".json") {
                archivos.push_back(entry.path().filename().string());
            }
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
    // Detectar si es JSON
    if (filepath.size() >= 5 && filepath.substr(filepath.size() - 5) == ".json") {
        auto data = JsonUtils::leerFormulaJson(filepath);
        if (data.exito) {
            numVars = data.numVars;
            formula = data.clausulas;
            return true;
        }
        return false;
    }

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
    // Asegurar que existe el directorio out
    if (!fs::exists("out")) {
        fs::create_directory("out");
    }
    
    std::string fullPath = "out/" + filename;

    // Si es JSON, usar el formato estructurado
    if (fullPath.size() >= 5 && fullPath.substr(fullPath.size() - 5) == ".json") {
        // Silenciar salida durante la generación
        std::cout.setstate(std::ios_base::failbit);
        Reduccion3SATto3DM reduccion(numVars, formula);
        reduccion.generar();
        std::cout.clear();
        
        if (JsonUtils::guardarResultadoJson(fullPath, reduccion.getTripletas())) {
            std::cout << "✓ Resultados guardados en JSON: " << fullPath << "\n";
        } else {
            std::cout << "❌ Error al guardar el archivo JSON.\n";
        }
        return;
    }

    std::ofstream file(fullPath);
    
    if (!file.is_open()) {
        std::cout << "❌ Error al abrir el archivo " << fullPath << "\n";
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
    std::cout << "✓ Resultados guardados en: " << fullPath << "\n";
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
