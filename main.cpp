/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:59:59 by aalazhar          #+#    #+#             */
/*   Updated: 2023/05/12 23:26:16 by aalazhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parserObject.hpp"
#include "CGI.hpp"

int main(int ac, char **av){

    std::string title = "<html><head><title>Hello, world!</title></head><body>\n";
    std::string head = "<h1>Hello, world!</h1>\n";
    std::string footer = "</body></html>";
    // parserObject test("test.conf");
    // CGI obj("example.com", "5050", "GET", "HTTP/1.1 200 OK", "text/html", "/path/to/location", "HTML-body", "myQuiry", "/path/to/cgi", 1024);
    CGI obj("fileCGI1.php", "POST", "HTTP/1.1 200 OK", "/path/to/location", "body", "name=hamza&age=90", 10);

    obj.executeCGI();
    int i = 0;
    char ** en = obj.getEnv();
    // std::cerr << "hello there" << std::endl;
    // while (1);
    // while (i < 9){
    //     std::cout << en[i] << std::endl;
    //     i++;
    // }
    
    // test.open_config_file();
    // int c = test.check_dup_char("hey{", '{');

    // std::cout << c << std::endl;

    return (0);
}