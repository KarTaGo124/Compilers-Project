@echo off
echo 🚀 Iniciando Kotlin Compiler IDE...

REM Verificar si el entorno virtual existe
if not exist "venv\Scripts\activate.bat" (
    echo ❌ Error: Entorno virtual no encontrado
    echo 💡 Ejecuta setup.bat primero para configurar el entorno
    pause
    exit /b 1
)

REM Activar entorno virtual
echo 🔧 Activando entorno virtual...
call venv\Scripts\activate.bat

REM Verificar si Streamlit está instalado
python -c "import streamlit" >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Error: Streamlit no está instalado
    echo 💡 Ejecuta setup.bat primero para instalar las dependencias
    pause
    exit /b 1
)

REM Ejecutar la aplicación
echo ✅ Iniciando aplicación...
echo 🌐 Abriendo en http://localhost:8501
echo.
echo 💡 Presiona Ctrl+C para detener la aplicación
echo.

streamlit run app.py
