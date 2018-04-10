#include <stdio.h>
#include <stdlib.h> // exit
#include <errno.h>  // error codes
#include <fcntl.h>  // open
#include <unistd.h> // lseek

#define LINES_COUNT_MAX 100
#define END_LINE_NUMBER 0

typedef struct Line_Record
{
    off_t offset;
    unsigned length;
} Line_Record;

int build_search_table(int file_descriptor, Line_Record *search_table);
int request_processing(int file_descriptor, Line_Record *search_table, unsigned search_table_size);
void print_line(int file_descriptor, Line_Record line_record);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s [file]\n", argv[0]);
        return EINVAL;
    }

    Line_Record search_table[LINES_COUNT_MAX + 1];

    int file_descriptor = open(argv[1], O_RDONLY | O_NDELAY);
    if (file_descriptor == -1)
    {
        perror(argv[0]);
        return EIO;
    }

    int search_table_size = build_search_table(file_descriptor, search_table);
    return request_processing(file_descriptor, search_table, search_table_size);
}

// Returns size of builed search table.
int build_search_table(int file_descriptor, Line_Record *search_table)
{
    int current_line = 1;
    int current_line_position = 0;
    search_table[1].offset = 0L;

    while (1)
    {
        char current_char;
        int read_return_code = read(file_descriptor, &current_char, 1);

        if (read_return_code == -1)
        {
            perror("Reading Error: ");
            exit(EXIT_FAILURE);
        }
        if (read_return_code == 0)
        { // EOF
            break;
        }

        if (current_line > LINES_COUNT_MAX + 1)
        {
            fprintf(stderr, "Error: file have more than %d lines\n", LINES_COUNT_MAX);
            exit(EXIT_FAILURE);
        }

        current_line_position++;

        if (current_char != '\n')
        {
            continue;
        }

        off_t current_line_offset = lseek(file_descriptor, 0L, SEEK_CUR);
        if (current_line_offset == -1)
        {
            perror("Lseek Error: ");
            exit(EXIT_FAILURE);
        }

        search_table[current_line].length = current_line_position;
        printf("%d %d\n", search_table[current_line].offset, search_table[current_line].length);
        search_table[current_line + 1].offset = current_line_offset;
        current_line++;
        current_line_position = 0;
    }

    return current_line - 1;
}

// Loop proccessing of user input.
// Prints line by its number (starts form 1).
int request_processing(int file_descriptor, Line_Record *search_table, unsigned search_table_size)
{
    printf("Lines range: [%d, %d]\n", 1, search_table_size);
    do
    {
        printf("\n$ ");
        int line_number;
        scanf("%d", &line_number);

        if (line_number == END_LINE_NUMBER)
        {
            break;
        }
        if (line_number < 0 || line_number > search_table_size)
        {
            fprintf(stderr, "Out Of Range Error: lineNumber %d out of range [%d, %d]\n",
                    line_number, 1, search_table_size);
            continue;
        }

        print_line(file_descriptor, search_table[line_number]);
    } while (1);

    return EXIT_SUCCESS;
}

void print_line(int file_descriptor, Line_Record line_record)
{
    lseek(file_descriptor, line_record.offset, SEEK_SET);

    for (unsigned i = 0; i < line_record.length; i++)
    {
        char current_char;
        if (read(file_descriptor, &current_char, 1) == -1)
        {
            perror("Reading Error: ");
            exit(EXIT_FAILURE);
        }

        if (write(STDOUT_FILENO, &current_char, 1) == -1)
        {
            perror("Writing Error: ");
            exit(EXIT_FAILURE);
        }
    }
}