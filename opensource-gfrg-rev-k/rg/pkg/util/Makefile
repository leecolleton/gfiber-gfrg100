ifndef JMKE_ENV_INCLUDED
  JMK_ROOT=../../
  include $(JMK_ROOT)/jmk/env_root.mak
endif

JMK_JPKG_TARGET_BIN_jspawn=y
JMK_A_TARGET_PIC=$(JMK_A_TARGET)

SQL_H_FILES=dbi.h dbi_int.h dbi_helper.h dbi_init.h sql.h str_sql.h \
 sql_html_edit.h sql_print.h sql_util.h sql_mysql.h session_sql.h sql_query.h \
 sql_query_util.h

SQL_O_FILES=dbi_result.o dbi_helper.o dbi.o dbi_init.o sql.o \
  sql_file.o sql_html_edit.o sql_print.o sql_util.o str_sql.o sql_query.o \
  sql_query_util.o \
  $(if $(HAVE_MYSQL),$(if $(CONFIG_RG_JPKG_SRC),,sql_mysql.o)) \
  $(if $(HAVE_ODBC),sql_odbc.o)

# We need to compile the language compiler as stand alone, because we generate
# the language header files in "make config" stage
ifndef CONFIG_RG_GPL
  ifdef CONFIG_RG_LANG
    JMK_LOCAL_TARGET+=$(if ($(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG)),lang_compiler)
    JMK_O_OBJS_lang_compiler+=lang_compiler.o
    JMK_CFLAGS_local_lang_compiler.o+=-DLANG_COMPILER_STANDALONE \
      -I$(JMKE_PWD_SRC) -I$(JMKE_PWD_BUILD)
    JMK_CFLAGS_REMOVE_local_lang_compiler.o+=-Wstrict-prototypes
    JMK_JPKG_TARGET_BIN_lang_compiler=y
  endif
  JMK_LOCAL_TARGET+=$(if ($(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG)),text2c)
  JMK_O_OBJS_text2c+=text2c.o
  JMK_JPKG_TARGET_BIN_text2c=y

  ifdef CONFIG_OPENRG
    JMK_TARGET+=jdd
    JMK_O_OBJS_jdd=jdd.o
    JMK_RAMDISK_BIN_FILES+=jdd
    JMK_LOCAL_TARGET+=local_jdd
    JMK_O_OBJS_local_jdd=jdd.o
  endif
endif

ifdef CONFIG_RG_IR_TEST
  JMK_TARGET=tty_write
  JMK_O_OBJS_tty_write=tty_write.o
endif

JMK_CD_EXPORTED_FILES+=dep_l.mak test_lang_.csv
JMK_CD_EXPORTED_FILES+=kos_26_mod.c
JMK_EXPORT_AS_SRC+=enums.c mkconfig.c rg_version.c rg_version_jungo.c \
  openrg_gpl.c rtnetlink_address.c rtnetlink_route.c rtnetlink.c \
  rtnetlink_base.c rg_openlog.c rg_error_syslog.c rg_resolv.c
JMK_INTERNAL_HEADERS+=estream_int.h code2str_ex_test.h getdate.h range_set.h 
JMK_INTERNAL_HEADERS+=$(if $(CONFIG_RG_SESSION_MEMORY),session_mem.h)

# Used by create_config
JMK_EXPORT_AS_SRC+=str.c
# Used by test utility
JMK_INTERNAL_HEADERS+=test.c jtest_util.c jtest_util.h matrix_int.h test_ipcd.c test_ipc.c

JMK_LOCAL_TARGET+=mkconfig
MGT_LIB=libmgt_client.a
JMK_O_OBJS_mkconfig+=mkconfig.o
JMK_JPKG_TARGET_BIN_mkconfig=y

JMK_EXPORT_LIBS+=$(LIB_RG) $(LIB_JUTIL) $(LOCAL_LIB_RG) $(LOCAL_LIB_JUTIL) \
  $(LIB_GPL) $(MGT_LIB) $(CONFIG_LIB) $(CONFIG_LOCAL_LIB) \
  $(LIB_SESSION) $(LOCAL_LIB_SESSION)

