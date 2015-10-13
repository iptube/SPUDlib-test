#include "tube_manager.h"
#include "ls_error.h"
#include "ls_log.h"

int main(int argc, char **argv) {
  tube_manager* mgr;
  ls_err        err;

  UNUSED_PARAM(argc);
  UNUSED_PARAM(argv);

  if ( !tube_manager_create(0, &mgr, &err) )
  {
    LS_LOG_ERR(err, "tube_manager_create");
    return 1;
  }
  tube_manager_destroy(mgr);
  return 0;
}
