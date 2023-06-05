<?php
$uploaddir = '../../UPLOADS/';
$uploadfile = $uploaddir . basename($_FILES['file']['name']);
echo $_FILES['file']['name'];
if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadfile)) {
    echo " file is valid, and was successfully uploaded.\n";
} else {
    echo "Coulldn't open this file\n";
}
?>