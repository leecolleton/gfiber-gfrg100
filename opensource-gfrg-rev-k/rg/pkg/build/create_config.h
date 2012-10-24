/****************************************************************************
 *
 * rg/pkg/build/create_config.h
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

#ifndef _CREATE_CONFIG_H_
#define _CREATE_CONFIG_H_

#ifdef CONFIG_RG_DO_DEVICES
#include <enums.h>
#include <dev_type.h>
#endif

#define UNUSED_VAR(var) ({__typeof__(var) __attribute__ ((unused))x = var;})

extern char *major_features_file, *major_features_cfile;
extern int is_evaluation;

#ifdef CONFIG_RG_DO_DEVICES
void _dev_add(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net);
void _dev_add_disabled(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net);
#define dev_add(name, type, net) _dev_add(__FILE__, __LINE__, name, type, net)
#define dev_add_disabled(name, type, net) _dev_add_disabled(__FILE__, \
    __LINE__, name, type, net)

void _dev_set_static_ip(char *file, int line, char *name, char *ip,
    char *netmask);
#define dev_set_static_ip(name, ip, netmask) _dev_set_static_ip( \
    __FILE__, __LINE__, name, ip, netmask)
void _dev_set_id(char *file, int line, char *name, int id);
#define dev_set_id(name, id) _dev_set_id(__FILE__, __LINE__, name, id)
void _dsl_atm_dev_add(char *file, int line, char *name, dev_if_type_t type,
    char *dsl_dev);
#define dsl_atm_dev_add(name, type, dsl_dev) \
    _dsl_atm_dev_add(__FILE__, __LINE__, name, type, dsl_dev)
void _dsl_ptm_dev_add(char *file, int line, char *name, dev_if_type_t type, 
    char *dsl_dev);
#define dsl_ptm_dev_add(name, type, dsl_dev) \
    _dsl_ptm_dev_add(__FILE__, __LINE__, name, type, dsl_dev)
void _switch_dev_add(char *file, int line, char *name, char *depend_on,
    dev_if_type_t type, logical_network_t net, int cpu_port);
#define switch_dev_add(name, depend_on, type, net, cpu_port) \
    _switch_dev_add(__FILE__, __LINE__, name, depend_on, type, net, cpu_port)
void _switch_virtual_port_dev_add(char *file, int line, char *name, char *sw,
    int port, logical_network_t net);
#define switch_virtual_port_dev_add(name, sw, port, net) \
    _switch_virtual_port_dev_add(__FILE__, __LINE__, name, sw, port, net)
void _gsm_dev_add(char *file, int line, char *name, dev_if_type_t type);
#define gsm_dev_add(name, type) _gsm_dev_add(__FILE__, __LINE__, name, type)
void _dev_add_desc(char *file, int line, char *name, dev_if_type_t type,
    logical_network_t net, char *d);
#define dev_add_desc(name, type, net, d) _dev_add_desc(__FILE__, __LINE__, \
    name, type, net, d)

void dev_add_uml_hw_switch(char *name, logical_network_t net, int cpu_port, ...);

/* Add bridge. Enslaved devices are specified as a NULL terminated list of
 * device names. If the device list is empty, the enslaved devices are 
 * determined automatically, by enslaving all ethernet devices with the same 
 * logical network as bridge.
 */
void dev_add_bridge(char *name, logical_network_t net, ...);

/* Enslave a device to an existing bridge */
void dev_add_to_bridge_if_opt(char *name, char *enslaved, char *opt_verify);
void dev_add_to_bridge(char *bridge, char *enslaved);

void dev_set_dependency(char *dev_name, char *depend_on);
void dev_set_logical_dependency(char *dev_name, char *depend_on);
void dev_can_be_missing(char *dev_name);
void dev_wl_net_type(char *dev_name, wl_net_type_t net_type);
void dev_set_web_auth(char *dev_name);
void dev_set_ap_vlan_id(char *name, int vid);

void dev_set_route_group(char *name, dev_route_group_t grp);

/* Given an underlying (physical) dev, create a variable number (one or more)
 * of virtual wans above it.
 * main_vwan - the virtual wan that will act as the main wan connection.
 * ... - list of additional virtual wans (NULL terminated).
 */
void dev_add_virtual_wans(char *underlying, char *main_vwan, ...);

void dev_open_conf_file(char *filename);
void dev_close_conf_file(void);
#else
#define _dev_add(...)
#define _dev_add_disabled(...)
#define  dev_add(name, type, net)  UNUSED_VAR(name)
#define  dev_add_disabled(name, type, net)  UNUSED_VAR(name)
#define  dev_set_static_ip(...)
#define dev_set_id(...)
#define _dsl_atm_dev_add(...)
#define  dsl_atm_dev_add(name, ...)  UNUSED_VAR(name)
#define _dsl_ptm_dev_add(...)
#define  dsl_ptm_dev_add(name, ...)  UNUSED_VAR(name)
#define _switch_dev_add(...)
#define switch_dev_add(name, depend_on, type, net, cpu_port) UNUSED_VAR(name)
#define _switch_virtual_port_dev_add(...)
#define switch_virtual_port_dev_add(name, sw, port, net) UNUSED_VAR(name)
#define dev_add_bridge(...)
#define dev_add_uml_hw_switch(...)
#define dev_add_to_bridge_if_opt(...)
#define dev_add_to_bridge(...)
#define dev_set_dependency(...)
#define dev_set_logical_dependency(...)
#define dev_can_be_missing(...)
#define dev_set_web_auth(...)
#define dev_set_ap_vlan_id(...)
#define dev_set_route_group(...)
#define dev_add_virtual_wans(...)
#define dev_open_conf_file(...)
#define dev_close_conf_file()
#define gsm_dev_add(...)
#define _dev_add_desc(...)
#define dev_add_desc(name, ...) UNUSED_VAR(name)
#endif

/* If 'is_big' is set to 1 turn on the flags for a big endian target,
 * otherwise turn on the flags for a little endian target.
 */
void _set_big_endian(char *file, int line, int is_big);
#define set_big_endian(is_big) _set_big_endian(__FILE__, __LINE__, is_big)

void hardware_features(void);
void distribution_features(void);
void create_device_list(void);
void openrg_features(void);
void package_features(void);
void general_features(void);
void hw_wireless_features(void);
void bridge_config(void);
void print_major_features(void);
void target_os_features(char *os);
void target_primary_os(void);
void target_os_enable_wireless(void);
void config_host(void);

/* set value of LIC token by distribution or default
 * return the value set to LIC or NULL if none */
char *set_dist_license(void);

void conf_err(const char *format, ...)
    __attribute__((__format__(__printf__, 1, 2)));
char *sys_get(int *ret, char *command, ...)
    __attribute__((__format__(__printf__, 2, 3)));

/* Allow module to be compiled into distribution, if a license exist */
void _enable_module(char *file, int line, char *name);
#define enable_module(m) _enable_module(__FILE__, __LINE__, m)

typedef struct jpkg_dist_t {
    struct jpkg_dist_t *next;
    char *dist;
    option_t *options;
} jpkg_dist_t;

extern jpkg_dist_t *jpkg_dists;

void jpkg_dist_add(char *dist);

#endif
