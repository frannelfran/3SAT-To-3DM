#include "JsonUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Función auxiliar muy simple para limpiar caracteres de formato JSON
std::string limpiarToken(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
    s.erase(std::remove(s.begin(), s.end(), ','), s.end());
    s.erase(std::remove(s.begin(), s.end(), ':'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '['), s.end());
    s.erase(std::remove(s.begin(), s.end(), ']'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '{'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '}'), s.end());
    return s;
}

JsonUtils::FormulaData JsonUtils::leerFormulaJson(const std::string& filepath) {
    FormulaData data = {0, {}, false};
    std::ifstream file(filepath);
    
    if (!file.is_open()) return data;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Parser muy básico específico para este formato
    // Busca "variables": X
    size_t varPos = content.find("\"variables\"");
    if (varPos != std::string::npos) {
        size_t colonPos = content.find(":", varPos);
        size_t commaPos = content.find(",", colonPos);
        if (colonPos != std::string::npos) {
            std::string numStr = content.substr(colonPos + 1, commaPos - colonPos - 1);
            data.numVars = std::stoi(limpiarToken(numStr));
        }
    }

    // Busca "clauses": [[...], [...]]
    size_t clausesPos = content.find("\"clauses\"");
    if (clausesPos != std::string::npos) {
        size_t startArr = content.find("[", clausesPos);
        size_t endArr = content.rfind("]"); // Último corchete
        
        if (startArr != std::string::npos && endArr != std::string::npos) {
            std::string arrayContent = content.substr(startArr + 1, endArr - startArr - 1);
            
            // Procesar manualmente los subarrays
            std::vector<int> currentClause;
            std::string currentNum;
            bool inNumber = false;
            
            for (size_t i = 0; i < arrayContent.length(); ++i) {
                char c = arrayContent[i];
                
                if (c == '-' || isdigit(c)) {
                    currentNum += c;
                    inNumber = true;
                } else if (inNumber) {
                    currentClause.push_back(std::stoi(currentNum));
                    currentNum = "";
                    inNumber = false;
                    
                    if (currentClause.size() == 3) {
                        data.clausulas.push_back({currentClause[0], currentClause[1], currentClause[2]});
                        currentClause.clear();
                    }
                }
            }
        }
    }

    data.exito = (data.numVars > 0 && !data.clausulas.empty());
    return data;
}

bool JsonUtils::guardarResultadoJson(const std::string& filepath, const std::vector<Tripleta>& tripletas) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << "{\n";
    file << "  \"triplets\": [\n";

    for (size_t i = 0; i < tripletas.size(); ++i) {
        const auto& t = tripletas[i];
        file << "    {\n";
        file << "      \"w\": \"" << t.w << "\",\n";
        file << "      \"x\": \"" << t.x << "\",\n";
        file << "      \"y\": \"" << t.y << "\",\n";
        file << "      \"type\": \"" << t.tipo << "\"\n";
        file << "    }" << (i < tripletas.size() - 1 ? "," : "") << "\n";
    }

    file << "  ]\n";
    file << "}\n";
    
    return true;
}