JMK_EXPORT_HEADERS_DIR=util/
JMK_EXPORT_HEADERS+=estream_ssl.h rg_ssl.h clock.h base64.h etask.h \
  state_machine.h conv.h rg_system.h rg_time.h tftp_utils.h \
  openrg_gpl.h file.h date.h str.h strconv.h rg_checksum.h \
  rg_memory.h rg_random.h dot11.h eresolv.h names.h rg_error.h rg_print.h \
  rg_error_crypto.h memarea.h rg_error_rate.h util.h snprintf.h strace.h \
  alloc.h lines.h attrib.h attrib_io.h console_colors.h csv.h file_util.h \
  fnmatch_ext.h hash.h html.h jftw.h keyword.h mail.h md5.h proc.h \
  sed.h sort.h rg_openlog.h rg_error_conv.h user_kernel_util.h str_escape.h \
  str_regex.h str_util.h sys.h term.h testutil.h csv_util.h jtest.h parse.h \
  uml.h rg_serial.h net_wait.h mgt_client.h linked_list.h rg_auth.h \
  http_auth.h matrix.h matrix_io.h lang.h rtnetlink_base.h rtnetlink.h \
  rtnetlink_route.h rg_chardev.h queue.h str_iconv.h erecv.h iptables.h \
  range_set.h session.h session_utils.h rg_error_syslog.h session_context.h \
  zip.h netlink.h $(if $(CONFIG_RG_SQL),$(SQL_H_FILES)) jdd.h http_util.h \
  rg_resolv.h rg_list_p.h rtnetlink_address.h iaddr.h

JMK_LOCAL_TARGET+=jspawn
JMK_O_OBJS_jspawn=jspawn.o
JMK_LIBS_jspawn:=$(JMKE_PWD_BUILD)/liblocal_jutil.a $(JMK_LIBS_jspawn)

ifdef CONFIG_RG_LANG
ifeq ($(CONFIG_RG_JPKG_SRC),)
  JMK_LOCAL_TARGET+=local_test
  JMK_RUN_UNITTEST+=test
  JMK_O_OBJS_local_test+=test.o jtest_util.o test_lang_.o test_lang_static.o \
    range_set.o 
  JMK_LIBS_local_test:=$(JMKE_PWD_BUILD)/liblocal_jutil.a $(JMK_LIBS_local_test)
  ifeq ($(CONFIG_OPENRG),y)
    JMK_O_OBJS_local_test_ipcd=test_ipcd.o
    JMK_LIBS_local_test_ipcd=$(JMKE_PWD_BUILD)/liblocal_openrg.a \
      $(JMKE_PWD_BUILD)/liblocal_jutil.a \
      $(JMKE_PWD_BUILD)/liblocal_rg_config.a
    JMK_O_OBJS_local_test_ipc=test_ipc.o
    JMK_LIBS_local_test_ipc=$(JMKE_PWD_BUILD)/liblocal_openrg.a \
      $(JMKE_PWD_BUILD)/liblocal_jutil.a
  endif
  ifeq ($(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY)-$(CONFIG_OPENRG),-y)
    JMK_LIBS_local_test:=$(JMKE_PWD_BUILD)/liblocal_openrg.a \
      $(JMK_LIBS_local_test)
  endif
  ifeq ($(HAVE_MYSQL),y)
    JMK_LIBS_local_test:=__local_mysqlclient $(JMK_LIBS_local_test)
  endif
  JMK_LIBS_test:=$(OPENRG_LIBS) $(JMK_LIBS_test)
  JMK_LDFLAGS_REMOVE_local_test+=--static
endif
endif

JMK_RUN_UNITTEST_DATA+=test.csv test.sql test.tab test_expand.csv \
  test_lang_.csv attrib_test.txt

JMK_CD_EXPORTED_FILES+=$(JMK_RUN_UNITTEST_DATA)
		   
