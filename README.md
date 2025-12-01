# ToolTrack

ToolTrack es un sistema ligero para administrar herramientas, utiliza códigos de barra para hacer el seguimiento de salida y retorno de los elementos de trabajo en escuelas, talleres e instituciones. Cada persona y herramienta tiene un único identificador, que permite un rápido y confiable registro de quién retiró que y cuando.

## Características
- Basado en código de barras para la identificación de usuarios y herramientas
- Rápida registro de la salida y retorno de las herramientas
- Marcado de día y hora automático
- Base de datos local SQLite
- Interfaz C++/Qt
- Perfecto para mini-PCs basadas en Linux (e.g. Raspberry Pi)

## ¿Cómo funciona?
1. Un usuario escanea su código de barras identificatorio.
2. El sistema entra en modo de retiro.
3. Cada herramienta que el usuario toma es escaneada y registrada indicando día y hora.
4. Cuando se devuelven la o las herramientas, el usuario vuelve a escanear para completar el log de devolución.

## Tecnologías utilizadas
- **Lenguaje:** C++
- **Framework:** Qt
- **Base de Datos:** SQLite
- **Plataforma:** Linux

## Metas del Proyecto
- Proveer un sistema de trackeo simple, estable y offline
- Minimizar errores de entrada de datos a través de los códigos de barra
- Ser fácil de ejecutar en hardware de pocos recursos

## Licencia
Este proyecto esta licenciado bajo [GNU General Public License v3.0](LICENSE). Puedes encontrar toda la descripción de la licencia aquí [LICENCIA](LICENSE).
