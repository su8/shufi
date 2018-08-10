/*
   08/10/2018
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
*/
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

static void print_numbers(uintmax_t, uintmax_t);
static uintmax_t cnum(char *);
static void fill_strs(const char *);

static const char doc[] = "Write a random permutation of the input lines to standard output.\vMandatory arguments to long options are mandatory for short options too.\n";
const char *argp_program_version = "shufi 1.0.0";
static struct argp_option options[] =
{
  { .doc = "" },
  { .name = "input-range",   .key = 'i',  .arg="LO-HI, e.g: 1-100", .doc = "treat each number LO through HI as an input line" },
  { .name = "head-count",    .key = 'n', .arg="COUNT", .doc = "output at most COUNT lines"  },
  { .doc = NULL }
};
static uintmax_t arr[10000] = {0};
static char str1[256] = {""};
static char str2[256] = {""};
static uintmax_t res_iters = 10;

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  (void)state;

  switch(key)
  {
    case 'i': 
      fill_strs(arg);
      break;
    case 'n':
      res_iters = cnum(arg);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  struct argp arg_parser = {
    .doc = doc,
    .options = options, 
    .parser = parse_opt
  };
  argp_parse(&arg_parser, argc, argv, 0, NULL, NULL);

  if (argc < 2) {
    printf("%s\n", "usage: shufi -i 1-100 -n 10");
    return EXIT_FAILURE;
  }
  print_numbers(cnum(str2), res_iters);

  return EXIT_SUCCESS;
}

static void print_numbers(uintmax_t iters, uintmax_t restrict_iters) {
  time_t t;
  uintmax_t z = 0;
  uintmax_t x = 0;
  uintmax_t y = cnum(str1);
  uintmax_t w = 0;
  uintmax_t v = y;

  if (0 == iters) {
    iters = 10;
  }

  if (-1 == (t = time(NULL))) {
      printf("%s\n", "time(NULL) failed");
      return;
  }
  if (9999U < restrict_iters) {
      printf("%s\n", "usage: shufi -i 1-100 -n 10");
      return;
  }
  srandom((unsigned int)t);

  /* Fill the x-to-z range */
  for (; y < iters; y++, x++) {
    arr[x] = y;
  }

  if (v >= iters) {
      printf("%s\n", "usage: shufi -i 1-100 -n 10");
      return;
  }

  /* index the newly created array
      and make sure that we dont print
      any "0" in here */
  for (z = 0; z < restrict_iters; z++) {
    while (0 == (w = arr[(uintmax_t)random() % iters])) {
      continue;
    }
    printf("%" PRIuMAX "\n", w);
  }
}

static uintmax_t cnum(char *str) {
   return strtoumax(str ? str : "1000", (char **)NULL, 10);
}

static void fill_strs(const char *s) {
  char *s1 = str1;
  char *s2 = str2;

  if (255 < (strlen(s) - 1)) {
    snprintf(str1, 255, "%s", "1");
    snprintf(str2, 255, "%s", "10");
    return;
  }

  for (; *s && *s != '-'; s++) {
    *s1++ = *s;
  }
  *s1 = '\0';

  if (*s) {
    s++;
  }

  for (; *s; s++) {
    *s2++ = *s;
  }
  *s2 = '\0';
}