JMK_EXPORT_HEADERS+=rg_error.c__c/rg_error.c rg_error_rate.c__c/rg_error_rate.c \
  conv.c__c/conv.c ip_proto_name.c__c/ip_proto_name.c \
  inet_conv.c__c/inet_conv.c parse.c__c/parse.c str_util.c__c/str_util.c \
  rg_print.c__c/rg_print.c alloc.c__c/alloc.c lines.c__c/lines.c \
  rg_chardev.c__c/rg_chardev.c user_kernel_util.c__c/user_kernel_util.c \
  iaddr.c__c/iaddr.c

JMK_SUBDIRS+=eresolv

ifdef CONFIG_DYN_LINK
  LIB_RG=libopenrg.so
  LIB_JUTIL=libjutil.so
  CONFIG_LIB=librg_config.so
  JMK_SO_TARGET=$(if $(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG), \
    $(LIB_RG)) $(LIB_JUTIL) $(CONFIG_LIB)
  JMK_RAMDISK_LIB_FILES+=$(JMK_SO_TARGET)
else
  LIB_RG=libopenrg.a
  LIB_JUTIL=libjutil.a
  CONFIG_LIB=librg_config.a
  JMK_A_TARGET=$(if $(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG), \
    $(LIB_RG)) $(LIB_JUTIL) $(CONFIG_LIB)
endif

JMK_JPKG_TARGET_liblocal_rg_config.a=y
CONFIG_LOCAL_LIB=liblocal_rg_config.a
JMK_LOCAL_A_TARGET+=$(CONFIG_LOCAL_LIB)
JMK_O_OBJS_$(CONFIG_LIB)=rg_c_config.o
JMK_O_OBJS_$(CONFIG_LOCAL_LIB)+=rg_c_config.o
JMK_AUTOGEN_SRC=rg_c_config.c test_lang_static.c test_lang_.c
JMK_DONT_EXPORT+=$(JMKE_PWD_BUILD)/local_rg_c_config.o \
  $(JMKE_PWD_BUILD)/rg_c_config_pic.o

JMK_A_TARGET+=$(if $(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG),$(MGT_LIB))
JMK_O_OBJS_$(MGT_LIB)=mgt_client.o

ifneq ($(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG),)
ifneq ($(CONFIG_RG_OS_LINUX)$(CONFIG_RG_JPKG_SRC)$(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY),)
# Note: Any change here, should be done also in Makefile.gpl
  ifdef CONFIG_DYN_LINK
    LIB_GPL=libopenrg_gpl.so
    JMK_SO_TARGET+=$(LIB_GPL)
    ifeq ($(CONFIG_RG_SYSLOG_GLIBC),y)
      JMK_LIBS_$(LIB_GPL)+=$(JMKE_BUILDDIR)/pkg/syslog/glibc/libulibc_syslog.a
    endif
  else
    LIB_GPL=libopenrg_gpl.a
    JMK_A_TARGET+=$(LIB_GPL)
  endif
  JMK_EXPORT_LIBS+=$(LIB_GPL)
  JMK_O_OBJS_$(LIB_GPL)=openrg_gpl.o ipc.o bio_socket.o rg_memory.o \
      rg_openlog.o str.o rg_error_syslog.o \
      $(if $(CONFIG_RG_DBG_ULIBC_MALLOC),,alloc.o)
  ifneq ($(CONFIG_RG_JPKG_SRC)-$(CONFIG_RG_RGLOADER),-y)
      JMK_O_OBJS_$(LIB_GPL)+=rg_resolv.o
  endif
endif # CONFIG_RG_OS_LINUX CONFIG_RG_JPKG_SRC
endif # CONFIG_OPENRG CONFIG_RG_RGLOADER

LOCAL_LIB_RG=liblocal_openrg.a
LOCAL_LIB_JUTIL=liblocal_jutil.a
JMK_LIBS_$(LOCAL_LIB_RG):=$(JMKE_PWD_BUILD)/liblocal_rg_config.a \
  $(JMK_LIBS_$(LOCAL_LIB_RG))
