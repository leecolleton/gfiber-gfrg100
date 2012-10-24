ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_EXPORT_HEADERS=ec.h
JMK_INTERNAL_HEADERS=ec_lcl.h

JMK_O_OBJS=ec_lib.o ecp_smpl.o ecp_mont.o ecp_nist.o ec_cvt.o ec_mult.o \
	ec_err.o ec_curve.o ec_check.o ec_print.o ec_asn1.o ec_key.o \
	ec2_smpl.o ec2_mult.o ec_ameth.o ec_pmeth.o eck_prn.o \
	ecp_nistp224.o ecp_nistp256.o ecp_nistp521.o ecp_nistputil.o \
	ecp_oct.o ec2_oct.o ec_oct.o

$(call JMKE_INCLUDE_RULES)

