#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Función para convertir un carácter ASCII en su representación binaria de 8 bits
void convertirABinario(int numero, char* binario) {
    for (int i = 7; i >= 0; --i) {
        binario[7 - i] = (numero & (1 << i)) ? '1' : '0';
    }
    binario[8] = '\0';
}

// funcion para invertir bits
void invertirBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio; ++i) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

// funcion para desplazar bits hacia la izquierda
void desplazarBits(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[0];
    for (int i = 0; i < tamanio - 1; ++i) {
        bloque[i] = bloque[i + 1];
    }
    bloque[tamanio - 1] = temp;
}

// funcion para desplazar bits hacia la derecha (para decodificación)
void desplazarBitsDerecha(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[tamanio - 1];
    for (int i = tamanio - 1; i > 0; --i) {
        bloque[i] = bloque[i - 1];
    }
    bloque[0] = temp;
}

// funcion para leer el archivo y convertirlo en bloques
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
        bloque[tamanioBloque] = '\0';

        if (metodo == 1) {
            invertirBits(bloque, tamanioBloque);
        } else if (metodo == 2) {
            desplazarBits(bloque, tamanioBloque);
        }

        archivoSalida << bloque;
    }

    archivoSalida.close();
}

//funcion para decodificar archivo
void decodificarArchivo(const string& nombreArchivoEntrada, const string& nombreArchivoSalida, int n, int metodo) {
    ifstream archivo(nombreArchivoEntrada, ios::in);

    if (!archivo) {
        cout << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    string bitsTotales = "";
    char bloque[n + 1];

    while (archivo.read(bloque, n)) {
        bloque[n] = '\0';
        if (metodo == 1) {
            invertirBits(bloque, n);
        } else if (metodo == 2) {
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

// Función para encriptar una clave
string encriptarClave(const string& claveOriginal, int metodo) {
    string claveEncriptada;

    for (char c : claveOriginal) {
        char binario[9];
        convertirABinario(static_cast<int>(c), binario);

        if (metodo == 1) {
            invertirBits(binario, 8);
        } else if (metodo == 2) {
            desplazarBits(binario, 8);
        }

        claveEncriptada += binario;
    }

    return claveEncriptada;
}

// Función para desencriptar una clave
string desencriptarClave(const string& claveEncriptada, int metodo) {
    string claveDesencriptada;

    for (size_t i = 0; i < claveEncriptada.size(); i += 8) {
        char bloque[9];

        for (size_t j = 0; j < 8 && i + j < claveEncriptada.size(); ++j) {
            bloque[j] = claveEncriptada[i + j];
        }

        bloque[8] = '\0';

        if (metodo == 1) {
            invertirBits(bloque, 8);
        } else if (metodo == 2) {
            desplazarBitsDerecha(bloque, 8);
        }

        int valor = stoi(string(bloque), nullptr, 2);
        claveDesencriptada += static_cast<char>(valor);
    }

    return claveDesencriptada;
}

bool validarAccesoAdministrador() {
    const string nombreArchivoClave = "sudo.txt";

    ifstream archivoClave(nombreArchivoClave);

    if (!archivoClave.is_open()) {
        cout << "Error al abrir el archivo de claves." << endl;
        return false;
    }

    string claveEncriptada;
    getline(archivoClave, claveEncriptada);
    archivoClave.close();

    string claveIngresada;
    cout << "Ingrese la clave de administrador: ";
    cin >> claveIngresada;

    int metodoSeleccionado = 1;
    string claveIngresadaEncriptada = encriptarClave(claveIngresada, metodoSeleccionado);

    if (claveIngresadaEncriptada == claveEncriptada) {
        cout << "Clave correcta." << endl;
        return true;
    } else {
        cout << "Clave incorrecta." << endl;
        return false;
    }
}

// Nueva función para registrar usuario
void registrarUsuario() {
    string cedula, clave;
    double saldo;
    const string archivoUsuarios = "usuarios.txt";

    cout << "\nRegistro de nuevo usuario" << endl;
    cout << "Ingrese numero de cedula: ";
    cin >> cedula;
    cout << "Ingrese clave del usuario: ";
    cin >> clave;
    cout << "Ingrese saldo inicial (COP): ";
    cin >> saldo;

    stringstream ss;
    ss << cedula << "," << clave << "," << saldo;
    string datosUsuario = ss.str();

    string datosEncriptados = encriptarClave(datosUsuario, 1);

    ofstream archivo(archivoUsuarios, ios::app);
    if (!archivo) {
        cout << "Error al abrir el archivo de usuarios." << endl;
        return;
    }

    archivo << datosEncriptados << endl;
    archivo.close();

    cout << "Usuario registrado exitosamente." << endl;
}

// Nueva función para mostrar usuarios registrados
void mostrarUsuarios() {
    const string archivoUsuarios = "usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if (!archivo) {
        cout << "No hay usuarios registrados." << endl;
        return;
    }

    string lineaEncriptada;
    int contador = 1;

    cout << "\nListado de usuarios registrados:" << endl;
    cout << "--------------------------------" << endl;

    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);

        stringstream ss(lineaDesencriptada);
        string cedula, clave, saldo;

        getline(ss, cedula, ',');
        getline(ss, clave, ',');
        getline(ss, saldo, ',');

        cout << "Usuario " << contador << ":" << endl;
        cout << "Cédula: " << cedula << endl;
        cout << "Saldo: $" << saldo << " COP" << endl;
        cout << "--------------------------------" << endl;

        contador++;
    }

    archivo.close();
}

bool iniciarSesionUsuario(string& cedulaUsuario) {
    string cedula, clave;
    cout << "Ingrese su numero de cedula: ";
    cin >> cedula;
    cout << "Ingrese su clave: ";
    cin >> clave;

    const string archivoUsuarios = "usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if (!archivo) {
        cout << "Error al abrir el archivo de usuarios." << endl;
        return false;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, claveArchivo, saldo;

        getline(ss, cedulaArchivo, ',');
        getline(ss, claveArchivo, ',');

        if (cedula == cedulaArchivo && clave == claveArchivo) {
            cedulaUsuario = cedula;
            archivo.close();
            return true;
        }
    }

    archivo.close();
    cout << "Credenciales incorrectas." << endl;
    return false;
}

