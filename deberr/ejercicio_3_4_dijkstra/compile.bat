@echo off
echo Compilando Dijkstra en C y C++...
gcc -Wall -Wextra -O2 dijkstra.c -o dijkstra_c.exe
g++ -Wall -Wextra -O2 dijkstra.cpp -o dijkstra_cpp.exe
if %ERRORLEVEL% NEQ 0 (
    echo Error durante la compilacion.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilacion exitosa. Ejecutando dijkstra_c.exe...
echo.
dijkstra_c.exe
echo.
pause
