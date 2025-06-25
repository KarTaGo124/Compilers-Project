@echo off
echo 🚀 Configurando el frontend con Streamlit...

REM Verificar si Python está instalado
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Error: Python no está instalado o no está en el PATH
    echo 💡 Por favor instala Python desde https://python.org
    pause
    exit /b 1
)

echo ✅ Python encontrado

REM Crear entorno virtual
echo 📦 Creando entorno virtual...
python -m venv venv

REM Activar entorno virtual
echo 🔧 Activando entorno virtual...
call venv\Scripts\activate.bat

REM Actualizar pip
echo 📈 Actualizando pip...
python -m pip install --upgrade pip

REM Instalar dependencias
echo 📚 Instalando dependencias...
pip install -r requirements.txt

echo.
echo ✅ Frontend configurado exitosamente!
echo.
echo 🎯 Para ejecutar la aplicación:
echo    1. cd frontend
echo    2. venv\Scripts\activate
echo    3. streamlit run app.py
echo.
echo 🌐 La aplicación se abrirá en http://localhost:8501
echo.
pause
