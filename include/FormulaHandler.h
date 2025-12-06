/**
 * @file FormulaHandler.h
 * @brief Manejo de fórmulas 3SAT
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#ifndef FORMULA_HANDLER_H
#define FORMULA_HANDLER_H

#include "Clausula.h"
#include <string>
#include <vector>

/**
 * @brief Obtiene la lista de archivos .txt en la carpeta data/
 * @return Vector con nombres de archivos
 */
std::vector<std::string> obtenerArchivosData();

/**
 * @brief Muestra una fórmula en formato legible
 * @param numVars Número de variables
 * @param formula Vector de cláusulas
 */
void mostrarFormula(int numVars, const std::vector<Clausula>& formula);

/**
 * @brief Carga una fórmula desde un archivo
 * @param filepath Ruta al archivo
 * @param numVars Referencia donde guardar el número de variables
 * @param formula Referencia donde guardar la fórmula
 * @return true si se cargó correctamente, false en caso contrario
 */
bool cargarDesdeArchivo(const std::string& filepath, int& numVars, std::vector<Clausula>& formula);

/**
 * @brief Lee una fórmula manualmente del usuario
 * @param numVars Referencia donde guardar el número de variables
 * @return Vector de cláusulas introducidas
 */
std::vector<Clausula> leerFormulaManual(int& numVars);

/**
 * @brief Convierte una cláusula a string legible
 * @param c Cláusula a convertir
 * @return String con formato (a ∨ ¬b ∨ c)
 */
std::string clausulaToString(const Clausula& c);

/**
 * @brief Ejecuta la reducción de 3SAT a 3DM
 * @param numVars Número de variables
 * @param formula Vector de cláusulas
 * @param detalles Si se muestran detalles completos
 */
void ejecutarReduccion(int numVars, const std::vector<Clausula>& formula, bool detalles);

/**
 * @brief Guarda los resultados en un archivo
 * @param filename Nombre del archivo de salida
 * @param numVars Número de variables
 * @param formula Vector de cláusulas
 */
void guardarResultados(const std::string& filename, int numVars, const std::vector<Clausula>& formula);

#endif // FORMULA_HANDLER_H
