#!/bin/sh

if cmake --preset Debug_wasm; then
  node platforms/wasm/hot_reload_server/watcher.js
else
  echo "Could not configure cmake. The watcher wasn't started."
  exit 1
fi