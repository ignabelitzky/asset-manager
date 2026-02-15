# AssetManager

AssetManager es un sistema ligero para administrar items en general, los cuáles pueden ser herramientas, maquinaria, muebles, aparatos, insumos, entre otros. Utiliza códigos de barra para hacer el seguimiento de salida y retorno de los elementos de trabajo de escuelas, talleres, instituciones, etc. Cada persona e item tiene un único identificador, que permite un rápido y confiable registro de quién retiró qué y cuándo.

## Características
- Basado en código de barras para la identificación de usuarios e items
- Registro rápido de la salida y retorno de los items
- Marcado de día y hora automático
- Base de datos local SQLite
- Interfaz C++/Qt
- Diseñado para equipos de bajos recursos (Linux/Windows, e.g. Raspberry Pi)

## ¿Cómo funciona?
1. Al usuario se le presenta un panel del sistema.
2. El panel presenta distintos botones de navegación (Usuarios, Items, Retiro, Devolución, Log, etc.).
3. Cada item que el usuario decide retirar es escaneado y registrado indicando día y hora.
4. Cuando el usuario decide entregar un item retirado, se vuelve a escanear para completar el log de devolución.

## Tecnologías utilizadas
- **Lenguaje:** C++17
- **Framework:** Qt (Widgets + SQL)
- **Base de Datos:** SQLite
- **Plataforma:** Linux, Windows

## Requisitos de desarrollo
- Qt 5 o 6 con módulos `Core`, `Gui`, `Widgets` y `Sql`
- Compilador con soporte C++17 (`g++`, `clang++` o MSVC)
- `qmake` y `make` (o equivalente en Windows)

## Estructura del proyecto
- `AssetManager/src/core`: ciclo de vida de la app, conexión y migraciones de base de datos.
- `AssetManager/src/dao`: acceso a datos (SQLite) por entidad.
- `AssetManager/src/models`: modelos Qt para tablas y filtros.
- `AssetManager/src/pages`: páginas principales de UI.
- `AssetManager/src/dialogs`: diálogos de alta/edición.
- `AssetManager/resources/sql`: scripts de migración.

## Compilar y ejecutar
Desde la raíz del repositorio:

```bash
cd AssetManager
qmake AssetManager.pro
make -j"$(nproc)"
./AssetManager
```

En Windows, reemplazar `make` por `nmake` o `jom` según el toolchain de Qt.

## Base de datos y migraciones
- La base SQLite se crea automáticamente al iniciar la app.
- Ubicación: `QStandardPaths::AppDataLocation` del sistema operativo.
- Las migraciones SQL se ejecutan desde recursos embebidos (`resources/sql/*.sql`).

Si necesitas reiniciar datos en local, elimina el archivo `assetmanager.db` en la carpeta de datos de la aplicación y vuelve a iniciar.

## Guía de contribución (rápida)
1. Crear una rama para el cambio.
2. Mantener cambios enfocados por tema (UI, DAO, docs, etc.).
3. Verificar compilación local antes de abrir PR.
4. Describir en el PR: problema, solución y validación.

## Metas del proyecto
- Proveer un sistema de trackeo simple, estable y offline.
- Minimizar errores de entrada de datos a través de los códigos de barra.
- Ser fácil de ejecutar en hardware de pocos recursos.

## Licencia
Este proyecto está licenciado bajo [GNU General Public License v3.0](LICENSE).
