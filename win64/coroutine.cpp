#include "coroutine.h"

#define MINICORO_IMPL
#include "minicoro/minicoro.h"
#include "SDL.h"


static void fail(const char* message, mco_result res) {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: %s\n", message, mco_result_description(res));
  exit(-1);
}

void create(co_func func, Coroutine& coro) {
  coro.func = func;

  mco_desc desc = mco_desc_init(coro.func, 0);
  coro.res = mco_create(&coro.handle, &desc);

  if (coro.res != MCO_SUCCESS)
    fail("Failed to create coroutine", coro.res);
}

void destroy(Coroutine& coro) {
  coro.res = mco_destroy(coro.handle);
  if (coro.res != MCO_SUCCESS)
    fail("Failed to destroy coroutine", coro.res);
}

void execute(Coroutine& coro) {
  coro.status = mco_status(coro.handle);

  if (coro.status == MCO_SUSPENDED) {

    /* Resume the coroutine. */
    coro.res = mco_resume(coro.handle);
    if (coro.res != MCO_SUCCESS)
      fail("Failed to resume coroutine", coro.res);

//    mco_push(coro.handle, &delta_time_s, sizeof(delta_time_s));
//    /* Retrieve storage set in last coroutine yield. */
//    unsigned long ret = 0;
//    coro.res = mco_pop(coro.handle, &ret, sizeof(ret));
//    if (coro.res != MCO_SUCCESS)
//      fail("Failed to retrieve coroutine storage", coro.res);
  }
}

bool finished(Coroutine& coro) {
  return coro.status == MCO_DEAD;
}
