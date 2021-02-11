#include <uvc_gui.h>
#include <stdio.h>
#include <uvc_assets\logo_dark.h>

static lv_obj_t *tv;
static lv_obj_t *t1;
static lv_obj_t *t2;
static lv_obj_t *t3;
static lv_obj_t *t4;

static lv_obj_t *fan_speed_slider;
static lv_obj_t *fan_speed_slider_label;

/*
static lv_obj_t *enable_switch_label;
static lv_obj_t *enable_switch;
*/

static lv_obj_t *fan_1_status_led;
static lv_obj_t *fan_2_status_led;
static lv_obj_t *fan_3_status_led;
static lv_obj_t *fan_4_status_led;

static lv_obj_t *lamp_1_status_led;
static lv_obj_t *lamp_2_status_led;
static lv_obj_t *lamp_3_status_led;
static lv_obj_t *lamp_4_status_led;

lv_obj_t *timerList;
lv_obj_t *timerButtons[MAX_TIMER_COUNT];
lv_obj_t *timeLabel;

static lv_style_t style_box;

lv_disp_size_t disp_size = LV_DISP_SIZE_SMALL;

static void create_controls(lv_obj_t *parent);
static void create_components_status(lv_obj_t *parent);
static void create_info(lv_obj_t *parent);
static void create_timer_controls(lv_obj_t *parent);

static void fan_speed_slider_event_cb(lv_obj_t *slider, lv_event_t event);
//static void enable_switch_event_cb(lv_obj_t *sw, lv_event_t event);
static void timer_btn_event_cb(lv_obj_t *btn, lv_event_t event);
static void timer_item_add_event_cb(lv_obj_t *add_btn, lv_event_t event);

static void sensor_updater(lv_task_t *t);

void createTimerItemDetailsWindow(bool update, int timerItemIndex);
void removeTimerItemByIndex(int timerItemindex, lv_obj_t *win);

LV_IMG_DECLARE(logo_dark);

void create_gui(void)
{
    tv = lv_tabview_create(lv_scr_act(), NULL);
    t1 = lv_tabview_add_tab(tv, "CONTROL");
    t2 = lv_tabview_add_tab(tv, "STATUS");
    t3 = lv_tabview_add_tab(tv, "TIMER");
    t4 = lv_tabview_add_tab(tv, "INFO");

    lv_tabview_set_btns_pos(tv, LV_TABVIEW_TAB_POS_BOTTOM);

    lv_style_init(&style_box);
    lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
    lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, -LV_DPX(10));
    lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(10));

    create_controls(t1);
    create_components_status(t2);
    create_timer_controls(t3);
    create_info(t4);

    /* show splash screen */
    lv_obj_t *splashscreen = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(splashscreen, &logo_dark);
    lv_obj_fade_out(splashscreen, 0, 2000);

    lv_task_create(sensor_updater, 1000, LV_TASK_PRIO_LOWEST, NULL);
}

static void create_controls(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

    //lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);

    lv_obj_t *deviceLabel = lv_label_create(parent, NULL);
    lv_label_set_text(deviceLabel, "ReTecCom UV-Cero");
    lv_obj_set_width(deviceLabel, 150);
    lv_obj_set_height(deviceLabel, 5);

    timeLabel = lv_label_create(parent, NULL);
    lv_obj_set_width(timeLabel, 150);
    lv_obj_set_height(timeLabel, 5);

    lv_obj_t *h = lv_cont_create(parent, NULL);
    lv_cont_set_fit2(h, LV_FIT_PARENT, LV_FIT_TIGHT);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);

    lv_obj_t *fan_speed_label = lv_label_create(h, NULL);
    lv_label_set_text(fan_speed_label, "AIR PURIFICATION");
    lv_obj_set_style_local_margin_top(fan_speed_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_DPX(10));

    fan_speed_slider = lv_slider_create(h, NULL);
    lv_slider_set_value(fan_speed_slider, FAN_SPEED_HIGH, LV_ANIM_ON);
    lv_slider_set_range(fan_speed_slider, FAN_SPEED_OFF, FAN_SPEED_HIGH);
    lv_obj_set_event_cb(fan_speed_slider, fan_speed_slider_event_cb);
    lv_obj_set_width_fit(h, lv_obj_get_width_fit(h));

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

