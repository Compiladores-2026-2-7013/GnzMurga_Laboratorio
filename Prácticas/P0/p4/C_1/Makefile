# Makefile - Analizador léxico y sintáctico C_1
# Uso: make | make test | make clean

CXX      = g++
CXXFLAGS = -std=c++11 -Wall
FLEX     = flex++
BISON    = bison

TARGET      = ejecut
LEXER_SRC   = lexer.ll
PARSER_SRC  = gramatica.yy
LEXER_OUT   = Lexer.cpp
PARSER_OUT  = Parser.cpp
PARSER_HDR  = Parser.hpp

# ── Compilación completa ──────────────────────────────────────────
all: $(TARGET)

$(LEXER_OUT): $(LEXER_SRC)
	@echo "[1/3] Generando analizador léxico..."
	$(FLEX) $(LEXER_SRC)

$(PARSER_OUT) $(PARSER_HDR): $(PARSER_SRC)
	@echo "[2/3] Generando analizador sintáctico..."
	$(BISON) $(PARSER_SRC)

$(TARGET): $(LEXER_OUT) $(PARSER_OUT)
	@echo "[3/3] Compilando ejecutable..."
	$(CXX) $(CXXFLAGS) $(PARSER_OUT) $(LEXER_OUT) -o $(TARGET)
	@echo "Listo: ./$(TARGET) <archivo>"

# ── Pruebas ───────────────────────────────────────────────────────
test: $(TARGET)
	@echo "\n=== Pruebas válidas ==="
	@for f in prueba_valida1.txt prueba_valida2.txt; do \
		echo "\n--- $$f ---"; \
		./$(TARGET) $$f; \
	done
	@echo "\n=== Pruebas inválidas ==="
	@for f in prueba_invalida1.txt prueba_invalida2.txt; do \
		echo "\n--- $$f ---"; \
		./$(TARGET) $$f || true; \
	done

test_valid: $(TARGET)
	@echo "\n=== Pruebas válidas ==="
	@for f in prueba_valida1.txt prueba_valida2.txt; do \
		echo "\n--- $$f ---"; \
		./$(TARGET) $$f; \
	done

test_invalid: $(TARGET)
	@echo "\n=== Pruebas inválidas ==="
	@for f in prueba_invalida1.txt prueba_invalida2.txt; do \
		echo "\n--- $$f ---"; \
		./$(TARGET) $$f || true; \
	done

# ── Limpieza ──────────────────────────────────────────────────────
clean:
	@echo "Limpiando archivos generados..."
	rm -f $(LEXER_OUT) $(PARSER_OUT) $(PARSER_HDR) $(TARGET)
	@echo "✔  Limpieza completada."

.PHONY: all test test_valid test_invalid clean
