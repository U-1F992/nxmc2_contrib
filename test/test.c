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

static void test_util_append_all(NXMC2CommandBuilder *builder, uint8_t *data, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        nxmc2_command_builder_append(builder, data[i]);
    }
}

void test_build(void)
{
    uint8_t test[] = {0xABU, 1U, 0U, 8U, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    nxmc2_command_builder_initialize(&builder);
    test_util_append_all(&builder, test, 11);

    NXMC2Command command;
    assert(nxmc2_command_builder_build(&builder, &command));

    NXMC2CommandHandlers handlers;
    nxmc2_command_handlers_initialize(&handlers);
    handlers.y = y;
    handlers.b = b;
    handlers.hat = hat;
    handlers.l_stick = l_stick;
    handlers.ext = ext;
    nxmc2_command_execute(&command, &handlers);
    assert(y_state == NXMC2_COMMAND_BUTTON_STATE_PRESSED);
    assert(b_state == NXMC2_COMMAND_BUTTON_STATE_RELEASED);
    assert(hat_state == NXMC2_COMMAND_HAT_STATE_NEUTRAL);
    assert(l_stick_x == 126U);
    assert(l_stick_y == 127U);
    assert(ext0_ == 0xFFU);
    assert(ext1_ == 0xFEU);
    assert(ext2_ == 0xFDU);
}

void test_invalid_header(void)
{
    uint8_t test[] = {/* */ 0xFFU /* */, 1U, 0U, 8U, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    nxmc2_command_builder_initialize(&builder);
    test_util_append_all(&builder, test, 11);

    NXMC2Command command;
    assert(!nxmc2_command_builder_build(&builder, &command));
}

void test_invalid_hat_range(void)
{
    int8_t test[] = {0xABU, 0U, 0U, /* */ 0xFFU /* */, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    nxmc2_command_builder_initialize(&builder);
    test_util_append_all(&builder, test, 11);

    NXMC2Command command;
    assert(!nxmc2_command_builder_build(&builder, &command));
}

int main(void)
{
    test_build();
    test_invalid_header();
    test_invalid_hat_range();

    printf("ok\n");
    return 0;
}