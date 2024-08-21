#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
	const char *name = "shared_mem";   // Name of the shared memory object
	const int SIZE = 4096;             // Size of the shared memory segment

	// Open the shared memory object
	int shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		perror("shm_open");
		exit(EXIT_FAILURE);
	}

	// Map the shared memory object into the process's address space
	void *ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	
	char buffer[SIZE];
	char prev[SIZE];
//	while(*(int*)((char*)ptr+4000) != -1)
	while(*(int*)(ptr) != -1)
	{
		// Read from the shared memory
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, (char*)ptr);
		// only print if there is new data
		if(strcmp(prev,buffer)!=0 && strlen(buffer)!=0)
		{
			memset(prev, 0, sizeof(prev));
			strcpy(prev, buffer);
			printf("Read COPIED from shared memory: %s\n", buffer);
		}
		//printf("Read from shared memory: %s\n", (char *)ptr);
	}
	
	// Clean up
	munmap(ptr, SIZE);  // Unmap the shared memory
	close(shm_fd);      // Close the file descriptor

	return 0;
}

