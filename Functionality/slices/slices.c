
#include  <register.h>

private   int         x_axes[] = { Y, X, X };
private   int         y_axes[] = { Z, Z, Y };
private   BOOLEAN     x_axes_flip[] = { FALSE, FALSE, FALSE };
private   BOOLEAN     y_axes_flip[] = { FALSE, FALSE, FALSE };

private  void  check_axes_assigned()
{
    private  BOOLEAN  first = TRUE;

    if( first )
    {
        first = FALSE;
        x_axes[0] = Slice_1_x_axis_index;
        x_axes[1] = Slice_2_x_axis_index;
        x_axes[2] = Slice_3_x_axis_index;
        y_axes[0] = Slice_1_y_axis_index;
        y_axes[1] = Slice_2_y_axis_index;
        y_axes[2] = Slice_3_y_axis_index;
        x_axes_flip[0] = Slice_1_x_axis_flip;
        x_axes_flip[1] = Slice_2_x_axis_flip;
        x_axes_flip[2] = Slice_3_x_axis_flip;
        y_axes_flip[0] = Slice_1_y_axis_flip;
        y_axes_flip[1] = Slice_2_y_axis_flip;
        y_axes_flip[2] = Slice_3_y_axis_flip;
    }
}

public  void  get_slice_axes(
    int   view_index,
    int   *x_axis_index,
    int   *y_axis_index )
{
    check_axes_assigned();

    *x_axis_index = x_axes[view_index];
    *y_axis_index = y_axes[view_index];
}

public  void  get_slice_axes_flip(
    int       view_index,
    BOOLEAN   *x_axis_flip,
    BOOLEAN   *y_axis_flip )
{
    check_axes_assigned();

    *x_axis_flip = x_axes_flip[view_index];
    *y_axis_flip = y_axes_flip[view_index];
}

public  int  get_slice_axis(
    int   view_index )
{
    int   axis;

    check_axes_assigned();

    for_less( axis, 0, N_DIMENSIONS )
    {
        if( x_axes[view_index] != axis && 
            y_axes[view_index] != axis )
        {
            return( axis );
        }
    }

    return( 0 );
}

public  int  get_slice_viewport_index( int volume, int view )
{
    return( volume * N_VIEWS + view );
}

public  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view )
{
    *volume = viewport_index / N_VIEWS;
    *view = viewport_index % N_VIEWS;
}

public  void  get_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_min,
    int          *x_max,
    int          *y_min,
    int          *y_max )
{
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );
}

public  void  get_slice_viewport_size(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_size,
    int          *y_size )
{
    int   x_min, x_max, y_min, y_max;
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           &x_min, &x_max, &y_min, &y_max );

    *x_size = x_max - x_min + 1;
    *y_size = y_max - y_min + 1;
}

public  void  set_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_min,
    int          x_max,
    int          y_min,
    int          y_max )
{
    set_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );

    if( is_volume_active( main, volume ) )
        fit_slice_to_view( main, volume, view );
}

public  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume_index,
    Real           position[N_DIMENSIONS] )
{
    BOOLEAN        changed;
    Volume         volume;
    int            view, axis, sizes[N_DIMENSIONS];
    Real           pos;
    Real           *cursor_ptr;

    volume = get_slice_volume( main, volume_index );
    get_volume_sizes( volume, sizes );
    cursor_ptr = get_volume_cursor( main, volume_index );

    changed = FALSE;

    for_less( axis, 0, N_DIMENSIONS )
    {
        if( position[axis] < -0.5 )
            pos = -0.5;
        else if( position[axis] > (Real) sizes[axis] - 0.5 )
            pos = (Real) sizes[axis] - 0.5;
        else
            pos = position[axis];

        if( pos != cursor_ptr[axis] )
        {
            changed = TRUE;

            cursor_ptr[axis] = pos;

            for_less( view, 0, N_VIEWS )
            {
                if( get_slice_axis( view ) == axis )
                {
                    set_recreate_slice_flag( main, volume_index, view );
                    update_slice_tag_objects( main, volume_index, view );
                }
            }
        }
    }

    if( changed )
        update_volume_cursors( main, volume_index );
}

public  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    Real          position[N_DIMENSIONS] )
{
    Real           *cursor_ptr;

    cursor_ptr = get_volume_cursor( main, volume );

    position[X] = cursor_ptr[X];
    position[Y] = cursor_ptr[Y];
    position[Z] = cursor_ptr[Z];
}

