/**
 * @file main.cpp
 * @brief Programa interactivo para demostrar la reducción de 3SAT a 3DM
 * @author Proyecto de Complejidad Computacional
 * @date 2025
 */

#include "Reduccion3SATto3DM.h"
#include "Utils.h"
#include "UI.h"
#include "FormulaHandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

int main() {
    limpiarPantalla();
    mostrarBanner();
    pausar(800);
    
    bool continuar = true;
    
    while (continuar) {
        limpiarPantalla();
        mostrarMenu();
        
        int opcion;
        std::cout << "\n🎯 Selecciona una opción: ";
        
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            limpiarBuffer();
            std::cout << "\n❌ Opción inválida. Intenta de nuevo.\n";
            pausar(1500);
            continue;
        }
        limpiarBuffer();
        
        std::vector<Clausula> formula;
        int numVars = 0;
        bool mostrarDetalles = true;
        
        switch (opcion) {
            case 1: {
                // Cargar desde archivo
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        CARGAR MODELO PREDEFINIDO                 ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                auto archivos = obtenerArchivosData();
                
                if (archivos.empty()) {
                    std::cout << "❌ No hay archivos en la carpeta data/\n";
                    pausar(2000);
                    break;
                }
                
                std::cout << "Archivos disponibles:\n\n";
                for (size_t i = 0; i < archivos.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << archivos[i] << "\n";
                }
                
                std::cout << "\n💡 Selecciona un archivo (1-" << archivos.size() << "): ";
                int seleccion;
                
                if (!(std::cin >> seleccion) || seleccion < 1 || seleccion > (int)archivos.size()) {
                    std::cin.clear();
                    limpiarBuffer();
                    std::cout << "\n❌ Selección inválida.\n";
                    pausar(1500);
                    break;
                }
                limpiarBuffer();
                
                std::string filepath = "data/" + archivos[seleccion - 1];
                
                if (cargarDesdeArchivo(filepath, numVars, formula)) {
                    std::cout << "\n✓ Archivo cargado exitosamente\n\n";
                    mostrarFormula(numVars, formula);
                    
                    std::cout << "\n¿Mostrar detalles completos? (s/n): ";
                    char resp;
                    std::cin >> resp;
                    limpiarBuffer();
                    mostrarDetalles = (resp == 's' || resp == 'S');
                    
                    limpiarPantalla();
                    ejecutarReduccion(numVars, formula, mostrarDetalles);
                    
                    if (isatty(fileno(stdin))) {
                        std::cout << "\n\nPresiona Enter para continuar...";
                        std::cin.get();
                    }
                } else {
                    std::cout << "\n❌ Error al cargar el archivo.\n";
                    pausar(2000);
                }
                break;
            }
            
            case 2: {
                // Entrada manual
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        ENTRADA MANUAL DE FÓRMULA                 ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                formula = leerFormulaManual(numVars);
                
                if (!formula.empty()) {
                    std::cout << "\n¿Mostrar detalles completos? (s/n): ";
                    char resp;
                    std::cin >> resp;
                    limpiarBuffer();
                    mostrarDetalles = (resp == 's' || resp == 'S');
                    
                    limpiarPantalla();
                    ejecutarReduccion(numVars, formula, mostrarDetalles);
                    
                    if (isatty(fileno(stdin))) {
                        std::cout << "\n\nPresiona Enter para continuar...";
                        std::cin.get();
                    }
                }
                break;
            }
            
            case 3: {
                // Guardar ejemplo
                limpiarPantalla();
                std::cout << "\n";
                animarTexto("╔══════════════════════════════════════════════════╗\n");
                animarTexto("║        GUARDAR RESULTADOS                        ║\n");
                animarTexto("╚══════════════════════════════════════════════════╝\n");
                std::cout << "\n";
                
                auto archivos = obtenerArchivosData();
                
                if (archivos.empty()) {
                    std::cout << "❌ No hay archivos en la carpeta data/\n";
                    pausar(2000);
                    break;
                }
                
                std::cout << "Selecciona el modelo a guardar:\n\n";
                for (size_t i = 0; i < archivos.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << archivos[i] << "\n";
                }
                
                std::cout << "\n💡 Selecciona (1-" << archivos.size() << "): ";
                int sel;
                
                if (!(std::cin >> sel) || sel < 1 || sel > (int)archivos.size()) {
                    std::cin.clear();
                    limpiarBuffer();
                    std::cout << "\n❌ Selección inválida.\n";
                    pausar(1500);
                    break;
                }
                limpiarBuffer();
                
                std::string filepath = "data/" + archivos[sel - 1];
                
                if (cargarDesdeArchivo(filepath, numVars, formula)) {
                    std::cout << "\nNombre del archivo de salida (sin extensión): ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    
                    filename += ".json";
                    
                    guardarResultados(filename, numVars, formula);
                    pausar(1500);
                } else {
                    std::cout << "\n❌ Error al cargar el archivo.\n";
                    pausar(2000);
                }
                break;
            }
            
            case 4: {
                // Ayuda
                limpiarPantalla();
                mostrarAyuda();
                if (isatty(fileno(stdin))) {
                    std::cout << "\nPresiona Enter para continuar...";
                    std::cin.get();
                }
                break;
            }
            
            case 0: {
                // Salir
                limpiarPantalla();
                std::cout << "\n\n";
                animarTexto("    ╔══════════════════════════════════════════╗\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ║         ¡Hasta luego!                    ║\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ║    Gracias por usar 3SAT-To-3DM          ║\n", 1);
                animarTexto("    ║                                          ║\n", 1);
                animarTexto("    ╚══════════════════════════════════════════╝\n", 1);
                std::cout << "\n\n";
                pausar(800);
                limpiarPantalla();
                continuar = false;
                break;
            }
            
            default:
                std::cout << "\n❌ Opción no válida. Intenta de nuevo.\n";
                pausar(1500);
        }
    }
    
    return 0;
}