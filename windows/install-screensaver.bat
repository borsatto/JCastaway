@echo off
setlocal EnableDelayedExpansion
title Instalador do Johnny Castaway Screensaver

:: 1. Verifica privilegios de Administrador (necessario para copiar em C:\Windows\System32)
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [*] Solicitando permissao de Administrador para instalar em C:\Windows\System32...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Start-Process cmd -ArgumentList '/c """%~f0"""' -Verb RunAs"
    exit /b
)

echo ==========================================================
echo        Johnny Castaway Screensaver Installer
echo ==========================================================
echo.

set "SCRIPT_DIR=%~dp0"
set "REPO_DIR=%SCRIPT_DIR%.."
set "SCR_SRC=%REPO_DIR%\JohnnyCastaway.scr"
set "SYS32_DEST=%SystemRoot%\System32\JohnnyCastaway.scr"
set "APP_DATA=%APPDATA%\JohnnyCastaway"

if not exist "%SCR_SRC%" (
    echo [ERRO] Nao foi possivel encontrar %SCR_SRC%
    pause
    exit /b 1
)

:: 2. Encerra qualquer instancia em execucao (ex: preview do Windows) para liberar o arquivo
echo [*] Fechando instancias ativas do JohnnyCastaway...
taskkill /F /IM JohnnyCastaway.scr >nul 2>&1
timeout /t 1 /nobreak >nul

:: 3. Copia o .scr para System32 (onde o desk.cpl do Windows descobre os screensavers)
echo [*] Instalando JohnnyCastaway.scr em:
echo     %SYS32_DEST%
copy /Y "%SCR_SRC%" "%SYS32_DEST%" >nul
if errorlevel 1 (
    echo [!] Falha ao copiar para System32. Certifique-se de que a janela de Protecao de Tela do Windows esta fechada.
    pause
    exit /b 1
)
echo [OK] Executavel instalado em System32!

:: 4. Copia os recursos para %APPDATA%\JohnnyCastaway
echo.
if not exist "%APP_DATA%" mkdir "%APP_DATA%"

if exist "%REPO_DIR%\original\RESOURCE.MAP" (
    echo [*] Copiando RESOURCE.MAP e RESOURCE.001 para %APP_DATA%...
    copy /Y "%REPO_DIR%\original\RESOURCE.MAP" "%APP_DATA%\" >nul
    copy /Y "%REPO_DIR%\original\RESOURCE.001" "%APP_DATA%\" >nul
    echo [OK] Recursos instalados com sucesso!
) else if exist "%APP_DATA%\RESOURCE.MAP" (
    echo [OK] Recursos ja configurados em %APP_DATA%!
) else (
    echo [AVISO] RESOURCE.MAP nao encontrado em %REPO_DIR%\original.
    echo Por favor, coloque RESOURCE.MAP e RESOURCE.001 em:
    echo %APP_DATA%
)

:: 4. Define no Registro do Windows o screensaver ativo
echo.
echo [*] Definindo Johnny Castaway como protecao de tela ativa...
reg add "HKCU\Control Panel\Desktop" /v SCRNSAVE.EXE /t REG_SZ /d "%SYS32_DEST%" /f >nul
reg add "HKCU\Control Panel\Desktop" /v ScreenSaveActive /t REG_SZ /d "1" /f >nul

echo.
echo ==========================================================
echo [SUCESSO] Instalacao concluida com exito!
echo Abrindo Configuracoes de Protecao de Tela do Windows...
echo ==========================================================
control desk.cpl,,@screensaver
exit /b 0
