#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t my_mutex;
static int tab[5];

void *read_tab_process(void * arg)
{
  int i;

  pthread_mutex_lock (&my_mutex);
  for (i = 0 ; i != 5 ; i++)
    printf ("read_process, tab[%d] vaut %d\n", i, tab[i]);
  pthread_mutex_unlock (&my_mutex);
  pthread_exit (0);
}

void *write_tab_process(void * arg)
{
  int i;

  pthread_mutex_lock(&my_mutex);
  for (i = 0 ; i != 5 ; i++) {
    tab[i] = 2 * i;
    printf ("write_process, tab[%d] vaut %d\n", i, tab[i]);
  }
  pthread_mutex_unlock(&my_mutex);
  pthread_exit (0);
}

int main(int ac, char **av)
{
  pthread_t th1, th2;
  void *ret;

  pthread_mutex_init(&my_mutex, NULL);

  if (pthread_create(&th1, NULL, write_tab_process, NULL) < 0) {
    fprintf (stderr, "pthread_create error for thread 1\n");
    exit (1);
  }

  if (pthread_create(&th2, NULL, read_tab_process, NULL) < 0) {
    fprintf (stderr, "pthread_create error for thread 2\n");
    exit (1);
  }

  (void)pthread_join(th1, &ret);
  (void)pthread_join(th2, &ret);
  return (0);
}
