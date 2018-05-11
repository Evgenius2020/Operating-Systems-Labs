#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>  // popen(), pclose(), fread(), fwrite(), stdout
#include <ctype.h>  // toupper()

#define COMMAND_TO_WRITE_TEXT "echo SoMe TeXt"
#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
    // ============= Opening pipe. ======================================================
    FILE *pipe = popen(COMMAND_TO_WRITE_TEXT, "r");
    if (pipe == NULL)
    {
        perror("popen");
        return EXIT_FAILURE;
    }
    // ----------------------------------------------------------------------------------

    while (1)
    {
        // ========= Reading from stdout of writing process. ============================
        char buf[BUFFER_SIZE];
        int bytes_read = fread(buf, 1, BUFFER_SIZE, pipe);
        if (bytes_read == -1)
        {
            perror("fread");
            return EXIT_FAILURE;
        }
        if (bytes_read == 0)
        {
            break;
        }
        // ------------------------------------------------------------------------------

        // ========= Converting to upper. ===============================================
        unsigned i;
        for (i = 0; i < bytes_read; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        // ------------------------------------------------------------------------------

        // ========= Writing into stdout. ===============================================
        if (fwrite(buf, bytes_read, 1, stdout) == -1)
        {
            perror("fwrite");
            return EXIT_FAILURE;
        }
        // ------------------------------------------------------------------------------
    }

    // ============= Closing pipe. ======================================================
    if (pclose(pipe) == -1)
    {
        perror("pclose");
        return EXIT_FAILURE;
    }
    // ----------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}