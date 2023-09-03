<?php  
//Connect to database
require 'connectDB.php';
date_default_timezone_set('America/Chicago');
$d = date("Y-m-d");
$t = date("H:i:s");

if (isset($_GET['card_uid']) && isset($_GET['device_token'])) {
    $card_uid = $_GET['card_uid'];
    $device_uid = $_GET['device_token'];

    $sql = "SELECT * FROM devices WHERE device_uid=?";
    $result = mysqli_stmt_init($conn);
    if (!mysqli_stmt_prepare($result, $sql)) {
        echo "SQL_Error_Select_device";
        exit();
    }
    mysqli_stmt_bind_param($result, "s", $device_uid);
    mysqli_stmt_execute($result);
    $resultl = mysqli_stmt_get_result($result);
    if ($row = mysqli_fetch_assoc($resultl)){
        // Recognized scanner device, get mode and department
        $device_mode = $row['device_mode'];
        $device_dep = $row['device_dep'];
        if ($device_mode == 1) {
            // Attendance mode
            $sql = "SELECT * FROM users WHERE card_uid=?";
            $result = mysqli_stmt_init($conn);
            if (!mysqli_stmt_prepare($result, $sql)) {
                echo "SQL_Error_Select_card";
                exit();
            }
            mysqli_stmt_bind_param($result, "s", $card_uid);
            mysqli_stmt_execute($result);
            $resultl = mysqli_stmt_get_result($result);
            if ($row = mysqli_fetch_assoc($resultl)){
                // The card is associated with a user
                if ($row['add_card'] == 1) {
                    // Card is registered
                    if ($row['device_uid'] == $device_uid || $row['device_uid'] == 0){
                        // OK to scan from this device
                        $Uname = $row['username'];
                        $Number = $row['serialnumber'];
                        // Is this user checked in?
                        $sql = "SELECT * FROM users_logs WHERE card_uid=? AND checkindate=? AND card_out=0";
                        $result = mysqli_stmt_init($conn);
                        if (!mysqli_stmt_prepare($result, $sql)) {
                            echo "SQL_Error_Select_logs";
                            exit();
                        }
                        mysqli_stmt_bind_param($result, "ss", $card_uid, $d);
                        mysqli_stmt_execute($result);
                        $resultl = mysqli_stmt_get_result($result);
                        if (!$row = mysqli_fetch_assoc($resultl)){
                            // Not checked in. Do so
                            $sql = "INSERT INTO users_logs (username, serialnumber, card_uid, device_uid, device_dep, checkindate, timein, timeout) VALUES (? ,?, ?, ?, ?, ?, ?, ?)";
                            $result = mysqli_stmt_init($conn);
                            if (!mysqli_stmt_prepare($result, $sql)) {
                                echo "SQL_Error_Select_login1";
                                exit();
                            }
                            $timeout = "00:00:00";
                            mysqli_stmt_bind_param($result, "sdssssss", $Uname, $Number, $card_uid, $device_uid, $device_dep, $d, $t, $timeout);
                            if (mysqli_stmt_execute($result)) {
                                echo "login".$Uname;
                            }
                            else {
                                echo "Error: " . $result->error;
                            }
                            exit();
                        }
                        else{
                            // Checked in now -  Check out 
                            $sql="UPDATE users_logs SET timeout=?, card_out=1 WHERE card_uid=? AND checkindate=? AND card_out=0";
                            $result = mysqli_stmt_init($conn);
                            if (!mysqli_stmt_prepare($result, $sql)) {
                                echo "SQL_Error_insert_logout1";
                                exit();
                            }
                            mysqli_stmt_bind_param($result, "sss", $t, $card_uid, $d);
                            mysqli_stmt_execute($result);

                            echo "logout".$Uname;
                            exit();
                        }
                    }
                    else {
                        // NOT OK to scan from this device
                        echo "Not Allowed!";
                        exit();
                    }
                }
                else if ($row['add_card'] == 0){
                    // Card is registered
                    echo "Not registerd!";
                    exit();
                }
            }
            else{
                // The card is NOT associated with a user
                echo "Not found!";
                exit();
            }
        }
        else if ($device_mode == 0) {
            // Registration mode
            $sql = "SELECT * FROM users WHERE card_uid=?";
            $result = mysqli_stmt_init($conn);
            if (!mysqli_stmt_prepare($result, $sql)) {
                echo "SQL_Error_Select_card";
                exit();
            }
            mysqli_stmt_bind_param($result, "s", $card_uid);
            mysqli_stmt_execute($result);
            $resultl = mysqli_stmt_get_result($result);
            if ($row = mysqli_fetch_assoc($resultl)){
                //The Card is associated with a user
                $sql = "SELECT card_select FROM users WHERE card_select=1";
                $result = mysqli_stmt_init($conn);
                if (!mysqli_stmt_prepare($result, $sql)) {
                    echo "SQL_Error_Select";
                    exit();
                }
                mysqli_stmt_execute($result);
                $resultl = mysqli_stmt_get_result($result);
                
                if ($row = mysqli_fetch_assoc($resultl)) {
                    // Clear all select states
                    $sql="UPDATE users SET card_select=0";
                    $result = mysqli_stmt_init($conn);
                    if (!mysqli_stmt_prepare($result, $sql)) {
                        echo "SQL_Error_insert";
                        exit();
                    }
                    mysqli_stmt_execute($result);
                    // Set select true for this UID
                    $sql="UPDATE users SET card_select=1 WHERE card_uid=?";
                    $result = mysqli_stmt_init($conn);
                    if (!mysqli_stmt_prepare($result, $sql)) {
                        echo "SQL_Error_insert_An_available_card";
                        exit();
                    }
                    else{
                        mysqli_stmt_bind_param($result, "s", $card_uid);
                        mysqli_stmt_execute($result);

                        echo "available";
                        exit();
                    }
                }
                else{
                    // Set select true for this UID
                    $sql="UPDATE users SET card_select=1 WHERE card_uid=?";
                    $result = mysqli_stmt_init($conn);
                    if (!mysqli_stmt_prepare($result, $sql)) {
                        echo "SQL_Error_insert_An_available_card";
                        exit();
                    }
                    mysqli_stmt_bind_param($result, "s", $card_uid);
                    mysqli_stmt_execute($result);

                    echo "available";
                    exit();
                }
            }
            else{
                //The Card is NOT associated with a user
                // Clear all select states
                $sql="UPDATE users SET card_select=0";
                $result = mysqli_stmt_init($conn);
                if (!mysqli_stmt_prepare($result, $sql)) {
                    echo "SQL_Error_insert";
                    exit();
                }
                mysqli_stmt_execute($result);
                // Insert new user record with select = true
                $sql = "INSERT INTO users (card_uid, card_select, device_uid, device_dep, user_date) VALUES (?, 1, ?, ?, CURDATE())";
                $result = mysqli_stmt_init($conn);
                if (!mysqli_stmt_prepare($result, $sql)) {
                    echo "SQL_Error_Select_add";
                    exit();
                }
                mysqli_stmt_bind_param($result, "sss", $card_uid, $device_uid, $device_dep );
                mysqli_stmt_execute($result);

                echo "succesful";
                exit();
            }    
        }
    }
    else{
        // Unrecognized scanner device
        echo "Invalid Device!";
        exit();
    }
}
?>
