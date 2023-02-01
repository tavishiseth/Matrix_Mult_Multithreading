#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include <sys/shm.h>

#define no_of_threads 1
int N;

int offsets[100000];
int a,i0=0,i1;
int shmid1;
int shmid2;
char filename1[100], filename2[100];

struct arguments
{
  long int rows, columns, idx;
  FILE *file;
  int *arr_ptr;
};


int min(int i, int j)
{
    if(i < j)
        return i;
    else 
        return j;
}

pthread_mutex_t lock,lock1;
void * runner (void *arg)
{
  pthread_mutex_lock (&lock);
  struct arguments *Arg = (struct arguments *) arg;

  int *q = Arg->arr_ptr;

  int no_of_rows_per_thread;

  if (Arg->idx == N - 1)
    no_of_rows_per_thread = Arg->rows / N + (Arg->rows % N);
  else
    no_of_rows_per_thread = Arg->rows / N;

  for (int i = 0; i < Arg->columns * no_of_rows_per_thread; i++)
    {
      fscanf (Arg->file, "%d", &a);
      q[i] = a;
    }
  pthread_mutex_unlock (&lock);
  return (void *) q;
}

void p1 (char *filename, int r, int c, int idx)
{
  char a;
  FILE *fp;
  offsets[++idx] = ftell (fp = fopen (filename, "r"));

  while (fscanf (fp, "%c", &a) != EOF)
    {
      if (a == '\n')
	offsets[++idx] = ftell (fp);
    }
  FILE *file[N];
  pthread_t p[N];

  struct arguments arg[N];
  for (int i = 0; i < N; i++)
    {
      file[i] = fopen (filename, "r");
      arg[i].rows = r;
      arg[i].columns = c;
      arg[i].arr_ptr = (int *) malloc (100000 * sizeof (int));
      arg[i].idx = i;
      fseek (file[i], offsets[i * (r / N)], SEEK_SET);
      arg[i].file = file[i];
      
    }
  pthread_mutex_init (&lock, NULL);
  for (int i = 0; i < N; i++)
    pthread_create (&p[i], NULL, runner, (void *) &arg[i]);
  int t;
  if (strcmp (filename, filename1) == 0)
    {
      int s = -1, q = 0;
      shmid1 = shmget (2041, sizeof (int[r][c]), 0666 | IPC_CREAT);
      int (*shmptr1)[c];
      shmptr1 = shmat (shmid1, NULL, 0);
      for (int i = 0; i < N; i++)
	{
	  if (i == N - 1)
	    t = r / N + (r % N);
	  else
	    t = r / N;

	  void *rv;
	  pthread_join (p[i], &rv);
	  printf ("\nhi from thread %d\n", i);


	  int *data = rv;
	  for (int i = 0; i < c * t; i++)
	    {
	      //printf("%d ", *data);
	      if (q == 0)
		{
		  s++;
		  shmptr1[s][q] = *data;
		}
	      else
		{
		  shmptr1[s][q] = *data;
		}
	      q = (q + 1) % c;
	      printf ("%d ", *data);
	      data++;
	    }

	}
      for (int i = 0; i < r; i++)
	{
	  printf ("\n\n");
	  for (int j = 0; j < c; j++)
	    printf ("%d ", shmptr1[i][j]);
	}
    }
  else if (strcmp (filename, filename2) == 0)
    {
      int s = -1, q = 0;
      shmid2 = shmget (2042, sizeof (int[r][c]), 0666 | IPC_CREAT);
      int (*shmptr2)[c];
      shmptr2 = shmat (shmid2, NULL, 0);
      for (int i = 0; i < N; i++)
	{
	  if (i == N - 1)
	    t = r / N + (r % N);
	  else
	    t = r / N;

	  void *rv;
	  pthread_join (p[i], &rv);
	  printf ("\nhi from thread %d\n", i);


	  int *data = rv;
	  for (int i = 0; i < c * t; i++)
	    {
	      //printf("%d ", *data);
	      if (q == 0)
		{
		  s++;
		  shmptr2[s][q] = *data;
		}
	      else
		{
		  shmptr2[s][q] = *data;
		}
	      q = (q + 1) % c;
	      printf ("%d ", *data);
	      data++;
	    }

	}
      for (int i = 0; i < r; i++)
	{
	  printf ("\n\n");
	  for (int j = 0; j < c; j++)
	    printf ("%d ", shmptr2[i][j]);
	}
      pthread_exit (NULL);
    }


}

int i, j, k;
int matC[10000][10000];
int matA[10000][10000];
int matB[10000][10000];
void *multi (void *arg)
{
  pthread_mutex_lock (&lock1);
  int h = (int*) arg;
  int t;
  if (h == N - 1)
	    t = i / N + (i % N);
	  else
	    t = i / N;
  for(int l=0;l<t;l++)
  {
    i1=i0++;
    for (int a = 0; a < k; a++)
    {
        for (int b = 0; b < j; b++)
        {
            matC[i1][a] += matA[i1][b] * matB[b][a];
        } 
    }
  }
  pthread_mutex_unlock (&lock1);
}

int main (int argc, char *argv[])
{
  i = atoi (argv[1]);
  j = atoi (argv[2]);
  k = atoi (argv[3]);
  strcpy (filename1, argv[4]);
  strcpy (filename2, argv[5]);
  if(no_of_threads > min(i,j))
    N=min(i,j);
  else
    N=no_of_threads;
    
  pid_t r = fork ();
  if (r == 0)
    {
      p1 (filename1, i, j, -1);
      p1 (filename2, j, k, -1);
    }
  else if (r != 0)
    {
      sleep (20);
      shmid1 = shmget (2041, sizeof (int[i][j]), 0);
      int (*shmptr1)[j];
      shmptr1 = shmat (shmid1, NULL, 0);

      shmid2 = shmget (2042, sizeof (int[j][k]), 0);
      int (*shmptr2)[k];
      shmptr2 = shmat (shmid2, NULL, 0);
      
      for (int a = 0; a < i; a++)
	{
	  for (int b = 0; b < j; b++)
	   matA[a][b]=shmptr1[a][b];
	}
    
    for (int a = 0; a < j; a++)
	{
	  for (int b = 0; b < k; b++)
	   matB[a][b]=shmptr2[a][b];
	}
    
    
      if(no_of_threads > i)
        N=i;
      else
        N=no_of_threads;

      pthread_t threads[N];
        pthread_mutex_init (&lock1, NULL);
        
      for (int i = 0; i < N; i++)
	{
	  pthread_create (&threads[i], NULL, multi, (void *)i);
	}

      for (int i = 0; i < N; i++)
	    pthread_join (threads[i], NULL);

      printf ("\nMultiplication\n");
      FILE *FP = fopen (argv[6], "w");

      for (int a = 0; a < i; a++)
	{
	  for (int b = 0; b < k; b++)
	    fprintf (FP, "%d ", matC[a][b]);
	  fprintf (FP, "\n");
	}
    }

}
