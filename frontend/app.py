import streamlit as st
import subprocess
import tempfile
import os
import sys
import time
import re

def execute_assembly_simple(s_file_path):
    """Ejecuta assembly usando ubuntu directamente"""
    try:
        if not os.path.exists(s_file_path):
            return "❌ Error: No se encontró el archivo assembly (.s)", 1
        
        normalized_path = os.path.normpath(s_file_path)
        drive_letter = normalized_path[0].lower()
        path_without_drive = normalized_path[2:].replace('\\', '/')
        wsl_path = f"/mnt/{drive_letter}{path_without_drive}"
        
        base_name = os.path.splitext(os.path.basename(s_file_path))[0]
        wsl_dir = os.path.dirname(wsl_path)
        
        compile_cmd = f'cd "{wsl_dir}" && gcc -no-pie -m64 "{os.path.basename(wsl_path)}" -o "{base_name}_exec" 2>&1'
        compile_result = subprocess.run(['ubuntu', 'run', compile_cmd], 
                                       capture_output=True, text=True, timeout=30)
        
        if compile_result.returncode != 0:
            return f"❌ Error de compilación:\n{compile_result.stdout}", compile_result.returncode
        
        exec_cmd = f'cd "{wsl_dir}" && "./{base_name}_exec" 2>&1'
        exec_result = subprocess.run(['ubuntu', 'run', exec_cmd], 
                                    capture_output=True, text=True, timeout=30)
        
        output = exec_result.stdout if exec_result.stdout else "(sin salida)"
        
        subprocess.run(['ubuntu', 'run', f'rm -f {wsl_dir}/{base_name}_exec'], capture_output=True, timeout=5)
        
        return output, exec_result.returncode
        
    except subprocess.TimeoutExpired:
        return "❌ Timeout (30s)", 1
    except Exception as e:
        return f"❌ Error: {str(e)}", 1

try:
    from streamlit_ace import st_ace
    ACE_AVAILABLE = True
except ImportError:
    ACE_AVAILABLE = False

st.set_page_config(
    page_title="Kotlin Compiler IDE",
    page_icon="⚡",
    layout="wide",
    initial_sidebar_state="expanded"
)

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

