#!/bin/sh

if cmake --preset Debug_wasm32; then
  node tools/hot_reload_server/watcher.js
else
  echo "Could not configure cmake. The watcher wasn't started."
  exit 1
fi