# Calculadora con Flex y Bison

Este directorio contiene una calculadora en C++ generada con Flex y Bison. El analizador reconoce numeros enteros y flotantes, operaciones aritmeticas basicas, parentesis, numeros negativos y saltos de linea como separadores de expresiones.

## Archivos principales

- `lexer.ll`: especificacion del analizador lexico.
- `calculadora.yy`: especificacion del analizador sintactico.
- `Lexer.cpp` y `Lexer.hpp`: codigo lexico generado y envoltorio C++.
- `Parser.cpp` y `Parser.hpp`: codigo sintactico generado por Bison.
- `main.cpp`: copia antigua del programa principal; no forma parte del flujo de compilacion recomendado.
- `prueba_flotantes.txt` y `prueba_negativos.txt`: archivos de prueba.

## Requisitos

- `g++` con soporte para C++11.
- `bison` 3.2 o superior.
- `flex++` o las cabeceras de Flex para C++ (`FlexLexer.h`).

## Compilacion

Si necesitas regenerar los archivos fuente:

```bash
flex++ lexer.ll
bison calculadora.yy
```

Despues compila el ejecutable con:

```bash
g++ -std=c++11 -Wall Parser.cpp Lexer.cpp -o calculadora
```

Importante: no compiles `main.cpp` junto con `Parser.cpp`, porque `Parser.cpp` ya incluye el punto de entrada del programa y se produciria una definicion duplicada de `main`.

## Ejecucion

```bash
./calculadora prueba_flotantes.txt
./calculadora prueba_negativos.txt
```

## Pruebas incluidas

- `prueba_flotantes.txt`: valida operaciones con flotantes.
- `prueba_negativos.txt`: valida numeros negativos y operadores.
- `prueba_valida3.txt`: operaciones con parentesis y precedencia.
- `prueba_valida4.txt`: multiples lineas con operaciones diversas.
- `prueba_invalida3.txt`: parentesis desbalanceados.
- `prueba_invalida4.txt`: caracteres invalidos o sintaxis erronea.

## Notas

- El proyecto depende de `FlexLexer.h`. Si ese encabezado no esta instalado en el sistema, la compilacion fallara aunque los fuentes esten correctos.
- Los mensajes impresos por el lexer y el parser ayudan a seguir el analisis paso a paso.