// Función para actualizar el saldo en el archivo
void actualizarSaldo(const string& cedula, double nuevoSaldo) {
    const string archivoUsuarios = "usuarios.txt";
    const string archivoTemp = "temp.txt";
    ifstream archivo(archivoUsuarios);
    ofstream archivoTemporal(archivoTemp);

    if (!archivo || !archivoTemporal) {
        cout << "Error al acceder a los archivos." << endl;
        return;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, clave, saldoStr;

        getline(ss, cedulaArchivo, ',');
        getline(ss, clave, ',');
        getline(ss, saldoStr, ',');

        if (cedula == cedulaArchivo) {
            // Crear nueva línea con saldo actualizado
            stringstream nuevaLinea;
            nuevaLinea << cedulaArchivo << "," << clave << "," << nuevoSaldo;
            string lineaNueva = nuevaLinea.str();
            string lineaNuevaEncriptada = encriptarClave(lineaNueva, 1);
            archivoTemporal << lineaNuevaEncriptada << endl;
        } else {
            archivoTemporal << lineaEncriptada << endl;
        }
    }

    archivo.close();
    archivoTemporal.close();

    // Reemplazar el archivo original con el temporal
    remove(archivoUsuarios.c_str());
    rename(archivoTemp.c_str(), archivoUsuarios.c_str());
}

// Función para obtener el saldo actual
double obtenerSaldo(const string& cedula) {
    const string archivoUsuarios = "usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if (!archivo) {
        cout << "Error al abrir el archivo de usuarios." << endl;
        return -1;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, clave, saldoStr;

        getline(ss, cedulaArchivo, ',');
        getline(ss, clave, ',');
        getline(ss, saldoStr, ',');

        if (cedula == cedulaArchivo) {
            archivo.close();
            return stod(saldoStr);
        }
    }

    archivo.close();
    return -1;
}

// Función modificada para mostrar el saldo
void mostrarSaldo(const string& cedula) {
    double saldoActual = obtenerSaldo(cedula);
    if (saldoActual >= 0) {
        // Cobrar comisión por consulta
        double nuevoSaldo = saldoActual - 1000;
        if (nuevoSaldo >= 0) {
            actualizarSaldo(cedula, nuevoSaldo);
            cout << "Saldo anterior: $" << saldoActual << " COP" << endl;
            cout << "Cargo por consulta: $1000 COP" << endl;
            cout << "Saldo actual: $" << nuevoSaldo << " COP" << endl;
        } else {
            cout << "Saldo insuficiente para realizar la consulta." << endl;
        }
    } else {
        cout << "Error: No se encontro la informacion del usuario." << endl;
    }
}

// Función para realizar retiro
void realizarRetiro(const string& cedula) {
    double saldoActual = obtenerSaldo(cedula);
    if (saldoActual < 0) {
        cout << "Error al obtener el saldo." << endl;
        return;
    }

    double montoRetiro;
    cout << "Ingrese el monto a retirar: $";
    cin >> montoRetiro;

    // Verificar si hay saldo suficiente (incluyendo la comisión)
    if (saldoActual >= (montoRetiro + 1000)) {
        double nuevoSaldo = saldoActual - montoRetiro - 1000;
        actualizarSaldo(cedula, nuevoSaldo);
        cout << "\nTransacción exitosa:" << endl;
        cout << "Saldo anterior: $" << saldoActual << " COP" << endl;
        cout << "Monto retirado: $" << montoRetiro << " COP" << endl;
        cout << "Cargo por transaccion: $1000 COP" << endl;
        cout << "Saldo actual: $" << nuevoSaldo << " COP" << endl;
    } else {
        cout << "Saldo insuficiente para realizar el retiro." << endl;
        cout << "Recuerde que cada transaccion tiene un costo de $1000 COP." << endl;
    }
}

