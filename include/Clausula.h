#ifndef CLAUSULA_H
#define CLAUSULA_H

/**
 * @brief Estructura para representar una cláusula del problema 3SAT
 * 
 * Una cláusula está compuesta por tres literales. Cada literal es un entero
 * que representa una variable o su negación:
 * - Valores positivos: variable sin negar (ej: 1 = p, 2 = q, 3 = r)
 * - Valores negativos: variable negada (ej: -1 = ¬p, -2 = ¬q, -3 = ¬r)
 */
struct Clausula {
    int l1; // Primer literal
    int l2; // Segundo literal
    int l3; // Tercer literal
};

#endif // CLAUSULA_H
