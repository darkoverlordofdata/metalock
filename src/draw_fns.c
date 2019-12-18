#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef WITH_XFT
#include <Xft/Xft.h>
#endif

#include "data.h"

int lx, ly, ux, uy, px, py, llen, ullen, pllen;
char *line = "Locked Screen";
char uline[255] = "";
char pline[255] = "";

#ifdef WITH_XFT
extern XftFont *font;
#endif

int redraw_text(Session s) {
  XClearWindow(s.dpy, s.ww);
  lx = 40; ly = 50; ux = lx; uy = ly + 40; px = ux; py = uy + 40;
  llen = strlen(line), ullen = strlen(uline), pllen = strlen(pline);

#ifdef WITH_XFT
  XftDrawString8(s.xftd, &s.xftc, font, lx, ly, (XftChar8 *)line, llen);
  XftDrawString8(s.xftd, &s.xftc, font, ux, uy, (XftChar8 *)uline, ullen);
  XftDrawString8(s.xftd, &s.xftc, font, px, py, (XftChar8 *)pline, pllen);
#else
  XDrawString(s.dpy, s.ww, s.gc2, lx, ly, line, llen);
  XDrawString(s.dpy, s.ww, s.gc2, ux, uy, uline, ullen);
  XDrawString(s.dpy, s.ww, s.gc2, px, py, pline, pllen);
#endif
  return 0;
}

int draw_box(Session s) { redraw_text(s); return 0; }
