# Reporte de la Practica P2

## 1. Gramatica definida
Se implemento la siguiente gramatica para el analisis sintactico:

- programa -> declaraciones sentencias EOF
- declaraciones -> declaracion declaraciones | e
- declaracion -> tipo lista_ids ;
- tipo -> int | float
- lista_ids -> id ( , id )*
- sentencias -> sentencia sentencias | e
- sentencia -> id = expr ;
- expr -> termino ( + termino )*
- termino -> id | num | ( expr )

## 2. Lenguaje y conjuntos formales
- Lenguaje: asignaciones con declaraciones previas de variables `int` o `float`, usando expresiones con suma y parentesis.

### 2.1 Conjuntos de la gramatica base (enunciado)
- N0 = { programa, declaraciones, declaracion, tipo, lista_var, sentencias, sentencia, expresion }
- Sigma0 = { int, float, id, numero, ',', ';', '=', '+', '-', '*', '/', 'if', 'else', 'while', '(', ')' }
- S0 = programa
- P0 = {
   - programa -> declaraciones sentencias
   - declaraciones -> declaraciones declaracion | declaracion
   - declaracion -> tipo lista_var ;
   - tipo -> int | float
   - lista_var -> lista_var , identificador | identificador
   - sentencias -> sentencias sentencia | sentencia
   - sentencia -> identificador = expresion ; | if ( expresion ) sentencias else sentencias | while ( expresion ) sentencias
   - expresion -> expresion + expresion | expresion - expresion | expresion * expresion | expresion / expresion | identificador | numero | ( expresion )
}

### 2.2 Conjuntos actualizados (tras eliminacion de recursividad/factorizacion para descenso recursivo)
- N = { programa, declaraciones, declaracion, tipo, lista_ids, lista_ids_p, sentencias, sentencia, expr, expr_p, termino }
- Sigma = { int, float, id, num, ',', ';', '=', '+', '(', ')', EOF }
- S = programa
- P = {
   - programa -> declaraciones sentencias EOF
   - declaraciones -> declaracion declaraciones | e
   - declaracion -> tipo lista_ids ;
   - tipo -> int | float
   - lista_ids -> id lista_ids_p
   - lista_ids_p -> , id lista_ids_p | e
   - sentencias -> sentencia sentencias | e
   - sentencia -> id = expr ;
   - expr -> termino expr_p
   - expr_p -> + termino expr_p | e
   - termino -> id | num | ( expr )
}

## 3. Actividades realizadas
1. Se completo la enumeracion de terminales y no terminales en `headers/Symbols.hpp`.
2. Se implemento un lexer manual en `Lexer.cpp` y `headers/Lexer.hpp` para reconocer:
   - Palabras reservadas: `int`, `float`
   - Identificadores (incluyendo `_`)
   - Numeros enteros
   - Simbolos: `, ; = + ( )`
   - Fin de archivo y error lexico
3. Se implemento el parser recursivo descendente en `Parser.cpp` y `headers/Parser.hpp`.
4. Se corrigio el `Makefile` para compilar los objetos reales del proyecto (`main`, `Parser`, `Lexer`) y usar `g++` por defecto.

## 4. Prueba ejecutada
Archivo de entrada probado: `prueba`

Comando ejecutado en Windows:

```powershell
g++ -std=c++14 -Wall -O0 main.cpp Parser.cpp Lexer.cpp -o compiler
./compiler prueba
```

Salida obtenida:

```text
La cadena es aceptada
```

## 5. Estado final
- El proyecto compila correctamente con `g++`.
- El parser acepta el archivo de prueba proporcionado.
- Los errores de sintaxis reportan linea mediante el lexer.
