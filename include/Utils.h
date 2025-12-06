/**
 * @file Utils.h
 * @brief Funciones de utilidad para el programa
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @brief Limpia la pantalla del terminal
 */
void limpiarPantalla();

/**
 * @brief Pausa la ejecución por un tiempo determinado
 * @param ms Milisegundos a pausar
 */
void pausar(int ms = 500);

/**
 * @brief Anima texto carácter por carácter
 * @param texto Texto a animar
 * @param delay Delay entre caracteres en ms
 */
void animarTexto(const std::string& texto, int delay = 1);

/**
 * @brief Limpia el buffer de entrada estándar
 */
void limpiarBuffer();

#endif // UTILS_H
