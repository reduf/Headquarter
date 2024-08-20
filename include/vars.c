#ifdef VARS_C_INC
#error "vars.c is already included"
#endif
#define VARS_C_INC

#include "str.h"
#include <common/hash.h>
#include <common/array.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum VarType {
    VT_ERROR,
    VT_BOOL,
    VT_FLOAT,
    VT_STRING,
    VT_INTEGER,
} VarType;

typedef struct Var {
    VarType type;
    union {
        int   i;
        bool  b;
        float f; 
    };
    const char *str;
    const char *name;
} Var;

typedef struct HashSlot {
    bool occupied;
    uint32_t hash;
    uint32_t value;
    const char *key;
} HashSlot;

typedef struct HashTable {
    size_t allocated;
    size_t count;
    HashSlot *slots;
} HashTable;

typedef array(Var) VarArray;

typedef struct VarSection {
    const char *name;
    VarArray vars;
    HashTable table;
} VarSection;

typedef array(VarSection) VarSectionArray;

typedef struct VarFile {
    bool error;
    char *data;
    size_t data_size;
    VarSectionArray sections;
    HashTable table;
} VarFile;

void create_file_table(VarFile *file);
void create_section_table(VarSection *section);

size_t file_size(FILE *file)
{
    int pos = ftell(file);
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, pos, SEEK_SET);
    return len;
}

void *read_entire_file(const char *path, size_t *length)
{
    FILE *file = fopen(path, "rb");
    if (!file) return NULL;
    size_t size = file_size(file);
    char *buffer = cast(char *)malloc(size + 1);
    size_t read = fread(buffer, 1, size, file);
    if (read > 0 && (size_t)read == size) {
        if (length) *length = size;
        buffer[read] = 0;
    } else {
        free(buffer);
        buffer = NULL;
    }
    fclose(file);
    return buffer;
}

