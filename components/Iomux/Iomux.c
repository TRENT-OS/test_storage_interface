/*
 * Copyright (C) 2020, HENSOLDT Cyber GmbH
 */

#include "OS_Error.h"
#include "OS_Dataport.h"

#include "LibDebug/Debug.h"

#include <stddef.h>
#include <string.h>

#include <camkes.h>
#include <camkes/io.h>

#include <platsupport/io.h>
#include <platsupport/mux.h>
#include <platsupport/plat/mux.h>

//------------------------------------------------------------------------------
ps_io_ops_t      io_ops_ref;

//------------------------------------------------------------------------------
void
post_init(void)
{
    Debug_LOG_DEBUG("Initialize IOMUX ...");

    int rslt = camkes_io_ops(&io_ops_ref);
    if (0 != rslt)
    {
        Debug_LOG_ERROR("camkes_io_ops() failed: rslt = %i", rslt);
        return;
    }

    printf("Initialize IOMUXC\n");
    rslt = mux_sys_init(&io_ops_ref, NULL, &io_ops_ref.mux_sys);

    if (rslt) {
        ZF_LOGE("Failed to initialise the MUX subsystem");
    }

    printf("io_ops->mux_sys: %p\n", &io_ops_ref.mux_sys);

    Debug_LOG_DEBUG("IOMUX successfully initialized.");

    /* Configure MUX */
    rslt = mux_feature_enable(&io_ops_ref.mux_sys, 5, MUX_DIR_NOT_A_GPIO);
    if (rslt) {
        assert(!"Failed to configure SD IO mux");
    }

    Debug_LOG_DEBUG("IOMUX features successfully enabled.");
}

int run() {
    Debug_LOG_DEBUG("Run IOMUX ...");
    return 0;
}