<?php
    ob_start();
    $cookie_name = "hammza";
    $cookie_value = "ghayerha";
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
?>

<!DOCTYPE html>
<html>
<body>        
<?php
    echo "Cookie is set!<br>";
    // if(!isset($_COOKIE[$cookie_name])) {
    //     echo "Cookie named '" . $cookie_name . "' is not set!";
    // } else {
    //     echo "Cookie '" . $cookie_name . "' is set!<br>";
    //     echo "Value is: " . $_COOKIE[$cookie_name];
    // }
?>

<?php
    setcookie($cookie_name, $cookie_value, time() - 3600);
    echo "Cookie is Deleted after Created";
?>
</body>
</html>