#include <iostream>
#include <cstdio>

int main() {
    FILE* archivo;
    const char* nombre = "C:\\Users\\cesar\\Desktop\\prueba3.bmp";
    archivo = fopen(nombre, "rb+");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    unsigned char cabecera[14];
    fread(cabecera, sizeof(unsigned char), 14, archivo);

    printf("\nFormato: %c%c\n", cabecera[0], cabecera[1]);
    printf("Tamano del archivo: %d bytes\n", *(int*)&cabecera[2]);

    unsigned char infoHeader[40];
    fread(infoHeader, sizeof(unsigned char), 40, archivo);

    int ancho = *(int*)&infoHeader[4];
    int alto = *(int*)&infoHeader[8];
    short bpp = *(short*)&infoHeader[14];

    if (bpp != 24) {
        printf("El programa solo funciona con imágenes BMP de 24 bits.\n");
        fclose(archivo);
        return 1;
    }

    int offset = *(int*)&cabecera[10];
    fseek(archivo, offset, SEEK_SET);

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            unsigned char pixel[3];
            fread(pixel, sizeof(unsigned char), 3, archivo);

            if (pixel[2] > pixel[1] && pixel[2] > pixel[0]) {
                pixel[2] = 0;
                pixel[0] = 255;
            }

            fseek(archivo, -3, SEEK_CUR);
            fwrite(pixel, sizeof(unsigned char), 3, archivo);
        }
    }

    fclose(archivo);

    printf("Se ha completado el cambio de color de rojo a azul.\n");
    return 0;
}
