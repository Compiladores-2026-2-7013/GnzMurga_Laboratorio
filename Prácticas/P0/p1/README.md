Practica-01
Gonzalez Murga Alan Alexzander

¿Qué ocurre si en la primera sección se quitan las llaves al nombre de la macro letra?
Flex dejaría de interpretarlo como una macro definida previamente y lo buscaría como una cadena literal, rompiendo así la lógica del analizador

¿Qué ocurre si en la segunda sección se quitan las llaves a las macros?
El analizador intentará hacer match con el texto literal de la macro

¿Cómo se escribe un comentario en flex?
de forma similar a comentar en C

 ¿Qué se guarda en yytext?
Contiene el texto exacto (lexema) que hizo match con la expresión regular actual.

¿Qué pasa al ejecutar el programa e introducir cadenas de caracteres y de dígitos por la consola?
El programa imprimirá el tipo de token encontrado y el valor

¿Qué ocurre si introducimos caracteres como "*" en la consola?

Si no hay una regla que coincida con “*”, Flex aplica la regla por defecto: imprimir el carácter en la salida estándar
