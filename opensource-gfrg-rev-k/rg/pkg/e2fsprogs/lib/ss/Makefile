E2FSSRC=../..
include $(E2FSSRC)/envir.mak

JMK_A_TARGET=libss.a

JMK_O_OBJS_libss.a=ss_err.o std_rqs.o invocation.o help.o execute_cmd.o listen.o \
  parse.o error.o prompt.o request_tbl.o list_rqs.o pager.o requests.o data.o

ET_SRC=ss_err.c ss_err.h
ET=ss_err.et

CT_SRC=std_rqs.c
CT=std_rqs.ct

include $(E2FSMK)

archconfig:: $(CT_SRC) $(ET_SRC)

$(CT_SRC): $(CT)
	./mk_cmds $<

$(ET_SRC): $(ET)
	DIR=. ET_DIR=../et ../et/compile_et $<

