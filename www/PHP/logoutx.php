<?php
   session_name('XXX');
   session_start();
   session_destroy();
   $_SESSION = array();
   setcookie(session_name('XXX'), "", time() - 1, "/");
   header('Refresh: 3; URL = loginx.php');
   echo 'Wait a 3sec, redirecting you to login page...';
?>

<style type=""text/css">
    * {
        font-family: "Courier new";
    }
</style>