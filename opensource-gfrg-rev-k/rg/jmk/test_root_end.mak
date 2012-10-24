UNITTEST_OPT=$(if $(CONFIG_RG_VALGRIND_LOCAL_TARGET),$(VALGRIND_CMD))

run_unittests:
	$(foreach f, $(sav_JMK_RUN_UNITTEST_DATA), \
	  $(JMKE_LN) $(call BUILD2SRC,$f) $f &&) true
	$(foreach t,$(sav_JMK_RUN_UNITTEST), \
	  cd $(dir $t) && \
	  $(UNITTEST_SPAWNER) $(UNITTEST_OPT) ./local_$(notdir $t) &&) true

run_tests: run_unittests
