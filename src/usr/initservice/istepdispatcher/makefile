# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/usr/initservice/istepdispatcher/makefile $
#
# OpenPOWER HostBoot Project
#
# Contributors Listed Below - COPYRIGHT 2011,2015
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
ROOTPATH = ../../../..
MODULE = istepdisp

EXTRAINCDIR += ${ROOTPATH}/src/usr/hwpf/hwp/establish_system_smp
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/ecmddatabuffer
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/fapi
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/plat
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/hwp

VPATH = ${ROOTPATH}/src/usr/initservice/bootconfig/

OBJS += istepdispatcher.o
OBJS += initsvcudistep.o
OBJS += sptask.o
OBJS += splesscommon.o
OBJS += bootconfig.o
OBJS += bootconfigif.o
OBJS += $(if $(CONFIG_BMC_AST2400),bootconfig_ast2400.o)

## SUBDIRS += test.d

include ${ROOTPATH}/config.mk
