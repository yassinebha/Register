#include  <def_user_interface.h>

public  void  initialize_event_viewports(
    event_viewports_struct  *event_viewports )
{
    event_viewports->n_event_viewports = 0;
}

public  void  delete_event_viewports(
    event_viewports_struct  *event_viewports )
{
    int          i;

    if( event_viewports->n_event_viewports > 0 )
    {
        for_less( i, 0, event_viewports->n_event_viewports )
        {
            delete_event_table( event_viewports->event_viewports[i].table );
        }

        if( event_viewports->n_event_viewports > 0 )
            FREE( event_viewports->event_viewports );
    }
}

private  void  check_event_viewport_exists(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index )
{
    int      i, prev_size;

    if( event_viewport_index >= event_viewports->n_event_viewports )
    {
        prev_size = event_viewports->n_event_viewports;
        SET_ARRAY_SIZE( event_viewports->event_viewports,
                        event_viewports->n_event_viewports,
                        event_viewport_index+1, DEFAULT_CHUNK_SIZE );
        event_viewports->n_event_viewports = event_viewport_index + 1;
        for_less( i, prev_size, event_viewport_index+1 )
        {
            initialize_event_table( event_viewports->event_viewports[i].table );
        }
    }
}

public  void  set_event_viewport(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    event_viewports->event_viewports[event_viewport_index].x_min = x_min;
    event_viewports->event_viewports[event_viewport_index].x_max = x_max;
    event_viewports->event_viewports[event_viewport_index].y_min = y_min;
    event_viewports->event_viewports[event_viewport_index].y_max = y_max;
}

public  void  get_event_viewport(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    int                     *x_min,
    int                     *x_max,
    int                     *y_min,
    int                     *y_max )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    *x_min = event_viewports->event_viewports[event_viewport_index].x_min;
    *x_max = event_viewports->event_viewports[event_viewport_index].x_max;
    *y_min = event_viewports->event_viewports[event_viewport_index].y_min;
    *y_max = event_viewports->event_viewports[event_viewport_index].y_max;
}

public  void  set_event_viewport_callback_enabled(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    Boolean                 enabled )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    set_event_callback_enabled(
      &event_viewports->event_viewports[event_viewport_index].table[event_type],
      callback_function, callback_data, enabled );
}

public  void  set_event_viewport_callback_viewport(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    set_event_callback_viewport(
      &event_viewports->event_viewports[event_viewport_index].table[event_type],
      callback_function, callback_data,
      x_min, x_max, y_min, y_max );
}

public  void  add_event_viewport_callback(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    Event_types             event_type,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max,
    event_function_type     callback_function,
    void                    *callback_data )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    add_event_callback_function(
      &event_viewports->event_viewports[event_viewport_index].table[event_type],
      x_min, x_max, y_min, y_max, callback_function, callback_data );
}

public  void  remove_event_viewport_callback(
    event_viewports_struct  *event_viewports,
    int                     event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data )
{
    check_event_viewport_exists( event_viewports, event_viewport_index );

    remove_event_callback_function(
      &event_viewports->event_viewports[event_viewport_index].table[event_type],
      callback_function, callback_data );
}

public  void  execute_event_viewport_events(
    event_viewports_struct  *event_viewports,
    int                     x_mouse,
    int                     y_mouse,
    Event_types             event_type,
    int                     key_pressed )
{
    int      i, x_mouse_viewport, y_mouse_viewport;
    Boolean  found;

    found = FALSE;

    for_less( i, 0, event_viewports->n_event_viewports )
    {
        if( event_viewports->event_viewports[i].x_min < 0 ||
            x_mouse >= event_viewports->event_viewports[i].x_min &&
            x_mouse <= event_viewports->event_viewports[i].x_max &&
            y_mouse >= event_viewports->event_viewports[i].y_min &&
            y_mouse <= event_viewports->event_viewports[i].y_max )
        {
            x_mouse_viewport = x_mouse -
                               event_viewports->event_viewports[i].x_min;
            y_mouse_viewport = y_mouse -
                               event_viewports->event_viewports[i].y_min;
            found = execute_event_callback_functions(
                     &event_viewports->event_viewports[i].table[event_type],
                     x_mouse_viewport, y_mouse_viewport, i, key_pressed );

            if( found )
                break;
        }
    }
}
