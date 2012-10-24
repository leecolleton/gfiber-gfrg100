/****************************************************************************
 *
 * rg/pkg/kernel/linux/boot/cramfs_in_ubi.c
 * 
 * Copyright (C) Jungo LTD 2004
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General 
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02111-1307, USA.
 *
 * Developed by Jungo LTD.
 * Residential Gateway Software Division
 * www.jungo.com
 * info@jungo.com
 */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/workqueue.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/ubi.h>
#include <asm/io.h>

static const char *part_probes[] = { "cmdlinepart", NULL };

typedef struct {
    struct work_struct work;
    char *mtd_name;
} ciu_work_t;

static void ciu_mainfs_mtdparts(struct work_struct *work)
{
    ciu_work_t *w = container_of(work, ciu_work_t, work);
    struct mtd_partition *mtd_parts = NULL;
    struct mtd_info *mtd;
    int n = 0;

    mtd = get_mtd_device_nm(w->mtd_name);
    if (IS_ERR(mtd))
    {
	printk("Failed to find MTD device %s\n", w->mtd_name);
	return;
    }
    /* The kernel comamnd line is expected to contain information for creating 
     * the mainfs partition on top of the
     * CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION partition,
     * i.e: OpenRG:rofs_len@rofs_addr(mainfs)
     */
    n = parse_mtd_partitions(mtd, part_probes, &mtd_parts, 0);
    if (n <= 0)
    {
	printk("Failed to find partitions for MTD device %s\n", mtd->name);
	put_mtd_device(mtd);
	return;
    }
    add_mtd_partitions(mtd, mtd_parts, n);
    /* At this point, the mainfs partition is created on top of the
     * CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION partition.
     * We keep our reference of mtd so that the UBI volume is kept open.
     */
}

static void ciu_ubi_attach(struct work_struct *work)
{
    ciu_work_t *w = container_of(work, ciu_work_t, work);
    int err;

    if ((err = ubi_attach_mtd_dev_nm(w->mtd_name, UBI_DEV_NUM_AUTO, 0)) < 0)
    {
	printk("Failed to attach UBI volume to MTD %s, err %d\n", w->mtd_name,
	    err);
    }
}

static ciu_work_t ciu_work[] = {
#define CIU_WORK(i, name, func) \
    [i] = { \
	.work = __WORK_INITIALIZER(ciu_work[i].work, func), \
	.mtd_name = name \
    }
    CIU_WORK(0, CONFIG_RG_UBI_PARTITION, ciu_ubi_attach),
    CIU_WORK(1, CONFIG_RG_PERM_STORAGE_DEFAULT_PARTITION, ciu_mainfs_mtdparts),
    CIU_WORK(2, NULL, NULL)
};

static void ciu_mtd_notify_add(struct mtd_info *mtd)
{
    ciu_work_t *w;

    /* Since we cannot directly call code that creates MTD devices from
     * an MTD notifier, we defer the work to a workqueue.
     * Workqueues are flushed after initcalls so we can rely on all the
     * operations to complete before mainfs is mounted.
     */
    for (w = &ciu_work[0]; w->mtd_name && strcmp(w->mtd_name, mtd->name); w++);
    if (!w->mtd_name)
	return;

    schedule_work(&w->work);
}

static struct mtd_notifier cramfs_in_ubi = {
    .add = ciu_mtd_notify_add,
};

static int __init cramfs_in_ubi_init(void)
{
    register_mtd_user(&cramfs_in_ubi);
    printk("Registered CRAMFS in UBI MTD notifier\n");
    return 0;
}
/* We register our init function before driver_initcalls so that we
 * receive all MTD notifications.
 */
rootfs_initcall(cramfs_in_ubi_init);
