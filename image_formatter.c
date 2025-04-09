#include <stdio.h>
#include <stdlib.h>
#include "fft_data.h"
#include "config.h"

#include <stdint.h> 
#include <stddef.h> 
#include <unistd.h>  // sleep()



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
    for (int x = MARGE; x < VGA_WIDTH - MARGE; x++) {
        vga_matrix[VGA_HEIGHT - 1 - MARGE][x] = (RGB_Point){AXIS_COLOR_R, AXIS_COLOR_G, AXIS_COLOR_B};
    }

    // Axe Y (amplitude) sur la gauche
    for (int y = MARGE; y < VGA_HEIGHT - MARGE; y++) {
        vga_matrix[y][MARGE] = (RGB_Point){AXIS_COLOR_R, AXIS_COLOR_G, AXIS_COLOR_B};
    }
}

// Ajoute les points FFT avec des colonnes les reliant à l'axe X

void update_vga_display(RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH], FFT_Point fft_data[NUM_FFT_POINTS]) {

    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        int x = fft_data[i].x + MARGE;
        int y = fft_data[i].y;

        // Vérifier que les points sont dans les limites de l'affichage

        if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
            continue;

        // Dessiner une ligne verticale reliant le point FFT à l'axe X --- but visuel

        for (int j = 0; j <= y; j++) {
            int row = VGA_HEIGHT - 1 - MARGE - j; // Conversion vers la matrice
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

// Renvoie un pointeur vers le tableau de FFT_Point stocké à FFT_DATA_ADDRESS
FFT_Point* get_fft_data(void) {
    return (FFT_Point*) FFT_DATA_ADDRESS;
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


/*
Reformater les données d'entrée en fonction de la frequence d'échantillonage pour s'adapter à l'affichage où l'axe x est de 800 pixels
*/

// Fonction qui ajuste les points FFT pour les adapter à l'écran VGA
void adjust_fft_points(FFT_Point fft_data[NUM_FFT_POINTS], int freq_echantillonnage) {
    // Trouver les valeurs maximales de X et Y dans fft_data pour effectuer l'ajustement proportionnel
    int max_x = 0, max_y = 0;
    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        if (fft_data[i].x > max_x) max_x = fft_data[i].x;
        if (fft_data[i].y > max_y) max_y = fft_data[i].y;
    }

    // Ajustement des points en X et Y
    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        // Ajuster les coordonnées en X et Y pour tenir dans la taille de l'écran
        int adjusted_x = (int)((float)fft_data[i].x / max_x * (VGA_WIDTH - 1));
        int adjusted_y = (int)((float)fft_data[i].y / max_y * (VGA_HEIGHT - 1));

        // Mettre à jour directement fft_data avec les nouvelles coordonnées ajustées
        fft_data[i].x = adjusted_x;
        fft_data[i].y = adjusted_y;
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
    printf("============ Affichage du buffer sous forme moche ============\n");
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        printf("(%d, %d, %d) ", framebuffer[i].r, framebuffer[i].g, framebuffer[i].b);
    }
    printf("\n");
    printf("==============================================================\n");
}

// lire le buffer en diagramme

void print_framebuffer_diagramme(RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH]) {
    printf("========= Affichage du buffer sous forme de diagramme =========\n");
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
    printf("\n");
    printf("=============================================================\n");
}

// ------------------------ DMA ------------------------

// usage du DMA, definition adresses dans le config.h
void start_dma_transfer(void* framebuffer, int size_bytes) {
    DMA_MM2S_DMACR = 0x4;                      // reset DMA
    DMA_MM2S_DMACR = 0x1;                      // enable MM2S
    DMA_MM2S_SA = (uintptr_t)framebuffer;       // source addr (cast avec intptr_t pour éviter warning, architecture 64bits)
    DMA_MM2S_LENGTH = size_bytes;              // taille

    // Vérif démarrage DMA￼// (bit d'erreur dans le registre DMASR)
        printf("Erreur DMA : échec du transfert.\n");
}


