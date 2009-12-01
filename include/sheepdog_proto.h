/*
 * Copyright (C) 2009 Nippon Telegraph and Telephone Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __SHEEPDOG_PROTO_H__
#define __SHEEPDOG_PROTO_H__

#include <stdint.h>
#include <openssl/sha.h>
#include "util.h"

#define SD_LISTEN_PORT 7000

#define SD_MAX_NODES 1024
#define SD_MAX_VMS   4096

/* -> vmon */

#define SD_OP_NEW_VDI        0x11
#define SD_OP_DEL_VDI        0x12
#define SD_OP_LOCK_VDI       0x16
#define SD_OP_RELEASE_VDI    0x17
#define SD_OP_GET_VDI_INFO   0x18
#define SD_OP_GET_NODE_LIST  0x19
#define SD_OP_GET_VM_LIST    0x20
#define SD_OP_MAKE_FS        0x21
#define SD_OP_UPDATE_EPOCH   0x22
#define SD_OP_GET_EPOCH      0x23
#define SD_OP_SHUTDOWN       0x24

#define SD_OP_DEBUG_INC_NVER 0xA0
#define SD_OP_DEBUG_SET_NODE 0xA1

#define SD_OP_CREATE_AND_WRITE_OBJ  0x01
#define SD_OP_REMOVE_OBJ     0x02
#define SD_OP_READ_OBJ       0x03
#define SD_OP_WRITE_OBJ      0x04
#define SD_OP_SYNC_OBJ       0x05

#define SD_OP_STAT_SHEEP     0xB0

#define SD_FLAG_CMD_WRITE    0x01
#define SD_FLAG_CMD_COW      0x02

#define SD_RES_SUCCESS       0x00 /* Success */
#define SD_RES_UNKNOWN       0x01 /* Unknown error */
#define SD_RES_NO_OBJ        0x02 /* No object found */
#define SD_RES_EIO           0x03 /* I/O error */
#define SD_RES_OLD_NODE_VER  0x04 /* Remote node has an old epoch */
#define SD_RES_NEW_NODE_VER  0x05 /* Remote node has a new epoch */
#define SD_RES_VDI_EXIST     0x06 /* Vdi exists already */
#define SD_RES_INVALID_PARMS 0x07 /* Invalid parameters */
#define SD_RES_SYSTEM_ERROR  0x08 /* System error */
#define SD_RES_VDI_LOCKED    0x09 /* Vdi is locked */
#define SD_RES_NO_SUPER_OBJ  0x0A /* No super object found */
#define SD_RES_NO_VDI        0x0B /* No vdi found */
#define SD_RES_NO_BASE_VDI   0x0C /* No base vdi found */
#define SD_RES_DIFFERENT_EPOCH  0x0D /* Epochs are different between nodes */
#define SD_RES_DIR_READ      0x0E /* Cannot read directory */
#define SD_RES_DIR_WRITE     0x0F /* Cannot write directory */
#define SD_RES_VDI_READ      0x10 /* Cannot read requested vdi */
#define SD_RES_VDI_WRITE     0x11 /* Cannot write requested vdi */
#define SD_RES_BASE_VDI_READ 0x12 /* Cannot read base vdi */
#define SD_RES_BASE_VDI_WRITE   0x13 /* Cannot write base vdi */
#define SD_RES_NO_TAG        0x14 /* Requested tag is not found */
#define SD_RES_STARTUP       0x15 /* Sheepdog is on starting up */
#define SD_RES_NO_EPOCH      0x16 /* Requested epoch is not found */
#define SD_RES_VDI_NOT_LOCKED   0x17 /* Vdi is not locked */
#define SD_RES_SHUTDOWN      0x18 /* Sheepdog is shutting down */


struct sd_req {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t	opcode_specific[8];
};

struct sd_rsp {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t        result;
	uint32_t	opcode_specific[7];
};

struct sd_obj_req {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint64_t        oid;
	uint64_t        cow_oid;
	uint32_t        copies;
	uint32_t        obj_ver;
	uint64_t        offset;
};

struct sd_obj_rsp {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t        result;
	uint32_t        obj_ver;
	uint32_t        copies;
	uint32_t        pad[5];
};

struct sd_vdi_req {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint64_t        base_oid;
	uint64_t        tag;
	uint64_t	vdi_size;
	uint32_t        pad[2];
};

#define SD_VDI_RSP_FLAG_CURRENT 0x01;

struct sd_vdi_rsp {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t        result;
	uint32_t        rsvd;
	uint64_t        oid;
	uint32_t	vdi_epoch;
	uint32_t        pad[3];
};

struct sd_node_req {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t	request_ver;
	uint32_t	pad[7];
};

struct sd_node_rsp {
	uint8_t		proto_ver;
	uint8_t		opcode;
	uint16_t	flags;
	uint32_t	epoch;
	uint32_t        id;
	uint32_t        data_length;
	uint32_t        result;
	uint32_t	nr_nodes;
	uint32_t	local_idx;
	uint32_t	master_idx;
	uint64_t	store_size;
	uint64_t	store_free;
};

struct sheepdog_vm_list_entry {
	uint8_t         name[32];
	uint8_t         host_addr[16];
	uint16_t        host_port;
	uint8_t	        pad[6];
};

struct sheepdog_node_list_entry {
	uint8_t         id[20];
	uint8_t         addr[16];
	uint16_t        port;
	uint16_t	pad;
};

static inline int obj_to_sheep(struct sheepdog_node_list_entry *entries,
			       int nr_entries, uint64_t oid, int idx)
{
	SHA_CTX ctx;
	uint8_t id[20];
	int i;
	struct sheepdog_node_list_entry *e = entries, *n;

	SHA1_Init(&ctx);

	SHA1_Update(&ctx, &oid, sizeof(oid));

	SHA1_Final(id, &ctx);

	for (i = 0; i < nr_entries - 1; i++, e++) {
		n = e + 1;

		if (memcmp(id, e->id, sizeof(id)) > 0 &&
		    memcmp(id, n->id, sizeof(id)) <= 0)
			break;
	}

	return (i + 1 + idx) % nr_entries;
}

static inline void print_node_list_entry(struct sheepdog_node_list_entry *e,
					 char *str, size_t size)
{
	uint32_t *p = (uint32_t *)e->id;

	snprintf(str, size, "%08x:%08x:%08x:%08x:%08x - %d.%d.%d.%d:%d",
		p[0] ,p[1], p[2], p[3] ,p[4],
		e->addr[12], e->addr[13],
		e->addr[14], e->addr[15], e->port);
}

#endif
