/* Copyright (c) 2004-2007, 2009-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef _SURF_TMGR_H
#define _SURF_TMGR_H

#include "xbt/heap.h"
#include "xbt/dynar.h"
#include "surf/maxmin.h"
#include "surf/datatypes.h"
#include "simgrid/platf_interface.h"
#include "simgrid/forward.h"

SG_BEGIN_DECL()
#include "xbt/base.h"
#include "xbt/swag.h"
#include "xbt/heap.h"
#include "trace_mgr.hpp"
#include "xbt/RngStream.h"

typedef struct tmgr_event {
  double delta;
  double value;
} s_tmgr_event_t, *tmgr_event_t;

enum e_trace_type {
  e_trace_list, e_trace_probabilist
};

enum e_event_generator_type {
  e_generator_uniform, e_generator_exponential, e_generator_weibull
};

typedef struct probabilist_event_generator {
  enum e_event_generator_type type;
  RngStream rng_stream;
  double next_value;
  union {
    struct {
      double min;
      double max;
    } s_uniform_parameters;
    struct {
      double rate;
    } s_exponential_parameters;
    struct {
      double scale;
      double shape;
    } s_weibull_parameters;
  };
} s_probabilist_event_generator_t;

typedef struct tmgr_trace {
  enum e_trace_type type;
  union {
    struct {
      xbt_dynar_t event_list;
    } s_list;
    struct {
      probabilist_event_generator_t event_generator[2];
      int is_state_trace;
      int next_event;
    } s_probabilist;
  };
} s_tmgr_trace_t;

/* Iterator within a trace */
typedef struct tmgr_trace_iterator {
  tmgr_trace_t trace;
  unsigned int idx;
  sg_resource_t resource;
  int free_me;
} s_tmgr_trace_event_t;

XBT_PRIVATE double tmgr_event_generator_next_value(probabilist_event_generator_t generator);

/* Creation functions */
XBT_PUBLIC(tmgr_trace_t) tmgr_empty_trace_new(void);
XBT_PUBLIC(void) tmgr_trace_free(tmgr_trace_t trace);
/**
 * \brief Free a trace event structure
 *
 * This function frees a trace_event if it can be freed, ie, if it has the free_me flag set to 1.
 * This flag indicates whether the structure is still used somewhere or not.
 * When the structure is freed, the argument is set to nullptr
*/
XBT_PUBLIC(void) tmgr_trace_event_unref(tmgr_trace_iterator_t *trace_event);

XBT_PUBLIC(void) tmgr_finalize(void);

SG_END_DECL()

#ifdef __cplusplus
namespace simgrid {
  namespace trace_mgr {

/* Future Event Set (collection of iterators over the traces)
 * That's useful to quickly know which is the next occurring event in a set of traces. */
XBT_PUBLIC_CLASS future_evt_set {
public:
  future_evt_set();
  virtual ~future_evt_set();
  double next_date();
  tmgr_trace_iterator_t pop_leq(double date, double *value, simgrid::surf::Resource** resource);
  tmgr_trace_iterator_t add_trace(tmgr_trace_t trace, double start_time, simgrid::surf::Resource *resource);

private:
  // TODO: use a boost type for the heap (or a ladder queue)
  xbt_heap_t p_heap = xbt_heap_new(8, xbt_free_f); /* Content: only trace_events (yep, 8 is an arbitrary value) */
};

}} // namespace simgrid::trace_mgr
#endif /* C++ only */

#endif                          /* _SURF_TMGR_H */