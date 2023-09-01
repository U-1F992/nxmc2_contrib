#include "nxmc2_contrib.h"

void nxmc2_command_handlers_initialize(NXMC2CommandHandlers *handlers)
{
    handlers->y = NULL;
    handlers->b = NULL;
    handlers->a = NULL;
    handlers->x = NULL;
    handlers->l = NULL;
    handlers->r = NULL;
    handlers->zl = NULL;
    handlers->zr = NULL;
    handlers->minus = NULL;
    handlers->plus = NULL;
    handlers->l_click = NULL;
    handlers->r_click = NULL;
    handlers->home = NULL;
    handlers->capture = NULL;
    handlers->hat = NULL;
    handlers->l_stick = NULL;
    handlers->r_stick = NULL;
    handlers->ext = NULL;
}