#
# Component Makefile
#

COMPONENT_SRCDIRS := src
COMPONENT_ADD_INCLUDEDIRS := src/include

COMPONENT_OBJS := src/ps4.o src/ps4_spp.o src/ps4_parser.o src/ps4_gap.o

COMPONENT_EXTRA_INCLUDES +=     $(IDF_PATH)/components/bt/common/include/                     \
                                $(IDF_PATH)/components/bt/host/bluedroid/common/include/      \
                                $(IDF_PATH)/components/bt/host/bluedroid/stack/include        \
                                $(IDF_PATH)/components/bt/host/bluedroid/stack/gap/include    \
                                $(IDF_PATH)/components/bt/host/bluedroid/osi/include

# Legacy bluedroid location include paths
# i.e. ESP-IDF prior to revision 21165edf41813482bd1d0a8183969854d1011f24
COMPONENT_EXTRA_INCLUDES +=     $(IDF_PATH)/components/bt/bluedroid/common/include/           \
                                $(IDF_PATH)/components/bt/bluedroid/stack/include             \
                                $(IDF_PATH)/components/bt/bluedroid/stack/gap/include         \
								$(IDF_PATH)/components/bt/bluedroid/osi/include

COMPONENT_DEPENDS := bt
