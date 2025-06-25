import streamlit as st
import subprocess
import tempfile
import os
import sys
import time
import re

# Importar streamlit-ace solo si está disponible
try:
    from streamlit_ace import st_ace
    ACE_AVAILABLE = True
except ImportError:
    ACE_AVAILABLE = False

# Configuración de la página
st.set_page_config(
    page_title="Kotlin Compiler IDE",
    page_icon="⚡",
    layout="wide",
    initial_sidebar_state="expanded"
)

# CSS personalizado
st.markdown("""
<style>
    .main-header {
        background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
        padding: 1rem;
        border-radius: 10px;
        margin-bottom: 2rem;
    }
    
    .main-header h1 {
        color: white;
        margin: 0;
        text-align: center;
    }
    
    .code-container {
        background-color: #f8f9fa;
        padding: 1rem;
        border-radius: 8px;
        border-left: 4px solid #667eea;
    }
    
    .success-box {
        background-color: #d4edda;
        color: #155724;
        padding: 1rem;
        border-radius: 5px;
        border: 1px solid #c3e6cb;
    }
    
    .error-box {
        background-color: #f8d7da;
        color: #721c24;
        padding: 1rem;
        border-radius: 5px;
        border: 1px solid #f5c6cb;
    }
    
    /* Mejorar el editor de texto cuando ACE no está disponible */
    .stTextArea textarea {
        font-family: 'Consolas', 'Monaco', 'Courier New', monospace !important;
        font-size: 14px !important;
        line-height: 1.5 !important;
        tab-size: 4 !important;
        -moz-tab-size: 4 !important;
    }
</style>

<script>
document.addEventListener('DOMContentLoaded', function() {
    function enableTabInTextarea() {
        const textareas = document.querySelectorAll('.stTextArea textarea');
        textareas.forEach(function(textarea) {
            if (!textarea.hasAttribute('data-tab-enabled')) {
                textarea.setAttribute('data-tab-enabled', 'true');
                textarea.addEventListener('keydown', function(e) {
                    if (e.key === 'Tab') {
                        e.preventDefault();
                        const start = this.selectionStart;
                        const end = this.selectionEnd;
                        
                        this.value = this.value.substring(0, start) + '    ' + this.value.substring(end);
                        
                        this.selectionStart = this.selectionEnd = start + 4;
                        
                        this.dispatchEvent(new Event('input', { bubbles: true }));
                    }
                });
            }
        });
    }
    
    setTimeout(enableTabInTextarea, 500);
    
    const observer = new MutationObserver(function(mutations) {
        mutations.forEach(function(mutation) {
            if (mutation.addedNodes.length > 0) {
                setTimeout(enableTabInTextarea, 100);
            }
        });
    });
    
    observer.observe(document.body, {
        childList: true,
        subtree: true
    });
});
</script>
""", unsafe_allow_html=True)

# Título principal
st.markdown("""
<div class="main-header">
    <h1>⚡ Kotlin Compiler IDE</h1>
    <p style="color: white; text-align: center; margin: 0;">Compilador de Kotlin desarrollado en C++</p>
</div>
""", unsafe_allow_html=True)

# Botones de información en la parte superior
col_info1, col_info2 = st.columns(2)

with col_info1:
    info_btn = st.button("ℹ️ Información del Compilador", use_container_width=True)

with col_info2:
    help_btn = st.button("❓ Guía de Sintaxis", use_container_width=True)

# Modal para información del compilador
@st.dialog("ℹ️ Información del Compilador")
def show_compiler_info():
    st.markdown("""
    ### 🔧 **Compilador de Kotlin**
    
    Este es un compilador de Kotlin desarrollado completamente en **C++** como proyecto académico.
    
    **📋 Características implementadas:**
    - ✅ **Análisis Léxico**: Tokenización completa del código Kotlin
    - ✅ **Análisis Sintáctico**: Parser que construye el AST
    - ✅ **Print Visitor**: Reconstrucción del código fuente
    - ✅ **Eval Visitor**: Ejecución e interpretación del código
    
    **🎯 Funcionalidades soportadas:**
    - Variables y tipos de datos (`Int`, `Float`, `String`, `Boolean`)
    - Operaciones aritméticas (`+`, `-`, `*`, `/`, `%`)
    - Operaciones lógicas (`&&`, `||`, `!`)
    - Operaciones relacionales (`==`, `!=`, `<`, `>`, `<=`, `>=`)
    - Estructuras de control (`if/else`, `for`, `while`)
    - Funciones con parámetros y valores de retorno
    - Bloques `run` con alcance de variables
    - Rangos con `..` y `step`
    - Incremento/decremento (`++`, `--`)
    - Asignación compuesta (`+=`, `-=`, `*=`, `/=`)
    
    **🏗️ Arquitectura:**
    - **Scanner**: Análisis léxico usando autómatas finitos
    - **Parser**: Análisis sintáctico con gramática LL(1)
    - **AST**: Árbol de sintaxis abstracta
    - **Visitors**: Patrón Visitor para recorrido del AST
    
    **📚 Proyecto desarrollado en:**
    - Universidad de Ingeniería y Tecnología (UTEC)
    - Curso: Compiladores
    - Año: 2025-1
    """)
    
    if st.button("✅ Cerrar", use_container_width=True):
        st.rerun()