static void create_components_status(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, 2, 1);

    lv_obj_t *fan_status_container = lv_cont_create(parent, NULL);
    lv_cont_set_layout(fan_status_container, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(fan_status_container, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(fan_status_container, true);

    lv_cont_set_fit2(fan_status_container, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(fan_status_container, grid_w);

    lv_obj_t *fan_1_status_label = lv_label_create(fan_status_container, NULL);
    lv_label_set_text(fan_1_status_label, " FAN 1");

    fan_1_status_led = lv_led_create(fan_status_container, NULL);
    lv_obj_align(fan_1_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(fan_1_status_led, 16, 16);

    lv_obj_t *fan_2_status_label = lv_label_create(fan_status_container, NULL);
    lv_label_set_text(fan_2_status_label, " FAN 2");

    fan_2_status_led = lv_led_create(fan_status_container, NULL);
    lv_obj_align(fan_2_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(fan_2_status_led, 16, 16);

    lv_obj_t *fan_3_status_label = lv_label_create(fan_status_container, NULL);
    lv_label_set_text(fan_3_status_label, " FAN 3");

    fan_3_status_led = lv_led_create(fan_status_container, NULL);
    lv_obj_align(fan_3_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(fan_3_status_led, 16, 16);

    lv_obj_t *fan_4_status_label = lv_label_create(fan_status_container, NULL);
    lv_label_set_text(fan_4_status_label, " FAN 4");

    fan_4_status_led = lv_led_create(fan_status_container, NULL);
    lv_obj_align(fan_4_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(fan_4_status_led, 16, 16);

    lv_obj_t *lamp_status_container = lv_cont_create(parent, NULL);
    lv_cont_set_layout(lamp_status_container, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(lamp_status_container, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(lamp_status_container, true);

    lv_cont_set_fit2(lamp_status_container, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(lamp_status_container, grid_w);

    lv_obj_t *lamp_1_status_label = lv_label_create(lamp_status_container, NULL);
    lv_label_set_text(lamp_1_status_label, " LAMP 1");

    lamp_1_status_led = lv_led_create(lamp_status_container, NULL);
    lv_obj_align(lamp_1_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(lamp_1_status_led, 16, 16);

    lv_obj_t *lamp_2_status_label = lv_label_create(lamp_status_container, NULL);
    lv_label_set_text(lamp_2_status_label, " LAMP 2");

    lamp_2_status_led = lv_led_create(lamp_status_container, NULL);
    lv_obj_align(lamp_2_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(lamp_2_status_led, 16, 16);

    lv_obj_t *lamp_3_status_label = lv_label_create(lamp_status_container, NULL);
    lv_label_set_text(lamp_3_status_label, " LAMP 3");

    lamp_3_status_led = lv_led_create(lamp_status_container, NULL);
    lv_obj_align(lamp_3_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(lamp_3_status_led, 16, 16);

    lv_obj_t *lamp_4_status_label = lv_label_create(lamp_status_container, NULL);
    lv_label_set_text(lamp_4_status_label, " LAMP 4");

    lamp_4_status_led = lv_led_create(lamp_status_container, NULL);
    lv_obj_align(lamp_4_status_led, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(lamp_4_status_led, 16, 16);
}

static void create_info(lv_obj_t *parent)
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
}

static void create_timer_controls(lv_obj_t *parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_GRID);

    timerList = lv_list_create(parent, NULL);
    lv_cont_set_fit2(timerList, LV_FIT_PARENT, LV_FIT_TIGHT);
    lv_obj_align(timerList, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *addTimer = lv_list_add_btn(timerList, LV_SYMBOL_PLUS, "ADD TIMER");
    lv_obj_set_event_cb(addTimer, timer_item_add_event_cb);

    if (currentTimerItemCount > 0)
    {
        for (int i = 0; i < currentTimerItemCount; i++)
        {
            char labelText[50];
            sprintf(labelText, "%s %s for %dmin %s",
                    timerItems[i]->getWeekdayAsString().c_str(),
                    timerItems[i]->getStartTimeAsString().c_str(),
                    timerItems[i]->getDuration(),
                    timerItems[i]->getRepeatAsString().c_str());

            timerButtons[i] = lv_list_add_btn(timerList, LV_SYMBOL_SETTINGS, labelText);
            lv_obj_set_event_cb(timerButtons[i], timer_btn_event_cb);
        }
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

    if (lamps[0]->getState())
    {
        lv_led_on(lamp_1_status_led);
    }
    else
    {
        lv_led_off(lamp_1_status_led);
    }

    if (lamps[1]->getState())
    {
        lv_led_on(lamp_2_status_led);
    }
    else
    {
        lv_led_off(lamp_2_status_led);
    }

    if (lamps[2]->getState())
    {
        lv_led_on(lamp_3_status_led);
    }
    else
    {
        lv_led_off(lamp_3_status_led);
    }

    if (lamps[3]->getState())
    {
        lv_led_on(lamp_4_status_led);
    }
    else
    {
        lv_led_off(lamp_4_status_led);
    }
    
    if (fans[0]->getState())
    {
        lv_led_on(fan_1_status_led);
    }
    else
    {
        lv_led_off(fan_1_status_led);
    }
    
    if (fans[1]->getState())
    {
        lv_led_on(fan_2_status_led);
    }
    else
    {
        lv_led_off(fan_2_status_led);
    }
    
    if (fans[2]->getState())
    {
        lv_led_on(fan_3_status_led);
    }
    else
    {
        lv_led_off(fan_3_status_led);
    }
    
    if (fans[3]->getState())
    {
        lv_led_on(fan_4_status_led);
    }
    else
    {
        lv_led_off(fan_4_status_led);
    }

    char now[25] = "";
    sprintf(now, "%d/%02d/%02d %02d:%02d", rtc.day(), rtc.month(), rtc.day(), rtc.hour(), rtc.minute());
    lv_label_set_text(timeLabel, now);
}

static void fan_speed_slider_event_cb(lv_obj_t *slider, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:

        switch (lv_slider_get_value(slider))
        {
        case FAN_SPEED_OFF:
            updateLampState(LAMPS_OFF);
            updateFanSpeed(FAN_SPEED_OFF);
            break;

        case FAN_SPEED_LOW:
            updateLampState(LAMPS_ON);
            updateFanSpeed(FAN_SPEED_LOW);
            break;

        case FAN_SPEED_MEDIUM:
            updateLampState(LAMPS_ON);
            updateFanSpeed(FAN_SPEED_MEDIUM);
            break;

        case FAN_SPEED_HIGH:
            updateLampState(LAMPS_ON);
            updateFanSpeed(FAN_SPEED_HIGH);
            break;
        }
        break;

    default:
        break;
    }
}

static void timer_item_delete_event_cb(lv_obj_t *delete_btn, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        lv_obj_t *win = lv_win_get_from_btn(delete_btn);
        String winTitle = String(lv_win_get_title(win));
        winTitle.replace("EDIT TIMER ", "");
        int timerItemIdx = winTitle.toInt();

        removeTimerItemByIndex(timerItemIdx, win);
    }
}

static void timer_item_add_event_cb(lv_obj_t *add_btn, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        createTimerItemDetailsWindow(false, currentTimerItemCount);
    }
}

static void timer_btn_event_cb(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        createTimerItemDetailsWindow(true, lv_list_get_btn_index(timerList, btn) - 1);
    }
}

void createTimerItemDetailsWindow(bool update, int timerItemIndex)
{
    char winTitle[20];

    if (update)
    {
        sprintf(winTitle, "EDIT TIMER %d", timerItemIndex);
    }
    else
    {
        sprintf(winTitle, "ADD TIMER %d", timerItemIndex + 1);
    }

    /** Window **/
    lv_obj_t *win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, winTitle);
    lv_win_set_layout(win, LV_LAYOUT_PRETTY_TOP);

    /* Titlebar buttons */
    lv_obj_t *close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);

    if (update)
    {
        lv_obj_t *delete_btn = lv_win_add_btn(win, LV_SYMBOL_TRASH);
        lv_obj_set_event_cb(delete_btn, timer_item_delete_event_cb);
    }

    lv_obj_t *save_btn = lv_win_add_btn(win, LV_SYMBOL_SAVE);
    lv_obj_set_event_cb(save_btn, NULL);

    /** Content **/

    /* Repetition area */
    lv_obj_t *repetitionContainer = lv_cont_create(win, NULL);
    lv_cont_set_layout(repetitionContainer, LV_LAYOUT_PRETTY_MID);
    lv_cont_set_fit2(repetitionContainer, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(repetitionContainer, 140);

    lv_obj_t *repetitionLabel = lv_label_create(repetitionContainer, NULL);
    lv_label_set_text(repetitionLabel, "Repetition");
    lv_label_set_align(repetitionLabel, LV_LABEL_ALIGN_LEFT);

    lv_obj_t *repeatDropDown = lv_dropdown_create(repetitionContainer, NULL);
    lv_dropdown_set_options(repeatDropDown, "none\nhourly\ndaily\nweekly");

    if (update)
    {
        lv_dropdown_set_selected(repeatDropDown, timerItems[timerItemIndex]->getRepeat());
    }

    /* Weekday area */
    lv_obj_t *weekdayContainer = lv_cont_create(win, NULL);
    lv_cont_set_layout(weekdayContainer, LV_LAYOUT_PRETTY_MID);
    lv_cont_set_fit2(weekdayContainer, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(weekdayContainer, 140);

    lv_obj_t *weekdayLabel = lv_label_create(weekdayContainer, NULL);
    lv_label_set_text(weekdayLabel, "Weekday");
    lv_label_set_align(weekdayLabel, LV_LABEL_ALIGN_LEFT);

    lv_obj_t *weekdayDropDown = lv_dropdown_create(weekdayContainer, NULL);
    lv_dropdown_set_options(weekdayDropDown, "Monday\nTuesday\nWednesday\nThurday\nFriday\nSaturday\nSunday");

    if (update)
    {
        lv_dropdown_set_selected(weekdayDropDown, timerItems[timerItemIndex]->getWeekday());
    }

    /* Start time area */
    lv_obj_t *startTimeContainer = lv_cont_create(win, NULL);
    lv_cont_set_layout(startTimeContainer, LV_LAYOUT_PRETTY_MID);
    lv_cont_set_fit2(startTimeContainer, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(startTimeContainer, 140);

    lv_obj_t *startTimeLabel = lv_label_create(startTimeContainer, NULL);
    lv_label_set_text(startTimeLabel, "      Start      ");
    lv_label_set_align(startTimeLabel, LV_LABEL_ALIGN_LEFT);

    lv_obj_t *startHoursRoller = lv_roller_create(startTimeContainer, NULL);
    lv_roller_set_options(startHoursRoller, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(startHoursRoller, 2);
    lv_obj_align(startHoursRoller, NULL, LV_ALIGN_CENTER, 0, 0);

    if (update)
    {
        lv_roller_set_selected(startHoursRoller, timerItems[timerItemIndex]->getHour(), LV_ANIM_ON);
    }

    lv_obj_t *startMinutesRoller = lv_roller_create(startTimeContainer, NULL);
    lv_roller_set_options(startMinutesRoller, "00\n15\n30\n45", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(startMinutesRoller, 2);
    lv_obj_align(startMinutesRoller, NULL, LV_ALIGN_CENTER, 0, 0);

    if (update)
    {
        lv_roller_set_selected(startMinutesRoller, timerItems[timerItemIndex]->getMinute(), LV_ANIM_ON);
    }

    /* Duration area */
    lv_obj_t *durationContainer = lv_cont_create(win, NULL);
    lv_cont_set_layout(durationContainer, LV_LAYOUT_PRETTY_MID);
    lv_cont_set_fit2(durationContainer, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(durationContainer, 140);

    lv_obj_t *durationLabel = lv_label_create(durationContainer, NULL);
    lv_label_set_text(durationLabel, "    Duration    ");
    lv_label_set_align(durationLabel, LV_LABEL_ALIGN_LEFT);

    lv_obj_t *durationHoursRoller = lv_roller_create(durationContainer, NULL);
    lv_roller_set_options(durationHoursRoller, "00\n01\n02\n03\n04\n05\n06\n07\n08", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(durationHoursRoller, 2);
    lv_obj_align(durationHoursRoller, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *durationMinutesRoller = lv_roller_create(durationContainer, NULL);
    lv_roller_set_options(durationMinutesRoller, "00\n15\n30\n45", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(durationMinutesRoller, 2);
    lv_obj_align(durationMinutesRoller, NULL, LV_ALIGN_CENTER, 0, 0);

    /* Speed area */
    lv_obj_t *speedContainer = lv_cont_create(win, NULL);
    lv_cont_set_layout(speedContainer, LV_LAYOUT_PRETTY_MID);
    lv_cont_set_fit2(speedContainer, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(speedContainer, 290);

    lv_obj_t *fanSpeedLabel = lv_label_create(speedContainer, NULL);
    lv_label_set_text(fanSpeedLabel, "AIR PURIFICATION");
    lv_label_set_align(fanSpeedLabel, LV_LABEL_ALIGN_LEFT);

    lv_obj_t *fanSpeedDropDown = lv_dropdown_create(speedContainer, NULL);
    lv_dropdown_set_options(fanSpeedDropDown, "LOW\nMEDIUM\nHIGH");

    if (update)
    {
        lv_dropdown_set_selected(fanSpeedDropDown, timerItems[timerItemIndex]->getSpeed() - 1);
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

void removeTimerItemByIndex(int timerItemindex, lv_obj_t *win)
{
    for (int i = 0; i < currentTimerItemCount; i++)
    {
        if (i >= timerItemindex)
        {
            timerItems[i] = timerItems[i + 1];
        }
    }

    lv_list_remove(timerList, timerItemindex);

    currentTimerItemCount--;

    lv_obj_del(win);
}
