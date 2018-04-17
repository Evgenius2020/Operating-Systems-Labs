#include <unistd.h> // lseek
#include <stdio.h>  // perror
#include <stdlib.h> // exit
#include "Search_Table.h"

#define BUFFER_SIZE 200
#define END_LINE_NUMBER 0

char buffer[BUFFER_SIZE];

// Returns size of builed search table.
int build_search_table(int file_descriptor, Line_Record *search_table, unsigned max_size)
{
    int current_line = 1;
    int current_line_position = 0;
    search_table[1].offset = 0L;

    while (1)
    {
        int read_result = read(file_descriptor, buffer, BUFFER_SIZE);

        if (read_result == 0) // EOF
        {
            break;
        }
        if (read_result == -1)
        {
            perror("read() Error: ");
            exit(EXIT_FAILURE);
        }

        for (unsigned i = 0; i < read_result; i++)
        {
            current_line_position++;
            if (buffer[i] == '\n')
            {
                search_table[current_line].length = current_line_position - 1;
                if (current_line == max_size)
                {
                    printf("File is too long, readed first %d lines.\n", max_size);
                    return current_line - 1;
                }
                search_table[current_line + 1].offset = search_table[current_line].offset + current_line_position;
                current_line++;
                current_line_position = 0;
            }
        }
    }
    search_table[current_line].length = current_line_position;
    return current_line;
} 

void print_line(int file_descriptor, Line_Record line_record)
{
    lseek(file_descriptor, line_record.offset, SEEK_SET);

    for (unsigned i = 0; i < line_record.length; i += BUFFER_SIZE)
    {
        unsigned bytes_to_read =
            (BUFFER_SIZE > line_record.length - i) ? line_record.length - i : BUFFER_SIZE;
        if (read(file_descriptor, buffer, bytes_to_read) == -1)
        {
            perror("Reading Error: ");
            exit(EXIT_FAILURE);
        }

        if (write(STDOUT_FILENO, buffer, bytes_to_read) == -1)
        {
            perror("Writing Error: ");
            exit(EXIT_FAILURE);
        }
    }
}

// Loop proccessing of user input.
// Prints line by its number (starts form 1).
int print_table_console(int file_descriptor, Line_Record *search_table, unsigned search_table_size)
{
    printf("Lines range: [%d, %d]\n", 1, search_table_size);
    do
    {
        printf("$ ");
        int line_number;
        scanf("%d", &line_number);

        if (line_number == END_LINE_NUMBER)
        {
            break;
        }
        if (line_number < 0 || line_number > search_table_size)
        {
            fprintf(stderr, "Out Of Range Error: line_number %d out of range [%d, %d]\n",
                    line_number, 1, search_table_size);
            continue;
        }

        print_line(file_descriptor, search_table[line_number]);
        putchar('\n');
    } while (1);

    return EXIT_SUCCESS;
}