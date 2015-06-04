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
#include <linux/hrtimer.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

struct hrtimer_test_priv {
	struct hrtimer timer;
	struct tasklet_struct timer_tasklet;

	unsigned int pass;
	unsigned int fail;

	s64 hrexp;
	s64 hrprep;
};

static struct hrtimer_test_priv *hr_priv;

static void hrtest_print_stats(struct hrtimer_test_priv *priv)
{
	pr_info("hrtimer test stats: pass: %u, fail %u\n", priv->pass, priv->fail);
}

static int hrtest_stop_timer(struct hrtimer_test_priv *priv)
{
	hrtimer_cancel(&priv->timer);

	return 0;
}

static int hrtest_start_timer(struct hrtimer_test_priv *priv)
{
        unsigned int timeout_ns = 1000 * 1000 * 1000;
	ktime_t tout;

	hrtest_stop_timer(priv);

	tout = ktime_set(0, timeout_ns);

	hrtimer_start(&priv->timer, tout, HRTIMER_MODE_REL_PINNED);

        priv->hrexp = hrtimer_get_expires_ns(&priv->timer);
        priv->hrprep = ktime_to_ns(hrtimer_cb_get_time(&priv->timer));

	if (priv->hrexp < priv->hrprep) {
		priv->fail++;
		pr_warn("%s: warning! expiration time is in the past: now = %lld, exp = %lld. Stat: ok: %i, nok: %i\n",
			__func__, priv->hrprep, priv->hrexp, priv->pass, priv->fail);
	} else
		priv->pass++;

	return 0;
}

static void hrtimer_test_tasklet_cb(unsigned long data)
{
	struct hrtimer_test_priv *priv = (struct hrtimer_test_priv *)data;

	hrtest_start_timer(priv);
}

static enum hrtimer_restart hrtimer_test_cb(struct hrtimer *timer)
{
	struct hrtimer_test_priv *priv =
			container_of(timer, struct hrtimer_test_priv, timer);

	tasklet_schedule(&priv->timer_tasklet);

	return HRTIMER_NORESTART;
}

static int hrtimer_test_init(void)
{
	struct hrtimer_test_priv *priv;
	pr_info("%s\n",__func__);

	priv = kzalloc(sizeof(*hr_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	hr_priv = priv;

	tasklet_init(&priv->timer_tasklet, hrtimer_test_tasklet_cb,
			(unsigned long)priv);

	hrtimer_init(&priv->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	priv->timer.function = hrtimer_test_cb;

	hrtest_start_timer(priv);

	return 0;
}

static void hrtimer_test_exit(void)
{
	hrtest_stop_timer(hr_priv);
	tasklet_kill(&hr_priv->timer_tasklet);
	kfree(hr_priv);
	pr_info("%s\n",__func__);
}

module_init(hrtimer_test_init);
module_exit(hrtimer_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksij Rempel <linux@rempel-privat.de>");
MODULE_DESCRIPTION("hrtimer test module");
