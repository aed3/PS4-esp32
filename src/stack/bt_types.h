/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BT_TYPES_H
#define BT_TYPES_H

#include <stdbool.h>
#include <stdint.h>

// typedef uint8_t UINT8;
// typedef uint16_t UINT16;
// typedef uint32_t UINT32;
// typedef uint64_t UINT64;

// typedef int8_t INT8;
// typedef int16_t INT16;
// typedef int32_t INT32;
// typedef bool BOOLEAN;

/* Define the header of each buffer used in the Bluetooth stack.
 */
typedef struct {
  uint16_t event;
  uint16_t length;
  uint16_t offset;
  uint16_t layer_specific;
  uint8_t data[];
} BT_HDR;

#define BT_PSM_HID_CONTROL 0x0011
#define BT_PSM_HID_INTERRUPT 0x0013

typedef struct {
  uint8_t qos_flags;          /* TBD */
  uint8_t service_type;       /* see below */
  uint32_t token_rate;        /* bytes/second */
  uint32_t token_bucket_size; /* bytes */
  uint32_t peak_bandwidth;    /* bytes/second */
  uint32_t latency;           /* microseconds */
  uint32_t delay_variation;   /* microseconds */
} FLOW_SPEC;

/* bd addr length and type */
#ifndef BD_ADDR_LEN
#define BD_ADDR_LEN 6
typedef uint8_t BD_ADDR[BD_ADDR_LEN];
#endif

#endif
