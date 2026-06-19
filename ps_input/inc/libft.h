/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:42:40 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/25 15:14:25 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdint.h>
# include <stdarg.h>

int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
size_t				ft_strlen(const char *s);
void				*ft_memset(void *s, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
int					ft_toupper(int c);
int					ft_tolower(int c);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
int					ft_atoi(const char *str);
void				*ft_calloc(size_t count, size_t size);
char				*ft_strdup(const char *s);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putfloat_fd(double n, int prec, int fd);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

/* get_next_line — gnl_* helpers are namespaced to avoid clashing with libft */
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char				*get_next_line(int fd);
size_t				gnl_strlen(char const *s);
char				*gnl_strchr(const char *s, int c);
char				*gnl_strjoin(char const *s1, char const *s2);
void				*gnl_memcpy(void *dest, const void *src, size_t n);
char				*gnl_strdup(const char *s);

/* ft_printf */
typedef struct s_flags
{
	int				space;
	char			padchar;
	int				left;
	int				showsign;
	int				prefix;
	int				width;
	int				precision;
}					t_flags;

int					ft_printf(const char *format, ...);
int					ft_handle_text(const char *format, int i);
int					ft_handle_conversion(const char *format, int *i,
						va_list ap);
int					ft_return_check(int ret_pad, int ret_write);
t_flags				ft_printf_parser(const char *format, int *i);
t_flags				ft_parse_flags(const char *format, int *i, t_flags info);
t_flags				ft_parse_width(const char *format, int *i, t_flags info);
t_flags				ft_parse_precision(const char *format, int *i,
						t_flags info);
int					ft_dispatch(char specifier, t_flags info, va_list ap);
int					ft_putchar_count(char c);
int					ft_putchar_pad(int len, char c);
int					ft_print_char(int c, t_flags info);
int					ft_print_str(char *s, t_flags info);
int					ft_print_nbr(int n, t_flags info);
int					ft_print_nbr_content(int n, int nbrlen, int precise_len,
						t_flags info);
int					ft_print_nbr_base(unsigned long nbr, char *base,
						int nbrlen);
int					ft_nbrlen_base(unsigned long nbr, char *base);
int					ft_print_unsigned(unsigned int nbr, t_flags info);
int					ft_print_ptr(void *p, t_flags info);
int					ft_print_ptr_content(unsigned long nbr, int nbrlen,
						t_flags info, int is_pointer);
int					ft_print_hex(unsigned int nbr, t_flags info, int is_upper);
int					ft_print_hex_content(unsigned long nbr, int nbrlen,
						t_flags info, int is_upper);
int					ft_calc_precise_len(unsigned long nbr, char *base,
						t_flags info, int *nbrlen);
int					ft_apply_precision(int nbrlen, t_flags info);
int					ft_apply_sign(long n, t_flags info);
int					ft_apply_padding(int len, t_flags info);
int					ft_apply_prefix(int is_upper, int is_pointer, t_flags info);
void				ft_float_round(double f, int prec, long *ip, long *fp);
int					ft_float_frac(long frac, int precision);
int					ft_print_float(double f, t_flags info);

#endif
