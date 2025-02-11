/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:22:50 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 16:21:22 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*str;
	size_t	size;

	if (s1 == NULL || s2 == NULL)
		return (0);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = malloc(size);
	if (!str)
		return (0);
	i = 0;
	while (*s1)
	{
		str[i] = *s1;
		s1++;
		i++;
	}
	while (*s2)
	{
		str[i] = *s2;
		s2++;
		i++;
	}
	str[i] = '\0';
	return (str);
}

static size_t	nbr_len(long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		len++;
	}
	while (n > 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static void	putnbr(long n, char *str, int *i)
{
	if (n >= 10)
	{
		putnbr(n / 10, str, i);
		putnbr(n % 10, str, i);
	}
	else
	{
		str[*i] = n + '0';
		(*i)++;
	}
	return ;
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;
	long	nbr;
	size_t	len;

	nbr = n;
	len = nbr_len(nbr);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len] = '\0';
	i = 0;
	if (n < 0)
	{
		str[i] = '-';
		nbr = -nbr;
		i++;
	}
	putnbr(nbr, str, &i);
	return (str);
}
