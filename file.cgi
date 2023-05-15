#!/usr/local/bin/php

    <html>
    <title>My Thank You Page</title>
    <h1>Thank you for reading this document.</h1>
<?php
if (!file_exists("fileCGI.php")) {
    header('HTTP/1.1 404 Not Found');
    exit;
}
readfile ( "fileCGI.php" );
?>
    </html>