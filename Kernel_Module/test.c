#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char **argv) {
    int pid;
    if (argc < 2) {
        printf("Error: missing pid argument\n");
        return 1;
    }
    pid = atoi(argv[1]);
    syscall(__NR_syscall_module_syscall, pid);
    return 0;
}

