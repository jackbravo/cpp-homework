

class myshape_t
{
public:
  virtual int area(void) = 0;
  virtual void whoami(void) = 0;
  virtual ~myshape_t(void){printf("base destructor\n");};
};

