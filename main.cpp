#include <iostream>
#include <fstream>

using namespace std;

// Función para convertir un carácter ASCII en su representación binaria de 8 bits
void convertirABinario(int numero, char* binario) {
    for (int i = 7; i >= 0; --i) {
        binario[7 - i] = (numero & (1 << i)) ? '1' : '0';
    }
    binario[8] = '\0';  // Asegurar fin de cadena
}

// Función para invertir los bits de un bloque
void invertirBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio; ++i) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

// Función para contar la cantidad de '0's y '1's en un bloque
void contarCerosYUnos(const char* bloque, int tamanio, int& ceros, int& unos) {
    ceros = 0;
    unos = 0;
    for (int i = 0; i < tamanio; ++i) {
        if (bloque[i] == '0') ceros++;
        else unos++;
    }
}

// Función que invierte cada 2 bits del bloque (sin alterar el primer bit)
void invertirCadaDosBits(char* bloque, int tamanio) {
    for (int i = 1; i < tamanio; i += 2) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1'; // Invertir solo el segundo, cuarto, etc.
    }
}

// Función que invierte cada 3 bits del bloque
void invertirCadaTresBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio - 2; i += 3) {
        swap(bloque[i], bloque[i + 2]);
    }
}

// Función para dividir los bits en bloques de tamaño n y procesarlos
void dividirEnBloques(const string& bitsTotales, int n) {
    int longitud = bitsTotales.size();
    cout << "Bits divididos en bloques de " << n << ":\n";

    int cerosPrevios = 0, unosPrevios = 0;  // Contadores de ceros y unos del bloque anterior

    // Iterar sobre los bits y agruparlos en bloques de tamaño n
    for (int i = 0; i < longitud; i += n) {
        char bloque[n + 1];  // Espacio para el bloque y terminador de cadena
        int tamanioBloque = 0;  // Tamaño real del bloque (en caso de ser parcial)

        // Copiar los bits al bloque temporal
        for (int j = i; j < i + n && j < longitud; ++j) {
            bloque[tamanioBloque++] = bitsTotales[j];
        }
        bloque[tamanioBloque] = '\0';  // Terminador de cadena

        // Procesar el bloque
        int ceros, unos;
        contarCerosYUnos(bloque, tamanioBloque, ceros, unos);

        // Aplicar condiciones de inversión
        if (i == 0) {
            // Si es el primer bloque, invertimos todos los bits
            invertirBits(bloque, tamanioBloque);
        } else if (cerosPrevios == unosPrevios) {
            // Si hay igual cantidad de 1s y 0s en el bloque anterior, se invierten todos los bits
            invertirBits(bloque, tamanioBloque);
        } else if (cerosPrevios > unosPrevios) {
            // Si hay más ceros que unos en el bloque anterior, se invierte cada 2 bits
            invertirCadaDosBits(bloque, tamanioBloque);
        } else {
            // Si hay más unos que ceros en el bloque anterior, se invierte cada 3 bits
            invertirCadaTresBits(bloque, tamanioBloque);
        }

        // Mostrar el bloque procesado
        cout << "Bloque " << (i / n) + 1 << ": " << bloque << endl;

        // Actualizar contadores para el próximo bloque
        cerosPrevios = ceros;
        unosPrevios = unos;
    }
}

// Función para leer el archivo y convertirlo en bloques
void leerArchivoYConvertirEnBloques(const char* nombreArchivo, int n) {
    ifstream archivo(nombreArchivo, ios::in);  // Abrimos el archivo en modo lectura

    if (!archivo) {  // Verificamos si el archivo se abrió correctamente
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    char caracter;
    char binario[9];  // Espacio para 8 bits + terminador de cadena
    string bitsTotales = "";  // Cadena que almacena todos los bits

    // Leer el archivo y convertir cada carácter en binario
    while (archivo.get(caracter)) {
        convertirABinario((int)caracter, binario);  // Convertimos a binario
        bitsTotales += binario;  // Concatenamos los bits en una cadena
    }

    archivo.close();  // Cerramos el archivo

    // Dividir los bits en bloques de tamaño n y mostrarlos
    dividirEnBloques(bitsTotales, n);
}

int main() {
    const char* nombreArchivo = "input.txt";  // Nombre del archivo a leer
    int n;

    cout << "Ingrese el tamaño de los bloques (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "El tamaño de los bloques debe ser mayor que 0." << endl;
        return 1;
    }

    leerArchivoYConvertirEnBloques(nombreArchivo, n);
    return 0;
}