# Modal para guía de sintaxis
@st.dialog("❓ Guía de Sintaxis Kotlin")
def show_syntax_guide():
    st.markdown("""
    ### 📖 **Guía de Sintaxis Kotlin**
    
    Esta guía te ayudará a escribir código Kotlin compatible con nuestro compilador.
    """)
    
    # Tabs para organizar la información
    tab1, tab2, tab3, tab4 = st.tabs(["🔤 Básico", "🔧 Funciones", "🔄 Control", "🏃 Avanzado"])
    
    with tab1:
        st.markdown("""
        #### **Variables y Tipos**
        ```kotlin
        var nombre: String = "Juan"      // Variable mutable
        val edad: Int = 25               // Variable inmutable
        var precio: Float = 19.99f       // Flotante
        var activo: Boolean = true       // Booleano
        ```
        
        #### **Operaciones Aritméticas**
        ```kotlin
        var a: Int = 10
        var b: Int = 3
        
        println(a + b)    // Suma: 13
        println(a - b)    // Resta: 7
        println(a * b)    // Multiplicación: 30
        println(a / b)    // División: 3
        println(a % b)    // Módulo: 1
        
        a += 5           // Asignación compuesta
        a++              // Incremento
        ```
        
        #### **Operaciones Lógicas**
        ```kotlin
        var x: Boolean = true
        var y: Boolean = false
        
        println(x && y)   // AND lógico
        println(x || y)   // OR lógico
        println(!x)       // NOT lógico
        ```
        """)
    
    with tab2:
        st.markdown("""
        #### **Definición de Funciones**
        ```kotlin
        fun nombreFuncion(parametro: Tipo): TipoRetorno {
            // código
            return valor
        }
        ```
        
        #### **Ejemplos de Funciones**
        ```kotlin
        fun sumar(a: Int, b: Int): Int {
            return a + b
        }
        
        fun saludar(nombre: String): String {
            return "Hola, " + nombre + "!"
        }
        
        fun esPar(num: Int): Boolean {
            return num % 2 == 0
        }
        
        fun main(): Unit {
            var resultado: Int = sumar(5, 3)
            println(resultado)  // Imprime: 8
        }
        ```
        """)
    
    with tab3:
        st.markdown("""
        #### **Condicionales**
        ```kotlin
        if (condicion) {
            // código si verdadero
        } else {
            // código si falso
        }
        
        // Ejemplo
        var edad: Int = 18
        if (edad >= 18) {
            println("Mayor de edad")
        } else {
            println("Menor de edad")
        }
        ```
        
        #### **Bucles**
        ```kotlin
        // Bucle for con rango
        for (i in 1..5) {
            println(i)
        }
        
        // Bucle for con step
        for (i in 0..10 step 2) {
            println(i)  // 0, 2, 4, 6, 8, 10
        }
        
        // Bucle while
        var contador: Int = 0
        while (contador < 5) {
            println(contador)
            contador++
        }
        ```
        """)
    
    with tab4:
        st.markdown("""
        #### **Bloques Run**
        ```kotlin
        // Bloque run simple
        run {
            var temp: Int = 10
            println("Dentro del bloque: " + temp)
        }
        
        // Bloque run con valor de retorno
        var resultado: Int = run {
            var a: Int = 5
            var b: Int = 3
            a * b  // Última expresión es el valor de retorno
        }
        ```
        
        #### **Rangos**
        ```kotlin
        1..10        // Rango de 1 a 10 (inclusivo)
        1..10 step 2 // Rango con paso de 2
        ```
        
        #### **Notas Importantes**
        - ⚠️ **Punto y coma**: No necesarios al final de las líneas
        - ⚠️ **Tipos**: Siempre especificar tipos explícitamente
        - ⚠️ **Strings**: Usar comillas dobles `"texto"`
        - ⚠️ **Concatenación**: Usar `+` para unir strings
        - ⚠️ **Float**: Agregar `f` al final de números decimales
        """)
    
    if st.button("✅ Cerrar", use_container_width=True):
        st.rerun()

