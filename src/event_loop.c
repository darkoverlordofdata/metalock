#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/dpms.h>
#include <X11/keysym.h>

#include <pwd.h>

#ifdef WITH_IMLIB2
#include <Imlib2.h>
#endif

#ifdef WITH_XFT
#include <Xft/Xft.h>
#endif

#include "helper_fns.h"
#include "draw_fns.h"

/* how many characters of password, counting "Password: " to show visually */
const int pass_num_show = 32;

XEvent ev;
KeySym ksym;
char buf[32], passwd[256];
int num;

extern Bool running;

extern char **script1, **script2, **script3, **script4, **script5, **script6,
  **script7, **script8, **script9, **script0;

extern int modifier, control, verbosity;
extern unsigned int len;
extern const char *pws;

extern char *line;
extern char uline[255], pline[255];

int event_loop(Session s) {
  /* main event loop */
  while(running && !XNextEvent(s.dpy, &ev)) {
    if (ev.type == KeyPress && ev.xkey.state &&
        (((control == 1) &&
          ((modifier == 1 && (ev.xkey.state == (Mod1Mask | ControlMask))) ||
           (modifier == 2 && (ev.xkey.state == (Mod2Mask | ControlMask))) ||
           (modifier == 3 && (ev.xkey.state == (Mod3Mask | ControlMask))) ||
           (modifier == 4 && (ev.xkey.state == (Mod4Mask | ControlMask))) ||
           (modifier == 5 && (ev.xkey.state == (Mod5Mask | ControlMask))))) ||
         ((control == 0) &&
          ((modifier == 1 && ev.xkey.state == Mod1Mask) ||
           (modifier == 2 && ev.xkey.state == Mod2Mask) ||
           (modifier == 3 && ev.xkey.state == Mod3Mask) ||
           (modifier == 4 && ev.xkey.state == Mod4Mask) ||
           (modifier == 5 && ev.xkey.state == Mod5Mask))))) {
      switch (ev.xkey.keycode) {
      case 10:
        if (file_exists(script1[0]) >= 0 && fork() == 0)
          execv(script1[0], script1);
        break;
      case 11:
        if (file_exists(script2[0]) >= 0 && fork() == 0)
          execv(script2[0], script2);
        break;
      case 12:
        if (file_exists(script3[0]) >= 0 && fork() == 0)
          execv(script3[0], script3);
        break;
      case 13:
        if (file_exists(script4[0]) >= 0 && fork() == 0)
          execv(script4[0], script4);
        break;
      case 14:
        if (file_exists(script5[0]) >= 0 && fork() == 0)
          execv(script5[0], script5);
        break;
      case 15:
        if (file_exists(script6[0]) >= 0 && fork() == 0)
          execv(script6[0], script6);
        break;
      case 16:
        if (file_exists(script7[0]) >= 0 && fork() == 0)
          execv(script7[0], script7);
        break;
      case 17:
        if (file_exists(script8[0]) >= 0 && fork() == 0)
          execv(script8[0], script8);
        break;
      case 18:
        if (file_exists(script9[0]) >= 0 && fork() == 0)
          execv(script9[0], script9);
        break;
      case 19:
        if (file_exists(script0[0]) >= 0 && fork() == 0)
          execv(script0[0], script0);
        break;
      default:
        break;
      }
    }
    else if(ev.type == KeyPress) {
      redraw_text(s);
      buf[0] = 0;
      num = XLookupString(&ev.xkey, buf, sizeof buf, &ksym, 0);
      if(IsKeypadKey(ksym)) {
        if(ksym == XK_KP_Enter)
          ksym = XK_Return;
        else if(ksym >= XK_KP_0 && ksym <= XK_KP_9)
          ksym = (ksym - XK_KP_0) + XK_0;
      }
      if(IsFunctionKey(ksym) || IsKeypadKey(ksym)
         || IsMiscFunctionKey(ksym) || IsPFKey(ksym)
         || IsPrivateKeypadKey(ksym))
        continue;
      switch(ksym) {
      case XK_Return:
        passwd[len] = 0;
#ifdef WITH_BSD_AUTH
        running = !auth_userokay(getlogin(), NULL, "auth-xlock", passwd);
#else
        running = strcmp(crypt(passwd, pws), pws);
#endif
        if (verbosity > 1) { puts("[return]"); }

        if (running != 0) {
          XBell(s.dpy, 100);
          printf("Password failed!! Try again!\n");
          strcat(pline, "");
          strcpy(pline, "Password: ");
          redraw_text(s);
        }
        len = 0;
        if (running == 0) { puts("Unlocking Screen..."); }
        break;
      case XK_Escape:
        len = 0;
        break;
      case XK_BackSpace:
        if(len) {
          --len;
          if (verbosity > 1) { printf("[backspace]"); }
          char tmp[255] = "Password: ";
          strncpy(tmp, pline, strlen(tmp) + len);
          strcpy(pline, "");
          strcat(pline, tmp);
          strcpy(tmp, "");
          redraw_text(s);
        }
        break;
      default:
        if(num && !iscntrl((int) buf[0]) && (len + num < sizeof passwd)) {
          memcpy(passwd + len, buf, num);
          len += num;
          if (strlen(pline) < pass_num_show) { strcat(pline, "*"); }
          int new_pline_len = strlen(pline);
          redraw_text(s);
          if (verbosity > 1 && new_pline_len < pass_num_show) { printf("*"); }
        }
        else if (len + num >= sizeof passwd) {
          passwd[len] = 0;

          XBell(s.dpy, 100);
          if (verbosity > 1) { puts("(terminated at 255 chars)"); }
          printf("Specified password is *unreasonably* long!! Try again!\n");
          len = 0;
          strcat(pline, "");
          strcpy(pline, "Password: ");
          redraw_text(s);
        }
        break;
      }
    }
    else if(ev.type == MotionNotify || ev.type == ButtonPress)
      redraw_text(s);
  }
  return 0;
}
