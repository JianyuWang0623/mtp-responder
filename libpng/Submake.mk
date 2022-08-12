#
# Copyright (C) 2022 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include $(APPDIR)/Make.defs

ifneq ($(CONFIG_ARM_NEON),)
CSRCS += $(wildcard ./arm/*.c)
ASRCS += $(wildcard ./arm/*.S)
else
CFLAGS += -DPNG_ARM_NEON_IMPLEMENTATION=0
endif

CFLAGS += -Dcrc32=zlib_crc32
CFLAGS += ${shell $(INCDIR) $(INCDIROPT) "$(CC)" $(APPDIR)/external/libpng}
CSRCS += $(filter-out pngtest.c, $(wildcard *.c))

include $(APPDIR)/Application.mk
