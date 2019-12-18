#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#if ULTRAGETOPT_REPLACE_GETOPT
#include <ultragetopt.h>
#else
#include <getopt.h>
#endif

#include "helper_fns.h"

extern char **script1;
extern char **script2;
extern char **script3;
extern char **script4;
extern char **script5;
extern char **script6;
extern char **script7;
extern char **script8;
extern char **script9;
extern char **script0;
extern int bg_r;
extern int bg_g;
extern int bg_b;
extern int fg_r;
extern int fg_g;
extern int fg_b;
extern int control;
extern int modifier;
extern int verbosity;
extern int border_width;
extern int flat_color_only;
extern char* fontname;
extern char* theme_name;

static const struct option longopts[] = {
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'V'},
  {"verbosity", required_argument, NULL, 'v'},
  {"border_width", required_argument, NULL, 'w'},
  {"modifier", required_argument, NULL, 'm'},
  {"control_on", no_argument, NULL, 'c'},
  {"control_off", no_argument, NULL, 'C'},
  {"pixmap_on", no_argument, NULL, 'p'},
  {"pixmap_off", no_argument, NULL, 'P'},
  {"bg_color", required_argument, NULL, 'B'},
  {"fg_color", required_argument, NULL, 'F'},
  {"font", required_argument, NULL, 'f'},
  {"theme", required_argument, NULL, 't'},
  {"command_1", required_argument, NULL, '1'},
  {"command_2", required_argument, NULL, '2'},
  {"command_3", required_argument, NULL, '3'},
  {"command_4", required_argument, NULL, '4'},
  {"command_5", required_argument, NULL, '5'},
  {"command_6", required_argument, NULL, '6'},
  {"command_7", required_argument, NULL, '7'},
  {"command_8", required_argument, NULL, '8'},
  {"command_9", required_argument, NULL, '9'},
  {"command_0", required_argument, NULL, '0'}};

static const char *shortopts = "hVm:t:cCpPB:F:w:v:f:1:2:3:4:5:6:7:8:9:0:";

int scriptmutate(int scriptn, int scripti, char* instr) {
  int i = 0, done = 0;
  if (instr == NULL) {
    switch (scriptn) {
    case 1: script1[scripti] = (char *)0; break;
    case 2: script2[scripti] = (char *)0; break;
    case 3: script3[scripti] = (char *)0; break;
    case 4: script4[scripti] = (char *)0; break;
    case 5: script5[scripti] = (char *)0; break;
    case 6: script6[scripti] = (char *)0; break;
    case 7: script7[scripti] = (char *)0; break;
    case 8: script8[scripti] = (char *)0; break;
    case 9: script9[scripti] = (char *)0; break;
    case 0: script0[scripti] = (char *)0; break;
    default: puts("invalid script number\n"); break;
    }
    return 0;
  }
  /* keep on going until after \0 is encountered, to append the termination
     character */
  while(done == 0) {
    switch (scriptn) {
    case 1: script1[scripti][i] = instr[i]; break;
    case 2: script2[scripti][i] = instr[i]; break;
    case 3: script3[scripti][i] = instr[i]; break;
    case 4: script4[scripti][i] = instr[i]; break;
    case 5: script5[scripti][i] = instr[i]; break;
    case 6: script6[scripti][i] = instr[i]; break;
    case 7: script7[scripti][i] = instr[i]; break;
    case 8: script8[scripti][i] = instr[i]; break;
    case 9: script9[scripti][i] = instr[i]; break;
    case 0: script0[scripti][i] = instr[i]; break;
    default: puts("invalid script number\n"); break;
    }
    if (instr[i] == '\0') { done = 1; }
    i++;
  }
  return 0;
}

