@echo off
setlocal EnableDelayedExpansion
echo ==========================================================
echo        Johnny Castaway Screensaver Installer
echo ==========================================================
echo.

set "TARGET_DIR=%LOCALAPPDATA%\Microsoft\Windows\Screen Savers"
if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"

set "APP_DATA=%APPDATA%\JohnnyCastaway"
if not exist "%APP_DATA%" mkdir "%APP_DATA%"

echo [*] Instalando JohnnyCastaway.scr em:
echo     %TARGET_DIR%\JohnnyCastaway.scr
copy /Y "%~dp0..\JohnnyCastaway.scr" "%TARGET_DIR%\JohnnyCastaway.scr" >nul
if errorlevel 1 (
    echo [!] Erro ao copiar JohnnyCastaway.scr
    pause
    exit /b 1
)

echo.
echo [*] Verificando arquivos de recursos originais...
if exist "%~dp0..\original\RESOURCE.MAP" (
    echo [*] Copiando RESOURCE.MAP e RESOURCE.001 para %APP_DATA%...
    copy /Y "%~dp0..\original\RESOURCE.MAP" "%APP_DATA%\" >nul
    copy /Y "%~dp0..\original\RESOURCE.001" "%APP_DATA%\" >nul
    echo [OK] Recursos instalados com sucesso!
) else if exist "%APP_DATA%\RESOURCE.MAP" (
    echo [OK] Recursos ja presentes em %APP_DATA%!
) else (
    echo [AVISO] RESOURCE.MAP nao encontrado.
    echo Por favor, coloque RESOURCE.MAP e RESOURCE.001 na pasta:
    echo %APP_DATA%
)

echo.
echo ==========================================================
echo Instalacao concluida!
echo Abrindo Configuracoes de Protecao de Tela do Windows...
echo ==========================================================
control desk.cpl,,@screensaver
pause
