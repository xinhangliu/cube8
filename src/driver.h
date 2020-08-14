#ifndef _DRIVER_H_
#define _DRIVER_H_
#define uchar unsigned char
#define uint unsigned int
#define BUFF P0   // Pattern buff
#define LAYERS P1 // Control layers
#define PAGES P2  // Control pages

extern uchar pattern[64];

extern void delay_us(uint time);
extern void display(uchar *pattern);
extern void display_loop(uchar *pattern, uchar num);
extern void cube(bit status);
extern void box(uchar x0, uchar y0, uchar z0, uchar x1, uchar y1, uchar z1,
                bit status);
extern void dot(uchar x, uchar y, uchar z, bit status);
extern void linex(uchar y, uchar z, uchar line);
extern void liney(uchar x, uchar z, uchar line);
extern void linez(uchar x, uchar y, uchar line);
extern void surfacexy(uchar z, bit status);
extern void surfaceyz(uchar x, bit status);
extern void surfacexz(uchar y, bit status);
extern void shift_x(char step, bit patch);
extern void shift_y(char step, bit patch);
extern void shift_z(char step, bit patch);
extern void circshift_x(char step);
extern void circshift_y(bit direction);
extern void circshift_z(bit direction);
extern void rotate_x(bit direction);
extern void frame(uchar x0, uchar y0, uchar z0, uchar x1, uchar y1, uchar z1);
#endif
