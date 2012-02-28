#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <math.h>
#include "objdef.h"
                                                                                
extern "C" {
typedef double (*dfn_t)(double x);
typedef myshape_t *(*cfn_t)(int x);
}

/**************************/

int main(int argc, char *argv[]) 
{
  void *handle;
  dfn_t cosine;
  cfn_t cfn;
  char *error;
                                                                              
  char *sofile = "./shrobj1.so";
  if (argc > 1)
  {
    sofile = argv[1];
  }

  handle = dlopen (sofile, RTLD_LAZY);
  if (!handle) 
  {
    fprintf (stderr, "%s\n", dlerror());
    exit(1);
  }

  cfn = (cfn_t) dlsym(handle, "myfunc");
  if ((error = dlerror()) != NULL)  
  {
    fprintf (stderr, "%s\n", error);
    exit(1);
  }

  myshape_t *mybasep = (*cfn)(4.0);
  int area = mybasep->area();
  mybasep->whoami();
  delete mybasep;
  dlclose(handle);

  
  char *sofile2 = "./shrobj2.so";
  if (argc > 1)
  {
    sofile = argv[1];
  }

  handle = dlopen (sofile2, RTLD_LAZY);
  if (!handle) 
  {
    fprintf (stderr, "%s\n", dlerror());
    exit(1);
  }

  cfn = (cfn_t) dlsym(handle, "myfunc");
  if ((error = dlerror()) != NULL)  
  {
    fprintf (stderr, "%s\n", error);
    exit(1);
  }

  myshape_t *mybasep2 = (*cfn)(4.0);
  area = mybasep2->area();
  mybasep2->whoami();
  delete mybasep2;
  dlclose(handle);

  return 0;
}


