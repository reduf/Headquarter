#ifdef CORE_CHARACTER_H
#error "character.h included more than once"
#endif
#define CORE_CHARACTER_H

typedef struct Character {
    string name;
    char   name_buffer[40];
    uuid_t uuid;

    Profession prof1;
    Profession prof2;
    // int map;
} Character;
typedef array(Character) CharacterArray;
