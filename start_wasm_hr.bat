@echo off

cmake --preset Debug_wasm32
if %errorlevel% equ 0 (
    node tools/hot_reload_server/watcher.js
) else (
    echo "Could not configure cmake. The watcher wasn't started."
    exit /b 1
)