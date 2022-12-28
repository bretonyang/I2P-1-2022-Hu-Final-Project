#define _CRT_SECURE_NO_WARNINGS

#include "scene_menu_object.h"
#include "utility.h"


/* Button utility functions */

Button* button_create(float x, float y, float w, float h,
                      const char* default_image_path, const char* hovered_image_path) {

    Button* button = (Button*)malloc(sizeof(Button));
    memset(button, 0, sizeof(Button));

    if (hovered_image_path) {
        button->hovered_img = load_bitmap(hovered_image_path);
    }

    button->default_img = load_bitmap(default_image_path);
    if (!button->default_img) {
        game_abort("failed loading button image!");
    }

    button->body.x = x;
    button->body.y = y;
    button->body.w = w;
    button->body.h = h;

    return button;
}

/**
 * If the button is hovered, then draw the hovered_img (if it doesn't exist, then use the default_img),
 * else draw the default_img.
 */
void button_draw(Button* button) {
    ALLEGRO_BITMAP* _img = button->hovered_img ?
                           (button->hovered ? button->hovered_img : button->default_img) :
                           button->default_img;

    al_draw_scaled_bitmap(
        _img,
        0, 0,
        al_get_bitmap_width(_img), al_get_bitmap_height(_img),
        button->body.x, button->body.y,
        button->body.w, button->body.h, 0
    );
}

void button_destroy(Button* button) {
    // Destroy images if they were loaded
    if (button->hovered_img)
        al_destroy_bitmap(button->hovered_img);

    al_destroy_bitmap(button->default_img);

    free(button);
}

/**
 * Returns true if the button is hovered by the mouse, false otherwise.
 */
//bool button_hovered(Button* button, int mouse_x, int mouse_y) {
//    //	[HACKATHON 3-6]
//    //	TODO: Check if mouse is hovering on the button
//    //	Uncomment and fill the code below
//    return pnt_in_rect(mouse_x, mouse_y, button->body);
//}

/**
 * Updates the button's hovered state.
 */
void button_update_hover_state(Button* button, int mouse_x, int mouse_y) {
    button->hovered = pnt_in_rect(mouse_x, mouse_y, button->body);
}


/* Slider utility functions */

Slider* slider_create(float track_x, float track_y, float track_w, float track_h,
                      float handle_x, float handle_y, float handle_w, float handle_h,
                      int diff, const char* track_img_path, const char* handle_img_path) {

    Slider* slider = (Slider*)malloc(sizeof(Slider));
    memset(slider, 0, sizeof(Slider));

    // Load images
    slider->track_img = load_bitmap(track_img_path);
    slider->handle_img = load_bitmap(handle_img_path);

    // Set up track RecArea
    slider->track_body.x = track_x;
    slider->track_body.y = track_y;
    slider->track_body.w = track_w;
    slider->track_body.h = track_h;

    // Set up handle RecArea
    slider->handle_body.x = handle_x;
    slider->handle_body.y = handle_y;
    slider->handle_body.w = handle_w;
    slider->handle_body.h = handle_h;

    // Set up the length of each section of the slider
    slider->diff = diff;

    return slider;
}

void slider_draw(Slider* slider) {
    // Draw track
    al_draw_scaled_bitmap(
        slider->track_img,
        0, 0,
        al_get_bitmap_width(slider->track_img), al_get_bitmap_height(slider->track_img),
        slider->track_body.x, slider->track_body.y,
        slider->track_body.w, slider->track_body.h, 0
    );

    // Draw handle
    al_draw_scaled_bitmap(
        slider->handle_img,
        0, 0,
        al_get_bitmap_width(slider->handle_img), al_get_bitmap_height(slider->handle_img),
        slider->handle_body.x, slider->handle_body.y,
        slider->handle_body.w, slider->handle_body.h, 0
    );
}

void slider_destroy(Slider* slider) {
    al_destroy_bitmap(slider->handle_img);
    al_destroy_bitmap(slider->track_img);
    free(slider);
}

void slider_update_hover_state(Slider* slider, int mouse_x, int mouse_y) {
    slider->handle_hovered = pnt_in_rect(mouse_x, mouse_y, slider->handle_body);
}

void slider_update_value(Slider* slider, int mouse_x, float* value) {
    // mouse_x is in slider track range
    if (mouse_x >= slider->track_body.x &&
            mouse_x <= slider->track_body.x + slider->track_body.w) {
        // record handle new position
        slider->handle_body.x = mouse_x - (slider->handle_body.w / 2);

        // record slider value
        *value = ((float)mouse_x - slider->track_body.x) / slider->track_body.w;
    }
    // mouse_x is larger than track's end
    else if (mouse_x > slider->track_body.x + slider->track_body.w) {
        slider->handle_body.x = slider->track_body.x + slider->track_body.w - (slider->handle_body.w / 2);
        *value = 1.0;
    }
    // mouse_x is smaller than track's head
    else if (mouse_x < slider->track_body.x) {
        slider->handle_body.x = slider->track_body.x - (slider->handle_body.w / 2);
        *value = 0.0;
    }
}
















