/*
 * Lei Xue (carmark.dlut@gmail.com)
 *
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/errno.h>
#include <sys/uio.h>
#include <sys/buf.h>
#include <sys/modctl.h>
#include <sys/open.h>
#include <sys/kmem.h>
#include <sys/poll.h>
#include <sys/conf.h>
#include <sys/cmn_err.h>
#include <sys/stat.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/stropts.h>

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

/*
 * The entire state of each skeleton device.
 */
typedef struct {
    dev_info_t *dip; /* my devinfo handle */
} skel_devstate_t;

/*
 * Only descriptor for a shared memory region
 */
typedef struct umem_s {
    char name[20];
    size_t size;
    char * base;
    ddi_acc_handle_t acc_handle;
    ddi_umem_cookie_t cookie;
    uint_t dma_cookie_num;
    ddi_dma_cookie_t dma_cookie;
} umem_t;

dev_info_t *dip_one;
/*
 * An opaque handle where our set of skeleton devices live
 */
static void *skel_state;

/* more prototypes added as labs progress, not all are shown here */
/* prototypes can be found in corresponding man pages (man open.9e) */
/* and header file /usr/include/sys/devops.h */
static int skel_open(dev_t *devp, int flag, int otyp, cred_t *cred);
static int skel_read(dev_t dev, struct uio *uiop, cred_t *credp);
static int skel_write(dev_t dev, struct uio *uiop, cred_t *credp);
static int skel_getinfo(dev_info_t *dip, ddi_info_cmd_t infocmd, void *arg, void **result);
static int skel_attach(dev_info_t *dip, ddi_attach_cmd_t cmd);
static int skel_detach(dev_info_t *dip, ddi_detach_cmd_t cmd);
static int skel_ioctl(dev_t dev, int cmd, intptr_t arg, int mode, cred_t *crp, int *rvalp);
static int skel_devmap(dev_t dev, devmap_cookie_t dhp, offset_t off, size_t len, size_t *maplen, uint_t model);
static int skel_devmap_access(devmap_cookie_t dhp, void *pvtp, offset_t off, size_t len, uint_t type, uint_t rw);
static int skel_devmap_unmap(devmap_cookie_t dhp, void *pvtp, offset_t off, size_t len, devmap_cookie_t new_dhp1, void **new_pvtp1, devmap_cookie_t new_dhp2, void **new_pvtp2);
static int skel_segmap(dev_t dev, off_t off, struct as *asp, caddr_t *addrp, off_t len, unsigned int prot, unsigned int maxprot, unsigned int flags, cred_t *cred);
int skel_get_umem(umem_t *umem);
int skel_set_umem(umem_t umem);