public  void  convert_original_world_to_world(
    main_struct    *main,
    int            volume_index,
    Real           x_original,
    Real           y_original,
    Real           z_original,
    Real           *x_world,
    Real           *y_world,
    Real           *z_world )
{
    General_transform  *transform;

    if( volume_index == RESAMPLED_VOLUME_INDEX &&
        get_tag_point_transform( main, &transform ) )
    {
        general_transform_point( transform, x_original, y_original, z_original,
                                 x_world, y_world, z_world );
    }
    else
    {
        *x_world = x_original;
        *y_world = y_original;
        *z_world = z_original;
    }
}

public  void  convert_world_to_original_world(
    main_struct    *main,
    int            volume_index,
    Real           x_world,
    Real           y_world,
    Real           z_world,
    Real           *x_original,
    Real           *y_original,
    Real           *z_original )
{
    General_transform  *transform;

    if( volume_index == RESAMPLED_VOLUME_INDEX &&
        get_tag_point_transform( main, &transform ) )
    {
        general_inverse_transform_point( transform, x_world, y_world, z_world,
                                         x_original, y_original, z_original );
    }
    else
    {
        *x_original = x_world;
        *y_original = y_world;
        *z_original = z_world;
    }
}

public  void  set_volume_world_position(
    main_struct    *main,
    int            volume_index,
    Real           world_position[N_DIMENSIONS] )
{
    Real           original_world[N_DIMENSIONS];
    Real           voxel_position[N_DIMENSIONS];

    convert_world_to_original_world( main, volume_index,
                                     world_position[X],
                                     world_position[Y],
                                     world_position[Z],
                                     &original_world[X],
                                     &original_world[Y],
                                     &original_world[Z] );
    convert_original_world_to_voxel( main, volume_index,
                                     original_world[X],
                                     original_world[Y],
                                     original_world[Z],
                                     &voxel_position[X],
                                     &voxel_position[Y],
                                     &voxel_position[Z] );

    set_volume_voxel_position( main, volume_index, voxel_position );
}

public  void  get_volume_world_position(
    main_struct   *main,
    int           volume_index,
    Real          world_position[N_DIMENSIONS] )
{
    Real           voxel_position[N_DIMENSIONS];
    Real           original_world[N_DIMENSIONS];

    get_volume_voxel_position( main, volume_index, voxel_position );

    convert_voxel_to_original_world( main, volume_index,
                                     voxel_position[X],
                                     voxel_position[Y],
                                     voxel_position[Z],
                                     &original_world[X],
                                     &original_world[Y],
                                     &original_world[Z] );
    convert_original_world_to_world( main, volume_index,
                                     original_world[X],
                                     original_world[Y],
                                     original_world[Z],
                                     &world_position[X],
                                     &world_position[Y],
                                     &world_position[Z] );
}

public  void  convert_original_world_to_voxel(
    main_struct    *main,
    int            volume_index,
    Real           x_original,
    Real           y_original,
    Real           z_original,
    Real           *x_voxel,
    Real           *y_voxel,
    Real           *z_voxel )
{
    Real   x_world, y_world, z_world, voxel[MAX_DIMENSIONS];

    if( volume_index == RESAMPLED_VOLUME_INDEX && main->resampled_file_loaded )
    {
        general_transform_point( &main->resampling_transform,
                                 x_original, y_original, z_original,
                                 &x_world, &y_world, &z_world );
    }
    else
    {
        x_world = x_original;
        y_world = y_original;
        z_world = z_original;
    }

    convert_world_to_voxel( get_slice_volume(main,volume_index),
                            x_world, y_world, z_world, voxel );

    *x_voxel = voxel[X];
    *y_voxel = voxel[Y];
    *z_voxel = voxel[Z];
}

public  void  convert_voxel_to_original_world(
    main_struct    *main,
    int            volume_index,
    Real           x_voxel,
    Real           y_voxel,
    Real           z_voxel,
    Real           *x_original,
    Real           *y_original,
    Real           *z_original )
{
    Real   voxel[MAX_DIMENSIONS], world_position[N_DIMENSIONS];

    voxel[X] = x_voxel;
    voxel[Y] = y_voxel;
    voxel[Z] = z_voxel;

    convert_voxel_to_world( get_slice_volume(main,volume_index),
                            voxel,
                            &world_position[X],
                            &world_position[Y],
                            &world_position[Z] );

    if( volume_index == RESAMPLED_VOLUME_INDEX && main->resampled_file_loaded )
    {
        general_inverse_transform_point( &main->resampling_transform,
                        world_position[X], world_position[Y], world_position[Z],
                        x_original, y_original, z_original );
    }
    else
    {
        *x_original = world_position[X];
        *y_original = world_position[Y];
        *z_original = world_position[Z];
    }
}

