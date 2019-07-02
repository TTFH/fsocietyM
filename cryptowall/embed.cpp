#include <stdio.h>
#include <stdlib.h>

static const char* code =
  "const char *find_embedded_file(const char *name, size_t *size) {\n"
  "  const struct embedded_file *p;\n"
  "  for (p = embedded_files; p->name != NULL; p++) {\n"
  "    if (!strcmp(p->name, name)) {\n"
  "      if (size != NULL) { *size = p->size; }\n"
  "      return (const char *) p->data;\n"
  "    }\n"
  "  }\n"
  "  return NULL;\n"
  "}\n";

int main(int argc, char *argv[]) {
  FILE *fp;
  int i, j, ch;

  for (i = 1; i < argc; i++) {
    if ((fp = fopen(argv[i], "rb")) == NULL) {
      exit(EXIT_FAILURE);
    } else {
      printf("static const unsigned char v%d[] = {", i);
      for (j = 0; (ch = fgetc(fp)) != EOF; j++) {
        if ((j % 12) == 0) {
          printf("%s", "\n ");
        }
        printf(" %#04x,", ch);
      }
      // Append zero byte at the end, to make text files appear in memory
      // as nul-terminated strings.
      printf("%s", " 0x00\n};\n");
      fclose(fp);
    }
  }

  printf("%s", "\nconst struct embedded_file {\n");
  printf("%s", "  const char *name;\n");
  printf("%s", "  const unsigned char *data;\n");
  printf("%s", "  size_t size;\n");
  printf("%s", "} embedded_files[] = {\n");

  for (i = 1; i < argc; i++) {
    printf("  {\"%s\", v%d, sizeof(v%d) - 1},\n", argv[i], i, i);
  }
  printf("%s", "  {NULL, NULL, 0}\n");
  printf("%s", "};\n\n");
  printf("%s", code);

  return EXIT_SUCCESS;
}
