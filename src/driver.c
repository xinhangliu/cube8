/* LED Structure ******************************************
                 PAGE0                  PAGE1
  LAYER0 -> o o o o o o o o        o o o o o o o o
  LAYER1 -> o o o o o o o o        o o o o o o o o
  LAYER2 -> o o o o o o o o        o o o o o o o o
  LAYER3 -> o o o o o o o o        o o o o o o o o
  LAYER4 -> o o o o o o o o        o o o o o o o o    ...
  LAYER5 -> o o o o o o o o        o o o o o o o o
  LAYER6 -> o o o o o o o o        o o o o o o o o
  LAYER7 -> o o o o o o o o        o o o o o o o o
            |_____________|        |_____________|
            0     BUFF    7        0     BUFF    7
**********************************************************/

#include <intrins.h>
#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
#define BUFF P0   // Pattern buff
#define LAYERS P1 // Control layers
#define PAGES P2  // Control pages

uchar pattern[64];

// 12MHz
void delay_us(uint time) {
  time /= 2;
  while (time-- > 0)
    ;
}

/* Basic display function *********************************
Display the pattern which is defined in an <array 1x64>
**********************************************************/
void display(uchar *pattern) {
  uchar i, j;
  LAYERS = 0;
  for (i = 0; i < 8; i++) {
    PAGES = 0xff;
    BUFF = 0;
    PAGES = 0x00;
    LAYERS = 0x01 << i;
    for (j = 0; j < 8; j++) {
      BUFF = *(pattern + i * 8 + j);
      PAGES = 0x01 << j;
      PAGES = 0;
    }
    // Pay attention to this delay,
    // too long will cause stroboscopic,
    // too short will result in uneven brightness.
    delay_us(200);
  }
}

void display_loop(uchar *pattern, uchar num) {
  if (num == 0)
    return;
  while (num--)
    display(pattern);
}

uchar bit_to_uchar(bit b) {
  if (b)
    return (0xff);
  else
    return (0);
}

void cube(bit status) {
  uchar i;
  for (i = 0; i < 64; i++)
    pattern[i] = bit_to_uchar(status);
}

void box(uchar x0, uchar y0, uchar z0, uchar x1, uchar y1, uchar z1,
         bit status) {
  uchar i, j, tmp = 0x00;

  for (i = x0; i <= x1; i++) {
    tmp = tmp | (0x01 << i);
  }

  for (i = y0; i <= y1; i++) {
    for (j = z0; j <= z1; j++) {
      if (status)
        pattern[i * 8 + j] = pattern[i * 8 + j] | tmp;
      else
        pattern[i * 8 + j] = pattern[i * 8 + j] & ~tmp;
    }
  }
}

void dot(uchar x, uchar y, uchar z, bit status) {
  uchar s;
  s = 0x01 << x;
  if (status)
    pattern[y * 8 + z] = pattern[y * 8 + z] | s;
  else
    pattern[y * 8 + z] = pattern[y * 8 + z] & ~s;
}

void linex(uchar y, uchar z, uchar line) {
  uchar i, s;
  for (i = 0; i < 8; i++) {
    s = (line >> i) & 0x01;
    if (s)
      pattern[y * 8 + z] = pattern[y * 8 + z] | _crol_(s, i);
    else {
      s = 0xfe;
      pattern[y * 8 + z] = pattern[y * 8 + z] & _crol_(s, i);
    }
  }
}

void liney(uchar x, uchar z, uchar line) {
  uchar i, s;
  for (i = 0; i < 8; i++) {
    s = (line >> i) & 0x01;
    if (s)
      pattern[i * 8 + z] = pattern[i * 8 + z] | _crol_(s, x);
    else {
      s = 0xfe;
      pattern[i * 8 + z] = pattern[i * 8 + z] & _crol_(s, x);
    }
  }
}

void linez(uchar x, uchar y, uchar line) {
  uchar i, s;
  for (i = 0; i < 8; i++) {
    s = (line >> i) & 0x01;
    if (s)
      pattern[y * 8 + i] = pattern[y * 8 + i] | _crol_(s, x);
    else {
      s = 0xfe;
      pattern[y * 8 + i] = pattern[y * 8 + i] & _crol_(s, x);
    }
  }
}

void surfacexy(uchar z, bit status) {
  uchar i;
  for (i = 0; i < 8; i++)
    pattern[i * 8 + z] = bit_to_uchar(status);
}

void surfaceyz(uchar x, bit status) {
  uchar i;
  for (i = 0; i < 8; i++)
    liney(x, i, bit_to_uchar(status));
}

void surfacexz(uchar y, bit status) {
  uchar i;
  for (i = 0; i < 8; i++)
    linex(y, i, bit_to_uchar(status));
}

void shift_x(char step, bit patch) {
  uchar i, p;
  for (i = 0; i < 64; i++) {
    if (step >= 0) {
      pattern[i] = pattern[i] << step;
      if (patch) {
        p = ~(0xfe << step);
        pattern[i] = pattern[i] | p;
      }
    } else if (step == 0)
      return;
    else {
      p = -step;
      pattern[i] = pattern[i] >> p;
      if (patch) {
        p = ~(0x7f >> p);
        pattern[i] = pattern[i] | p;
      }
    }
  }
}

