#include  <register.h>

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_size, y_size, axis;
    Pixel_types      pixel_type;
    Filter_types     filter_type;
    Real             filter_width;
    Real             x_translation, y_translation, x_scale, y_scale;
    Volume           volume;
    Real             *position;
    unsigned short   *cmode_colour_map;
    Colour           *rgb_colour_map;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    axis = get_slice_axis( view );
    get_slice_transform( main, volume_index, view,
                         &x_translation, &y_translation, &x_scale, &y_scale );
    filter_type = get_slice_filter_type( main, volume_index, view );
    filter_width = get_slice_filter_width( main, volume_index, view );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, volume_index, view, &x_size, &y_size );

    cmode_colour_map = main->trislice[volume_index].cmode_colour_map;
    rgb_colour_map = main->trislice[volume_index].rgb_colour_map;

    create_volume_slice( filter_type, filter_width, volume, position[axis],
                    x_translation, y_translation, x_scale, y_scale,
                    (Volume) NULL, 0.0, 0.0, 0.0, 0.0, 0.0,
                    x_axis_index, y_axis_index, axis,
                    x_size, y_size, pixel_type,
                    main->interpolation_flag,
                    &cmode_colour_map,
                    &rgb_colour_map,
                    &main->trislice[volume_index].slices[view].n_pixels_alloced,
                    main->trislice[volume_index].slices[view].pixels );
}

