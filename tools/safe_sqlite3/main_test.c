#include "sqlite3.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Usage: ./sqlite3_test [sqlite_filename] [key] [tablename]\n");
        return;
    }
    sqlite3 *database = NULL;
    sqlite3_open(argv[1], &database);
    
    sqlite3_key(database,argv[2],strlen(argv[2]));
    char sql[100];
    sprintf(sql, "select * from %s", argv[3]);
    sqlite3_stmt *statement;
    int res = sqlite3_prepare_v2(database, sql, -1, &statement, NULL);
    int column_count = sqlite3_column_count(statement);
    printf("select %d columns:\n", column_count);
    int i;
    for (i = 0; i < column_count; ++i) {
        printf("%s\t", sqlite3_column_name(statement, i));
    }
    printf("\n");
    while (sqlite3_step(statement) == SQLITE_ROW) {
        for (i = 0; i < column_count; ++i) {
            printf("%s\t", sqlite3_column_text(statement, i));
        }
        printf("\n");
    }
    char* error;
    sqlite3_exec(database, sql, NULL, NULL, &error);
    
    sqlite3_close(database);
    database = NULL;
    
    return 0;
}