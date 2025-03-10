/*
 * Copyright (c) 2012, 2013, 2018 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/usb/ch9.h>
#include <linux/usb/functionfs.h>
#include <endian.h>
#include <linux/version.h>

#if (LINUX_VERSION_CODE >> 16) < 4
#ifndef FUNCTIONFS_DESCRIPTORS_MAGIC_V2
#define FUNCTIONFS_DESCRIPTORS_MAGIC_V2 3
enum functionfs_flags {
        FUNCTIONFS_HAS_FS_DESC = 1,
        FUNCTIONFS_HAS_HS_DESC = 2,
        FUNCTIONFS_HAS_SS_DESC = 4,
        FUNCTIONFS_HAS_MS_OS_DESC = 8,
};
#endif
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define cpu_to_le16(x)  (x)
#define cpu_to_le32(x)  (x)
#else
#define cpu_to_le16(x)  ((((x) >> 8) & 0xffu) | (((x) & 0xffu) << 8))
#define cpu_to_le32(x)  \
	((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) | \
	(((x) & 0x0000ff00u) <<  8) | (((x) & 0x000000ffu) << 24))
#endif
#define le32_to_cpu(x)  le32toh(x)
#define le16_to_cpu(x)  le16toh(x)

extern struct mtp_usb_descs{
	struct {
		__le32 magic;
		__le32 length;
		__le32 flags;
		__le32 fs_count;
		__le32 hs_count;
		// __le32 os_count;
	} header;
	struct {
		struct usb_interface_descriptor intf;
		struct usb_endpoint_descriptor_no_audio bulk_in;
		struct usb_endpoint_descriptor_no_audio bulk_out;
		struct usb_endpoint_descriptor_no_audio int_in;
	} __attribute__((packed)) fs_descs, hs_descs;
	// struct {} __attribute__((packed)) os_descs;
} __attribute__((packed)) descriptors;

#define STR_INTERFACE "Samsung MTP"

extern struct mtp_usb_strs {
	struct usb_functionfs_strings_head header;
	struct {
		__le16 code;
		const char str1[sizeof(STR_INTERFACE)];
	} __attribute__((packed)) lang0;
} __attribute__((packed)) strings;
