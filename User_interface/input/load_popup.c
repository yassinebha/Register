#include  <user_interface.h>

private  DEFINE_WIDGET_CALLBACK( cancel_button_callback )  /* ARGSUSED */
{
    cancel_loading( (load_struct *) callback_data );
}

private  DEFINE_EVENT_FUNCTION( quit_window_callback )    /* ARGSUSED */
{
    cancel_loading( (load_struct *) callback_data );
}

public  void  initialize_load_popup(
    load_struct   *load_data,
    int           x_position,
    int           y_position,
    char          filename[] )
{
    widget_struct  *widget;

    create_popup_window( &load_data->popup, filename,
                         x_position, y_position,
                         Load_popup_x_size, Load_popup_y_size,
                         quit_window_callback, (void *) load_data );

    initialize_meter( &load_data->popup,
                      Load_meter_x_size, Load_meter_y_size,
                      (Colour) Load_meter_background_colour,
                      (Colour) Load_meter_colour,
                      &load_data->meter_background,
                      &load_data->meter );

    widget = create_button( &load_data->popup.graphics,
                   0, Interface_x_spacing, Interface_y_spacing,
                   Button_width, Button_height,
                   "Cancel",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   cancel_button_callback, (void *) load_data );

    (void) add_widget_to_list( &load_data->popup.widgets, widget );
}

public  void  delete_load_popup(
    load_struct   *load_data )
{
    delete_popup_window( &load_data->popup );
}

public  void  set_load_popup_meter(
    load_struct   *load_data,
    Real          fraction_done )
{
    set_meter_position( &load_data->popup, load_data->meter_background,
                        load_data->meter, fraction_done );
}
