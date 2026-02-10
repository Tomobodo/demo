set(CMAKE_SYSTEM_NAME WASM)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
set(CMAKE_CXX_COMPILER_TARGET wasm32-unknown-unknown)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_LINKER "wasm-ld")

# bypass compiler tests since they try to compile with c runtime wich we don't provide
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)