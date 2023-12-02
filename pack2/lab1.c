#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    gid_t gid = getgid();
    uid_t uid = getuid();
    gid_t egid = getegid();
    uid_t euid = geteuid();
    gid_t pgrp = getpgrp();

    printf("Current pid ID: %d\n", pid);
    printf("Parent pid ID: %d\n", ppid);
    printf("Group ID of the calling pid: %d\n", gid);
    printf("Real owner ID: %d\n", uid);
    printf("Owner's Real Group ID: %d\n", egid);
    printf("Effective owner ID: %d\n", euid);
    printf("Owner's effective group ID: %d\n", pgrp);

    return 0;
}
