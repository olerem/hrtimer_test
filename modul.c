/*
 * Copyright (c) 2015 Oleksij Rempel <linux@rempel-privat.de>.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/init.h>
#include <linux/module.h>

static int hrtimer_test_init(void)
{
	pr_info("%s\n",__func__);
	return 0;
}

static void hrtimer_test_exit(void)
{
	pr_info("%s\n",__func__);
}

module_init(hrtimer_test_init);
module_exit(hrtimer_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksij Rempel <linux@rempel-privat.de>");
MODULE_DESCRIPTION("hrtimer test module");
