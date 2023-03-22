/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:59:59 by aalazhar          #+#    #+#             */
/*   Updated: 2023/03/22 20:29:42 by aalazhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parserObject.hpp"

int main(int ac, char **av){


    parserObject test("test.conf");

    test.open_config_file();
    // int c = test.check_dup_char("hey{", '{');

    // std::cout << c << std::endl;

    return (0);
}