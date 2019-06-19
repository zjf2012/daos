/**
 * (C) Copyright 2019 Intel Corporation.
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

#include <mpi.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <daos/common.h>
#include <daos/tests_lib.h>
#include "daos_types.h"
#include "daos_api.h"
#include "daos_fs.h"
#include "dts_common.h"

struct dts_context	 ts_ctx;

int
main(int argc, char *argv[])
{
	d_rank_t	svc_rank  = 0;	/* pool service rank */
	daos_size_t	scm_size = (1ULL << 30); /* default pool SCM size */
	daos_size_t	nvme_size = (8ULL << 30); /* default pool NVMe size */
	int		rc = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ts_ctx.tsc_mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ts_ctx.tsc_mpi_size);

	if (ts_ctx.tsc_mpi_size != 1) {
		fprintf(stderr, "run test with 1 rank.\n");
		return -1;
	}

	uuid_generate(ts_ctx.tsc_pool_uuid);
	uuid_generate(ts_ctx.tsc_cont_uuid);
	ts_ctx.tsc_cred_nr	= -1;
	ts_ctx.tsc_svc.rl_nr	= 1;
	ts_ctx.tsc_svc.rl_ranks	= &svc_rank;
	ts_ctx.tsc_cred_vsize	= 32;
	ts_ctx.tsc_scm_size	= scm_size;
	ts_ctx.tsc_nvme_size	= nvme_size;
	ts_ctx.tsc_pmem_file	= NULL;

	rc = dts_ctx_init(&ts_ctx);
	if (rc)
		return -1;

	if (ts_ctx.tsc_mpi_rank == 0) {
		fprintf(stdout, "POOL UUID = "DF_UUIDF"\n",
			DP_UUID(ts_ctx.tsc_pool_uuid));
		fprintf(stdout, "CONT UUID = "DF_UUIDF"\n",
			DP_UUID(ts_ctx.tsc_cont_uuid));
		fprintf(stdout, "Started...\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);

	int		mask, perm, i;
	dfs_t		*dfs;
	char		name[] = "testfile";
	d_sg_list_t	sgl;
	d_iov_t		iov;
	char		*buf, *rbuf;
	dfs_obj_t	*obj = NULL;
	daos_size_t	size;
	daos_size_t	actual = 0;

        mask = umask(022);
        umask(mask);
        perm = mask ^ 0666;
	perm = S_IFREG | perm;

	sgl.sg_nr	= 1;
	sgl.sg_nr_out	= 0;
	sgl.sg_iovs	= &iov;

	for (i = 0; i < 11; i++) {
		rc = dfs_mount(ts_ctx.tsc_poh, ts_ctx.tsc_coh, O_RDWR, &dfs);
		if (rc) {
			fprintf(stderr, "Failed dfs_mount() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		rc = dfs_open(dfs, NULL, name, perm, O_CREAT | O_EXCL | O_RDWR,
			      DAOS_OC_LARGE_RW, 0, NULL, &obj);
		if (rc) {
			fprintf(stderr, "Failed dfs_open() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		size = 80000;
		D_ALLOC(buf, size);
		dts_buf_render(buf, size);
		D_ALLOC(rbuf, size);

		d_iov_set(&iov, buf, size);
		rc = dfs_write(dfs, obj, sgl, 0);
		if (rc) {
			fprintf(stderr, "Failed dfs_write() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		d_iov_set(&iov, rbuf, size);
		rc = dfs_read(dfs, obj, sgl, 0, &actual);
		if (rc) {
			fprintf(stderr, "Failed dfs_write() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		D_FREE(rbuf);
		D_FREE(buf);

		rc = dfs_release(obj);
		if (rc) {
			fprintf(stderr, "Failed dfs_close() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		rc = dfs_umount(dfs);
		if (rc) {
			fprintf(stderr, "Failed dfs_umount() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		printf("Rank 0 removing file.\n"); 

		rc = dfs_mount(ts_ctx.tsc_poh, ts_ctx.tsc_coh, O_RDWR, &dfs);
		if (rc) {
			fprintf(stderr, "Failed dfs_mount() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		rc = dfs_remove(dfs, NULL, name, true);
		if (rc) {
			fprintf(stderr, "Failed dfs_remove() %d (%d)\n", __LINE__, rc);
			goto out;
		}

		rc = dfs_umount(dfs);
		if (rc) {
			fprintf(stderr, "Failed dfs_umount() %d (%d)\n", __LINE__, rc);
			goto out;
		}
	}

	size = 1048576;
	/** Allocate and set buffer */
	D_ALLOC(buf, size);
	dts_buf_render(buf, size);
	D_ALLOC(rbuf, size);

	rc = dfs_mount(ts_ctx.tsc_poh, ts_ctx.tsc_coh, O_RDWR, &dfs);
	if (rc) {
		fprintf(stderr, "Failed dfs_mount() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	memset(rbuf, 0, size);

	rc = dfs_open(dfs, NULL, name, perm, O_CREAT | O_EXCL | O_RDWR,
		      DAOS_OC_LARGE_RW, 0, NULL, &obj);
	if (rc) {
		fprintf(stderr, "Failed dfs_open() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	for (i = 0 ; i < 4; i++) {
		daos_off_t offset;

		offset = ts_ctx.tsc_mpi_rank * size * ts_ctx.tsc_mpi_size +
			i * size;

		d_iov_set(&iov, buf, size);
		rc = dfs_write(dfs, obj, sgl, offset);
		if (rc) {
			fprintf(stderr, "Failed dfs_write() %d (%d)\n", __LINE__, rc);
			goto out;
		}
		printf("%d: wrote %zu at %zi\n", ts_ctx.tsc_mpi_rank, size, offset);
	}

	rc = dfs_release(obj);
	if (rc) {
		fprintf(stderr, "Failed dfs_close() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	rc = dfs_umount(dfs);
	if (rc) {
		fprintf(stderr, "Failed dfs_umount() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	rc = dfs_mount(ts_ctx.tsc_poh, ts_ctx.tsc_coh, O_RDONLY, &dfs);
	if (rc) {
		fprintf(stderr, "Failed dfs_mount() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	rc = dfs_open(dfs, NULL, name, perm, O_RDONLY, DAOS_OC_LARGE_RW, 0, NULL, &obj);
	if (rc) {
		fprintf(stderr, "Failed dfs_open() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	rc = dfs_release(obj);
	if (rc) {
		fprintf(stderr, "Failed dfs_close() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	rc = dfs_umount(dfs);
	if (rc) {
		fprintf(stderr, "Failed dfs_umount() %d (%d)\n", __LINE__, rc);
		goto out;
	}

	D_FREE(rbuf);
	D_FREE(buf);

out:
	dts_ctx_fini(&ts_ctx);
	if (ts_ctx.tsc_mpi_rank == 0)
		fprintf(stdout, "Finished...\n");
	MPI_Finalize();

	return 0;
}
