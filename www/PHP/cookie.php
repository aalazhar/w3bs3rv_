<?php
    ob_start();
    $cookie_name = "samaykum";
    $cookie_value = "alaykumsamay";
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
?>

<!DOCTYPE html>
<html>
<body>        
<?php
    echo "<hr>";
    echo "<hr>";
    echo "<h1>Cookie is set!<h1>";
    echo "<hr>";
    echo "<hr>";
?>