int handle_args(int argc, char **argv) {

  int longindex = -1, opt, j;

  int h = 0, o = 0, c = 0, v = 0;

  flat_color_only = 0;
  border_width = 0;
  modifier = 4;
  verbosity = 0;
  theme_name = DEFAULT_THEME;

  bg_r = 0x00;
  bg_g = 0x00;
  bg_b = 0x00;

  fg_r = 0xFF;
  fg_g = 0xFF;
  fg_b = 0xFF;

#ifdef WITH_XFT
  fontname = "bitstream vera sans-16";
#else
  fontname = "-*-helvetica-*-r-*-*-24-*-*-*-*-*-*-*";
#endif

  char *t;

  if (verbosity > 1) { puts("process command line arguments...\n"); }
  while ((opt = getopt_long(argc, argv, shortopts, longopts, &longindex))
         != -1) {
    j = 0;
    switch (opt) {
    case 'h':
      h = 1;
      break;
    case 'V':
      v = 1;
      break;
    case 'm':
      modifier = atoi(optarg);
      break;
    case 'w':
      border_width = atoi(optarg);
      break;
    case 'B':
      if (string_is_hex(optarg) == 0) {
        int offset = 0;
        if(strlen(optarg) == 7) { offset = 1; }
        if(strlen(optarg) == 8) { offset = 2; }
        char *r = malloc(5 * sizeof(char));
        char *g = malloc(5 * sizeof(char));
        char *b = malloc(5 * sizeof(char));

        strcpy(r, "");
        strcat(r, "0x");
        r[2] = optarg[offset + 0];
        r[3] = optarg[offset + 1];
        bg_r = strtol(r, NULL, 16);

        strcpy(g, "");
        strcat(g, "0x");
        g[2] = optarg[offset + 2];
        g[3] = optarg[offset + 3];
        bg_g = strtol(g, NULL, 16);

        strcpy(b, "");
        strcat(b, "0x");
        b[2] = optarg[offset + 4];
        b[3] = optarg[offset + 5];
        bg_b = strtol(b, NULL, 16);

        free(r); free(g); free(b);
      }
      else { puts("Invalid background color specified"); }
      break;
    case 'F':
      if (string_is_hex(optarg) == 0) {
        int offset = 0;
        if(strlen(optarg) == 7) { offset = 1; }
        if(strlen(optarg) == 8) { offset = 2; }
        char *r = malloc(5 * sizeof(char));
        char *g = malloc(5 * sizeof(char));
        char *b = malloc(5 * sizeof(char));

        strcpy(r, "");
        strcat(r, "0x");
        r[2] = optarg[offset + 0];
        r[3] = optarg[offset + 1];
        fg_r = strtol(r, NULL, 16);

        strcpy(g, "");
        strcat(g, "0x");
        g[2] = optarg[offset + 2];
        g[3] = optarg[offset + 3];
        fg_g = strtol(g, NULL, 16);

        strcpy(b, "");
        strcat(b, "0x");
        b[2] = optarg[offset + 4];
        b[3] = optarg[offset + 5];
        fg_b = strtol(b, NULL, 16);

        free(r); free(g); free(b);
      }
      else { puts("Invalid foreground color specified"); }
      break;
    case 'c':
      c = 1;
      break;
    case 'C':
      o = 1;
      break;
    case 'f':
      fontname = optarg;
      break;
    case 't':
      theme_name = optarg;
      break;
    case 'v':
      verbosity = atoi(optarg);
      break;
    case 'p':
      flat_color_only = 0;
      break;
    case 'P':
      flat_color_only = 1;
      break;
    case '1':
      t = strtok(optarg, " "); scriptmutate(1, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(1, j, t);
        scriptmutate(1, j, NULL); }
      break;
    case '2':
      t = strtok(optarg, " "); scriptmutate(2, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(2, j, t); }
      break;
    case '3':
      t = strtok(optarg, " "); scriptmutate(3, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(3, j, t); }
      break;
    case '4':
      t = strtok(optarg, " "); scriptmutate(4, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(4, j, t); }
      break;
    case '5':
      t = strtok(optarg, " "); scriptmutate(5, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(5, j, t); }
      break;
    case '6':
      t = strtok(optarg, " "); scriptmutate(6, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(6, j, t); }
      break;
    case '7':
      t = strtok(optarg, " "); scriptmutate(7, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(7, j, t); }
      break;
    case '8':
      t = strtok(optarg, " "); scriptmutate(8, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(8, j, t); }
      break;
    case '9':
      t = strtok(optarg, " "); scriptmutate(9, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(9, j, t); }
      break;
    case '0':
      t = strtok(optarg, " "); scriptmutate(0, 0, t);
      while (t != NULL) { j++; t = strtok(NULL, " "); scriptmutate(0, j, t); }
      break;
    default:
      break;
    }
  }

  /* can't have control on and off at the same time... */
  if (c == 1 && o == 1) {
    printf("can't specify both -c and -o"); exit(-1);
  }

  /* if o is set, then control is turned off */
  if (c == 0 && o == 1) { control = 0; }

  /* if neither option is set, control is turned on */
  if (c == 0 && o == 0) { control = 1; }

  if (verbosity > 0) {
    printf("font: \"%s\"\n", fontname);
    printf("verbosity: %d\n", verbosity);
    printf("control: %d\n", control);
    printf("modifier: %d\n", modifier);
    if (flat_color_only > 0) { printf("pixmap: %d\n\n", 0); }
    else { printf("pixmap: %d\n\n", 1); }
  }

  if (verbosity > 1) {
#ifdef WITH_IMLIB2
    printf("imlib2: yes\n");
#else
    printf("imlib2: no\n");
#endif

#ifdef WITH_XFT
    printf("xft: yes\n");
#else
    printf("xft: no\n");
#endif

#ifdef ULTRAGETOPT_REPLACE_GETOPT
    printf("ultragetopt: yes\n\n");
#else
    printf("ultragetopt: no\n\n");
#endif
  }

  if (v == 1) {
    printf("metalock-%s, © 2012 Timothy Beyer\n", VERSION);
    printf("based on slock-0.9, © 2006-2008 Anselm R Garbe\n");
    exit(0);
  }

  if (c == 1) { }
  if (h == 1) {
    puts("Usage: metalock [option]\n");

    puts("-h / --help               help (this)");
    puts("-V / --version            version information");
    puts("-c / --control_on         use control with keys (default)");
    puts("-C / --control_off        do not use control with keys");
    puts("-m n / --modifier n       modifier to use with keys (default: 4)");
    puts("-v n / --verbosity n      verbosity level (default: 0)");
    puts("-w n / --border_width n   login box border width (default: 0)");
    puts("-f str / --font str       X11 quoted font name string");
    puts("-p / --pixmap_on          show pixmap image background (default)");
    puts("-P / --pixmap_off         only show flat color background");
    puts("-B / --bg_color           background color hex (default: #000000)");
    puts("-F / --fg_color           background color hex (default: #FFFFFF)");
    puts("-t name / --theme name    theme name (default: " DEFAULT_THEME ")");
    puts("-1 cmd / --command_1 cmd  quoted command to run on cmd 1");
    puts("-2 cmd / --command_2 cmd  quoted command to run on cmd 2");
    puts("-3 cmd / --command_3 cmd  quoted command to run on cmd 3");
    puts("-4 cmd / --command_4 cmd  quoted command to run on cmd 4");
    puts("-5 cmd / --command_5 cmd  quoted command to run on cmd 5");
    puts("-6 cmd / --command_6 cmd  quoted command to run on cmd 6");
    puts("-7 cmd / --command_7 cmd  quoted command to run on cmd 7");
    puts("-8 cmd / --command_8 cmd  quoted command to run on cmd 8");
    puts("-9 cmd / --command_9 cmd  quoted command to run on cmd 9");
    puts("-0 cmd / --command_0 cmd  quoted command to run on cmd 0");

    puts("\nmail bug reports and suggestions to " EMAIL);
    exit(0);
  }
  return 0;
}
