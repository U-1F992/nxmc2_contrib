#ifndef NXMC2_CONTRIB_H_
#define NXMC2_CONTRIB_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

extern const uint8_t NXMC2_COMMAND_HEADER;

typedef enum Nxmc2CommandButtonState
{
    NXMC2_COMMAND_BUTTON_STATE_RELEASED,
    NXMC2_COMMAND_BUTTON_STATE_PRESSED
} Nxmc2CommandButtonState;

typedef enum Nxmc2CommandHatState
{
    NXMC2_COMMAND_HAT_STATE_UP,
    NXMC2_COMMAND_HAT_STATE_UPRIGHT,
    NXMC2_COMMAND_HAT_STATE_RIGHT,
    NXMC2_COMMAND_HAT_STATE_DOWNRIGHT,
    NXMC2_COMMAND_HAT_STATE_DOWN,
    NXMC2_COMMAND_HAT_STATE_DOWNLEFT,
    NXMC2_COMMAND_HAT_STATE_LEFT,
    NXMC2_COMMAND_HAT_STATE_UPLEFT,
    NXMC2_COMMAND_HAT_STATE_NEUTRAL
} Nxmc2CommandHatState;

extern const uint8_t NXMC2_COMMAND_STICK_NEUTRAL;

typedef struct Nxmc2Command
{
    uint8_t header;

    Nxmc2CommandButtonState y;
    Nxmc2CommandButtonState b;
    Nxmc2CommandButtonState a;
    Nxmc2CommandButtonState x;
    Nxmc2CommandButtonState l;
    Nxmc2CommandButtonState r;
    Nxmc2CommandButtonState zl;
    Nxmc2CommandButtonState zr;

    Nxmc2CommandButtonState minus;
    Nxmc2CommandButtonState plus;
    Nxmc2CommandButtonState l_click;
    Nxmc2CommandButtonState r_click;
    Nxmc2CommandButtonState home;
    Nxmc2CommandButtonState capture;
    uint8_t _;

    Nxmc2CommandHatState hat;
    uint8_t lx;
    uint8_t ly;
    uint8_t rx;
    uint8_t ry;

    uint8_t ext0;
    uint8_t ext1;
    uint8_t ext2;
} Nxmc2Command;

typedef struct Nxmc2CommandBuilder
{
    uint8_t buf_[11];
    size_t pos_;
} Nxmc2CommandBuilder;

typedef struct Nxmc2CommandHandler
{
    void (*y)(Nxmc2CommandButtonState state);
    void (*b)(Nxmc2CommandButtonState state);
    void (*a)(Nxmc2CommandButtonState state);
    void (*x)(Nxmc2CommandButtonState state);
    void (*l)(Nxmc2CommandButtonState state);
    void (*r)(Nxmc2CommandButtonState state);
    void (*zl)(Nxmc2CommandButtonState state);
    void (*zr)(Nxmc2CommandButtonState state);
    void (*minus)(Nxmc2CommandButtonState state);
    void (*plus)(Nxmc2CommandButtonState state);
    void (*l_click)(Nxmc2CommandButtonState state);
    void (*r_click)(Nxmc2CommandButtonState state);
    void (*home)(Nxmc2CommandButtonState state);
    void (*capture)(Nxmc2CommandButtonState state);
    void (*hat)(Nxmc2CommandHatState state);
    void (*l_stick)(uint8_t x, uint8_t y);
    void (*r_stick)(uint8_t x, uint8_t y);
    void (*ext)(uint8_t ext0, uint8_t ext1, uint8_t ext2);
} Nxmc2CommandHandler;

typedef enum Nxmc2Result
{
    NXMC2_RESULT_OK,
    NXMC2_RESULT_NULL_POINTER_ERROR,
    NXMC2_RESULT_INVALID_HEADER_ERROR,
    NXMC2_RESULT_INVALID_HAT_ERROR,
    NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR,
    NXMC2_RESULT_COMMAND_READY_ERROR
} Nxmc2Result;

Nxmc2Result nxmc2_command_builder_flush(Nxmc2CommandBuilder *builder);
Nxmc2Result nxmc2_command_builder_append(Nxmc2CommandBuilder *builder, uint8_t packet);
Nxmc2Result nxmc2_command_builder_build(Nxmc2CommandBuilder *builder, Nxmc2Command *command);
Nxmc2Result nxmc2_command_builder_new(Nxmc2CommandBuilder *builder);
Nxmc2Result nxmc2_command_execute(Nxmc2Command *command, Nxmc2CommandHandler *handler);
Nxmc2Result nxmc2_command_handler_new(Nxmc2CommandHandler *handler);

#ifdef __cplusplus
}
#endif

#endif