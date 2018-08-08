#! /bin/bash

start_skill=$1
end_skill=$2
max_hours=$3

printf "%-5s" Skill
for h in $(seq 5 5 $max_hours); do
    printf "%5d" "$h"
done
printf "\n"
for dash in $(seq 0 $(( max_hours+5 ))); do
    printf "-"
done
printf "\n"
for s in $(seq $start_skill 5 $end_skill); do
    printf "%3s  " $s
    for h in $(seq 5 5 $max_hours); do
        printf "%5d" "$(rqt $s $h hours)"
    done
    printf "\n"
done
