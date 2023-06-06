<?php
    ob_start();

    echo $_SERVER['BODY'];
    $email = "aalazhar@42.com";
    $pass = "123456";
    if (!isset($_POST['login']))
        echo "makaynch";
    if (isset($_POST['login']) && isset($_POST['email']) && isset($_POST['pass'])){
        $emailX = $_POST['email'];
        $passX = $_POST['pass'];
        $loginX = $_POST['login'];

        if ($email == $emailX && $pass == $passX){
            session_start();
            $_SESSION['login'] = $loginX;
            $_SESSION['email'] = $emailX;
            $_SESSION['pass'] = $passX;
            header ("location: welcom.php");
        }
        else {
            echo "<p>Your email or pass is invalid. Click here <a href='login.php' >to try again.</a></p>";

        }
    }
    else{
        header("location: login.php");
    }
?>

<style type="text/css">
    * {
        font-family: "Courier new";
    }
</style>
