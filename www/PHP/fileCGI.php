<?php
    if ($_SERVER['REQUEST_METHOD'] === 'GET'){
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
    else if ($_SERVER['REQUEST_METHOD'] === 'POST'){
        $name = $_POST['fname'];
        $age = $_POST['lname'];
        echo "<h1>$name</h1>";
        echo "<h1>$age</h1>";
    }
    else{
        echo "<h1>PROBLEEEEEEEEM</h1>";
    }
?>