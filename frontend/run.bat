@echo off
echo ============================
echo   INICIANDO FRONTEND
echo ============================
echo.

echo Activando entorno virtual...
call venv\Scripts\activate.bat

echo Iniciando aplicacion...
python -m streamlit run app.py
