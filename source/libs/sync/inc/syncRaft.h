/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_LIBS_SYNC_RAFT_H
#define _TD_LIBS_SYNC_RAFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sync.h"
#include "syncMessage.h"
#include "taosdef.h"

typedef struct SRaftId {
  SyncNodeId  nodeId;
  SyncGroupId vgId;
} SRaftId;

typedef struct SRaft {
  SRaftId id;
} SRaft;

int32_t raftPropose(SRaft* pRaft, const SSyncBuffer* pBuf, bool isWeak);

static int raftSendMsg(SRaftId destRaftId, const void* pMsg, const SRaft* pRaft);

#ifdef __cplusplus
}
#endif

#endif /*_TD_LIBS_SYNC_RAFT_H*/