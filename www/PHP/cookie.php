<?php
    ob_start();
    session_name('my_session');
    session_start();
    $cookie_name = "hello";
    $cookie_value = "motherfucker";
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
    setcookie("opop", "zaza", time() + (86400 * 30), "/"); // 86400 = 1 day
    setcookie("sss", "nnn", time() + (86400 * 30), "/"); // 86400 = 1 day
?>

<!DOCTYPE html>
<html>
<body>        
<?php
    echo "Cookie is set!<br>";
    echo $_SERVER['HTTP_COOKIE'];
    $_SESSION['session'] = $_COOKIE['my_session'];
    $_SESSION['new'] = "New_session";
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