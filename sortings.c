#include "sortings.h"

void Reverse(u32* arr, u32 n){
    u32* aux = calloc(n, sizeof(u32));
    for (u32 i = 0; i < n; i++){
        aux[i] = arr[i];
    }

    for (u32 i = 0; i < n; i++){
        arr[i] = aux[n - i - 1];
    }
    free(aux);
}//O(n)

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