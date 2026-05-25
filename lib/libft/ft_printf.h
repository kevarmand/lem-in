/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:44:19 by kearmand          #+#    #+#             */
/*   Updated: 2025/02/14 17:43:03 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <unistd.h>
# include <stdarg.h>

# define TABSIZE 23

# define TTYPE "%cspdiuxX"
# define NB_TYPE 9
# define PERCENT 0
# define CHAR 1
# define STR 2
# define POINT 3
# define INT 4
# define UNSIGNED 6
# define HEX 7
# define HEXA 8

# define TFLAG "# 0+-"
# define NBFLAG 5
# define DIESE 0
# define SPACE 1
# define ZERO 2
# define PLUS 3
# define MINUS 4
# define PRECISION 5

typedef struct s_data
{
	int		lenght;
	int		type;
	int		flag[NBFLAG + 1];
	int		sign;
	int		field;
	int		precision;
	int		size;
	int		n_head;
	int		n_zero;
	int		n_tail;
}	t_data;

int		ft_printf(const char *format, ...);

void	ft_print_head(t_data *data);
void	ft_print_tail(t_data *data);
int		ft_write(t_data *data, char *str, int n);

int		ft_parse(char *str, t_data *data, va_list *ap);
int		ft_is_flag(char str, t_data *data);
int		ft_parse_field(char **str, t_data *data);

int		ft_calc_int(t_data *data, int size);
int		ft_calc_str(t_data *data, char **str);

int		ft_print_char(t_data *data, char c);
int		ft_print_str(t_data *data, char *str);
int		ft_print_pointer(t_data *data, unsigned long p);
int		ft_print_int(t_data *data, int n);
int		ft_print_uns(t_data *data, unsigned int n);
int		ft_print_hex(t_data *data, unsigned int n);
int		ft_print_hexa(t_data *data, int n);

int		ft_putnbr_base(t_data *data, char *tab, long n, char *base);
int		ft_putnbr_base_u(t_data *d, char *t, unsigned long n, char *b);
int		ft_atoi_pf(char **str);
void	ft_init(t_data *data);

int		ft_print_data(t_data *data);
#endif
