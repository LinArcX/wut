#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

static int base = 1;
char* version = "0.1.0";

void display_tree(char* path, int level) {
  WIN32_FIND_DATA find_data;
  HANDLE hFind;
  char new_path[MAX_PATH];

  sprintf(new_path, "%s\\*", path);
  hFind = FindFirstFile(new_path, &find_data);

  if (hFind == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError();
    LPSTR message = NULL;
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&message,
      0,
      NULL
    );
    printf("Error: %s\n", message);
    LocalFree(message);
    return;
  }

  do {
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
        printf("%*s%s\\\n", base, "", find_data.cFileName);
        sprintf(new_path, "%s\\%s", path, find_data.cFileName);
        if (base < level)
        {
          ++base;
          display_tree(new_path, level);
        }
      }
    }
    else {
      printf("%*s%s\n", base, "", find_data.cFileName);
    }
  } while (FindNextFile(hFind, &find_data) != 0);
  --base;

  FindClose(hFind);
}

void help()
{
  printf("NAME\ttree\n");
  printf("\ttree - list contents of directories in a tree-like format.(using Win32 API)\n\n\n");

  printf("SYNOPSIS\n");
  printf("\t tree.exe [OPTIONS...]\n\n\n");

  printf("DESCRIPTION\n");
  printf("\tTree is a recursive directory listing program that produces a depth indented listing of files\n \\
    \twhich is colorized ala dircolors if the LS_COLORS environment variable is set and output is to tty.\n\n \\
    \tWith no arguments, tree lists the files in the current directory. When directory arguments are given, tree lists all the files and/or directories found in the given directories each in turn.\n\n \\
    \tUpon completion of listing all files/directories found, tree returns the total number of files and/or directories listed.\n\n \\
    \n\n");

  printf("OPTIONS\n");
  printf("\t -p\n");
  printf("\t\tThe path that `tree` will work on it.\n\n");

  printf("\t -l\n");
  printf("\t\tThe level of hierarchy.\n\n");

  printf("\t -v\n");
  printf("\t\tVersion number.\n\n");

  printf("\t -h\n");
  printf("\t\tShows this help page.\n\n");
}

int main(int argc, char* argv[]) {
  int level = 1;
  char* path = ".";

  for (size_t i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-v"))
    {
      printf("version: %s\n", version);
      return 0;
    }
    else if (0 == strcmp(argv[i], "-h"))
    {
      help();
      return 0;
    }
    else if (0 == strcmp(argv[i], "-p"))
    {
      path = argv[i + 1];
    }
    else if (0 == strcmp(argv[i], "-l"))
    {
      level = (int)strtol(argv[i + 1], NULL, 10);
    }
  }

  display_tree(path, level);
  return 0;
}
