#!/usr/bin/env python3
import subprocess
import os
import glob
import sys
import re

def natural_sort_key(text):
    def convert(part):
        return int(part) if part.isdigit() else part.lower()
    return [convert(c) for c in re.split('([0-9]+)', text)]

def main():
    print("🔧 EJECUTOR DE ARCHIVOS ASSEMBLY (.s)")
    print("="*60)
    
    s_files = glob.glob("test/*.s")
    if not s_files:
        print("⚠️  No se encontraron archivos .s en test/")
        return
    
    s_files.sort(key=natural_sort_key)

    print(f"📋 Encontrados {len(s_files)} archivos .s")
    print()

    successful = 0
    failed = 0

    for s_file in s_files:
        base_name = os.path.basename(s_file).replace('.s', '')
        print(f"📁 {base_name}")
        print("-" * 40)

        try:
            compile_result = subprocess.run(['gcc', s_file], 
                                          capture_output=True, text=True, timeout=10)
            
            if compile_result.returncode != 0:
                print(f"❌ Error compilando: {compile_result.stderr.strip()}")
                failed += 1
                continue
            
            run_result = subprocess.run(['./a.out'], 
                                      capture_output=True, text=True, timeout=5)
            
            output = run_result.stdout.strip()
            
            if output:
                print(f"✅ Ejecución exitosa (código: {run_result.returncode})")
                for line in output.split('\n'):
                    print(f"📄 {line}")
                successful += 1
            else:
                if run_result.stderr.strip():
                    print(f"❌ Error (código {run_result.returncode}): {run_result.stderr.strip()}")
                    failed += 1
                else:
                    print(f"✅ Ejecutado sin salida (código: {run_result.returncode})")
                    successful += 1

        except subprocess.TimeoutExpired:
            print("❌ Timeout - programa colgado")
            failed += 1
        except Exception as e:
            print(f"❌ Error: {e}")
            failed += 1
        finally:
            if os.path.exists('a.out'):
                try:
                    os.remove('a.out')
                except:
                    pass
        
        print()  
    
    print("="*60)
    print("=== RESUMEN ===")
    print(f"📊 Total: {len(s_files)}")
    print(f"✅ Exitosos: {successful}")
    print(f"❌ Fallidos: {failed}")

    if successful == len(s_files):
        print("🎉 ¡Todos los archivos funcionan!")
    elif successful > 0:
        print(f"⚠️  {failed} archivo(s) tuvieron problemas")
    else:
        print("❌ Ningún archivo funcionó correctamente")

if __name__ == "__main__":
    main()