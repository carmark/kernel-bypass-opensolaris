/*
 *
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

/* more prototypes added as labs progress, not all are shown here */
/* prototypes can be found in corresponding man pages (man open.9e) */
/* and header file /usr/include/sys/devops.h */
static int skel_open(dev_t *devp, int flag, int otyp, cred_t *cred);
static int skel_read(dev_t dev, struct uio *uiop, cred_t *credp);
static int skel_write(dev_t dev, struct uio *uiop, cred_t *credp);
static int skel_getinfo(dev_info_t *dip, ddi_info_cmd_t infocmd, void *arg, void **result);
static int skel_attach(dev_info_t *dip, ddi_attach_cmd_t cmd);
static int skel_detach(dev_info_t *dip, ddi_detach_cmd_t cmd);

/*
 * The entire state of each skeleton device.
 */
typedef struct {
    dev_info_t *dip; /* my devinfo handle */
} skel_devstate_t;

/*
 * An opaque handle where our set of skeleton devices live
 */
static void *skel_state;

static struct cb_ops skel_cb_ops = { /* see cb_ops(9s) for details */
    skel_open,
    nulldev, /* close */
    nodev, /* strategy */
    nodev, /* print */
    nodev, /* dump */
    skel_read,
    skel_write,
    nodev, /* ioctl */
    nodev, /* devmap */
    nodev, /* mmap */
    nodev, /* segmap */
    nochpoll, /* poll */
    ddi_prop_op,
    NULL, /* streamtab */
    D_NEW | D_MP,
    CB_REV,
    nodev, /* aread */
    nodev /* awrite */
};
static struct dev_ops skel_ops = { /* see dev_ops(9s) */
    DEVO_REV,
    0, /* refcnt */
    skel_getinfo,
    nulldev, /* identify */
    nulldev, /* probe */
    skel_attach,
    skel_detach,
    nodev, /* reset */
    &skel_cb_ops,
    (struct bus_ops *)0,
    nodev /* power */
};

extern struct mod_ops mod_driverops;

static struct modldrv modldrv = { /* see modldrv(9s) */
    &mod_driverops,
    "skeleton driver v1.0",
    &skel_ops
};
static struct modlinkage modlinkage = { /* see modlinkage(9s) */
    MODREV_1,
    &modldrv,
    0
};

int
_init(void) /* see _init(9e) */
{
    int e;
    if ((e = ddi_soft_state_init(&skel_state,
                    sizeof (skel_devstate_t), 1)) != 0) {
        return (e);
    }
    if ((e = mod_install(&modlinkage)) != 0) {
        ddi_soft_state_fini(&skel_state);
    }
    return (e);
}

int
_fini(void) /* see _fini(9e) */
{
    int e;
    if ((e = mod_remove(&modlinkage)) != 0) {
        return (e);
    }
    ddi_soft_state_fini(&skel_state);
    return (e);
}
int
_info(struct modinfo *modinfop) /* _info(9e) */
{
    return (mod_info(&modlinkage, modinfop));
}

static int /* called for each device instance, see attach(9e) */
skel_attach(dev_info_t *dip, ddi_attach_cmd_t cmd)
{
    int instance;
    skel_devstate_t *rsp;
    switch (cmd) {
        case DDI_ATTACH:
            instance = ddi_get_instance(dip);
            if (ddi_soft_state_zalloc(skel_state, instance) != DDI_SUCCESS) {
                cmn_err(CE_CONT, "%s%d: can't allocate state\n", ddi_get_name(dip), instance);
                return (DDI_FAILURE);
            } else
                rsp = ddi_get_soft_state(skel_state, instance);
            if (ddi_create_minor_node(dip, "skel", S_IFCHR, instance, DDI_PSEUDO, 0) == DDI_FAILURE) {
                ddi_remove_minor_node(dip, NULL);
                goto attach_failed;
            }
            rsp->dip = dip;
            ddi_report_dev(dip);
            return (DDI_SUCCESS);
        default:
            return (DDI_FAILURE);
    }
attach_failed:
    (void) skel_detach(dip, DDI_DETACH);
    return (DDI_FAILURE);
}

static int /* see detach(9e) */
skel_detach(dev_info_t *dip, ddi_detach_cmd_t cmd)
{
    int instance;
    register skel_devstate_t *rsp;
    switch (cmd) {
        case DDI_DETACH:
            ddi_prop_remove_all(dip);
            instance = ddi_get_instance(dip);
            rsp = ddi_get_soft_state(skel_state, instance);
            ddi_remove_minor_node(dip, NULL);
            ddi_soft_state_free(skel_state, instance);
            return (DDI_SUCCESS);
        default:
            return (DDI_FAILURE);
    }
}

static int
skel_getinfo(dev_info_t *dip, ddi_info_cmd_t infocmd, void *arg, void **result)
{
	return (DDI_SUCCESS);
}

/*ARGSUSED*/
static int /* called on open(2), see open(9e) */
skel_open(dev_t *devp, int flag, int otyp, cred_t *cred)
{
    if (otyp != OTYP_BLK && otyp != OTYP_CHR)
        return (EINVAL);
    if (ddi_get_soft_state(skel_state, getminor(*devp)) == NULL)
        return (ENXIO);
    return (0);
}

static int
skel_read(dev_t dev, struct uio *uiop, cred_t *credp)
{
    int instance = getminor(dev);
    skel_devstate_t *rsp = ddi_get_soft_state(skel_state, instance);
    return(0);
}
/*ARGSUSED*/
static int
skel_write(dev_t dev, register struct uio *uiop, cred_t *credp)
{
    int instance = getminor(dev);
    skel_devstate_t *rsp = ddi_get_soft_state(skel_state, instance);
    return(0);
}


