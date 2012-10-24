export DLMFS_DIR=$(DISK_IMAGE_DIR)/dlmfs_dir
export DLM_STAGING_DIR=$(JMKE_BUILDDIR)/pkg/build/dlm_staging

# $1 - source file + path
# $2 - destination file path
# $3 - name of the strip function
DLM_CP_FUNC = \
  $(JMKE_MKDIR) $(dir $2) && \
  rm -f $2 && \
  $(JMKE_CP) -L $1 $2 && \
  $(call $3,$2)
