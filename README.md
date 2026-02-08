# Demo template

Just a demo template to test some things out.  
It has hot reloading and a seek bar for smooth  
workflow and debugging.

all is left to do is some audio glue and an actual demo...

## TODO

- [x] hotreload in wasm32
- [ ] hotreload in win32
- [ ] audio glue code with webaudio api

## Building

Use cmake, I made some presets.  
Make sure you have Crinkler and wasm-ld, wasm-strip, clang in your path.

### wasm with hot reload

#### Build the demo

First, check if the cmake/toolchains/wasm32.cmake match your system.  
Some paths may differ.

Then run :

```
cmake --preset Debug_wasm32 

cmake --build build/wasm32/Debug
```

The index.html file is generated in build/wasm32/Debug/index.html

#### Enable hot reloading

Build the node server to notify index.html a new wasm build is available :

```shell
cd templates/wasm32/hot_reload_server
npm install
npm run dev
```

then launch some file watcher to watch the source files and trigger a build.  
I use CLion's file watcher plugin, you can also use watchexec :

```shell
watchexec -e cpp,hpp -- "cmake --build --preset Debug_wasm32"    
```