# Mostrar modales cuando se presionen los botones
if info_btn:
    show_compiler_info()

if help_btn:
    show_syntax_guide()

# Inicializar estados de sesión
if 'code_content' not in st.session_state:
    st.session_state.code_content = """fun main(): Unit {
    println("¡Hola Mundo!")
    println("Bienvenido al compilador Kotlin")
}"""

if 'editor_key' not in st.session_state:
    st.session_state.editor_key = 0

if 'last_scanner_output' not in st.session_state:
    st.session_state.last_scanner_output = ""

if 'last_parser_output' not in st.session_state:
    st.session_state.last_parser_output = ""

if 'last_print_visitor_output' not in st.session_state:
    st.session_state.last_print_visitor_output = ""

if 'last_eval_visitor_output' not in st.session_state:
    st.session_state.last_eval_visitor_output = ""

if 'last_error' not in st.session_state:
    st.session_state.last_error = ""

if 'compilation_status' not in st.session_state:
    st.session_state.compilation_status = ""

# Layout principal en dos columnas
col1, col2 = st.columns([1, 1])

with col1:
    st.header("📝 Editor de Código")
    
    if ACE_AVAILABLE:
        st.markdown("**Editor Avanzado** - Usa Tab para indentar, Ctrl+Z para deshacer")
        
        # Editor de código con ACE Editor
        code_input = st_ace(
            value=st.session_state.code_content,
            language='kotlin',
            theme='monokai',
            key=f"code_editor_{st.session_state.editor_key}",
            height=400,
            auto_update=True,
            font_size=14,
            tab_size=4,
            wrap=False
        )
    else:
        st.markdown("**Editor de Texto** - Usa Tab para indentar (4 espacios)")
        
        # Editor de texto mejorado con JavaScript
        code_input = st.text_area(
            "Escribe tu código Kotlin aquí:",
            height=400,
            value=st.session_state.code_content,
            key="code_editor_fallback",
            help="Editor básico con soporte para tabulación. Tab insertará 4 espacios.",
            placeholder="fun main(): Unit {\n    println(\"¡Hola Mundo!\")\n}"
        )
    
    # Actualizar el estado de sesión solo si el contenido cambió
    if code_input != st.session_state.code_content:
        st.session_state.code_content = code_input
    
    # Botones cerca del editor
    col_btn1, col_btn2, col_btn3 = st.columns(3)
    
    with col_btn1:
        run_btn = st.button("▶️ Compilar", use_container_width=True, type="primary")
    
    with col_btn2:
        clear_btn = st.button("🗑️ Limpiar Editor", use_container_width=True)
    
    with col_btn3:
        download_btn = st.button("💾 Descargar Código", use_container_width=True)
    
    # Lógica del botón limpiar
    if clear_btn:
        st.session_state.code_content = """fun main(): Unit {
    println("¡Hola Mundo!")
}"""
        st.session_state.editor_key += 1  # Forzar recarga del editor
        st.session_state.last_scanner_output = ""
        st.session_state.last_parser_output = ""
        st.session_state.last_print_visitor_output = ""
        st.session_state.last_eval_visitor_output = ""
        st.session_state.last_error = ""
        st.session_state.compilation_status = ""
        st.rerun()
    
    # Lógica del botón descargar
    if download_btn:
        timestamp = time.strftime("%Y%m%d_%H%M%S")
        filename = f"codigo_kotlin_{timestamp}.kt"
        st.download_button(
            label="📥 Descargar archivo .kt",
            data=st.session_state.code_content,
            file_name=filename,
            mime="text/plain",
            use_container_width=True
        )
    
    # Información del archivo
    st.info(f"📄 Líneas: {len(st.session_state.code_content.splitlines())} | Caracteres: {len(st.session_state.code_content)}")

