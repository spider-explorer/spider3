# Pack Things.
def main [
  --full: bool # Full Pack.	
] {
  let nonzero_exit_errors = true
  echo $full
  #^false.exe
  ^true.exe
  echo $env.LAST_EXIT_CODE
  if ($env.LAST_EXIT_CODE != 0) {
    exit 1
  }
  if ($full) {
    echo "<full>"
  } else {
    echo "<not full>"
  }
  let cwd = (^pwd)
  echo $cwd
  ^killall boot2-x86_64-static
  ^sleep 3

}