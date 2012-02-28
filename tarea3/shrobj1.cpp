
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objdef.h"

class square_t : public myshape_t
{
  private:
  int side;
  public:
  int area(void) {
      int area = side*4;
      printf("My area is: %d\n", area);
      return area;
  };
  void whoami(void) {
      printf("I'm a square\n");
  };
  square_t(int);
  ~square_t(void);
};

/***********************************************/

square_t::square_t(int ext_side)
{
  side = ext_side;
  printf("This is a square\n");
} 

/***********************************************/

square_t::~square_t(void)
{
}

/***********************************************/

extern "C" myshape_t *myfunc(int side)
{
  square_t *myp = new square_t(side);
  return myp;
}

/***********************************************/