with col2:
    st.header("📊 Resultados de Compilación")
    
    # Función para parsear la salida del compilador (LIMPIA)
    def parse_compiler_output(output):
        sections = {
            'tokens': '',
            'print_visitor': '',
            'eval_visitor': ''
        }
        
        lines = output.split('\n')
        current_section = None
        
        for line in lines:
            # Detectar inicio de secciones
            if 'Starting Scanner Test:' in line:
                current_section = 'tokens'
                sections['tokens'] += line + '\n'
                continue
            
            # Filtrar líneas que no queremos mostrar
            if any(phrase in line for phrase in ['Iniciando Visitor:', 'Iniciando parsing:', 'Scanner exitoso', 'Parsing exitoso']):
                continue
            
            # Detectar inicio de Print Visitor
            if 'IMPRIMIR:' in line:
                current_section = 'print_visitor'
                continue
            
            # Detectar inicio de Eval Visitor
            if 'EJECUTAR:' in line:
                current_section = 'eval_visitor'
                continue
            
            # Agregar contenido a las secciones correspondientes
            if current_section == 'tokens' and line.strip():
                if line.startswith('TOKEN(') or 'TOKEN(END)' in line:
                    sections['tokens'] += line + '\n'
            
            elif current_section == 'print_visitor' and line.strip():
                # No incluir líneas que indican cambio de sección
                if not line.startswith('EJECUTAR:'):
                    sections['print_visitor'] += line + '\n'
            
            elif current_section == 'eval_visitor' and line.strip():
                sections['eval_visitor'] += line + '\n'
        
        return sections

    # Función para ejecutar el compilador
    def run_compiler(code):
        try:
            # Crear archivo temporal
            with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False, encoding='utf-8') as f:
                f.write(code)
                temp_file = f.name
            
            # Obtener la ruta del compilador
            compiler_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "main.exe")
            
            # Verificar que el compilador existe
            if not os.path.exists(compiler_path):
                return "", "❌ Error: No se encontró el compilador main.exe en la carpeta raíz del proyecto", 1
            
            # Ejecutar compilador
            result = subprocess.run(
                [compiler_path, temp_file],
                capture_output=True,
                text=True,
                timeout=30,
                encoding='utf-8',
                errors='replace'
            )
            
            # Limpiar archivo temporal
            os.unlink(temp_file)
            
            return result.stdout, result.stderr, result.returncode
        
        except subprocess.TimeoutExpired:
            return "", "❌ Error: El programa tardó demasiado en ejecutarse (timeout de 30s)", 1
        except Exception as e:
            return "", f"❌ Error al ejecutar el compilador: {str(e)}", 1

    # Tabs para diferentes fases de compilación
    tab1, tab2, tab3 = st.tabs(["🔍 Tokens", "🖨️ Print Visitor", "▶️ Eval Visitor"])
    
    with tab1:
        if st.session_state.last_scanner_output:
            st.code(st.session_state.last_scanner_output, language="text")
        else:
            st.info("👆 Ejecuta el código para ver el análisis de tokens")
    
    with tab2:
        if st.session_state.last_print_visitor_output:
            st.code(st.session_state.last_print_visitor_output.strip(), language="kotlin")
        else:
            st.info("👆 Ejecuta el código para ver el código reconstruido")
    
    with tab3:
        if st.session_state.last_eval_visitor_output:
            st.code(st.session_state.last_eval_visitor_output.strip(), language="text")
        else:
            st.info("👆 Ejecuta el código para ver la salida del programa")
    
    # Mostrar errores si los hay
    if st.session_state.last_error:
        st.error("🚨 Errores de Compilación:")
        st.code(st.session_state.last_error, language="text")

# Lógica de los botones
if run_btn:
    if not st.session_state.code_content.strip():
        st.error("❌ Por favor, escribe algún código antes de compilar")
    else:
        with st.spinner("⏳ Compilando y ejecutando..."):
            # Mostrar progreso
            progress_bar = st.progress(0)
            progress_bar.progress(25)
            time.sleep(0.1)
            
            stdout, stderr, returncode = run_compiler(st.session_state.code_content)
            progress_bar.progress(100)
            time.sleep(0.1)
            progress_bar.empty()
            
            if returncode == 0:
                # Parsear la salida del compilador
                sections = parse_compiler_output(stdout)
                
                # Guardar resultados en el estado de sesión
                st.session_state.last_scanner_output = sections['tokens']
                st.session_state.last_parser_output = ""  # Ya no se usa por separado
                st.session_state.last_print_visitor_output = sections['print_visitor']
                st.session_state.last_eval_visitor_output = sections['eval_visitor']
                st.session_state.last_error = ""
                
                st.success("✅ Compilación y ejecución exitosa")
                st.rerun()
            else:
                st.session_state.last_error = stderr if stderr else "Error desconocido durante la compilación"
                st.error("❌ Error en la compilación")
                st.rerun()

