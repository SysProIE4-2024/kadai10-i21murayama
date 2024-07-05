/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
int mysystem(char *command) {
  int status = 0;
  char *execpath = "/bin/sh";

  if (command == NULL) {
    return 1;
  }

  int pid;
  if ((pid=fork()) < 0) {
    return -1;
  }

  if (pid != 0) { // 親プロセス
    while (wait(&status) != pid);
  } else {        // 子プロセス
    execl(execpath, "sh", "-c", command, NULL);
    exit(127);
  }

  return status;
}

/* 実行例

% make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c


% ls -l
total 616
-rw-r--r--  1 Ryoma  staff     143  7  4 09:42 Makefile
-rw-r--r--  1 Ryoma  staff    2795  7  4 09:42 README.md
-rw-r--r--  1 Ryoma  staff  238232  7  4 09:42 README.pdf
-rwxr-xr-x  1 Ryoma  staff   50392  7  4 10:14 mysysmain
-rw-r--r--  1 Ryoma  staff     925  7  4 09:42 mysysmain.c
-rw-r--r--  1 Ryoma  staff    1942  7  4 10:14 mysystem.c
-rw-r--r--  1 Ryoma  staff      90  7  4 09:42 mysystem.h

% ./mysysmain "ls -l"
mysystem:
total 616
-rw-r--r--  1 Ryoma  staff     143  7  4 09:42 Makefile
-rw-r--r--  1 Ryoma  staff    2795  7  4 09:42 README.md
-rw-r--r--  1 Ryoma  staff  238232  7  4 09:42 README.pdf
-rwxr-xr-x  1 Ryoma  staff   50392  7  4 10:14 mysysmain
-rw-r--r--  1 Ryoma  staff     925  7  4 09:42 mysysmain.c
-rw-r--r--  1 Ryoma  staff    1942  7  4 10:14 mysystem.c
-rw-r--r--  1 Ryoma  staff      90  7  4 09:42 mysystem.h
retval = 00000000
system:
total 616
-rw-r--r--  1 Ryoma  staff     143  7  4 09:42 Makefile
-rw-r--r--  1 Ryoma  staff    2795  7  4 09:42 README.md
-rw-r--r--  1 Ryoma  staff  238232  7  4 09:42 README.pdf
-rwxr-xr-x  1 Ryoma  staff   50392  7  4 10:14 mysysmain
-rw-r--r--  1 Ryoma  staff     925  7  4 09:42 mysysmain.c
-rw-r--r--  1 Ryoma  staff    1942  7  4 10:14 mysystem.c
-rw-r--r--  1 Ryoma  staff      90  7  4 09:42 mysystem.h
retval = 00000000


% ./mysysmain
使い方 : ./mysysmain コマンド文字列


% ./mysysmain hoge
mysystem:
sh: hoge: command not found
retval = 00007f00
system:
sh: hoge: command not found
retval = 00007f00


% ./mysysmain mkdir
mysystem:
usage: mkdir [-pv] [-m mode] directory_name ...
retval = 00004000
system:
usage: mkdir [-pv] [-m mode] directory_name ...
retval = 00004000

*/
