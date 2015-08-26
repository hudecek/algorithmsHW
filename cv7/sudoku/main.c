#include <stdio.h>
typedef unsigned char uc;

uc* firstEmpty(uc x, uc y, uc** array) {
  for(uc i=x;i<9;i++)
    for(uc j=y;j<9;j++)
      if(array[i][j] == 0)
        return &array[i][j];
  return NULL;
}

uc solve (uc x, uc y, uc** array) {

}

int main(void)
{
  uc array[9][9] = { 0 };
  for(uc i=0;i<9;i++)
    for(uc j=0;j<9;j++)
      scanf("%hhu", &array[i][j]);
  solve(0,0,array);

  return 0;
}

