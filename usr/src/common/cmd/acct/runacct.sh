#!/sbin/sh
#	copyright	"%c%"

#!/sbin/sh


#ident	"@(#)acct:common/cmd/acct/runacct.sh	1.9.1.9"
#ident "$Header$"
#       "nitely accounting shell, should be run from cron (adm) at 4am"
#	"does process, connect, disk, and fee accounting"
#	"prepares command summaries"
#	"shell is restartable and provides reasonable diagnostics"
_adm=/var/adm
_nite=/var/adm/acct/nite
_sum=/var/adm/acct/sum
_wtmp=/var/adm/wtmp
export PATH
PATH=/usr/lib/acct:/usr/bin:/usr/sbin
_statefile=${_nite}/statefile
_active=${_nite}/active
_lastdate=${_nite}/lastdate
_errormsg="\n\n************ ACCT ERRORS : see  ${_active}${_date}********\n\n"
_MIN_BLKS=500

cd ${_adm}
#		"make sure that 2 crons weren't started, or leftover problems"
date  > ${_nite}/lock1
chmod 400 ${_nite}/lock1
ln ${_nite}/lock1 ${_nite}/lock
if test $? -ne 0; then
	_lnkerr="\n\n*********** 2 CRONS or ACCT PROBLEMS***********\n\n\n"
	(date ; echo "$_lnkerr" ) >/dev/console
	echo "$_lnkerr" | mail adm root
	echo "ERROR: locks found, run aborted" >> ${_active}
	rm -f ${_nite}/lock*
	exit 1
fi

# Check to see if there is enough space in /var to do nitely accounting
#
_blocks=`df /var | sed "s/.*:  *\([0-9][0-9]*\) blocks.*/\1/"`
if [ "$_blocks" -le $_MIN_BLKS ];then
	echo "runacct: Insufficient space in /var ($_blocks blks); \c"
	echo "Terminating procedure"
	( echo "runacct: Insufficient space in /var ($_blocks blks); \c"
	echo "Terminating procedure" ) | \
		 tee /dev/console ${_active} | mail root adm
	rm -f ${_nite}/lock*
	exit 1
fi


case $# in
0)
#	"as called by the cron each day"
	_date="`date +%m%d`"
	if test ! -r ${_lastdate} ; then
		nulladm ${_lastdate}
		echo "0000" > ${_lastdate}
	fi
	if test "${_date}" = "`cat ${_lastdate}`"; then
		(date; echo "${_errormsg}") > /dev/console
		echo "${_errormsg}" | mail root adm
		echo "ERROR: acctg already run for `date`: check ${_lastdate}" >> ${_active}
		rm -f ${_nite}/lock*
		mv ${_active} ${_active}${_date}
		exit 1
	fi
	echo ${_date} > ${_lastdate}
	nulladm ${_statefile}
	echo "SETUP" > ${_statefile}
	nulladm ${_active}
	echo ${_date} > ${_active}    # debuging
	echo "\n\n\n\n\n**********  SYSTEM ACCOUNTING STARTED `date`  **********\n\n\n\n\n" > /dev/console
	echo ${_date} > ${_active}    # debuging
	;;

1)
#	"runacct MMDD  (date)  will restart at current state"
	_date=$1
	echo "restarting acctg for ${_date} at `cat ${_statefile}`" >> ${_active}
	echo "\n\n\n\n\n********** SYSTEM ACCOUNTING RESTARTED `date` **********\n\n\n\n\n" > /dev/console
	;;

2)
#	"runacct MMDD STATE  restart at specified state"
	_date=$1
	echo "restarting acctg for ${_date} at $2" >> ${_active}
	echo "previous state was `cat ${_statefile}`" >> ${_active}
	echo "$2" > ${_statefile}
	echo "\n\n\n\n\n********** SYSTEM ACCOUNTING RESTARTED `date` **********\n\n\n\n\n" > /dev/console
	;;
*)
	(date; echo "${_errormsg}") > /dev/console
	echo "${_errormsg}" | mail root adm
	echo "ERROR: runacct called with invalid arguments" > ${_active}
	rm -f ${_nite}/lock*
	mv ${_active} ${_active}${_date}
	exit 1
	;;
esac

if test ! -r ${_sum}/rprt${_date} ; then
	nulladm ${_sum}/rprt${_date}
