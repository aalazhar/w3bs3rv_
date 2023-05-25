<?php
    // header("Location: /index.html");
    // $name = $_SERVER['QUERY_STRING'];
    // $new_query_string = $name . '&ages=30' . '&name=argg' . '&aged=999';
    // parse_str($new_query_string, $params);
    // $name_value = $params['name'];
    // $ages_value = $params['ages'];
    // $aged_value = $params['aged'];
    // $age_value = $params['age'];
    // header('Location: ' . $_SERVER['PHP_SELF'] . '?' . $new_query_string);
    
    // Output the value of the "name" parameter in an HTML page
    // echo "<html><head><title>My Page</title></head><body>";
    // echo "<h1>Hello, $name_value!</h1>";
    // echo "<h1>Hello, $ages_value!</h1>";
    // echo "<h1>Hello, $aged_value!</h1>";
    // echo "<h1>Hello, $age_value!</h1>";
    // echo "</body></html>";
    $method1 = $_SERVER['REQUEST_METHOD'];
    // readfile ( "fileCGI1.php" );

    // if ($_SERVER['REQUEST_METHOD'] != 'GET' && $_SERVER['REQUEST_METHOD'] != 'POST') {
    //     header('HTTP/1.1 405 Method Not Allowed');
    //     print '<h1>METHOD NOT ALLOWED MDF !!!</h1>';

    //     exit;
    // }

    // if (!isset($_POST['fname']) || !isset($_POST['lname'])) {
    //     header('HTTP/1.1 400 Bad Request');
    //     print"<h1>BAD REQUEST MDF !!!</h1>";

    //     exit;
    // }
    // if (!file_exists("fileCGI.php")) {
    //     header('HTTP/1.1 404 Not Found');
    //     exit;
    // }

    // echo "<html>";
    // echo "<head>";
    // echo "<title>HTML Page with Images</title>";
    // echo "</head>";
    // echo "<body>";
    // echo "<h1>HTML Page with Images</h1>";
    // echo "<img src=\"./warning.png\" alt=\"Image 1\">";
    // echo "</body>";
    // echo "</html>";
    // echo "<!DOCTYPE html>";
    // echo "<html>";
    // echo "<head>";
    // echo "<title>NO FUCK</title>";
    // echo "</head>";
    // echo "<body>";

    // echo "<img src=\"./warning.png\" alt=Image />";  
    // echo "</body>";
    // echo "</html>";
    
    if ($method1 === 'GET'){
        
        print $method1;
        $name = $_GET['fname'];
        $age = $_GET[('age')];
        $image = '';
        $path = '';
        echo"<h1>Hello Mrs $name </h1>";
        echo"<h1>But this  age : $age, is too young to watch porn !!!! . </h1>";

        exit;
    }
    // elseif ($method1 === 'POST'){
    //     print $method1;
    //     $name = $_POST['fname'];
    //     $age = $_POST[('lname')];
    //     print"<h1>$name</h1>";
    //     print"<h1>$age</h1>";
    //     exit;
    // }
    
    // exit;
?>
<!DOCTYPE html>
<html>
    <head>
        <style>
            body {background-color: powderblue;}
            h1   {color: blue;}
            p    {color: red;}
            </style>
    </head>
    <body>
        
        <h1>HELLO WORLD !!! SERVER MDF</h1>
        <h3>ERROR 404 Ya ZEBI : Page Not found MDF .</h3><h1>HELLO WORLD !!! SERVER MDF</h1>
        <h3>ERROR 404 Ya ZEBI : Page Not found MDF .</h3><h1>HELLO WORLD !!! SERVER MDF</h1>
        <h3>ERROR 404 Ya ZEBI : Page Not found MDF .</h3><h1>HELLO WORLD !!! SERVER MDF</h1>
    <h3>ERROR 404 Ya ZEBI : Page Not found MDF .</h3><h1>HELLO WORLD !!! SERVER MDF</h1>


    
    
<!</body>
</html>

