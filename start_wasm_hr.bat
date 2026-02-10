@echo off

cmake --preset Debug_wasm
if %errorlevel% equ 0 (
    node platforms/wasm/hot_reload_server/watcher.js
) else (
    echo "Could not configure cmake. The watcher wasn't started."
    exit /b 1
)