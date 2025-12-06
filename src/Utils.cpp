/**
 * @file Utils.cpp
 * @brief Implementación de funciones de utilidad
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <unistd.h>

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

void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
