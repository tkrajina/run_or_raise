#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int get_window_pid(Display *display, Window *window) {
    int result = 0;
    XTextProperty text_data;
    Atom atom = XInternAtom(display, "_NET_WM_PID", True);
    int status = XGetTextProperty(display, window, &text_data, atom);
    if(text_data.nitems) {
        result = text_data.value[1] * 256;
        result += text_data.value[0];
        //printf("pid=%i\n", result);
        return result;
    }
    return result;
}

void bring_to_front(Display *display, Window *window, int pid) {
    Window parent;
    Window *children;
    Window *child;
    unsigned int no_of_children;

    if(!XQueryTree(display, window, &window, &parent, &children, &no_of_children))
        return;

    if(!no_of_children)
        return;

    int i;
    for(i = 0; i < no_of_children; i++) {
        child = children[i];
        int this_window_pid = get_window_pid(display, child);
        if(this_window_pid == pid) {
            printf("Raising pid=%i\n", this_window_pid);
            XRaiseWindow(display, child);
        }
        bring_to_front(display, child, pid);
    }
}

int is_numeric(char *string) {
    int i = atoi(string);
    char *back_to_string[20];
    sprintf(back_to_string, "%i", i);
    return strcmp(string, back_to_string) == 0;
}

int find_pid(char *app_name) {
    DIR *proc_dir = opendir("/proc");
    struct dirent *dir_entry;
    if(!proc_dir) {
        return -1;
    }

    while(dir_entry = readdir(proc_dir)) {
        char cmdline_file[100],
             line[50][200];
        int line_part = 0;

        if(is_numeric(dir_entry->d_name)) {
            int pid = atoi(dir_entry->d_name);
            sprintf(cmdline_file, "/proc/%i/cmdline", pid);
            FILE *f = fopen(cmdline_file, "r");
            if(f) {
                char c;
                int i = 0;
                while((c = fgetc(f)) != EOF && (i < sizeof(line) - 1)) {
                    if(c == '\0') {
                        line[line_part][i] = c;
                        ++ line_part;
                        i = 0;
                    } else {
                        line[line_part][i] = c;
                        ++ i;
                    }
                    //printf("line[%i]=%s\n", line_part, line[line_part]);
                }
                if(i > 0) {
                    line[line_part][i] = '\0';
                }

                fclose(f);

                /*
                for(i = 0; i < line_part; i++) {
                    printf("%10s -> %i: %s\n", dir_entry->d_name, i, line[i]);
                }
                */

                //printf("strcmp(%s, %s)=%i\n", line[0], app_name, strcmp(line[0], app_name));
                if(strcmp(line[0], app_name) == 0) {
                    return pid;
                }

            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("No appname!\n", argc);
        return 1;
    }

    char *app_name = argv[1];

    int pid = find_pid(app_name);
    if(pid) {
        Display *display;
        display = XOpenDisplay(NULL);
        if(display == NULL) {
            fprintf(stderr, "Cannot connect to X server\n");
            exit(1);
        }

        Window root_window = DefaultRootWindow(display);

        bring_to_front(display, root_window, pid);
    } else {
        printf("Can't find PID for this app!\n", argc);
        return 1;
    }

    return 0;
}
