#!/bin/bash
# filename: SerialTiming.sh
# Description: To sync the RTC chip on Arduino, which needs correspondent program.
# Author: Weihong Guan (@aGuegu)
# blog: http://aguegu.net

port=${1:-"/dev/ttyACM0"}
baud=${2:-"9600"}

stty -F $port $baud cs8 -cstopb -parenb cread clocal
exec 3> $port
sleep 2

date +"[%T %D %w]" >$port 
printf "mcu reply: \e[1;34m"
sleep 2
head $port -n 2
echo -e "\e[0mdone."

exec 3>&-