fi
if test ! -r ${_nite}/daytacct ; then
	nulladm ${_nite}/daytacct
fi

#	"processing is broken down into seperate, restartable states"
#	"the statefile is updated at the end of each state so that the"
#	"next loop through the while statement switches to the next state"
while [ 1 ]
do
case "`cat ${_statefile}`" in
SETUP)

cd ${_adm}

(date ; ls -l fee pacct* ${_wtmp}* ) >> ${_active}

#	"switch current pacct file"
turnacct switch
_rc=$?
if test ${_rc} -ne 0; then
	(date ; echo "${_errormsg}" ) > /dev/console
	echo "${_errormsg}" | mail root adm
	echo "ERROR: turnacct switch returned rc=${_rc}" >> ${_active}
	rm -f ${_nite}/lock*
	mv ${_active} ${_active}${_date}
	exit 1
fi

#	" give pacct files unique names for easy restart "
for _i in pacct?*
do
	if test -r S${_i}.${_date} ; then
		 (date ; echo "${_errormsg}" ) > /dev/console
		echo "${_errormsg}" | mail root adm
		echo "ERROR: S${_i}.${_date} already exists" >> ${_active}
		echo "file setups probably already run" >> ${_active}
		rm -f ${_nite}/lock*
		mv ${_active} ${_active}${_date}
		exit 1
	fi
	mv ${_i} S${_i}.${_date}
done


#	"add current time on end"
if test -r ${_nite}/wtmp.${_date} ; then
	(date ; echo "${_errormsg}" ) > /dev/console
	echo "${_errormsg}" | mail root adm
	echo "ERROR: ${_nite}/wtmp.${_date} already exists: run setup manually" > ${_active}
	rm -f ${_nite}/lock*
	mv ${_active} ${_active}${_date}
	exit 1
	nulladm ${_nite}/wtmp.${_date}
fi
closewtmp	# fudge a DEAD_PROCESS for /var/adm/wtmp
cp ${_wtmp} ${_nite}/wtmp.${_date}
acctwtmp "runacct" >> ${_nite}/wtmp.${_date}
nulladm ${_wtmp}
acctwtmp "runacct" >> ${_wtmp}
utmp2wtmp	# fudge active user from utmp to wtmp

echo "files setups complete" >> ${_active}
echo "WTMPFIX" > ${_statefile}
;;

WTMPFIX)
#	"verify the integrity of the wtmp file"
#	"wtmpfix will automatically fix date changes"
cd ${_nite}
nulladm tmpwtmp wtmperror
wtmpfix < wtmp.${_date} > tmpwtmp 2>wtmperror
if test $? -ne 0 ; then
	(date ; echo "${_errormsg}") | mail root adm
	echo "${_errormsg}" > /dev/console
	echo "ERROR: wtmpfix errors see ${_nite}/wtmperror${_date}" >> ${_active}
	rm -f ${_nite}/lock*
	mv ${_active} ${_active}${_date}
	mv wtmperror wtmperror${_date}
	exit 1
fi

echo "wtmp processing complete" >> ${_active}
echo "CONNECT" > ${_statefile}
;;


CONNECT)
#	"produce connect records"
#	"the lineuse and reboots files are used by prdaily"
cd ${_nite}
nulladm lineuse reboots log ctacct.${_date}
acctcon -l lineuse -o reboots < tmpwtmp  2> log > ctacct.${_date}
_rc=$?
if test -s log ; then 
	fmtmsg -u console "`echo "${_errormsg}"`"
	cat ${_nite}/log >> ${_active}
	(date ; cat ${_nite}/log) | mail adm root
	if test ${_rc} -eq 0 ; then
		# if this test is true, then pnpsplit complained about
		# the year and holidays not being up to date.
		# This used to be a fatal error, but now it will
		# continue to process the accounting.
		fmtmsg -u console "     holiday file error in acctcon"
	fi
	if test ${_rc} -eq 4 ; then
		# acctcon ran out of room to account for all users
		# accounting completed for users we had room for
		# nonfatal error
		fmtmsg -u console "     connect accounting incomplete"
	fi
	if test ${_rc} -eq 2 ; then
		# bad records, types or times encountered
		# nonfatal error
		fmtmsg -u console "     bad records encountered by acctcon"
	fi
	if test ${_rc} -eq 3 ; then
		# could not complete connect acctg due to not enough
		# room to store tty line names
		# fatal error
		fmtmsg -u console "Terminating procedure"
		rm -f ${_nite}/lock*
		mv ${_active} ${_active}${_date}
		exit 3
	fi
