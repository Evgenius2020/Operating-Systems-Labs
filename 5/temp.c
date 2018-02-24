#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_STR 1000
#define ERROR_CODE -1

#define EXIT_NUMBER 0

#define true 1
int std_out = 1;

int not_end_of_file = 1;

int makeTable(int fd, long* lines_indents_in_file, int* lines_lengths);
int linesNumbersProcessing(int fd, int lines_in_file, long* lines_indents_in_file, int* lines_lengths);
int printLine(int fd, int line_length);

int main(int argc, char* argv[])
{
	if (argc < 2) {
                fprintf(stderr,"usage: %s [file]\n", argv[0]);
                return EINVAL;
        }

	long lines_indents_in_file[MAX_STR + 1];
	int lines_lengths[MAX_STR + 1];

	int fd;
	int lines_in_file;
	

	if ((fd = open(argv[1], O_RDONLY | O_NDELAY)) == ERROR_CODE) {
		perror(argv[0]);
		return EIO;
	}

	lines_in_file = makeTable(fd, lines_indents_in_file, lines_lengths);

	linesNumbersProcessing(fd, lines_in_file, lines_indents_in_file, lines_lengths);

	return EXIT_SUCCESS;
}


int makeTable(int fd, long* lines_indents_in_file, int* lines_lengths)
{
        char reading_symb;
        int current_file_line = 1;
        int current_position_inline = 0;
	int bytes_read;

	int line_indent;
        lines_indents_in_file[1] = 0L;

	while (not_end_of_file) {
                bytes_read = read(fd, &reading_symb, 1);
                if (bytes_read == ERROR_CODE) {
                        perror("Reading from file error");
                        exit(EXIT_FAILURE);
                }

		if (bytes_read == 0)
			break;

		if (current_file_line > MAX_STR + 1) {
                        fprintf(stderr, "File have more than %d lines\n", MAX_STR);
                        exit(EXIT_FAILURE);
                }

                current_position_inline++;

                if (reading_symb != '\n') {
                        continue;
                }

                lines_lengths[current_file_line++] = current_position_inline;
                line_indent = lseek(fd, 0L, 1);

                if (line_indent == ERROR_CODE) {
                        perror("File move error");
                        exit(EXIT_FAILURE);
                }

                lines_indents_in_file[current_file_line] = line_indent;
                current_position_inline = 0;
        } 
        return current_file_line;
}

int linesNumbersProcessing(int fd, int lines_in_file, long* lines_indents_in_file, int* lines_lengths)
{
        int line_number;
	char end_symbol;
	int variables_read;

	printf("Print lines in range from 1 to %d\n", lines_in_file - 1);
	printf("Line number : ");
	variables_read = scanf("%d%c", &line_number, &end_symbol);

        while (variables_read == 2) {

		if (end_symbol != '\n') {
			fprintf(stderr, "Data format error\n");
			exit(EXIT_FAILURE);
		}

                if (line_number == EXIT_NUMBER)
                        return EXIT_SUCCESS;

                if (line_number < 0 || line_number >= lines_in_file) {
			fprintf(stderr, "Line number must be in the range from 1 to %d\n", lines_in_file - 1);
	        
		        printf("Line number : ");
        	        variables_read = scanf("%d%c", &line_number, &end_symbol);
                        continue;
                }

                if (lseek(fd, lines_indents_in_file[line_number], 0) == ERROR_CODE) {
                        perror("File move error");
                        exit(EXIT_FAILURE);
                }

		printLine(fd, lines_lengths[line_number]);

		printf("Line number : ");
		variables_read = scanf("%d%c", &line_number, &end_symbol);
        }

        fprintf(stderr, "Data format error\n");
        exit(EXIT_FAILURE);
}


int printLine(int fd, int line_length)
{
	int position_in_line;
	int bytes_read;
	int bytes_wrote;

	char reading_symb;	

        for (position_in_line = 0; position_in_line < line_length; position_in_line++) {
                bytes_read = read(fd, &reading_symb, 1);

                if (bytes_read == ERROR_CODE) {
                      perror("Reading from file error");
                      exit(EXIT_FAILURE);
     	     	}
	
                bytes_wrote = write(std_out, &reading_symb, 1);

                if (bytes_wrote == ERROR_CODE) {
                      perror("Writing to stdout error");
       	              exit(EXIT_FAILURE);
            	}
      	}

}

