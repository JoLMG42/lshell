#include "minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
	}
	return (tmp);
}

void	ft_lstadd_back_env(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = *alst;
	if ((*alst))
	{
		tmp = ft_lstlast_env(*alst);
		tmp->next = new;
	}
	else
	{
		*alst = new;
	}
}

// void	del(void *data)
// {
// 	free(data);
// }


void	ft_lstdelone_env(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst->line)
		free(lst->line);
	if (lst->name)
		free(lst->name);
	if (lst->content)
		free(lst->content);
	free(lst);
}

void	ft_lstclear_env(t_env **lst, void (*del)(void *))
{
	t_env	*list;
	t_env	*tmp;

	if (!lst || !del)
		return ;
	list = *lst;
	while (list)
	{
		tmp = list->next;
		ft_lstdelone_env(list, del);
		list = tmp;
	}
	*lst = NULL;
}



t_env	*ft_lstnew_env(char	*line, char	*name, char	*content)
{
	t_env	*tmp;

	tmp = malloc(sizeof(struct s_env));
	if (!tmp)
		return (0);
	tmp->line = line;
	tmp->name = name;
	tmp->content = content;
	tmp->next = NULL;
	return (tmp);
}




static int	ft_check_charset(const char str, char c)
{
	if (str == c)
		return (1);
	return (0);
}

static int	ft_count_word(const char *str, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (ft_check_charset(str[i], c) == 0)
			i++;
		else
		{
			while (str[i] && ft_check_charset(str[i], c) == 1)
				i++;
			count++;
		}
	}
	return (count);
}

static int	ft_lenword(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && ft_check_charset(str[i], c) == 0)
	{
		i++;
	}
	return (i);
}

static char	*ft_cpy(const char *str, char c, int n)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_lenword(str, c) + 1));
	if (!cpy)
		return (0);
	while (str[i] && i < n)
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	**ft_supersplit_lucie(char *s, char c)
{
	int		i;
	int		j;
	char	**str;

	i = 0;
	j = 0;
	str = malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!str)
		return (0);
	while (s[i])
	{
		if (ft_check_charset(s[i], c) == 1)
			i++;
		else
		{
			str[j] = ft_cpy(s + i, c, ft_lenword(s + i, c));
			j++;
			i = i + ft_lenword(s + i, c);
		}
	}
	str[j] = 0;
	return (str);
}

char *checkegal(char *str, char c)
{
    int i = 0;

    while (str && str[i] )
    {
        if (str[i] == c)
        {
            str = *ft_supersplit_lucie(str, c);
            return (str);
        }
        i++;
    }
    return 0;
}

char *checkafteregal(char *str, char c)
{
    int i = 0;
    while(str &&str[i])
    {
        if (str[i] == c)
        {
            str = *ft_supersplit_lucie(str + i, c);
            return (str);
        }
        i++;
    }
    return 0;
}

void get_env(t_env **env, char **envi)
{
    t_env   *tmp;
    char *name;
    char *content;
    int i = 0;

    tmp = *env;

    while(envi[i++])
    {
        // printf("ENV: %s\n\n\n", envi[i]);
        name = checkegal(envi[i], '=');
        content = checkafteregal(envi[i], '=');
        ft_lstadd_back_env(&tmp, ft_lstnew_env(ft_strdup(envi[i]), ft_strdup(name), ft_strdup(content)));
        free(name);
        free(content);
    }
}

void printenv(t_env **env, char **envi)
{
    t_env   *tmp;

    tmp = (*env)->next;
    // printf("%s\n", tmp->name);
    // printf("%s\n", tmp->content);
    while(tmp)
    {
        printf("%s\n", tmp->line);
        tmp = tmp->next;
    }
}


char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (to_find[0] == '\0')
		return (str);
	i = 0;
	while (str && str[i] != '\0')
	{
		j = 0;
		if (str[i] == to_find[j])
		{
			while (str[i + j] == to_find[j])
			{
				j++;
				if (to_find[j] == '\0')
					return (str + i);
			}
		}
		i++;
	}
	return (0);
}
