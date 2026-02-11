#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  // printf("Hello lets learn something new today!\n");
  // printf("We are building a system command 'cp' today!!\n");

  if (argc < 3)
  {
    printf("Error 01: Invalid Number of arguements!\n");
    printf("Please enter the source and final destinations\n");
    return 1;
  }
  else if (argc > 3)
  {
    printf("Error 01: Invalid Number of arguements!\n");
    printf("Please enter only a source and a final destination!\n");
    return 1;
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
  // lets first go and check validity of the files

  // lets check if they even exist or not

  int chk_src = open(argv[1], O_RDONLY);
  if (chk_src < 0)
  {
    printf("Error 02: Source file not found.\n");
    return 1;
  }
  // printf("Source file found.\n")

  int chk_dest = open(argv[2], O_RDONLY);
  if (chk_dest < 0)
  {
    printf("Error 02: Destination not found.\n");
    return 1;
  }
  // printf("Destination file found.\n")

  // validation done for src and dest when botha re files
  // i want to make it work for fianl destination as folderas well
  // i willl hv to create a new file inside the dir using the same name and type
  // so i hv to scrap the name from source as well

  // next: make the copy loop using the buffer to copythe file to a file
  // i will implement the file creation inside a folder later

  return 0;
}