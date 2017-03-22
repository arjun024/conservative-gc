#include <iostream>
using namespace std;

#include "gcmalloc.hh"

int main()
{
  cout << "-----------" << endl;
  for (int i = 0; i < 10; i++) {
    char * ptr = (char *) malloc(171);
    if (!ptr) {
      printf("%s\n", "malloc returned NULL");
      return 0;
    }
    *ptr = 'a' + i;
    *(ptr + 1) = '\0';
    cout << ptr << endl;
    /* printf("%p\n", (void*)ptr); */
    /* free(ptr); */
  }

  walk([&](Header * h){ cout << "Object address = " << (void *) (h + 1) << ", allocated size = " << h->getAllocatedSize() << endl; });
  return 0;
}
