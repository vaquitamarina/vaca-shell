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
