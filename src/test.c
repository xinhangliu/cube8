#include "driver.h"
#include <intrins.h>
#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

void test_cube() {
  cube(0);
  display_loop(pattern, 20);

  cube(1);
  display_loop(pattern, 20);
}

void test_dot() {
  uchar i;
  cube(0);
  display_loop(pattern, 20);

  for (i = 0; i < 8; i++) {
    dot(0, i, 0, 1);
    display_loop(pattern, 10);
  }

  for (i = 1; i < 8; i++) {
    dot(i, 7, 0, 1);
    display_loop(pattern, 10);
  }

  for (i = 6; i != 255; i--) {
    dot(7, i, 0, 1);
    display_loop(pattern, 10);
  }

  for (i = 6; i > 0; i--) {
    dot(i, 0, 0, 1);
    display_loop(pattern, 10);
  }
  cube(1);
  display_loop(pattern, 20);
}

void test_line() {
  cube(0);
  display_loop(pattern, 20);
  linex(1, 1, 0xff);
  display_loop(pattern, 20);

  liney(1, 1, 0xfd);
  display_loop(pattern, 20);

  linez(1, 1, 0xff);
  display_loop(pattern, 20);
}

void test_surface() {
  cube(0);
  display_loop(pattern, 20);
  surfacexz(0, 1);
  surfacexz(7, 1);
  display_loop(pattern, 20);

  surfaceyz(1, 1);
  surfaceyz(3, 1);
  surfaceyz(5, 1);
  surfaceyz(7, 1);
  display_loop(pattern, 20);

  surfacexy(3, 0);
  display_loop(pattern, 20);
}

void test_shift_x() {
  uchar i;
  cube(0);
  surfaceyz(0, 1);
  for (i = 1; i < 8; i++) {
    shift_x(1, 0);
    display_loop(pattern, 40);
  }

  for (i = 1; i < 8; i++) {
    shift_x(-1, 0);
    display_loop(pattern, 40);
  }
  for (i = 1; i < 8; i++) {
    shift_x(1, 1);
    display_loop(pattern, 40);
  }
  cube(0);
  surfaceyz(7, 1);
  for (i = 1; i < 8; i++) {
    shift_x(-1, 1);
    display_loop(pattern, 40);
  }
}

void test_shift_y() {
  uchar i;
  cube(0);
  surfacexz(0, 1);
  for (i = 1; i < 8; i++) {
    shift_y(1, 0);
    display_loop(pattern, 40);
  }

  for (i = 1; i < 8; i++) {
    shift_y(-1, 0);
    display_loop(pattern, 40);
  }
  for (i = 1; i < 8; i++) {
    shift_y(1, 1);
    display_loop(pattern, 40);
  }
  cube(0);
  surfacexz(7, 1);
  for (i = 1; i < 8; i++) {
    shift_y(-1, 1);
    display_loop(pattern, 40);
  }
}

void test_shift_z() {
  uchar i;
  cube(0);
  surfacexy(0, 1);
  for (i = 1; i < 8; i++) {
    shift_z(1, 0);
    display_loop(pattern, 40);
  }

  for (i = 1; i < 8; i++) {
    shift_z(-1, 0);
    display_loop(pattern, 40);
  }
  for (i = 1; i < 8; i++) {
    shift_z(1, 1);
    display_loop(pattern, 40);
  }
  cube(0);
  surfacexy(7, 1);
  for (i = 1; i < 8; i++) {
    shift_z(-1, 1);
    display_loop(pattern, 40);
  }
}

void test_circshift_x() {
  uchar i;
  cube(0);
  linex(2, 2, 0xf0);
  liney(3, 3, 0xff);
  for (i = 0; i < 10; i++) {
    display_loop(pattern, 20);
    circshift_x(1);
  }
}

void test_circshift_y() {
  uchar i;
  cube(0);
  linex(2, 2, 0xff);
  liney(3, 3, 0xf0);
  for (i = 0; i < 8; i++) {
    display_loop(pattern, 20);
    circshift_y(1);
  }

  for (i = 0; i < 8; i++) {
    display_loop(pattern, 20);
    circshift_y(0);
  }
}

void test_circshift_z() {
  uchar i;
  cube(0);
  linex(2, 2, 0xff);
  liney(3, 3, 0xf0);
  linez(2, 3, 0xf0);
  for (i = 0; i < 8; i++) {
    display_loop(pattern, 20);
    circshift_z(1);
  }
  for (i = 0; i < 8; i++) {
    display_loop(pattern, 20);
    circshift_z(0);
  }
}

void test_rotate_x() {
  uchar i;
  cube(0);
  surfacexz(0, 1);
  linex(7, 3, 0xff);
  linez(3, 7, 0xff);
  liney(3, 0, 0xff);
  for (i = 0; i < 28; i++) {
    display_loop(pattern, 20);
    rotate_x(1);
  }
  for (i = 0; i < 28; i++) {
    display_loop(pattern, 20);
    rotate_x(0);
  }
}
