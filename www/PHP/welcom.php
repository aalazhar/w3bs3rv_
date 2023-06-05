<?php
    session_start();
    if (isset($_SESSION['login'])){
        $login = $_SESSION['login'];
        echo "<h1>Hello Mr $login.</h1>"; 
        echo "<h2>You are loged in now succefully.</h2>";
        echo "If you want to logout, <a href='logout.php'>click here.</a>";
    }
    else {
        header("location: login.php");
    }
?>
<style type=""text/css">
    * {
        font-family: "Courier new";
    }
</style>