fi

echo "connect acctg complete" >> ${_active}
echo "PROCESS" > ${_statefile}
;;


PROCESS)
#	"correlate Spacct and ptacct files by number"
#	"will not process Spacct file if corresponding ptacct exists"
#	"remove the ptacct file to rurun the Spacct file"
#	"if death occurs here, rerunacct should remove last ptacct file"

cd ${_nite}
for _Spacct in ${_adm}/Spacct*.${_date}
do
	_ptacct=`basename ${_Spacct} | sed 's/Sp/pt/'`
	if test -s ${_ptacct}; then
		echo "WARNING: accounting already run for ${_Spacct}" \
			>> ${_active}
		echo "WARNING: remove ${_nite}/${_ptacct} to rerun" \
			>> ${_active}
	else
		nulladm ${_ptacct}
		acctprc < ${_Spacct} > ${_ptacct}
	
		echo "process acctg complete for ${_Spacct}" >> ${_active}
	fi
done
echo "all process actg complete for ${_date}" >> ${_active}
echo "MERGE" > ${_statefile}
;;


MERGE)
cd ${_nite}
#	"merge ctacct and ptacct files together"
acctmerg ptacct*.${_date} < ctacct.${_date} > daytacct

echo "tacct merge to create daytacct complete" >> ${_active}
echo "FEES" > ${_statefile}
;;


FEES)
cd ${_nite}
#	"merge in fees"
if test -s ${_adm}/fee; then
	cp daytacct tmpdayt
	sort +0n +2 ${_adm}/fee | acctmerg -i | acctmerg tmpdayt  > daytacct
	echo "merged fees" >> ${_active}
	rm -f tmpdayt
else
	echo "no fees" >> ${_active}
fi
echo "DISK" > ${_statefile}
;;


DISK)
cd ${_nite}
#	"the last act of any disk acct procedure should be to mv its"
#	"entire output file to disktacct, where it will be picked up"
if test -r disktacct; then
	cp daytacct tmpdayt
	acctmerg disktacct  < tmpdayt > daytacct
	echo "merged disk records" >> ${_active}
	rm -f tmpdayt
	mv disktacct /tmp/disktacct.${_date}
else
	echo "no disk records" >> ${_active}
fi
echo "MERGETACCT" > ${_statefile}
;;

MERGETACCT)
cd ${_adm}/acct
#	"save each days tacct file in sum/tacct.${_date}"
#	"if sum/tacct gets corrupted or lost, could recreate easily"
#	"the monthly acctg procedure should remove all sum/tacct files"
nulladm sum/tacct${_date}
cp nite/daytacct sum/tacct${_date}
if test ! -r sum/tacct; then
	echo "WARNING: recreating ${_adm}/sum/tacct " >> ${_active}
	nulladm sum/tacct sum/tacctprev
fi

#	"merge in todays tacct with the summary tacct"
cp sum/tacct sum/tacctprev
if test -s sum/tacctprev
then	
	acctmerg sum/tacctprev  <sum/tacct${_date} >sum/tacct
else
	acctmerg <sum/tacct${_date} >sum/tacct
fi

echo "updated sum/tacct" >> ${_active}
echo "CMS" > ${_statefile}
;;


CMS)
cd ${_adm}/acct
#	"do command summaries"
nulladm sum/daycms
if test ! -r sum/cms; then
	nulladm sum/cms sum/cmsprev nite/daycms nite/cms
	echo "WARNING: recreating ${_adm}/sum/cms " >> ${_active}
fi
cp sum/cms sum/cmsprev
acctcms ${_adm}/Spacct*.${_date}  > sum/daycms
acctcms -s sum/daycms sum/cmsprev  > sum/cms
acctcms -a -s sum/daycms | sed -n 1,56p  > nite/daycms
acctcms -a -s sum/cms | sed -n 1,56p  > nite/cms
lastlogin 
echo "command summaries complete" >> ${_active}
echo "USEREXIT" > ${_statefile}
;;


USEREXIT)
#	"any installation dependant accounting programs should be run here"
[ -s /usr/lib/acct/runacct.local ] && /usr/lib/acct/runacct.local

