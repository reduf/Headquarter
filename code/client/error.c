#ifdef ERROR_C_INC
#error "error.c included more than once"
#endif
#define ERROR_C_INC

const char *get_error_s(int error_code)
{
    switch (error_code)
    {
        case 0:
            return "No error";
        case 2:
            return "Network error.";
        case 7:
            return "Your connection to the server was lost. Please check your Internet connection.";
        case 11:
            return "The password you have provided is incorrect.";
        case 19:
            return "This district you requested is being closed. Please choose another district.";
        case 21:
            return "The email address <text> was not found.";
        case 38:
            return "Network error.";
        case 39:
            return "The character name <text> is already in use by another player";
        case 30:
            return "Network error.";
        case 31:
            return "The character name <text> contains words or partial words that are not permitted in Guild Wars.";
        case 34:
            return "Your transaction is still in progress. Please wait a few moments and try again.";
        case 35:
            return "You have been disconnected from Guild Wars because another client has connected using your account. Please try again later.";
        case 41:
            return "You have attempted to create too many characters recently.";
        case 42:
            return "Guild Wars was unable to complete the operation.";
        case 45:
            return "Your account has been banned, blocked or terminated from Guild Wars.";
        case 141:
            return "Your play time has ended. To continue playing, please purchase Guid Wars. See www.guildwars.com for more information.";
        case 181:
            return "The Guid Wars Official Store is currently down for maintenance. We apologize for any inconveniences this maintenance may cause, and hope to have the Guid Wars Official Store running again in short order";
        case 3032:
            return "Network error. Please check your Internet connection and try again. (This was observed when the socket to 'Portal' was closed before connection was established)";
        default:
            return "Unknown Error";
    }
}

#if defined(OS_WINDOWS)
void win32_perror(const char *str)
{
    char *buffer = NULL;
    int error = os_errno;

    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
    DWORD lang = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

    if (FormatMessageA(flags, 0, error, lang, (LPSTR)&buffer, 0, NULL) == 0) {
        LogError("'FormatMessageA' failed. (%lu)", GetLastError());
        return;
    }

    fprintf(stderr, "%s: %s\n", str, buffer);
    LocalFree(buffer);
}
#endif // defined(OS_WIN32)
