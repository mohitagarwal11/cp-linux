#include <stdio.h>
#include <fcntl.h>  //for flags O_RDONLY, O_RWONLY etc
#include <unistd.h> //for ssize_t and open write etc

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

  // src_fd and dest_fd are file descriptors that are basically
  // index we get as return from open for kernal objects for our opened files
  // 0 -> stdin
  // 1 -> stdout
  // 2 -> stderr
  // 3rd index onwards arent generally used by the kernal in a C compiler
  // so we get those indexes for these opened files
  // these indexes point to the object that has the metadata of the said file
  // very interesting!

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd < 0)
  {
    printf("Error 02: Source file not found.\n");
    return 1;
  }
  // printf("Source file found.\n")

  // we hv to write to this file so we use WRONLY and
  // CREAT flag will create the file if needed
  // i am not sure what trunc does tbh

  // but the code 0644 is an octal code for the file permissions
  // codeType-ownerPermission-groupPermission-othersPermission
  // 0 -> octal type code
  // 4 -> read
  // 2 -> write
  // 1 -> execute
  // so i wanted owner to read and write therefore 6 == 4+2
  // group only read so 4
  // same for others
  // i want to transfer the permissions from src to dest file
  // but i need to learn stat, mode and chmod so maybe next time

  int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  // int dest_fd = open(argv[2], O_RDONLY);
  if (dest_fd < 0)
  {
    printf("Error 02: Destination not found.\n");
    return 1;
  }
  // printf("Destination file found.\n")

  // printf("src_fd = %d\n", src_fd);
  // printf("dest_fd = %d\n", dest_fd);

  // validation done for src and dest when both are files
  // and if file doesnt exist now we create one there itself usign the flag and code
  // i want to make it work for final destination as folders as well later

  // next: make the copy loop using the buffer to copy the file to a file

  // 4096 is the bytes for one page file in linux
  // so 4kb blocks will be read at once frm the file
  // or one page at a time
  // helps reduce syscalls

  char buffer[4096];

  // ssize_t is Signed Size Type so it allows both pos and neg numbers
  // ned -1 for eof error
  // why not use int then?
  // cuz INT_MAX is 32 bit and ssize_t is 64bit
  // and open read write may exceed 32bit

  ssize_t bytes_read;

  while ((bytes_read = read(src_fd, buffer, 4096)) > 0)
  {
    ssize_t total_written_yet = 0;
    // now we are writing after the point it is already written
    // and writting only when we hv not written all read bytes
    while (total_written_yet < bytes_read)
    {
      ssize_t bytes_written = write(dest_fd,
                                    total_written_yet + buffer,
                                    bytes_read - total_written_yet);

      // update total written
      total_written_yet += bytes_written;
    }
  }
  close(src_fd);
  close(dest_fd);

  printf("File content copied.\n");
  return 0;
}