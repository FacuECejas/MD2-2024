#include "API2024Parte2.h"
#include "EstructuraGrafo24.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ERROR UINT_MAX

int comparador_ascendente_u32(const void *a, const void *b) {
    const u32 *elemento_a = (const u32 *)a;
    const u32 *elemento_b = (const u32 *)b;
    
    if (*elemento_a < *elemento_b) {
        return -1; // Colocar 'a' antes que 'b'
    } else if (*elemento_a > *elemento_b) {
        return 1; // Colocar 'a' después que 'b'
    } else {
        return 0; // 'a' y 'b' son iguales
    }
}

int assertOrden(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);
    u32 ordenAux[n];
    for (u32 i = 0; i < n; i++){
        ordenAux[i] = Orden[i];
    }
    qsort(ordenAux, n, sizeof(u32), comparador_ascendente_u32);
    for (u32 i = 0; i < n; i++){
        if (ordenAux[i] != i){
            return -1;
        }
    }
    return 0;

}

u32 Greedy(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);

    int err = assertOrden(G, Orden); //O(nlogn), en el peor caso O(n^2)
    if (err == -1){
        return ERROR;
    }

    AsignarColor(1, Orden[0], G);
    
    for (u32 i = 1; i < n; i++){
        u32 v = Orden[i];
        u32 j = 0;
        u32 vecino = Vecino(j, v, G);
        u32 coloresUsados[n + 1];
        for(u32 i = 0; i < n + 1; i++){
            coloresUsados[i] = 0;
        }
        coloresUsados[0] = 1;
        while(vecino != ERROR){
            coloresUsados[Color(vecino, G)] = 1; 
            j++;
            vecino = Vecino(j, v, G);
        }
        u32 menorColorDisp = 0;
        for (u32 i = 0; i < n + 1; i++){
            if (coloresUsados[i] == 0){
                menorColorDisp = i;
                break;
            }
        }
        AsignarColor(menorColorDisp, v, G);
    } //O(n^2)

    color mayorColor = 0;
    for (u32 i = 0; i < n; i++){
        if (Color(i, G) > mayorColor){
            mayorColor = Color(i, G);
        }
    } //O(n)

    return mayorColor;
} //O(n^2)
