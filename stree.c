#define _XOPEN_SOURCE 700 /* Get nftw() */

#include <ftw.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int num_directories = 0;
int num_files = 0;


static const char *get_username(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    if (pw != NULL)
        return pw->pw_name;
    else
        return "unknown";
}

static const char *get_groupname(gid_t gid) {
    struct group *gr = getgrgid(gid);
    if (gr != NULL)
        return gr->gr_name;
    else
        return "unknown";
}



static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb) {
    // if (type == FTW_NS) {
    //     printf("?");
    // } else {
        int i;
        for (i = 0; i < ftwb->level; i++) {
            if (ftwb->level > 0 && i == ftwb->level - 1)
                printf("├── ");
            else
                printf("│   └── ");
        }
        printf("[");
        switch (sbuf->st_mode & S_IFMT) {
            case S_IFREG:
                printf("-");
                num_files++;
                break;
            case S_IFDIR:
                printf("d");
                num_directories++;
                break;
            case S_IFCHR:
                printf("c");
                break;
            case S_IFBLK:
                printf("b");
                break;
            case S_IFLNK:
                printf("l");
                break;
            case S_IFIFO:
                printf("p");
                break;
            case S_IFSOCK:
                printf("s");
                break;
            default:
                printf("?");
                break;
        }
    // }

    // if (type != FTW_NS)
    //     printf((sbuf->st_mode & S_IRUSR) ? "r" : "-");
    // else
    printf("-");

    printf((sbuf->st_mode & S_IWUSR) ? "w" : "-");
    printf((sbuf->st_mode & S_IXUSR) ? "x" : "-");
    printf((sbuf->st_mode & S_IRGRP) ? "r" : "-");
    printf((sbuf->st_mode & S_IWGRP) ? "w" : "-");
    printf((sbuf->st_mode & S_IXGRP) ? "x" : "-");
    printf((sbuf->st_mode & S_IROTH) ? "r" : "-");
    printf((sbuf->st_mode & S_IWOTH) ? "w" : "-");
    printf((sbuf->st_mode & S_IXOTH) ? "x" : "-");

    printf(" %s", get_username(sbuf->st_uid));
    printf(" %s", get_groupname(sbuf->st_gid));

    printf("        ");

    if (type == FTW_F || type == FTW_D) {
        printf("%lld]", (long long)sbuf->st_size);
    } else {
        printf(" ]");
    }

    printf(" %s\n", &pathname[ftwb->base]);
    return 0;
}

int main(int argc, char *argv[]) {
    int flags = 0;

    if (argc == 1) {
        if (nftw(".", dirTree, 10, flags) == -1) {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
    } else if (argc == 2) {
        if (nftw(argv[1], dirTree, 10, flags) == -1) {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Usage: %s directory-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("\n%d directory, %d files \n", num_directories, num_files);

    exit(EXIT_SUCCESS);
}
