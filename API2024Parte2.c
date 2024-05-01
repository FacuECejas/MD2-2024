#include "API2024Parte2.h"
#include "EstructuraGrafo24.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ERROR UINT_MAX

int assertOrden(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);
    u32* ordenAux = calloc(n, sizeof(u32));
    for (u32 i = 0; i < n; i++){
        if (Orden[i] >= n){
            return -1;
        }
        ordenAux[Orden[i]]++;
    }
    for (u32 i = 0; i < n; i++){
        if (ordenAux[i] != 1){
            return -1;
        }
    }
    free(ordenAux);
    return 0;
} //O(n)


u32 Greedy(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);

    int err = assertOrden(G, Orden); //O(n)
    if (err == -1){
        return ERROR;
    }

    AsignarColor(1, Orden[0], G);
    
    color mayorColor = 1;
    for (u32 i = 1; i < n; i++){
        u32 v = Orden[i];
        u32 j = 0;
        u32 vecino = Vecino(j, v, G);

        u32 d = Grado(v, G);
        
        u32* coloresUsados = calloc(d, sizeof(u32));

        while(vecino != ERROR){
            if (Color(vecino, G) != 0){
                coloresUsados[Color(vecino, G) - 1] = 1;
            }
            j++;
            vecino = Vecino(j, v, G);
        } //O(d(v))

        u32 menorColorDisp = 0;
        while(coloresUsados[menorColorDisp] == 1){
            menorColorDisp++;
        } //O(d(v)))

        AsignarColor(menorColorDisp + 1, v, G);
        
        if (menorColorDisp + 1 > mayorColor){
            mayorColor = menorColorDisp + 1;
        }

        free(coloresUsados);
    } //O(n * d(v))

    return mayorColor;
} //O(n + n * d(v)) = O(n * d(v)) = O(2m) = O(m) -> del handshaking lemma

//Count sort para u32**:
void CountSort2(u32** arr, u32 n, u32 exp){ //exp = 1, 10, 100, 1000, ...
    u32* output[n];
    for (u32 i = 0; i < n; i++){
        output[i] = calloc(2, sizeof(u32));
    }
    u32 count[10] = {0};

    for (u32 i = 0; i < n; i++){
        count[(arr[i][1]/exp)%10]++; //arr[i][1]/exp)%10 es el digito de interes de arr[i][1]
    }

    for (u32 i = 1; i < 10; i++){ //suma acumulada
        count[i] += count[i-1];
    }

    for (u32 i = 0; i < n; i++){ //ordenamiento
        output[count[(arr[i][1]/exp)%10] - 1][0] = arr[i][0];
        output[count[(arr[i][1]/exp)%10] - 1][1] = arr[i][1];
        count[(arr[i][1]/exp)%10]--;
    }

    for (u32 i = 0; i < n; i++){
        arr[i][0] = output[i][0];
        arr[i][1] = output[i][1];
        free(output[i]);
    }
} //O(n)

//Radix sort para u32**:
void RadixSort2(u32** arr, u32 n){
    u32 max = arr[0][1];
    for (u32 i = 1; i < n; i++){
        if (arr[i][1] > max){
            max = arr[i][1];
        }
    }

    for (u32 exp = 1; max/exp > 0; exp *= 10){
        CountSort2(arr, n, exp);
    }
}//O(b*n) -> b = cantidad de digitos de max (nunca va a ser n teniendo vertices del 0 al n-1)
 // => O(n)

 //Reverse para u32**:
void Reverse2(u32** arr, u32 n){
    u32* aux[n];
    for (u32 i = 0; i < n; i++){
        aux[i] = calloc(2, sizeof(u32));
    }

    for (u32 i = 0; i < n; i++){
        aux[i][0] = arr[i][0];
        aux[i][1] = arr[i][1];
    }

    for (u32 i = 0; i < n; i++){
        arr[i][0] = aux[n - i - 1][0];
        arr[i][1] = aux[n - i - 1][1];
        free(aux[n - i - 1]);
    }
}//O(n)

