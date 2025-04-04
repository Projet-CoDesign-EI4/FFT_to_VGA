#include "config.h"

typedef struct {
    int x;
    int y;
} FFT_Point;

typedef struct { // 24bits par pixel
    unsigned char r; // 8bits, 1 octet par couleur d'ou le unsigned char
    unsigned char g;
    unsigned char b;
} RGB_Point;


// ce qu'il y a avant c'est des nombres complexes partie réel partie imaginaire.

// // reçoit les données avant 

// FFT_Point exemple[NUM_FFT_POINTS] = {
//     { 0, 12 }, { 3, 45 }, { 6, 23 }, { 9, 34 }, { 12, 56 },
//     { 15, 7 }, { 18, 29 }, { 21, 50 }, { 24, 15 }, { 27, 42 },
//     { 30, 9 }, { 33, 58 }, { 36, 27 }, { 39, 38 }, { 42, 19 },
//     { 45, 20 }, { 47, 22 }, { 61, 11 }, { 67, 40 }
// }; 


// |!| on a le format (y,x) avec x_max=79 et y_max=59 pour apparaître
// sur l'affichage le y est axe ordonné