echo "CLEANUP" > ${_statefile}
;;


CLEANUP)
cd ${_adm}/acct
#	" finally clear files; could be done next morning if desired"
nulladm ${_adm}/fee
rm -f ${_adm}/Spacct*.${_date}
#	"put reports onto a file"
prdaily >> sum/rprt${_date};
rm -f nite/lock*
rm -f nite/ptacct*.${_date} nite/ctacct.${_date}
mv nite/wtmp.${_date} nite/owtmp
rm -f nite/wtmperror${_date} nite/active${_date} nite/tmpwtmp
echo "system accounting completed at `date`" >> ${_active}
mv ${_active} ${_active}${_date}
echo "********** SYSTEM ACCOUNTING COMPLETED `date` **********" > /dev/console
echo "COMPLETE" > ${_statefile}
exit 0
;;

*)
	(date;echo "${_errormsg}") > /dev/console
	echo "${_errormsg}" | mail adm root
	echo "ERROR: invalid state, check ${_statefile}" >> ${_active}
	rm -f ${_nite}/lock*
	mv ${_active} ${_active}${_date}
	exit 1
	;;
esac
done


#	" runacct is normally called with no arguments from the cron"
#	" it checks its own locks to make sure that 2 crons or previous"
#	" problems have not occured"

#	" runacct uses the statefile to record its progress"
#	" each state updates the statefile upon completion"
#	" then the next loop though the while picks up the new state"

#	" to restart this shell,  check the active file for diagnostics"
#	" fix up any corrupted data (ie. bad pacct or wtmp files)"
#	" if runacct detected the error it removes the locks"
#	" remove the locks if necessary, otherwise runacct will complain"
#	" the lastdate file should be removed or changed"
#	" restart runacct at current state with:  runacct MMDD"
#	" to override the statefile: runacct MMDD STATE"


#	" if runacct has been executed after the latest failure"
#	" ie. it ran ok today but failed yesterday"
#	" the statefile will not be correct"
#	" check the active files and restart properly"

#	" if runacct failed in the PROCESS state, remove the last"
#	" ptacct file because it may not be complete"

#	" if shell has failed several days, do SETUP manually"
#	" then rerun runacct once for each day failed"
#	" could use fwtmp here to split up wtmp file correctly"

#	" normally not a good idea to restart the SETUP state"
#	" should be done manually, or just cleanup first"


#	" FILE USAGE:	all files in /var/adm/acct/nite unless specified"

#	" statefile	records progess of runacct"
#	" lastdate	last day runacct ran in date +%m%d format"
#	" lock lock1	controls serial use of runacct"
#	" active	place for all descriptive and error messages"
#	" fd2log	fd2 output for runacct ( see cron entry ) "
#	" wtmp.MMDD owtmp yesterdays wtmp file"
#	" tmpwtmp	yesterdays wtmp corrected by wtmpfix"
#	" wtmperror	place for wtmpfix error messages"
#	" lineuse	lineusage report used in prdaily"
#	" reboots	reboots report used in prdaily"
#	" log		place for error messages from acctcon1"
#	" ctacct.MMDD	connect tacct records for MMDD"
#	" ptacct.n.MMDD	process tacct records n files for MMDD"
#	" daytacct	total tacct records for this days accounting"
#	" disktacct	disk tacct records produced by disk shell"
#	" daycms	ascii daily command summary used by prdaily"
#	" cms		acsii total command summary used by prdaily"

#	" following files in /var/adm directory"

#	" fee		output from chargefee program"
#	" pacct		active pacct file"
#	" pacctn	switched pacct files"
#	" Spacctn.MMDD	pacct files for MMDD after SETUP state"
#	" wtmp		active wtmp file"

#	" following files in /var/adm/acct/sum"

#	" loginlog	output of lastlogin used in prdaily"
#	" tacct		total tacct file for current fiscal"
#	" tacct.MMDD	tacct file for day MMDD"
#	" cms		total cms file for current fiscal"
#	" rprt.MMDD	output of prdaily program"
#	" wtmp.MMDD	saved copy of wtmp for MMDD"
#	" pacct.MMDD	concatenated version of all pacct files for MMDD"
#	" cmsprev	total cms file without latest update"
#	" tacctprev	total tacct file without latest update"
#	" daycms	cms files for todays usage"
