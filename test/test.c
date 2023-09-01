#include <assert.h>
#include <stdio.h>

#include "nxmc2_contrib.h"

static NXMC2CommandButtonState y_state;
static void y(NXMC2CommandButtonState state)
{
    y_state = state;
}

static NXMC2CommandButtonState b_state;
static void b(NXMC2CommandButtonState state)
{
    b_state = state;
}

static NXMC2CommandHatState hat_state;
static void hat(NXMC2CommandHatState state)
{
    hat_state = state;
}

static uint8_t l_stick_x;
static uint8_t l_stick_y;
static void l_stick(uint8_t x, uint8_t y)
{
    l_stick_x = x;
    l_stick_y = y;
}

static uint8_t ext0_;
static uint8_t ext1_;
static uint8_t ext2_;
static void ext(uint8_t ext0, uint8_t ext1, uint8_t ext2)
{
    ext0_ = ext0;
    ext1_ = ext1;
    ext2_ = ext2;
}

void test_build(void)
{
    uint8_t data[] = {0xABU, 1U, 0U, 8U, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    assert(nxmc2_command_builder_initialize(&builder) == NXMC2_RESULT_OK);
    for (int i = 0; i < 11; i++)
    {
        assert(nxmc2_command_builder_append(&builder, data[i]) == NXMC2_RESULT_OK);
    }

    NXMC2Command command;
    assert(nxmc2_command_builder_build(&builder, &command) == NXMC2_RESULT_OK);

    NXMC2CommandHandler handler;
    assert(nxmc2_command_handler_initialize(&handler) == NXMC2_RESULT_OK);
    handler.y = y;
    handler.b = b;
    handler.hat = hat;
    handler.l_stick = l_stick;
    handler.ext = ext;
    assert(nxmc2_command_execute(&command, &handler) == NXMC2_RESULT_OK);
    assert(y_state == NXMC2_COMMAND_BUTTON_STATE_PRESSED);
    assert(b_state == NXMC2_COMMAND_BUTTON_STATE_RELEASED);
    assert(hat_state == NXMC2_COMMAND_HAT_STATE_NEUTRAL);
    assert(l_stick_x == 126U);
    assert(l_stick_y == 127U);
    assert(ext0_ == 0xFFU);
    assert(ext1_ == 0xFEU);
    assert(ext2_ == 0xFDU);

    // Once the command is completed, flush required.
    assert(nxmc2_command_builder_append(&builder, 0xAB) == NXMC2_RESULT_COMMAND_READY_ERROR);
    assert(nxmc2_command_builder_flush(&builder) == NXMC2_RESULT_OK);
    assert(nxmc2_command_builder_append(&builder, 0xAB) == NXMC2_RESULT_OK);
}

void test_invalid_header(void)
{
    uint8_t data[] = {/* */ 0xFFU /* */, 1U, 0U, 8U, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    assert(nxmc2_command_builder_initialize(&builder) == NXMC2_RESULT_OK);

    assert(nxmc2_command_builder_append(&builder, data[0]) == NXMC2_RESULT_INVALID_HEADER_ERROR);
    for (int i = 1; i < 11; i++)
    {
        assert(nxmc2_command_builder_append(&builder, data[i]) == NXMC2_RESULT_INVALID_HEADER_ERROR);
    }

    NXMC2Command command;
    assert(nxmc2_command_builder_build(&builder, &command) == NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR);
}

void test_invalid_hat(void)
{
    int8_t data[] = {0xABU, 0U, 0U, /* */ 0xFFU /* */, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    assert(nxmc2_command_builder_initialize(&builder) == NXMC2_RESULT_OK);

    for (int i = 0; i < 3; i++)
    {
        assert(nxmc2_command_builder_append(&builder, data[i]) == NXMC2_RESULT_OK);
    }
    assert(nxmc2_command_builder_append(&builder, data[3]) == NXMC2_RESULT_INVALID_HAT_ERROR);
    for (int i = 4; i < 11; i++)
    {
        assert(nxmc2_command_builder_append(&builder, data[i]) == NXMC2_RESULT_INVALID_HAT_ERROR);
    }

    NXMC2Command command;
    assert(nxmc2_command_builder_build(&builder, &command) == NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR);
}

int main(void)
{
    test_build();
    test_invalid_header();
    test_invalid_hat();

    printf("ok\n");
    return 0;
}