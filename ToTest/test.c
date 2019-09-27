#include <stdio.h>
#include <sys/syscall.h>
#include <errno.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <string.h>

int main(int argc , char** argv)
{
	int pid = atoi(argv[1]);
	char path[100];
	strcpy(path , "/usr/src/linux-3.16-UPDATED/taskinfo/ToTest/");
	strcat(path , argv[2]);
	long int call = syscall(318 , pid , path);

	printf("\n\t-X-\t\n");

	if(call == 0)
	{
		printf("Task Struct Info Syscall Successful! \n");
		printf("Check info file for details. \n");
	}
	else
	{
		printf("Something Went Wrong\n");
		perror("Error ");
		printf("Error No. : %d\n" , errno);
	}

	printf("\t-X-\t\n\n");

	return 0;
}
