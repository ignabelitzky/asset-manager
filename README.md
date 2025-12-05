# AssetManager

AssetManager es un sistema ligero para administrar items en general, los cuáles pueden ser herramientas, maquinaria, muebles, aparatos, insumos, entre otros, utiliza códigos de barra para hacer el seguimiento de salida y retorno de los elementos de trabajo de escuelas, talleres, instituciones, etc. Cada persona e item tiene un único identificador, que permite un rápido y confiable registro de quién retiró que y cuando.

## Características
- Basado en código de barras para la identificación de usuarios e items
- Rápida registro de la salida y retorno de los items
- Marcado de día y hora automático
- Base de datos local SQLite
- Interfaz C++/Qt
- Perfecto para mini-PCs basadas en Linux o Windows (e.g. Raspberry Pi)

## ¿Cómo funciona?
1. Al usuario se le presenta un panel del sistema.
2. El panel presenta distintos botones de navegación (Usuarios, Items, Retiro, Devolución, Log, etc.).
3. Cada item que el usuario decide retirar es escaneada y registrada indicando día y hora.
4. Cuando el usuario decide entregar un item retirado, se vuelve a escanear para completar el log de devolución.

## Tecnologías utilizadas
- **Lenguaje:** C++
- **Framework:** Qt
- **Base de Datos:** SQLite
- **Plataforma:** Linux, Windows

## Metas del Proyecto
- Proveer un sistema de trackeo simple, estable y offline
- Minimizar errores de entrada de datos a través de los códigos de barra
- Ser fácil de ejecutar en hardware de pocos recursos

## Licencia
Este proyecto esta licenciado bajo [GNU General Public License v3.0](LICENSE). Puedes encontrar toda la descripción de la licencia aquí [LICENCIA](LICENSE).