public  void  create_merged_pixels(
    main_struct   *main,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_size, y_size, axis;
    int              sizes1[N_DIMENSIONS], sizes2[N_DIMENSIONS];
#ifdef OLD
    Real             separations2[N_DIMENSIONS];
#endif
    Pixel_types      pixel_type;
    Real             x_scale1, y_scale1, x_scale2, y_scale2;
    Real             x_translation1, y_translation1;
    Real             x_translation2, y_translation2;
    Volume           volume1, volume2;
    Real             tmp[N_DIMENSIONS];
    Real             *position1, position2[N_DIMENSIONS];
    Real             x_pixel_min, x_pixel_max, y_pixel_min, y_pixel_max;

    volume1 = get_slice_volume( main, 0 );
    volume2 = get_slice_volume( main, 1 );
    position1 = get_volume_cursor( main, MERGED_VOLUME_INDEX );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    axis = get_slice_axis( view );
    get_slice_transform( main, MERGED_VOLUME_INDEX, view,
                         &x_translation1, &y_translation1,
                         &x_scale1, &y_scale1 );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, MERGED_VOLUME_INDEX, view, &x_size, &y_size);

    /* convert centre of volume 1 slice to volume 2 position, to
       get the volume 2 slice */

    get_volume_sizes( volume1, sizes1 );
    get_volume_sizes( volume2, sizes2 );

    tmp[x_axis_index] = (Real) (sizes1[x_axis_index]-1) / 2.0;
    tmp[y_axis_index] = (Real) (sizes1[y_axis_index]-1) / 2.0;
    tmp[axis] = position1[axis];

    convert_voxel_to_world( volume1, tmp, &tmp[X], &tmp[Y], &tmp[Z] );

    convert_world_to_original_world( main, RESAMPLED_VOLUME_INDEX,
                                     tmp[X], tmp[Y], tmp[Z],
                                     &position2[X], &position2[Y],
                                     &position2[Z] );

    convert_original_world_to_voxel( main, RESAMPLED_VOLUME_INDEX,
                                     position2[X], position2[Y], position2[Z],
                                     &position2[X], &position2[Y],
                                     &position2[Z] );

    if( position2[axis] < -0.5 ||
        position2[axis] > (Real) sizes2[axis] - 0.5 )
    {
        modify_pixels_size( &main->merged.slices[view].n_pixels_alloced,
                            main->merged.slices[view].pixels,
                            0, 0, pixel_type );
    }
    else
    {
        /* determine which pixel that (0,0) voxel maps to */

        tmp[x_axis_index] = 0.0;
        tmp[y_axis_index] = 0.0;
        tmp[axis] = position2[axis];

        convert_voxel_to_original_world( main, RESAMPLED_VOLUME_INDEX,
                                         tmp[X], tmp[Y], tmp[Z],
                                         &tmp[X], &tmp[Y], &tmp[Z] );
        convert_original_world_to_world( main, RESAMPLED_VOLUME_INDEX,
                                         tmp[X], tmp[Y], tmp[Z],
                                         &tmp[X], &tmp[Y], &tmp[Z] );
        convert_world_to_voxel( volume1, tmp[X], tmp[Y], tmp[Z], tmp );
        convert_voxel_to_pixel( main, MERGED_VOLUME_INDEX, view,
                                tmp, &x_pixel_min, &y_pixel_min );
        
#ifdef OLD
        /* determine which pixel that (x_size,y_size) voxel maps to */

        tmp[x_axis_index] = (Real) sizes2[x_axis_index];
        tmp[y_axis_index] = (Real) sizes2[y_axis_index];
        tmp[axis] = position2[axis];

        convert_voxel_to_original_world( main, RESAMPLED_VOLUME_INDEX,
                                         tmp[X], tmp[Y], tmp[Z],
                                         &tmp[X], &tmp[Y], &tmp[Z] );
        convert_original_world_to_world( main, RESAMPLED_VOLUME_INDEX,
                                         tmp[X], tmp[Y], tmp[Z],
                                         &tmp[X], &tmp[Y], &tmp[Z] );
        convert_world_to_voxel( volume1, tmp[X], tmp[Y], tmp[Z], tmp );
        convert_voxel_to_pixel( main, MERGED_VOLUME_INDEX, view,
                                tmp, &x_pixel_max, &y_pixel_max );

        get_volume_separations( volume2, separations2 );
        x_translation2 = x_pixel_min;
        y_translation2 = y_pixel_min;
        x_scale2 = (x_pixel_max - x_pixel_min) / (Real) sizes2[x_axis_index] /
                   separations2[x_axis_index];
        y_scale2 = (y_pixel_max - y_pixel_min) / (Real) sizes2[y_axis_index] /
                   separations2[y_axis_index];
#else
        x_translation2 = x_pixel_min;
        y_translation2 = y_pixel_min;
        x_scale2 = x_scale1;
        y_scale2 = y_scale1;
#endif

        create_volume_slice( NEAREST_NEIGHBOUR, 0.0,
                    volume1, position1[axis],
                    x_translation1, y_translation1, x_scale1, y_scale1,
                    volume2, position2[axis],
                    x_translation2, y_translation2, x_scale2, y_scale2,
                    x_axis_index, y_axis_index, axis,
                    x_size, y_size, pixel_type,
                    main->interpolation_flag,
                    main->merged.cmode_colour_map,
                    main->merged.rgb_colour_map,
                    &main->merged.slices[view].n_pixels_alloced,
                    main->merged.slices[view].pixels );
    }
}

public  BOOLEAN   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] )
{
    BOOLEAN        in_volume;
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    Volume         volume;
    Real           *position;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    in_volume = convert_slice_pixel_to_voxel(
              volume, x_pixel, y_pixel, position,
              x_axis_index, y_axis_index,
              x_translation, y_translation, x_scale, y_scale,
              voxel_position );

    return( in_volume );
}

