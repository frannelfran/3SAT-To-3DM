#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "Tripleta.h"
#include "Clausula.h"
#include <vector>
#include <string>

class JsonUtils {
public:
    // Estructura para devolver el resultado de la lectura
    struct FormulaData {
        int numVars;
        std::vector<Clausula> clausulas;
        bool exito;
    };

    // Lee un archivo JSON con el formato:
    // { "variables": 3, "clauses": [[1, -2, 3], ...] }
    static FormulaData leerFormulaJson(const std::string& filepath);

    // Escribe el resultado en JSON con el formato:
    // { "triplets": [ { "w": "...", "x": "...", "y": "...", "type": "..." }, ... ] }
    static bool guardarResultadoJson(const std::string& filepath, const std::vector<Tripleta>& tripletas, int targetMatching);
};

#endif // JSON_UTILS_H
