// Parametres de l'affichage VGA si on veut le tracer

#define VGA_WIDTH  80   // Largeur VGA (ex: 800)
#define VGA_HEIGHT 60   // Hauteur VGA (ex: 600)

#define MARGE 2  // Épaisseur de la bordure qui est modifiable ici


#define NUM_FFT_POINTS 21  // Nombre de points FFT simulés


// couleur de l'axe 
#define AXIS_COLOR_R 244
#define AXIS_COLOR_G 237
#define AXIS_COLOR_B 222
// blanc cassé

// couleur des points FFT
#define FFT_COLOR_R 135
#define FFT_COLOR_G 206
#define FFT_COLOR_B 235
// sky blue

#define freq_echantillonage 1000 // à voir


// ============ Config des adresses DMA ===============

#define DMA_BASE_ADDR     0x40400000  // <==========================changer en fonction du mapping
#define DMA_MM2S_DMACR    (*(volatile uint32_t *)(DMA_BASE_ADDR + 0x00))
#define DMA_MM2S_DMASR    (*(volatile uint32_t *)(DMA_BASE_ADDR + 0x04))
#define DMA_MM2S_SA       (*(volatile uint32_t *)(DMA_BASE_ADDR + 0x18))
#define DMA_MM2S_LENGTH   (*(volatile uint32_t *)(DMA_BASE_ADDR + 0x28))