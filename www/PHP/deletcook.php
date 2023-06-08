<?php
// set the expiration date to one hour ago
setcookie("hammza", "ghayerha", time() - 50000000);
?>
<html>
<body>

<?php
    echo "Cookie 'user' is deleted.";
?>
</body>
</html>