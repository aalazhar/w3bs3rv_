<?php
// set the expiration date to one hour ago
setcookie("hello", "motherfucker", time() - (86400 * 30));
?>
<html>
<body>

<?php
    echo "<hr>";
    echo "<hr>";
    echo "<h1>Cookie is deleted<h1>";
    echo "<hr>";
    echo "<hr>";
?>
</body>
</html>