#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Flag {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flag;

void echo();
void print(const char *name, Flag flags);
static void meow_cat(FILE *f, Flag flags);
void print_wo_flags(const char *name);
void treatment(char *argv[], int argc, int checker, Flag flags);
static void usage(void);
void set_flag(int ch, Flag *flags);
void flags_input(int argc, char *argv[], Flag *flags);

int main(int argc, char **argv) {
  if (argc == 1) {
    echo();
  } else {
    Flag flags = {0, 0, 0, 0, 0, 0};
    flags_input(argc, argv, &flags);
    if (flags.b || flags.e || flags.n || flags.s || flags.t || flags.v) {
      treatment(argv, argc, 1, flags);
    } else {
      treatment(argv, argc, 0, flags);
    }
  }

  return 0;
}

void usage() {
  printf("usage: s21_cat [OPTION]... [FILE]...");
  exit(1);
}

void treatment(char *argv[], int argc, int checker, Flag flags) {
  for (int i = 1; i < argc; i++) {
    if (checker) {
      if (i > 1 && argv[i][0] != '-') {
        print(argv[i], flags);
      }
    } else {
      print_wo_flags(argv[i]);
    }
  }
}

void print_wo_flags(const char *name) {
  FILE *f = fopen(name, "r");
  if (f == NULL) {
    printf("Failed to open the file.\n");
  } else {
    char *line = 0;
    size_t len = 0;
    while (getline(&line, &len, f) != -1) {
      printf("%s", line);
    }
    free(line);
  }
  fclose(f);
}

void echo() {
  while (1) {
    char text[256];
    if (fgets(text, sizeof(text), stdin) == NULL) {
      break;
    }
    printf("%s", text);
  }
}

void set_flag(int ch, Flag *flags) {
  switch (ch) {
    case 'b':
      flags->b = 1;
      flags->n = 1;
      break;
    case 'e':
      flags->e = 1;
      flags->v = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 't':
      flags->t = 1;
      flags->v = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'E':
      flags->e = 1;
      break;
    case 'T':
      flags->t = 1;
      break;
    default:
      usage();
  }
}

void flags_input(int argc, char *argv[], Flag *flags) {
  int ch;
  static struct option const long_options[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}};
  while ((ch = getopt_long(argc, argv, "benstvET", long_options, NULL)) != -1) {
    set_flag(ch, flags);
  }
}

void print(const char *name, Flag flags) {
  FILE *f = fopen(name, "r");
  if (!f) {
    printf("cat: %s: No such file or directory\n", name);
  } else {
    meow_cat(f, flags);
    fclose(f);
  }
}

static void meow_cat(FILE *f, Flag flags) {
  int ch, prev, gobble = 0, line = 0;
  for (prev = '\n'; (ch = getc(f)) != EOF; prev = ch) {
    if (prev == '\n') {
      if (flags.s) {
        if (ch == '\n') {
          if (gobble) {
            continue;
          }
          gobble = 1;
        } else {
          gobble = 0;
        }
      }
      if (flags.n && (!flags.b || ch != '\n')) {
        printf("%6d\t", ++line);
      }
    }
    if (ch != '\n' && ch != '\t' && flags.v) {
      if (ch < 32) {
        printf("^%c", ch + 64);
      } else if (ch == 127) {
        printf("^?");
      } else if (ch > 127 && ch < 160) {
        printf("M-^%c", ch - 64);
      } else {
        printf("%c", ch);
      }
    } else {
      if (flags.e && ch == '\n') {
        printf("$%c", ch);
      } else if (flags.t && ch == '\t') {
        printf("^I");
      } else {
        printf("%c", ch);
      }
    }
  }
}
