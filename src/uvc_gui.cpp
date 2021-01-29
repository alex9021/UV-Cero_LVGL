#include <uvc_gui.h>
#include <stdio.h>

static lv_obj_t *tv;
static lv_obj_t *t1;
static lv_obj_t *t2;
static lv_obj_t *t3;
static lv_obj_t *t4;

static lv_obj_t *fan_speed_slider;
static lv_obj_t *fan_speed_slider_label;

static lv_obj_t *lamp_switch_label;
static lv_obj_t *lamp_switch;

static lv_obj_t *fan_rpm_1;
static lv_obj_t *fan_rpm_2;
static lv_obj_t *fan_rpm_3;
static lv_obj_t *fan_rpm_4;

static lv_obj_t *lamp_lm_1;
static lv_obj_t *lamp_lm_2;
static lv_obj_t *lamp_lm_3;
static lv_obj_t *lamp_lm_4;

static lv_style_t style_box;

lv_disp_size_t disp_size = LV_DISP_SIZE_SMALL;

static void create_controls(lv_obj_t *parent);
static void create_fans_status(lv_obj_t *parent);
static void create_lamps_status(lv_obj_t *parent);
static void create_timer_controls(lv_obj_t *parent);

static void fan_speed_slider_event_cb(lv_obj_t *slider, lv_event_t event);
static void lamp_switch_event_cb(lv_obj_t *sw, lv_event_t event);

static void sensor_updater(lv_task_t *t);

void showSplashScreen(void)
{
    digitalWrite(TFT_BL, LOW);
    delay(1000);
    digitalWrite(TFT_BL, HIGH);
}

void create_gui(void)
{
    tv = lv_tabview_create(lv_scr_act(), NULL);
    t1 = lv_tabview_add_tab(tv, "CONTROLS");
    t2 = lv_tabview_add_tab(tv, "FANS");
    t3 = lv_tabview_add_tab(tv, "LAMPS");
    t4 = lv_tabview_add_tab(tv, "TIMER");

    lv_style_init(&style_box);
    lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
    lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, -LV_DPX(10));
    lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

    create_controls(t1);
    create_fans_status(t2);
    create_lamps_status(t3);
    create_timer_controls(t4);

    lv_task_create(sensor_updater, 200, LV_TASK_PRIO_LOWEST, NULL);
}

static void create_controls(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

    lv_obj_t *h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);

    lamp_switch_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_switch_label, "LAMP SWITCH");

    lamp_switch = lv_switch_create(h, NULL);
    lv_obj_set_event_cb(lamp_switch, lamp_switch_event_cb);

    lv_coord_t fit_w = lv_obj_get_width_fit(h);

    lv_obj_t *fan_speed_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_speed_label, "FAN SPEED CONTROL");
    lv_obj_set_style_local_margin_top(fan_speed_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_DPX(10));

    fan_speed_slider = lv_slider_create(h, NULL);
    lv_slider_set_value(fan_speed_slider, FAN_SPEED_HIGH, LV_ANIM_ON);
    lv_slider_set_range(fan_speed_slider, FAN_SPEED_OFF, FAN_SPEED_HIGH);
    lv_obj_set_height(fan_speed_slider, LV_DPX(18));
    lv_obj_set_event_cb(fan_speed_slider, fan_speed_slider_event_cb);
    lv_obj_set_width_margin(fan_speed_slider, fit_w);

    fan_speed_slider_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_speed_slider_label, "HIGH");

    lv_obj_set_style_local_margin_top(fan_speed_slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, LV_DPX(5));
    lv_obj_set_style_local_value_font(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_ofs_y(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, -LV_DPX(25));
    lv_obj_set_style_local_value_opa(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_value_opa(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, LV_OPA_COVER);
    lv_obj_set_style_local_transition_time(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, 200);
    lv_obj_set_style_local_transition_prop_5(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OFS_Y);
    lv_obj_set_style_local_transition_prop_6(fan_speed_slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OPA);
}

static void create_fans_status(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

    lv_obj_t *h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);

    lv_obj_t *fan_rpm_1_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_rpm_1_label, "FAN 1 RPM");

    fan_rpm_1 = lv_bar_create(h, NULL);
    lv_obj_set_size(fan_rpm_1, 170, 20);
    lv_bar_set_range(fan_rpm_1, 0, 10000);
    lv_bar_set_anim_time(fan_rpm_1, 200);
    lv_bar_set_value(fan_rpm_1, fans[0]->getCurrentRPM(), LV_ANIM_ON);

    lv_obj_t *fan_rpm_2_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_rpm_2_label, "FAN 2 RPM");

    fan_rpm_2 = lv_bar_create(h, NULL);
    lv_obj_set_size(fan_rpm_2, 170, 20);
    lv_bar_set_range(fan_rpm_2, 0, 10000);
    lv_bar_set_anim_time(fan_rpm_2, 200);
    lv_bar_set_value(fan_rpm_2, fans[1]->getCurrentRPM(), LV_ANIM_ON);

    lv_obj_t *fan_rpm_3_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_rpm_3_label, "FAN 3 RPM");

    fan_rpm_3 = lv_bar_create(h, NULL);
    lv_obj_set_size(fan_rpm_3, 170, 20);
    lv_bar_set_range(fan_rpm_3, 0, 10000);
    lv_bar_set_anim_time(fan_rpm_3, 200);
    lv_bar_set_value(fan_rpm_3, fans[2]->getCurrentRPM(), LV_ANIM_ON);

    lv_obj_t *fan_rpm_4_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_rpm_4_label, "FAN 4 RPM");

    fan_rpm_4 = lv_bar_create(h, NULL);
    lv_obj_set_size(fan_rpm_4, 170, 20);
    lv_bar_set_range(fan_rpm_4, 0, 10000);
    lv_bar_set_anim_time(fan_rpm_4, 200);
    lv_bar_set_value(fan_rpm_4, fans[3]->getCurrentRPM(), LV_ANIM_ON);
}

