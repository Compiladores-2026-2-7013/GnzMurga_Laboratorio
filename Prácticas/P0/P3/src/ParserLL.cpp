#include "headers/ParserLL.hpp"

ParserLL::ParserLL(Lexer *lexer)
{
    this->lexer = lexer;
    loadGrammar();
    loadTable();
}

ParserLL::~ParserLL()
{
}

void ParserLL::loadGrammar() {
    // Configuración inicial: 17 terminales y 18 no terminales
    grammar.setNum(17, 18);
    grammar.start = 1000 + nt_programa;
    
    // ========================================================================
    // Registro de todos los símbolos terminales con sus índices correspondientes
    // ========================================================================
    grammar.addSymbol(t_int, Symbol(t_int));           // Palabra reservada 'int'
    grammar.addSymbol(t_float, Symbol(t_float));       // Palabra reservada 'float'
    grammar.addSymbol(t_if, Symbol(t_if));             // Palabra reservada 'if'
    grammar.addSymbol(t_else, Symbol(t_else));         // Palabra reservada 'else'
    grammar.addSymbol(t_while, Symbol(t_while));       // Palabra reservada 'while'
    grammar.addSymbol(t_id, Symbol(t_id));             // Identificadores de variables
    grammar.addSymbol(t_num, Symbol(t_num));           // Constantes numéricas
    grammar.addSymbol(t_assign, Symbol(t_assign));     // Operador de asignación '='
    grammar.addSymbol(t_plus, Symbol(t_plus));         // Operador suma '+'
    grammar.addSymbol(t_minus, Symbol(t_minus));       // Operador resta '-'
    grammar.addSymbol(t_times, Symbol(t_times));       // Operador multiplicación '*'
    grammar.addSymbol(t_div, Symbol(t_div));           // Operador división '/'
    grammar.addSymbol(t_lparen, Symbol(t_lparen));     // Paréntesis izquierdo '('
    grammar.addSymbol(t_rparen, Symbol(t_rparen));     // Paréntesis derecho ')'
    grammar.addSymbol(t_semicolon, Symbol(t_semicolon)); // Punto y coma ';'
    grammar.addSymbol(t_comma, Symbol(t_comma));       // Coma ','
    grammar.addSymbol(t_eof, Symbol(t_eof));           // Fin de archivo '$'
    
    // ========================================================================
    // Registro de todos los símbolos no terminales (offset de 1000)
    // ========================================================================
    grammar.addSymbol(1000 + nt_empty, Symbol(nt_empty));               // Símbolo épsilon (vacío)
    grammar.addSymbol(1000 + nt_programa, Symbol(nt_programa));         // Símbolo inicial del programa
    grammar.addSymbol(1000 + nt_declaraciones, Symbol(nt_declaraciones)); // Lista de declaraciones
    grammar.addSymbol(1000 + nt_declaraciones_p, Symbol(nt_declaraciones_p)); // Continuación de declaraciones
    grammar.addSymbol(1000 + nt_declaracion, Symbol(nt_declaracion));   // Una declaración individual
    grammar.addSymbol(1000 + nt_tipo, Symbol(nt_tipo));                 // Tipos de datos
    grammar.addSymbol(1000 + nt_lista_var, Symbol(nt_lista_var));       // Lista de variables
    grammar.addSymbol(1000 + nt_lista_var_p, Symbol(nt_lista_var_p));   // Continuación de lista de variables
    grammar.addSymbol(1000 + nt_sentencias, Symbol(nt_sentencias));     // Bloque de sentencias
    grammar.addSymbol(1000 + nt_sentencias_p, Symbol(nt_sentencias_p)); // Continuación de sentencias
    grammar.addSymbol(1000 + nt_sentencia, Symbol(nt_sentencia));       // Una sentencia individual
    grammar.addSymbol(1000 + nt_S_emparejada, Symbol(nt_S_emparejada)); // Sentencia con else emparejado
    grammar.addSymbol(1000 + nt_S_abierta, Symbol(nt_S_abierta));       // Sentencia sin else emparejado
    grammar.addSymbol(1000 + nt_expresion, Symbol(nt_expresion));       // Expresión aritmética
    grammar.addSymbol(1000 + nt_expresion_p, Symbol(nt_expresion_p));   // Continuación de expresión
    grammar.addSymbol(1000 + nt_termino, Symbol(nt_termino));           // Término en expresión
    grammar.addSymbol(1000 + nt_termino_p, Symbol(nt_termino_p));       // Continuación de término
    grammar.addSymbol(1000 + nt_factor, Symbol(nt_factor));             // Factor en expresión
    
    // ========================================================================
    // Definición de todas las producciones de la gramática
    // ========================================================================
    
    // P0: programa → declaraciones sentencias
    Production p0(1000 + nt_programa);
    p0.addSymbol(1000 + nt_declaraciones);
    p0.addSymbol(1000 + nt_sentencias);
    grammar.addProd(0, p0);
    
    // P1: declaraciones → declaracion declaraciones'
    Production p1(1000 + nt_declaraciones);
    p1.addSymbol(1000 + nt_declaracion);
    p1.addSymbol(1000 + nt_declaraciones_p);
    grammar.addProd(1, p1);
    
    // P2: declaraciones' → declaracion declaraciones'
    Production p2(1000 + nt_declaraciones_p);
    p2.addSymbol(1000 + nt_declaracion);
    p2.addSymbol(1000 + nt_declaraciones_p);
    grammar.addProd(2, p2);
    
    // P3: declaraciones' → ε
    Production p3(1000 + nt_declaraciones_p);
    p3.addSymbol(1000 + nt_empty);
    grammar.addProd(3, p3);
    
    // P4: declaracion → tipo lista_var ;
    Production p4(1000 + nt_declaracion);
    p4.addSymbol(1000 + nt_tipo);
    p4.addSymbol(1000 + nt_lista_var);
    p4.addSymbol(t_semicolon);
    grammar.addProd(4, p4);
    
    // P5: tipo → int
    Production p5(1000 + nt_tipo);
    p5.addSymbol(t_int);
    grammar.addProd(5, p5);
    
    // P6: tipo → float
    Production p6(1000 + nt_tipo);
    p6.addSymbol(t_float);
    grammar.addProd(6, p6);
    
    // P7: lista_var → identificador lista_var'
    Production p7(1000 + nt_lista_var);
    p7.addSymbol(t_id);
    p7.addSymbol(1000 + nt_lista_var_p);
    grammar.addProd(7, p7);
    
    // P8: lista_var' → , identificador lista_var'
    Production p8(1000 + nt_lista_var_p);
    p8.addSymbol(t_comma);
    p8.addSymbol(t_id);
    p8.addSymbol(1000 + nt_lista_var_p);
    grammar.addProd(8, p8);
    
    // P9: lista_var' → ε
    Production p9(1000 + nt_lista_var_p);
    p9.addSymbol(1000 + nt_empty);
    grammar.addProd(9, p9);
    
    // P10: sentencias → sentencia sentencias'
    Production p10(1000 + nt_sentencias);
    p10.addSymbol(1000 + nt_sentencia);
    p10.addSymbol(1000 + nt_sentencias_p);
    grammar.addProd(10, p10);
    
    // P11: sentencias' → sentencia sentencias'
    Production p11(1000 + nt_sentencias_p);
    p11.addSymbol(1000 + nt_sentencia);
    p11.addSymbol(1000 + nt_sentencias_p);
    grammar.addProd(11, p11);
    
    // P12: sentencias' → ε
    Production p12(1000 + nt_sentencias_p);
    p12.addSymbol(1000 + nt_empty);
    grammar.addProd(12, p12);
    
    // P13: sentencia → S_emparejada
    Production p13(1000 + nt_sentencia);
    p13.addSymbol(1000 + nt_S_emparejada);
    grammar.addProd(13, p13);
    
    // P14: sentencia → S_abierta
    Production p14(1000 + nt_sentencia);
    p14.addSymbol(1000 + nt_S_abierta);
    grammar.addProd(14, p14);
    
    // P15: S_emparejada → identificador = expresion ;
    Production p15(1000 + nt_S_emparejada);
    p15.addSymbol(t_id);
    p15.addSymbol(t_assign);
    p15.addSymbol(1000 + nt_expresion);
    p15.addSymbol(t_semicolon);
    grammar.addProd(15, p15);
    
    // P16: S_emparejada → while ( expresion ) S_emparejada
    Production p16(1000 + nt_S_emparejada);
    p16.addSymbol(t_while);
    p16.addSymbol(t_lparen);
    p16.addSymbol(1000 + nt_expresion);
    p16.addSymbol(t_rparen);
    p16.addSymbol(1000 + nt_S_emparejada);
    grammar.addProd(16, p16);
    
    // P17: S_emparejada → if ( expresion ) S_emparejada else S_emparejada
    Production p17(1000 + nt_S_emparejada);
    p17.addSymbol(t_if);
    p17.addSymbol(t_lparen);
    p17.addSymbol(1000 + nt_expresion);
    p17.addSymbol(t_rparen);
    p17.addSymbol(1000 + nt_S_emparejada);
    p17.addSymbol(t_else);
    p17.addSymbol(1000 + nt_S_emparejada);
    grammar.addProd(17, p17);
    
    // P18: S_abierta → if ( expresion ) sentencia
    Production p18(1000 + nt_S_abierta);
    p18.addSymbol(t_if);
    p18.addSymbol(t_lparen);
    p18.addSymbol(1000 + nt_expresion);
    p18.addSymbol(t_rparen);
    p18.addSymbol(1000 + nt_sentencia);
    grammar.addProd(18, p18);
    
    // P19: S_abierta → if ( expresion ) S_emparejada else S_abierta
    Production p19(1000 + nt_S_abierta);
    p19.addSymbol(t_if);
    p19.addSymbol(t_lparen);
    p19.addSymbol(1000 + nt_expresion);
    p19.addSymbol(t_rparen);
    p19.addSymbol(1000 + nt_S_emparejada);
    p19.addSymbol(t_else);
    p19.addSymbol(1000 + nt_S_abierta);
    grammar.addProd(19, p19);
    
    // P20: S_abierta → while ( expresion ) S_abierta
    Production p20(1000 + nt_S_abierta);
    p20.addSymbol(t_while);
    p20.addSymbol(t_lparen);
    p20.addSymbol(1000 + nt_expresion);
    p20.addSymbol(t_rparen);
    p20.addSymbol(1000 + nt_S_abierta);
    grammar.addProd(20, p20);
    
    // P21: expresion → termino expresion'
    Production p21(1000 + nt_expresion);
    p21.addSymbol(1000 + nt_termino);
    p21.addSymbol(1000 + nt_expresion_p);
    grammar.addProd(21, p21);
    
    // P22: expresion' → + termino expresion'
    Production p22(1000 + nt_expresion_p);
    p22.addSymbol(t_plus);
    p22.addSymbol(1000 + nt_termino);
    p22.addSymbol(1000 + nt_expresion_p);
    grammar.addProd(22, p22);
    
    // P23: expresion' → - termino expresion'
    Production p23(1000 + nt_expresion_p);
    p23.addSymbol(t_minus);
    p23.addSymbol(1000 + nt_termino);
    p23.addSymbol(1000 + nt_expresion_p);
    grammar.addProd(23, p23);
    
    // P24: expresion' → ε
    Production p24(1000 + nt_expresion_p);
    p24.addSymbol(1000 + nt_empty);
    grammar.addProd(24, p24);
    
    // P25: termino → factor termino'
    Production p25(1000 + nt_termino);
    p25.addSymbol(1000 + nt_factor);
    p25.addSymbol(1000 + nt_termino_p);
    grammar.addProd(25, p25);
    
    // P26: termino' → * factor termino'
    Production p26(1000 + nt_termino_p);
    p26.addSymbol(t_times);
    p26.addSymbol(1000 + nt_factor);
    p26.addSymbol(1000 + nt_termino_p);
    grammar.addProd(26, p26);
    
    // P27: termino' → / factor termino'
    Production p27(1000 + nt_termino_p);
    p27.addSymbol(t_div);
    p27.addSymbol(1000 + nt_factor);
    p27.addSymbol(1000 + nt_termino_p);
    grammar.addProd(27, p27);
    
    // P28: termino' → ε
    Production p28(1000 + nt_termino_p);
    p28.addSymbol(1000 + nt_empty);
    grammar.addProd(28, p28);
    
    // P29: factor → ( expresion )
    Production p29(1000 + nt_factor);
    p29.addSymbol(t_lparen);
    p29.addSymbol(1000 + nt_expresion);
    p29.addSymbol(t_rparen);
    grammar.addProd(29, p29);
    
    // P30: factor → identificador
    Production p30(1000 + nt_factor);
    p30.addSymbol(t_id);
    grammar.addProd(30, p30);
    
    // P31: factor → numero
    Production p31(1000 + nt_factor);
    p31.addSymbol(t_num);
    grammar.addProd(31, p31);
}

