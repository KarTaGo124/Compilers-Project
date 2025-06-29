import subprocess
import os
import glob

# Directorios
test_dir = "test"

# Archivos fuente del compilador (los originales de tu proyecto)
source_files = [
    "main.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp"
]

def compile_project():
    """Compila el proyecto del compilador"""
    print("=== COMPILANDO KOTLIN COMPILER ===")
    
    # Determinar nombre del ejecutable según el OS
    if os.name == 'nt':  # Windows
        executable = "main.exe"
    else:  # Linux/Unix
        executable = "main"
    
    compile_cmd = ["g++", "-o", executable] + source_files
    result = subprocess.run(compile_cmd)
    
    if result.returncode != 0:
        print("❌ Error de compilación.")
        return False
    
    print(f"✅ Compilación exitosa. Ejecutable: {executable}\n")
    return True

def run_test_file(test_file):
    """Ejecuta un archivo de prueba individual y verifica generación de .s"""
    print(f"\n📁 Ejecutando {os.path.basename(test_file)}")
    print("=" * 60)
    
    # Determinar nombre del compilador según el OS
    if os.name == 'nt':  # Windows
        compiler = "main.exe"
    else:  # Linux/Unix
        compiler = "./main"
    
    # Ejecutar el compilador con el archivo de prueba
    result = subprocess.run([compiler, test_file])
    
    if result.returncode == 0:
        # Verificar si se generó el archivo .s
        base_name = os.path.splitext(test_file)[0]
        s_file = base_name + ".s"
        
        if os.path.exists(s_file):
            print(f"✅ {os.path.basename(test_file)} ejecutado exitosamente")
            print(f"📄 Archivo generado: {os.path.basename(s_file)}")
            return True
        else:
            print(f"⚠️  {os.path.basename(test_file)} ejecutado, pero no se generó archivo .s")
            return False
    else:
        print(f"❌ Error en {os.path.basename(test_file)}")
        return False

def run_all_tests():
    """Ejecuta todos los archivos de prueba en la carpeta test"""
    print("=== EJECUTANDO TODAS LAS PRUEBAS ===")
    
    # Verificar que la carpeta test existe
    if not os.path.exists(test_dir):
        print(f"❌ La carpeta '{test_dir}' no existe.")
        return
    
    # Obtener todos los archivos .txt en la carpeta test
    test_files = glob.glob(os.path.join(test_dir, "*.txt"))
    
    if not test_files:
        print(f"⚠️  No se encontraron archivos .txt en la carpeta '{test_dir}'.")
        return
    
    # Ordenar los archivos para ejecución consistente
    test_files.sort()
    
    print(f"📋 Se encontraron {len(test_files)} archivos de prueba:")
    for test_file in test_files:
        print(f"   • {os.path.basename(test_file)}")
    print()
    
    # Contadores de resultados
    successful_tests = 0
    failed_tests = 0
    generated_s_files = []
    
    # Ejecutar cada archivo de prueba
    for test_file in test_files:
        success = run_test_file(test_file)
        if success:
            successful_tests += 1
            # Agregar archivo .s generado a la lista
            s_file = os.path.splitext(test_file)[0] + ".s"
            if os.path.exists(s_file):
                generated_s_files.append(s_file)
        else:
            failed_tests += 1
    
    # Resumen final
    print("\n" + "=" * 60)
    print("=== RESUMEN DE PRUEBAS ===")
    print(f"📊 Total de pruebas: {len(test_files)}")
    print(f"✅ Exitosas: {successful_tests}")
    print(f"❌ Fallidas: {failed_tests}")
    print(f"📄 Archivos .s generados: {len(generated_s_files)}")
    
    if generated_s_files:
        print("\nArchivos de ensamblador generados:")
        for s_file in generated_s_files:
            print(f"   • {os.path.basename(s_file)}")
    
    if failed_tests == 0:
        print("\n🎉 ¡Todas las pruebas pasaron exitosamente!")
        if len(generated_s_files) == len(test_files):
            print("🔧 ¡Todos los archivos .s fueron generados correctamente!")
        else:
            print("⚠️  Algunos archivos .s no se generaron.")
    else:
        print(f"\n⚠️  {failed_tests} prueba(s) fallaron.")

def main():
    """Función principal"""
    print("🚀 KOTLIN COMPILER - SISTEMA DE PRUEBAS CON GENERACIÓN .S")
    print("=" * 60)
    
    # Compilar el proyecto
    if not compile_project():
        print("\n❌ No se puede continuar sin una compilación exitosa.")
        return
    
    # Ejecutar todas las pruebas
    run_all_tests()
    
    print("\n🏁 Proceso completado.")

if __name__ == "__main__":
    main()