static void create_lamps_status(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

    lv_obj_t *h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);

    lv_obj_t *lamp_lm_1_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_1_label, "LAMP 1 LM");

    lamp_lm_1 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_1, 170, 20);
    lv_bar_set_range(lamp_lm_1, 0, 1000);
    lv_bar_set_anim_time(lamp_lm_1, 200);
    lv_bar_set_value(lamp_lm_1, lamps[0]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_2_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_2_label, "LAMP 2 LM");

    lamp_lm_2 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_2, 170, 20);
    lv_bar_set_range(lamp_lm_2, 0, 1000);
    lv_bar_set_anim_time(lamp_lm_2, 200);
    lv_bar_set_value(lamp_lm_2, lamps[1]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_3_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_3_label, "LAMP 3 LM");

    lamp_lm_3 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_3, 170, 20);
    lv_bar_set_range(lamp_lm_3, 0, 1000);
    lv_bar_set_anim_time(lamp_lm_3, 200);
    lv_bar_set_value(lamp_lm_3, lamps[2]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_4_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_4_label, "LAMP 4 LM");

    lamp_lm_4 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_4, 170, 20);
    lv_bar_set_range(lamp_lm_4, 0, 1000);
    lv_bar_set_anim_time(lamp_lm_4, 200);
    lv_bar_set_value(lamp_lm_4, lamps[3]->getCurrentLumen(), LV_ANIM_ON);
}

