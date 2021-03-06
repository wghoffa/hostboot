# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/usr/pore/poreve/makefile $
#
# OpenPOWER HostBoot Project
#
# COPYRIGHT International Business Machines Corp. 2011,2014
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
# Module poreve built from 3 different source code areas:
#
# model         The source code of the PoreInterface and PoreModel classes,
#               including the Transaction model. This pair of abstract classes
#               is implemented on the model side by a hardware model
#               (e.g., pore_model), and on the interface side by a virtual
#               environment (e.g., poreve).
#
# pore_model    Model the hardware of the PORE. This containts the "pore-engine-core/interpreter" of the PORe simulation
#               The code in this directory is owned by Boeblingen team.
#
# porevesrc     The Source code of the PoreVe class, including the Bus and
#               Slave models, and the HookManager.
#
ROOTPATH = ../../../..
MODULE = poreve
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/ecmddatabuffer
EXTRAINCDIR += ${ROOTPATH}/src/usr/pore/poreve/model
EXTRAINCDIR += ${ROOTPATH}/src/usr/pore/poreve/pore_model/include
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/fapi
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/plat
EXTRAINCDIR += ${ROOTPATH}/src/include/usr/hwpf/hwp
EXTRAINCDIR += ${ROOTPATH}/src/usr/pore/poreve/porevesrc

CFLAGS += -DDEBUG_FASTI2C=1 -D_BIG_ENDIAN=1

# Override to use C++ compiler for %.c/h files
CC_OVERRIDE = 1

OBJS += poreveutil.o
OBJS += transaction.o
OBJS += poreaddress.o
OBJS += poremodel.o
OBJS += poreregister.o
OBJS += poreinterface.o
OBJS += porestate.o
OBJS += pore_model.o
OBJS += pore_bus.o
OBJS += pore_fi2c.o
OBJS += pore_inline_decode.o
OBJS += vsbe.o
OBJS += pibmem.o
OBJS += pore.o
OBJS += bus.o
OBJS += hookmanager.o
OBJS += poreve.o
OBJS += pib2cfam.o
OBJS += fasti2c.o
OBJS += sbevital.o
OBJS += create.o

HOOK_SOURCE_FILES = $(notdir $(wildcard ./hook/*sbe*.hooks.cc))
OBJS += $(patsubst %.cc,%.o,$(HOOK_SOURCE_FILES))

vpath %.C ./model/
vpath %.C ./pore_model/wrapper/
# Note: small c files
vpath %.c ./pore_model/ibuf/
vpath %.C ./porevesrc/
vpath %.cc ./hook/

include ${ROOTPATH}/config.mk
