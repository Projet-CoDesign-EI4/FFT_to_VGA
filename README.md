# 📊 FFT to VGA

Affichage en VGA du résultat d'un calcul de Transformée de Fourier (FFT).  
Le programme reçoit des points FFT (amplitude/fréquence) en entrée, les transforme visuellement, et génère un buffer à envoyer vers l’écran VGA via DMA.

---

## 📁 Structure du projet

| Fichier                          | Rôle                                                                 |
|----------------------------------|----------------------------------------------------------------------|
| `image_formatter.c`              | Programme principal : envoi réellement au DMA                        |
| `image_formatter_test_dma.c`     | Programme de test en terminal (simulation du DMA)                    |
| `fft_data.h`                     | Structures des points FFT et RGB                                     |
| `config.h`                       | Paramètres VGA (dimensions, couleurs, adresses DMA, etc.)            |

---

## ⚙️ Compilation & Exécution

### ✅ Test terminal

```bash
gcc -o test_dma image_formatter_test_dma.c
./test_dma