void shift_y(char step, bit patch) {
  uchar i, j, p, index0, index1;
  if (step == 0)
    return;
  else if (step < 0)
    p = -step;
  else
    p = step;

  for (i = 0; i < (8 - p); i++) {
    if (step < 0) {
      index0 = i;
      index1 = i + p;
    } else {
      index0 = 7 - i;
      index1 = 7 - i - p;
    }
    for (j = 0; j < 8; j++) {
      pattern[index0 * 8 + j] = pattern[index1 * 8 + j];
    }
  }

  for (i = 0; i < p; i++) {
    if (step < 0)
      index0 = i;
    else
      index0 = 7 - i;
    for (j = 0; j < 8; j++) {
      pattern[(7 - index0) * 8 + j] = bit_to_uchar(patch);
    }
  }
}

void shift_z(char step, bit patch) {
  uchar i, j, p, index0, index1;
  if (step == 0)
    return;
  else if (step < 0)
    p = -step;
  else
    p = step;

  for (i = 0; i < (8 - p); i++) {
    if (step < 0) {
      index0 = i;
      index1 = i + p;
    } else {
      index0 = 7 - i;
      index1 = 7 - i - p;
    }
    for (j = 0; j < 8; j++) {
      pattern[j * 8 + index0] = pattern[j * 8 + index1];
    }
  }

  for (i = 0; i < p; i++) {
    if (step < 0)
      index0 = i;
    else
      index0 = 7 - i;
    for (j = 0; j < 8; j++) {
      pattern[j * 8 + (7 - index0)] = bit_to_uchar(patch);
    }
  }
}

void circshift_x(char step) {
  uchar i, p;
  if (step == 0)
    return;
  for (i = 0; i < 64; i++)
    if (step > 0) {
      p = step;
      pattern[i] = _crol_(pattern[i], p);
    } else {
      p = -step;
      pattern[i] = _cror_(pattern[i], p);
    }
}

void circshift_y(bit direction) {
  uchar i, j, index0, index1;
  uchar tmp[8];
  for (i = 0; i < 7; i++) {
    if (direction) {
      index0 = 7 - i;
      index1 = 6 - i;
    } else {
      index0 = i;
      index1 = i + 1;
    }
    for (j = 0; j < 8; j++) {
      if (i == 0)
        tmp[j] = pattern[index0 * 8 + j];
      pattern[index0 * 8 + j] = pattern[index1 * 8 + j];
    }
  }

  if (direction)
    index0 = 0;
  else
    index0 = 7;
  for (j = 0; j < 8; j++) {
    pattern[index0 * 8 + j] = tmp[j];
  }
}

void circshift_z(bit direction) {
  uchar i, j, index0, index1;
  uchar tmp[8];
  for (i = 0; i < 7; i++) {
    if (direction) {
      index0 = 7 - i;
      index1 = 6 - i;
    } else {
      index0 = i;
      index1 = i + 1;
    }
    for (j = 0; j < 8; j++) {
      if (i == 0)
        tmp[j] = pattern[j * 8 + index0];
      pattern[j * 8 + index0] = pattern[j * 8 + index1];
    }
  }

  if (direction)
    index0 = 0;
  else
    index0 = 7;
  for (j = 0; j < 8; j++) {
    pattern[j * 8 + index0] = tmp[j];
  }
}

void rotate_x(bit direction) {
  uchar i;
  uchar tmp;

  if (direction) {
    tmp = pattern[0];
    for (i = 0; i < 7; i++) {
      pattern[i * 8] = pattern[(i + 1) * 8];
    }
    for (i = 0; i < 7; i++) {
      pattern[56 + i] = pattern[56 + i + 1];
    }
    for (i = 0; i < 7; i++) {
      pattern[(7 - i) * 8 + 7] = pattern[(7 - i - 1) * 8 + 7];
    }
    for (i = 0; i < 6; i++) {
      pattern[7 - i] = pattern[7 - i - 1];
    }
    pattern[1] = tmp;
  } else {
    tmp = pattern[0];
    for (i = 0; i < 7; i++) {
      pattern[i] = pattern[i + 1];
    }
    for (i = 0; i < 7; i++) {
      pattern[i * 8 + 7] = pattern[(i + 1) * 8 + 7];
    }
    for (i = 0; i < 7; i++) {
      pattern[56 + 7 - i] = pattern[56 + 7 - i - 1];
    }
    for (i = 0; i < 6; i++) {
      pattern[(7 - i) * 8] = pattern[(7 - i - 1) * 8];
    }
    pattern[8] = tmp;
  }
}

void frame(uchar x0, uchar y0, uchar z0, uchar x1, uchar y1, uchar z1) {
  uchar i, tmp;

  tmp = 0x00;
  for (i = x0; i <= x1; i++) {
    tmp = tmp | (0x01 << i);
  }

  linex(y0, z0, tmp);
  linex(y0, z1, tmp);
  linex(y1, z0, tmp);
  linex(y1, z1, tmp);

  tmp = 0;
  for (i = y0; i <= y1; i++) {
    tmp = tmp | (0x01 << i);
  }

  liney(x0, z0, tmp);
  liney(x0, z1, tmp);
  liney(x1, z0, tmp);
  liney(x1, z1, tmp);

  tmp = 0;
  for (i = z0; i <= z1; i++) {
    tmp = tmp | (0x01 << i);
  }

  linez(x0, y0, tmp);
  linez(x0, y1, tmp);
  linez(x1, y0, tmp);
  linez(x1, y1, tmp);
}