JMK_JPKG_TARGET_BIN_liblocal_openrg.a=y
JMK_JPKG_TARGET_liblocal_jutil.a=y

JMK_LOCAL_A_TARGET+=$(if $(CONFIG_WINDOWS_HOST),,\
  $(if $(CONFIG_RG_RGLOADER)$(CONFIG_OPENRG)$(CONFIG_AUTOTEST),$(LOCAL_LIB_RG)) $(LOCAL_LIB_JUTIL))
  
OBJS_COMMON_RG+=rg_iplist.o rg_list.o ipc.o async_ipc.o \
  rg_error_rate.o rg_random.o bio_socket.o stop.o etask.o \
  memarea.o ebuf.o ip_proto_name.o
ifndef CONFIG_AUTOTEST
  OBJS_COMMON_RG+=syscalls.o rg_chardev.o
endif

ifeq ($(CONFIG_GLIBC)$(GLIBC_HAVE_FORK),yy)
  JMK_JPKG_BIN_LIBCS_daemon_init.o+=GLIBC
  OBJS_COMMON_RG+=daemon_init.o
endif
ifeq ($(CONFIG_ULIBC)$(ULIBC_HAVE_FORK),yy)
  JMK_JPKG_BIN_LIBCS_daemon_init.o+=ULIBC
  OBJS_COMMON_RG+=daemon_init.o
endif

OBJS_COMMON_RG+=$(if $(CONFIG_HW_CLOCK),clock.o) \
  $(if $(CONFIG_RG_UML)$(CONFIG_RG_JPKG_SRC)$(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY),uml.o) \
  $(if $(CONFIG_RG_SERIAL),rg_serial.o) $(if $(CONFIG_RG_AUTH),rg_auth.o)

OBJS_COMMON_JUTIL+=event.o array.o set.o set_utils.o net_utils.o str.o \
  lines.o conv.o rg_checksum.o rg_memory.o base64.o rg_time.o attrib.o \
  proc.o str_escape.o inet_conv.o str_regex.o sys.o file_util.o str_util.o \
  fnmatch_ext.o jftw.o lang.o strconv.o estream.o rg_print.o erecv.o \
  estream_utils.o html.o net_wait.o snprintf.o \
  rtnetlink_base.o syscalls_base.o names.o dot11.o \
  rg_openlog.o rg_error_conv.o user_kernel_util.o \
  range_set.o estream_throttle.o enums.o tftp_utils.o date.o \
  rg_set_utils.o netlink.o http_util.o iaddr.o \
  $(if $(CONFIG_OPENRG)$(CONFIG_RG_BUILD_LOCAL_TARGETS_ONLY),sed.o) \
  $(if $(CONFIG_RG_CRYPTO),rg_error_crypto.o) \
  $(if $(CONFIG_RG_OPENSSL),rg_ssl.o estream_ssl.o) \
  $(if $(CONFIG_RG_OPENSSL_MD5),obscure.o http_auth.o) \
  $(if $(CONFIG_RG_DBG_ULIBC_MALLOC),,alloc.o) \
  $(JMKE_O_LANG_FILES)
 
ifndef CONFIG_RG_JPKG_SRC
    # zip.c is compiled for RG (target & local) and Jnet server
    OBJS_COMMON_JUTIL+=$(if $(strip $(CONFIG_OPENRG)$(CONFIG_RG_JNET_SERVER)\
	$(CONFIG_RG_RGLOADER)),zip.o)
else
    JMK_CD_EXPORTED_FILES+=zip.c
endif

ifdef CONFIG_RG_SESSION_MYSQL
  OBJS_LIBSESSION+=session_sql.o
  JMK_LOCAL_LDFLAGS:=$(filter-out --static,$(JMK_LOCAL_LDFLAGS))
endif
ifdef CONFIG_RG_SESSION_MEMORY
  OBJS_LIBSESSION+=session_mem.o session_context.o
