/*
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  *  Copyright (C) 2009-2017 Broadcom Corporation
 */

#ifndef __HCI_SNOOP_DUMP_V4L2
#define __HCI_SNOOP_DUMP_V4L2

/******************************************************************************
**  Constants & data types
******************************************************************************/
#define HCI_SNOOP_PATH_LEN 200

#define TRUE   1
#define FALSE  0

/* HCI HCI_V4L2 message type definitions */
#define HCI_V4L2_TYPE_COMMAND         1
#define HCI_V4L2_TYPE_ACL_DATA        2
#define HCI_V4L2_TYPE_SCO_DATA        3
#define HCI_V4L2_TYPE_EVENT           4
#define HCI_V4L2_TYPE_FM_CMD          8

/* Preamble length for HCI Commands:
**      2-bytes for opcode and 1 byte for length
*/
#define HCI_CMD_PREAMBLE_SIZE   3

/* Preamble length for HCI Events:
**      1-byte for opcode and 1 byte for length
*/
#define HCI_EVT_PREAMBLE_SIZE   2

/* Preamble length for SCO Data:
**      2-byte for Handle and 1 byte for length
*/
#define HCI_SCO_PREAMBLE_SIZE   3

/* Preamble length for ACL Data:
**      2-byte for Handle and 2 byte for length
*/
#define HCI_ACL_PREAMBLE_SIZE   4

#define ACL_RX_PKT_START        2
#define ACL_RX_PKT_CONTINUE     1
#define L2CAP_HEADER_SIZE       4
#define MAX_ACL_PDU_SIZE ((uint16_t)0xFFFF - HCI_ACL_PREAMBLE_SIZE \
                                                     - L2CAP_HEADER_SIZE)

#define STREAM_TO_UINT16(u16, p) {u16 = ((uint16_t)(*(p)) + (((uint16_t)(*((p) + 1))) << 8)); (p) += 2;}
#define UINT16_TO_STREAM(p, u16) {*(p)++ = (uint8_t)(u16); *(p)++ = (uint8_t)((u16) >> 8);}
#define UINT32_TO_STREAM(p, u32) {*(p)++ = (uint8_t)(u32); *(p)++ = (uint8_t)((u32) >> 8); *(p)++ = (uint8_t)((u32) >> 16); *(p)++ = (uint8_t)((u32) >> 24);}

typedef enum
{
    HCI_SNOOP_STARTING,
    HCI_SNOOP_RUNNING,
    HCI_SNOOP_STOP
} HCI_SNOOP_STATUS;

enum {
    HCISNOOP_EXIT,
};

/*******************************************************************************
**  Static variables
*******************************************************************************/
/* Table of HCI preamble sizes for the different HCI message types */
static const uint8_t hci_preamble_table[] =
{
    HCI_CMD_PREAMBLE_SIZE,
    HCI_ACL_PREAMBLE_SIZE,
    HCI_SCO_PREAMBLE_SIZE,
    HCI_EVT_PREAMBLE_SIZE
};

/******************************************************************************
**  Function declarations
******************************************************************************/
int v4l2_start_hci_snoop();
int v4l2_stop_hci_snoop();

#endif