# Sidebar con ejemplos únicamente
with st.sidebar:
    st.header("🎯 Ejemplos de Código")
    
    st.markdown("Haz clic en cualquier ejemplo para cargarlo en el editor:")
    
    examples = {
        "🌟 Hola Mundo": """fun main(): Unit {
    println("¡Hola Mundo!")
    println("Bienvenido al compilador Kotlin")
}""",
        
        "🔢 Variables y Tipos": """fun main(): Unit {
    var entero: Int = 42
    val flotante: Float = 3.14159f
    var texto: String = "Kotlin"
    var booleano: Boolean = true
    
    println("Entero: " + entero)
    println("Flotante: " + flotante)
    println("Texto: " + texto)
    println("Booleano: " + booleano)
}""",
        
        "➕ Operaciones Aritméticas": """fun main(): Unit {
    var a: Int = 10
    var b: Int = 3
    
    println("Suma: " + (a + b))
    println("Resta: " + (a - b))
    println("Multiplicación: " + (a * b))
    println("División: " + (a / b))
    println("Módulo: " + (a % b))
    
    a += 5
    println("Después de += 5: " + a)
}""",
        
        "🔄 Bucles y Rangos": """fun main(): Unit {
    println("Bucle for con rango:")
    for (i in 1..5) {
        println("Número: " + i)
    }
    
    println("Bucle while:")
    var contador: Int = 0
    while (contador < 3) {
        println("Contador: " + contador)
        contador++
    }
    
    println("Rango con step:")
    for (j in 2..10 step 2) {
        println("Par: " + j)
    }
}""",
        
        "🎛️ Condicionales": """fun main(): Unit {
    var edad: Int = 18
    var nombre: String = "Juan"
    
    if (edad >= 18) {
        println(nombre + " es mayor de edad")
    } else {
        println(nombre + " es menor de edad")
    }
    
    if (edad >= 21 && nombre == "Juan") {
        println("Juan puede votar")
    }

    var resultado: String

    if (edad > 65) {
        resultado = "Senior"
    } else {
        resultado = "Joven"
    }

    println("Categoría: " + resultado)
}""",
        
        "🔧 Funciones": """fun sumar(a: Int, b: Int): Int {
    return a + b
}

fun saludar(nombre: String): String {
    return "Hola, " + nombre + "!"
}

fun esPar(numero: Int): Boolean {
    return numero % 2 == 0
}

fun main(): Unit {
    var resultado: Int = sumar(5, 3)
    println("5 + 3 = " + resultado)
    
    var saludo: String = saludar("María")
    println(saludo)
    
    if (esPar(4)) {
        println("4 es par")
    }
}""",
        
        "🏃 Bloques Run": """fun main(): Unit {
    var x: Int = 5
    
    println("Antes del bloque run: " + x)
    
    run {
        var y: Int = 10
        x = x + y
        println("Dentro del bloque run: " + x)
        println("Variable local y: " + y)
    }
    
    println("Después del bloque run: " + x)
    
    var result : Int = run {
        var temp : Int = x * 2
        temp = temp * 5
        temp
    }

    println("Resultado del bloque run: " + result)
}""",
        
        "🧮 Ejemplo Complejo": """fun factorial(n: Int): Int {
    if (n <= 1) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fun fibonacci(n: Int): Int {
    if (n <= 1) {
        return n
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2)
    }
}

fun main(): Unit {
    println("=== CALCULADORA MATEMÁTICA ===")
    
    var numero: Int = 5
    var fact: Int = factorial(numero)
    println("Factorial de " + numero + " = " + fact)
    
    println("Serie Fibonacci:")
    for (i in 0..7) {
        var fib: Int = fibonacci(i)
        println("F(" + i + ") = " + fib)
    }
    
    println("Números primos hasta 20:")
    for (num in 2..20) {
        var esPrimo: Boolean = true
        for (div in 2..(num-1)) {
            if (num % div == 0) {
                esPrimo = false
            }
        }
        if (esPrimo) {
            println(num + " es primo")
        }
    }
}"""
    }
    
    for name, code in examples.items():
        if st.button(name, key=f"example_{name}", use_container_width=True):
            st.session_state.code_content = code
            st.session_state.editor_key += 1  # Forzar recarga del editor
            st.rerun()
    
    st.markdown("---")

# Footer
st.markdown("---")
st.markdown("""
<div style="text-align: center; color: #666; font-size: 0.9em;">
    🎓 Proyecto de Compiladores - Universidad de Ingeniería y Tecnología (UTEC)
</div>
""", unsafe_allow_html=True)