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

#include "tdbInt.h"

typedef struct SPage1 {
  char     magic[64];
  SPgno   mdbRootPgno;  // master DB root page number
  SPgno   freePgno;     // free list page number
  uint32_t nFree;        // number of free pages
} SPage1;

typedef struct SFreePage {
  /* TODO */
} SFreePage;

TDB_STATIC_ASSERT(sizeof(SPage1) <= TDB_MIN_PGSIZE, "TDB Page1 definition too large");

static int pgFileRead(SPgFile *pPgFile, SPgno pgno, uint8_t *pData);

int pgFileOpen(SPgFile **ppPgFile, const char *fname, TENV *pEnv) {
  SPgFile * pPgFile;
  SPgCache *pPgCache;
  size_t    fnameLen;
  SPgno    fsize;

  *ppPgFile = NULL;

  // create the handle
  fnameLen = strlen(fname);
  pPgFile = (SPgFile *)calloc(1, sizeof(*pPgFile) + fnameLen + 1);
  if (pPgFile == NULL) {
    return -1;
  }

  ASSERT(pEnv != NULL);

  // init the handle
  pPgFile->fname = (char *)(&(pPgFile[1]));
  memcpy(pPgFile->fname, fname, fnameLen);
  pPgFile->fname[fnameLen] = '\0';
  pPgFile->fd = -1;

  pPgFile->fd = open(fname, O_CREAT | O_RDWR, 0755);
  if (pPgFile->fd < 0) {
    // TODO: handle error
    return -1;
  }

  tdbGnrtFileID(fname, pPgFile->fileid, false);
  tdbGetFileSize(fname, tdbEnvGetPageSize(pEnv), &fsize);

  pPgFile->fsize = fsize;
  pPgFile->lsize = fsize;

  if (pPgFile->fsize == 0) {
    // A created file
    SPgno pgno;
    pgid_t pgid;

    pgFileAllocatePage(pPgFile, &pgno);

    ASSERT(pgno == 1);

    memcpy(pgid.fileid, pPgFile->fileid, TDB_FILE_ID_LEN);
    pgid.pgno = pgno;

    pgCacheFetch(pPgCache, pgid);
    // Need to allocate the first page as a description page
  } else {
    // An existing file
  }

  /* TODO: other open operations */

  // add the page file to the environment
  tdbEnvRgstPageFile(pEnv, pPgFile);
  pPgFile->pEnv = pEnv;

  *ppPgFile = pPgFile;
  return 0;
}

int pgFileClose(SPgFile *pPgFile) {
  if (pPgFile) {
    if (pPgFile->fd >= 0) {
      close(pPgFile->fd);
    }

    tfree(pPgFile->fname);
    free(pPgFile);
  }

  return 0;
}

SPage *pgFileFetch(SPgFile *pPgFile, SPgno pgno) {
  SPgCache *pPgCache;
  SPage *   pPage;
  pgid_t    pgid;

  // 1. Fetch from the page cache
  // pgCacheFetch(pPgCache, pgid);

  // 2. If only get a page frame, no content, maybe
  // need to load from the file
  if (1 /*page not initialized*/) {
    if (pgno < pPgFile->fsize) {
      // load the page content from the disk
      // ?? How about the freed pages ??
    } else {
      // zero the page, make the page as a empty
      // page with zero records.
    }
  }

#if 0
  pPgCache = pPgFile->pPgCache;
  pPage = NULL;
  memcpy(pgid.fileid, pPgFile->fileid, TDB_FILE_ID_LEN);
  pgid.pgno = pgno;

  if (pgno > pPgFile->pgFileSize) {
    // TODO
  } else {
    pPage = pgCacheFetch(pPgCache, pgid);
    if (1 /*Page is cached, no need to load from file*/) {
      return pPage;
    } else {
      // TODO: handle error
      if (pgFileRead(pPgFile, pgno, (void *)pPage) < 0) {
        // todoerr
      }
      return pPage;
    }
  }
#endif

  return pPage;
}

int pgFileRelease(SPage *pPage) {
  pgCacheRelease(pPage);
  return 0;
}

int pgFileWrite(SPage *pPage) {
  // TODO
  return 0;
}

int pgFileAllocatePage(SPgFile *pPgFile, SPgno *pPgno) {
  SPgno    pgno;
  SPage1 *  pPage1;
  SPgCache *pPgCache;
  pgid_t    pgid;
  SPage *   pPage;

  if (pPgFile->lsize == 0) {
    pgno = ++(pPgFile->lsize);
  } else {
    if (0) {
      // TODO: allocate from the free list
      pPage = pgCacheFetch(pPgCache, pgid);

      if (pPage1->nFree > 0) {
        // TODO
      } else {
        pgno = ++(pPgFile->lsize);
      }
    } else {
      pgno = ++(pPgFile->lsize);
    }
  }

  *pPgno = pgno;
  return 0;
}

static int pgFileRead(SPgFile *pPgFile, SPgno pgno, uint8_t *pData) {
  pgsz_t   pgSize;
  ssize_t  rsize;
  uint8_t *pTData;
  size_t   szToRead;

#if 0

  // pgSize = ; (TODO)
  pTData = pData;
  szToRead = pgSize;
  for (; szToRead > 0;) {
    rsize = pread(pPgFile->fd, pTData, szToRead, pgno * pgSize);
    if (rsize < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    } else if (rsize == 0) {
      return -1;
    }

    szToRead -= rsize;
    pTData += rsize;
  }
#endif

  return 0;
}