int is_alpha(int c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int is_num(int c)
{
    return ('0' <= c && c <= '9');
}

int is_alnum(int c)
{
    return is_alpha(c) || is_num(c);
}

char *str_trim(char *str)
{
    while (*str && isspace((unsigned char)*str)) str++;
    size_t len = strlen(str);
    if (!len) return str;
    char *s = str + len - 1;
    while (isspace((unsigned char)*s) && (str < s)) s--;
    s[1] = 0;
    return str;
}

char *consume_line(char **sp)
{
    char *line = *sp;
    char *next_line = strchr(line, '\n');
    
    if (!next_line) {
        *sp = NULL;
        return line;
    }

    if (next_line[-1] == '\r') next_line[-1] = 0;
    *next_line = 0;
    *sp = next_line + 1;
    return line;
}

char *eat_spaces(char *str)
{
    if (!str) return NULL;
    while (*str && (*str == ' ')) str++;
    if (*str == 0) return NULL;
    return str;
}

void free_vars_file(VarFile *file)
{
    if (!file) return;
    if (file->table.slots)
        free(file->table.slots);

    VarSection *section;
    array_foreach(section, &file->sections) {
        if (section->table.slots)
            free(section->table.slots);
        array_reset(&section->vars);
    }

    array_reset(&file->sections);
    free(file->data);
}

VarFile parse_vars_file(const char *path)
{
    VarFile var_file = {0};
    size_t line_number = 0;
    size_t size;

    char *data = cast(char *)read_entire_file(path, &size);
    if (!data) {
        var_file.error = true;
        return var_file;
    }

    var_file.data = data;
    var_file.data_size = size;

    VarSectionArray *sections = &var_file.sections;
    array_init(sections);

    VarSection global;
    global.name = "global";
    array_init(&global.vars);
    array_add(sections, global);

    VarSection *current_section = &array_back(sections);

    char *line;
    while (data) {
        line_number += 1;
        line = consume_line(&data);

        line = eat_spaces(line);
        if (!line) continue;
        if (*line == '#') continue;

        // printf("line %d: %s\n", line_number, line);

        if (*line == ':') {
            if (line[1] != '/') {
                printf("%s:%zu Section name must start with ':/'\n", path, line_number);
                continue;
            }

            if (!is_alpha(line[2])) {
                printf("%s:%zu Section must have a name\n", path, line_number);
                continue;
            }

            char *name = line + 2;
            name = str_trim(name);

            VarSection sect;
            sect.name = strlwc(name, strlen(name));
            array_init(&sect.vars);
            array_add(sections, sect);
            current_section = &array_back(sections);

            // printf("New section '%s'\n", name);
        } else {
            char *lhs = line;
            lhs = str_trim(lhs);
            while (*line) {
                if (*line == ' ') break;
                if (*line == '=') break;
                line++;
            }

            if (!*line) {
                printf("%s:%zu expected expression\n", path, line_number);
                continue;
            }

            *line = 0;
            char *rhs = line + 1;
            while (*rhs) {
                if (*rhs == '"') break; // if the rhs is a string
                if (*rhs == '.') break; // if the rhs is a float
                if (is_alnum(*rhs)) break;
                rhs++;
            }

            if (!*rhs) {
                printf("%s:%zu expected expression\n", path, line_number);
                continue;
            }

            char *hashtag = strchr(rhs, '#');
            if (hashtag) *hashtag = 0;
            rhs = str_trim(rhs);

            Var var;
            var.str = rhs;
            var.name = strlwc(lhs, strlen(lhs));

            if (*rhs == '"') {
                var.type = VT_STRING;
                char *quote = NULL;
                while (rhs) {
                    quote = strchr(rhs + 1, '"');
                    if (!quote) break;
                    if (quote[-1] != '\\') break;
                    rhs = quote;
                }

                if (!quote) {
                    printf("%s:%zu a string need a closing quote\n", path, line_number);
                    continue;
                }
                var.str++;
                *quote = 0;
            } else if (*rhs == 't' || *rhs == 'y') {
                var.type = VT_BOOL;
                var.b = true;
            } else if (*rhs == 'f' || *rhs == 'n') {
                var.type = VT_BOOL;
                var.b = false;
            } else {
                if (strchr(rhs, '.')) {
                    var.type = VT_FLOAT;
                    var.f = (float)atof(rhs);
                } else {
                    var.type = VT_INTEGER;
                    var.i = atoi(rhs);
                }
            }

            array_add(&current_section->vars, var);

            // printf("Line %u: lhs: '%s', rhs: '%s'\n", line_number, lhs, rhs);
        }
    }

    if (!var_file.error) {
        create_file_table(&var_file);
    }


    return var_file;
}

void create_file_table(VarFile *file)
{
    size_t table_size = 2 * array_size(&file->sections);
    file->table.slots = cast(HashSlot *)calloc(table_size, sizeof(HashSlot));
    file->table.count = 0;
    file->table.allocated = table_size;

    HashSlot *slots = file->table.slots;
    for (size_t i = 0; i < array_size(&file->sections); i++) {
        VarSection *section = &array_at(&file->sections, i);

        uint32_t str_hash = hash_str(section->name);
        uint32_t hash = str_hash;
        uint32_t index = hash % table_size;
        while (slots[index].occupied) {
            hash = hash_int32(hash);
            index = hash % table_size;
        }

        /*
        printf("Section %s\n", section->name);
        printf("  hash: %08X\n", hash);
        printf("  index: %zu\n", i);
        */

        slots[index].occupied = true;
        slots[index].hash = hash;
        slots[index].value = (uint32_t)i;
        slots[index].key = section->name;

        create_section_table(section);
    }
}

void create_section_table(VarSection *section)
{
    size_t table_size = 2 * array_size(&section->vars);
    section->table.slots = cast(HashSlot *)calloc(table_size, sizeof(HashSlot));
    section->table.count = 0;
    section->table.allocated = table_size;

    HashSlot *slots = section->table.slots;
    for (size_t i = 0; i < array_size(&section->vars); i++) {
        Var *var = &array_at(&section->vars, i);

        uint32_t str_hash = hash_str(var->name);
        uint32_t hash = str_hash;
        uint32_t index = hash % table_size;
        while (slots[index].occupied) {
            hash = hash_int32(hash);
            index = hash % table_size;
        }

        /*
        printf("Var %s\n", var->name);
        printf("  hash: %08X\n", hash);
        printf("  index: %zu\n", i);
        */

        slots[index].occupied = true;
        slots[index].hash = hash;
        slots[index].value = (uint32_t)i;
        slots[index].key = var->name;
    }
}

VarSection *find_section(VarFile *file, const char *name)
{
    HashSlot *slots = file->table.slots;
    size_t table_size = file->table.allocated;

    uint32_t str_hash = hash_stri(name);
    uint32_t hash = str_hash;
    uint32_t index = hash % table_size;
    while (slots[index].occupied) {
        if (!stricmp(name, slots[index].key)) {
            return &array_at(&file->sections, slots[index].value);
        }

        hash = hash_int32(hash);
        index = hash % table_size;
    }

    return NULL;
}

Var *find_var(VarSection *section, const char *name)
{
    HashSlot *slots = section->table.slots;
    size_t table_size = section->table.allocated;

    uint32_t str_hash = hash_stri(name);
    uint32_t hash = str_hash;
    uint32_t index = hash % table_size;
    while (slots[index].occupied) {
        if (!stricmp(name, slots[index].key)) {
            return &array_at(&section->vars, slots[index].value);
        }

        hash = hash_int32(hash);
        index = hash % table_size;
    }

    return NULL;
}

bool var_get_boolean_default(VarSection *section, const char *name, bool def)
{
    Var *var = find_var(section, name);
    if (!var) return def;
    if (var->type == VT_INTEGER) return (var->i != 0);
    if (var->type == VT_BOOL) return var->b;
    return def;
}

int var_get_int_default(VarSection *section, const char *name, int def)
{
    Var *var = find_var(section, name);
    if (!var) return def;
    if (var->type == VT_INTEGER) return var->i;
    return def;
}

const char *var_get_string_default(VarSection *section, const char *name, const char *def)
{
    Var *var = find_var(section, name);
    if (!var) return def;
    if (var->type == VT_STRING) return var->str;
    return def;
}

const char *type_name(VarType type) {
    switch (type) {
        case VT_BOOL:    return "VT_BOOL";
        case VT_FLOAT:   return "VT_FLOAT";
        case VT_STRING:  return "VT_STRING";
        case VT_INTEGER: return "VT_INTEGER";
        default:         return "VT_UNKNOW";
    }
}
