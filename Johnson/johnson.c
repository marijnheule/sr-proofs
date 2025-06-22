#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int edge (int a, int b) {
  assert (a != b);
  assert (a >  0);
  assert (b >  0);
  int min, max;
  min = a; max = b;
  if (min > max) { min = b; max = a; }

  return (max - 2) * (max - 1) / 2 + min; }

void swap (int center, int vertex, int n, FILE* out) {
  int* map = (int *) malloc (sizeof (int) * (n+1));
  for (int i = 0; i <= n; i++) map[i] = i;

  for (int i = center + 1; i <= vertex; i++)
    map[i] = i-1;
  map[center+1] = vertex;

  for (int j = center+1; j <= n; j++)
    for (int i = center+1; i < j; i++)
      if (edge (i,j) != edge (map[i],map[j]))
        fprintf (out, "%i %i ", edge (i,j), edge (map[i],map[j]));
}

void swapx (int center, int vertex, int n, FILE* out) {
  int* map = (int *) malloc (sizeof (int) * (n+1));
  for (int i = 0; i <= n; i++) map[i] = i;

  map[4] = 5;
  map[5] = 6;
  map[6] = 4;

  for (int j = center+1; j <= n; j++)
    for (int i = center+1; i < j; i++)
      if (edge (i,j) != edge (map[i],map[j]))
        fprintf (out, "%i %i ", edge (i,j), edge (map[i],map[j]));
}

void swapy (int center, int vertex, int n, FILE* out) {
  int* map = (int *) malloc (sizeof (int) * (n+1));
  for (int i = 0; i <= n; i++) map[i] = i;

  map[3] = 4;
  map[4] = 5;
  map[5] = 6;
  map[6] = 3;

  for (int j = center+1; j <= n; j++)
    for (int i = center+1; i < j; i++)
      if (edge (i,j) != edge (map[i],map[j]))
        fprintf (out, "%i %i ", edge (i,j), edge (map[i],map[j]));
}

int main (int argc, char** argv) {
  int n = atoi (argv[1]);

  FILE* cnf;
  cnf = fopen ("tmp.cnf", "w");
/*
  fprintf (cnf, "p cnf %i %i\n", n * (n-1) / 2, n * (n-1) * (n-2) / 3);
  for (int i = 1; i <= n; i++)
    for (int j = i+1; j <= n; j++)
      for (int k = j+1; k <= n; k++) {
        fprintf (cnf, " %i  %i  %i 0\n", edge (i,j), edge (i,k), edge (j,k));
        fprintf (cnf, "-%i -%i -%i 0\n", edge (i,j), edge (i,k), edge (j,k)); }
*/
  fprintf (cnf, "p cnf %i %i\n", n * (n-1) / 2, n * (n-1) * (n-2) * (n-3) / 12);
  for (int i = 1; i <= n; i++)
    for (int j = i+1; j <= n; j++)
      for (int k = j+1; k <= n; k++)
        for (int l = k+1; l <= n; l++) {
          fprintf (cnf, " %i  %i  %i  %i  %i  %i 0\n", edge (i,j), edge (i,k), edge (j,k), edge (i,l), edge (j,l), edge (k,l));
          fprintf (cnf, "-%i -%i -%i -%i -%i -%i 0\n", edge (i,j), edge (i,k), edge (j,k), edge (i,l), edge (j,l), edge (k,l)); }

  fclose (cnf);

  FILE *sr;

  sr = fopen ("tmp.sr", "w");

  for (int i = 2; i < n; i++) {
    fprintf (sr, "%i -%i %i -%i %i ", edge (1,i+1), edge (1,i), edge (1,i+1), edge (1,2), edge (1,i+1));

    for (int j = i; j > 2; j--)
      fprintf (sr, "%i %i ", edge (1,j), edge (1,j-1));

    swap (1, i+1, n, sr);

    fprintf (sr, "0\n");
  }

  fprintf (sr, "%i -%i %i %i -%i %i ", edge (2,4), edge (2,3), edge (1,4), edge (2,4), edge (2,3), edge (2,4));
  swap (2, 4, n, sr);
  fprintf (sr, "0\n");

  fprintf (sr, "%i -%i %i %i -%i %i ", edge (2,5), edge (2,4), edge (1,5), edge (2,5), edge (2,3), edge (2,5));
  fprintf (sr, "%i %i ", edge (2,4), edge (2,3));
  swap (2, 5, n, sr);
  fprintf (sr, "0\n");

  fprintf (sr, "%i -%i %i %i -%i %i ", edge (2,6), edge (2,5), edge (1,6), edge (2,6), edge (2,3), edge (2,6));
  fprintf (sr, "%i %i ", edge (2,4), edge (2,3));
  fprintf (sr, "%i %i ", edge (2,5), edge (2,4));
  swap (2, 6, n, sr);
  fprintf (sr, "0\n");


  fprintf (sr, "-%i %i -%i -%i %i -%i ", edge (2,5), edge (2,6), edge (1,5), edge (2,5), edge (2,6), edge (2,5));
  fprintf (sr, "%i %i %i %i ", edge (3,5), edge (3,6), edge (3,6), edge (3,5));
  fprintf (sr, "%i %i %i %i ", edge (4,5), edge (4,6), edge (4,6), edge (4,5));
  fprintf (sr, "0\n");

  fprintf (sr, "-%i %i -%i -%i %i -%i ", edge (2,4), edge (2,5), edge (1,4), edge (2,4), edge (2,6), edge (2,4));
  fprintf (sr, "%i %i ", edge (2,5), edge (2,6));
  swapx (2, 6, n, sr);
  fprintf (sr, "0\n");

  fprintf (sr, "-%i %i -%i -%i %i -%i ", edge (2,3), edge (2,4), edge (1,3), edge (2,3), edge (2,6), edge (2,3));
  fprintf (sr, "%i %i ", edge (2,4), edge (2,5));
  fprintf (sr, "%i %i ", edge (2,5), edge (2,6));
  swapy  (2,6,n,sr);
  fprintf (sr, "0\n");


  fprintf (sr, "%i 0\n", edge (1,4));
  fprintf (sr, "0\n");

  fclose (sr);


}
