#include"../inc/server.h"

void daemonize_server(void) {
    pid_t pid, sid;

    //already daemon
    if(getppid() == 1) {
        return;
    }

    pid = fork();

    if(pid > 0) {
        exit(0);
    }
    
    if(pid < 0) {
        exit(1);
    }

    //Change the file mode mask
    umask(0);

    sid = setsid();

    if(sid < 0) {
        exit(1);
    }

    if((chdir(getenv("HOME"))) < 0) {
        exit(1);
    }
    
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}
