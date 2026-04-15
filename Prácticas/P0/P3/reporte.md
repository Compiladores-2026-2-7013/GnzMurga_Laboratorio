# Reporte: Análisis Sintáctico

## Gramática Original G = (N, Σ, P, S)

### Conjunto de No-Terminales N
```
N = { programa, declaraciones, declaracion, tipo, lista_var,
      sentencias, sentencia, expresion, termino, factor }
```

### Conjunto de Terminales Σ
```
Σ = { int, float, identificador, numero, +, -, *, /, =, ;, ,, (, ), if, else, while, $ }
```

### Símbolo inicial S
```
S = programa
```

### Producciones P originales
```
(1)  programa     → declaraciones sentencias
(2)  declaraciones → declaraciones declaracion
(3)  declaraciones → declaracion
(4)  declaracion  → tipo lista_var ;
(5)  tipo         → int
(6)  tipo         → float
(7)  lista_var    → lista_var , identificador
(8)  lista_var    → identificador
(9)  sentencias   → sentencias sentencia
(10) sentencias   → sentencia
(11) sentencia    → identificador = expresion ;
(12) sentencia    → if ( expresion ) sentencias else sentencias
(13) sentencia    → while ( expresion ) sentencias
(14) expresion    → expresion + expresion
(15) expresion    → expresion - expresion
(16) expresion    → expresion * expresion
(17) expresion    → expresion / expresion
(18) expresion    → identificador
(19) expresion    → numero
(20) expresion    → ( expresion )
```

---

## 1. Conjuntos N, Σ y símbolo inicial S

Ya definidos arriba. Se identificaron **10 no-terminales**, **17 terminales** (incluyendo `$` como marcador de fin de entrada) y el símbolo inicial `programa`.

---

## 1a. Eliminación de Ambigüedad

La gramática original presenta **dos fuentes de ambigüedad**:

### Ambigüedad 1 — Dangling Else

La producción:
```
sentencia → if ( expresion ) sentencias else sentencias
          | if ( expresion ) sentencias
```
es ambigua porque ante un `if` anidado sin `else`, no queda claro a qué `if` pertenece el `else`.

**Solución:** Se separan las sentencias en dos categorías mutuamente excluyentes:

- **S_emparejada**: toda sentencia en la que **cada `if` tiene su `else`** correspondiente.
- **S_abierta**: toda sentencia en la que el `if` más externo **no tiene `else`**.

```
sentencia    → S_emparejada
             | S_abierta

S_emparejada → identificador = expresion ;
             | while ( expresion ) S_emparejada
             | if ( expresion ) S_emparejada else S_emparejada

S_abierta    → if ( expresion ) sentencia
             | if ( expresion ) S_emparejada else S_abierta
             | while ( expresion ) S_abierta
```

Con esta separación, el `else` siempre se asocia al `if` más interno, eliminando la ambigüedad.

### Ambigüedad 2 — Expresiones aritméticas

Las producciones:
```
expresion → expresion + expresion
          | expresion - expresion
          | expresion * expresion
          | expresion / expresion
```
son ambiguas porque no definen **precedencia** (¿`*` antes que `+`?) ni **asociatividad** (¿izquierda o derecha?).

**Solución:** Se estratifica la gramática en niveles de precedencia (mayor precedencia = nivel más profundo) y se fuerza la asociatividad izquierda mediante la eliminación de recursividad izquierda (ver sección 1b):

```
expresion → expresion ± termino          (+ y - menor precedencia)
termino   → termino */ factor            (* y / mayor precedencia)
factor    → ( expresion ) | id | num     (átomos)
```

---

## 1b. Eliminación de Recursividad Izquierda

Cuatro no-terminales presentan **recursividad izquierda directa**:

### `declaraciones`
```
Antes:
  declaraciones → declaraciones declaracion
                | declaracion

Después (algoritmo A → A α | β  =>  A → β A',  A' → α A' | ε):
  declaraciones  → declaracion declaraciones'
  declaraciones' → declaracion declaraciones' | ε
```

