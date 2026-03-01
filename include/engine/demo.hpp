#pragma once

extern "C" {
void demo_init();

void demo_deinit();

void demo_update(float time);

unsigned int* demo_get_buffer();

float demo_get_duration();
};