endif
ifneq ($(CONFIG_RG_SESSION_MYSQL)$(CONFIG_RG_SESSION_MEMORY),)
  LIB_SESSION=libsession.a
  LOCAL_LIB_SESSION=liblocal_session.a
  JMK_A_TARGET+=$(LIB_SESSION)
  JMK_LOCAL_A_TARGET+=$(LOCAL_LIB_SESSION)
  OBJS_LIBSESSION+=session.o session_utils.o
  JMK_O_OBJS_$(LIB_SESSION)+=$(OBJS_LIBSESSION)
  JMK_O_OBJS_$(LOCAL_LIB_SESSION)+=$(OBJS_LIBSESSION)
endif

ifndef CONFIG_LSP_DIST
  OBJS_COMMON_JUTIL+=rg_version.o rg_version_jungo.o jtest.o \
    keyword.o attrib_io.o sort.o testutil.o parse.o rg_error.o rg_random.o
endif

LINUX_OBJS+=rtnetlink.o rtnetlink_address.o rtnetlink_route.o rg_system.o \
  process_funcs.o

JMK_O_OBJS_$(LIB_RG)+=$(OBJS_COMMON_RG) \
  $(if $(CONFIG_RG_TARGET_LINUX),$(LINUX_OBJS))
  
JMK_O_OBJS_$(LIB_JUTIL)+=$(OBJS_COMMON_JUTIL) \
  $(if $(CONFIG_RG_TARGET_LINUX),file.o) rg_error_syslog.o \
  $(if $(CONFIG_TARGET_EVENT_EPOLL),event_epoll.o) \
  $(if $(CONFIG_TARGET_EVENT_POLL),event_poll.o) \
  $(if $(CONFIG_TARGET_EVENT_SELECT),event_select.o)
JMK_O_OBJS_$(LOCAL_LIB_RG)+=$(OBJS_COMMON_RG) $(LINUX_OBJS)

JMK_O_OBJS_$(LOCAL_LIB_JUTIL)+=$(OBJS_COMMON_JUTIL) rg_system.o csv.o csv_util.o \
  file.o mail.o md5.o term.o matrix.o matrix_io.o getdate.o str_iconv.o \
  str_util.o iptables.o sed.o user_kernel_util.o process_funcs.o \
  $(if $(CONFIG_LOCAL_EVENT_EPOLL),event_epoll.o) \
  $(if $(CONFIG_LOCAL_EVENT_POLL),event_poll.o) \
  $(if $(CONFIG_LOCAL_EVENT_SELECT),event_select.o) \
  $(if $(CONFIG_RG_DBG_ULIBC_MALLOC),alloc.o)

ifdef CONFIG_RG_SQL
  JMK_O_OBJS_$(LOCAL_LIB_JUTIL)+=$(SQL_O_FILES)
  JMK_CD_EXPORTED_FILES+=$(if $(CONFIG_RG_JPKG_SRC),sql_mysql.c)
endif

ifdef CONFIG_RG_LIBICONV
  JMK_O_OBJS_$(LIB_JUTIL)+=str_iconv.o
  JMK_LIBS_$(LIB_JUTIL)+=$(JMKE_BUILDDIR)/pkg/libiconv/libiconv.a
endif

ifeq ($(CONFIG_OPENRG),)
  JMK_O_OBJS_$(LOCAL_LIB_JUTIL)+=rg_version.o rg_version_jungo.o attrib_io.o date.o jtest.o base64.o
endif  

# There are c files which are included from inside lang_compiler (e.g. file.c),
# but are not exported to the JMKE_BUILDDIR at that stage:
JMK_CFLAGS_local_lang_compiler.o+=-I$(JMK_ROOT)/pkg/include
JMK_CFLAGS_local_lang_compiler.o+=-I$(JMK_ROOT)/pkg/util -I$(JMK_ROOT)/pkg

JMK_CFLAGS_REMOVE_local_getdate.o+=-Wstrict-prototypes
JMK_CFLAGS_snprintf.o+=$(if $(CONFIG_CC_HAVE_VA_COPY),-DHAVE_VA_COPY)

