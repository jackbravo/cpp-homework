
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objdef.h"

class triangle_t : public myshape_t
{
  private:
  int side;
  public:
  int area(void) {
      int area = side*3;
      printf("My area is: %d\n", area);
      return area;
  };
  void whoami(void) {
      printf("I'm a triangle\n");
  };
  triangle_t(int);
  ~triangle_t(void);
};

/***********************************************/

triangle_t::triangle_t(int ext_side)
{
  side = ext_side;
  printf("This is a triangle\n");
} 

/***********************************************/

triangle_t::~triangle_t(void)
{
}

/***********************************************/

extern "C" myshape_t *myfunc(int side)
{
  triangle_t *myp = new triangle_t(side);
  return myp;
}

/***********************************************/


