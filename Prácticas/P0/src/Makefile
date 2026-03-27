CXX?=g++
EXE=compiler
CXXDEBUG= -g -Wall
CXXSTD=-std=c++14
CXXFLAGS= -Wno-deprecated-register -O0 $(CXXDEBUG) $(CXXSTD)

CPPOBJ= main Parser Lexer
TEST_VALID= prueba_valida_1.txt prueba_valida_2.txt
TEST_INVALID= prueba_invalida_1.txt prueba_invalida_2.txt

FILES= $(addsuffix .cpp, $(CPPOBJ))
OBJS = $(addsuffix .o, $(CPPOBJ))

CLEANLIST = $(OBJS) $(EXE)


.PHONY: all
all: $(EXE)

$(EXE): $(FILES)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)

.PHONY: run
run: $(EXE)
	./$(EXE) prueba

.PHONY: test-valid
test-valid: $(EXE)
	@echo "== Pruebas validas =="
	@for f in $(TEST_VALID); do \
		echo "[OK?] $$f"; \
		./$(EXE) $$f; \
	done

.PHONY: test-invalid
test-invalid: $(EXE)
	@echo "== Pruebas invalidas =="
	@for f in $(TEST_INVALID); do \
		echo "[ERROR?] $$f"; \
		./$(EXE) $$f; \
	done

.PHONY: test
test: test-valid test-invalid

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)
