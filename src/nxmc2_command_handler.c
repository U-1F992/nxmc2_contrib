#include "nxmc2_contrib.h"

NXMC2Result nxmc2_command_handler_initialize(NXMC2CommandHandler *handler)
{
    if (handler == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    handler->y = NULL;
    handler->b = NULL;
    handler->a = NULL;
    handler->x = NULL;
    handler->l = NULL;
    handler->r = NULL;
    handler->zl = NULL;
    handler->zr = NULL;
    handler->minus = NULL;
    handler->plus = NULL;
    handler->l_click = NULL;
    handler->r_click = NULL;
    handler->home = NULL;
    handler->capture = NULL;
    handler->hat = NULL;
    handler->l_stick = NULL;
    handler->r_stick = NULL;
    handler->ext = NULL;
    return NXMC2_RESULT_OK;
}