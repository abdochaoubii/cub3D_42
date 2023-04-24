/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kboughal < kboughal@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 20:46:24 by kboughal          #+#    #+#             */
/*   Updated: 2023/04/24 15:39:54 by kboughal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/wall.h"

t_vars *g_vars;

int g_map[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 1, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

int32_t create_color(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void draw_ray(t_vars *vars)
{
	// t_ray	ray;
	int	      r;
	int	      mx;
	int			my;
	int			mp;
	int			dof;
	double      rx;
	double		ry;
	double		ra;
	double		xo; //x offset
	double		yo; //y offset

	ra = vars->player.angle;
	for (int i = 0; i < 60; i++)
	{
		dof = 0;
		float aTan = -1/tan(ra);
		if (ra > M_PI) //facing down
		{
			ry = (((int)vars->player.y>>6)<<6)-0.0001;
			rx = (vars->player.y - ry) * aTan + vars->player.x;
			yo = -64;
			xo = -yo * aTan;
		}
		if (ra < M_PI) //facing up
		{
			ry = (((int)vars->player.y>>6)<<6) + 64;
			rx = (vars->player.y - ry) * aTan + vars->player.x;
			yo = 64;
			xo = -yo * aTan;
		}
		 if(ra == 0 || ra == M_PI)
		{
			rx = vars->player.x;
			ry = vars->player.y;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) / 64;
			my = (int)(ry) / 64;
			mp = my * vars->map.height + mx;
			printf("mx: %d my: %d\n", mx, my);
			if(mx > 0 && my > 0 && mx < 8 && my < 8 && g_map[my][mx] == 1)
				dof = 8;
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}
		put_line(vars->mlx, vars->win, vars->player.x, vars->player.y, rx, ry, create_color(255,0,255,255));
		ra += 0.01;
	}
}

void	draw_partcle(t_vars *vars)
{
	int			radius;
	int 		i;
	int 		j;
	int 		py;
	int 		px;
	uint32_t	color;

	i = -1;
	radius = 10;
	px = vars->player.x;
	py = vars->player.y;
	color = create_color(255, 255, 255, 255);
	for (int x = 0; x < 640; x++)
	{
		for (int y = 0; y < 640; y++)
		{
			if ((x - px) * (x - px) + (y - py) * (y - py) < radius * radius)
				mlx_put_pixel(vars->img, x ,y , color);
		}
	}
	put_line(vars->mlx, vars->win, vars->player.x, vars->player.y, vars->player.x + vars->player.dx * 10, vars->player.y + vars->player.dy * 10 , create_color(0,255,0,255));
}

void	draw_tile(t_vars *vars, int y, int x)
{
	int	i;
	int	j;
	uint32_t color_black = create_color(0, 0, 0, 255);
	uint32_t color_white = create_color(255, 255, 255, 255);
	uint32_t color_red = create_color(255, 0, 0, 255);
	i = -1;
	while (++i < 64)
	{
		j = -1;
		while (++j < 64)
		{
			mlx_put_pixel(vars->img, (64 * x) + i, (64 * y) + j, color_black);
			if(g_map[y][x])
				mlx_put_pixel(vars->img, (64 * x) + i, (64 * y) + j, color_red);
			if(j == 63 || i == 63)
				mlx_put_pixel(vars->img, (64 * x) + i, (64 * y) + j, color_white);
		}
	}

}

void	draw_map(t_vars *vars)
{
	int	x;
	int	y;

	y = -1;
	while (++y < 8)
	{
		x = -1;
		while (++x < 8)
			draw_tile(vars, y, x);
	}

}

void	redraw(t_vars *vars)
{
	draw_map(vars);
	draw_partcle(vars);
	draw_ray(vars);
}

int key_press_handler(mlx_key_data_t keydata, void *param)
{
	t_vars	*vars;

	vars = (t_vars *)param;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_D))
	{
		vars->player.angle += 0.1;
		if(vars->player.angle > 2*M_PI)
			vars->player.angle -= 2 * M_PI;
		vars->player.dx = 5 * cos(vars->player.angle);
		vars->player.dy = 5 * sin(vars->player.angle);
	}
	if (mlx_is_key_down(vars->mlx, MLX_KEY_A))
	{
		vars->player.angle -= 0.1;
		if(vars->player.angle < 0)
			vars->player.angle += 2 * M_PI;
		vars->player.dx = 5 * cos(vars->player.angle);
		vars->player.dy = 5 * sin(vars->player.angle);
	}
	if (mlx_is_key_down(vars->mlx, MLX_KEY_W))
	{
			vars->player.x += vars->player.dx;
			vars->player.y += vars->player.dy;
	}
	if (mlx_is_key_down(vars->mlx, MLX_KEY_S))
	{
			vars->player.x -= vars->player.dx;
			vars->player.y -= vars->player.dy;
	}
	redraw(vars);
	// if (mlx_is_key_down(vars->mlx, MLX_KEY_S))
	// 	player_advance(vars->map, vars->player, -1);
	// if (mlx_is_key_down(vars->mlx, MLX_KEY_LEFT))
	// {
	// 	draw_map(vars);
	// 	draw_partcle(vars);
	// 	vars->player.direction--;
	// 	draw_rays(vars);
	// }
	// if (mlx_is_key_down(vars->mlx, MLX_KEY_RIGHT))
	// {
	// 	draw_map(vars);
	// 	draw_partcle(vars);
	// 	vars->player.direction++;
	// 	draw_rays(vars);
	// }
	if (mlx_is_key_down(vars->mlx, MLX_KEY_ESCAPE)) //free shit
		exit(0);
	return (0);
}

void	render_window(t_vars *vars)
{
	int	width;
	int	height;

	width = vars->window_info.width;
	height = vars->window_info.height;
	vars->mlx = mlx_init(width, height, "cub3D", 1);
	mlx_set_window_limit(vars->mlx,  width - 200, height - 200, width, height);
	vars->img = mlx_new_image(vars->mlx, width, height);
	draw_map(vars);
	draw_partcle(vars);
	// draw_ray(vars);
	// draw_rays(vars);
	mlx_key_hook(vars->mlx, (mlx_keyfunc)key_press_handler, vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
	mlx_loop(vars->mlx);
	mlx_terminate(vars->mlx);
}

int	init_vars(void)
{
	g_vars = (t_vars *)malloc(sizeof(t_vars));
	if(!g_vars)
		return (0);
	g_vars->window_info.height = 520;
	g_vars->window_info.width = 1024;
	g_vars->player.angle = 45 * (M_PI/180);
	g_vars->player.x = 257;
	g_vars->player.y = 257;
	g_vars->player.dx = 5 * cos(g_vars->player.angle);
	g_vars->player.dy = 5 * sin(g_vars->player.angle);
	return (1);
}


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if(!init_vars())
		return (0);
	render_window(g_vars);
	return (0);
}