void ParserLL::loadTable()
{
  // Construcción de la matriz de análisis sintáctico predictivo
  // Formato: parseTable[NoTerminal][Terminal] = índice_de_producción
  
  // Entradas para el no terminal 'programa'
  parseTable[nt_programa][t_int] = 0;
  parseTable[nt_programa][t_float] = 0;
  
  // Entradas para el no terminal 'declaraciones'
  parseTable[nt_declaraciones][t_int] = 1;
  parseTable[nt_declaraciones][t_float] = 1;
  
  // Entradas para el no terminal 'declaraciones''
  parseTable[nt_declaraciones_p][t_int] = 2;
  parseTable[nt_declaraciones_p][t_float] = 2;
  parseTable[nt_declaraciones_p][t_id] = 3;
  parseTable[nt_declaraciones_p][t_if] = 3;
  parseTable[nt_declaraciones_p][t_while] = 3;
  
  // Entradas para el no terminal 'declaracion'
  parseTable[nt_declaracion][t_int] = 4;
  parseTable[nt_declaracion][t_float] = 4;
  
  // Entradas para el no terminal 'tipo'
  parseTable[nt_tipo][t_int] = 5;
  parseTable[nt_tipo][t_float] = 6;
  
  // Entradas para el no terminal 'lista_var'
  parseTable[nt_lista_var][t_id] = 7;
  
  // Entradas para el no terminal 'lista_var''
  parseTable[nt_lista_var_p][t_comma] = 8;
  parseTable[nt_lista_var_p][t_semicolon] = 9;
  
  // Entradas para el no terminal 'sentencias'
  parseTable[nt_sentencias][t_id] = 10;
  parseTable[nt_sentencias][t_if] = 10;
  parseTable[nt_sentencias][t_while] = 10;
  
  // Entradas para el no terminal 'sentencias''
  parseTable[nt_sentencias_p][t_id] = 11;
  parseTable[nt_sentencias_p][t_if] = 11;
  parseTable[nt_sentencias_p][t_while] = 11;
  parseTable[nt_sentencias_p][t_else] = 12;
  parseTable[nt_sentencias_p][t_eof] = 12;
  
  // Entradas para el no terminal 'sentencia'
  parseTable[nt_sentencia][t_id] = 13;
  parseTable[nt_sentencia][t_if] = 14;
  parseTable[nt_sentencia][t_while] = 13;
  
  // Entradas para los no terminales 'S_emparejada' y 'S_abierta'
  parseTable[nt_S_emparejada][t_id] = 15;
  parseTable[nt_S_emparejada][t_if] = 17;
  parseTable[nt_S_emparejada][t_while] = 16;
  
  parseTable[nt_S_abierta][t_id] = 15;
  parseTable[nt_S_abierta][t_if] = 18;
  parseTable[nt_S_abierta][t_while] = 20;
  
  // Entradas para el no terminal 'expresion'
  parseTable[nt_expresion][t_id] = 21;
  parseTable[nt_expresion][t_num] = 21;
  parseTable[nt_expresion][t_lparen] = 21;
  
  // Entradas para el no terminal 'expresion''
  parseTable[nt_expresion_p][t_plus] = 22;
  parseTable[nt_expresion_p][t_minus] = 23;
  parseTable[nt_expresion_p][t_semicolon] = 24;
  parseTable[nt_expresion_p][t_rparen] = 24;
  
  // Entradas para el no terminal 'termino'
  parseTable[nt_termino][t_id] = 25;
  parseTable[nt_termino][t_num] = 25;
  parseTable[nt_termino][t_lparen] = 25;
  
  // Entradas para el no terminal 'termino''
  parseTable[nt_termino_p][t_times] = 26;
  parseTable[nt_termino_p][t_div] = 27;
  parseTable[nt_termino_p][t_plus] = 28;
  parseTable[nt_termino_p][t_minus] = 28;
  parseTable[nt_termino_p][t_semicolon] = 28;
  parseTable[nt_termino_p][t_rparen] = 28;
  
  // Entradas para el no terminal 'factor'
  parseTable[nt_factor][t_id] = 30;
  parseTable[nt_factor][t_num] = 31;
  parseTable[nt_factor][t_lparen] = 29;
}

