#include <sys/types.h>

typedef struct Line_Record
{
    off_t offset;
    unsigned length;
} Line_Record;

int build_search_table(int file_descriptor, Line_Record *search_table, unsigned max_size);
int print_table_console(int file_descriptor, Line_Record *search_table, unsigned search_table_size);
void print_line(int file_descriptor, Line_Record line_record);