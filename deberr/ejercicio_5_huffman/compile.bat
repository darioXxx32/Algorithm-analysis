@echo off
echo Compilando Huffman en C y C++...
gcc -Wall -Wextra -O2 huffman.c -o huffman_c.exe
g++ -Wall -Wextra -O2 huffman.cpp -o huffman_cpp.exe
if %ERRORLEVEL% NEQ 0 (
    echo Error durante la compilacion.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilacion exitosa. Ejecutando huffman_c.exe...
echo.
huffman_c.exe
echo.
pause
