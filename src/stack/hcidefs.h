/******************************************************************************
 *
 *  Copyright (C) 1999-2014 Broadcom Corporation
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

#ifndef HCIDEFS_H
#define HCIDEFS_H


#include "stack/bt_types.h"


/* Define the extended flow specification fields used by AMP */
typedef struct {
    UINT8       id;
    UINT8       stype;
    UINT16      max_sdu_size;
    UINT32      sdu_inter_time;
    UINT32      access_latency;
    UINT32      flush_timeout;
} tHCI_EXT_FLOW_SPEC;

#endif
