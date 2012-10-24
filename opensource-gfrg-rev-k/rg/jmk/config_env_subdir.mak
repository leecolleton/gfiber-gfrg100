# allow archconfig targets to compile before headers are exported using
# the export_headers target (the alternative is to divide archconfig into 
# two passes over the whole tree; first for export_headers and second for
# archconfig targets)
ifdef JMKE_DOING_MAKE_CONFIG
  JMK_LOCAL_CFLAGS+=-I$(JMK_ROOT)/pkg/include
endif
