/* Copyright (c) 2015. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <xbt/asserts.h>

#include "src/surf/callbacks.h"
#include "src/surf/surf_interface.hpp"
#include "src/surf/host_interface.hpp"

void surf_host_created_callback(void (*callback)(sg_host_t))
{
  hostCreatedCallbacks.connect([callback](Host* host) {
    const char* id = host->getName();
    sg_host_t h = sg_host_by_name(id);
    xbt_assert(h != NULL, "Host not found for name %s", id);
    callback(h);
  });
}