void ParserLL::advance() {
    // Lee el siguiente token desde el analizador léxico
    currentToken = (Token)lexer->yylex();
}

void ParserLL::match(Token expected) {
    // Verifica que el token actual coincida con el esperado
    if (currentToken == expected) {
        advance();
    } else {
        error("Token inesperado");
    }
}

void ParserLL::error(string msg) {
    // Imprime mensaje de error y termina la ejecución
    cout << "ERROR SINTACTICO en linea " << lexer->getLine() 
         << ": " << msg << endl;
    exit(1);
}

void ParserLL::parse() {
    cout << "Iniciando analisis sintactico..." << endl;
    
    // Inicialización de la pila: primero el marcador de fin, luego el símbolo inicial
    parseStack.push(t_eof);
    parseStack.push(1000 + nt_programa);
    
    // Obtención del primer token de entrada
    advance();
    
    cout << "DEBUG: Primer token = " << currentToken << endl;
    
    // Ciclo principal del análisis sintáctico
    while (!parseStack.empty()) {
        int top = parseStack.top();
        parseStack.pop();
        
        Symbol topSymbol = grammar.getSym(top);
        
        cout << "DEBUG: Tope pila = " << top 
             << " | Tipo = " << (topSymbol.getType() == terminal ? "TERM" : "NO-TERM") 
             << " | Token actual = " << currentToken << endl;
        
        if (topSymbol.getType() == terminal) {
            // ================================================================
            // Procesamiento de símbolos terminales
            // ================================================================
            Token topToken = topSymbol.getToken();
            
            if (topToken == currentToken) {
                // Coincidencia exitosa: consumir el token
                cout << "  -> Match exitoso del token " << topToken << endl;
                if (currentToken != t_eof) {
                    advance();
                    cout << "  -> Nuevo token leido = " << currentToken << endl;
                }
            } else {
                // Error: el token no coincide con el esperado
                cout << "  -> ERROR: Esperaba token " << topToken 
                     << " pero llego " << currentToken << endl;
                error("Token inesperado");
            }
            
        } else {
            // ================================================================
            // Procesamiento de símbolos no terminales
            // ================================================================
            NoTerm topNT = topSymbol.getNoTerm();
            
            cout << "  -> Buscando tabla[" << topNT << "][" << currentToken << "]" << endl;
            
            // Búsqueda en la tabla de análisis predictivo
            if (parseTable.find(topNT) != parseTable.end() &&
                parseTable[topNT].find(currentToken) != parseTable[topNT].end()) {
                
                int prodIndex = parseTable[topNT][currentToken];
                cout << "  -> Aplicando produccion P" << prodIndex << endl;
                
                Production prod = grammar.getProd(prodIndex);
                vector<int> body = prod.getBody();
                
                cout << "  -> Simbolos en cuerpo: ";
                for (int sym : body) {
                    cout << sym << " ";
                }
                cout << endl;
                
                // Apilado del cuerpo de la producción en orden inverso
                for (int i = body.size() - 1; i >= 0; i--) {
                    Symbol sym = grammar.getSym(body[i]);
                    
                    // Verificación de épsilon para omitirlo del apilado
                    bool isEpsilon = (sym.getType() == nonterminal && 
                                     sym.getNoTerm() == nt_empty);
                    
                    if (!isEpsilon) {
                        parseStack.push(body[i]);
                        cout << "    -> Apilando simbolo " << body[i] << endl;
                    } else {
                        cout << "    -> Saltando epsilon (nt_empty)" << endl;
                    }
                }
            } else {
                // Error: no existe entrada en la tabla para esta combinación
                cout << "  -> ERROR: No existe entrada en tabla" << endl;
                cout << "  -> parseTable tiene " << parseTable.size() << " no-terminales" << endl;
                cout << "  -> Existe topNT en tabla? " 
                     << (parseTable.find(topNT) != parseTable.end() ? "SI" : "NO") << endl;
                
                if (parseTable.find(topNT) != parseTable.end()) {
                    cout << "  -> Tokens disponibles para este no-terminal: ";
                    for (auto& p : parseTable[topNT]) {
                        cout << p.first << " ";
                    }
                    cout << endl;
                }
                
                error("No hay entrada en la tabla de analisis");
            }
        }
    }
    
    cout << " Analisis sintactico completado exitosamente." << endl;
}