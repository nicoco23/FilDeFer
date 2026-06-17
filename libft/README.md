*This project has been created as part of the 42 curriculum by ntassin*

# Libft — Your very first own library

## Description
**Libft** is a personal C library that reimplements a set of standard libc functions and adds additional utility functions (strings, memory, conversions, I/O helpers, and optional linked-list helpers).  
The goal is to build a reliable toolbox that will be reused in later 42 projects, while following the 42 Norm and common constraints.

Key constraints (as required by the subject):
- Written in **C** and compliant with the **42 Norm**
- No unexpected crashes (segfault, double free, etc.)
- No memory leaks (heap allocations must be freed when needed)
- Compilation with `cc` and flags `-Wall -Wextra -Werror`
- Library built with `ar` (no `libtool`)
- Output archive: **`libft.a`** at the repository root
- All submitted files at the repository root (no unused files)

## Instructions

### Build
Compile the library:
```bash
make
```
## Function reference (usage table)

> Conventions: all functions are prefixed with `ft_`. Examples are minimal and assume `#include "libft.h"`.

### Part 1 — Libc reimplementations

| Function | Typical prototype | Use / Example |
|---|---|---|
| `ft_isalpha` | `int ft_isalpha(int c);` | Check if `c` is a letter. `if (ft_isalpha(ch)) ...` |
| `ft_isdigit` | `int ft_isdigit(int c);` | Check if `c` is a digit. `if (ft_isdigit(ch)) ...` |
| `ft_isalnum` | `int ft_isalnum(int c);` | Letter or digit. `ft_isalnum('A')` |
| `ft_isascii` | `int ft_isascii(int c);` | ASCII range (0–127). `ft_isascii(128)` |
| `ft_isprint` | `int ft_isprint(int c);` | Printable char. `ft_isprint('\n')` |
| `ft_strlen` | `size_t ft_strlen(const char *s);` | String length (no `\0`). `len = ft_strlen("42");` |
| `ft_memset` | `void *ft_memset(void *b, int c, size_t len);` | Fill memory. `ft_memset(buf, 0, n);` |
| `ft_bzero` | `void ft_bzero(void *s, size_t n);` | Zero memory. `ft_bzero(buf, n);` |
| `ft_memcpy` | `void *ft_memcpy(void *dst, const void *src, size_t n);` | Copy memory (no overlap). `ft_memcpy(a,b,n);` |
| `ft_memmove` | `void *ft_memmove(void *dst, const void *src, size_t n);` | Copy memory (overlap-safe). `ft_memmove(a,a+1,n);` |
| `ft_strlcpy` | `size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);` | Copy string with size. `ft_strlcpy(d,s,sizeof d);` |
| `ft_strlcat` | `size_t ft_strlcat(char *dst, const char *src, size_t dstsize);` | Append with size. `ft_strlcat(d,s,sizeof d);` |
| `ft_toupper` | `int ft_toupper(int c);` | To uppercase. `c = ft_toupper(c);` |
| `ft_tolower` | `int ft_tolower(int c);` | To lowercase. `c = ft_tolower(c);` |
| `ft_strchr` | `char *ft_strchr(const char *s, int c);` | First occurrence. `p = ft_strchr(s,'/');` |
| `ft_strrchr` | `char *ft_strrchr(const char *s, int c);` | Last occurrence. `p = ft_strrchr(s,'.');` |
| `ft_strncmp` | `int ft_strncmp(const char *s1, const char *s2, size_t n);` | Compare up to `n`. `ft_strncmp(a,b,5)==0` |
| `ft_memchr` | `void *ft_memchr(const void *s, int c, size_t n);` | Find byte in memory. `ft_memchr(buf,0,n);` |
| `ft_memcmp` | `int ft_memcmp(const void *s1, const void *s2, size_t n);` | Compare memory blocks. `ft_memcmp(a,b,n)` |
| `ft_strnstr` | `char *ft_strnstr(const char *hay, const char *needle, size_t len);` | Find substring in first `len` chars. `ft_strnstr(s,"42",10);` |
| `ft_atoi` | `int ft_atoi(const char *str);` | ASCII to int. `n = ft_atoi("  -42");` |
| `ft_calloc` | `void *ft_calloc(size_t count, size_t size);` | Allocate zeroed memory. `p = ft_calloc(n, sizeof *p);` |
| `ft_strdup` | `char *ft_strdup(const char *s1);` | Duplicate string (malloc). `copy = ft_strdup(s);` |

### Part 2 — Additional functions

| Function | Typical prototype | Use / Example |
|---|---|---|
| `ft_substr` | `char *ft_substr(char const *s, unsigned int start, size_t len);` | Extract substring. `sub = ft_substr("hello",1,3); // "ell"` |
| `ft_strjoin` | `char *ft_strjoin(char const *s1, char const *s2);` | Concatenate new string. `s = ft_strjoin("42","Paris");` |
| `ft_strtrim` | `char *ft_strtrim(char const *s1, char const *set);` | Trim chars from ends. `t = ft_strtrim("  hi  "," ");` |
| `ft_split` | `char **ft_split(char const *s, char c);` | Split string by delimiter. `arr = ft_split("a,b,c",',');` |
| `ft_itoa` | `char *ft_itoa(int n);` | Int to string. `s = ft_itoa(-42);` |
| `ft_strmapi` | `char *ft_strmapi(char const *s, char (*f)(unsigned int, char));` | Map to new string. `ft_strmapi("abc", f);` |
| `ft_striteri` | `void ft_striteri(char *s, void (*f)(unsigned int, char*));` | In-place iteration. `ft_striteri(s, f);` |
| `ft_putchar_fd` | `void ft_putchar_fd(char c, int fd);` | Write one char. `ft_putchar_fd('A', 1);` |
| `ft_putstr_fd` | `void ft_putstr_fd(char *s, int fd);` | Write string. `ft_putstr_fd("hi", 1);` |
| `ft_putendl_fd` | `void ft_putendl_fd(char *s, int fd);` | Write string + newline. `ft_putendl_fd("ok", 1);` |
| `ft_putnbr_fd` | `void ft_putnbr_fd(int n, int fd);` | Write integer. `ft_putnbr_fd(42, 1);` |

### Bonus — Linked list (optional)

| Function | Typical prototype | Use / Example |
|---|---|---|
| `ft_lstnew` | `t_list *ft_lstnew(void *content);` | Create a node. `node = ft_lstnew(ptr);` |
| `ft_lstadd_front` | `void ft_lstadd_front(t_list **lst, t_list *new);` | Push front. `ft_lstadd_front(&lst, node);` |
| `ft_lstsize` | `int ft_lstsize(t_list *lst);` | Count nodes. `n = ft_lstsize(lst);` |
| `ft_lstlast` | `t_list *ft_lstlast(t_list *lst);` | Last node. `last = ft_lstlast(lst);` |
| `ft_lstadd_back` | `void ft_lstadd_back(t_list **lst, t_list *new);` | Push back. `ft_lstadd_back(&lst, node);` |
| `ft_lstdelone` | `void ft_lstdelone(t_list *lst, void (*del)(void*));` | Delete one node content+node. `ft_lstdelone(node, free);` |
| `ft_lstclear` | `void ft_lstclear(t_list **lst, void (*del)(void*));` | Clear entire list. `ft_lstclear(&lst, free);` |
| `ft_lstiter` | `void ft_lstiter(t_list *lst, void (*f)(void *));` | Apply `f` to each content. `ft_lstiter(lst, print);` |
| `ft_lstmap` | `t_list *ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));` | Map list to new list. `new = ft_lstmap(lst, f, del);` |
