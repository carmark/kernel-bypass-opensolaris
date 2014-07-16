/* set of commands handled by driver */
#define GETSTATUS (('f'<< 8)|01)
#define SETCTL (('f'<< 8)|02)

/* structures for 3rd arg for each command */
struct foostatus {
    short status;
};

struct fooctl {
    short ctl;
};
