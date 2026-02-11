#include <stdio.h>

int main(int argc, char *argv[])
{
  // printf("Hello lets learn something new today!\n");
  // printf("We are building a system command 'cp' today!!\n");

  if (argc < 3)
  {
    printf("Error 01: Invalid Number of arguements!\n");
    printf("Please enter the source and final destinations\n");
    return 0;
  }
  else if (argc > 3)
  {
    printf("Error 01: Invalid Number of arguements!\n");
    printf("Please enter only a source and a final destination!\n");
  }
  // else
  // {
  //   printf("%s\n", argv[1]);
  //   printf("%s\n", argv[2]);
  // }

  // I hv the two arguements
  // next:
  // now i need to copy byte by byte from first to second source
  // ik i need a char buffer for it
  // size of the buffer i am not sure
  // i also need to check if the sources exists or not even

  return 0;
}