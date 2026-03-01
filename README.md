# Demo template

![The demo running, just a basic bouncing ball](docs/demo.gif)

Just a demo template to test some things out.  
It has hot reloading and a seek bar for smooth  
workflow and debugging.

all is left to do is some audio glue and an actual demo...

## Features

- hotreload for wasm32
- hotreload for windows

### Todo

- cross platform audio glue code with webaudio api

## Building

Use cmake, I made some presets.  
Make sure you have Crinkler and wasm-ld, wasm-strip, clang in your path.

### windows

#### Building the demo

Firstly, make sure you have Crinkler in your path.

Then, open the x86 Native Tools Command Prompt distributed with visual studio and cd  
into the project root.  
Then :

```shell
cmake --preset Release_windows_crinkler

cmake --build --preset Release_windows_crinkler --target demo
```

The demo should build and be runnable.

#### Building with hotreload

Build using this preset :

```shell
cmake --preset Debug_windows_hotreload

cmake --build --preset Debug_windows_hotreload --target demo demo_lib
```

Then run the demo.

While the demo is running, make some change in the src/demo directory

Then rebuild the library :

```shell
cmake --build --preset Debug_windows_hotreload --target demo_lib
```

The demo executable will detect a new library is available and load it.

It's better to use an IDE with the demo_lib target selected,  
that way you can just press the build shortcut and see the result of your changes.  
I use clion so I just have to press F9 to trigger a build.

You also can configure your editor of choice to rebuild anytime you save.
In clion there is the file watcher plugin to trigger the above cmake command everytime you save.

You can also use watchexec

```shell
watchexec -e cpp,hpp -- "cmake --build --preset Debug_window_hotreload"  
```

### wasm with hot reload

#### Build the demo

Firstly, make sure you have clang++ and wasm-ld installed and in your path.  
Some paths may differ.

Then run :

```
cmake --preset Debug_wasm32 

cmake --build --preset Debug_wasm32 --target demo
```

The index.html file is generated in build/wasm32/Debug/dist/index.html

#### Enable hot reloading

First, install the dependencies :

```shell
cd tools/hot_reload_server
npm install
```

Then go back in the project's root andrun the bat or sh script depending on your os :

linux / mac :

```shell
./start_wasm_hr.sh
```

windows:

```shell
./start_wasm_hr.bat
```