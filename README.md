# every

    every 10s date

Run a command periodically in a given interval.

## Examples

- print date every 5 seconds:  
  `every 5s date`
- print 42 every minute:  
  `every 1m echo 42`
- list contents of current directory every second:
  `every 1s bash -c "clear && ls -lah"`

## Build

    cmake -B build
    cmake --build build
    cmake --install build

## Command line arguments

    every <interval> <command> [<args> ...]

- interval:  
  Interval to run command periodically.
- command:  
  Command to run.

Interval suffix:
- s: seconds _(default)_
- m: minutes
- h: hours
- d: days
