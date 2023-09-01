#include "nxmc2_contrib.h"

void nxmc2_command_builder_clear(NXMC2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return;
    }

    builder->ptr_ = 0;
}

static bool can_build_(NXMC2CommandBuilder *builder)
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

void nxmc2_command_builder_append(NXMC2CommandBuilder *builder, uint8_t packet)
{
    if (builder == NULL)
    {
        return;
    }

    if (!(is_valid_header_(builder->ptr_, packet) && is_valid_hat_range_(builder->ptr_, packet)))
    {
        nxmc2_command_builder_clear(builder);
        return;
    }

    if (can_build_(builder))
    {
        nxmc2_command_builder_clear(builder);
    }
    builder->raw_[builder->ptr_] = packet;
    builder->ptr_++;
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

bool nxmc2_command_builder_build(NXMC2CommandBuilder *builder, NXMC2Command *command)
{
    if (builder == NULL || command == NULL)
    {
        return false;
    }

    if (!can_build_(builder))
    {
        return false;
    }
    transfer_(builder->raw_, command);
    return true;
}

void nxmc2_command_builder_initialize(NXMC2CommandBuilder *builder)
{
    if (builder == NULL)
    {
        return;
    }

    nxmc2_command_builder_clear(builder);
}