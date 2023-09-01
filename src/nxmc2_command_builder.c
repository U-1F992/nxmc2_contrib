#include "nxmc2_contrib.h"

NXMC2Result nxmc2_command_builder_clear(NXMC2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    builder->ptr_ = 0;
    return NXMC2_RESULT_OK;
}

static bool is_complete_command_(NXMC2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return false;
    }

    if (builder->ptr_ != (sizeof(builder->raw_) / sizeof(uint8_t)))
    {
        return false;
    }
    return true;
}

static bool is_valid_header_(size_t ptr, uint8_t packet)
{
    if (ptr != 0)
    {
        return true;
    }
    return packet == NXMC2_COMMAND_HEADER;
}

static bool is_valid_hat_range_(size_t ptr, uint8_t packet)
{
    if (ptr != 3)
    {
        return true;
    }
    return packet <= NXMC2_COMMAND_HAT_STATE_NEUTRAL;
}

NXMC2Result nxmc2_command_builder_append(NXMC2CommandBuilder *builder, uint8_t packet)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (!is_valid_header_(builder->ptr_, packet))
    {
        NXMC2Result ret = nxmc2_command_builder_clear(builder);
        if (ret != NXMC2_RESULT_OK)
        {
            return ret;
        }
        return NXMC2_RESULT_INVALID_HEADER_ERROR;
    }
    else if (!is_valid_hat_range_(builder->ptr_, packet))
    {
        NXMC2Result ret = nxmc2_command_builder_clear(builder);
        if (ret != NXMC2_RESULT_OK)
        {
            return ret;
        }
        return NXMC2_RESULT_INVALID_HAT_RANGE_ERROR;
    }

    if (is_complete_command_(builder))
    {
        NXMC2Result ret = nxmc2_command_builder_clear(builder);
        if (ret != NXMC2_RESULT_OK)
        {
            return ret;
        }
    }
    builder->raw_[builder->ptr_] = packet;
    builder->ptr_++;
    return NXMC2_RESULT_OK;
}

static void transfer_(uint8_t *raw, NXMC2Command *command)
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

NXMC2Result nxmc2_command_builder_build(NXMC2CommandBuilder *builder, NXMC2Command *command)
{
    if (builder == NULL || command == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    if (!is_complete_command_(builder))
    {
        return NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR;
    }
    transfer_(builder->raw_, command);
    return NXMC2_RESULT_OK;
}

NXMC2Result nxmc2_command_builder_initialize(NXMC2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return NXMC2_RESULT_NULL_POINTER_ERROR;
    }

    NXMC2Result ret = nxmc2_command_builder_clear(builder);
    if (ret != NXMC2_RESULT_OK)
    {
        return ret;
    }
    return NXMC2_RESULT_OK;
}