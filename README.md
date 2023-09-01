# nxmc2_contrib

## Overview

The `nxmc2_contrib` library provides a straightforward way to build and execute commands conforming to the [NX Macro Controller v2](https://blog.bzl-web.com/entry/2020/01/20/165719) protocol[^1].

It includes the functionality for parsing raw byte sequences into structured command objects and subsequently executing these commands through a extensible executor. This makes it easy to develop NXMC2-compatible devices or software.

Written in the C programming language, this library is highly portable, making it easy to adapt across a variety of platforms.

[^1]: https://scrapbox.io/yatsuna827827-12010999/Nintendo_Switch%E3%82%92%E6%93%8D%E4%BD%9C%E3%81%99%E3%82%8B%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E3%81%AE%E4%BB%95%E6%A7%98%E3%82%92%E8%AA%BF%E3%81%B9%E3%82%8B

## Example

```c
#include <assert.h>

#include "nxmc2_contrib.h"

static NXMC2CommandButtonState y_state;
static void y(NXMC2CommandButtonState state) { y_state = state; }

static NXMC2CommandButtonState b_state;
static void b(NXMC2CommandButtonState state) { b_state = state; }

static NXMC2CommandHatState hat_state;
static void hat(NXMC2CommandHatState state) { hat_state = state; }

static uint8_t l_stick_x;
static uint8_t l_stick_y;
static void l_stick(uint8_t x, uint8_t y) { l_stick_x = x; l_stick_y = y; }

static uint8_t ext0_;
static uint8_t ext1_;
static uint8_t ext2_;
static void ext(uint8_t ext0, uint8_t ext1, uint8_t ext2) { ext0_ = ext0; ext1_ = ext1; ext2_ = ext2; }

int main(void)
{
    uint8_t data[] = {0xABU, 1U, 0U, 8U, 126U, 127U, 128U, 129U, 0xFFU, 0xFEU, 0xFDU};

    NXMC2CommandBuilder builder;
    nxmc2_command_builder_initialize(&builder);
    for (int i = 0; i < 11; i++)
    {
        nxmc2_command_builder_append(&builder, data[i]);
    }

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

    return 0;
}
```