@echo off
REM ============================================================
REM  HuffZip - moteur C : compile, compresse, decompresse,
REM  et verifie que l'original est reconstruit a l'identique.
REM
REM  Usage :
REM    - double-clique sur ce fichier, puis indique un .txt, ou
REM    - glisse-depose un fichier .txt directement sur ce .bat.
REM ============================================================
setlocal enableextensions enabledelayedexpansion
chcp 65001 >nul
title HuffZip - demonstration Huffman (C)

set "ROOT=%~dp0"
set "BIN=%ROOT%bin"
set "WORK=%ROOT%work"

echo ============================================================
echo   HuffZip  -  compression / decompression Huffman ^(moteur C^)
echo ============================================================
echo.

REM --- verifie la presence de gcc ---
where gcc >nul 2>nul
if errorlevel 1 (
  echo [ERREUR] gcc est introuvable dans le PATH.
  echo Installe MinGW-w64 ou w64devkit, puis relance ce script :
  echo   https://www.mingw-w64.org/
  echo.
  pause
  exit /b 1
)

REM --- compile les deux programmes ---
if not exist "%BIN%" mkdir "%BIN%"
echo Compilation du moteur...
gcc -O2 -o "%BIN%\compresser.exe" "%ROOT%compression\FonctionsArbre.c" "%ROOT%compression\FonctionsListe.c" "%ROOT%compression\FonctionsCompression.c" "%ROOT%compression\main.c"
if errorlevel 1 goto :builderr
gcc -O2 -o "%BIN%\decompresser.exe" "%ROOT%decompression\Decompression.c"
if errorlevel 1 goto :builderr
echo   OK : bin\compresser.exe et bin\decompresser.exe
echo.

REM --- choisit le fichier a traiter ---
set "INPUT=%~1"
if "%INPUT%"=="" set /p "INPUT=Chemin du fichier .txt (ou glisse-le ici) puis Entree : "
set "INPUT=%INPUT:"=%"
if not exist "%INPUT%" (
  echo [ERREUR] Fichier introuvable : %INPUT%
  echo.
  pause
  exit /b 1
)

REM --- prepare le dossier de travail ---
if not exist "%WORK%" mkdir "%WORK%"
copy /y "%INPUT%" "%WORK%\Fichier a compresser.txt" >nul

pushd "%WORK%"
echo.
echo Compression...
"%BIN%\compresser.exe"
echo Decompression...
"%BIN%\decompresser.exe"
popd

REM --- verifie l'aller-retour ---
echo.
echo ------------------------------------------------------------
fc /b "%WORK%\Fichier a compresser.txt" "%WORK%\Fichier Decompresse.txt" >nul
if errorlevel 1 (
  echo [ECHEC] Le fichier decompresse differe de l'original.
) else (
  echo [OK] Aller-retour sans perte : l'original est reconstruit a l'identique.
)

REM --- affiche les tailles ---
set "SZ_IN=?"
set "SZ_OUT=?"
for %%A in ("%WORK%\Fichier a compresser.txt") do set "SZ_IN=%%~zA"
for %%A in ("%WORK%\Fichier Compresse.txt")   do set "SZ_OUT=%%~zA"
echo.
echo   Taille originale  : !SZ_IN! octets
echo   Taille compressee : !SZ_OUT! octets
echo   Fichiers produits dans : %WORK%
echo ------------------------------------------------------------
echo.
pause
exit /b 0

:builderr
echo.
echo [ERREUR] La compilation a echoue.
echo.
pause
exit /b 1
