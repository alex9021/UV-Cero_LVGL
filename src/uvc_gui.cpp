#include <uvc_gui.h>
#include <stdio.h>
#include <uvc_assets\logo_dark.h>
#include <uvc_assets\logo_light.h>

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

lv_obj_t *timerButtons[MAX_TIMER_COUNT];

static lv_style_t style_box;

lv_disp_size_t disp_size = LV_DISP_SIZE_SMALL;

static void create_controls(lv_obj_t *parent);
static void create_fans_status(lv_obj_t *parent);
static void create_lamps_status(lv_obj_t *parent);
static void create_timer_controls(lv_obj_t *parent);

static void fan_speed_slider_event_cb(lv_obj_t *slider, lv_event_t event);
static void lamp_switch_event_cb(lv_obj_t *sw, lv_event_t event);
static void timer_btn_event_cb(lv_obj_t *btn, lv_event_t event);

static void sensor_updater(lv_task_t *t);

LV_IMG_DECLARE(logo_dark);
LV_IMG_DECLARE(logo_light);

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

    /* show splash screen */
    lv_obj_t *splashscreen = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(splashscreen, &logo_dark);

    lv_obj_t *splashscreen_light = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(splashscreen_light, &logo_light);

    lv_obj_fade_out(splashscreen, 0, 4000);
    lv_obj_fade_out(splashscreen_light, 0, 2000);

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
    lv_bar_set_range(lamp_lm_1, 0, 1);
    lv_bar_set_anim_time(lamp_lm_1, 200);
    lv_bar_set_value(lamp_lm_1, lamps[0]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_2_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_2_label, "LAMP 2 LM");

    lamp_lm_2 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_2, 170, 20);
    lv_bar_set_range(lamp_lm_2, 0, 1);
    lv_bar_set_anim_time(lamp_lm_2, 200);
    lv_bar_set_value(lamp_lm_2, lamps[1]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_3_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_3_label, "LAMP 3 LM");

    lamp_lm_3 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_3, 170, 20);
    lv_bar_set_range(lamp_lm_3, 0, 1);
    lv_bar_set_anim_time(lamp_lm_3, 200);
    lv_bar_set_value(lamp_lm_3, lamps[2]->getCurrentLumen(), LV_ANIM_ON);

    lv_obj_t *lamp_lm_4_label = lv_label_create(h, NULL);
    lv_label_set_text(lamp_lm_4_label, "LAMP 4 LM");

    lamp_lm_4 = lv_bar_create(h, NULL);
    lv_obj_set_size(lamp_lm_4, 170, 20);
    lv_bar_set_range(lamp_lm_4, 0, 1);
    lv_bar_set_anim_time(lamp_lm_4, 200);
    lv_bar_set_value(lamp_lm_4, lamps[3]->getCurrentLumen(), LV_ANIM_ON);
}

static void create_timer_controls(lv_obj_t *parent)
{

    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    lv_obj_t *timerList = lv_list_create(parent, NULL);
    lv_cont_set_fit2(timerList, LV_FIT_PARENT, LV_FIT_TIGHT);
    lv_obj_align(timerList, NULL, LV_ALIGN_CENTER, 0, 0);

    for (int i = 0; i < currentTimerItemCount; i++)
    {
        char labelText[50];
        sprintf(labelText, "%s %s for %dmin %s",
                timerItems[i]->getWeekdayAsString().c_str(),
                timerItems[i]->getStartTimeAsString().c_str(),
                timerItems[i]->getDuration(),
                timerItems[i]->getRepeatAsString().c_str());

        timerButtons[i] = lv_list_add_btn(timerList, NULL, labelText);
        lv_obj_set_event_cb(timerButtons[i], timer_btn_event_cb);
    }
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

    lv_bar_set_value(lamp_lm_1, lamps[0]->getCurrentLumen(), LV_ANIM_OFF);
    lv_bar_set_value(lamp_lm_2, lamps[1]->getCurrentLumen(), LV_ANIM_OFF);
    lv_bar_set_value(lamp_lm_3, lamps[2]->getCurrentLumen(), LV_ANIM_OFF);
    lv_bar_set_value(lamp_lm_4, lamps[3]->getCurrentLumen(), LV_ANIM_OFF);
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

static void timer_btn_event_cb(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        for (int i = 0; i < currentTimerItemCount; i++)
        {
            if (timerButtons[i] == btn)
            {
                Serial.println(timerItems[i]->getStartTimeAsString().c_str());
            }
        }

        lv_obj_t *win = lv_win_create(lv_scr_act(), NULL);
        lv_win_set_title(win, "EDIT TIMER");

        lv_win_set_layout(win, LV_LAYOUT_GRID);

        //lv_coord_t grid_w = lv_win_get_width(win);

        lv_obj_t *close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE); /*Add close button and use built-in close action*/
        lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
        
        lv_obj_t *delete_btn = lv_win_add_btn(win, LV_SYMBOL_TRASH); /*Add close button and use built-in close action*/
        lv_obj_set_event_cb(delete_btn, NULL);

        lv_obj_t *repeatDropDown = lv_dropdown_create(win, NULL);
        lv_dropdown_set_options(repeatDropDown, "none\n"
                                                "hourly\n"
                                                "daily\n"
                                                "weekly\n");

        lv_obj_align(repeatDropDown, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
        //lv_obj_set_event_cb(weekdayDropDown, NULL);

        lv_obj_t *weekdayDropDown = lv_dropdown_create(win, NULL);
        lv_dropdown_set_options(weekdayDropDown, "Monday\n"
                                                 "Tuesday\n"
                                                 "Wednesday\n"
                                                 "Thurday\n"
                                                 "Friday\n"
                                                 "Saturday\n"
                                                 "Sunday\n");

        lv_obj_align(weekdayDropDown, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

        lv_obj_t *hoursRoller = lv_roller_create(win, NULL);
        lv_roller_set_options(hoursRoller,
                              "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\11\n"
                              "12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
                              LV_ROLLER_MODE_INFINITE);

        lv_roller_set_visible_row_count(hoursRoller, 4);
        lv_obj_align(hoursRoller, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(hoursRoller, NULL);

        lv_obj_t *minutesRoller = lv_roller_create(win, NULL);
        lv_roller_set_options(minutesRoller,
                              "00\n"
                              "15\n"
                              "30\n"
                              "45",
                              LV_ROLLER_MODE_INFINITE);

        lv_roller_set_visible_row_count(minutesRoller, 4);
        lv_obj_align(minutesRoller, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(minutesRoller, NULL);

        lv_obj_t *btnApply = lv_btn_create(win, NULL);
        lv_obj_set_event_cb(btnApply, NULL);
        lv_cont_set_fit2(btnApply, LV_FIT_PARENT, LV_FIT_TIGHT);
        lv_obj_set_style_local_bg_color(btnApply, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);

        lv_obj_t *btnApplyLabel = lv_label_create(btnApply, NULL);
        lv_label_set_text(btnApplyLabel, "APPLY");
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
