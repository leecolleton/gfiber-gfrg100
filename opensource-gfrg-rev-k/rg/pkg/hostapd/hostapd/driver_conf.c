/* LICENSE JGPL */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "hostapd.h"
#include "driver.h"

#ifdef CONFIG_DRIVER_MTLK
void mtlk_driver_register(void);
#endif

#ifdef CONFIG_DRIVER_MADWIFI
void atheros_driver_register(void);
#endif

void register_drivers(void)
{
#ifdef CONFIG_DRIVER_MTLK
    mtlk_driver_register();
#endif
    
#ifdef CONFIG_DRIVER_MADWIFI
    atheros_driver_register();
#endif
}

