#ifndef EstructuraGrafo24
#define EstructuraGrafo24

typedef unsigned int u32;

typedef u32 color;

typedef struct GrafoSt {
    u32 numeroDeVertices;
    u32 numeroDeLados;
    u32 Delta;            // Grado máximo
    color* colores;       // colores[i] = color del vértice i
    u32* grados;          // grados[i] = grado del vértice i
    u32** lados;          // Matriz de adyacencia
} GrafoSt;

#endif