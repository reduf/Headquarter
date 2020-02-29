#ifdef COMMAND_H_INC
#error "command.h included more than once"
#endif
#define COMMAND_H_INC

typedef struct CommandOptions {
    bool        print_help;
    bool        print_version;
    bool        verbose;
    bool        newauth;

    const char *script;
    const char *auth_srv;
    const char *conf_file;

    char        email[64];
    char        password[100];
    char        charname[20];

    bool        launch_service;
    int         service_option; // windows => {'r', 'i', 'u'} or linux => {'r', 's'}

    int32_t     seed;
} CommandOptions;

extern CommandOptions options;

void print_help(bool terminate);
void parse_command_args(int argc, const char **argv);
