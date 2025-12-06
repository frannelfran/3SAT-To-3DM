#ifndef TRIPLETA_H
#define TRIPLETA_H

#include <string>

/**
 * @brief Representación de una tripleta (w, x, y) perteneciente a M
 * 
 * Estructura que representa una tripleta del problema 3-Dimensional Matching (3DM).
 * Cada tripleta está compuesta por tres elementos (w, x, y) y un tipo identificador.
 */
struct Tripleta {
    std::string w;    // Primer elemento de la tripleta
    std::string x;    // Segundo elemento de la tripleta
    std::string y;    // Tercer elemento de la tripleta
    std::string tipo; // Identificador: Verdad, Falso, Clausula o Basura
};

#endif // TRIPLETA_H
