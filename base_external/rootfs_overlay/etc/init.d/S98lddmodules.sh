#! /bin/sh

case "$1" in
    start)
        echo "Loading module misc-modules and scull"
        modprobe hello
        /usr/bin/module_load faulty
        /usr/bin/scull_load
        ;;
    stop)
        echo "Stopping scull and misc-modules"
        rmmod hello
        /usr/bin/module_unload faulty
        /usr/bin/scull_unload
        ;;
    *)
    exit 1
esac

exit 0