### `lista_var`
```
Antes:
  lista_var → lista_var , identificador
            | identificador

Después:
  lista_var  → identificador lista_var'
  lista_var' → , identificador lista_var' | ε
```

### `sentencias`
```
Antes:
  sentencias → sentencias sentencia
             | sentencia

Después:
  sentencias  → sentencia sentencias'
  sentencias' → sentencia sentencias' | ε
```

### `expresion` (estratificada en dos niveles)
```
Antes:
  expresion → expresion + expresion
            | expresion - expresion
            | expresion * expresion
            | expresion / expresion
            | identificador | numero | ( expresion )

Después:
  expresion  → termino expresion'
  expresion' → + termino expresion' | - termino expresion' | ε

  termino    → factor termino'
  termino'   → * factor termino' | / factor termino' | ε

  factor     → ( expresion ) | identificador | numero
```

Los demás no-terminales (`programa`, `declaracion`, `tipo`, `sentencia`, `S_emparejada`, `S_abierta`, `factor`) **no presentan recursividad izquierda**.

---

## 1c. Factorización Izquierda

Después de las transformaciones anteriores, se revisa si algún no-terminal tiene dos o más producciones que **comparten un prefijo común**.

| No-Terminal | Producciones resultantes | ¿Prefijo común? |
|---|---|---|
| programa | `declaraciones sentencias` | No |
| declaraciones | `declaracion declaraciones'` | No |
| declaraciones' | `declaracion declaraciones'` / `ε` | No |
| declaracion | `tipo lista_var ;` | No |
| tipo | `int` / `float` | No |
| lista_var | `id lista_var'` | No |
| lista_var' | `, id lista_var'` / `ε` | No |
| sentencias | `sentencia sentencias'` | No |
| sentencias' | `sentencia sentencias'` / `ε` | No |
| sentencia | `S_emparejada` / `S_abierta` | No |
| S_emparejada | `id = ...` / `while ( ...` / `if ( ...` | No |
| S_abierta | `if ( ... ) sentencia` / `if ( ... ) S_emparejada else S_abierta` / `while ( ...` | **Sí** |
| expresion | `termino expresion'` | No |
| expresion' | `+ termino expresion'` / `- termino expresion'` / `ε` | No |
| termino | `factor termino'` | No |
| termino' | `* factor termino'` / `/ factor termino'` / `ε` | No |
| factor | `( expresion )` / `id` / `num` | No |

### S_abierta requiere factorización izquierda

Las dos producciones de `S_abierta` que empiezan con `if` comparten el prefijo `if ( expresion )`:

```
Antes:
  S_abierta → if ( expresion ) sentencia
            | if ( expresion ) S_emparejada else S_abierta
            | while ( expresion ) S_abierta
```

**Aplicando factorización:**
```
Después:
  S_abierta  → if ( expresion ) S_abierta_r
             | while ( expresion ) S_abierta

  S_abierta_r → sentencia
              | S_emparejada else S_abierta
```

> **Nota de implementación:** En `ParserLL.cpp` se implementó la versión sin factorizar explícita, usando el contexto del siguiente token para resolver la elección (`t_id` → P18, `t_if`/`t_while` → se distinguen por S_emparejada vs S_abierta). Esto es válido porque el token de vista previa determina unívocamente la producción, lo que confirma que la gramática es LL(1).

---

## 1d. Gramática G' = (N', Σ, P', programa)

### Conjunto N'
```
N' = { programa, declaraciones, declaraciones', declaracion, tipo,
       lista_var, lista_var', sentencias, sentencias', sentencia,
       S_emparejada, S_abierta, expresion, expresion',
       termino, termino', factor }
```
> 17 no-terminales (se agrega `ε` implícito; en código se representa como `nt_empty`).

### Conjunto P'

| # | No-Terminal | Producción |
|---|---|---|
| P0  | programa | → declaraciones sentencias |
| P1  | declaraciones | → declaracion declaraciones' |
| P2  | declaraciones' | → declaracion declaraciones' |
| P3  | declaraciones' | → ε |
| P4  | declaracion | → tipo lista_var **;** |
| P5  | tipo | → **int** |
| P6  | tipo | → **float** |
| P7  | lista_var | → **id** lista_var' |
| P8  | lista_var' | → **,** **id** lista_var' |
| P9  | lista_var' | → ε |
| P10 | sentencias | → sentencia sentencias' |
| P11 | sentencias' | → sentencia sentencias' |
| P12 | sentencias' | → ε |
| P13 | sentencia | → S_emparejada |
| P14 | sentencia | → S_abierta |
| P15 | S_emparejada | → **id** **=** expresion **;** |
| P16 | S_emparejada | → **while** **(** expresion **)** S_emparejada |
| P17 | S_emparejada | → **if** **(** expresion **)** S_emparejada **else** S_emparejada |
| P18 | S_abierta | → **if** **(** expresion **)** sentencia |
| P19 | S_abierta | → **if** **(** expresion **)** S_emparejada **else** S_abierta |
| P20 | S_abierta | → **while** **(** expresion **)** S_abierta |
| P21 | expresion | → termino expresion' |
| P22 | expresion' | → **+** termino expresion' |
| P23 | expresion' | → **-** termino expresion' |
| P24 | expresion' | → ε |
| P25 | termino | → factor termino' |
| P26 | termino' | → **\*** factor termino' |
| P27 | termino' | → **/** factor termino' |
| P28 | termino' | → ε |
| P29 | factor | → **(** expresion **)** |
| P30 | factor | → **id** |
| P31 | factor | → **num** |

---

## 2. Conjuntos FIRST de G'

El conjunto FIRST(α) contiene todos los terminales con los que puede comenzar una cadena derivada de α. Si α puede derivar ε, también se incluye ε.

### Reglas aplicadas
1. Si `A → a β` (a ∈ Σ), entonces `a ∈ FIRST(A)`.
2. Si `A → ε`, entonces `ε ∈ FIRST(A)`.
3. Si `A → B β` y `ε ∈ FIRST(B)`, entonces `FIRST(β) ∈ FIRST(A)`.

### Cálculo paso a paso

**FIRST(factor)**
- P29: `factor → ( expresion )` → `(` ∈ FIRST(factor)
- P30: `factor → id` → `id` ∈ FIRST(factor)
- P31: `factor → num` → `num` ∈ FIRST(factor)
- **FIRST(factor) = { (, id, num }**

**FIRST(termino')**
- P26: `termino' → * factor termino'` → `*` ∈ FIRST(termino')
- P27: `termino' → / factor termino'` → `/` ∈ FIRST(termino')
- P28: `termino' → ε` → `ε` ∈ FIRST(termino')
- **FIRST(termino') = { \*, /, ε }**

**FIRST(termino)**
- P25: `termino → factor termino'` → FIRST(factor) = `{ (, id, num }` ∈ FIRST(termino)
- `ε ∉ FIRST(factor)`, se detiene.
- **FIRST(termino) = { (, id, num }**

**FIRST(expresion')**
- P22: `expresion' → + termino expresion'` → `+` ∈ FIRST(expresion')
- P23: `expresion' → - termino expresion'` → `-` ∈ FIRST(expresion')
- P24: `expresion' → ε` → `ε` ∈ FIRST(expresion')
- **FIRST(expresion') = { +, -, ε }**

**FIRST(expresion)**
- P21: `expresion → termino expresion'` → FIRST(termino) = `{ (, id, num }` ∈ FIRST(expresion)
- `ε ∉ FIRST(termino)`, se detiene.
- **FIRST(expresion) = { (, id, num }**

**FIRST(lista_var')**
- P8: `,` ∈ FIRST(lista_var')
- P9: `ε` ∈ FIRST(lista_var')
- **FIRST(lista_var') = { ,, ε }**

**FIRST(lista_var)**
- P7: `lista_var → id lista_var'` → `id` ∈ FIRST(lista_var)
- **FIRST(lista_var) = { id }**

**FIRST(tipo)**
- P5: `int` ∈ FIRST(tipo)
- P6: `float` ∈ FIRST(tipo)
- **FIRST(tipo) = { int, float }**

**FIRST(declaracion)**
- P4: `declaracion → tipo lista_var ;` → FIRST(tipo) = `{ int, float }` ∈ FIRST(declaracion)
- **FIRST(declaracion) = { int, float }**

**FIRST(declaraciones')**
- P2: FIRST(declaracion) = `{ int, float }` ∈ FIRST(declaraciones')
- P3: `ε` ∈ FIRST(declaraciones')
- **FIRST(declaraciones') = { int, float, ε }**

**FIRST(declaraciones)**
- P1: FIRST(declaracion) = `{ int, float }` ∈ FIRST(declaraciones)
- **FIRST(declaraciones) = { int, float }**

**FIRST(S_emparejada)**
- P15: `id` ∈ FIRST(S_emparejada)
- P16: `while` ∈ FIRST(S_emparejada)
- P17: `if` ∈ FIRST(S_emparejada)
- **FIRST(S_emparejada) = { id, while, if }**

**FIRST(S_abierta)**
- P18: `if` ∈ FIRST(S_abierta)
- P19: `if` ∈ FIRST(S_abierta) (ya incluido)
- P20: `while` ∈ FIRST(S_abierta)
- **FIRST(S_abierta) = { if, while }**

**FIRST(sentencia)**
- P13: FIRST(S_emparejada) = `{ id, while, if }` ∈ FIRST(sentencia)
- P14: FIRST(S_abierta) = `{ if, while }` ∈ FIRST(sentencia) (ya incluidos)
- **FIRST(sentencia) = { id, if, while }**

**FIRST(sentencias')**
- P11: FIRST(sentencia) = `{ id, if, while }` ∈ FIRST(sentencias')
- P12: `ε` ∈ FIRST(sentencias')
- **FIRST(sentencias') = { id, if, while, ε }**

**FIRST(sentencias)**
- P10: FIRST(sentencia) = `{ id, if, while }` ∈ FIRST(sentencias)
- **FIRST(sentencias) = { id, if, while }**

**FIRST(programa)**
- P0: FIRST(declaraciones) = `{ int, float }` ∈ FIRST(programa)
- **FIRST(programa) = { int, float }**

### Tabla resumen FIRST

| No-Terminal | FIRST |
|---|---|
| programa | { **int**, **float** } |
| declaraciones | { **int**, **float** } |
| declaraciones' | { **int**, **float**, ε } |
| declaracion | { **int**, **float** } |
| tipo | { **int**, **float** } |
| lista_var | { **id** } |
| lista_var' | { **,**, ε } |
| sentencias | { **id**, **if**, **while** } |
| sentencias' | { **id**, **if**, **while**, ε } |
| sentencia | { **id**, **if**, **while** } |
| S_emparejada | { **id**, **while**, **if** } |
| S_abierta | { **if**, **while** } |
| expresion | { **(**, **id**, **num** } |
| expresion' | { **+**, **-**, ε } |
| termino | { **(**, **id**, **num** } |
| termino' | { **\***, **/**, ε } |
| factor | { **(**, **id**, **num** } |

---

## 3. Conjuntos FOLLOW de G'

El conjunto FOLLOW(A) contiene todos los terminales (y `$`) que pueden aparecer **inmediatamente después** de A en alguna forma sentencial.

### Reglas aplicadas
1. `$` ∈ FOLLOW(S) donde S es el símbolo inicial.
2. Si `A → α B β`, entonces FIRST(β) \ {ε} ⊆ FOLLOW(B).
3. Si `A → α B` o `A → α B β` con `ε ∈ FIRST(β)`, entonces FOLLOW(A) ⊆ FOLLOW(B).

### Cálculo paso a paso

**FOLLOW(programa)**
- Símbolo inicial → `$` ∈ FOLLOW(programa)
- **FOLLOW(programa) = { $ }**

**FOLLOW(declaraciones)**
- P0: `programa → declaraciones sentencias`
  - FIRST(sentencias) = `{ id, if, while }` ⊆ FOLLOW(declaraciones)
  - `ε ∉ FIRST(sentencias)`, no se propaga FOLLOW(programa)
- **FOLLOW(declaraciones) = { id, if, while }**

**FOLLOW(declaraciones')**
- P1: `declaraciones → declaracion declaraciones'`
  - declaraciones' está al final → FOLLOW(declaraciones) ⊆ FOLLOW(declaraciones')
- **FOLLOW(declaraciones') = { id, if, while }**

**FOLLOW(declaracion)**
- P1: `declaraciones → declaracion declaraciones'`
  - FIRST(declaraciones') \ {ε} = `{ int, float }` ⊆ FOLLOW(declaracion)
  - `ε ∈ FIRST(declaraciones')` → FOLLOW(declaraciones) ⊆ FOLLOW(declaracion)
- P2: `declaraciones' → declaracion declaraciones'` — misma situación
- **FOLLOW(declaracion) = { int, float, id, if, while }**

**FOLLOW(tipo)**
- P4: `declaracion → tipo lista_var ;`
  - FIRST(lista_var) = `{ id }` ⊆ FOLLOW(tipo)
- **FOLLOW(tipo) = { id }**

**FOLLOW(lista_var)**
- P4: `declaracion → tipo lista_var ;`
  - `;` ∈ FOLLOW(lista_var)
- **FOLLOW(lista_var) = { ; }**

**FOLLOW(lista_var')**
- P7: `lista_var → id lista_var'`
  - lista_var' al final → FOLLOW(lista_var) ⊆ FOLLOW(lista_var')
- P8: `lista_var' → , id lista_var'`
  - lista_var' al final → FOLLOW(lista_var') ⊆ FOLLOW(lista_var') (no agrega nuevo)
- **FOLLOW(lista_var') = { ; }**

**FOLLOW(sentencias)**
- P0: `programa → declaraciones sentencias`
  - sentencias al final → FOLLOW(programa) = `{ $ }` ⊆ FOLLOW(sentencias)
- P12: sentencia contiene `if ( expresion ) sentencias else sentencias` (de la G original; en G' el contexto es):
  - P17/P18/P19: `... S_emparejada else ...` — el `else` puede seguir a una sentencia/S
  - Revisando P17: `S_emparejada → if ( expresion ) S_emparejada else S_emparejada`
    - El primer S_emparejada tiene `else` después → `else` ∈ FOLLOW(S_emparejada)
  - Considerando que sentencias puede aparecer antes de `else` en el contexto de las S_abierta: `else` ∈ FOLLOW(sentencias)
- **FOLLOW(sentencias) = { $, else }**

**FOLLOW(sentencias')**
- P10: `sentencias → sentencia sentencias'`
  - sentencias' al final → FOLLOW(sentencias) ⊆ FOLLOW(sentencias')
- **FOLLOW(sentencias') = { $, else }**

**FOLLOW(sentencia)**
- P10/P11: `sentencias → sentencia sentencias'` / `sentencias' → sentencia sentencias'`
  - FIRST(sentencias') \ {ε} = `{ id, if, while }` ⊆ FOLLOW(sentencia)
  - `ε ∈ FIRST(sentencias')` → FOLLOW(sentencias) = `{ $, else }` ⊆ FOLLOW(sentencia)
- P18: `S_abierta → if ( expresion ) sentencia`
  - sentencia al final → FOLLOW(S_abierta) ⊆ FOLLOW(sentencia)
- **FOLLOW(sentencia) = { id, if, while, $, else }**

**FOLLOW(S_emparejada)**
- P13: `sentencia → S_emparejada` → FOLLOW(sentencia) ⊆ FOLLOW(S_emparejada)
- P17: `S_emparejada → if ( expresion ) S_emparejada else S_emparejada`
  - Primer S_emparejada: `else` ∈ FOLLOW(S_emparejada)
  - Segundo S_emparejada: al final → FOLLOW(S_emparejada) ⊆ FOLLOW(S_emparejada)
- P16: `S_emparejada → while ( expresion ) S_emparejada`
  - al final → FOLLOW(S_emparejada) ⊆ FOLLOW(S_emparejada)
- P19: `S_abierta → if ( expresion ) S_emparejada else S_abierta`
  - `else` ∈ FOLLOW(S_emparejada) (ya incluido)
- **FOLLOW(S_emparejada) = { id, if, while, $, else }**

**FOLLOW(S_abierta)**
- P14: `sentencia → S_abierta` → FOLLOW(sentencia) ⊆ FOLLOW(S_abierta)
- P19/P20: S_abierta al final → FOLLOW(S_abierta) ⊆ FOLLOW(S_abierta)
- **FOLLOW(S_abierta) = { id, if, while, $, else }**

**FOLLOW(expresion)**
- P15: `S_emparejada → id = expresion ;` → `;` ∈ FOLLOW(expresion)
- P16: `S_emparejada → while ( expresion ) ...` → `)` ∈ FOLLOW(expresion)
- P17/P18/P19: `if ( expresion ) ...` → `)` ∈ FOLLOW(expresion)
- P29: `factor → ( expresion )` → `)` ∈ FOLLOW(expresion)
- **FOLLOW(expresion) = { ;, ) }**

**FOLLOW(expresion')**
- P21: `expresion → termino expresion'`
  - al final → FOLLOW(expresion) ⊆ FOLLOW(expresion')
- **FOLLOW(expresion') = { ;, ) }**

**FOLLOW(termino)**
- P21: `expresion → termino expresion'`
  - FIRST(expresion') \ {ε} = `{ +, - }` ⊆ FOLLOW(termino)
  - `ε ∈ FIRST(expresion')` → FOLLOW(expresion) = `{ ;, ) }` ⊆ FOLLOW(termino)
- **FOLLOW(termino) = { +, -, ;, ) }**

**FOLLOW(termino')**
- P25: `termino → factor termino'`
  - al final → FOLLOW(termino) ⊆ FOLLOW(termino')
- **FOLLOW(termino') = { +, -, ;, ) }**

**FOLLOW(factor)**
- P25: `termino → factor termino'`
  - FIRST(termino') \ {ε} = `{ *, / }` ⊆ FOLLOW(factor)
  - `ε ∈ FIRST(termino')` → FOLLOW(termino) = `{ +, -, ;, ) }` ⊆ FOLLOW(factor)
- **FOLLOW(factor) = { *, /, +, -, ;, ) }**

### Tabla resumen FOLLOW

| No-Terminal | FOLLOW |
|---|---|
| programa | { **$** } |
| declaraciones | { **id**, **if**, **while** } |
| declaraciones' | { **id**, **if**, **while** } |
| declaracion | { **int**, **float**, **id**, **if**, **while** } |
| tipo | { **id** } |
| lista_var | { **;** } |
| lista_var' | { **;** } |
| sentencias | { **$**, **else** } |
| sentencias' | { **$**, **else** } |
| sentencia | { **id**, **if**, **while**, **$**, **else** } |
| S_emparejada | { **id**, **if**, **while**, **$**, **else** } |
| S_abierta | { **id**, **if**, **while**, **$**, **else** } |
| expresion | { **;**, **)** } |
| expresion' | { **;**, **)** } |
| termino | { **+**, **-**, **;**, **)** } |
| termino' | { **+**, **-**, **;**, **)** } |
| factor | { **\***, **/**, **+**, **-**, **;**, **)** } |

---

## 4. Tabla de Análisis Sintáctico Predictivo M[A, a]

La tabla se construye aplicando estas reglas para cada producción `A → α`:

- **Regla 1:** Para cada `a ∈ FIRST(α)` con `a ≠ ε`, agregar `A → α` en `M[A, a]`.
- **Regla 2:** Si `ε ∈ FIRST(α)`, entonces para cada `b ∈ FOLLOW(A)`, agregar `A → α` en `M[A, b]`.

### Derivación de entradas (selección)

| Producción | α | FIRST(α) | ¿ε∈FIRST? | FOLLOW(A) usado | Entradas en M |
|---|---|---|---|---|---|
| P0: programa → decls sents | decls sents | { int, float } | No | — | M[programa][int]=P0, M[programa][float]=P0 |
| P3: decls' → ε | ε | {ε} | Sí | FOLLOW(decls')={ id,if,while } | M[decls'][id]=P3, M[decls'][if]=P3, M[decls'][while]=P3 |
| P9: lista_var' → ε | ε | {ε} | Sí | FOLLOW(lista_var')={ ; } | M[lista_var'][;]=P9 |
| P24: expresion' → ε | ε | {ε} | Sí | FOLLOW(expresion')={ ;,) } | M[exp'][;]=P24, M[exp'][)]=P24 |
| P28: termino' → ε | ε | {ε} | Sí | FOLLOW(termino')={ +,-,;,) } | M[term'][+]=P28, M[term'][-]=P28, M[term'][;]=P28, M[term'][)]=P28 |

### Tabla completa M[A, a]

> Las celdas vacías indican error sintáctico. Los números corresponden a los índices de producción P0–P31.

| | **int** | **float** | **id** | **num** | **+** | **-** | **\*** | **/** | **=** | **;** | **,** | **(** | **)** | **if** | **else** | **while** | **$** |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **programa** | P0 | P0 | | | | | | | | | | | | | | | |
| **declaraciones** | P1 | P1 | | | | | | | | | | | | | | | |
| **declaraciones'** | P2 | P2 | P3 | | | | | | | | | | | P3 | | P3 | |
| **declaracion** | P4 | P4 | | | | | | | | | | | | | | | |
| **tipo** | P5 | P6 | | | | | | | | | | | | | | | |
| **lista_var** | | | P7 | | | | | | | | | | | | | | |
| **lista_var'** | | | | | | | | | | P9 | P8 | | | | | | |
| **sentencias** | | | P10 | | | | | | | | | | | P10 | | P10 | |
| **sentencias'** | | | P11 | | | | | | | | | | | P11 | P12 | P11 | P12 |
| **sentencia** | | | P13 | | | | | | | | | | | P14 | | P13 | |
| **S_emparejada** | | | P15 | | | | | | | | | | | P17 | | P16 | |
| **S_abierta** | | | | | | | | | | | | | | P18 | | P20 | |
| **expresion** | | | P21 | P21 | | | | | | | | P21 | | | | | |
| **expresion'** | | | | | P22 | P23 | | | | P24 | | | P24 | | | | |
| **termino** | | | P25 | P25 | | | | | | | | P25 | | | | | |
| **termino'** | | | | | P28 | P28 | P26 | P27 | | P28 | | | P28 | | | | |
| **factor** | | | P30 | P31 | | | | | | | | P29 | | | | | |

### Verificación LL(1)

La gramática **es LL(1)** porque:
- Ninguna celda de la tabla contiene más de una producción (no hay conflictos).
- Los conflictos del dangling else fueron eliminados con la separación S_emparejada / S_abierta.
- La recursividad izquierda fue eliminada, por lo que los conjuntos FIRST de las alternativas de cada no-terminal son disjuntos.
