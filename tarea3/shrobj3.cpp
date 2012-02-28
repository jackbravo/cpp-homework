
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objdef.h"

class nuevo_derived_t : public mybase_t
{
  private:
  char *str;
  public:
  void myprint(void) {printf("Esto es que hicimos 28-02-2008\n");};
  nuevo_derived_t(void);
  ~nuevo_derived_t(void);
};

/**********************************/

nuevo_derived_t::nuevo_derived_t(void)
{
  printf("in the constructor for type 3\n");
  str = strdup("testing 3");
} 

/**********************************/

nuevo_derived_t::~nuevo_derived_t(void)
{
  printf("Go ahead, KILL me . . . . \n");
  printf("destroying: %s\n", str);
  free(str); 
}

/**********************************/

extern "C" mybase_t *myfunc(void)
{
  printf("Hello, I am constructing type 3 for you \n");
    
  nuevo_derived_t *myp = new nuevo_derived_t;
  return myp;
}

/**********************************/


