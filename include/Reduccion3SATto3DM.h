#ifndef REDUCCION3SATTO3DM_H
#define REDUCCION3SATTO3DM_H

#include "Tripleta.h"
#include "Clausula.h"
#include <vector>
#include <map>
#include <string>

/**
 * @brief Clase que implementa la reducción de 3SAT a 3DM
 * 
 * Esta clase realiza la reducción polinomial del problema 3-Satisfiability (3SAT)
 * al problema 3-Dimensional Matching (3DM), demostrando que 3DM es NP-Completo.
 * 
 * La reducción se compone de tres fases principales:
 * 1. Truth-Setting: Componentes para cada variable que fuerzan elegir True o False
 * 2. Satisfaction Testing: Tripletas que verifican si las cláusulas se satisfacen
 * 3. Garbage Collection: Elementos adicionales para completar el matching perfecto
 */
class Reduccion3SATto3DM {
private:
    int n; // Número de variables
    int m; // Número de cláusulas
    std::vector<Clausula> formula; // Fórmula 3SAT de entrada
    std::vector<Tripleta> M;       // Conjunto M de tripletas resultante
    
    // Almacenamos los "tips" (puntas) de W generados por las variables
    // map[variable_id][clausula_index] -> nombre_del_nodo_w
    std::map<int, std::vector<std::string>> tipsPositivos;
    std::map<int, std::vector<std::string>> tipsNegativos;

    /**
     * @brief Genera los componentes de variables (Truth-Setting)
     * 
     * Crea un anillo de 'm' etapas por cada variable, con tripletas que
     * representan las dos opciones: asignar True o False a la variable.
     */
    void generarComponentesVariables();

    /**
     * @brief Genera los componentes de cláusulas (Satisfaction Testing)
     * 
     * Crea tripletas que conectan las cláusulas con los "tips" libres
     * de las variables, permitiendo verificar la satisfacción.
     */
    void generarComponentesClausulas();

    /**
     * @brief Genera los componentes de basura (Garbage Collection)
     * 
     * Añade tripletas adicionales para asegurar que el matching perfecto
     * tenga la cardinalidad correcta (m * n tripletas).
     */
    void generarGarbageCollection();

public:
    /**
     * @brief Constructor de la clase
     * @param numVars Número de variables en la fórmula 3SAT
     * @param f Vector de cláusulas que conforman la fórmula 3SAT
     */
    Reduccion3SATto3DM(int numVars, std::vector<Clausula> f);

    /**
     * @brief Ejecuta la reducción completa
     * 
     * Genera todos los componentes (variables, cláusulas y basura)
     * necesarios para la reducción de 3SAT a 3DM.
     */
    void generar();

    /**
     * @brief Imprime los resultados de la reducción
     * 
     * Muestra todas las tripletas generadas y estadísticas sobre
     * el conjunto M resultante.
     */
    void imprimirResultados() const;

    /**
     * @brief Obtiene el conjunto de tripletas generado
     * @return Vector de tripletas
     */
    const std::vector<Tripleta>& getTripletas() const { return M; }
};

#endif // REDUCCION3SATTO3DM_H
