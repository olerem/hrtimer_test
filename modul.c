#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

static int hrtimer_test_init(void)
{
	pr_info("Hello, world\n");
	return 0;
}

static void hrtimer_test_exit(void)
{
	pr_info("Goodbye, cruel world\n");
}

module_init(hrtimer_test_init);
module_exit(hrtimer_test_exit);