st.markdown("""
<div class="main-header">
    <h1>⚡ Kotlin Compiler IDE</h1>
    <p style="color: white; text-align: center; margin: 0;">Compilador de Kotlin desarrollado en C++</p>
</div>
""", unsafe_allow_html=True)

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
    - ✅ **Generación de Código**: Código assembly x86-64
    
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
    - **CodeGen**: Generador de código assembly x86-64
    
    **📚 Proyecto desarrollado en:**
    - Universidad de Ingeniería y Tecnología (UTEC)
    - Curso: Compiladores
    - Año: 2025-1
    """)
    
    if st.button("✅ Cerrar", use_container_width=True):
        st.rerun()

@st.dialog("❓ Guía de Sintaxis Kotlin")
def show_syntax_guide():
    st.markdown("""
    ### 📖 **Guía de Sintaxis Kotlin**
    
    Esta guía te ayudará a escribir código Kotlin compatible con nuestro compilador.
    """)
    
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

def load_test_file(test_number):
    """Carga un archivo de test específico por número"""
    test_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "test")
    txt_file = os.path.join(test_dir, f"test{test_number}.txt")
    
    if os.path.exists(txt_file):
        try:
            with open(txt_file, 'r', encoding='utf-8') as f:
                return f.read()
        except Exception as e:
            return f"// Error al cargar el archivo test{test_number}.txt: {str(e)}"
    else:
        return f"// No se encontró el archivo test{test_number}.txt"

def read_assembly_file(file_path):
    """Lee el archivo .s generado por el compilador"""
    s_file = file_path.replace('.txt', '.s')
    if os.path.exists(s_file):
        try:
            with open(s_file, 'r', encoding='utf-8') as f:
                return f.read()
        except Exception as e:
            return f"// Error al leer el archivo assembly: {str(e)}"
    return ""

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

if 'last_gencode_output' not in st.session_state:
    st.session_state.last_gencode_output = ""

if 'last_error' not in st.session_state:
    st.session_state.last_error = ""

if 'last_assembly_execution' not in st.session_state:
    st.session_state.last_assembly_execution = ""

col1, col2 = st.columns([1, 1])

with col1:
    st.header("📝 Editor de Código")
    
    if ACE_AVAILABLE:
        st.markdown("**Editor Avanzado** - Usa Tab para indentar, Ctrl+Z para deshacer")
        
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
        
        code_input = st.text_area(
            "Escribe tu código Kotlin aquí:",
            height=400,
            value=st.session_state.code_content,
            key="code_editor_fallback",
            help="Editor básico con soporte para tabulación. Tab insertará 4 espacios.",
            placeholder="fun main(): Unit {\n    println(\"¡Hola Mundo!\")\n}"
        )
    
    if code_input != st.session_state.code_content:
        st.session_state.code_content = code_input
    
    col_btn1, col_btn2, col_btn3 = st.columns(3)
    
    with col_btn1:
        run_btn = st.button("▶️ Compilar", use_container_width=True, type="primary")
    
    with col_btn2:
        clear_btn = st.button("🗑️ Limpiar Editor", use_container_width=True)
    
    with col_btn3:
        download_btn = st.button("💾 Descargar Código", use_container_width=True)
    
    if clear_btn:
        st.session_state.code_content = """fun main(): Unit {
}"""
        st.session_state.editor_key += 1  
        st.session_state.last_scanner_output = ""
        st.session_state.last_parser_output = ""
        st.session_state.last_print_visitor_output = ""
        st.session_state.last_eval_visitor_output = ""
        st.session_state.last_gencode_output = ""
        st.session_state.last_assembly_execution = ""
        st.session_state.last_error = ""
        st.session_state.compilation_status = ""
        st.rerun()
    
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
    
    st.info(f"📄 Líneas: {len(st.session_state.code_content.splitlines())} | Caracteres: {len(st.session_state.code_content)}")

with col2:
    st.header("📊 Resultados de Compilación")
    
    def parse_compiler_output(output):
        sections = {
            'tokens': '',
            'print_visitor': '',
            'eval_visitor': '',
            'gencode': ''
        }
        
        lines = output.split('\n')
        current_section = None
        
        for line in lines:
            if 'Starting Scanner Test:' in line:
                current_section = 'tokens'
                sections['tokens'] += line + '\n'
                continue
            
            if any(phrase in line for phrase in ['Iniciando Visitor:', 'Iniciando parsing:', 'Scanner exitoso', 'Parsing exitoso']):
                continue
            
            if 'IMPRIMIR:' in line:
                current_section = 'print_visitor'
                continue
            
            if 'EJECUTAR:' in line:
                current_section = 'eval_visitor'
                continue
            
            if 'GENERAR CODIGO ASSEMBLY:' in line:
                current_section = 'gencode'
                continue
            
            if current_section == 'tokens' and line.strip():
                if line.startswith('TOKEN(') or 'TOKEN(END)' in line:
                    sections['tokens'] += line + '\n'
            
            elif current_section == 'print_visitor' and line.strip():
                if not any(x in line for x in ['EJECUTAR:', 'GENERAR CODIGO ASSEMBLY:']):
                    sections['print_visitor'] += line + '\n'
            
            elif current_section == 'eval_visitor' and line.strip():
                if 'GENERAR CODIGO ASSEMBLY:' not in line:
                    sections['eval_visitor'] += line + '\n'
            
            elif current_section == 'gencode' and line.strip():
                sections['gencode'] += line + '\n'
        
        return sections

    def execute_assembly(s_file_path):
        """Compila y ejecuta un archivo .s usando gcc a través de WSL"""
        try:
            if not os.path.exists(s_file_path):
                return "❌ Error: No se encontró el archivo assembly (.s)", 1
            
            try:
                wsl_check = subprocess.run(
                    ['wsl', '--version'],
                    capture_output=True,
                    text=True,
                    timeout=10
                )
                if wsl_check.returncode != 0:
                    return "❌ Error: WSL no está disponible o no está configurado correctamente. \nPor favor instala WSL2 y una distribución de Linux.", 1
            except (subprocess.TimeoutExpired, FileNotFoundError):
                return "❌ Error: WSL no está instalado. \nInstala WSL2 desde Microsoft Store o usando 'wsl --install' en PowerShell como administrador.", 1
            
            normalized_path = os.path.normpath(s_file_path)
            
            wsl_path = normalized_path.replace('\\', '/').replace(':', '').lower()
            wsl_path = f"/mnt/{wsl_path[0]}{wsl_path[1:]}"
            
            base_name = os.path.splitext(os.path.basename(s_file_path))[0]
            wsl_dir = os.path.dirname(wsl_path)
            wsl_executable = f"{wsl_dir}/{base_name}_exec"
            
            compile_command = [
                'wsl', 'bash', '-c',
                f'cd "{wsl_dir}" && gcc -no-pie -m64 "{os.path.basename(wsl_path)}" -o "{base_name}_exec" 2>&1'
            ]
            
            compile_result = subprocess.run(
                compile_command,
                capture_output=True,
                text=True,
                timeout=45,
                encoding='utf-8',
                errors='replace'
            )
            
            if compile_result.returncode != 0:
                error_msg = f"❌ Error de compilación con gcc (WSL):\n\n"
                error_msg += f"Comando ejecutado: gcc -no-pie -m64 {os.path.basename(wsl_path)} -o {base_name}_exec\n\n"
                error_msg += f"Salida de error:\n{compile_result.stdout}\n"
                if compile_result.stderr:
                    error_msg += f"Error stderr:\n{compile_result.stderr}\n"
                error_msg += "\n💡 Posibles soluciones:\n"
                error_msg += "   • Verifica que el archivo .s tenga sintaxis válida\n"
                error_msg += "   • Asegúrate de que WSL tenga gcc instalado: wsl sudo apt install build-essential\n"
                error_msg += "   • El archivo debe tener punto de entrada _start para ejecutables"
                return error_msg, compile_result.returncode
            
            exec_command = ['wsl', 'bash', '-c', f'cd "{wsl_dir}" && "./{base_name}_exec" 2>&1']
            
            exec_result = subprocess.run(
                exec_command,
                capture_output=True,
                text=True,
                timeout=45,
                encoding='utf-8',
                errors='replace'
            )
            
            output = f"🔧 Comando de compilación: gcc -no-pie -m64 {os.path.basename(wsl_path)} -o {base_name}_exec\n"
            output += f"📂 Directorio de trabajo: {wsl_dir}\n"
            output += f"✅ Compilación exitosa\n\n"
            output += f"🚀 Ejecutando: ./{base_name}_exec\n"
            output += f"📤 Salida del programa:\n"
            output += "="*50 + "\n"
            
            if exec_result.stdout:
                output += exec_result.stdout
            else:
                output += "(sin salida)\n"
            
            output += "\n" + "="*50 + "\n"
            
            if exec_result.stderr:
                output += f"⚠️ Errores/Advertencias:\n{exec_result.stderr}\n"
            
            if exec_result.returncode == 0:
                output += f"✅ Programa ejecutado exitosamente (código de salida: 0)"
            else:
                output += f"❌ Programa terminó con código de salida: {exec_result.returncode}"
                if exec_result.returncode == 139:
                    output += " (Segmentation fault)"
                elif exec_result.returncode == 1:
                    output += " (Error general)"
            
            try:
                subprocess.run(
                    ['wsl', 'rm', f'{wsl_dir}/{base_name}_exec'],
                    capture_output=True,
                    timeout=10
                )
            except:
                pass  
            
            return output, exec_result.returncode
            
        except subprocess.TimeoutExpired:
            return "❌ Error: El programa tardó demasiado en ejecutarse (timeout de 45s). \nPosibles causas:\n  • WSL está iniciando por primera vez\n  • El programa tiene un bucle infinito\n  • Problema de red o sistema lento", 1
        except Exception as e:
            return f"❌ Error inesperado al ejecutar assembly: {str(e)}\n\n💡 Consejos:\n  • Verifica que WSL esté instalado y funcionando\n  • Intenta ejecutar 'wsl --version' en cmd para verificar\n  • Reinicia WSL con: wsl --shutdown", 1
    def run_compiler(code):
        try:
            project_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            test_dir = os.path.join(project_dir, "test")
            
            if not os.path.exists(test_dir):
                os.makedirs(test_dir)
            
            editor_file = os.path.join(test_dir, "editor_code.txt")
            
            with open(editor_file, 'w', encoding='utf-8') as f:
                f.write(code)
            
            compiler_path = os.path.join(project_dir, "main.exe")
            
            if not os.path.exists(compiler_path):
                return "", "❌ Error: No se encontró el compilador main.exe en la carpeta raíz del proyecto", 1, ""
            
            result = subprocess.run(
                [compiler_path, editor_file],
                capture_output=True,
                text=True,
                timeout=30,
                encoding='utf-8',
                errors='replace'
            )
            
            assembly_code = read_assembly_file(editor_file)
            
            return result.stdout, result.stderr, result.returncode, assembly_code
        
        except subprocess.TimeoutExpired:
            return "", "❌ Error: El programa tardó demasiado en ejecutarse (timeout de 30s)", 1, ""
        except Exception as e:
            return "", f"❌ Error al ejecutar el compilador: {str(e)}", 1, ""

    tab1, tab2, tab3, tab4, tab5 = st.tabs(["🔍 Tokens", "🖨️ Print Visitor", "▶️ Eval Visitor", "⚙️ Código Assembly", "🚀 Ejecutar Assembly"])
    
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
    
    with tab4:
        if st.session_state.last_gencode_output:
            st.code(st.session_state.last_gencode_output.strip(), language="nasm")
        else:
            st.info("👆 Ejecuta el código para ver el código assembly generado")
    
    with tab5:
        if st.session_state.last_gencode_output:
            project_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            test_dir = os.path.join(project_dir, "test")
            editor_s_file = os.path.join(test_dir, "editor_code.s")
            
            if os.path.exists(editor_s_file):
                if st.button("🚀 Ejecutar Código Assembly del Editor", use_container_width=True, type="primary"):
                    with st.spinner("🔧 Compilando y ejecutando..."):
                        result, returncode = execute_assembly_simple(editor_s_file)
                        st.session_state.last_assembly_execution = result
                        st.rerun()
                
                if st.session_state.last_assembly_execution:
                    st.code(st.session_state.last_assembly_execution, language="text")
            else:
                st.warning("⚠️ No se ha generado aún un archivo assembly del editor. Compila tu código primero.")
        else:
            st.info("👆 Ejecuta el código para generar assembly y luego poder ejecutarlo")
    
    if st.session_state.last_error:
        st.error("🚨 Errores de Compilación:")
        st.code(st.session_state.last_error, language="text")

if run_btn:
    if not st.session_state.code_content.strip():
        st.error("❌ Por favor, escribe algún código antes de compilar")
    else:
        with st.spinner("⏳ Compilando y ejecutando..."):
            progress_bar = st.progress(0)
            progress_bar.progress(25)
            time.sleep(0.1)
            
            stdout, stderr, returncode, assembly_code = run_compiler(st.session_state.code_content)
            progress_bar.progress(100)
            time.sleep(0.1)
            progress_bar.empty()
            
            if returncode == 0:
                sections = parse_compiler_output(stdout)
                
                st.session_state.last_scanner_output = sections['tokens']
                st.session_state.last_parser_output = ""  
                st.session_state.last_print_visitor_output = sections['print_visitor']
                st.session_state.last_eval_visitor_output = sections['eval_visitor']
                
                if assembly_code.strip():
                    st.session_state.last_gencode_output = assembly_code
                else:
                    st.session_state.last_gencode_output = sections['gencode']
                
                st.session_state.last_error = ""
                
                st.success("✅ Compilación y ejecución exitosa")
                st.rerun()
            else:
                st.session_state.last_error = stderr if stderr else "Error desconocido durante la compilación"
                st.error("❌ Error en la compilación")
                st.rerun()

with st.sidebar:
    st.header("🧪 Tests del Compilador")
    
    st.markdown("Ingresa el número del test (1-25) para cargar:")
    
    test_number = st.number_input(
        "Número de test:",
        min_value=1,
        max_value=25,
        value=1,
        step=1,
        key="test_number_input"
    )
    
    if st.button(f"📄 Cargar Test {test_number}", use_container_width=True, type="primary"):
        test_content = load_test_file(test_number)
        if not test_content.startswith("//"):  
            st.session_state.code_content = test_content
            st.session_state.editor_key += 1  
            st.success(f"✅ Test {test_number} cargado exitosamente")
            st.rerun()
        else:
            st.error(f"❌ {test_content}")
        test_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "test")
        s_file = os.path.join(test_dir, f"test{test_number}.s")
        
        if os.path.exists(s_file):
            with st.spinner(f"🔧 Compilando y ejecutando test{test_number}.s con gcc..."):
                def execute_assembly_local(s_file_path):
                    try:
                        if not os.path.exists(s_file_path):
                            return "❌ Error: No se encontró el archivo assembly (.s)", 1
                        
                        try:
                            wsl_check = subprocess.run(['wsl', '--version'], capture_output=True, timeout=5)
                            if wsl_check.returncode != 0:
                                return "❌ Error: WSL no disponible", 1
                        except:
                            return "❌ Error: WSL no instalado", 1
                        
                        normalized_path = os.path.normpath(s_file_path)
                        wsl_path = normalized_path.replace('\\', '/').replace(':', '').lower()
                        wsl_path = f"/mnt/{wsl_path[0]}{wsl_path[1:]}"
                        
                        base_name = os.path.splitext(os.path.basename(s_file_path))[0]
                        wsl_dir = os.path.dirname(wsl_path)
                        
                        compile_command = [
                            'wsl', 'bash', '-c',
                            f'cd "{wsl_dir}" && gcc -no-pie -m64 "{os.path.basename(wsl_path)}" -o "{base_name}_test" 2>&1'
                        ]
                        
                        compile_result = subprocess.run(
                            compile_command,
                            capture_output=True,
                            text=True,
                            timeout=30,
                            encoding='utf-8',
                            errors='replace'
                        )
                        
                        if compile_result.returncode != 0:
                            return f"❌ Error de compilación:\n{compile_result.stdout}", compile_result.returncode
                        
                        exec_command = ['wsl', 'bash', '-c', f'cd "{wsl_dir}" && "./{base_name}_test" 2>&1']
                        exec_result = subprocess.run(
                            exec_command,
                            capture_output=True,
                            text=True,
                            timeout=30,
                            encoding='utf-8',
                            errors='replace'
                        )
                        
                        output = f"🔧 Compilación: gcc -no-pie -m64 {os.path.basename(wsl_path)} -o {base_name}_test\n"
                        output += f"✅ Compilación exitosa\n\n"
                        output += f"🚀 Ejecutando test {test_number}\n"
                        output += "="*40 + "\n"
                        
                        if exec_result.stdout:
                            output += exec_result.stdout
                        else:
                            output += "(sin salida)\n"
                        
                        output += "\n" + "="*40 + "\n"
                        
                        if exec_result.returncode == 0:
                            output += "✅ Ejecución exitosa"
                        else:
                            output += f"❌ Código de salida: {exec_result.returncode}"
                        
                        try:
                            subprocess.run(['wsl', 'rm', f'{wsl_dir}/{base_name}_test'], capture_output=True, timeout=5)
                        except:
                            pass
                        
                        return output, exec_result.returncode
                        
                    except subprocess.TimeoutExpired:
                        return "❌ Timeout (30s) - Programa muy lento o bucle infinito", 1
                    except Exception as e:
                        return f"❌ Error: {str(e)}", 1
                
                result, returncode = execute_assembly_local(s_file)
                
                with st.expander(f"Resultado de Ejecución del Test {test_number}", expanded=True):
                    st.code(result, language="text")
        else:
            st.error(f"❌ No se encontró el archivo test{test_number}.s")
    
    st.markdown("---")
    
    st.markdown("### 🌟 Ejemplo básico")
    if st.button("📄 Cargar Hola Mundo", use_container_width=True):
        st.session_state.code_content = """fun main(): Unit {
    println("¡Hola Mundo!")
    println("Bienvenido al compilador Kotlin")
}"""
        st.session_state.editor_key += 1  
        st.rerun()
    
    st.markdown("---")
    
    st.markdown("### 📚 Información")
    
    info_btn = st.button("ℹ️ Información del Compilador", use_container_width=True)
    help_btn = st.button("❓ Guía de Sintaxis", use_container_width=True)

if info_btn:
    show_compiler_info()

if help_btn:
    show_syntax_guide()

st.markdown("---")
st.markdown("""
<div style="text-align: center; color: #666; font-size: 0.9em;">
    🎓 Proyecto de Compiladores - Universidad de Ingeniería y Tecnología (UTEC)
</div>
""", unsafe_allow_html=True)