char GulDukat(Grafo G,u32* Orden){
    u32 n = NumeroDeVertices(G);

    //Puntero a M(x) y m(x) de cada color, donde indice = color - 1
    u32** M_m = malloc(n * sizeof(u32*));
    for (u32 i = 0; i < n; i++){
        M_m[i] = malloc(2 * sizeof(u32));
        M_m[i][0] = 0;
        M_m[i][1] = n + 1;
    }

    u32 j = 0;
    u32 k = 0;
    u32 l = 0;
    for (u32 i = 0; i < n; i++){
        if (Color(i, G) % 4 == 0){
            j++;
            if (Grado(i, G) > M_m[Color(i, G) - 1][0]){
                M_m[Color(i, G) - 1][0] = Grado(i, G);
            }
        } else if (Color(i, G) % 2 == 0){
            k++;
            if (Grado(i, G) > M_m[Color(i, G) - 1][0]){
                M_m[Color(i, G) - 1][0] = Grado(i, G);
            }
            if (Grado(i, G) < M_m[Color(i, G) - 1][1]){
                M_m[Color(i, G) - 1][1] = Grado(i, G);
            }
        } else {
            l++;
            if (Grado(i, G) < M_m[Color(i, G) - 1][1]){
                M_m[Color(i, G) - 1][1] = Grado(i, G);
            }
        }
    }

    if (j + k + l != n){
        for(u32 i = 0; i < n; i++){
            free(M_m[i]);
        }
        free(M_m);
        return '1';
    }

    u32** v4 = malloc(j * sizeof(u32*));
    u32** v2 = malloc(k * sizeof(u32*));
    u32** vI = malloc(l * sizeof(u32*));
    for (u32 i = 0; i < n; i++){
        if (i < j){
            v4[i] = calloc(2, sizeof(u32)); //vertices y M(x)
        }
        if (i < k){
            v2[i] = calloc(2, sizeof(u32)); //vertices y m(x) + M(x)
        }
        if (i < l){
            vI[i] = calloc(2, sizeof(u32)); //vertices y m(x)
        }
        if (i >= j && i >= k && i >= l) break;
    }

    u32 a = 0;
    u32 b = 0;
    u32 c = 0;
    for (u32 i = 0; i < n; i++){
        if (Color(i, G) % 4 == 0){
            v4[a][0] = i;
            v4[a][1] = M_m[Color(i, G) - 1][0];
            a++;
        } else if (Color(i, G) % 2 == 0){
            v2[b][0] = i;
            v2[b][1] = M_m[Color(i, G) - 1][0] + M_m[Color(i, G) - 1][1];
            b++;
        } else {
            vI[c][0] = i;
            vI[c][1] = M_m[Color(i, G) - 1][1];
            c++;
        }
    }

    for(u32 i = 0; i < n; i++){
        free(M_m[i]);
    }
    free(M_m);

    RadixSort2(v4, j);
    Reverse2(v4, j);
    RadixSort2(v2, k);
    Reverse2(v2, k);
    RadixSort2(vI, l);
    Reverse2(vI, l);

    for (u32 i = 0; i < n; i++){
        if (i < j){
            Orden[i] = v4[i][0];
        } else if (i < j + k){
            Orden[i] = v2[i - j][0];
        } else {
            Orden[i] = vI[i - j - k][0];
        }
    }
    
    for (u32 i = 0; i < n; i++){
        if (i < j) free(v4[i]);
        if (i < k) free(v2[i]);
        if (i < l) free(vI[i]);
        if (i >= j && i >= k && i >= l) break;
    }
    free(v4);
    free(v2);
    free(vI);

    return '0';
}


//Problema: Para el grafo más grande, el comprobar orden nos da un segmentation fault, especificamente en la parte de declerar un array de tamaño n.