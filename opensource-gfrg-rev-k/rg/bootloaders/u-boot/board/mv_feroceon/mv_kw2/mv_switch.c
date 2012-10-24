#include <common.h>
#include <mvTypes.h>
#include <mvCommon.h>

// SMI API
extern MV_STATUS mvEthPhyRegRead(MV_U32 phyAddr, MV_U32 regOffs, MV_U16 *data);
extern MV_STATUS mvEthPhyRegWrite(MV_U32 phyAddr, MV_U32 regOffs, MV_U16 data);

#define mdio_read mvEthPhyRegRead
#define mdio_write mvEthPhyRegWrite

#define MULTI_PHY 0x4

#ifdef MULTI_PHY

// multi-chip addressing mode
static void switch_busy(void)
{
    u16 val;
    do { mdio_read(MULTI_PHY, 0x0, &val); } while((val & BIT15));
}

static u16 switch_read(u8 phy, u8 reg)
{
    u16 val;

    switch_busy();
    mdio_write(MULTI_PHY, 0x0, reg | (phy << 5) | BIT11 | BIT12 | BIT15);
    switch_busy();

    mdio_read(MULTI_PHY, 0x1, &val);
    return val;
}

static void switch_write(u8 phy, u8 reg, u16 val)
{
    switch_busy();
    mdio_write(MULTI_PHY, 0x1, val);
    mdio_write(MULTI_PHY, 0x0, reg | (phy << 5) | BIT10 | BIT12 | BIT15);
}

#else

// single-chip addressing mode
#define switch_write mdio_write
#define switch_read mdio_read

#endif

#define PORT_NUM 7
#define PHYS_NUM 5
#define CPU_PORT 5
// #define WAN_PORT 4
#define PHY_PHY 0x00
#define MAC_PHY 0x10
#define SWITCH_PHY 0x1b

#define FLD(v, m, o) (((v) & ((m) << (o))) >> (o))

typedef struct {
	char *label;
	int dir;
	int stats[PORT_NUM];
} mv_switch_stats_t;

static mv_switch_stats_t mv_switch_stats[] = {
	[0x00] = {"InGoodOctetsLo", 0},
	[0x01] = {"InGoodOctetsHi", 0},
	[0x02] = {"InBadOctets", 0},
	[0x03] = {"OutFCSErr", 1},
	[0x04] = {"InUnicast", 0},
	[0x05] = {"Deferred", 1},
	[0x06] = {"InBroadcasts", 0},
	[0x07] = {"InMulticasts", 0},
	[0x08] = {"64 Octets", 2},
	[0x09] = {"65 to 127 Octets", 2},
	[0x0A] = {"128 to 255 Octets", 2},
	[0x0B] = {"256 to 511 Octets", 2},
	[0x0C] = {"512 to 1023 Octets", 2},
	[0x0D] = {"1024 to Max Octets", 2},
	[0x0E] = {"OutOctetsLo", 1},
	[0x0F] = {"OutOctetsHi", 1},
	[0x10] = {"OutUnicast", 1},
	[0x11] = {"Excessive", 1},
	[0x12] = {"OutMulticasts", 1},
	[0x13] = {"OutBroadcasts", 1},
	[0x14] = {"Single", 1},
	[0x15] = {"OutPause", 1},
	[0x16] = {"InPause", 0},
	[0x17] = {"Multiple", 1},
	[0x18] = {"InUndersize", 0},
	[0x19] = {"InFragments", 0},
	[0x1A] = {"InOversize", 0},
	[0x1B] = {"InJabber", 0},
	[0x1C] = {"InRxErr", 0},
	[0x1D] = {"InFCSErr", 0},
	[0x1E] = {"Collisions", 1},
	[0x1F] = {"Late", 1},
};

static char *mv_switch_dirs[] = {
	[0] = "Ingress",
	[1] = "Egress",
	[2] = "Histogram",
};

static void mv_switch_busy(u8 phy, u8 reg)
{
    u16 val;
    do { val = switch_read(phy, reg); } while((val & BIT15));
}

static u16 mv_switch_phy_read(u8 phy, u8 reg)
{
    u16 val;

    mv_switch_busy(0x1C, 0x18);
    switch_write(0x1c, 0x18, reg | (phy << 5) | BIT11 | BIT12 | BIT15);
    mv_switch_busy(0x1C, 0x18);

    val = switch_read(0x1C, 0x19);
    return val;
}

static void mv_switch_phy_write(u8 phy, u8 reg, u16 val)
{
    mv_switch_busy(0x1C, 0x18);
    switch_write(0x1C, 0x19, val);
    switch_write(0x1C, 0x18, reg | (phy << 5) | BIT10 | BIT12 | BIT15);
}

static int mv_switch_stat_get(int ptr, int port)
{
    int stat;

    mv_switch_busy(0x1B, 0x1D);
    switch_write(0x1B, 0x1D, 0x1 << 15 | 0x4 << 12 | (port + 1) << 5 | ptr);
    mv_switch_busy(0x1B, 0x1D);

    stat = switch_read(0x1B, 0x1E) << 16 | switch_read(0x1B, 0x1F);
    return stat;
}

static void mv_switch_stats_update(void)
{
    int ptr, port;

    for (ptr = 0; ptr < 0x1F; ptr++)
	for (port = 0; port < PORT_NUM; port++)
	    mv_switch_stats[ptr].stats[port] = mv_switch_stat_get(ptr, port);
}

