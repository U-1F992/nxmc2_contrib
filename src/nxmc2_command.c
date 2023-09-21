#include "nxmc2_contrib.h"

const uint8_t NXMC2_COMMAND_HEADER = 0xABU;
const uint8_t NXMC2_COMMAND_STICK_NEUTRAL = 128U;

Nxmc2Result nxmc2_command_execute(Nxmc2Command *command, Nxmc2CommandHandler *handler)
{
    if (command == NULL || handler == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (handler->y != NULL)
    {
        handler->y(command->y);
    }
    if (handler->b != NULL)
    {
        handler->b(command->b);
    }
    if (handler->a != NULL)
    {
        handler->a(command->a);
    }
    if (handler->x != NULL)
    {
        handler->x(command->x);
    }
    if (handler->l != NULL)
    {
        handler->l(command->l);
    }
    if (handler->r != NULL)
    {
        handler->r(command->r);
    }
    if (handler->zl != NULL)
    {
        handler->zl(command->zl);
    }
    if (handler->zr != NULL)
    {
        handler->zr(command->zr);
    }
    if (handler->minus != NULL)
    {
        handler->minus(command->minus);
    }
    if (handler->plus != NULL)
    {
        handler->plus(command->plus);
    }
    if (handler->l_click != NULL)
    {
        handler->l_click(command->l_click);
    }
    if (handler->r_click != NULL)
    {
        handler->r_click(command->r_click);
    }
    if (handler->home != NULL)
    {
        handler->home(command->home);
    }
    if (handler->capture != NULL)
    {
        handler->capture(command->capture);
    }
    if (handler->hat != NULL)
    {
        handler->hat(command->hat);
    }
    if (handler->l_stick != NULL)
    {
        handler->l_stick(command->lx, command->ly);
    }
    if (handler->r_stick != NULL)
    {
        handler->r_stick(command->rx, command->ry);
    }
    if (handler->ext != NULL)
    {
        handler->ext(command->ext0, command->ext1, command->ext2);
    }
    return NXMC2_RESULT_OK;
}