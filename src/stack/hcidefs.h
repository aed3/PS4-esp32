#ifndef HCIDEFS_H
#define HCIDEFS_H

#include "stack/bt_types.h"

/* Define the extended flow specification fields used by AMP */
typedef struct {
  uint8_t id;
  uint8_t stype;
  uint16_t max_sdu_size;
  uint32_t sdu_inter_time;
  uint32_t access_latency;
  uint32_t flush_timeout;
} tHCI_EXT_FLOW_SPEC;

#endif
