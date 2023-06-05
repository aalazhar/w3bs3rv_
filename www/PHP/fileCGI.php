<?php
    if ($_SERVER['REQUEST_METHOD'] === 'GET'){
        $name = $_GET['fname'];
        $age = $_GET['lname'];
        echo "<h1>Hello Mrs $name</h1>";
        echo "<h1>But this age: $age is too young to watch French films!</h1>";
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