static void create_timer_controls(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

    lv_obj_t *h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);

    lv_obj_t * btn_monday = lv_btn_create(h, NULL);
    lv_obj_set_event_cb(btn_monday, NULL);
    lv_obj_align(btn_monday, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_btn_set_fit2(btn_monday, LV_FIT_NONE, LV_FIT_TIGHT);

    lv_obj_t *btn_monday_label = lv_label_create(btn_monday, NULL);
    lv_label_set_text(btn_monday_label, "Monday");

    lv_obj_t * btn_tuesday = lv_btn_create(h, NULL);
    lv_obj_set_event_cb(btn_tuesday, NULL);
    lv_obj_align(btn_tuesday, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_btn_set_fit2(btn_tuesday, LV_FIT_NONE, LV_FIT_TIGHT);

    lv_obj_t *btn_tuesday_label = lv_label_create(btn_tuesday, NULL);
    lv_label_set_text(btn_tuesday_label, "Tuesday");
}

static void sensor_updater(lv_task_t *t)
{
    switch (currentFanSpeed)
    {
    case FAN_SPEED_OFF:
        lv_slider_set_value(fan_speed_slider, FAN_SPEED_OFF, LV_ANIM_ON);
        lv_label_set_text(fan_speed_slider_label, "OFF");
        break;

    case FAN_SPEED_LOW:
        lv_slider_set_value(fan_speed_slider, FAN_SPEED_LOW, LV_ANIM_ON);
        lv_label_set_text(fan_speed_slider_label, "LOW");
        break;

    case FAN_SPEED_MEDIUM:
        lv_slider_set_value(fan_speed_slider, FAN_SPEED_MEDIUM, LV_ANIM_ON);
        lv_label_set_text(fan_speed_slider_label, "MEDIUM");
        break;

    case FAN_SPEED_HIGH:
        lv_slider_set_value(fan_speed_slider, FAN_SPEED_HIGH, LV_ANIM_ON);
        lv_label_set_text(fan_speed_slider_label, "HIGH");
        break;
    }

    switch (currentLampState)
    {
    case LAMPS_OFF:
        lv_switch_off(lamp_switch, LV_ANIM_ON);
        break;

    case LAMPS_ON:
        lv_switch_on(lamp_switch, LV_ANIM_ON);
        break;
    }

    lv_bar_set_value(fan_rpm_1, fans[0]->getCurrentRPM(), LV_ANIM_ON);
    lv_bar_set_value(fan_rpm_2, fans[1]->getCurrentRPM(), LV_ANIM_ON);
    lv_bar_set_value(fan_rpm_3, fans[2]->getCurrentRPM(), LV_ANIM_ON);
    lv_bar_set_value(fan_rpm_4, fans[3]->getCurrentRPM(), LV_ANIM_ON);

    lv_bar_set_value(lamp_lm_1, lamps[0]->getCurrentLumen(), LV_ANIM_ON);
    lv_bar_set_value(lamp_lm_2, lamps[1]->getCurrentLumen(), LV_ANIM_ON);
    lv_bar_set_value(lamp_lm_3, lamps[2]->getCurrentLumen(), LV_ANIM_ON);
    lv_bar_set_value(lamp_lm_4, lamps[3]->getCurrentLumen(), LV_ANIM_ON);
}

static void fan_speed_slider_event_cb(lv_obj_t *slider, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:

        switch (lv_slider_get_value(slider))
        {
        case FAN_SPEED_OFF:
            updateFanSpeed(FAN_SPEED_OFF);
            break;

        case FAN_SPEED_LOW:
            updateFanSpeed(FAN_SPEED_LOW);
            break;

        case FAN_SPEED_MEDIUM:
            updateFanSpeed(FAN_SPEED_MEDIUM);
            break;

        case FAN_SPEED_HIGH:
            updateFanSpeed(FAN_SPEED_HIGH);
            break;
        }
        break;

    default:
        break;
    }
}

static void lamp_switch_event_cb(lv_obj_t *sw, lv_event_t event)
{

    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:

        switch (lv_slider_get_value(sw))
        {
        case LAMPS_OFF:
            updateLampState(LAMPS_OFF);
            break;

        case LAMPS_ON:
            updateLampState(LAMPS_ON);
            break;
        }

        break;

    default:
        break;
    }
}

void updateFanSpeed(fan_speed_t fanSpeed)
{
    if (xQueueSend(xFAN_SPEED, &fanSpeed, portMAX_DELAY) == pdPASS)
    {
        switch (currentFanSpeed)
        {
        case FAN_SPEED_OFF:
            lv_slider_set_value(fan_speed_slider, FAN_SPEED_OFF, LV_ANIM_ON);
            lv_label_set_text(fan_speed_slider_label, "OFF");
            break;

        case FAN_SPEED_LOW:
            lv_slider_set_value(fan_speed_slider, FAN_SPEED_LOW, LV_ANIM_ON);
            lv_label_set_text(fan_speed_slider_label, "LOW");
            break;

        case FAN_SPEED_MEDIUM:
            lv_slider_set_value(fan_speed_slider, FAN_SPEED_MEDIUM, LV_ANIM_ON);
            lv_label_set_text(fan_speed_slider_label, "MEDIUM");
            break;

        case FAN_SPEED_HIGH:
            lv_slider_set_value(fan_speed_slider, FAN_SPEED_HIGH, LV_ANIM_ON);
            lv_label_set_text(fan_speed_slider_label, "HIGH");
            break;
        }
    }
}

void updateLampState(lamp_state_t lampsState)
{
    switch (lampsState)
    {
    case LAMPS_OFF:
        currentLampState = LAMPS_OFF;
        digitalWrite(LAMP_SWITCH_PIN, LOW);
        Serial.println("Lamp switch turned off.");
        break;

    case LAMPS_ON:
        currentLampState = LAMPS_ON;
        digitalWrite(LAMP_SWITCH_PIN, HIGH);
        Serial.println("Lamp switch turned on.");
        break;
    }
}