ifdef CONFIG_RG_DATE
  JMK_TARGET+=rg_date
  JMK_O_OBJS_rg_date=rg_date.o
  JMK_LIBS_rg_date:=$(OPENRG_LIBS) $(JMK_LIBS_rg_date)
  JMK_EXPORT_AS_SRC+=rg_date.c
endif

ifdef CONFIG_RG_EXEC_CLI_CMD
  JMK_TARGET+=exec_cli_cmd
  JMK_O_OBJS_exec_cli_cmd=exec_cli_cmd.o
  JMK_LIBS_exec_cli_cmd=$(JMKE_PWD_BUILD)/libmgt_client.a $(OPENRG_LIBS)
  JMK_RAMDISK_BIN_FILES+=exec_cli_cmd
endif

ifndef CONFIG_RG_GPL
  JMK_AUTOGEN_SRC+=getdate.c
  JMK_INTERNAL_HEADERS+=getdate.y
  ifneq ("$(wildcard $(JMKE_PWD_SRC)/getdate.y)","")
    $(JMKE_PWD_BUILD)/local_lang_compiler.o: $(JMKE_PWD_BUILD)/getdate.c
  endif
  JMK_ARCHCONFIG_JPKG_LAST_TASKS+=$(JMKE_PWD_BUILD)/text2c

$(JMKE_PWD_BUILD)/local_test.o $(JMKE_PWD_BUILD)/test.o: \
  $(JMKE_PWD_BUILD)/test_lang_.h $(JMKE_PWD_BUILD)/test_lang_static.h

$(JMKE_PWD_BUILD)/test_lang_static.csv: $(JMKE_PWD_SRC)/test_lang_.csv
	# the following should have been 'V\?T', but this ('?')
	# does not seem to work on SunOS's /bin/sed, so it was replaced
	# by 'V*T', which is close enough for this purpose.
	sed 's/^\(V*T[^,]*\),/\1_static,/g' < $< > $@

# We need explicit make rules for the language test files, since we need to
# compile them with the --static option on/off, no matter if CONFIG_RG_LANG_TEST
# is set.
$(JMKE_PWD_BUILD)/test_lang_.h: $(JMKE_PWD_SRC)/test_lang_.csv $(LANG_COMPILER)
	$(LANG_COMPILER) csv2h test $< $@

$(JMKE_PWD_BUILD)/test_lang_.c: $(JMKE_PWD_SRC)/test_lang_.csv $(LANG_COMPILER)
	$(LANG_COMPILER) csv2c test $< $@

$(JMKE_PWD_BUILD)/test_lang_static.h: $(JMKE_PWD_BUILD)/test_lang_static.csv \
   $(LANG_COMPILER)
	$(LANG_COMPILER) --static csv2h test_static $< $@

$(JMKE_PWD_BUILD)/test_lang_static.c: $(JMKE_PWD_BUILD)/test_lang_static.csv \
   $(LANG_COMPILER)
	$(LANG_COMPILER) --static csv2c test_static $< $@
else
JMK_LOCAL_TARGET:= $(filter %config,$(JMK_LOCAL_TARGET))    
JMK_TARGET:= $(filter %config %gpl,$(JMK_TARGET))    
JMK_SO_TARGET:= $(filter %config.so %gpl.so,$(JMK_SO_TARGET))    
JMK_A_TARGET:= $(filter %config.a %gpl.a $(MGT_LIB) ,$(JMK_A_TARGET))    
JMK_LOCAL_A_TARGET:= $(filter %config.a %gpl.a,$(JMK_LOCAL_A_TARGET))
JMK_EXPORT_LIBS:= $(filter %config.a %gpl.a %config.so %gpl.so $(MGT_LIB) ,$(JMK_EXPORT_LIBS))  
JMK_SUBDIRS=
endif

ifdef CONFIG_RG_JSMIN
  JMK_LOCAL_TARGET+=jsmin
  JMK_O_OBJS_jsmin=jsmin.o
endif

$(call JMKE_INCLUDE_RULES)
