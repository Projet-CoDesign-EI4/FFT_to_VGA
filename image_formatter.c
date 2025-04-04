#include <stdio.h>
#include <stdlib.h>
#include "fft_data.h"
#include "config.h"

#include <stdint.h> 
#include <stddef.h> 


// ===================== FONCTIONS =====================

// Initialisation du fond noir avec axes blancs

void initialize_vga_matrix(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH]) {

    // Tout en noire de base
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_matrix[y][x] = (RGB_Point){0, 0, 0};
        }
    }

    // Axe X (fréquence) en bas
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_matrix[VGA_HEIGHT - 1][x] = (RGB_Point){AXIS_COLOR_R, AXIS_COLOR_G, AXIS_COLOR_B};
    }

    // Axe Y (amplitude) sur la gauche
    for (int y = 0; y < VGA_HEIGHT; y++) {
        vga_matrix[y][0] = (RGB_Point){AXIS_COLOR_R, AXIS_COLOR_G, AXIS_COLOR_B};
    }
}

// Ajoute les points FFT avec des colonnes les reliant à l'axe X

void update_vga_display(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH], FFT_Point fft_data[NUM_FFT_POINTS]) {

    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        int x = fft_data[i].x;
        int y = fft_data[i].y;

        // Vérifier que les points sont dans les limites de l'affichage

        if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
            continue;

        // Dessiner une ligne verticale reliant le point FFT à l'axe X --- but visuel

        for (int j = 0; j <= y; j++) {
            int row = VGA_HEIGHT - 1 - j; // Conversion vers la matrice
            vga_matrix[row][x] = (RGB_Point){FFT_COLOR_R, FFT_COLOR_G, FFT_COLOR_B};
        }
    }
}

// Transformer notre matrice 2D en un tableau 1D pour la VGA

void convert_matrix_to_buffer(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH], RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH]) {
    int index = 0;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            framebuffer[index++] = vga_matrix[y][x];
        }
    }
}

// ------------------- Affichage terminal -------------------

// matrice dans terminal

void print_vga_matrix(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH]) {
    printf("Affichage de la matrice sous forme moche\n");
    for (int y = 0; y < VGA_HEIGHT; y++) {  // Parcours des Y du plus bas au plus haut
        for (int x = 0; x < VGA_WIDTH; x++) {  // Parcours des X de gauche à droite
            RGB_Point p = vga_matrix[y][x];
            printf("(R%3d, G%3d, B%3d) ", p.r, p.g, p.b);  // Affiche les valeurs RGB
        }
        printf("\n");
    }
}

// revoir cette fonction pour s'assurer que ça marche 
/*
Reformater les données d'entrée en fonction de la frequence d'échantillonage pour s'adapter à l'affichage où l'axe x est de 800 pixels
*/

void adjust_fft_points(FFT_Point fft_data[NUM_FFT_POINTS], int adjusted_points[NUM_FFT_POINTS][2], int freq_echantillonnage) {
    // Empêcher la division par zéro
    float pixel_per_frequency = (freq_echantillonnage < 1600) ? 1.0 : (float)freq_echantillonnage / 1600.0;

    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        // Calcul de la position en X sur l'axe des fréquences (axe X de 0 à 799)
        int adjusted_x = (int)(fft_data[i].x / pixel_per_frequency);
        
        // Limite X à l'intervalle de 0 à 799 (puisque nous avons 800 pixels)
        if (adjusted_x < 0) adjusted_x = 0;
        if (adjusted_x >= VGA_WIDTH) adjusted_x = VGA_WIDTH - 1;

        // La position Y peut rester la même, sauf si elle dépasse la hauteur de l'affichage
        int adjusted_y = fft_data[i].y;
        if (adjusted_y < 0) adjusted_y = 0;
        if (adjusted_y >= VGA_HEIGHT) adjusted_y = VGA_HEIGHT - 1;

        // Stockage du point ajusté dans le tableau
        adjusted_points[i][0] = adjusted_x;
        adjusted_points[i][1] = adjusted_y;
    }
}

// lire la matrice en diagramme

