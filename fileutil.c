#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *default_file_name = "sample.txt";
char *source_file_name = NULL;
char *target_file_name = NULL;
// write to standard output directly
// type == 1: write to standard output
// type == 2: append to target_file
void print_char(char c, int fd)
{
	write(fd, &c, 1);
}

void print(char *fmt)
{
	for (int i = 0; i < strlen(fmt); i++)
	{
		print_char(fmt[i], STDOUT_FILENO);
	}
}

// free memory allocated for string
void free_memory()
{
	if (source_file_name != NULL)
		free(source_file_name);
	if (target_file_name != NULL)
		free(target_file_name);
}

// After memory allocated, copy source string to destination string
char *copy_string(char *dst, char *src)
{
	if (dst != NULL)
		free(dst);
	dst = malloc(strlen(src) + 1);
	strcpy(dst, src);
	return dst;
}

int main(int argc, char *argv[])
{
	// set default word_count to 10
	int word_count = 10;
	// -a option flag
	int a_option_flag = 0;
	// copy default_file_name to source_file_name
	source_file_name = copy_string(source_file_name, default_file_name);

	// start handling the command line arguments
	if (argc >= 2)
	{
		//  the first argument must be the source file path argument if it is not started with '-'
		if (argv[1][0] != '-')
		{
			source_file_name = copy_string(source_file_name, argv[1]);
		}
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-a") == 0)
			{
				if (i + 1 < argc)
				{
					target_file_name = copy_string(target_file_name, argv[i + 1]);
					a_option_flag = 1;
					i++;
				}
				else
				{
					print("Invalid argument for -a option.\n");
					free_memory();
					return 1;
				}
			}
			else if (strcmp(argv[i], "-n") == 0)
			{
				if (i + 1 < argc)
				{
					word_count = atoi(argv[i + 1]);
					i++;
				}
				else
				{
					print("Invalid argument for -n option.\n");
					free_memory();
					return 1;
				}
			}
		}
	}

	// open source file
	int fd_src = open(source_file_name, O_RDONLY);
	int target_fd = STDOUT_FILENO;
	if (fd_src == -1)
	{
		print("Could not open file\n");
	}
	// start print or append specified number of words
	char ch;
	int count = 0, word_flag = 0; // word_flag indates that the ch is a part of the word or not
	// if -a option is specified, open the target_file
	if (a_option_flag == 1)
	{
		target_fd = open(target_file_name, O_WRONLY | O_CREAT);
	}
	while (read(fd_src, &ch, 1) == 1)
	{
		if (ch == ' ' || ch == '\t' || ch == '\n')
		{
			if (word_flag == 1)
			{
				count++;
				word_flag = 0;
				if (count < word_count)
					print_char(' ', target_fd);
			}
		}
		else
		{
			if (word_flag == 0)
				word_flag = 1;
			print_char(ch, target_fd);
		}
		if (count == word_count)
		{
			// if user input the -a option
			if (a_option_flag == 1)
			{
				print("Append successful\n");
			}
			break; // end
		}
	}

	// Over handling
	print("\n");
	// free memory
	free_memory();
	// Close the file
	close(fd_src);
	return 0;
}