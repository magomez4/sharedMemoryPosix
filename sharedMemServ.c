#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	const int SIZE = 4096;
	const char *name = "shared_mem";
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); // create shared memory object
	ftruncate(shm_fd, 4096); // set size

	void *ptr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, shm_fd, 0); // map memory
	char stringBuffer[4000];
	while(fgets(stringBuffer, sizeof(stringBuffer), stdin) != NULL)
	{
		int len = strlen(stringBuffer);
		if(len > 0 && stringBuffer[len-1] == '\n')
		{
			stringBuffer[len-1] = '\0';
		}
		if(strlen(stringBuffer) == 0)
		{
			break;
		}
		else
		{
			sprintf(ptr, stringBuffer); // write to shared memory
		}
		//sprintf(ptr, "Hello, World!"); // write to shared memory
	}
	memset(ptr, 0, SIZE);
	*(int*)ptr = -1; // sentinel value to indicate server is done
//	sleep(3);
	munmap(ptr, 4096); // unmap
	close(shm_fd); // close the file descriptor
	shm_unlink(name); // remove the shared memory object

	return 0;
}

