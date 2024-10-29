#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Función para convertir un carácter ASCII en su representación binaria de 8 bits
void convertirABinario(int numero, char* binario) {
    for (int i = 7; i >= 0; --i) {
        binario[7 - i] = (numero & (1 << i)) ? '1' : '0';
    }
    binario[8] = '\0';  // Asegurar fin de cadena
}

// Método para invertir bits
void invertirBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio; ++i) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

// Método para desplazar bits hacia la izquierda
void desplazarBits(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[0]; // Cambiar el orden de desplazamiento
    for (int i = 0; i < tamanio - 1; ++i) {
        bloque[i] = bloque[i + 1];
    }
    bloque[tamanio - 1] = temp;
}

// Método para desplazar bits hacia la derecha (para decodificación)
void desplazarBitsDerecha(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[tamanio - 1]; // Guardar el último bit
    for (int i = tamanio - 1; i > 0; --i) {
        bloque[i] = bloque[i - 1]; // Desplazar hacia la derecha
    }
    bloque[0] = temp; // Colocar el último bit en la primera posición
}

// Función para leer el archivo y convertirlo en bloques
void leerArchivoYConvertirEnBloques(const string& nombreArchivoEntrada, const string& nombreArchivoSalida, int n, int metodo) {
    ifstream archivo(nombreArchivoEntrada, ios::in);

    if (!archivo) {
        cout << "Error al abrir el archivo de entrada." << endl;
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

    ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida) {
        cout << "Error al crear el archivo de salida." << endl;
        return;
    }

    for (size_t i = 0; i < bitsTotales.size(); i += n) {
        char bloque[n + 1];
        int tamanioBloque = 0;

        for (size_t j = i; j < i + n && j < bitsTotales.size(); ++j) {
            bloque[tamanioBloque++] = bitsTotales[j];
        }
        bloque[tamanioBloque] = '\0'; // Asegurar fin de cadena

        if (metodo == 1) {
            invertirBits(bloque, tamanioBloque);
        } else if (metodo == 2) {
            desplazarBits(bloque, tamanioBloque);
        }

        archivoSalida << bloque;
    }

    archivoSalida.close();
}

void decodificarArchivo(const string& nombreArchivoEntrada, const string& nombreArchivoSalida, int n, int metodo) {
    ifstream archivo(nombreArchivoEntrada, ios::in);

    if (!archivo) {
        cout << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    string bitsTotales = "";
    char bloque[n + 1];

    while (archivo.read(bloque, n)) {
        bloque[n] = '\0'; // Asegurar fin de cadena
        if (metodo == 1) {
            // Invertir los bits del bloque para decodificar
            invertirBits(bloque, n);
        } else if (metodo == 2) {
            // Desplazar los bits en sentido contrario
            desplazarBitsDerecha(bloque, n);
        }
        bitsTotales += bloque;
    }

    archivo.close();

    ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida) {
        cout << "Error al crear el archivo de salida." << endl;
        return;
    }

    // Convertir los bits totales de vuelta a caracteres ASCII
    for (size_t i = 0; i < bitsTotales.size(); i += 8) {
        if (i + 8 <= bitsTotales.size()) {
            int valor = 0;
            for (int j = 0; j < 8; ++j) {
                valor = (valor << 1) | (bitsTotales[i + j] - '0');
            }
            archivoSalida << static_cast<char>(valor);
        }
    }

    archivoSalida.close();
}

int main() {
    string nombreArchivoEntrada, nombreArchivoSalida;
    int n, metodo, operacion;

    cout << "Seleccione la operación (1 para codificar, 2 para decodificar): ";
    cin >> operacion;

    cout << "Ingrese el nombre del archivo de entrada: ";
    cin >> nombreArchivoEntrada;

    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> nombreArchivoSalida;

    cout << "Ingrese el tamaño de los bloques (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "El tamaño de los bloques debe ser mayor que 0." << endl;
        return 1;
    }

    if (operacion == 1) {
        cout << "Seleccione el método de codificación (1 para invertir bits, 2 para desplazar bits): ";
        cin >> metodo;
        leerArchivoYConvertirEnBloques(nombreArchivoEntrada, nombreArchivoSalida, n, metodo);
    } else if (operacion == 2) {
        cout << "Seleccione el método de decodificación (1 para invertir bits, 2 para desplazar bits): ";
        cin >> metodo;
        decodificarArchivo(nombreArchivoEntrada, nombreArchivoSalida, n, metodo);
    } else {
        cout << "Operación no válida." << endl;
    }

    return 0;
}
