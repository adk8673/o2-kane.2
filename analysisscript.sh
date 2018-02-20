#!/bin/bash

files="*.log"
masterlog="master.log"
sleepTotal=0
localwritecount=0
totalwritecount=0
localreadcount=0
totalreadcount=0
localcritical=0
totalcritical=0
localsleepcount=0
numFiles=0
sleepRegex="Sleep.+([0-9]+)$"
writeRegex="Write"
readRegex="Read"
criticalRegex="Attempt"

for f in $files ; 
do
	localsleepcount=0
	localreadcount=0
	localwritecount=0
	localcritical=0
	if [[ $f != $masterlog ]]
	then
		echo $f
		let "numFiles++"
		while read line; do
			if [[ $line =~ $sleepRegex ]] 
			then
				matchvalue="${BASH_REMATCH[1]}"
				localsleepcount=$(($localsleepcount+$matchvalue))
			fi
		done <$f
		echo Total sleep time for child: $localsleepcount seconds
		sleepTotal=$(($sleepTotal+$localsleepcount))
		if [[ $f != "prod.log" ]]
		then
			while read line; do
				if [[ $line =~ $readRegex ]]
				then
					localreadcount=$(($localreadcount+1))	
				fi
				if [[ $line =~ $criticalRegex ]]
				then
					localcritical=$(($localcritical+1))
				fi
			done<$f
			totalreadcount=$(($totalreadcount+$localreadcount))
			totalcritical=$(($totalcritical+$localcritical))
			echo Total reads for consumer child: $localreadcount
			echo Total attempts to enter critical section for consumer child: $localcritical
		else
			while read line; do
				if [[ $line =~ $writeRegex ]]
				then
					localwritecount=$(($localwritecount+1))	
				fi
				if [[ $line =~ $criticalRegex ]]
				then
					localcritical=$(($localcritical+1))
				fi
			done<$f
			totalwritecount=$(($totalwritecount+$localwritecount))
			totalcritical=$(($totalcritical+$localcritical))
			echo Total writes for producer child: $localwritecount
			echo Total attempts to enter critical section for producer child: $localcritical
		fi
		echo
	fi
done

echo
echo Overal statistics:
echo Total child log files: $numFiles
echo Average sleep time for all children: $(($sleepTotal/$numFiles)) 
echo Average critical section accesses for all children: $(($totalcritical/$numFiles))
echo Average consumer child number of reads: $(($totalreadcount/($numFiles-1)))
