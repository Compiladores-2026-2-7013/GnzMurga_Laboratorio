# C_1 — Analizador Léxico y Sintáctico

Analizador léxico y sintáctico para un lenguaje con declaraciones de variables, asignaciones y estructuras de control (`if-else`, `while`). Construido con **Flex** y **Bison** en C++.

---

## Gramática

```
programa       → declaraciones sentencias

declaraciones  → declaraciones declaracion
               | declaracion

declaracion    → tipo lista_var ;

tipo           → int | float

lista_var      → lista_var , identificador
               | identificador

sentencias     → sentencias sentencia
               | sentencia

sentencia      → identificador = expresion ;
               | if ( expresion ) { sentencias } else { sentencias }
               | while ( expresion ) { sentencias }

expresion      → expresion + expresion
               | expresion - expresion
               | expresion * expresion
               | expresion / expresion
               | ( expresion )
               | identificador
               | numero
```

---

## Archivos

| Archivo | Descripción |
|---|---|
| `lexer.ll` | Especificación del analizador léxico (Flex) |
| `gramatica.yy` | Especificación del analizador sintáctico (Bison) |
| `Lexer.hpp` | Clase C++ del lexer para integración con Bison |
| `Makefile` | Compilación y pruebas automatizadas |
| `prueba_valida1.txt` | Declaraciones y asignaciones simples |
| `prueba_valida2.txt` | `while` e `if-else` |
| `prueba_invalida1.txt` | Error: falta `;` en declaración |
| `prueba_invalida2.txt` | Error: falta `;` en sentencia |

---

## Compilación

### Requisitos

- `g++` con soporte C++11
- `flex++` (Flex para C++)
- `bison` 3.2 o superior

En Ubuntu/Debian:
```bash
sudo apt install flex bison g++
```

### Pasos manuales

```bash
# 1. Generar el analizador léxico
flex++ lexer.ll

# 2. Generar el analizador sintáctico
bison gramatica.yy

# 3. Compilar
g++ -std=c++11 -Wall Parser.cpp Lexer.cpp -o ejecut
```

### Con Makefile (recomendado)

```bash
make          # compila todo
make clean    # elimina archivos generados
```

---

## Ejecución

```bash
./ejecut <archivo_entrada>
```

Ejemplos:
```bash
./ejecut prueba_valida1.txt
./ejecut prueba_invalida1.txt
```

---

## Pruebas

```bash
make test           # válidas e inválidas
make test_valid     # solo válidas
make test_invalid   # solo inválidas
```

### Salida esperada — archivo válido

```
  [tipo] int
  [var]  x
  [decl] Declaración reconocida.
  [sent] Asignación → x = 10

✔ Programa analizado correctamente.
```

### Salida esperada — archivo inválido

```
  [tipo] int
  [var]  x

╔══ ERROR SINTÁCTICO ══════════════════════════╗
║  Detalle : syntax error                      ║
╚═════════════════════════════════════════════╝
```

---

## Notas de implementación

- Los saltos de línea son ignorados; las sentencias terminan en `;`
- La división por cero se detecta en tiempo de análisis y reporta un error semántico
- El parser implementa recuperación de errores con `error PUNTOCOMA`: al encontrar un error sintáctico dentro de una sentencia, descarta tokens hasta el siguiente `;` y continúa el análisis
- Los números con punto decimal se clasifican como `float`; sin punto, como `int`
