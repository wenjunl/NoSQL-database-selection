#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define FILE_DIR "/home/wenjun/result_data/"

#define NANO (1000000000)
#define MAX 1024
#define PID 6
#define PATH_SIZE 128

static int flag = 0;
static struct timespec start = {0, 0L};
static struct timespec end = {0, 0L};

//去除尾部空格
char *rtrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str  && isspace(*p))
	{
		*p = '\0';
		--p;
	}
 
	return str;
}
 
//去除首部空格
char *ltrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = 0;
	char *p = str;
	while (*p != '\0' && isspace(*p))
	{
		++p;
		++len;
	}
 
	memmove(str, p, strlen(str) - len + 1);
 
	return str;
}
 
//去除首尾空格
char *trim(char *str)
{
	str = rtrim(str);
	str = ltrim(str);
	
	return str;
}

void dbtime_start ()
{
	if (flag)
	{
		fprintf (stderr, "error\n");
		exit (0);
	}

	flag = 1;
	//clock_gettime (CLOCK_REALTIME, &start);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);
	return;
}

void dbtime_end()
{
	if (!flag)
	{
		fprintf (stderr, "error\n");
		exit (0);
	}
	flag = 0;
	//clock_gettime (CLOCK_REALTIME, &end);
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	return;
}

double dbtime_show()
{
	struct timespec ltime;
	double us, ms, s;
	if (flag)
	{
 		fprintf (stderr, "error\n");
		exit (0);
	}
	if (end.tv_nsec <= start.tv_nsec)
	{
		ltime.tv_nsec = NANO + end.tv_nsec - start.tv_nsec;
		ltime.tv_sec = end.tv_sec - start.tv_sec - 1;
	}
	else 
	{
		ltime.tv_nsec = end.tv_nsec - start.tv_nsec;
		ltime.tv_sec = end.tv_sec - start.tv_sec;
	}

	// pretty-printing it
	us = ltime.tv_nsec * 1.0 / 1000;
	ms = ltime.tv_nsec * 1.0 / 1000000;
	s = ltime.tv_nsec * 1.0 / NANO;

	// to shut up the compiler
	us = us;
	ms = ms;
  double total_time = (int)ltime.tv_sec + s;
	printf ("%lf\n", total_time);
	return total_time;
}

void write_result(char *file_name)
{
  double total_time = dbtime_show();
  char file_path[1024] = FILE_DIR;
  strcat(file_path, file_name);
  FILE *fp = fopen(file_path, "a");
  fprintf(fp, "%f,", total_time);
  fclose(fp);
}

void write_vm_message(char *msg, char *prefix, char *postfix)
{
	// int buf_len = strlen(buf);
	// char msg[20] = {0};
	// int index = 0;
	// for (int i=0; i<buf_len; i++)
	// {
	// 	char c = buf[i];
	// 	if (c >= '0' && c<= '9') {
	// 		msg[index] = c;
	// 		index ++;
	// 	}
	// }

	char file_name[30] = {0};
	strcpy(file_name, prefix);
	strcat(file_name, postfix);

	char file_path[1024] = FILE_DIR;
	strcat(file_path, file_name);
	FILE *fp = fopen(file_path, "a");
	fprintf(fp, "%s,", msg);
	fclose(fp);
}


void memory_message(char *prefix)
{
	FILE *fp;
	pid_t pid;
	char pid_str[PID];
	char path[PATH_SIZE];
	char pathC[PATH_SIZE];
	char buf[MAX];

	pid = getpid();
	sprintf(pid_str, "%d", pid);
	strcpy(path, "/proc/self/task/");
	strcat(path, pid_str);
	strcpy(pathC, path);
	strcat(path, "/status");
	strcat(pathC, "/cmdline");

	fp = fopen(pathC, "r");
	if (fp == NULL){
		perror("fail to open");
		exit(1);
	}

	while(fgets(buf, MAX, fp) != NULL){
		printf("%s\n", buf);
	}

	fp = fopen(path, "r");
		if(fp == NULL){
		perror("fail to open");
		exit(1);
	}

	while(fgets(buf, MAX, fp) != NULL){
		
		if (strncmp(buf, "VmSize", 6) == 0) 
		{
			char dest[20] = {0};
			int str_len = strlen(buf);
			strncpy(dest, buf+7,str_len-10);
			char* msg = trim(dest);
			write_vm_message(msg, prefix, "vmsize.txt");
			printf("%s", buf);
		}
			
		if (strncmp(buf, "VmRSS", 5) == 0)
		{
			char dest[20] = {0};
			int str_len = strlen(buf);
			strncpy(dest, buf+6,str_len-9);
			char* msg = trim(dest);
			write_vm_message(msg, prefix, "vmrss.txt");
			printf("%s", buf);
		}
		if (strncmp(buf, "VmData", 6) == 0)
		{
			char dest[20] = {0};
			int str_len = strlen(buf);
			strncpy(dest, buf+7,str_len-10);
			char* msg = trim(dest);
			write_vm_message(msg, prefix, "vmdata.txt");
			printf("%s", buf);
		}
		if (strncmp(buf, "VmStk", 5) == 0)
		{
			char dest[20] = {0};
			int str_len = strlen(buf);
			strncpy(dest, buf+6,str_len-9);
			char* msg = trim(dest);
			write_vm_message(msg, prefix, "vmstk.txt");
			printf("%s", buf);
		}

	}

	fclose(fp);

}
