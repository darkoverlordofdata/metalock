#include <stdlib.h>

char **script1;
char **script2;
char **script3;
char **script4;
char **script5;
char **script6;
char **script7;
char **script8;
char **script9;
char **script0;

int arg_length = 256;
int max_args = 64;

int alloc_data() {

  int i = 0;
  script1 = (char**)malloc(max_args * sizeof(char *));
  *script1 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script1[i] = *script1 + (i * arg_length); }
  script2 = (char**)malloc(max_args * sizeof(char *));
  *script2 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script2[i] = *script2 + (i * arg_length); }
  script3 = (char**)malloc(max_args * sizeof(char *));
  *script3 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script3[i] = *script3 + (i * arg_length); }
  script4 = (char**)malloc(max_args * sizeof(char *));
  *script4 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script4[i] = *script4 + (i * arg_length); }
  script5 = (char**)malloc(max_args * sizeof(char *));
  *script5 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script5[i] = *script5 + (i * arg_length); }
  script6 = (char**)malloc(max_args * sizeof(char *));
  *script6 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script6[i] = *script6 + (i * arg_length); }
  script7 = (char**)malloc(max_args * sizeof(char *));
  *script7 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script7[i] = *script7 + (i * arg_length); }
  script8 = (char**)malloc(max_args * sizeof(char *));
  *script8 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script8[i] = *script8 + (i * arg_length); }
  script9 = (char**)malloc(max_args * sizeof(char *));
  *script9 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script9[i] = *script9 + (i * arg_length); }
  script0 = (char**)malloc(max_args * sizeof(char *));
  *script0 = (char*)malloc(max_args * arg_length * sizeof(char));
  for (i = 0; i < max_args; i++) { script0[i] = *script0 + (i * arg_length); }
  return 0;
}

int dealloc_data() {
  free(*script1); free(script1);
  free(*script2); free(script2);
  free(*script3); free(script3);
  free(*script4); free(script4);
  free(*script5); free(script5);
  free(*script6); free(script6);
  free(*script7); free(script7);
  free(*script8); free(script8);
  free(*script9); free(script9);
  free(*script0); free(script0);
  return 0;
}
