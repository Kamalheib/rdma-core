// SPDX-License-Identifier: GPL-2.0
/*
 * Software RDMA simulator driver
 * Copyright (C) 2020 Kamal Heib <kamalheib1@gmail.com>
 */

#ifndef _RDMASIM_H
#define _RDMASIM_H

#include <infiniband/driver.h>

#define RDMA_DRIVER_RDMASIM 19

struct rdmasim_device {
	struct verbs_device ibv_dev;
};

struct rdmasim_context {
	struct verbs_context ibv_ctx;
};

#endif
