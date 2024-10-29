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

// Método 1: Invertir bits
void invertirBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio; ++i) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

void contarCerosYUnos(const char* bloque, int tamanio, int& ceros, int& unos) {
    ceros = 0;
    unos = 0;
    for (int i = 0; i < tamanio; ++i) {
        if (bloque[i] == '0') ceros++;
        else unos++;
    }
}

void invertirCadaDosBits(char* bloque, int tamanio) {
    for (int i = 1; i < tamanio; i += 2) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

void invertirCadaTresBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio - 2; i += 3) {
        swap(bloque[i], bloque[i + 2]);
    }
}

void dividirEnBloques(const string& bitsTotales, int n, ofstream& archivoSalida) {
    int longitud = bitsTotales.size();
    int cerosPrevios = 0, unosPrevios = 0;

    for (int i = 0; i < longitud; i += n) {
        char bloque[n + 1];
        int tamanioBloque = 0;

        for (int j = i; j < i + n && j < longitud; ++j) {
            bloque[tamanioBloque++] = bitsTotales[j];
        }
        bloque[tamanioBloque] = '\0';

        int ceros, unos;
        contarCerosYUnos(bloque, tamanioBloque, ceros, unos);

        if (i == 0) {
            invertirBits(bloque, tamanioBloque);
        } else if (cerosPrevios == unosPrevios) {
            invertirBits(bloque, tamanioBloque);
        } else if (cerosPrevios > unosPrevios) {
            invertirCadaDosBits(bloque, tamanioBloque);
        } else {
            invertirCadaTresBits(bloque, tamanioBloque);
        }

        archivoSalida << bloque;

        cerosPrevios = ceros;
        unosPrevios = unos;
    }
}

// Método 2: Desplazar bits
void desplazarBits(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[tamanio - 1];
    for (int i = tamanio - 1; i > 0; --i) {
        bloque[i] = bloque[i - 1];
    }
    bloque[0] = temp;
}

void dividirEnBloquesYDesplazar(const string& bitsTotales, int n, ofstream& archivoSalida) {
    int longitud = bitsTotales.size();

    for (int i = 0; i < longitud; i += n) {
        char bloque[n + 1];
        int tamanioBloque = 0;

        for (int j = i; j < i + n && j < longitud; ++j) {
            bloque[tamanioBloque++] = bitsTotales[j];
        }
        bloque[tamanioBloque] = '\0';

        desplazarBits(bloque, tamanioBloque);

        archivoSalida << bloque;
    }
}

// Función para leer el archivo y convertirlo en bloques
void leerArchivoYConvertirEnBloques(const char* nombreArchivo, int n, int metodo) {
    ifstream archivo(nombreArchivo, ios::in);

    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    char caracter;
    char binario[9];
    string bitsTotales = "";

    while (archivo.get(caracter)) {
        convertirABinario((int)caracter, binario);
        bitsTotales += binario;
    }

    archivo.close();

    ofstream archivoSalida("resultado.txt");
    if (!archivoSalida) {
        cout << "Error al crear el archivo de salida." << endl;
        return;
    }

    if (metodo == 1) {
        dividirEnBloques(bitsTotales, n, archivoSalida);
    } else if (metodo == 2) {
        dividirEnBloquesYDesplazar(bitsTotales, n, archivoSalida);
    } else {
        cout << "Método no válido." << endl;
    }

    archivoSalida.close();
}

int main() {
    const char* nombreArchivo = "input.txt";
    int n, metodo;

    cout << "Ingrese el tamaño de los bloques (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "El tamaño de los bloques debe ser mayor que 0." << endl;
        return 1;
    }

    cout << "Seleccione el método de codificación (1 para invertir bits, 2 para desplazar bits): ";
    cin >> metodo;

    leerArchivoYConvertirEnBloques(nombreArchivo, n, metodo);
    return 0;
}