public  void  set_volume_original_world_position(
    main_struct    *main,
    int            volume_index,
    Real           original_world_position[N_DIMENSIONS] )
{
    Real           voxel_position[N_DIMENSIONS];

    convert_original_world_to_voxel( main, volume_index,
                                     original_world_position[X],
                                     original_world_position[Y],
                                     original_world_position[Z],
                                     &voxel_position[X],
                                     &voxel_position[Y],
                                     &voxel_position[Z] );

    set_volume_voxel_position( main, volume_index, voxel_position );
}

public  void  get_volume_original_world_position(
    main_struct   *main,
    int           volume_index,
    Real          original_world_position[N_DIMENSIONS] )
{
    Real           voxel_position[N_DIMENSIONS];

    get_volume_voxel_position( main, volume_index, voxel_position );

    convert_voxel_to_original_world( main, volume_index,
                                     voxel_position[X],
                                     voxel_position[Y],
                                     voxel_position[Z],
                                     &original_world_position[X],
                                     &original_world_position[Y],
                                     &original_world_position[Z] );
}

public  Volume  get_slice_volume(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index < N_VOLUMES )
        return( main->trislice[volume_index].volume );
    else
        return( main->trislice[volume_index-MERGED_VOLUME_INDEX].volume );
}

public  void  get_volume_range_of_voxels(
    main_struct   *main,
    int           volume_index,
    int           *min_value,
    int           *max_value )
{
    Real    real_min, real_max;
    Volume  volume;

    if( is_volume_active( main, volume_index ) )
    {
        volume = get_slice_volume( main, volume_index );

        get_volume_voxel_range( volume, &real_min, &real_max );
        *min_value = ROUND( real_min );
        *max_value = ROUND( real_max );
    }
    else
    {
        *min_value = 0;
        *max_value = -1;
    }
}

public  void  get_volume_value_range(
    main_struct   *main,
    int           volume_index,
    Real          *min_value,
    Real          *max_value )
{
    get_volume_real_range( get_slice_volume(main,volume_index),
                           min_value, max_value );
}

public  Real  *get_volume_cursor(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.position );
    else
        return( main->trislice[volume_index].position );
}

public  slice_struct  *get_slice_struct(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( &main->merged.slices[view] );
    else
        return( &main->trislice[volume_index].slices[view] );
}

public  void  get_slice_transform(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          *x_trans,
    Real          *y_trans,
    Real          *x_scale,
    Real          *y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    *x_trans = slice->x_translation;
    *y_trans = slice->y_translation;
    *x_scale = slice->x_scale;
    *y_scale = slice->y_scale;
}

public  void  set_slice_translation(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_trans,
    Real          y_trans )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_translation = x_trans;
    slice->y_translation = y_trans;
}

public  void  set_slice_scale(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_scale,
    Real          y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_scale = x_scale;
    slice->y_scale = y_scale;
}

public  Real  get_voxel_value(
    main_struct   *main,
    int           volume_index,
    Real          x_voxel,
    Real          y_voxel,
    Real          z_voxel )
{
    Real           value, position[N_DIMENSIONS];
    Volume         volume;

    volume = get_slice_volume( main, volume_index );

    position[X] = x_voxel;
    position[Y] = y_voxel;
    position[Z] = z_voxel;

    if( voxel_is_within_volume( volume, position ) )
    {
        GET_VOXEL_3D( value, volume, ROUND(x_voxel),
                      ROUND(y_voxel), ROUND(z_voxel) );
        value = CONVERT_VOXEL_TO_VALUE( volume, value );
    }
    else
        value = 0.0;

    return( value );
}

public  void  set_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view,
    Filter_types  filter_type )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->filter_type = filter_type;

    set_recreate_slice_flag( main, volume_index, view );
}

public  Filter_types  get_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    return( slice->filter_type );
}

public  void  set_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          filter_width )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->filter_width = filter_width;

    set_recreate_slice_flag( main, volume_index, view );
}

public  Real  get_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    return( slice->filter_width );
}
