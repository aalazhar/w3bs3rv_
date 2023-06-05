<?php
    session_start();
    ob_start();
    if (isset($_SESSION['login'])){
        header("location: welcom.php");
    }
?>

<style type=""text/css">
    * {
        font-family: "Courier new";
    }
    h1 {
        text-align : center;
    }
    div {
        border: 2px solid;
        border-radius : 5px;
        position: absolute;
        border-color: #ff5000;
        top: 20%;
        left: 50%;
        transform: translate(-50%, -50%);
        padding: 10px;
    }
    /* button {

        position: absolute;
        top: 30%;
        left: 50%;

    } */

    .button{
        position: absolute;
        top: 30%;
        left: 47%;
        outline: none;
        cursor: pointer;
        font-weight: 600;
        border-radius: 3px;
        padding: 12px 24px;
        border: 0;
        color: #fff;
        background: #ff5000;
        line-height: 1.15;
        font-size: 16px;
    }
                
</style>
</br>
</br>
<h1>Form For Session</h1>
<form method="POST" action="validat.php">
    <div>
        Login  <input type="text" name="login" placeholder = "login here" size=30>
        </br>
        </br>
        Email  <input type="text" name="email" placeholder = "email here" size=30>
        </br>
        </br>
        Passw  <input type="password" name="pass" placeholder = "password here" size=30>
        </br>
        </br>
    </div>
        <input class="button" type="submit" value="login">
</form>