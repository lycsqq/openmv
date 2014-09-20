/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Clock Python module.
 *
 */
#include <mp.h>
#include "systick.h"
#include "py_clock.h"

/* Clock Type */
typedef struct _py_clock_obj_t {
    mp_obj_base_t base;
    uint32_t t_start;
    uint32_t t_ticks;
    uint32_t t_frame;
} py_clock_obj_t;

mp_obj_t py_clock_tick(py_clock_obj_t *clock)
{
    clock->t_start = systick_current_millis();
    return mp_const_none;
}

mp_obj_t py_clock_fps(py_clock_obj_t *clock)
{
    clock->t_frame++;
    clock->t_ticks += (systick_current_millis()-clock->t_start);
    return mp_obj_new_float(1000.0f/(clock->t_ticks/(float)clock->t_frame));
}

mp_obj_t py_clock_avg(py_clock_obj_t *clock)
{
    clock->t_frame++;
    clock->t_ticks += (systick_current_millis()-clock->t_start);
    return mp_obj_new_float(clock->t_ticks/(float)clock->t_frame);
}

mp_obj_t py_clock_reset(py_clock_obj_t *clock)
{
    clock->t_start = 0;
    clock->t_ticks = 0;
    clock->t_frame = 0;
    return mp_const_none;
}

static void py_clock_print(void (*print)(void *env, const char *fmt, ...),
                           void *env, mp_obj_t self_in, mp_print_kind_t kind)
{
    py_clock_obj_t *self = self_in;

    /* print some info */
    print(env, "t_start:%d t_ticks:%d t_frame:%d\n",
            self->t_start, self->t_ticks, self->t_frame);
}

static MP_DEFINE_CONST_FUN_OBJ_1(py_clock_tick_obj,  py_clock_tick);
static MP_DEFINE_CONST_FUN_OBJ_1(py_clock_fps_obj,   py_clock_fps);
static MP_DEFINE_CONST_FUN_OBJ_1(py_clock_avg_obj,   py_clock_avg);
static MP_DEFINE_CONST_FUN_OBJ_1(py_clock_reset_obj, py_clock_reset);

static const mp_map_elem_t locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_tick),   (mp_obj_t)&py_clock_tick_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_fps),    (mp_obj_t)&py_clock_fps_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_avg),    (mp_obj_t)&py_clock_avg_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_reset),  (mp_obj_t)&py_clock_reset_obj},
    { NULL, NULL },
};

STATIC MP_DEFINE_CONST_DICT(locals_dict, locals_dict_table);

static const mp_obj_type_t py_clock_type = {
    { &mp_type_type },
    .name  = MP_QSTR_Clock,
    .print = py_clock_print,
    .locals_dict = (mp_obj_t)&locals_dict,
};

mp_obj_t py_clock()
{
    py_clock_obj_t *clock =NULL;
    clock = m_new_obj(py_clock_obj_t);
    clock->base.type = &py_clock_type;
    clock->t_start = 0;
    clock->t_ticks = 0;
    clock->t_frame = 0;

    return clock;
}
