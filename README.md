# 🐮 VacaShell - Mini-shell POSIX en C++

![Versión](https://img.shields.io/badge/versi%C3%B3n-1.0-blue)
![C++](https://img.shields.io/badge/C++-17-orange)
![POSIX](https://img.shields.io/badge/POSIX-compliant-green)
![Licencia](https://img.shields.io/badge/licencia-MIT-brightgreen)

**VacaShell** es una mini-shell completa implementada en C++ que simula el comportamiento de una terminal Unix/Linux. Utiliza llamadas POSIX estándar (`fork`, `execvp`, `wait`, `waitpid`, `pipe`, `dup2`, `open`, `close`, `sigaction`) y está diseñada con fines educativos para comprender el funcionamiento interno de shells como Bash.

## ✨ Características

### Características Base (Obligatorias) ✅

| Característica | Implementación | Estado |
|----------------|----------------|--------|
| **Prompt personalizado** | usuario@hostname:directorio símbolo $ | ✅ |
| **Resolución de rutas** | Absolutas, relativas, búsqueda en /bin/ | ✅ |
| **Ejecución con fork+exec** | fork(), execvp() en proceso hijo | ✅ |
| **Espera de procesos** | wait(), waitpid() | ✅ |
| **Manejo de errores** | errno, perror() con mensajes en español | ✅ |
| **Redirección salida (>)** | open(), dup2(), close() | ✅ |
| **Comando salir** | Termina la shell limpiamente | ✅ |

### Extensiones Implementadas ✅

| Extensión | Descripción | Estado |
|-----------|-------------|--------|
| **Pipes (\|)** | cmd1 \| cmd2 \| cmd3 (múltiples pipes) | ✅ |
| **Background (&)** | Ejecución no bloqueante con waitpid(WNOHANG) | ✅ |
| **Redirección entrada (<)** | Redirigir stdin desde archivo | ✅ |
| **Redirección append (>>)** | Agregar a archivo sin truncar | ✅ |
| **Comandos internos** | cd, pwd, help, history, alias, jobs | ✅ |
| **Manejo de señales** | sigaction() para SIGINT y SIGCHLD | ✅ |
| **Historial** | Almacena comandos ejecutados | ✅ |
| **Aliases** | Crea atajos de comandos | ✅ |

---

## 📦 Requisitos

### Software Necesario

- **Sistema Operativo**: Linux (Ubuntu, Fedora, Debian, Arch, etc.)
- **Compilador**: g++ con soporte C++17 o superior
- **Make**: Para automatizar la compilación
- **Git**: Para control de versiones (opcional)

### Instalación de Dependencias

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential git
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-c++ make git
```

**Arch Linux:**
```bash
sudo pacman -S base-devel git
```

---

## 🚀 Instalación

### 1. Clonar o descargar el repositorio

```bash
git clone https://github.com/vaquitamarina/vaca-shell.git
cd vaca-shell
```

### 2. Compilar

```bash
make
```

### 3. Ejecutar

```bash
./vacashell
```

### 4. Si hay problemas compilando el make

```bash
make clean && make
```

---

## 🧪 Casos de Prueba

### Prueba 1: Comando Simple
Ejecutar comandos básicos del sistema.

```bash
🐮 $ ls
🐮 $ ls -la
🐮 $ pwd
🐮 $ echo "Hola VacaShell"
```

**Resultado esperado:** Los comandos se ejecutan correctamente y muestran su salida.

---

### Prueba 2: Comandos Internos (Builtins)

#### cd - Cambiar directorio
```bash
🐮 $ pwd
/home/usuario
🐮 $ cd /tmp
🐮 $ pwd
/tmp
🐮 $ cd ..
🐮 $ pwd
/
🐮 $ cd
🐮 $ pwd
/home/usuario
```

**Resultado esperado:** El directorio cambia correctamente y el prompt se actualiza.

#### help - Ayuda
```bash
🐮 $ help
```

**Resultado esperado:** Muestra lista de comandos disponibles y su uso.

---

### Prueba 3: Pipes Simples
Conectar la salida de un comando con la entrada de otro.

```bash
🐮 $ ls -1 | grep .cpp
🐮 $ cat README.md | grep VacaShell
🐮 $ ps aux | grep bash
```

**Resultado esperado:** Los comandos se ejecutan en paralelo y los datos fluyen correctamente.

---

### Prueba 4: Pipes Múltiples
Cadenas de 3 o más comandos conectados.

```bash
🐮 $ cat archivo.txt | grep error | wc -l
🐮 $ ls -1 | sort | uniq | wc -l
🐮 $ ps aux | grep user | awk '{print $2}' | head -5
```

**Resultado esperado:** Todos los comandos se ejecutan correctamente en cadena.

---

### Prueba 5: Redirección de Salida (>)
Guardar la salida de un comando en un archivo.

```bash
🐮 $ ls -la > listado.txt
🐮 $ cat listado.txt
🐮 $ echo "Nueva línea" > archivo.txt
🐮 $ cat archivo.txt
```

**Resultado esperado:** Los archivos se crean/truncan y contienen la salida del comando.

---

### Prueba 6: Redirección Append (>>)
Agregar salida al final de un archivo sin truncar.

```bash
🐮 $ echo "Línea 1" > log.txt
🐮 $ echo "Línea 2" >> log.txt
🐮 $ echo "Línea 3" >> log.txt
🐮 $ cat log.txt
Línea 1
Línea 2
Línea 3
```

**Resultado esperado:** El archivo crece con cada append, sin perder contenido anterior.

---

### Prueba 7: Redirección de Entrada (<)
Leer entrada desde un archivo.

```bash
🐮 $ echo -e "línea1\nlínea2\nlínea3" > entrada.txt
🐮 $ wc -l < entrada.txt
3
🐮 $ sort < entrada.txt
```

**Resultado esperado:** El comando lee desde el archivo en vez del teclado.

---

### Prueba 8: Combinación de Redirecciones
Usar entrada y salida simultáneamente.

```bash
🐮 $ sort < desordenado.txt > ordenado.txt
🐮 $ grep error < log.txt > errores.txt
```

**Resultado esperado:** El comando lee de un archivo y escribe a otro.

---

### Prueba 9: Ejecución en Background (&)
Ejecutar comandos sin bloquear el prompt.

```bash
🐮 $ sleep 10 &
[1] 12345
🐮 $ echo "Shell sigue disponible"
Shell sigue disponible
🐮 $ jobs
[1]  Running    sleep 10 &
```

**Resultado esperado:** El prompt retorna inmediatamente. El proceso corre en segundo plano.

---

### Prueba 10: Historial de Comandos
Navegar por comandos anteriores.

```bash
🐮 $ ls
🐮 $ pwd
🐮 $ echo "test"
🐮 $ history
1  ls
2  pwd
3  echo "test"
4  history
```

**Resultado esperado:** Muestra todos los comandos ejecutados en la sesión.

**Navegación con flechas:**
- ⬆️ (Flecha arriba): Comando anterior
- ⬇️ (Flecha abajo): Comando siguiente

---

### Prueba 11: Manejo de Señales (Ctrl+C)
Interrumpir procesos sin cerrar la shell.

```bash
🐮 $ cat
(espera entrada...)
^C
🐮 $ pwd
/home/usuario
```

**Resultado esperado:** 
- Ctrl+C termina el proceso hijo (cat)
- La shell NO se cierra
- El prompt vuelve a aparecer

---

### Prueba 12: Caso Complejo - Todo Junto
Combinación de múltiples funcionalidades.

```bash
🐮 $ cat datos.txt | grep importante | sort | uniq > resultados.txt &
[1] 12350
🐮 $ jobs
[1]  Running    cat datos.txt | grep importante | sort | uniq > resultados.txt
🐮 $ 
[1]  Done       cat datos.txt | grep importante | sort | uniq > resultados.txt
🐮 $ cat resultados.txt
```

**Resultado esperado:** 
- 4 comandos en pipeline
- Redirección de salida
- Ejecución en background
- Todo funciona correctamente

---

### Prueba 13: Manejo de Errores

#### Comando no existe
```bash
🐮 $ comandoinexistente
Error: comando no encontrado: comandoinexistente
```

#### Archivo no existe
```bash
🐮 $ cat archivoquenoexiste.txt
cat: archivoquenoexiste.txt: No existe el archivo o el directorio
```

#### Permiso denegado
```bash
🐮 $ cd /root
cd: /root: Permiso denegado
```

**Resultado esperado:** Mensajes de error descriptivos. La shell continúa funcionando.

---

### Prueba 15: Comandos con Comillas
Manejo de argumentos con espacios.

```bash
🐮 $ echo "Hola Mundo"
Hola Mundo
🐮 $ mkdir "Mi Carpeta"
🐮 $ ls -d "Mi Carpeta"
Mi Carpeta
```

**Resultado esperado:** Los argumentos entre comillas se tratan como uno solo.

---

### Prueba 16: Comentarios
Líneas que empiezan con # son ignoradas.

```bash
🐮 $ # Esto es un comentario
🐮 $ echo "Visible"
Visible
🐮 $    # Comentario con espacios
🐮 $ 
```

**Resultado esperado:** Los comentarios no se ejecutan. No producen error.

---

### Prueba 17: Salir de la Shell

```bash
🐮 $ exit
¡Hasta luego! 👋
$ 
```

**O también:**
```bash
🐮 $ salir
¡Hasta luego! 👋
$ 
```

**Resultado esperado:** La shell termina limpiamente y retorna al shell del sistema.

---

## 📊 Resumen de Pruebas

| Categoría | Casos | Estado |
|-----------|-------|--------|
| Comandos básicos | 3 | ✅ |
| Builtins | 2 | ✅ |
| Pipes | 2 | ✅ |
| Redirecciones | 4 | ✅ |
| Background | 1 | ✅ |
| Historial | 1 | ✅ |
| Aliases | 1 | ✅ |
| Señales | 1 | ✅ |
| Casos complejos | 1 | ✅ |
| Manejo de errores | 1 | ✅ |
| Características avanzadas | 3 | ✅ |
| **TOTAL** | **20** | **✅** |

---

