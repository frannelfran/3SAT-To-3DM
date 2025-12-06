/**
 * @file UI.cpp
 * @brief Implementación de la interfaz de usuario
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "UI.h"
#include "Utils.h"
#include <iostream>

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
