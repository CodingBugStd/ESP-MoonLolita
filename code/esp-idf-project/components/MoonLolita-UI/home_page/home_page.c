#include "home_page.h"

#include "esp_log.h"

#define TAG "home_page"

static void lable_create( lv_obj_t *parent ){
    lv_label_t *label = NULL;

    label = lv_label_create( parent );
    lv_label_set_text(label,"HelloWorld!\nIMG Test");
}

static void button_callback( lv_event_t * event ){
    // printf("HelloWorld!\n");
    ESP_LOGI(TAG,"button press");
}

static void button_create( lv_obj_t * parent){
    lv_btn_t * btn = lv_btn_create( parent );
    lv_label_t * btn_label = lv_label_create( btn );

    lv_obj_set_align( btn , LV_ALIGN_CENTER );
    lv_obj_set_size( btn , 80 , 40 );

    lv_label_set_text( btn_label , "test" );
    lv_obj_set_align( btn_label , LV_ALIGN_CENTER );

    lv_obj_set_style_bg_color( btn , lv_palette_main(LV_PALETTE_RED) , 0 );
    lv_obj_set_style_bg_opa( btn , LV_OPA_60 , 0 );

    lv_obj_add_event_cb( btn , button_callback , LV_EVENT_CLICKED , NULL );

    //触摸测试
    btn = lv_btn_create( parent );
    btn_label = lv_label_create( btn );

    lv_obj_set_pos( btn , 0 , 0 );
    lv_obj_set_size( btn , 80 , 40 );

    lv_label_set_text( btn_label , "test" );
    lv_obj_set_align( btn_label , LV_ALIGN_CENTER );

    lv_obj_set_style_bg_color( btn , lv_palette_main(LV_PALETTE_RED) , 0 );
    lv_obj_set_style_bg_opa( btn , LV_OPA_60 , 0 );

    lv_obj_add_event_cb( btn , button_callback , LV_EVENT_CLICKED , NULL );

    //触摸测试
    btn = lv_btn_create( parent );
    btn_label = lv_label_create( btn );

    lv_obj_set_pos( btn , 240 , 0 );
    lv_obj_set_size( btn , 80 , 40 );

    lv_label_set_text( btn_label , "test" );
    lv_obj_set_align( btn_label , LV_ALIGN_CENTER );

    lv_obj_set_style_bg_color( btn , lv_palette_main(LV_PALETTE_RED) , 0 );
    lv_obj_set_style_bg_opa( btn , LV_OPA_60 , 0 );

    lv_obj_add_event_cb( btn , button_callback , LV_EVENT_CLICKED , NULL );

    //触摸测试
    btn = lv_btn_create( parent );
    btn_label = lv_label_create( btn );

    lv_obj_set_pos( btn , 0 , 200 );
    lv_obj_set_size( btn , 80 , 40 );

    lv_label_set_text( btn_label , "test" );
    lv_obj_set_align( btn_label , LV_ALIGN_CENTER );

    lv_obj_set_style_bg_color( btn , lv_palette_main(LV_PALETTE_RED) , 0 );
    lv_obj_set_style_bg_opa( btn , LV_OPA_60 , 0 );

    lv_obj_add_event_cb( btn , button_callback , LV_EVENT_CLICKED , NULL );

    //触摸测试
    btn = lv_btn_create( parent );
    btn_label = lv_label_create( btn );

    lv_obj_set_pos( btn , 240 , 200 );
    lv_obj_set_size( btn , 80 , 40 );

    lv_label_set_text( btn_label , "test" );
    lv_obj_set_align( btn_label , LV_ALIGN_CENTER );

    lv_obj_set_style_bg_color( btn , lv_palette_main(LV_PALETTE_RED) , 0 );
    lv_obj_set_style_bg_opa( btn , LV_OPA_60 , 0 );

    lv_obj_add_event_cb( btn , button_callback , LV_EVENT_CLICKED , NULL );
}

static void home_page_create(){
    lv_img_t *screen = NULL;

    screen = lv_img_create( NULL );

    //lv_img_set_src( screen , "A:sdcard/img/test.png" );

    lable_create( screen );
    button_create( screen );
    lv_scr_load( screen );
}

void home_page_load(){
    home_page_create();
}

