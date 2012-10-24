JMK_ROOT=../../..
include $(JMK_ROOT)/envir.mak

JMK_SUBDIRS=Common 7zip/Compress/LZMA 7zip/Compress/LZ 7zip/Compress/RangeCoder \
  7zip/Common 7zip 7zip/Compress/LZMA_C 7zip/Compress/LZMA_Alone

LZMA_LIB=liblocal_lzma.a
JMK_EXPORT_LIBS+=$(LZMA_LIB)
LZMA_LIB_OBJS=7zip/Compress/LZ/LZInWindow.o 7zip/Compress/LZ/LZOutWindow.o \
  7zip/Compress/LZMA/LZMADecoder.o 7zip/Compress/LZMA/LZMAEncoder.o \
  7zip/Compress/RangeCoder/RangeCoderBit.o 7zip/Common/InBuffer.o \
  7zip/Common/OutBuffer.o 7zip/Common/FileStreams.o Common/C_FileIO.o \
  Common/Alloc.o Common/String.o Common/CRC.o \
  Common/StringConvert.o Common/StringToInt.o Common/Vector.o 7zip/lzma_encode.o

JMK_LOCAL_A_TARGET=$(LZMA_LIB)
JMK_L_OBJS_$(LZMA_LIB)=$(LZMA_LIB_OBJS)
JMK_JPKG_EXPORTED_FILES+=lzma.mak

LZMA_BIN=lzma
JMK_LOCAL_CXX_TARGET=$(LZMA_BIN)
JMK_L_OBJS_$(LZMA_BIN)=Common/CommandLineParser.o \
  7zip/Compress/LZMA_Alone/LzmaAlone.o 7zip/Compress/LZMA_Alone/LzmaBench.o

JMK_LOCAL_LDLIBS:=-llocal_lzma $(JMK_LOCAL_LDLIBS)

include $(JMKE_MK)
