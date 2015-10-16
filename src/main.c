/*
Copyright 2015 Jochem Raat

This file is part of etbi.

etbi is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your
option) any later version.

etbi is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with etbi.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "parse.h"
#include "optimize.h"
#include "eval.h"
#include "tape.h"

void perror_file (char *, char *);
void usage (char *);
void version (void);

static int verbose_flag;

int
main (int argc, char **argv)
{
  struct option long_options[] =
    {
      {"verbose", no_argument, &verbose_flag, 1},
      {"version", no_argument, NULL, 'V'},
      {"help", no_argument, NULL, 'h'},
      {"example", required_argument, NULL, 'e'},
      {0, 0, 0, 0}
    };

  int c, option_index = 0;
  char *input_file_name = NULL;

  while ((c = getopt_long (argc, argv, "hVe:v",
                           long_options, &option_index))
         != -1)
    {
      switch (c)
        {
        case 0:
          break;
        case 'V':
          version();
          break;
        case 'h':
          usage (argv[0]);
          break;
        case 'v':
          verbose_flag = 1;
        case 'e':
          input_file_name
            = (char *) malloc (strlen (EXAMPLE_PATH) + strlen (optarg)
                               + strlen (BF_SUFFIX) + 2);
          strcpy (input_file_name, EXAMPLE_PATH);
          strcat (input_file_name, "/");
          strcat (input_file_name, optarg);
          strcat (input_file_name, BF_SUFFIX);
          break;
        case '?':
          usage (argv[0]);
          break;
        default:
          abort ();
        }
    }

  if (optind < argc)
    input_file_name = argv[optind];

  FILE *input;
  if (input_file_name)
    input = fopen (input_file_name, "r");
  else
    input = stdin;

  if (!input)
    {
      perror_file(argv[0], input_file_name);
      return 1;
    }


  instruction_list *insts = parse_brainfuck (input);
  insts = optimize_brainfuck (insts);
  if (verbose_flag)
    print_instructions (insts);

  eval_brainfuck (insts);

  return 0;
}

void
perror_file (char *prog_name, char *file_name)
{
  char *error_prefix = (char *)
    malloc (strlen (prog_name) + strlen (file_name) + 3);
  strcpy (error_prefix, prog_name);
  strcat (error_prefix, ": ");
  strcat (error_prefix, file_name);

  perror (error_prefix);
}

/**
 * Print usage message and exit normally
 */
void
usage (char *prog_name)
{
  printf
    (PACKAGE_STRING "\n"
     COPYRIGHT_STRING "\n"
     "Endless tape brainfuck interpreter\n"
     "\n"
     "Usage: %s [OPTION]... [FILE]\n"
     "\n"
     "  -e, --example=NAME    Load example program\n"
     "  -v, --verbose         Print extra debugging information\n"
     "  -V, --version         Output version information and exit\n"
     "  -h, --help            Display this help and exit\n"
     "\n"
     "NOTE: if no FILE is specified %s will default to\n"
     "reading a brainfuck file from standard input\n"
     "\n"
     "Report bugs to <" PACKAGE_BUGREPORT ">\n",
     prog_name, prog_name);
  exit(0);
}

/**
 * Print version message and exit normally
 */
void
version ()
{
  printf
    (PACKAGE_STRING "\n"
     COPYRIGHT_STRING "\n"
     "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n");
  exit(0);
}
