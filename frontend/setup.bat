@echo off
echo ============================
echo   SETUP COMPILADOR KOTLIN
echo ============================
echo.

echo [1/4] Creando entorno virtual...
python -m venv venv

echo [2/4] Activando entorno virtual...
call venv\Scripts\activate.bat

echo [3/4] Instalando dependencias Python...
pip install -r requirements.txt

echo [4/4] Instalando GCC en Ubuntu...
ubuntu run "sudo apt update && sudo apt install -y build-essential"

echo.
echo Setup completado!
echo Usa run.bat para iniciar el frontend.
pause
