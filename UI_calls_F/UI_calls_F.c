/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/UI_calls_F/UI_calls_F.c,v 1.21 2004-10-25 19:11:08 bert Exp $";
#endif

#include  <UI_calls_F.h>

public  void  IF_initialize_register(
    Gwindow  window,
    VIO_STR   executable_name )
{
    (void) initialize_register( window, executable_name );
}

public  VIO_Status   UI_set_global_variable(
    VIO_STR  variable_name,
    VIO_STR  value_to_set )
{
    return( set_functional_global_variable( variable_name, value_to_set ) );
}

public  void  IF_terminate_register( void )
{
    terminate_register();
}

public  VIO_Status  IF_start_loading_volume(
    int     volume_index,
    VIO_STR  filename )
{
    return( start_loading_volume( get_main_struct(), volume_index, filename ) );
}

public  VIO_BOOL  IF_load_more_of_volume(
    int     volume_index,
    VIO_Real    max_time,
    VIO_Real    *fraction_done )
{
    return( load_more_of_volume( get_main_struct(), volume_index, max_time,
                                 fraction_done ) );
}

public  void  IF_cancel_loading_volume(
    int     volume_index )
{
    cancel_loading_volume( get_main_struct(), volume_index );
}

public  VIO_BOOL  IF_volume_is_loaded(
    int            volume_index )
{
    return( is_volume_active( get_main_struct(), volume_index ) );
}

public  VIO_BOOL  IF_is_an_rgb_volume(
    int            volume_index )
{
    return( is_volume_rgb( get_main_struct(), volume_index ) );
}

public  VIO_BOOL  IF_is_resampled_volume_loaded( void )
{
    return( is_resampled_volume_loaded( get_main_struct() ) );
}

public  VIO_Status  IF_do_resampling(
    VIO_STR   resampled_filename )
{
    return( resample_the_volume( get_main_struct(), resampled_filename ) );
}

public  VIO_STR  IF_get_volume_filename(
    int  volume_index )
{
    return( get_volume_filename( get_main_struct(), volume_index ) );
}

public  void  IF_save_image(
    int   volume,
    int   view )
{
    save_image( get_main_struct(), volume, view );
}

public  VIO_BOOL  IF_get_merged_slice_visibility( void )
{
    return( get_merged_volume_activity(get_main_struct()) );
}

public  void  IF_set_merged_slice_visibility(
    VIO_BOOL  visible )
{
    set_merged_volume_activity( get_main_struct(), visible );
}

public  void  IF_set_volume(
    int     volume_index,
    VIO_STR  filename )
{
    set_register_volume( get_main_struct(), volume_index, filename );
}

public  void  IF_set_resampled_volume(
    int                    volume_index,
    VIO_STR                 filename,
    VIO_STR                 original_filename,
    VIO_General_transform      *resampling_transform )
{
    set_register_resampled_volume( get_main_struct(), volume_index, filename,
                                   original_filename, resampling_transform );
}

public  VIO_Real  IF_get_voxel_value(
    int     volume_index,
    VIO_Real    x_voxel,
    VIO_Real    y_voxel,
    VIO_Real    z_voxel )
{
    return( get_voxel_value( get_main_struct(), volume_index,
            x_voxel, y_voxel, z_voxel ) );
}

public  void  IF_get_volume_value_range(
    int     volume_index,
    VIO_Real    *min_value,
    VIO_Real    *max_value )
{
    get_volume_value_range( get_main_struct(), volume_index,
                            min_value, max_value );
}

public  void  IF_delete_volume(
    int            volume_index )
{
    delete_register_volume( get_main_struct(), volume_index );
}

public  VIO_BOOL  IF_get_interpolation_flag( void )
{
    return( get_interpolation_mode( get_main_struct() ) );
}

public  void  IF_set_interpolation_flag( VIO_BOOL  flag )
{
    set_interpolation_mode( get_main_struct(), flag );
}

public  void  IF_set_recreate_slice_flag(
    int    volume,
    int    view )
{
    set_recreate_slice_flag( get_main_struct(), volume, view );
}

public  void  IF_reset_slice_view(
    int             volume,
    int             view )
{
    initialize_slice_view( get_main_struct(), volume, view );
}

public  void  IF_set_update_slice_viewport_flag(
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_update_slice_viewport_flag( get_main_struct(), volume, view,
                                    bitplane );
}

