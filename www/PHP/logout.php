<?php
   session_start();
   session_destroy();
   header('Refresh: 3; URL = login.php');
   echo 'Wait a 3sec, redirecting you to login page...';
?>

<style type=""text/css">
    * {
        font-family: "Courier new";
    }
</style>