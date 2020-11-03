// SPDX-License-Identifier: GPL-2.0
/*
 * Software RDMA simulator driver
 * Copyright (C) 2020 Kamal Heib <kamalheib1@gmail.com>
 */

#include <stdlib.h>

#include "rdmasim.h"
#include "rdmasim-abi.h"

static const struct verbs_context_ops rdmasim_ctx_ops = {
};

static struct verbs_context *rdmasim_alloc_context(struct ibv_device *ibv_dev,
						   int fd, void *pdata)
{
	struct ib_uverbs_get_context_resp resp;
	struct ibv_get_context cmd = {};
	struct rdmasim_context *ctx;

	ctx = verbs_init_and_alloc_context(ibv_dev, fd, ctx, ibv_ctx,
					   RDMA_DRIVER_RDMASIM);
	if (!ctx)
		return NULL;

	if (ibv_cmd_get_context(&ctx->ibv_ctx, &cmd, sizeof(cmd), &resp,
				sizeof(resp)))
		goto out;

	verbs_set_ops(&ctx->ibv_ctx, &rdmasim_ctx_ops);

	return &ctx->ibv_ctx;
out:
	verbs_uninit_context(&ctx->ibv_ctx);
	free(ctx);
	return NULL;
}

static struct verbs_device *rdmasim_device_alloc(struct verbs_sysfs_dev *unused)
{
	struct rdmasim_device *dev;

	dev = calloc(1, sizeof(*dev));
	if (!dev)
		return NULL;

	return &dev->ibv_dev;
}

static void rdmasim_uninit_device(struct verbs_device *verbs_device)
{
	struct rdmasim_device *dev =
		container_of(verbs_device, struct rdmasim_device, ibv_dev);
	free(dev);
}

static const struct verbs_match_ent hca_table[] = {
	VERBS_DRIVER_ID(RDMA_DRIVER_RDMASIM),
	{},
};

static const struct verbs_device_ops rdmasim_dev_ops = {
	.name = "rdmasim",
	.match_min_abi_version = RDMASIM_ABI_VERSION,
	.match_max_abi_version = RDMASIM_ABI_VERSION,
	.match_table = hca_table,
	.alloc_device = rdmasim_device_alloc,
	.uninit_device = rdmasim_uninit_device,
	.alloc_context = rdmasim_alloc_context,
};

PROVIDER_DRIVER(rdmasim, rdmasim_dev_ops);
