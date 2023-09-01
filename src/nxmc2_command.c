#include "nxmc2_contrib.h"

const uint8_t NXMC2_COMMAND_HEADER = 0xABU;
const uint8_t NXMC2_COMMAND_STICK_NEUTRAL = 128U;

NXMC2Result nxmc2_command_execute(NXMC2Command *command, NXMC2CommandHandlers *handlers)
{
    if (command == NULL || handlers == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (handlers->y != NULL)
    {
        handlers->y(command->y);
    }
    if (handlers->b != NULL)
    {
        handlers->b(command->b);
    }
    if (handlers->a != NULL)
    {
        handlers->a(command->a);
    }
    if (handlers->x != NULL)
    {
        handlers->x(command->x);
    }
    if (handlers->l != NULL)
    {
        handlers->l(command->l);
    }
    if (handlers->r != NULL)
    {
        handlers->r(command->r);
    }
    if (handlers->zl != NULL)
    {
        handlers->zl(command->zl);
    }
    if (handlers->zr != NULL)
    {
        handlers->zr(command->zr);
    }
    if (handlers->minus != NULL)
    {
        handlers->minus(command->minus);
    }
    if (handlers->plus != NULL)
    {
        handlers->plus(command->plus);
    }
    if (handlers->l_click != NULL)
    {
        handlers->l_click(command->l_click);
    }
    if (handlers->r_click != NULL)
    {
        handlers->r_click(command->r_click);
    }
    if (handlers->home != NULL)
    {
        handlers->home(command->home);
    }
    if (handlers->capture != NULL)
    {
        handlers->capture(command->capture);
    }
    if (handlers->hat != NULL)
    {
        handlers->hat(command->hat);
    }
    if (handlers->l_stick != NULL)
    {
        handlers->l_stick(command->lx, command->ly);
    }
    if (handlers->r_stick != NULL)
    {
        handlers->r_stick(command->rx, command->ry);
    }
    if (handlers->ext != NULL)
    {
        handlers->ext(command->ext0, command->ext1, command->ext2);
    }
    return NXMC2_RESULT_OK;
}