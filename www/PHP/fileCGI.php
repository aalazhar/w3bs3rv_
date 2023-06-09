<?php
ob_start();
set_time_limit(3);
echo $_SERVER['REQUEST_METHOD'];
if ($_SERVER['REQUEST_METHOD'] !== 'GET'){
    echo ":ERRO";
}
    if ($_SERVER['REQUEST_METHOD'] === 'GET'){
        if (isset($_GET['fname']) && isset($_GET['lname'])){

            $name = $_GET['fname'];
            $last = $_GET['lname'];
            echo "<hr>";
            echo "<h1>Hello Mrs $name</h1>";
            echo "<hr>";
            echo "<hr>";
            echo "<hr>";
            echo "<h1> You Are Welcome $last </h1>";
            echo "<hr>";
        }
        else
            echo "Your form is empty.";
    }
    else{
        echo "<html>";
        echo "<title> 405 Error: Method Not Allowed </title>";
        echo "<body>";
        echo "<div>";
        echo "<h1> 405 Method Not Allowed </h1>";
        echo "<p> the server received a request method that it does not support for the target resource </p>";
        echo "</div>";
        echo "</body>";
        echo "</html>";
        header('Status: 405 Not Allowed'); exit;
    }
?>