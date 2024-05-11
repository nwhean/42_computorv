#ifndef MATHEMATICS_H
# define MATHEMATICS_H

# include <stdlib.h>

size_t	max(size_t a, size_t b);
void	*ft_exp(const void *params, void *env);
void	*ft_ln(const void *params, void *env);
void	*ft_sin(const void *params, void *env);
void	*ft_cos(const void *params, void *env);
void	*ft_tan(const void *params, void *env);
void	*ft_sinh(const void *params, void *env);
void	*ft_cosh(const void *params, void *env);
void	*ft_tanh(const void *params, void *env);
void	*ft_sqrt(const void *params, void *env);
void	*ft_abs(const void *params, void *env);
void	*ft_radians(const void *params, void *env);
void	*ft_degrees(const void *params, void *env);
void	*ft_norm(const void *params, void *env);
void	*ft_inv(const void *params, void *env);

#endif
