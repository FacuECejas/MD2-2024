#include "APIG24.h"
#include <limits.h>
#include <string.h>
#include <assert.h>

Grafo inicializarGrafo(){
    Grafo grafo = malloc(sizeof(GrafoSt));
    grafo->numeroDeVertices = 0;
    grafo->numeroDeLados = 0;
    grafo->Delta = 0;
    grafo->colores = NULL;
    grafo->grados = NULL;
    grafo->lados = NULL;
    return grafo;
}

Grafo ConstruirGrafo(){

    Grafo grafo = inicializarGrafo();

    char linea[1024];   //No estamos tomando lineas de tamaño arbitrario
    u32 numLados = 0;
    while(fgets(linea, 1024, stdin) != NULL){

        if(linea[0] == 'c'){
            continue;
        }
        else if (linea[0] == 'p'){
            sscanf(linea, "p edge %u %u", &grafo->numeroDeVertices, &grafo->numeroDeLados);
            if(grafo->numeroDeVertices <= 1 || grafo->numeroDeLados == 0){
                printf("El archivo no tiene un formato válido\n");
                DestruirGrafo(grafo);
                return grafo;
            }

            grafo->colores = calloc(grafo->numeroDeVertices, sizeof(color));
            grafo->grados = calloc(grafo->numeroDeVertices, sizeof(u32));
            grafo->lados = malloc(grafo->numeroDeVertices * sizeof(u32*));

            for(u32 i = 0; i < grafo->numeroDeVertices; i++){
                grafo->lados[i] = malloc(sizeof(u32));
            }

            numLados = grafo->numeroDeLados;
        }
        else if (linea[0] == 'e'){
            numLados--;
            u32 v1 = 0;
            u32 v2 = 0;
            sscanf(linea, "e %u %u", &v1, &v2);
            if(v1 >= grafo->numeroDeVertices || v2 >= grafo->numeroDeVertices){
                printf("El archivo no tiene un formato válido\n");
                DestruirGrafo(grafo);
                return grafo;
            }

            grafo->grados[v1]++;
            grafo->grados[v2]++;

            grafo->lados[v1] = realloc(grafo->lados[v1], grafo->grados[v1] * sizeof(u32));
            grafo->lados[v2] = realloc(grafo->lados[v2], grafo->grados[v2] * sizeof(u32));

            grafo->lados[v1][grafo->grados[v1] - 1] = v2;
            grafo->lados[v2][grafo->grados[v2] - 1] = v1;

            if(numLados == 0) {
                for(u32 i = 0; i < grafo->numeroDeVertices; i++){
                    if(grafo->grados[i] > grafo->Delta){
                        grafo->Delta = grafo->grados[i];
                    }
                } 
                break;
            }
        }
        else {
            printf("El archivo no tiene un formato válido\n");
            DestruirGrafo(grafo);
            return grafo;
        }
    }
    return grafo;
}

void DestruirGrafo(Grafo G){
    for(u32 i = 0; i < G->numeroDeVertices; i++){
        free(G->lados[i]);
    }
    free(G->lados);
    free(G->colores);
    free(G->grados);
    free(G);
}

u32 NumeroDeVertices(Grafo G){
    assert(G != NULL);
    return G->numeroDeVertices;
}

u32 NumeroDeLados(Grafo G){
    assert(G != NULL);
    return G->numeroDeLados;
}

u32 Delta(Grafo G){
    assert(G != NULL);
    return G->Delta;
}

u32 Grado(u32 i,Grafo G){
    assert(G != NULL);
    if(i >= G->numeroDeVertices) return 0;
    return G->grados[i];
}

color Color(u32 i,Grafo G){
    assert(G != NULL);
    if(i >= G->numeroDeVertices) return UINT_MAX;
    return G->colores[i];
}

u32 Vecino(u32 j,u32 i,Grafo G){
    assert(G != NULL);
    if(i >= G->numeroDeVertices || j >= G->grados[i]) return UINT_MAX;
    return G->lados[i][j];
}

void AsignarColor(color x,u32 i,Grafo  G){
    assert(G != NULL);
    if(i < G->numeroDeVertices) {
        G->colores[i] = x;
    }
}

void ExtraerColores(Grafo G,color* Color){
    assert(G != NULL);
    for(u32 i = 0; i < G->numeroDeVertices; i++){
        Color[i] = G->colores[i];
    }
}

void ImportarColores(color* Color,Grafo  G){
    assert(G != NULL);
    for(u32 i = 0; i < G->numeroDeVertices; i++){
        G->colores[i] = Color[i];
    }
}
