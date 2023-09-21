#include <Arduino.h>

#include "nxmc2_contrib.h"

static char buf_[256];

static void handle_y(NXMC2CommandButtonState state)
{
    sprintf(buf_, "Y\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_b(NXMC2CommandButtonState state)
{
    sprintf(buf_, "B\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_a(NXMC2CommandButtonState state)
{
    sprintf(buf_, "A\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_x(NXMC2CommandButtonState state)
{
    sprintf(buf_, "X\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_l(NXMC2CommandButtonState state)
{
    sprintf(buf_, "L\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_r(NXMC2CommandButtonState state)
{
    sprintf(buf_, "R\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_zl(NXMC2CommandButtonState state)
{
    sprintf(buf_, "ZL\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_zr(NXMC2CommandButtonState state)
{
    sprintf(buf_, "ZR\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_minus(NXMC2CommandButtonState state)
{
    sprintf(buf_, "Minus\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_plus(NXMC2CommandButtonState state)
{
    sprintf(buf_, "Plus\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_l_click(NXMC2CommandButtonState state)
{
    sprintf(buf_, "L Click\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_r_click(NXMC2CommandButtonState state)
{
    sprintf(buf_, "R Click\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_home(NXMC2CommandButtonState state)
{
    sprintf(buf_, "Home\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_capture(NXMC2CommandButtonState state)
{
    sprintf(buf_, "Capture\t%s", state == NXMC2_COMMAND_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial.println(buf_);
}
static void handle_hat(NXMC2CommandHatState state)
{
    const char *tmp;
    switch (state)
    {
    case NXMC2_COMMAND_HAT_STATE_UP:
        tmp = "Up";
        break;
    case NXMC2_COMMAND_HAT_STATE_UPRIGHT:
        tmp = "UpRight";
        break;
    case NXMC2_COMMAND_HAT_STATE_RIGHT:
        tmp = "Right";
        break;
    case NXMC2_COMMAND_HAT_STATE_DOWNRIGHT:
        tmp = "DownRight";
        break;
    case NXMC2_COMMAND_HAT_STATE_DOWN:
        tmp = "Down";
        break;
    case NXMC2_COMMAND_HAT_STATE_DOWNLEFT:
        tmp = "DownLeft";
        break;
    case NXMC2_COMMAND_HAT_STATE_LEFT:
        tmp = "Left";
        break;
    case NXMC2_COMMAND_HAT_STATE_UPLEFT:
        tmp = "UpLeft";
        break;
    case NXMC2_COMMAND_HAT_STATE_NEUTRAL:
        tmp = "Neurtal";
        break;
    default:
        tmp = "[Error]";
        break;
    }
    sprintf(buf_, "Hat\t%s", tmp);
    Serial.println(buf_);
}
static void handle_l_stick(uint8_t x, uint8_t y)
{
    sprintf(buf_, "L Stick\tx:%d,y:%d", x, y);
    Serial.println(buf_);
}
static void handle_r_stick(uint8_t x, uint8_t y)
{
    sprintf(buf_, "R Stick\tx:%d,y:%d", x, y);
    Serial.println(buf_);
}
static void handle_ext(uint8_t ext0, uint8_t ext1, uint8_t ext2)
{
    sprintf(buf_, "Ext\t%d,%d,%d", ext0, ext1, ext2);
    Serial.println(buf_);
}

static const int SERIAL_INACTIVE_TIMEOUT = 100;
static int inactive_count = 0;

static NXMC2CommandBuilder builder;
static NXMC2Command command;
static NXMC2CommandHandler handler;

void setup()
{
    Serial.begin(9600);

    nxmc2_command_builder_new(&builder);
    nxmc2_command_handler_new(&handler);
    handler.y = handle_y;
    handler.b = handle_b;
    handler.a = handle_a;
    handler.x = handle_x;
    handler.l = handle_l;
    handler.r = handle_r;
    handler.zl = handle_zl;
    handler.zr = handle_zr;
    handler.minus = handle_minus;
    handler.plus = handle_plus;
    handler.l_click = handle_l_click;
    handler.r_click = handle_r_click;
    handler.home = handle_home;
    handler.capture = handle_capture;
    handler.hat = handle_hat;
    handler.l_stick = handle_l_stick;
    handler.r_stick = handle_r_stick;
    handler.ext = handle_ext;
}

void loop()
{
    if (Serial.available() == 0)
    {
        inactive_count++;
        if (SERIAL_INACTIVE_TIMEOUT < inactive_count)
        {
            inactive_count = 0;
            nxmc2_command_builder_flush(&builder);
        }
        return;
    }
    inactive_count = 0;

    NXMC2Result ret = nxmc2_command_builder_append(&builder, Serial.read());
    if (ret != NXMC2_RESULT_OK)
    {
        // NXMC2_RESULT_INVALID_HEADER_ERROR
        // NXMC2_RESULT_INVALID_HAT_ERROR
        // NXMC2_RESULT_COMMAND_READY_ERROR
        nxmc2_command_builder_flush(&builder);
        return;
    }

    ret = nxmc2_command_builder_build(&builder, &command);
    if (ret != NXMC2_RESULT_OK)
    {
        // NXMC2_RESULT_INCOMPLETE_COMMAND_ERROR
        return;
    }
    nxmc2_command_execute(&command, &handler);
    
    nxmc2_command_builder_flush(&builder);
}