#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int countFiles;
  int numberLine;
  int countMatches;
} flags;

void usage() {
  printf("usage: grep [options] template [file_name]\n");
  exit(2);
}

void work(int argc, char **argv);

int flags_input(int argc, char **argv, flags *flag, char **template);
void meow_grep(char *str, flags *flags, char *tmpl, char *name);
void fFunc(flags *flag, char **template, const char *optarg);
void eFunc(flags *flag, char **template, const char *optarg);
void oFunc(regex_t my_regex, char *str, flags flag);
void s21_grep(char *name, flags flag, char *tmpl);
void grepCL(flags flag, const char *file_name);
int createTemplate(char **str, const char *optarg);
void OLVC(flags *flag, int argc);
void addTemplate(char **str, const char *optarg);

int main(int argc, char **argv) {
  work(argc, argv);
  return 0;
}

void work(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    flags flag = {0};

    for (int x = flags_input(argc, argv, &flag, &temp); x < argc; x += 1)
      s21_grep(argv[x], flag, temp);

    if (temp != NULL) free(temp);
  }
  if (argc == 1) {
    usage();
  }
}

int flags_input(int argc, char **argv, flags *flag, char **template) {
  for (int ch = 0; (ch = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
    switch (ch) {
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      case 'e':
        eFunc(flag, template, optarg);
        break;
      case 'f':
        fFunc(flag, template, optarg);
        break;
      default:
        usage();
    }
  }
  if ((flag->e || flag->f) == 0) {
    createTemplate(template, argv[optind]);
    optind += 1;
  }
  OLVC(flag, argc);
  return optind;
}

void s21_grep(char *file_name, flags flag, char *template) {
  FILE *file = fopen(file_name, "r");
  if (file != NULL) {
    for (int ch = '0'; (ch = getc(file)) != EOF;) {
      char *line = calloc(256, 1);
      flag.numberLine += 1;
      int length = 0;

      for (line[length] = '\0'; ch != EOF && ch != '\n'; ch = getc(file)) {
        line[length] = ch;
        line[length += 1] = '\0';
        if (length % 255 == 0) {
          line = realloc(line, length + 256);
          if (line == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(EXIT_FAILURE);
          }
        }
      }

      meow_grep(line, &flag, template, file_name);
      free(line);
    }
    grepCL(flag, file_name);
    fclose(file);
  } else if (flag.s == 0) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
  }
}

void fFunc(flags *flag, char **template, const char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    for (int ch = '0'; (ch = getc(file)) != EOF;) {
      int length = 0;
      char *line = calloc(256, 1);

      for (line[length] = '\0'; ch != EOF && ch != '\n'; ch = getc(file)) {
        line[length] = ch;
        line[length += 1] = '\0';
        if (length % 255 == 0) {
          line = realloc(line, length + 256);
          if (line == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(EXIT_FAILURE);
          }
        }
      }

      if (!(flag->e || flag->f))
        flag->f = createTemplate(template, line);
      else
        addTemplate(template, line);
      free(line);
    }
    fclose(file);
  } else if (flag->s == 0) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
  }
}

void eFunc(flags *flag, char **template, const char *optarg) {
  if (!(flag->e || flag->f))
    flag->e = createTemplate(template, optarg);
  else
    addTemplate(template, optarg);
}

void grepCL(flags flag, const char *file_name) {
  if (flag.c) {
    if (flag.l) {
      flag.countFiles > 1 ? printf("%s:1\n", file_name) : printf("1\n");
    } else {
      if (flag.countFiles > 1) printf("%s:", file_name);
      printf("%i\n", flag.countMatches);
    }
  }
  if (flag.l && flag.countMatches) printf("%s\n", file_name);
}

void meow_grep(char *line, flags *flag, char *template, char *file_name) {
  regex_t regex;
  if (regcomp(&regex, template, flag->i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == flag->v) {
      flag->countMatches += 1;
      if ((flag->c || flag->l) == 0) {
        if (flag->countFiles > 1 && !(flag->h)) printf("%s:", file_name);
        if (flag->n) printf("%i:", flag->numberLine);
        if (!flag->o)
          printf("%s\n", line);
        else
          oFunc(regex, line, *flag);
      }
    }
    regfree(&regex);
  }
}

void oFunc(regex_t regex, char *line, flags flag) {
  while (regexec(&regex, line, 0, NULL, 0) == flag.v) {
    int end;
    char *tmpstr = (char *)calloc(strlen(line) + 1, 1);
    strcpy(tmpstr, line);
    end = 3;
    end = end - 3;
    end = strlen(line);

    while (regexec(&regex, tmpstr, 0, NULL, 0) == flag.v) {
      end--;
      tmpstr[strlen(tmpstr) - 1] = 0;
    }
    tmpstr[strlen(tmpstr)] = line[strlen(tmpstr)];
    int start = 0;
    while (regexec(&regex, tmpstr, 0, NULL, 0) == flag.v &&
           strlen(tmpstr) > 0) {
      start++;
      int j = 0;
      while (tmpstr[j] != 0) {
        tmpstr[j] = tmpstr[j + 1];
        j++;
      }
    }
    start--;
    int i = strlen(tmpstr);
    while (i != 0) {
      tmpstr[i] = tmpstr[i - 1];
      i--;
    }
    tmpstr[0] = line[start];
    printf("%s\n", tmpstr);
    free(tmpstr);
    i = start + 1;
    while (line[i] != 0) {
      line[i - start - 1] = line[i];
      i++;
    }
    line[i - start - 1] = 0;
  }
}

void OLVC(flags *flag, int argc) {
  if (flag->o && (flag->l || flag->v || flag->c)) flag->o = 0;
  flag->countFiles = argc - optind;
}

int createTemplate(char **str, const char *optarg) {
  *str = calloc(strlen(optarg) + 1, 1);
  if (*str) strcpy(*str, optarg);
  return *str ? 1 : 0;
}

void addTemplate(char **str, const char *optarg) {
  int size = strlen(*str) + strlen(optarg) + 2;
  *str = realloc(*str, size);
  if (*str != NULL) {
    strcat(strcat(*str, "|"), optarg);
  } else {
    fprintf(stderr, "Error: Unable to reallocate memory\n");
    exit(EXIT_FAILURE);
  }
}
