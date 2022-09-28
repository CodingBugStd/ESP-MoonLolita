#include "home_page.h"

static lv_obj_t * screen = NULL;
static lv_obj_t * label = NULL;

static void home_page_obj_create(){
    screen = lv_obj_create(NULL);
    label = lv_label_create( screen );
    lv_label_set_text( label , "HelloWorld!");
}

static void home_page_obj_delete(){
    lv_obj_del( screen );
    screen = NULL;
    label = NULL;
}

void home_page_load(){
    home_page_obj_create();
    lv_scr_load( screen );
}

