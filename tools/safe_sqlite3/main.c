#include "sqlite3.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("Usage: ./sqlite3 [sqlite_filename] [key]\n");
    }
    sqlite3 *database = NULL;
    sqlite3_open(argv[1], &database);
    
    sqlite3_rekey(database,argv[2],strlen(argv[2]));
    sqlite3_close(database);
    database = NULL;
    
    return 0;
}