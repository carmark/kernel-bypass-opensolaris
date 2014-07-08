#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/modctl.h>
#include <sys/cmn_err.h>

#include <sys/types.h>
#include <sys/kmem.h>

extern struct mod_ops mod_miscops;

static struct modlmisc modlmisc = {
        &mod_miscops,
        "Test Kernel Module",
};

static struct modlinkage modlinkage = {
        MODREV_1,
        (void *)&modlmisc,
        NULL,
};

void *va;

int _init(void)
{
        int i, j;
        char buf[] = "hello world!";

        if ((i = mod_install(&modlinkage)) != 0)
                cmn_err(CE_NOTE,"aaron: Could not install module\n");
        else
                cmn_err(CE_NOTE,"aaron: successfully installed");

        va = kmem_zalloc(4096, KM_NOSLEEP);
        cmn_err(CE_NOTE, "aaron: virtual addr 0x%llx", va);

        for (j = 0; j < sizeof(buf); j++)
                ((char *)va)[j] = buf[j];

        return i;
}

int _info(struct modinfo *modinfop)
{
        return (mod_info(&modlinkage, modinfop));
}

int _fini(void)
{
        int i;

        kmem_free(va, 4096);

        if ((i = mod_remove(&modlinkage)) != 0)
                cmn_err(CE_NOTE,"aaron: Could not remove module\n");
        else
                cmn_err(CE_NOTE,"aaron: successfully removed");

        return i;

}
