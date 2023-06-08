<?php
session_name('my_session');
session_start();
echo $_SERVER['HTTP_COOKIE'];
echo $_COOKIE['hello'];
echo $_SESSION['new'];
echo $_SESSION['session'];
?>