// convertir un pixel RGB en 32 bits (9 bits pour RGB, le reste à 0)
/*
Bits 31-12    | Bits 11-8 | Bits 7-4 | Bits 3-0
   (0)        |    R      |    G     |    B
*/
uint32_t encode_rgb_to_32bit(RGB_Point pixel) {
    uint8_t r = pixel.r & 0xF;  // 4 bits
    uint8_t g = pixel.g & 0xF;  // 4 bits
    uint8_t b = pixel.b & 0xF;  // 4 bits

    // Combinaison de R, G, B pour former un nombre de 12 bits tout à droite des 32 bits
    uint32_t encoded_pixel = (r << 8) | (g << 4) | b;

    // Les 20 autres bits sont mis à zéro
    return encoded_pixel; 
}

// envoyer les données pixel par pixel, batches 32 bits
void send_framebuffer_in_batches(RGB_Point * framebuffer) {

    uint32_t batch[1];  // batch 32 bits

    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        batch[0] = encode_rgb_to_32bit(framebuffer[i]);
        start_dma_transfer(batch, sizeof(batch));
        sleep(1);
    }
} 

// Fonction pour remplir un tableau de FFT_Point à partir de l'adresse mémoire
void load_fft_data_from_memory(uint32_t* base_address, FFT_Point fft_data[NUM_FFT_POINTS]) {

    uint32_t* memory_ptr = base_address; // pointeur vers l'adresse mémoire où les données FFT sont stockées

    // fft_data à partir de l'adresse mémoire
    for (int i = 0; i < NUM_FFT_POINTS; i++) {
        // 2 entiers de 2 octets (x et y) pour chaque FFT_Point
        // x et y sont stockées sous 2 octets
        fft_data[i].x = *((short*)(memory_ptr + i * 2));       // x (2 octets)
        fft_data[i].y = *((short*)(memory_ptr + i * 2 + 1));   // y (2 octets)
    }
}


// =============================== MAIN ===============================

int main() {


    // ============ ENTREE, liste de coordonnées x,y :
    FFT_Point *fft_data = get_fft_data();
    
    // ------------ PRE-SORTIE, matrice vga :
    RGB_Point vga_matrix[VGA_HEIGHT][VGA_WIDTH]; 

    // ============ SORTIE, buffer 1D pour afficher (décomposé à l'envoi)
    RGB_Point framebuffer[VGA_HEIGHT * VGA_WIDTH];



    // Initialisation du fond noir avec axes blancs
    initialize_vga_matrix(vga_matrix);


    // ------------------- Affichage terminal -------------------

    //print_framebuffer_diagramme(framebuffer); //print à partir du buffer
    //print_framebuffer(framebuffer); //print à partir de la matrice
    //display_vga_matrix_diagramme(vga_matrix); //print à partir de la matrice


    // Boucle infinie qui envoi au DMA
    while(1){

        // FFT_Point fft_data[NUM_FFT_POINTS];
        // load_fft_data_from_memory((uint32_t*)FFT_DATA_ADDRESS, fft_data); // cast necessaire

        FFT_Point fft_data[NUM_FFT_POINTS] = {
            { 0, 12 }, { 1, 8 }, { 8, 23 }, { 9, 34 }, { 11, 28 },
            { 13, 7 }, { 20, 41 }, { 21, 50 }, { 22, 47 }, { 27, 9 },
            { 29, 14 }, { 30, 17 }, { 33, 27 }, { 39, 21 }, { 42, 19 },
            { 45, 20 }, { 46, 22 }, { 54, 11 }, { 55, 20 }, { 61, 11 }, 
            { 67, 7 }
        };

        // Reformater les données d'entrée en fonction de la frequence d'échantillonage pour s'adapter à l'affichage où l'axe x est de 800 pixels
        adjust_fft_points(fft_data, freq_echantillonage);

        // Ajoute les points FFT avec des colonnes les reliant à l'axe X
        update_vga_display(vga_matrix,fft_data);

        // Transformer notre matrice 2D en un tableau 1D pour la VGA
        convert_matrix_to_buffer( vga_matrix, framebuffer);

        // ============== DMA 
        // Appel de la fonction pour démarrer le transfert DMA
        // convertir un pixel RGB en 32 bits (9 bits pour RGB, le reste à 0)
        /*
        Bits 31-12    | Bits 11-8 | Bits 7-4 | Bits 3-0
            (0)      |    R      |    G     |    B
        */
        int framebuffer_size = VGA_HEIGHT * VGA_WIDTH * sizeof(RGB_Point);
        send_framebuffer_in_batches(framebuffer); // envoi en batches

        sleep(1);

    }


    return 0;
}

