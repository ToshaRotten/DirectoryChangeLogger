#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))
#define DEFAULT_LOG_PATH "../logs/"

#include <errno.h>
#include <poll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include "loger/loger.h"

Loger loger;

static void handle_events(int fd, int *wd, int argc, char* argv[]){
    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;

    for (;;) {
        len = read(fd, buf, sizeof(buf));
        if (len == -1 && errno != EAGAIN) {
            printf("read");
            exit(EXIT_FAILURE);
        }
        if (len <= 0) break;
        for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;
            if (event->mask & IN_CREATE && !(event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" file is created"});
            }
            if (event->mask & IN_MODIFY && !(event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" file is modify"});
            }
            if (event->mask & IN_DELETE && !(event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" file is deleted"});
            }
            if (event->mask & IN_CREATE && (event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" directory is created"});
            }
            if (event->mask & IN_MODIFY && (event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" directory is modified"});
            }
            if (event->mask & IN_DELETE && (event->mask & IN_ISDIR)){
                loger.Logln(event->name + std::string{" directory is deleted"});
            }
        }
    }
}

int main(int argc, char* argv[]){
    loger.Configurate("JSON", DEFAULT_LOG_PATH, true);
    //loger.SetFormat("CSV");
    char buf;
    int fd, i, poll_num;
    int *WatchDescriptors;
    nfds_t nfds;
    struct pollfd fds[2];

    if (argc < 2) {
        printf("Usage: %s PATH [PATH ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Press ENTER key to terminate.\n");

    fd = inotify_init1(IN_NONBLOCK);
    if (fd == -1) {
        printf("inotify_init1");
        exit(EXIT_FAILURE);
    }
    WatchDescriptors = (int*)calloc(argc, sizeof(int));
    if (WatchDescriptors == NULL) {
        printf("calloc");
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++) {
        WatchDescriptors[i] = inotify_add_watch(fd, argv[i],IN_OPEN | IN_CLOSE | IN_CREATE | IN_MODIFY | IN_DELETE);
        if (WatchDescriptors[i] == -1) {
            fprintf(stderr, "Cannot watch '%s': %s\n", argv[i], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    nfds = 2;
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = fd;
    fds[1].events = POLLIN;

    printf("Listening for events.\n");
    while (1) {
        poll_num = poll(fds, nfds, -1);
        if (poll_num == -1) {
            if (errno == EINTR) continue;
            perror("poll");
            exit(EXIT_FAILURE);
        }
        if (poll_num > 0) {
            if (fds[0].revents & POLLIN) {
                while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n') continue;
                break;
            }
            if (fds[1].revents & POLLIN) {
                handle_events(fd, WatchDescriptors, argc, argv);
            }
        }
    }

    printf("Listening for events stopped.\n");
    close(fd);
    free(WatchDescriptors);
    exit(EXIT_SUCCESS);
}
