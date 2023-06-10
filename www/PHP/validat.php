<?php
    ob_start();
    session_name('XXX');
    session_start();
    if ($_SERVER['REQUEST_METHOD'] == "GET"){
        echo "<h1 style='font-family: 'Courier new''>BAD REQUEST.</h1>"; 
        http_response_code(405); exit;
    }
    else{

        
        
        
        $emailX = "aalazhar@42.com";
        $passX = "123456";
        $login = $_GET['login'];
        $email = $_GET['email'];
        $pass = $_GET['pass'];
        
        if ($email === $emailX && $pass === $passX){
            $_SESSION['session'] = $_COOKIE['XXX'];
            $_SESSION['login'] = $login;
            if (isset($_SESSION['login'])){
                header("location: loginx.php");   
            }
        }
        else {
            echo "<p>Your email or password is invalid. Click here <a href='loginx.php'>to try again.</a></p>";
        }
    }

?>

<style type="text/css">
    * {
        font-family: "Courier new";
    }
</style>
