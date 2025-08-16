/***************************************************************************//**
  @file         avr_sh_builtins.c
  @author       Adithya Venkat Ramanan
  @brief        AVR Shell
*******************************************************************************/

#include "avr_sh_builtins.h"

static void avr_sh_lsDir(const char* path, bool show_all)
{
    struct dirent ** namelist = NULL;
    int num = scandir(path, &namelist, NULL, alphasort);

    if(num < 0)
    {
        fprintf(stderr, "%savr_sh_ls: error in scandir()%s\n", BOLD_RED, DEFAULT);
        return;
    }

    for(int i = 0; i < num; i++)
    {
        const char * name = namelist[i]->d_name;
        if(!show_all && name[0] == '.')
        {
            free(namelist[i]);
            continue;
        }
        printf("%s\t", name);
        free(namelist[i]);
    }
    printf("\n");
    free(namelist);
}

static void avr_sh_lsOne(const char* path, bool show_all)
{
    struct stat st;

    if(lstat(path, &st) != 0)
    {
        fprintf(stderr, "%savr_sh_ls: error in lstat()%s\n", BOLD_RED, DEFAULT);
        return;
    }

    if(S_ISDIR(st.st_mode))
    {
        avr_sh_lsDir(path, show_all);
    }
}

/**
 * @brief "ls" - List Directory
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_ls(int argc, char ** argv)
{
    bool show_all = false;

    int i = 1;
    // Parse flags: only -a and -- supported
    while (i < argc && argv[i][0] == '-') 
    {
        if (strcmp(argv[i], "-a") == 0) 
        {
            show_all = true; 
            i++;
        } 
        else if (strcmp(argv[i], "--") == 0)
        {
            i++; 
            break;
        } 
        else 
        {
            fprintf(stderr, "%savr_sh_ls: unsupported option: %s%s\n", BOLD_RED, argv[i], DEFAULT);
            return;
        }
    }

    int numPaths = argc - i;

    if(numPaths <= 0)
    {
        DEBUG_PRINT("%s(): curr directory only\n", __func__);
        avr_sh_lsOne(".", show_all);
    }
    else
    {
        DEBUG_PRINT("%s(): multiple directories\n", __func__);
        for(int iPath = i; iPath < argc; iPath++)
        {
            if(numPaths > 1)
            {
                printf("%s:\n", argv[iPath]);
            }
            avr_sh_lsOne(argv[iPath], show_all);
            printf("\n");
        }
    }

}

/**
 * @brief "cd" - Change Directory
 * @param argc argument count
 * @param argv Array of args
 * @return None
 */
void avr_sh_cd(int argc, char ** argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "%savr_sh_cd: \"cd\" expects at most 1 argument\n%s",
                BOLD_RED, DEFAULT);
        return;
    }

    const char *dest = NULL;
    char *dest_owned = NULL;   // malloc'd when we need to build a path
    char *oldcwd = NULL;
    char *newcwd = NULL;

    if (argc == 1) 
    {
        // cd -> $HOME (fallback to "/")
        const char *home = getenv("HOME");
        dest = (home && *home) ? home : "/";
        DEBUG_PRINT("cd to HOME (%s)\n", dest);
    } 
    else if (strcmp(argv[1], "-") == 0)
    {
        // cd - -> $OLDPWD and print it on success
        const char *oldpwd = getenv("OLDPWD");
        if (!oldpwd || !*oldpwd)
        {
            fprintf(stderr, "%savr_sh_cd: OLDPWD not set\n%s", BOLD_RED, DEFAULT);
            return;
        }
        dest = oldpwd;
        DEBUG_PRINT("cd to OLDPWD (%s)\n", dest);
    }
    else if (argv[1][0] == '~')
    {
        // cd ~ or cd ~/sub/path
        const char *home = getenv("HOME");
        if (!home || !*home)
        {
            fprintf(stderr, "%savr_sh_cd: HOME not set\n%s", BOLD_RED, DEFAULT);
            return;
        }
        if (argv[1][1] == '\0') 
        {
            dest = home; // just "~"
            DEBUG_PRINT("cd to HOME (%s)\n", dest);
        } 
        else if (argv[1][1] == '/')
        {
            // "~/<rest>"
            size_t rest_path = strlen(home) + strlen(argv[1] + 1) + 1;
            dest_owned = (char *)malloc(rest_path);
            if (!dest_owned) {
                fprintf(stderr, "%savr_sh_cd: out of memory\n%s", BOLD_RED, DEFAULT);
                return;
            }
            strcpy(dest_owned, home);
            strcat(dest_owned, argv[1] + 1); // appends "/<rest>"
            dest_owned[rest_path-1] = '\0';
            dest = dest_owned;
            DEBUG_PRINT("cd to PATH (%s)\n", dest);
        }
        else
        {
            // "~<xyz>" not implemented in this minimal shell
            fprintf(stderr, "%savr_sh_cd: \"~<xyz>\" not supported\n%s", BOLD_RED, DEFAULT);
            return;
        }
    }
    else
    {
        // cd <path>
        dest = argv[1];
        DEBUG_PRINT("cd to PATH (%s)\n", dest);
    }

    // Save current cwd to update OLDPWD after a successful chdir
    oldcwd = getcwd(NULL, 0); // dynamic; may be NULL if cwd unreadable

    if (chdir(dest) != 0)
    {
        fprintf(stderr, "%savr_sh_cd: Error in chdir(dest)\n%s", BOLD_RED, DEFAULT);
        free(dest_owned);
        free(oldcwd);
        return;
    }

    newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        if (oldcwd)
        {
            setenv("OLDPWD", oldcwd, 1);
        }
        setenv("PWD", newcwd, 1);
    }

    // POSIX shells print the new dir for "cd -"
    if (argc == 2 && strcmp(argv[1], "-") == 0)
    {
        printf("%s\n", newcwd ? newcwd : dest);
    }

    free(dest_owned);
    free(oldcwd);
    free(newcwd);
}
