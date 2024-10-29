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

// Función para dividir los bits leídos en bloques de tamaño n y mostrarlos
void dividirEnBloques(const string& bitsTotales, int n) {
    int longitud = bitsTotales.size();
    cout << "Bits divididos en bloques de " << n << ":\n";

    // Iterar sobre los bits y agruparlos en bloques de tamaño n
    for (int i = 0; i < longitud; i += n) {
        for (int j = i; j < i + n && j < longitud; ++j) {
            cout << bitsTotales[j];
        }
        cout << endl;
    }
}

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

    // Dividimos los bits en bloques de tamaño n y los mostramos
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

