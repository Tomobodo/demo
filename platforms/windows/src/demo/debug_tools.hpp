#pragma once

#include <windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void init_debug_tool(HINSTANCE hinstance, float demo_duration, float* time);

void update_debug_tool(float time);
