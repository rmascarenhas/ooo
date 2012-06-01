/* This is an implementation of the getpwnam(3) function in terms of setpwenv(3),
 * getpwenv(3) and endpwenv(3). It scans your password file until the user with
 * the passed username is found.
 *
 * Signature:
 *
 *      struct passwd *getpwnam(const char *name);
 *
 * This function (as well as its glibc version) is not reentrant, since the
 * passwd struct is statically allocated. If you need reentrancy, use
 * getpwnam_r(3).
 */ 

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct passwd *
getpwnam(const char *name) {
    struct passwd *pwd = NULL;

    setpwent();

    while ((pwd = getpwent()) != NULL) {
        if (strncmp(pwd->pw_name, name, sysconf(_SC_LOGIN_NAME_MAX)) == 0) {
            break;
        }
    }

    endpwent();

    return pwd;
}


/****************************************************************************/

/* Usage:
 * $ ./a.out <username>
 * Prints user info if user is found, or an error message otherwise.
 */

#include <stdio.h>

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(1);
    }

    char *username = argv[1];
    struct passwd *user_info = getpwnam(username);

    if (user_info == NULL) {
        printf("%s: user not found.\n", username);
    } else {
        printf("Username: %s\n", user_info->pw_name);
        printf("Encrypted password: %s\n", user_info->pw_passwd);
        printf("UID: %ld\n", (long) user_info->pw_uid);
        printf("GID: %ld\n", (long) user_info->pw_gid);
        printf("Comment: %s\n", user_info->pw_gecos);
        printf("Home dir: %s\n", user_info->pw_dir);
        printf("Shell: %s\n", user_info->pw_shell);
    }

    return 0;
}
