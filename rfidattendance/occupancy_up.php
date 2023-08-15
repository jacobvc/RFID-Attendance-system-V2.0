<?php
session_start();
?>

<div class=card-columns>
  <?php
  //Connect to database
  require 'connectDB.php';

  $searchQuery = " ";
  $Start_date = " ";
  $End_date = " ";
  $Start_time = " ";
  $End_time = " ";
  $Card_sel = " ";

  if (isset($_POST['log_date'])) {
    //Start date filter
    if ($_POST['date_sel_start'] != 0) {
      $Start_date = $_POST['date_sel_start'];
      $_SESSION['searchQuery'] = "checkindate='" . $Start_date . "'";
    } else {
      $Start_date = date("Y-m-d");
      $_SESSION['searchQuery'] = "checkindate='" . date("Y-m-d") . "'";
    }
    //End date filter
  
    if ($_POST['date_sel_end'] != 0) {
      $End_date = $_POST['date_sel_end'];
      $_SESSION['searchQuery'] = "checkindate BETWEEN '" . $Start_date . "' AND '" . $End_date . "'";
    }

    //Time-In filter
    if ($_POST['time_sel'] == "Time_in") {
      //Start time filter
      if ($_POST['time_sel_start'] != 0 && $_POST['time_sel_end'] == 0) {
        $Start_time = $_POST['time_sel_start'];
        $_SESSION['searchQuery'] .= " AND timein='" . $Start_time . "'";
      } elseif ($_POST['time_sel_start'] != 0 && $_POST['time_sel_end'] != 0) {
        $Start_time = $_POST['time_sel_start'];
      }
      //End time filter
  
      if ($_POST['time_sel_end'] != 0) {
        $End_time = $_POST['time_sel_end'];
        $_SESSION['searchQuery'] .= " AND timein BETWEEN '" . $Start_time . "' AND '" . $End_time . "'";
      }
    }

    //Time-out filter
    if ($_POST['time_sel'] == "Time_out") {
      //Start time filter
  
      if ($_POST['time_sel_start'] != 0 && $_POST['time_sel_end'] == 0) {
        $Start_time = $_POST['time_sel_start'];
        $_SESSION['searchQuery'] .= " AND timeout='" . $Start_time . "'";
      } elseif ($_POST['time_sel_start'] != 0 && $_POST['time_sel_end'] != 0) {
        $Start_time = $_POST['time_sel_start'];
      }
      //End time filter
  
      if ($_POST['time_sel_end'] != 0) {
        $End_time = $_POST['time_sel_end'];
        $_SESSION['searchQuery'] .= " AND timeout BETWEEN '" . $Start_time . "' AND '" . $End_time . "'";
      }
    }
    //Card filter
  
    if ($_POST['card_sel'] != 0) {
      $Card_sel = $_POST['card_sel'];
      $_SESSION['searchQuery'] .= " AND card_uid='" . $Card_sel . "'";
    }
    //Department filter
  
    if ($_POST['dev_uid'] != 0) {
      $dev_uid = $_POST['dev_uid'];
      $_SESSION['searchQuery'] .= " AND device_uid='" . $dev_uid . "'";
    }
  }

  if ($_POST['select_date'] == 1) {
    $Start_date = date("Y-m-d");
    $_SESSION['searchQuery'] = "checkindate='" . $Start_date . "'";
  }
 
  $sql = "select t4.username as uname, t4.serialnumber as unumber, t3.* from users t4  
  left join (select t1.maxid, t2.* from 
  (select t2.username as uname, max(t2.id) as maxid from users_logs t2 group by serialnumber) 
  t1, users_logs t2 where t2.id = t1.maxid) t3  on t3.serialnumber=t4.serialnumber where t4.add_card > 0 order by t4.username;
  ";
  $result = mysqli_stmt_init($conn);

  if (!mysqli_stmt_prepare($result, $sql)) {
    echo '<p class="error">SQL Error</p>';
  } else {
    mysqli_stmt_execute($result);
    $resultl = mysqli_stmt_get_result($result);

    if (mysqli_num_rows($resultl) > 0) {
      while ($row = mysqli_fetch_assoc($resultl)) {
        if ($row['id'] === NULL) {
          ?>
          <div class="card">
            <div class="card-body">
              <h4 class="card-title">
                <?php echo $row['uname'] . ' (' . $row['unumber'] . ')'; ?>
              </h4>
              <p class="card-text">Seen:never</p>
            </div>
          </div>
          <?php
        } else /* if ($row['checkindate'] == date("Y-m-d")) */ {
          ?>
          <div class="card">
            <div class="card-body
              <?php
              if ($row['checkindate'] == date("Y-m-d") && $row['card_out'] == '0') {
                echo ' present';
              } ?>">
              <h4 class="card-title">
                <?php echo $row['username'] . ' (' . $row['serialnumber'] . ')'; ?>
              </h4>
              <p class="card-text">Seen:
                <?php echo $row['checkindate']; ?>
              </p>
              <!-- <p class="card-text"><?php echo $row['timeout']; ?></p> -->
            </div>
          </div>
          <?php
        }
      }
    }
  }
  // echo $sql;
   ?>
</div>