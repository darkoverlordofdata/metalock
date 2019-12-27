/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500
#if WITH_SHADOW_H
#include <shadow.h>
#endif

#include <pwd.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>

#ifdef WITH_IMLIB2
#include <Imlib2.h>
#endif

#ifdef WITH_XFT
#include <Xft/Xft.h>
#endif

#if WITH_BSD_AUTH
#include <login_cap.h>
#include <bsd_auth.h>
#endif

#include "data.h"
#include "alloc.h"
#include "args.h"
#include "event_loop.h"
#include "helper_fns.h"
#include "draw_fns.h"

extern char **script1, **script2, **script3, **script4, **script5, **script6,
  **script7, **script8, **script9, **script0;

extern char *line;
extern char uline[255], pline[255];

#ifndef WITH_BSD_AUTH
  const char *pws;
#endif

Colormap cm;
int bg_r, bg_g, bg_b, fg_r, fg_g, fg_b;
int modifier, control, verbosity, flat_color_only, border_width;
char *fontname, *theme_name;
Bool running = True;
unsigned int len;

Session s;

#ifdef WITH_XFT
XftFont *font;
XRenderColor xrc;
#else
XFontStruct *font;
#endif
XColor bg;
XColor fg;

Window root;

int main(int argc, char **argv) {
  char curs[] = {0, 0, 0, 0, 0, 0, 0, 0};
  int screen;

  Cursor invisible;

  Pixmap pmap;

  XColor black, dummy;
  XSetWindowAttributes wa;
  Visual  *vis;
  int depth;

  Pixmap pix;
#ifdef WITH_IMLIB2
  Imlib_Image image;
  Imlib_Image box_image;
#else
  XpmAttributes at;
  int result = -1;
#endif
  int box_size_x = 0;
  int box_size_y = 0;

  char imgfn[255] = "";
  char boximgfn[255] = "";

  strcpy(pline, "");
  strcat(pline, "Password: ");

  struct passwd *pw;
  char *user_name;
  pw = getpwuid(getuid());
  user_name = pw->pw_name;

  if (verbosity > 0) { puts("begin allocation of data structures...\n"); }
  alloc_data();
  handle_args(argc, argv);

#ifndef WITH_BSD_AUTH
  pws = get_password();
#endif

  if(!(s.dpy = XOpenDisplay(0))) { die("cannot open display\n"); }
  screen = DefaultScreen(s.dpy);
  depth = DefaultDepth(s.dpy, screen);
  root = RootWindow(s.dpy, screen);

  int width = DisplayWidth(s.dpy, screen);
  int height = DisplayHeight(s.dpy, screen);

  int multiplier = 0x101;

  cm = DefaultColormap(s.dpy, screen);

  /* background color */
  bg.red = bg_r * multiplier;
  bg.green = bg_g * multiplier;
  bg.blue = bg_b * multiplier;

  XAllocColor(s.dpy, cm, &bg);

  /* foreground color */
  fg.red = fg_r * multiplier;
  fg.green = fg_g * multiplier;
  fg.blue = fg_b * multiplier;

  XAllocColor(s.dpy, cm, &fg);

  /* init */
  wa.override_redirect = 1;
  wa.background_pixel = bg.pixel;
  vis = DefaultVisual(s.dpy, screen);
  s.w = XCreateWindow(s.dpy, root, 0, 0, width, height,
                    0, depth, CopyFromParent,
                    vis, CWOverrideRedirect | CWBackPixel, &wa);

  if (border_width > 255) {
    puts("Border width is too long, setting to 1...");
    border_width = 1;
  }

#ifdef WITH_IMLIB2
  imlib_context_set_dither(1);
  imlib_context_set_display(s.dpy);
  imlib_context_set_visual(vis);
#endif

#ifdef WITH_IMLIB2
  if (*imgfn == '\0' || file_exists(imgfn) < 0)
    image_filename(theme_name, "bg", "png", imgfn);
  if (*imgfn == '\0' || file_exists(imgfn) < 0) 
    image_filename(theme_name, "bg", "jpg", imgfn);
  if (*imgfn == '\0' || file_exists(imgfn) < 0)
    image_filename(theme_name, "bg", "jpeg", imgfn);
#endif
  if (*imgfn == '\0' || file_exists(imgfn) < 0)
    image_filename(theme_name, "bg", "xpm", imgfn);
  if (*imgfn == '\0' || file_exists(imgfn) < 0)
    image_filename(theme_name, "bg", "xpm.gz", imgfn);

  if (flat_color_only == 0 && verbosity > 1)
    printf("background image filename: %s\n", imgfn);

#ifdef WITH_IMLIB2
  if (*boximgfn == '\0' || file_exists(boximgfn) < 0) 
      image_filename(theme_name, "box", "png", boximgfn);
  if (*boximgfn == '\0' || file_exists(boximgfn) < 0)
    image_filename(theme_name, "box", "jpg", boximgfn);
  if (*boximgfn == '\0' || file_exists(boximgfn) < 0)
    image_filename(theme_name, "box", "jpeg", boximgfn);
#endif
  if (*boximgfn == '\0' || file_exists(boximgfn) < 0)
    image_filename(theme_name, "box", "xpm", boximgfn);
  if (*boximgfn == '\0' || file_exists(boximgfn) < 0)
    image_filename(theme_name, "box", "xpm.gz", boximgfn);

  if (flat_color_only == 0 && verbosity > 1)
    printf("box image filename: %s\n\n", boximgfn);

  if(flat_color_only != 1 && file_exists(imgfn) >= 0) {
#ifdef WITH_IMLIB2
    image = imlib_load_image(imgfn);
    imlib_context_set_image(image);
    int x = imlib_image_get_width();
    int y = imlib_image_get_height();

    Pixmap pixm = XCreatePixmap(s.dpy, s.w, x, y, depth);
    imlib_context_set_drawable(pixm);
    imlib_render_image_on_drawable(0, 0);

    XSetWindowBackgroundPixmap(s.dpy, s.w, pixm);
    XFreePixmap(s.dpy, pixm);
#else
    Pixmap pixm;
    int result = XpmReadFileToPixmap(s.dpy, s.w, imgfn, &pixm, NULL, NULL);
    if (result == XpmSuccess) {
      XSetWindowBackgroundPixmap(s.dpy, s.w, pixm);
      XFreePixmap(s.dpy, pixm);
    }
    else { printf("failed to read pixmap of xpm %s\n", imgfn); }
#endif
  }
  else if (flat_color_only == 0)
    printf("(image %s does not exist\n", imgfn);

  if(flat_color_only != 1 && file_exists(boximgfn) >= 0) {
#ifdef WITH_IMLIB2
    box_image = imlib_load_image(boximgfn);
    imlib_context_set_image(box_image);
    box_size_x = imlib_image_get_width();
    box_size_y = imlib_image_get_height();
#else
    at.valuemask = 0;
    result = XpmReadFileToPixmap(s.dpy, s.w, boximgfn, &pix, NULL, &at);
    box_size_x = at.width;
    box_size_y = at.height;
#endif
  }
  else if (flat_color_only == 1) {
    /* box size, both need to be an even number */
    box_size_x = 430;
    box_size_y = 170;
  }
  else if (flat_color_only == 0)
    printf("image %s does not exist\n", imgfn);

  int tl_x =  width / 2 - .5*box_size_x;
  // int tl_y =  height / 2 - .5*box_size_y;
  int tl_y =  height / 2 - 1.5*box_size_y;
  s.ww =
    XCreateSimpleWindow(s.dpy, s.w, tl_x, tl_y, box_size_x, box_size_y,
                        border_width, fg.pixel, bg.pixel);

  s.gc = XCreateGC(s.dpy, s.w, 0, 0);

  if(flat_color_only != 1 && file_exists(boximgfn) >= 0) {
#ifdef WITH_IMLIB2
    // Image already loaded earlier in this case
    pix = XCreatePixmap(s.dpy, s.ww, box_size_x, box_size_y, depth);
    imlib_context_set_drawable(pix);
    imlib_render_image_on_drawable(0, 0);

    XSetWindowBackgroundPixmap(s.dpy, s.ww, pix);
    XFreePixmap(s.dpy, pix);
#else
    // Pixmap already created earlier in this case
    if (result == XpmSuccess) {
      XSetWindowBackgroundPixmap(s.dpy, s.ww, pix);
      XFreePixmap(s.dpy, pix);
    }
    else { printf("failed to read pixmap of xpm %s\n", boximgfn); }
#endif
  }
  else if (flat_color_only == 0)
    printf("image %s does not exist\n", boximgfn);

#ifdef WITH_XFT
  /* set color to white */
  xrc.red = fg.red;
  xrc.green = fg.green;
  xrc.blue = fg.blue;
  xrc.alpha = 0xffff;
  XftColorAllocValue(s.dpy, DefaultVisual(s.dpy, screen), cm, &xrc, &s.xftc);
#endif
  s.gc2 = XCreateGC(s.dpy, s.ww, 0, 0);
  XAllocNamedColor(s.dpy, cm, "black", &black, &dummy);
  pmap = XCreateBitmapFromData(s.dpy, s.w, curs, 8, 8);
  invisible = XCreatePixmapCursor(s.dpy, pmap, pmap, &black, &black, 0, 0);
  XDefineCursor(s.dpy, s.w, invisible);
  XDefineCursor(s.dpy, s.ww, invisible);
  XMapRaised(s.dpy, s.w);
  XMapRaised(s.dpy, s.ww);

  for(len = 1000; len; len--) {
    if(XGrabPointer(s.dpy, root, False,
                    ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                    GrabModeAsync, GrabModeAsync, None, invisible, CurrentTime)
       == GrabSuccess)
      break;
    usleep(100);
  }
  if((running = running && (len > 0))) {
    for(len = 1000; len; len--) {
      if(XGrabKeyboard(s.dpy, root, True, GrabModeAsync, GrabModeAsync,
                       CurrentTime) == GrabSuccess)
        break;
      usleep(100);
    }
    running = (len > 0);
  }
  len = 0;
  XSync(s.dpy, False);

#ifdef WITH_XFT
  font = XftFontOpenName(s.dpy, screen, fontname);
#else
  font = XLoadQueryFont(s.dpy, fontname);
#endif

  /* if the font does not exist, then fallback to fixed and give warning */
  if (font == NULL) {
    printf("font \"%s\" does not exist\n", fontname);
#ifdef WITH_XFT
    fontname = "6x10-16";
    font = XftFontOpenName(s.dpy, screen, fontname);
#else
    fontname = "fixed";
    font = XLoadQueryFont(s.dpy, fontname);
#endif
  }

#ifdef WITH_XFT
  s.xftd = XftDrawCreate(s.dpy, s.ww, DefaultVisual(s.dpy, screen), cm);
#else
  XSetFont(s.dpy, s.gc, font->fid);
  XSetFont(s.dpy, s.gc2, font->fid);
#endif

  /* Display the pixmaps, if applicable */
  XClearWindow(s.dpy, s.w);
  XClearWindow(s.dpy, s.ww);

  XSetForeground(s.dpy, s.gc, fg.pixel);
  XSetForeground(s.dpy, s.gc2, fg.pixel);

  /* Set initial Username / Password State */
  strcat(uline, "Username: ");
  strcat(uline, user_name);
  strcpy(pline, "Password: ");

  draw_box(s);

  event_loop(s);
  dealloc_data();
  XUngrabPointer(s.dpy, CurrentTime);
  XFreePixmap(s.dpy, pmap);
#ifdef WITH_XFT
  XftColorFree(s.dpy, DefaultVisual(s.dpy, screen), cm, &s.xftc);
  XftDrawDestroy(s.xftd);
#endif
  XFreeGC(s.dpy, s.gc2);
  XFreeGC(s.dpy, s.gc);
  XDestroyWindow(s.dpy, s.w);
  XCloseDisplay(s.dpy);
  return 0;
}
