
// 説明: kernelのkzalloc interfaceの動作test
// 出展: https://hiboma.hatenadiary.jp/entry/2016/11/25/125004

#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Shin Matsuzaki");
MODULE_DESCRIPTION("kzalloc test");

void *kz, *km;

static int mymodule_init(void) {
        kz = kzalloc(PAGE_SIZE, GFP_KERNEL);
        km = kmalloc(PAGE_SIZE, GFP_KERNEL);
        pr_info("kazallo: %p kmalloc: %p\n", kz, km);

        return 0;
}

static void mymodule_exit(void) {
        kzfree(kz);
        kfree(km);
}

module_init(mymodule_init);
module_exit(mymodule_exit);
