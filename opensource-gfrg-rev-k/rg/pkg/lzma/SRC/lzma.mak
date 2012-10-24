LOCAL_OBJS=$(OBJS:%.o=local_%.o)

JMK_OTHER_TASKS=$(LOCAL_OBJS)
JMK_CLEAN=$(LOCAL_OBJS)

JMK_LOCAL_CFLAGS+=-D_NO_EXCEPTIONS

include $(JMKE_MK)

$(LOCAL_OBJS) : local_%.o : %.cpp
	$(CXX_FOR_BUILD) $(JMK_LOCAL_CFLAGS) $(JMK_CFLAGS_$@) -I$(JMK_ROOT)/pkg/include -c -o $@ $<
