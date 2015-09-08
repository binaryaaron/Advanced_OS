struct unmthread {
  int mutex;
  int test;
}; /* This type should be fully defined in unmthread_priv.h */

struct {
  int signal;
} unmcond;

struct unmmutex;


unmthread_t *thread_create(int mutex, int test);
