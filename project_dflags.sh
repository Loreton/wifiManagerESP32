#!/bin/bash

### file per impostare -D flags per il progetto pressControl
# viene letto dal programma piorun.sh e le prepara per il platformio.ini

declare -A LOG_LEVEL_MAP=(
      ["none"]=0
     ["error"]=1
      ["warn"]=2
   ["special"]=3
    ["notify"]=4
      ["info"]=5
     ["debug"]=6
     ["trace"]=7
)


ln_TEST=1
ln_PRODUCTION=2

ln_ESP32_WROOM_32E_MODULE=1
ln_ESP32_WROOM_32E_MODULE_2RELAY=2

myFlags=""

myFlags="${myFlags} -DLOG_LEVEL_NONE=${LOG_LEVEL_MAP["none"]}"
myFlags="${myFlags} -DLOG_LEVEL_ERROR=${LOG_LEVEL_MAP["error"]}"
myFlags="${myFlags} -DLOG_LEVEL_WARN=${LOG_LEVEL_MAP["warn"]}"
myFlags="${myFlags} -DLOG_LEVEL_SPECIAL=${LOG_LEVEL_MAP["special"]}"
myFlags="${myFlags} -DLOG_LEVEL_NOTIFY=${LOG_LEVEL_MAP["notify"]}"
myFlags="${myFlags} -DLOG_LEVEL_INFO=${LOG_LEVEL_MAP["info"]}"
myFlags="${myFlags} -DLOG_LEVEL_DEBUG=${LOG_LEVEL_MAP["debug"]}"
myFlags="${myFlags} -DLOG_LEVEL_TRACE=${LOG_LEVEL_MAP["trace"]}"


myFlags="${myFlags} -Dln_TIME_CLASS_SECONDS_VECTOR_xx"
myFlags="${myFlags} -Dln_TIME_CLASS_MINUTESS_VECTOR_xx"
myFlags="${myFlags} -Dln_TIME_CLASS_HOURS_VECTOR_xx"


myFlags="${myFlags} -Dln_TEST=${ln_TEST}"
myFlags="${myFlags} -Dln_PRODUCTION=${ln_PRODUCTION}"

myFlags="${myFlags} -Dln_ESP32_WROOM_32E_MODULE=${ln_ESP32_WROOM_32E_MODULE}"
myFlags="${myFlags} -Dln_ESP32_WROOM_32E_MODULE_2RELAY=${ln_ESP32_WROOM_32E_MODULE_2RELAY}"



#... selezione board and releas type
ln_RELEASE_TYPE=$ln_TEST
ln_ESP32_BOARD_TYPE=$ln_ESP32_WROOM_32E_MODULE


export myDFLAGS=${myFlags}
echo $myDFLAGS

# List all environment variables starting with "ln" and print them
# env | grep '^ln'


# List all environment variables whose value starts with "-D" and print them
# env | awk -F= '$2 ~ /^-D/ { print }'
