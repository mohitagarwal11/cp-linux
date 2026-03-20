#include <stdio.h>
#include <fcntl.h>  //for flags O_RDONLY, O_RWONLY etc
#include <unistd.h> //for ssize_t and open write etc
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

// basic function to print the progress based on the total size and total written size
void print_progress_bar(size_t count, size_t max)
{
  const int bar_width = 50;
  float progress = (float)count / max;
  int bar_length = progress * bar_width;

  printf("\rProgress: [");
  for (int i = 0; i < bar_length; ++i)
  {
    printf("#");
  }
  for (int i = bar_length; i < bar_width; ++i)
  {
    printf(" ");
  }
  printf("] %.0f%%", progress * 100);
  fflush(stdout); // Flush the output buffer immediately
}

int main(int argc, char *argv[])
{
  // Allowing user to use flags in command line arguements like:
  // "-a" to append to the destiantion file and not overwrite

  // to know if append flag is raised or not
  int append = 0;
  // to store src argc
  char *src;
  // to store dest argc
  char *dest;

  if (argc == 4 && strcmp(argv[1], "-a") == 0)
  {
    append = 1;
    src = argv[2];
    dest = argv[3];
  }
  else if (argc == 3)
  {
    src = argv[1];
    dest = argv[2];
  }
  else
  {
    printf("\nInvalid number of arguements!\n");
    printf("\nUsage: %s <source> <destination>\n", argv[0]);
    printf("\nFLAGS: \n----- [-a] -- to append to destination file\n");
    return 1;
  }

  // this noly compares the strings entered literally and not logically
  // i will owkr on this later
  if (strcmp(src, dest) == 0)
  {
    printf("Copy interrupted.\n");
    printf("Same source and destination file.\n");
    return 1;
  }

  // to append we can use the O_APPEND flag inplace of the O_TRUNC for dest_fd
  // to store the flags to be used for dest
  int dest_flags = O_WRONLY | O_CREAT;

  if (append)
  {
    dest_flags |= O_APPEND;
  }
  else
  {
    dest_flags |= O_TRUNC;
  }

  // src_fd and dest_fd are file descriptors that are basically
  // index we get as return from open for kernal objects for our opened files
  // 0 -> stdin
  // 1 -> stdout
  // 2 -> stderr
  // 3rd index onwards arent generally used by the kernal in a C compiler
  // so we get those indexes for these opened files
  // these indexes point to the object that has the metadata of the said file
  // very interesting!

  int src_fd = open(src, O_RDONLY);
  if (src_fd < 0)
  {
    perror("open");
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

  int dest_fd = open(dest, dest_flags, 0644);
  if (dest_fd < 0)
  {
    perror("open");
    return 1;
  }

  // validation done for src and dest when both are files
  // and if file doesnt exist now we create one there itself usign the flag and code

  // 4096 is the bytes for one page file in linux
  // so 4kb blocks will be read at once frm the file
  // or one page at a time
  // helps reduce syscalls

  char buffer[4096];
  // ssize_t is Signed Size Type so it allows both pos and neg numbers
  // returns -1 for eof error
  // why not use int then?
  // cuz INT_MAX is 32 bit and ssize_t is 64bit
  // and open read write may exceed 32bit

  struct stat st;
  if (fstat(src_fd, &st) < 0)
  {
    perror("fstat");
    close(src_fd);
    close(dest_fd);
    return 1;
  }

  ssize_t total_bytes = st.st_size;
  if (total_bytes == 0)
  {
    printf("Source file is empty.\n");
    return 1;
  }
  ssize_t total_written = 0;

  ssize_t bytes_read;

  while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0)
  {
    ssize_t total_written_yet = 0;
    // now we are writing after the point it is already written
    // and writting only when we hv not written all read bytes
    while (total_written_yet < bytes_read)
    {
      ssize_t bytes_written = write(dest_fd,
                                    total_written_yet + buffer,
                                    bytes_read - total_written_yet);

      if (bytes_written < 0)
      {
        perror("write");
        close(src_fd);
        close(dest_fd);
        return 1;
      }

      // update total written
      total_written_yet += bytes_written;
      total_written += bytes_written;
      print_progress_bar(total_written, total_bytes);
    }
  }
  if (bytes_read < 0)
  {
    perror("read");
    close(src_fd);
    close(dest_fd);
    return 1;
  }

  // to apply to same mode as src to dest file
  if (fchmod(dest_fd, st.st_mode & 0777) < 0)
  {
    perror("fchmod");
    close(src_fd);
    close(dest_fd);
    return 1;
  }

  close(src_fd);
  close(dest_fd);

  printf("\nFile content copied.\n");
  return 0;
}
