#include "nxmc2_contrib.h"

Nxmc2Result nxmc2_command_builder_clear(Nxmc2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    builder->pos_ = 0;
    return NXMC2_RESULT_OK;
}

static bool is_complete_command_(size_t pos)
{
    return pos == 11; // (sizeof(builder->buf_) / sizeof(uint8_t))
}

static bool is_valid_header_(size_t pos, uint8_t packet)
{
    if (pos != 0)
    {
        return true;
    }
    return packet == NXMC2_COMMAND_HEADER;
}

static bool is_valid_hat_(size_t pos, uint8_t packet)
{
    if (pos != 3)
    {
        return true;
    }
    return packet <= NXMC2_COMMAND_HAT_STATE_NEUTRAL;
}

Nxmc2Result nxmc2_command_builder_append(Nxmc2CommandBuilder *builder, uint8_t packet)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (!is_valid_header_(builder->pos_, packet))
    {
        return NXMC2_RESULT_INVALID_HEADER_ERROR;
    }
    else if (!is_valid_hat_(builder->pos_, packet))
    {
        return NXMC2_RESULT_INVALID_HAT_ERROR;
    }
    else if (is_complete_command_(builder->pos_))
    {
        return NXMC2_RESULT_COMMAND_READY_ERROR;
    }
    builder->buf_[builder->pos_] = packet;
    builder->pos_++;
    return NXMC2_RESULT_OK;
}

static void transfer_(uint8_t *raw, Nxmc2Command *command)
{
    // Equivalent to a struct with bit fields, but well-defined behavior.
    command->header = raw[0];

    command->y = (raw[1] & 0b00000001);
    command->b = (raw[1] & 0b00000010) >> 1;
    command->a = (raw[1] & 0b00000100) >> 2;
    command->x = (raw[1] & 0b00001000) >> 3;
    command->l = (raw[1] & 0b00010000) >> 4;
    command->r = (raw[1] & 0b00100000) >> 5;
    command->zl = (raw[1] & 0b01000000) >> 6;
    command->zr = (raw[1] & 0b10000000) >> 7;

    command->minus = (raw[2] & 0b00000001);
    command->plus = (raw[2] & 0b00000010) >> 1;
    command->l_click = (raw[2] & 0b00000100) >> 2;
    command->r_click = (raw[2] & 0b00001000) >> 3;
    command->home = (raw[2] & 0b00010000) >> 4;
    command->capture = (raw[2] & 0b00100000) >> 5;
    command->_ = (raw[2] & 0b11000000) >> 6;

    command->hat = raw[3];
    command->lx = raw[4];
    command->ly = raw[5];
    command->rx = raw[6];
    command->ry = raw[7];

    command->ext0 = raw[8];
    command->ext1 = raw[9];
    command->ext2 = raw[10];
}

Nxmc2Result nxmc2_command_builder_build(Nxmc2CommandBuilder *builder, Nxmc2Command *command)
{
    if (builder == NULL || command == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (!is_complete_command_(builder->pos_))
    {
        return NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR;
    }
    transfer_(builder->buf_, command);
    return NXMC2_RESULT_OK;
}

Nxmc2Result nxmc2_command_builder_new(Nxmc2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    Nxmc2Result ret = nxmc2_command_builder_clear(builder);
    if (ret != NXMC2_RESULT_OK)
    {
        return ret;
    }
    return NXMC2_RESULT_OK;
}