/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 */
#ifndef _IPSEC_COMPAT_H_
#define _IPSEC_COMPAT_H_

#ifdef CONFIG_RG_OS_LINUX_26
#define ipsec_register_sysctl_table(table, insert_at_head) \
    register_sysctl_table(table)
#else
#define ipsec_register_sysctl_table(table, insert_at_head) \
    register_sysctl_table(table, insert_at_head)
#endif
#define ipsec_unregister_sysctl_table(table) unregister_sysctl_table(table)

#ifdef CONFIG_RG_OS_LINUX_26
#define ipsec_module_param_int(name) module_param(name, int, 0)
#else
#define ipsec_module_param_int(name) MODULE_PARM(name, "i")
#endif

#endif
