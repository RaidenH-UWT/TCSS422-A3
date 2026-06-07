# TCSS422 Assignment 3
## Page Table Walker

- Make sure kernel vers 5.X (`uname -r`)

### To run
- Remove old module: `sudo rmmod ./[moduleName].ko`
- Install new module: `sudo insmod ./[moduleName].ko`
- View output: `sudo tail -fn 50 /var/log/syslog`

### Specification:

Print table with columns:
- Process ID
- Process Name
- Contiguous Pages [optional]
- Non-contuguous Pages [optional]
- Total Pages

For every running process with PID > 650

Then display totals for the page columns

All output goes to the kernel log files with `printk`
