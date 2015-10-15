/*
 *  SCANDIR
 *  Scan a directory, collecting all (selected) items into a an array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>


#define INITIAL_SIZE	20
#define DIRSIZ(d) (sizeof(struct direct))


int scandir(char *Name, struct direct ***List,
            int (*Selector)(struct direct *), int (*Sorter)())
{
  DIR *Dp;
  struct direct **names, *E;
  int i, size;

  /* Get initial list space and open directory. */
  size = INITIAL_SIZE;

  if ( (names = malloc(size * sizeof names[0])) == NULL
       || (Dp = opendir(Name)) == NULL )
    return -1;

  /* Read entries in the directory. */
  for ( i = 0; E = readdir(Dp); )
    if ( Selector == NULL || (*Selector)(E) )
    {
      /* User wants them all, or he wants this one. */
      if (++i >= size)
      {
        size <<= 1;
        names = realloc(names, size * sizeof names[0]);

        if (names == NULL)
        {
          closedir(Dp);
          return -1;
        }
      }

      /* Copy the struct direct. */
      if ( (names[i - 1] = malloc(sizeof(struct direct))) == NULL)
      {
        closedir(Dp);
        return -1;
      }

      *names[i - 1] = *E;
    }

  /* Close things off. */
  names[i] = NULL;
  *List = names;
  closedir(Dp);

  /* Sort? */
  if (i && Sorter)
    qsort((char *)names, i, sizeof names[0], Sorter);

  return i;
}