public  VIO_BOOL  IF_slices_to_be_updated(
    int   current_buffer )
{
    return( slices_to_be_updated( get_main_struct(), current_buffer ) );
}

public  VIO_BOOL  IF_redraw_slices(
    int             current_buffer )
{
    return( update_slice_display( get_main_struct(), current_buffer ) );
}

public  void  IF_set_volume_voxel_position(
    int       volume,
    VIO_Real      position[] )
{
    set_volume_voxel_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_voxel_position(
    int       volume,
    VIO_Real      position[] )
{
    get_volume_voxel_position( get_main_struct(), volume, position );
}

public  void  IF_set_volume_time_position(
    int       volume_index,
    VIO_Real      tpos )
{
    set_volume_time_position( get_main_struct(), volume_index, tpos );
}

public  void  IF_get_volume_time_position(
    int       volume_index,
    VIO_Real      *tpos_ptr )
{
    get_volume_time_position( get_main_struct(), volume_index, tpos_ptr );
}

public  void  IF_set_volume_world_position(
    int       volume,
    VIO_Real      position[] )
{
    set_volume_world_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_world_position(
    int       volume,
    VIO_Real      position[] )
{
    get_volume_world_position( get_main_struct(), volume, position );
}

public  void  IF_set_volume_original_world_position(
    int       volume,
    VIO_Real      position[] )
{
    set_volume_original_world_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_original_world_position(
    int       volume,
    VIO_Real      position[] )
{
    get_volume_original_world_position( get_main_struct(), volume, position );
}

public  int   IF_get_slice_axis(
    int   view )
{
    return( get_slice_axis( view ) );
}

public  void  IF_translate_slice(
    int       volume,
    int       view,
    int       x_translation,
    int       y_translation )
{
    translate_slice( get_main_struct(), volume, view,
                     x_translation, y_translation );
}

public  void  IF_scale_slice(
    int       volume,
    int       view,
    VIO_Real      scale_factor )
{
    scale_slice( get_main_struct(), volume, view, scale_factor );
}

public  void  IF_set_slice_viewport(
    int       volume,
    int       view,
    int       x_min,
    int       x_max,
    int       y_min,
    int       y_max )
{
    set_slice_viewport( get_main_struct(), volume, view,
                        x_min, x_max, y_min, y_max );
}

public  VIO_BOOL  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    VIO_Real   voxel_position[] )
{
    return( convert_pixel_to_voxel( get_main_struct(), volume, view,
                                    x_pixel, y_pixel, voxel_position ) );
}

public  VIO_BOOL  IF_can_switch_colour_modes( void )
{
    return( can_switch_colour_modes( get_main_struct() ) );
}

public  void  IF_colour_mode_has_toggled(
    int  start_index )
{
    colour_mode_has_toggled( get_main_struct(), start_index );
}

public  void  IF_set_under_colour(
    int        volume_index,
    VIO_Colour     colour )
{
    set_volume_under_colour( get_main_struct(), volume_index, colour );
}

public  void  IF_set_merge_method(
    Merge_methods     method )
{
    set_merged_method( get_main_struct(), method );
}

public  Merge_methods  IF_get_merge_method( void )
{
    return( get_merged_method( get_main_struct() ) );
}

public  void  IF_set_over_colour(
    int        volume_index,
    VIO_Colour     colour )
{
    set_volume_over_colour( get_main_struct(), volume_index, colour );
}

public  void  IF_set_volume_colour_coding_type(
    int                   volume_index,
    Colour_coding_types   type )
{
    set_volume_colour_coding_type( get_main_struct(), volume_index,
                                   type );
}

public  Colour_coding_types  IF_get_colour_coding_type(
    int    volume_index )
{
    return( get_volume_colour_coding_type( get_main_struct(), volume_index ) );
}

public  void  IF_set_colour_coding_limits(
    int    volume_index,
    VIO_Real   min_value,
    VIO_Real   max_value )
{
    set_volume_colour_coding_limits( get_main_struct(), volume_index,
                                     min_value, max_value );
}

public  void  IF_get_colour_coding_limits(
    int    volume_index,
    VIO_Real   *min_value,
    VIO_Real   *max_value )
{
    get_volume_colour_coding_limits( get_main_struct(), volume_index,
                                     min_value, max_value );
}

public  void  IF_set_merged_volume_opacity(
    int    which_volume,
    VIO_Real   opacity )
{
    set_merged_volume_opacity( get_main_struct(), which_volume, opacity );
}

public  VIO_Real  IF_get_merged_volume_opacity(
    int    which_volume )
{
    return( get_merged_volume_opacity( get_main_struct(), which_volume ) );
}

public  void  IF_set_slice_filter_type(
    int           volume_index,
    int           view_index,
    Filter_types  filter_type )
{
    set_slice_filter_type( get_main_struct(), volume_index, view_index,
                           filter_type );
}

public  Filter_types  IF_get_slice_filter_type(
    int           volume_index,
    int           view_index )
{
    return( get_slice_filter_type(get_main_struct(), volume_index, view_index));
}

public  void  IF_set_slice_filter_width(
    int      volume_index,
    int      view_index,
    VIO_Real     filter_width )
{
    set_slice_filter_width( get_main_struct(), volume_index, view_index,
                            filter_width );
}

public  VIO_Real  IF_get_slice_filter_width(
    int           volume_index,
    int           view_index )
{
    return(get_slice_filter_width(get_main_struct(), volume_index, view_index));
}

/* ----------------------- tag points -------------------------------- */

public  VIO_BOOL  IF_get_tags_visibility( void )
{
    return( get_tags_visibility(get_main_struct()) );
}

public  void  IF_set_tags_visibility( VIO_BOOL  visibility )
{
    set_tags_visibility( get_main_struct(), visibility );
}

public  void  IF_create_new_tag_point( void )
{
    create_new_tag_point( get_main_struct() );
}

public  void  IF_delete_tag_point(
    int      ind )
{
    delete_tag_point( get_main_struct(), ind );
}

public  int  IF_get_n_tag_points( void )
{
    return( get_n_tag_points( get_main_struct() ) );
}

public  VIO_BOOL  IF_get_tag_point_position(
    int              ind,
    int              volume_index,
    VIO_Real             position[] )
{
    return( get_tag_point_position( get_main_struct(),
                                    ind, volume_index, position ) );
}

public  void  IF_set_tag_point_position(
    int              ind,
    int              volume_index,
    VIO_Real             position[] )
{
    set_tag_point_position( get_main_struct(),
                            ind, volume_index, position );
}

public  VIO_STR  IF_get_tag_point_name(
    int              ind )
{
    return( get_tag_point_name( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_name(
    int              ind,
    VIO_STR           name )
{
    set_tag_point_name( get_main_struct(), ind, name );
}

public  VIO_BOOL  IF_get_tag_point_rms_error(
    int              ind,
    VIO_Real             *rms_error )
{
    return( get_tag_point_rms_error( get_main_struct(), ind, rms_error ) );
}

public  VIO_BOOL  IF_get_tag_point_avg_rms_error(
    VIO_Real             *avg_rms_error )
{
    return( get_tag_point_avg_rms_error( get_main_struct(), avg_rms_error ) );
}

public  VIO_BOOL  IF_get_tag_point_activity(
    int              ind )
{
    return( get_tag_point_activity( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_activity(
    int              ind,
    VIO_BOOL          activity )
{
    set_tag_point_activity( get_main_struct(), ind, activity );
}

public  void  IF_save_tags_file(
    VIO_STR   filename )
{
    (void) save_tag_points( get_main_struct(), filename );
}

public  void  IF_load_tags_file(
    VIO_STR   filename )
{
    (void) load_tag_points( get_main_struct(), filename );
}

public  void  IF_save_transform(
    VIO_STR   filename )
{
    (void) save_transform( get_main_struct(), filename );
}

public  VIO_BOOL  IF_does_transform_exist( void )
{
    return( get_tag_point_transform( get_main_struct(),
                                     (VIO_General_transform **) NULL ) );
}

public  VIO_BOOL  IF_get_resampling_transform(
    VIO_General_transform  **transform )
{
    return( get_tag_point_transform( get_main_struct(), transform ) );
}

public  VIO_BOOL  IF_tag_points_have_been_saved( void )
{
    return( get_tag_points_saved( get_main_struct() ) );
}

public  Trans_type  IF_get_transform_type( void )
{
    return( get_tag_transform_type( get_main_struct() ) );
}

public  void  IF_set_transform_type(
    Trans_type  type )
{
    set_tag_transform_type( get_main_struct(), type );
}

public  VIO_BOOL  IF_get_cursor_visibility( void )
{
    return( get_cursor_visibility(get_main_struct()) );
}

public  void  IF_set_cursor_visibility( VIO_BOOL  visibility )
{
    set_cursor_visibility( get_main_struct(), visibility );
}

