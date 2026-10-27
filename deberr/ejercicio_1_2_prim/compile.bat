@echo off
echo Compilando Prim en C y C++...
gcc -Wall -Wextra -O2 prim.c -o prim_c.exe
g++ -Wall -Wextra -O2 prim.cpp -o prim_cpp.exe
if %ERRORLEVEL% NEQ 0 (
    echo Error durante la compilacion.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilacion exitosa. Ejecutando prim_c.exe...
echo.
prim_c.exe
echo.
pause
