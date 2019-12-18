#ifndef _DATA_H
#define _DATA_H

typedef struct {
  Display *dpy;
  Window w;
  Window ww;
  GC gc;
  GC gc2;
#ifdef WITH_XFT
  XftDraw *xftd;
  XftColor xftc;
#endif
} Session;

#endif
