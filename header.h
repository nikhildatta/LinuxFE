#include <bits/stdc++.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

#define INVERT "\033[7m"
#define REVERT "\033[0m"
#define CLRSCR "\033c"
#define ALIGN_FN 24
#define ALIGN_SZ 40
#define ALIGN_PM 56
#define ALIGN_USR 68
#define ALIGN_GR 80
#define SCREEN_HT 30

using namespace std;
using namespace std::filesystem;

string file_size(double size);
string perms_linux(perms p);
void print_dir(path p, bool end_dir = true);
void print_dir_ent(directory_entry d, bool color);
void scroll_dir(path p, bool end_dir);
bool cmd_mode();

static path home;
extern char** environ;