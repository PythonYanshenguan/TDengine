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

#ifndef _TD_DNODE_INT_H_
#define _TD_DNODE_INT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "os.h"
#include "taosmsg.h"
#include "tglobal.h"
#include "tlog.h"
#include "trpc.h"

extern int32_t dDebugFlag;

#define dFatal(...) { if (dDebugFlag & DEBUG_FATAL) { taosPrintLog("DND FATAL ", 255, __VA_ARGS__); }}
#define dError(...) { if (dDebugFlag & DEBUG_ERROR) { taosPrintLog("DND ERROR ", 255, __VA_ARGS__); }}
#define dWarn(...)  { if (dDebugFlag & DEBUG_WARN)  { taosPrintLog("DND WARN ", 255, __VA_ARGS__); }}
#define dInfo(...)  { if (dDebugFlag & DEBUG_INFO)  { taosPrintLog("DND ", 255, __VA_ARGS__); }}
#define dDebug(...) { if (dDebugFlag & DEBUG_DEBUG) { taosPrintLog("DND ", dDebugFlag, __VA_ARGS__); }}
#define dTrace(...) { if (dDebugFlag & DEBUG_TRACE) { taosPrintLog("DND ", dDebugFlag, __VA_ARGS__); }}

typedef enum { DN_RUN_STAT_INIT, DN_RUN_STAT_RUNNING, DN_RUN_STAT_STOPPED } EDnStat;
typedef void (*MsgFp)(SRpcMsg *pMsg, SEpSet *pEpSet);

int32_t dnodeInit();
void    dnodeCleanup();

EDnStat dnodeGetRunStat();
void    dnodeSetRunStat(EDnStat stat);

void dnodeReportStartup(char *name, char *desc);
void dnodeReportStartupFinished(char *name, char *desc);
void dnodeGetStartup(SStartupMsg *);

#ifdef __cplusplus
}
#endif

#endif /*_TD_DNODE_INT_H_*/