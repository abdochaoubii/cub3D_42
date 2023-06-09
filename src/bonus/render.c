/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kboughal < kboughal@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 18:40:35 by kboughal          #+#    #+#             */
/*   Updated: 2023/05/19 19:58:08 by kboughal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	mouse_click(mouse_key_t button, action_t action, modifier_key_t mods,
		void *param)
{
	t_vars	*vars;

	(void)mods;
	vars = (t_vars *)param;
	if (button == 0 && action == 1 && vars->player.bullet
		&& !vars->player.reload)
	{
		vars->player.bullet--;
		vars->player.shoot = 8;
		show_gun_magazine(vars);
	}
	else if (button == 0 && action == 1 && !vars->player.bullet
		&& !vars->player.reload)
	{
		vars->player.bullet = 8;
		vars->player.reload = 42;
		show_gun_magazine(vars);
	}
}

void	check_textures(void)
{
	if (!g_vars->so_wall_texture || !g_vars->ea_wall_texture
		|| !g_vars->we_wall_texture || !g_vars->no_wall_texture
		|| !g_vars->door_texture || !g_vars->ammo_texture || !g_vars->ammo_img)
	{
		free_g_map(g_vars->map.map, g_vars->map.height);
		exit(0);
	}
}

void	initialize_textures(void)
{
	g_vars->player.reload = 0;
	g_vars->so_wall_texture = mlx_load_png(g_vars->map_info.so_texure);
	g_vars->ea_wall_texture = mlx_load_png(g_vars->map_info.we_texure);
	g_vars->we_wall_texture = mlx_load_png(g_vars->map_info.ea_texure);
	g_vars->no_wall_texture = mlx_load_png(g_vars->map_info.no_texure);
	g_vars->door_texture = mlx_load_png("./textures/doorx.png");
	g_vars->ammo_texture = mlx_load_png("./textures/bullet.png");
	g_vars->ammo_img = mlx_texture_to_image(g_vars->mlx, g_vars->ammo_texture);
	check_textures();
	free(g_vars->map_info.no_texure);
	free(g_vars->map_info.so_texure);
	free(g_vars->map_info.we_texure);
	free(g_vars->map_info.ea_texure);
}

void	my_mlx_hooks(t_vars *vars)
{
	if (vars->keyboard.cursor)
		mlx_set_cursor_mode(vars->mlx, MLX_MOUSE_NORMAL);
	else
		mlx_set_cursor_mode(vars->mlx, MLX_MOUSE_DISABLED);
	mlx_key_hook(vars->mlx, (mlx_keyfunc)key_press_handler_2, vars);
	mlx_cursor_hook(vars->mlx, (mlx_cursorfunc)mouse_handler, vars);
	mlx_mouse_hook(vars->mlx, (mlx_mousefunc)mouse_click, vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
	mlx_loop_hook(vars->mlx, &loop_func, vars);
	show_gun_magazine(vars);
	mlx_loop(vars->mlx);
	mlx_terminate(vars->mlx);
}

void	render_window(t_vars *vars)
{
	int	width;
	int	height;

	width = vars->window_info.width;
	height = vars->window_info.height;
	vars->mlx = mlx_init(width, height, "cub3D", 1);
	if (!vars->mlx)
	{
		ft_putstr_fd((char *)mlx_strerror(mlx_errno), 2);
		exit(1);
	}
	mlx_set_window_limit(vars->mlx, width - 200, height - 200, width, height);
	vars->img = mlx_new_image(vars->mlx, width, height);
	initialize_textures();
	redraw(vars);
	my_mlx_hooks(vars);
}