public  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    Real            voxel_position[N_DIMENSIONS],
    Real            *x_pixel,
    Real            *y_pixel )
{
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    Volume         volume;

    volume = get_slice_volume( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    convert_voxel_to_slice_pixel( volume, voxel_position,
                                  x_axis_index, y_axis_index,
                                  x_translation, y_translation,
                                  x_scale, y_scale, x_pixel, y_pixel );
}

private  void  record_slice_display_limits(
    main_struct  *main,
    int          volume,
    int          view )
{
    int            axis, x_viewport_size, y_viewport_size, axis_index[2];
    slice_struct   *slice;
    Real           voxel_position[N_DIMENSIONS];

    slice = get_slice_struct( main, volume, view );
    get_slice_viewport_size( main, volume, view,
                             &x_viewport_size, &y_viewport_size );

    get_slice_axes( view, &axis_index[0], &axis_index[1] );

    for_less( axis, 0, 2 )
    {
        (void) convert_pixel_to_voxel( main, volume, view, 0, 0,
                                       voxel_position );
        slice->lower_display_limits[axis] = voxel_position[axis_index[axis]];

        (void) convert_pixel_to_voxel( main, volume, view,
                                       x_viewport_size-1, y_viewport_size-1,
                                       voxel_position );
        slice->upper_display_limits[axis] = voxel_position[axis_index[axis]];
    }
}

public  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation_offset,
    int          y_translation_offset )
{
    Real           x_translation, y_translation, x_scale, y_scale;

    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    x_translation += (Real) x_translation_offset;
    y_translation += (Real) y_translation_offset;

    set_slice_translation( main, volume, view, x_translation, y_translation );
    record_slice_display_limits( main, volume, view );

    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  scale_slice(
    main_struct  *main,
    int          volume,
    int          view,
    Real         scale_factor )
{
    Real  x_translation, y_translation, x_scale, y_scale;
    int   x_size, y_size;

    get_slice_viewport_size( main, volume, view, &x_size, &y_size );
    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    scale_slice_about_viewport_centre( scale_factor, x_size, y_size,
                                       &x_scale, &y_scale,
                                       &x_translation, &y_translation );

    set_slice_translation( main, volume, view, x_translation, y_translation );
    set_slice_scale( main, volume, view, x_scale, y_scale );
    record_slice_display_limits( main, volume, view );

    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  fit_slice_to_view(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            x_viewport_size, y_viewport_size;
    int            x_axis_index, y_axis_index;
    Real           x_scale, y_scale, x_trans, y_trans;
    Volume         volume;
    slice_struct   *slice;

    volume = get_slice_volume( main, volume_index );
    slice = get_slice_struct( main, volume_index, view );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    get_slice_viewport_size( main, volume_index, view, &x_viewport_size,
                             &y_viewport_size );

    fit_slice_voxel_range_to_viewport( volume, x_axis_index, y_axis_index,
                                       slice->lower_display_limits[0],
                                       slice->upper_display_limits[0],
                                       slice->lower_display_limits[1],
                                       slice->upper_display_limits[1],
                                       x_viewport_size, y_viewport_size,
                                       &x_scale, &y_scale, &x_trans, &y_trans );
                       
    set_slice_translation( main, volume_index, view, x_trans, y_trans );
    set_slice_scale( main, volume_index, view, x_scale, y_scale );

    update_volume_cursor( main, volume_index, view );
    set_recreate_slice_flag( main, volume_index, view );
    update_slice_tag_objects( main, volume_index, view );
}

public  void  initialize_slice_view(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            x_viewport_size, y_viewport_size;
    int            x_axis_index, y_axis_index;
    BOOLEAN        x_flip, y_flip;
    Volume         volume;
    Real           x_trans, y_trans, x_scale, y_scale;

    volume = get_slice_volume( main, volume_index );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    get_slice_axes_flip( view, &x_flip, &y_flip );
    get_slice_viewport_size( main, volume_index, view, &x_viewport_size,
                             &y_viewport_size );

    fit_volume_slice_to_viewport( volume, x_axis_index, y_axis_index,
                                  x_flip, y_flip,
                                  x_viewport_size, y_viewport_size,
                                  1.0 - Slice_fit_size,
                                  &x_scale, &y_scale, &x_trans, &y_trans );

    set_slice_translation( main, volume_index, view, x_trans, y_trans );
    set_slice_scale( main, volume_index, view, x_scale, y_scale );
    record_slice_display_limits( main, volume_index, view );

    update_volume_cursor( main, volume_index, view );
    set_recreate_slice_flag( main, volume_index, view );
    update_slice_tag_objects( main, volume_index, view );
}
