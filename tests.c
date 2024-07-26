
#include "APIG24.h"
#include "API2024Parte2.h"
#include "sortings.h"

#include <limits.h>
#define ERROR UINT_MAX


u32 checkColoreoPropio(Grafo G){
    u32 n = NumeroDeVertices(G);

    for(u32 i = 0; i < n; i++){
        u32 j = 0;
        u32 vecino = Vecino(j, i, G);
        while(vecino != ERROR){
            if (Color(i, G) == Color(vecino, G)){
                return 1;
            }
            j++;
            vecino = Vecino(j, i, G);
        }
    }

    return 0;
}


//Orden no induce una biyeccion en {0, 1, ..., n-1}
u32 testGreedy_1(Grafo G){
    u32 n = NumeroDeVertices(G);
    u32 Orden[n];
    for (u32 i = 0; i < n; i++){
        Orden[i] = i * 2;
    }

    return Greedy(G, Orden); //deberia retornar ERROR
}

//Greedy con distintos ordenes permitidos de vertices
u32 testGreedy_2(Grafo G){
    u32 n = NumeroDeVertices(G);
    u32 Orden[n];

    //{0, 1, 2, ..., n-1}
    for (u32 i = 0; i < n; i++){
        Orden[i] = i;
    }

    u32 ret = Greedy(G, Orden);
    if (ret == ERROR){
        printf("TG2: Primer Greedy fallo\n");
        return 1;
    }
    if (checkColoreoPropio(G) == 1){
        printf("TG2: El primer coloreo no es propio\n");
        return 1;
    }

    //{n-1, n-2, ..., 2, 1, 0}
    for (u32 i = 0; i < n; i++){
        Orden[i] = n - i - 1;
    }

    ret = Greedy(G, Orden);
    if (ret == ERROR){
        printf("TG2: Segundo Greedy fallo\n");
        return 1;
    }
    if (checkColoreoPropio(G) == 1){
        printf("TG2: El segundo coloreo no es propio\n");
        return 1;
    }

    u32 offset = 0;
    if (n % 2 != 0) {
        offset = 1;
    }

    //{Primero los pares en orden decreciente, luego los impares en orden creciente}
    for (u32 i = 0; i < (n/2 + offset); i++){
        Orden[i] = ((n/2 - i) * 2) - 2*(1 - offset);
    }
    for (u32 i = 0; i < n/2; i++){
        Orden[i + n/2 + offset] = i * 2 + 1;
    }

    ret = Greedy(G, Orden);
    if (ret == ERROR){
        printf("TG2: Tercer Greedy fallo\n");
        return 1;
    }
    if (checkColoreoPropio(G) == 1){
        printf("TG2: El tercer coloreo no es propio\n");
        return 1;
    }

    u32** verticeGrado = malloc(n * sizeof(u32*));
    for (u32 i = 0; i < n; i++){
        verticeGrado[i] = calloc(2, sizeof(u32));
        verticeGrado[i][0] = i;
        verticeGrado[i][1] = Grado(i, G);
    }

    RadixSort2(verticeGrado, n);
    Reverse2(verticeGrado, n);

    //{Vertices ordenados por grado en orden decreciente}
    for (u32 i = 0; i < n; i++){
        Orden[i] = verticeGrado[i][0];
        free(verticeGrado[i]);
    }
    free(verticeGrado);

    ret = Greedy(G, Orden);
    if (ret == ERROR){
        printf("TG2: Cuarto Greedy fallo\n");
        return 1;
    }
    if (checkColoreoPropio(G) == 1){
        printf("TG2: El cuarto coloreo no es propio\n");
        return 1;
    }

    return 0;
}

int main(void){
    Grafo G = ConstruirGrafo();

    u32 ret = testGreedy_1(G);
    if (ret != ERROR){
        printf("testGreedy_1 fallo\n");
        return 1;
    }

    ret = testGreedy_2(G);
    if (ret != 0){
        printf("testGreedy_2 fallo\n");
        return 1;
    }

    printf("El programa paso todos los tests\n");
    DestruirGrafo(G);

    return 0;
}
