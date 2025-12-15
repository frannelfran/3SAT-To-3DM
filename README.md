# 3SAT-To-3DM

## Descripción

**Programa interactivo** con interfaz visual que implementa la reducción polinomial del problema **3-Satisfiability (3SAT)** al problema **3-Dimensional Matching (3DM)**, demostrando que 3DM es NP-Completo.

### Características Principales

- **Interfaz interactiva** con menú animado y visual
- **Carga de modelos** desde archivos en carpeta `data/`
- **Entrada manual** de fórmulas 3SAT con validación
- **Exportar resultados** a archivos JSON

## 🔧 Compilación

### Requisitos

- Compilador C++ con soporte para C++17 o superior (g++, clang++)
- Make

### Comandos de Compilación

```bash
# Compilación normal
make

# Compilación con símbolos de depuración
make debug

# Compilación optimizada
make release

# Compilar y ejecutar
make run

# Limpiar archivos compilados
make clean

# Ver todos los comandos disponibles
make help
```

## Uso

### Modo Interactivo

```bash
# Ejecutar el programa interactivo
./bin/3sat-to-3dm

# O usar el comando make
make run
```

### Características Interactivas

El programa ofrece un **menú interactivo visual** con las siguientes opciones:

1. **Cargar modelo predefinido**: 
   - Carga automáticamente archivos desde la carpeta `data/`
   - Detecta dinámicamente los archivos disponibles
   - Muestra la fórmula antes de ejecutar

2. **Entrada Manual**: 
   - Define tu propia fórmula 3SAT
   - Validación automática de datos
   - Confirmación visual de la fórmula

3. **Guardar Resultados**: 
   - Selecciona un modelo predefinido
   - Exporta la reducción a archivo .txt
json
4. **Ayuda**: 
   - Explicación de conceptos clave
   - Guía de notación y formato de archivos

### Formato de Archivos (data/*.json)

```json
{
  "numVars": <numero_variables>,
  "clausulas": [
    [<lit1>, <lit2>, <lit3>],
    [<lit1>, <lit2>, <lit3>],
    ...
  ]
}
```

**Ejemplo** (`data/ejemplo_json.json`):
```json
{
  "numVars": 3,
  "clausulas": [
    [1, -2, -3],
    [-1, -2, 3]
  ]
}
```

Esto representa: (p ∨ ¬q ∨ ¬r) ∧ (¬p ∨ ¬q ∨ r)

### Formato de Salida (out/*.json)

```json
{
  "totalTriplets": <total_tripletas>,
  "targetMatchingSize": <tamano_matching_objetivo>,
  "triplets": [
    {
      "w": "<elemento_w>",
      "x": "<elemento_x>",
      "y": "<elemento_y>",
      "type": "<tipo_tripleta>"
    },
    ...
  ]
}
```

## Componentes de la Reducción

### 1. **Truth-Setting (Configuración de Verdad)**
Crea componentes para cada variable que fuerzan a elegir `True` o `False`. Se genera un "anillo" de tripletas por cada variable.

### 2. **Satisfaction Testing (Comprobación de Satisfacción)**
Genera tripletas que conectan las cláusulas con los "tips" (puntas) libres de las variables, permitiendo verificar si las cláusulas se satisfacen.

### 3. **Garbage Collection (Recolección de Basura)**
Añade tripletas adicionales para asegurar que el matching perfecto tenga la cardinalidad correcta (`n × m` tripletas).

## Representación de Datos

### Cláusula
Cada cláusula contiene tres literales representados como enteros:
- **Positivos**: variable sin negar (ej: `1` = p, `2` = q, `3` = r)
- **Negativos**: variable negada (ej: `-1` = ¬p, `-2` = ¬q, `-3` = ¬r)

### Tripleta
Estructura `(w, x, y)` que representa un elemento del conjunto `M` en el problema 3DM, con un campo `tipo` para identificar su propósito:
- `Var-X-True`: Asignación verdadera de variable
- `Var-X-False`: Asignación falsa de variable
- `Clausula-N`: Satisfacción de cláusula
- `Garbage`: Elemento de relleno

## Referencias

Este proyecto está basado en la teoría de NP-Completitud y las reducciones polinomiales estudiadas en cursos de Complejidad Computacional.

## Desarrollo

Para modificar o extender el proyecto:

1. Los **headers** están en `include/`
2. Las **implementaciones** están en `src/`
3. Cada componente está separado en su propio archivo para facilitar el mantenimiento
4. El `main.cpp` es minimalista y solo orquesta las llamadas necesarias

## Licencia

Proyecto académico para el curso de Complejidad Computacional.