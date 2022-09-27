#ifndef __MICROSHELL_H__
# define __MICROSHELL_H__

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <string.h> 

#endif

int	tmp;


int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

int	ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (1);
}

int	exec_cd(char **av, int i)
{
	if (i != 2)
		return (ft_error("error: cd: bad arguments\n"));
	if (chdir(av[1]) == -1)
		return (ft_error("error: cd: cannot change dir to ") & ft_error(av[1]) & ft_error("\n"));
	return (0);
}

int	exec_cmd(char **av, int i, char **envp)
{
	int	pid;
	int	pip = (av[i] && !strcmp(av[i], "|"));
	int	ret;
	int	fd[2];

	if (pip && pipe(fd) == -1)
		return (ft_error("error: fatal\n"));
	pid = fork();
	if (!pid)
	{
		av[i] = 0;
		if ((dup2(tmp, 0) == -1) | (close(tmp) == -1) | (pip && ((dup2(fd[1], 1) == -1) | (close(fd[0] == -1)) | (close(fd[1]) == -1))))
			return (ft_error("error: fatal\n"));
		execve(*av, av, envp);
		return (ft_error("error: cannot execute ") & ft_error(*av) & ft_error("\n"));
	}
	if ((!pip && (dup2(0, tmp) == -1)) 
			| (pip && ((dup2(fd[0], tmp) == -1) | (close(fd[1]) == -1) | (close(fd[0]) == -1)))
			| (waitpid(pid, &ret, 0) == -1))
		return (ft_error("error: fatal\n"));
	return (WIFEXITED(ret) & WEXITSTATUS(ret));
}

int	main(int ac, char **av, char **envp)
{
	int	ret;
	int	i = 0;

	(void)ac;
	tmp = dup(0);
	while (av[i] && av[++i])
	{
		av = av + i;
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (!strcmp(*av, "cd"))
			ret = exec_cd(av, i);
		else if (i)
			ret = exec_cmd(av, i, envp);
	}
	return ((dup2(0, tmp) == -1 && ft_error("error: fatal\n"))) | ret;
}
