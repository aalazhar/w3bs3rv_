// std::string	Response::executeCgi(std::string file_name) {
	
// 	pid_t	pid;
// 	int		In;
// 	int		Out;
// 	int		rd;
// 	std::string	newbody;

// 	In = dup(STDIN_FILENO);
// 	Out = dup(STDOUT_FILENO);

// 	FILE	*fdIn = tmpfile();
// 	FILE	*fdOut = tmpfile();

// 	long	fileIn = fileno(fdIn);
// 	long	fileOut = fileno(fdOut);
	
// 	write(fileIn, this->Body.c_str(), this->Body.length());
// 	lseek(fileIn, 0, SEEK_SET);
	
// 	pid = fork();

// 	if (pid == -1) {

// 		std::cerr << "Fork !! \n";
// 		return ("Status: 500\r\n\r\n");
// 	}
// 	else if (!pid) {

// 		dup2(fileIn, STDIN_FILENO);
// 		dup2(fileOut, STDOUT_FILENO);
// 		char **env;
// 		execve(file_name.c_str(), NULL, env);
// 		std::cerr << "Execve !!" << std::endl;
// 		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
// 	}
// 	else {

// 		char	buffer[1024] = {0};

// 		waitpid(-1, NULL, 0);
// 		lseek(fileOut, 0, SEEK_SET);
		
// 		rd = 1;
// 		while (rd > 0) {

// 			memset(buffer, 0, 1024);
// 			rd = read(fileOut, buffer, 1024);
// 			newbody += buffer;
// 		}
// 	}
// 	dup2(In, STDIN_FILENO);
// 	dup2(Out, STDOUT_FILENO);
// 	fclose(fdIn);
// 	fclose(fdOut);
// 	close(fileIn);
// 	close(fileOut);
// 	close(In);
// 	close(Out);
// 	//
// 	return newbody;
// }