// Función de menú del usuario
void menuUsuario(const string& cedula) {
    int opcion;

    while (true) {
        cout << "\nPanel de Usuario" << endl;
        cout << "1. Consultar saldo" << endl;
        cout << "2. Realizar retiro" << endl;
        cout << "3. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        switch (opcion) {
        case 1:
            mostrarSaldo(cedula);
            break;

        case 2:
            realizarRetiro(cedula);
            break;

        case 3:
            cout << "Volviendo al menu principal..." << endl;
            return;

        default:
            cout << "Opcion no valida." << endl;
            break;
        }
    }
}

// Función de menú del administrador
void menuAdministrador() {
    int opcion;

    while (true) {
        cout << "\nPanel de Administrador" << endl;
        cout << "1. Registrar nuevo usuario" << endl;
        cout << "2. Ver usuarios registrados" << endl;
        cout << "3. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        switch (opcion) {
        case 1:
            registrarUsuario();
            break;

        case 2:
            mostrarUsuarios();
            break;

        case 3:
            cout << "Volviendo al menu principal..." << endl;
            return;

        default:
            cout << "Opcion no valida." << endl;
            break;
        }
    }
}

void entrarAplicacionBanco() {
    int opcion;

    while (true) {
        cout << "\nSeleccione una opcion:" << endl;
        cout << "1. Acceder como Administrador" << endl;
        cout << "2. Acceder como Usuario" << endl;
        cout << "3. Salir" << endl;

        cin >> opcion;

        switch (opcion) {
        case 1:
            if(validarAccesoAdministrador()) {
                menuAdministrador();
            }
            break;

        case 2:
        {
            string cedulaUsuario;
            if (iniciarSesionUsuario(cedulaUsuario)) {
                cout << "Inicio de sesion exitoso." << endl;
                menuUsuario(cedulaUsuario);
            }
        }
        break;

        case 3:
            cout << "Saliendo de la aplicacion bancaria..." << endl;
            return;

        default:
            cout << "Opcion no valida. Intente nuevamente." << endl;
            break;
        }
    }
}

int main() {
    string nombreArchivoEntrada, nombreArchivoSalida;
    int n, metodo, operacion;

    while (true) {
        cout << "\nSeleccione una operacion:" << endl;
        cout << "1. Codificar" << endl;
        cout << "2. Decodificar" << endl;
        cout << "3. Entrar a la aplicacion del banco" << endl;
        cout << "4. Salir" << endl;

        cin >> operacion;

        switch (operacion) {
        case 1:
            cout << "Ingrese el nombre del archivo de entrada: ";
            cin >> nombreArchivoEntrada;

            cout << "Ingrese el nombre del archivo de salida: ";
            cin >> nombreArchivoSalida;

            cout << "Ingrese el tamano de los bloques (n): ";
            cin >> n;

            if (n <= 0) {
                cout << "El tamano de los bloques debe ser mayor que 0." << endl;
                break;
            }

            cout << "Seleccione el metodo de codificacion (1 para invertir bits, 2 para desplazar bits): ";
            cin >> metodo;

            leerArchivoYConvertirEnBloques(nombreArchivoEntrada, nombreArchivoSalida, n, metodo);
            break;

        case 2:
            cout << "Ingrese el nombre del archivo de entrada: ";
            cin >> nombreArchivoEntrada;

            cout << "Ingrese el nombre del archivo de salida: ";
            cin >> nombreArchivoSalida;

            cout << "Ingrese el tamano de los bloques (n): ";
            cin >> n;

            if (n <= 0) {
                cout << "El tamano de los bloques debe ser mayor que 0." << endl;
                break;
            }

            cout << "Seleccione el metodo de decodificacion (1 para invertir bits, 2 para desplazar bits): ";
            cin >> metodo;

            decodificarArchivo(nombreArchivoEntrada, nombreArchivoSalida, n, metodo);
            break;

        case 3:
            entrarAplicacionBanco();
            break;

        case 4:
            cout << "Saliendo..." << endl;
            return 0;

        default:
            cout << "Operacion no valida. Intente nuevamente." << endl;
            break;
        }
    }

    return 0;
}

//profe el metodo para el tema de la clave del banco encriptada es el 1 y la semilla 8
