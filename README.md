# Demo template

![The demo running, just a basic bouncing ball](demo.gif)

Just a demo template to test some things out.  
It has hot reloading and a seek bar for smooth  
workflow and debugging.

all is left to do is some audio glue and an actual demo...

## TODO

- [x] hotreload in wasm32
- [x] hotreload in win32
- [ ] audio glue code with webaudio api

## Building

Use cmake, I made some presets.  
Make sure you have Crinkler and wasm-ld, wasm-strip, clang in your path.

### windows with hot reload

#### Build the demo

```shell
cmake --preset Debug_windows_hotreload

cmake --build --preset Debug_windows_hotreload
```

Then run the demo.

While the demo is runing, make some change in the src/demo/demo.cpp file

Then rebuild the library :

```shell
cmake --build --preset Debug_windows_hotreload --target demo_lib
```

The demo should detect changes in its working directory and reload the library

Then configure your editor of choice to rebuild anytime you save.
I use clion and the file watcher plugin to trigger the above cmake command everytime i save.

### wasm with hot reload

#### Build the demo

First, check if the cmake/toolchains/wasm.cmake match your system.  
Some paths may differ.

Then run :

```
cmake --preset Debug_wasm 

cmake --build build/wasm/Debug
```

The index.html file is generated in build/wasm/Debug/index.html

#### Enable hot reloading

Build the node server to notify index.html a new wasm build is available :

```shell
cd platforms/wasm/hot_reload_server
npm install
npm run dev
```

then launch some file watcher to watch the source files and trigger a build.  
I use CLion's file watcher plugin, you can also use watchexec :

```shell
watchexec -e cpp,hpp -- "cmake --build --preset Debug_wasm"    
```