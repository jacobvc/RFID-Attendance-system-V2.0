<?php
/* Database connection settings */
	$servername = "mysql.yourdevelopmentteam.com";
    $username = "attendanceuser";		//put your phpmyadmin username.(default is "root")
    $password = "100%AreWelcome";			//if your phpmyadmin has a password put it here.(default is "root")
    $dbname = "rfidattendance";
    
	$conn = mysqli_connect($servername, $username, $password, $dbname);
	
	if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }
?>
