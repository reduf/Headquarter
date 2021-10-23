#ifdef COMMAND_C_INC
#error "command.c included more than once"
#endif
#define COMMAND_C_INC

CommandOptions options;

void print_help(bool terminate)
{
    printf( "Usage: [options] <script>\n"

            "    --version                  print version and exist\n"
            "    -c <config_file>           use config_file as configuration file\n"
            "    -h, --help                 print this help\n\n"

            "    -email     <string>        Sets the client's email\n"
            "    -password  <string>        Enable auto-loging. use with -email and -charname\n"
            "    -character <string>        Sets the client's security question\n"
            "    -authsrv                   Specify authserver IP to connect to\n\n"
            ""
            "    -newauth                   The client will use Portal connection\n\n"

            "    -seed                      Specify the seed for pseudo random generation\n"
            "    -v, --verbose              Enable debug logs\n"
            "    -vv, --trace               Enable trace logs\n\n"

            "    -mapid                     Specify the map id you want to start in\n"
            "    -maptype                   Specify the map type you want to start in\\nn"

#ifdef OS_WINDOWS
            "    Running as service functions:\n"
            "    -s run                     run as service\n"
            "    -s install                 install service\n"
            "    -s uninstall               uninstall service\n"
#else
            "    Running as daemon functions:\n"
            "    -s run                     run as daemon\n"
            "    -s stop                    stop daemon\n"
#endif
        );

    if (terminate) exit(0);
}

void parse_command_args(int argc, const char **argv)
{
    // @Remark: Currently, if the format is not valid, for instance -email with no
    // arguments following, we will print the help and exit. Maybe we just want
    // to returns with an error flag set.

    options.mapid = 248; // By default with load GtoB first.
    options.maptype = 3;

    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];

        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            options.print_help = true;
        } else if (!strcmp(arg, "--version")) {
            options.print_version = true;
        } else if (!strcmp(arg, "-v") || !strcmp(arg, "--verbose")) {
            options.verbose = true;
        } else if (!strcmp(arg, "-vv") || !strcmp(arg, "--trace")) {
            options.trace = true;
        } else if (!strcmp(arg, "-authsrv")) {
            options.auth_srv = argv[++i];
        } else if (!strcmp(arg, "-email")) {
            if (i + 1 >= argc) print_help(true);

            // @Remark: We need the email to be in lower cases, because
            // it is user to compute the static hash of the password.
            safe_strcpy(options.email, ARRAY_SIZE(options.email), argv[++i]);
            strlwc(options.email, ARRAY_SIZE(options.email));
        } else if (!strcmp(arg, "-password")) {
            if (i + 1 >= argc) print_help(true);
            safe_strcpy(options.password, ARRAY_SIZE(options.password), argv[++i]);
        } else if (!strcmp(arg, "-character")) {
            if (i + 1 >= argc) print_help(true);
            safe_strcpy(options.charname, ARRAY_SIZE(options.charname), argv[++i]);
        } else if (!strcmp(arg, "-s") || !strcmp(arg, "-service")) {
            if (i + 1 >= argc) print_help(true);

            const char *cmd = argv[++i];
            options.launch_service = true;

            if (!strcmp(cmd, "run"))
                options.service_option = 'r';
            else if (!strcmp(cmd, "stop"))
                options.service_option = 's';
            else if (!strcmp(cmd, "install"))
                options.service_option = 'i';
            else if (!strcmp(cmd, "uninstall"))
                options.service_option = 'u';
        } else if (!strcmp(arg, "-newauth")) {
            options.newauth = true;
        } else if (!strcmp(arg, "-mapid")) {
            if (i + 1 >= argc) print_help(true);
            options.mapid = atoi(argv[++i]);
        } else if (!strcmp(arg, "-maptype")) {
            options.maptype = atoi(argv[++i]);
        } else {
            if (options.script) print_help(true);
            options.script = arg;
        }
    }
}
