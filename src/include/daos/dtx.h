/**
 * (C) Copyright 2019-2020 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */

#ifndef __DAOS_DTX_H__
#define __DAOS_DTX_H__

#include <time.h>
#include <uuid/uuid.h>

/* If the count of committable DTXs on leader exceeds this threshold,
 * it will trigger batched DTX commit globally. We will optimize the
 * threshould with considering RPC limitation, PMDK transaction, and
 * CPU schedule efficiency, and so on.
 */
#define DTX_THRESHOLD_COUNT		(1 << 9)

/* The time (in second) threshould for batched DTX commit. */
#define DTX_COMMIT_THRESHOLD_AGE	60

/**
 * DAOS two-phase commit transaction identifier,
 * generated by client, globally unique.
 */
struct dtx_id {
	/** The uuid of the transaction */
	uuid_t			dti_uuid;
	/** The HLC timestamp for the transaction */
	uint64_t		dti_hlc;
};

void daos_dti_gen(struct dtx_id *dti, bool zero);

static inline void
daos_dti_copy(struct dtx_id *des, struct dtx_id *src)
{
	if (src != NULL)
		*des = *src;
	else
		memset(des, 0, sizeof(*des));
}

static inline bool
daos_is_zero_dti(struct dtx_id *dti)
{
	return dti->dti_hlc == 0;
}

static inline bool
daos_dti_equal(struct dtx_id *dti0, struct dtx_id *dti1)
{
	return memcmp(dti0, dti1, sizeof(*dti0)) == 0;
}

static inline daos_epoch_t
daos_dti2epoch(struct dtx_id *dti)
{
	return dti->dti_hlc;
}

#define DF_DTI		DF_UUID"."DF_X64
#define DP_DTI(dti)	DP_UUID((dti)->dti_uuid), (dti)->dti_hlc

enum daos_ops_intent {
	DAOS_INTENT_DEFAULT	= 0,	/* fetch/enumerate/query */
	DAOS_INTENT_PURGE	= 1,	/* purge/aggregation */
	DAOS_INTENT_UPDATE	= 2,	/* write/insert */
	DAOS_INTENT_PUNCH	= 3,	/* punch/delete */
	DAOS_INTENT_REBUILD	= 4,	/* for rebuild related scan */
	DAOS_INTENT_CHECK	= 5,	/* check aborted or not */
	DAOS_INTENT_KILL	= 6,	/* delete object/key */
	DAOS_INTENT_COS		= 7,	/* add something into CoS cache. */
};

enum daos_dtx_alb {
	/* unavailable case */
	ALB_UNAVAILABLE		= 0,
	/* available, no (or not care) pending modification */
	ALB_AVAILABLE_CLEAN	= 1,
	/* available but with dirty modification or garbage */
	ALB_AVAILABLE_DIRTY	= 2,
};

#endif /* __DAOS_DTX_H__ */
