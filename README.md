# Parser & SDT - Grupo 5 (Equipo 02)

Este proyecto implementa un Analizador Sintáctico (Parser) y un Analizador Semántico (SDT) utilizando una **Arquitectura Híbrida**.

## Arquitectura de Software
Para maximizar el rendimiento y aplicar buenas prácticas de ingeniería, el sistema divide sus responsabilidades:
1. **Backend en C (`parser.c`):** Actúa como el Analizador Léxico y el Parser Top-Down. Lee la entrada, verifica la gramática y construye un Árbol de Sintaxis Abstracta (AST) en la memoria utilizando punteros. Si la estructura es correcta, serializa el AST en formato JSON.
2. **Frontend y SDT en Python (`main.py`):** Ejecuta la interfaz gráfica moderna (CustomTkinter), orquesta la ejecución del ejecutable en C mediante subprocesos y recibe el JSON. Finalmente, recorre el árbol para aplicar las reglas de la Traducción Dirigida por Sintaxis (Type Checking).

## Ejecución del Proyecto
1. Crear el entorno virtual: `python -m venv venv`
2. Activar el entorno: `.\venv\Scripts\activate` (Windows)
3. Instalar dependencias: `pip install -r requirements.txt`
4. Ejecutar: `python main.py`

## Argumentos Teóricos de las Salidas Posibles
El programa genera tres estados posibles basados en la validación del código fuente:

* **`Parsing Success!` y `SDT Verified!`**: 
  * *Justificación:* El código fuente cumple estrictamente con las reglas de producción de la Gramática Libre de Contexto (CFG). Además, el SDT confirma que la semántica es válida; por ejemplo, si se declara un `int`, la expresión asignada evalúa correctamente a un tipo numérico compatible.
* **`Parsing error...`**: 
  * *Justificación:* El Analizador Léxico encontró un símbolo no reconocido, o el Parser detectó que la secuencia de tokens viola las reglas gramaticales (por ejemplo, omitir un punto y coma `;` al final de la declaración). El Árbol de Sintaxis Abstracta no se genera.
* **`Parsing Success!` y `SDT error...`**: 
  * *Justificación:* El código es sintácticamente perfecto y el árbol AST se construye con éxito. Sin embargo, durante el recorrido del árbol, el Analizador Semántico detecta una inconsistencia lógica, como intentar asignar un valor de tipo `string` a una variable declarada como `int`.
