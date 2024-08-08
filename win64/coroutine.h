#pragma once

#include "minicoro/minicoro.h"


// TODO: https://github.com/bigrando420/thomas/blob/main/sauce/th_coroutine.h
// TODO: https://github.com/RandyGaul/cute_framework/blob/5d6ea567b60bf21b8cf4096b169689fba4d448ae/include/cute_coroutine.h
// TODO: https://github.com/RandyGaul/player2d/blob/master/cute_coroutine.h


typedef mco_coro* co_handle;
typedef mco_result co_result;
typedef mco_state co_status;
typedef void (*co_func) (mco_coro*);

struct Coroutine {
    co_handle handle;
    co_result res;
    co_status status;
    co_func func;
};

static void fail(const char* message, mco_result res);
void create(co_func func, Coroutine& coro);
void execute(Coroutine& coro);
void destroy(Coroutine& coro);
bool finished(Coroutine& coro);
