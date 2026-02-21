#include <stdio.h>

#define VERSION_SOPORTADA 2
#if VERSION_SOPORTADA < 1
    #error "Error crítico: La versión del sistema es demasiado antigua para este código."
#endif

#define MOSTRAR_VARIABLE(v) printf("El valor de " #v " es: %d\n", v)

#define TEMPORAL 100
#undef TEMPORAL

#pragma pack(push, 1)
struct Sensor {
    char id;
    int valor;
};
#pragma pack(pop)

int main() {
    int lectura = 42;

    MOSTRAR_VARIABLE(lectura);

    printf("Sistema ejecutándose en la versión: %d\n", VERSION_SOPORTADA);

    return 0;
}