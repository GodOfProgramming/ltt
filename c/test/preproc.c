#include <stdio.h>

#define Begin(test_name)			\
  int func_##test_name();			\
  int var_##test_name = func_##test_name();	\
  int func_##test_name() {

#define End()	\
    return 0;	\
  }

Begin(dummy_test);
printf("hello\n");
End();

int main() {
  return 0;
}
