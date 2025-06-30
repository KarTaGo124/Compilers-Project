import subprocess
import os
import sys

def compile_cpp():
    """Compila solo los archivos CPP del proyecto"""
    try:
        project_dir = os.path.dirname(os.path.abspath(__file__))
        
        print("Compilando archivos C++...")
        
        # Compilar con g++
        compile_command = [
            'g++', 
            '-o', 'main.exe',
            'main.cpp',
            'scanner.cpp', 
            'parser.cpp',
            'token.cpp',
            'exp.cpp',
            'visitor.cpp'
        ]
        
        result = subprocess.run(
            compile_command,
            cwd=project_dir,
            capture_output=True,
            text=True
        )
        
        if result.returncode == 0:
            print("✅ Compilación exitosa: main.exe generado")
        else:
            print("❌ Error de compilación:")
            print(result.stderr)
            return False
            
        return True
        
    except Exception as e:
        print(f"❌ Error: {str(e)}")
        return False

if __name__ == "__main__":
    compile_cpp()