static void mv_switch_stats_print(void)
{
    int dir, ptr, port;

    for (dir = 0; dir < 3; dir++)
    {
	printf("\n%s:\n", mv_switch_dirs[dir]);

	for (ptr = 0; ptr < 0x1F; ptr++)
	{
	    if (mv_switch_stats[ptr].dir != dir)
		continue;

	    printf("%20s:", mv_switch_stats[ptr].label);
	    for (port = 0; port < PORT_NUM; port++)
		printf("\t%d", mv_switch_stats[ptr].stats[port]);
	    printf("\n");
	}
    }
}

static void __init(void)
{
    int port;
    u16 val;

    printf("Initializing Ethernet switch\n");

    for (port = 0; port < PORT_NUM; port++)
    {
	/* LED config */
	if (port < PHYS_NUM)
	{
	    switch_write(MAC_PHY + port, 0x16, 0x8071);
	}

	/* enable RGMII delay on MoCA and CPU ports */
	if (port == 5 /*&& port == 6*/)
	{
	    switch_write(MAC_PHY + port, 0x1, 0xC0FE);
	}

	/* keep unused and MoCA port disabled */
	if (port != 4 && port != 6)
	{
	    val = switch_read(MAC_PHY + port, 0x4);
	    val |= 0x3;
	    switch_write(MAC_PHY + port, 0x4, val);
	}
    }
}

static void switch_reset(void)
{
    __init();
}

static void switch_info(void)
{
    int port, val;

    for (port = 0; port < PORT_NUM; port++)
    {
#define PV(lbl, fmt, v) printf(lbl ":" fmt ", ", v)
#define PFLD(lbl, v, m, o) PV(lbl, "%01x", FLD(v, m, o))
#define PP(lbl, v) PV(lbl, "%04x", v)
        printf("port%d: ", port);

	val = switch_read(MAC_PHY + port, 0x04);
	PFLD("S", val, 3, 0);

	val = switch_read(MAC_PHY + port, 0x00);
	PFLD("LinkPause", val, 1, 15);
	PFLD("MyPause", val, 1, 14);
	PFLD("Link", val, 1, 11);
	PFLD("Duplex", val, 1, 10);
	PFLD("Speed", val, 0x3, 8);
	PFLD("EEE", val, 1, 6);
	PFLD("TxPaused", val, 1, 5);
	PFLD("FlowCtrl", val, 1, 4);
	PFLD("Mode", val, 0x7, 0);
	printf("\n");
    }
}

static int do_switch(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (!strncmp (argv[1], "ini", 3) && argc == 2)
    {
	__init();
	return 0;
    }

    if (!strncmp (argv[1], "i", 1) && argc == 2)
    {
	switch_info();
	return 0;
    }

    if (!strncmp (argv[1], "r", 1) && argc == 2)
    {
	switch_reset();
	return 0;
    }

    if (!strcmp (argv[1], "stats") && argc == 2)
    {
	mv_switch_stats_update();
	mv_switch_stats_print();
	return 0;
    }

    if (!strcmp (argv[1], "read") && argc == 4)
    {
    	int phy = simple_strtoul (argv[2], NULL, 16);
    	int reg = simple_strtoul (argv[3], NULL, 16);
	int val = switch_read(phy, reg);
    	printf("switch: read @%02x,%02x = %04x\n", phy, reg, val);
	return 0;
    }

    if (!strcmp (argv[1], "write") && argc == 5)
    {
    	int phy = simple_strtoul (argv[2], NULL, 16);
    	int reg = simple_strtoul (argv[3], NULL, 16);
	int val = simple_strtoul (argv[4], NULL, 16);
	switch_write(phy, reg, val);
	int val2 = switch_read(phy, reg);
    	printf("switch: write @%02x,%02x = %04x(%04x)\n", phy, reg, val, val2);
	return 0;
    }
    if (!strcmp (argv[1], "phy_read") && argc == 4)
    {
    	int phy = simple_strtoul (argv[2], NULL, 16);
    	int reg = simple_strtoul (argv[3], NULL, 16);
	int val = mv_switch_phy_read(phy, reg);
    	printf("switch: PHY read @%02x,%02x = %04x\n", phy, reg, val);
	return 0;
    }

    if (!strcmp (argv[1], "phy_write") && argc == 5)
    {
    	int phy = simple_strtoul (argv[2], NULL, 16);
    	int reg = simple_strtoul (argv[3], NULL, 16);
	int val = simple_strtoul (argv[4], NULL, 16);
	mv_switch_phy_write(phy, reg, val);
	int val2 = mv_switch_phy_read(phy, reg);
    	printf("switch: PHY write @%02x,%02x = %04x(%04x)\n", phy, reg, val, val2);
	return 0;
    }

    printf ("Usage:\n%s\n", cmdtp->usage);
    return 1;
}

U_BOOT_CMD(switch, 5, 1, do_switch, "switch - Ethernet switch sub-system\n",
    "\t[info|reg_write|reg_read]\n"
    "\tinfo - dispaly switch info\n"
    "\treset - reset and reinitialize the switch\n"
    "\tstats - print statistics\n"
    "\twrite phy addr value - write switch register (up to 64bit)\n"
    "\tread phy addr - read switch register (up to 64bit)\n");


int mv_switch_init(void)
{
    __init();
    return 0;
}