void display_vga_matrix_diagramme(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH]) {
    printf("Affichage de la matrice sous forme de diagramme\n");
    for (int y = 0; y < VGA_HEIGHT; y++) {  // Parcours des Y du plus bas au plus haut
        for (int x = 0; x < VGA_WIDTH; x++) {  // Parcours des X de gauche à droite
            RGB_Point p = vga_matrix[y][x];
            if (p.r == FFT_COLOR_R && p.g == FFT_COLOR_G && p.b == FFT_COLOR_B)
                printf("█"); // FFT
            else if (p.r == AXIS_COLOR_R && p.g == AXIS_COLOR_G && p.b == AXIS_COLOR_B)
                printf("+"); // Axes
            else
                printf(" "); // Fond noir
        }
        printf("\n");
    }
}

// buffer dans terminal

void print_framebuffer(RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH]) {
    printf("Affichage du buffer sous forme moche\n");
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        printf("(%d, %d, %d) ", framebuffer[i].r, framebuffer[i].g, framebuffer[i].b);
    }
    printf("\n");
}

// lire le buffer en diagramme

void print_framebuffer_diagramme(RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH]) {
    printf("Affichage du buffer sous forme de diagramme\n");
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            RGB_Point p = framebuffer[y * VGA_WIDTH + x];

            if (p.r == FFT_COLOR_R && p.g == FFT_COLOR_G && p.b == FFT_COLOR_B)
                printf("█");  // Point FFT en rouge
            else if (p.r == AXIS_COLOR_R && p.g == AXIS_COLOR_G && p.b == AXIS_COLOR_B)
                printf("+");  // Axes en blanc
            else
                printf(" ");  // Fond noir 
        }
        printf("\n");
    }
}

// ------------------------ DMA ------------------------

// usage du DMA, definition adresses dans le config.h
void start_dma_transfer(void* framebuffer, int size_bytes) {
    DMA_MM2S_DMACR = 0x4;                      // reset DMA
    DMA_MM2S_DMACR = 0x1;                      // enable MM2S
    DMA_MM2S_SA = (uintptr_t)framebuffer;       // source addr (cast avec intptr_t pour éviter warning, architecture 64bits)
    DMA_MM2S_LENGTH = size_bytes;              // taille

    // Vérif démarrage DMA
    while ((DMA_MM2S_DMASR & 0x1) == 0) {  // bit de démarrage 
        // Attente active (polling) jusqu'à ce que le DMA commence
    }

    // Vérif fin transfert
    while ((DMA_MM2S_DMASR & 0x2) == 0) {  // bit de fin de transfert
        // Attente active (polling) jusqu'à ce que le DMA termine
    }

    // Vérif erreur 
    if (DMA_MM2S_DMASR & 0x4) {  // (bit d'erreur dans le registre DMASR)
        printf("Erreur DMA : échec du transfert.\n");
    }
}


// =============================== MAIN ===============================

int main() {

    // ============ ENTREE, liste de coordonnées x,y :
    FFT_Point fft_data[NUM_FFT_POINTS] = { 
        { 0, 12 }, { 1, 8 }, { 8, 23 }, { 9, 34 }, { 11, 28 },
        { 13, 7 }, { 20, 41 }, { 21, 50 }, { 22, 47 }, { 27, 9 },
        { 29, 14 }, { 30, 17 }, { 33, 27 }, { 39, 21 }, { 42, 19 },
        { 45, 20 }, { 46, 22 }, { 54, 11 }, { 55, 20 }, { 61, 11 }, 
        { 67, 7 }
    }; 
    
    // ------------ PRE-SORTIE, matrice vga :
    RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH];

    // ============ SORTIE, buffer 1D pour afficher
    RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH];


    initialize_vga_matrix(vga_matrix);

    int adjusted_points[NUM_FFT_POINTS][2];
    adjust_fft_points(fft_data, adjusted_points, freq_echantillonage);

    update_vga_display(vga_matrix,fft_data);

    convert_matrix_to_buffer( vga_matrix, framebuffer);


    print_framebuffer_diagramme(framebuffer); //print à partir du buffer
    //print_framebuffer(framebuffer); //print à partir de la matrice

    printf("Test DMA\n");

    // ============== DMA 
    // Appel de la fonction pour démarrer le transfert DMA
    int framebuffer_size = VGA_HEIGHT * VGA_WIDTH * sizeof(RGB_Point);
    start_dma_transfer(framebuffer, framebuffer_size);

    return 0;
}

