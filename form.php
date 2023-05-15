<?php

    $method = $_SERVER['REQUEST_METHOD'];
    print $method;
    $name = $_POST[('name')];
    $gmail = $_POST[('gmail')];
    print"<h1>---$name</h1>";
    print"<h1>---$gmail</h1>";
    // print "<h1>$name</h1>";
    // print "<h1>$gmail</h1>";
    // if ($method === 'POST'){
        // }

?>

