/**
 * @file FormulaHandler.cpp
 * @brief Implementación del manejo de fórmulas 3SAT
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "Reduccion3SATto3DM.h"
#include <iostream>
#include <cmath>
#include <string>

Reduccion3SATto3DM::Reduccion3SATto3DM(int numVars, std::vector<Clausula> f) 
    : n(numVars), formula(f) {
    m = formula.size();
}

void Reduccion3SATto3DM::generar() {
    std::cout << "--- Generando Reduccion 3SAT -> 3DM ---\n";
    
    // 1. Truth-Setting (Configuración de Verdad)
    // Se crean componentes para cada variable que fuerzan a elegir True o False.
    generarComponentesVariables();

    // 2. Satisfaction Testing (Comprobación de Satisfacción)
    // Se crean tripletas para cubrir las cláusulas usando los "tips" libres.
    generarComponentesClausulas();

    // 3. Garbage Collection (Recolección de Basura)
    // Se añaden elementos para asegurar que sea un matching perfecto.
    generarGarbageCollection();
}

void Reduccion3SATto3DM::imprimirResultados() const {
    std::cout << "\n--- Conjunto M (Tripletas) Generado ---\n";
    std::cout << "Formato: (W, X, Y)\n";
    for (const auto& t : M) {
        std::cout << "Tipo [" << t.tipo << "]: (" 
                  << t.w << ", " << t.x << ", " << t.y << ")\n";
    }
    std::cout << "\nTotal de Tripletas: " << M.size() << "\n";
    
    // Cardinalidad esperada para un matching perfecto q = n*m
    std::cout << "Matching Perfecto objetivo requiere seleccionar " << m * n << " tripletas.\n"; 
}

void Reduccion3SATto3DM::generarComponentesVariables() {
    // Definimos elementos en W, X, Y para cada variable.
    // Por cada variable 'i', generamos un anillo de 'm' etapas.
    
    for (int i = 1; i <= n; ++i) {
        // --- CORRECCIÓN DE FRANCO ---
        // Antes esto fallaba para la 4ta variable (ponía 'r' siempre).
        // Ahora genera nombres dinámicos: a, b, c, d...
        char letraVar = 'a' + (i - 1); 
        std::string varName(1, letraVar); 
        
        // Inicializar vectores de tips para esta variable
        tipsPositivos[i].resize(m);
        tipsNegativos[i].resize(m);

        for (int j = 0; j < m; ++j) {
            // Nombres de nodos internos (X, Y) y externos (W)
            // Usamos nomenclatura de los apuntes: puntas p1, -p1, etc.
            
            std::string x_ij = "x_" + varName + "_" + std::to_string(j+1);
            std::string y_ij = "y_" + varName + "_" + std::to_string(j+1);
            
            // Los elementos de W son los "tips" o puntas que conectan con las cláusulas
            // W consiste en componentes externas
            std::string w_ij  = "w_" + varName + "_" + std::to_string(j+1);     // Punta asociada a literal positivo
            std::string w_bar_ij = "w_neg_" + varName + "_" + std::to_string(j+1); // Punta asociada a literal negativo

            // Guardamos referencias para usarlas en las cláusulas
            tipsPositivos[i][j] = w_ij;
            tipsNegativos[i][j] = w_bar_ij;

            // Construcción del gadget (anillo):
            // Tripleta TRUE: selecciona la punta negativa para dejar libre la positiva a la cláusula (o viceversa según convención).
            
            // Opción A (Variable=True): (w_neg, x_current, y_current)
            M.push_back({w_bar_ij, x_ij, y_ij, "Var-" + varName + "-True"});

            // Opción B (Variable=False): (w_pos, x_next, y_current)
            // Nota: El índice x "siguiente" conecta el anillo. (j+1) % m.
            std::string x_next = "x_" + varName + "_" + std::to_string((j + 1) % m + 1);
            M.push_back({w_ij, x_next, y_ij, "Var-" + varName + "-False"});
        }
    }
}

void Reduccion3SATto3DM::generarComponentesClausulas() {
    // Satisfaction testing
    // Por cada cláusula 'j', creamos tripletas que intentan hacer "match" con los tips libres de las variables.
    
    for (int j = 0; j < m; ++j) {
        Clausula c = formula[j];
        std::string c_s1 = "s1_c" + std::to_string(j+1); // Componentes S 
        std::string c_s2 = "s2_c" + std::to_string(j+1); 
        
        // Función auxiliar para conectar un literal con la cláusula
        auto agregarTripletaClausula = [&](int literal) {
            int varIdx = std::abs(literal);
            bool esNegado = (literal < 0);
            
            // Si el literal es P, buscamos el tip de P.
            // Si la variable se puso a TRUE en el anillo, el tip P está LIBRE.
            std::string w_target = esNegado ? tipsNegativos[varIdx][j] : tipsPositivos[varIdx][j];
            
            M.push_back({w_target, c_s1, c_s2, "Clausula-" + std::to_string(j+1)});
        };

        agregarTripletaClausula(c.l1);
        agregarTripletaClausula(c.l2);
        agregarTripletaClausula(c.l3);
    }
}

void Reduccion3SATto3DM::generarGarbageCollection() {
    // Garbage Collection
    // NOTA SOBRE COMENTARIO DE FRANCO:
    // El Garbage Collection DEBE conectarse a todos los tips posibles.
    // Si excluimos los tips de las cláusulas, no podríamos recoger los literales "sobrantes"
    // en cláusulas con múltiples valores verdaderos.
    
    int totalGarbage = m * (n - 1); 
    
    for (int k = 1; k <= totalGarbage; ++k) {
        std::string g1 = "g1_" + std::to_string(k);
        std::string g2 = "g2_" + std::to_string(k);
        
        // La recolección de basura se conecta a CUALQUIER tip (positivo o negativo)
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < m; ++j) {
                M.push_back({tipsPositivos[i][j], g1, g2, "Garbage"});
                M.push_back({tipsNegativos[i][j], g1, g2, "Garbage"});
            }
        }
    }
}