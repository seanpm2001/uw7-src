/*		copyright	"%c%" 	*/

#ident	"@(#)schedule.c	1.6"
#ident "$Header$"

/*******************************************************************************
 *
 * FILENAME:    schedule.c
 *
 * DESCRIPTION: Handle Events in the Printer System
 *
 * SCCS:	schedule.c 1.6  9/10/97 at 10:07:37
 *
 * CHANGE HISTORY:
 *
 * 07-07-97  Paul Cunningham        ul97-04202 (ul95-16424)
 *           Change function ev_system() so that before scheduling a print
 *           job to a remote system it check to see if the printer is enabled.
 * 07-07-97  Paul Cunningham        ul96-24701
 *           Change function ev_system() so that when it a status check it
 *           check waht sort of remote system it is before doing it.
 * 28-08-97  Paul Cunningham        cs94-33202
 *           Change function schedule() so that for "case EV_ENABLE" if the
 *           enable fails it re-schedules another auto-enable retry. The enable
 *           would fail if the printer still has a fault on it, so we need to
 *           keep retrying until the fault has been cleared.
 * 08-09-97  Paul Cunningham        cs95-12901
 *           Change function ev_system() so that the check for "any 'moves' to 
 *           go out" (REX_MOVE) is correct.
 *           Also changed lp/cmd/lpsched/lpNet/svChild/svChild.c for this MR
 *
 *******************************************************************************
 */

#ifdef	__STDC__
#include "stdarg.h"
#else
#include "varargs.h"
#endif

#include "lpsched.h"
#include "_networkMgmt.h"

typedef struct later {
	struct later *		next;
	int			event,
				ticks;
	union arg {
		PSTATUS *		printer;
		RSTATUS *		request;
		SSTATUS *		system;
	}			arg;
}			LATER;

static LATER		LaterHead	= { 0 },
			TempHead;

#ifdef	__STDC__

static int	ev_slowf ( RSTATUS * );
static int	ev_notify ( RSTATUS * );
static void	ev_interf ( PSTATUS * );
static void	ev_system ( SSTATUS * );
static void	ev_printer ( PSTATUS * );
static void	ev_pollprinter ( PSTATUS * );
static void	_ev_pollprinter ( PSTATUS * );

static EXEC *	find_exec_slot ( EXEC *, int );

#else

static int	ev_slowf ();
static int	ev_notify ();
static void	ev_interf ();
static void	ev_system ();
static void	ev_printer ();
static void	ev_pollprinter ();
static void	_ev_pollprinter ();

static EXEC *	find_exec_slot ();

#endif

/**
 ** schedule() - SCHEDULE BY EVENT
 **/

/*VARARGS1*/
void
#ifdef	__STDC__
schedule (
	int			event,
	...
)
#else
schedule (event, va_alist)
	int			event;
	va_dcl
#endif
{
	DEFINE_FNNAME (schedule)

	va_list			ap;

	LATER *			plprev;
	LATER *			pl;
	LATER *			plnext	= 0;

#if	defined(CHECK_CHILDREN)
	int			do_ev_checkchild;
#endif

	register PSTATUS *	pps;

	register RSTATUS *	prs;

	register SSTATUS *	pss;


	ENTRYP
	/*
	 * If we're in the process of shutting down, don't
	 * schedule anything.
	 */
	if (Shutdown)
		return;

#ifdef	__STDC__
	va_start (ap, event);
#else
	va_start (ap);
#endif

	/*
	 * If we're still in the process of starting up, don't start
	 * anything! Schedule it for one tick later. While we're starting
	 * ticks aren't counted, so the events won't be started.
	 * HOWEVER, with a count of 1, a single EV_ALARM after we're
	 * finished starting will be enough to clear all things scheduled
	 * for later.
	 */
	if (Starting) {
		switch (event) {

		case EV_INTERF:
		case EV_ENABLE:
			pps = va_arg(ap, PSTATUS *);
			schedule (EV_LATER, 1, event, pps);
			goto Return;

		case EV_SLOWF:
		case EV_NOTIFY:
			prs = va_arg(ap, RSTATUS *);
			schedule (EV_LATER, 1, event, prs);
			goto Return;

		case EV_SYSTEM:
			pss = va_arg(ap, SSTATUS *);
			schedule (EV_LATER, 1, event, pss);
			goto Return;
		case EV_PRINTER:
			pps = va_arg(ap, PSTATUS *);
			schedule (EV_LATER, 1, event, pps);
			goto Return;

		case EV_LATER:
			/*
			 * This is okay--in fact it may be us!
			 */
			break;

		case EV_ALARM:
			/*
			 * The alarm will go off again, hold off for now.
			 */
			goto Return;

		}
	}

	/*
	 * Schedule something:
	 */
	switch (event) {

	case EV_INTERF:
		if ((pps = va_arg(ap, PSTATUS *)))
			ev_interf (pps);

		else
			for (
				pps = walk_ptable(1);
				pps;
				pps = walk_ptable(0)
			)
				ev_interf (pps);

		break;

	/*
	 * The EV_ENABLE event is used to get a printer going again
	 * after waiting for a fault to be cleared. We used to use
	 * just the EV_INTERF event, but this wasn't enough: For
	 * requests that can go on several different printers (e.g.
	 * queued for class, queued for ``any''), a printer is
	 * arbitrarily assigned. The EV_INTERF event just checks
	 * assignments, not possibilities, so a printer with no
	 * assigned requests but still eligible to handle one or
	 * more requests would never automatically start up again after
	 * a fault. The EV_ENABLE event calls "enable()" which eventually
	 * gets around to invoking the EV_INTERF event. However, it first
	 * calls "queue_attract()" to get an eligible request assigned
	 * so that things proceed. This also makes sense from the
	 * following standpoint: The documented method of getting a
	 * printer going, while it is waiting for auto-retry, is to
	 * manually issue the enable command!
	 *
	 * Note: "enable()" will destroy the current record of the fault,
	 * so if the fault is still with us any new alert will not include
	 * the history of each repeated fault. This is a plus and a minus,
	 * usually a minus: While a repeated fault may occasionally show
	 * a varied record, usually the same reason is given each time;
	 * before switching to EV_ENABLE we typically saw a boring, long
	 * list of identical reasons.
	 */
	case EV_ENABLE:
	{
		if ((pps = va_arg(ap, PSTATUS *)))
		{
		  /* try to re-enable the printer, but if it still has
		   * a fault the enable will fail
		   */
		  if ( enable( pps, (MESG *)0))
		  {
		    /* the enable failed, so re-schedule another auto-enable
		     */
		    schedule( EV_LATER, WHEN_PRINTER, EV_ENABLE, pps);
		  }
		}
		else
		{
			for ( pps = walk_ptable(1); pps; pps = walk_ptable(0))
			{
			  (void)enable( pps, (MESG *)0);
			}
		}
		break;
	}

	case EV_SLOWF:
		if ((prs = va_arg(ap, RSTATUS *)))
			(void)ev_slowf (prs);
		else
			for (
				prs = Request_List;
				prs && ev_slowf(prs) != -1;
				prs = prs->next
			)
				;
		break;

	case EV_NOTIFY:
		TRACEP ("case EV_NOTIFY")
		if ((prs = va_arg(ap, RSTATUS *)))
			(void)ev_notify (prs);
		else
			for (
				prs = Request_List;
				prs && ev_notify(prs) != -1;
				prs = prs->next
			)
				;
		TRACEP ("case EV_NOTIFY-break")
		break;

	case EV_SYSTEM:
		pss = va_arg(ap, SSTATUS *);
		ev_system (pss);
		break;

	case EV_PRINTER:
		pps = va_arg(ap, PSTATUS *);
		ev_printer (pps);
		break;
	case EV_LATER:
	    {
		int ticks, event;
		PSTATUS *printer;

		ticks = va_arg(ap, int);
		event = va_arg(ap, int);

		if (event == EV_POLLPRINTER) {
			printer = va_arg(ap, PSTATUS *);
			for (pl = LaterHead.next; pl; pl = pl->next) {
				if (printer == pl->arg.printer)
					break;
			}
			if (pl)
				break;
		}

		pl = (LATER *)Malloc(sizeof(LATER));

		if (!LaterHead.next)
			(void) alarm (CLOCK_TICK);

		pl->next = LaterHead.next;
		LaterHead.next = pl;

		pl->ticks = ticks;
		pl->event = event;

		switch (pl->event) {

		case EV_INTERF:
		case EV_ENABLE:
			pl->arg.printer = va_arg(ap, PSTATUS *);
			if (pl->arg.printer)
				pl->arg.printer->status |= PS_LATER;
			break;

		case EV_SLOWF:
		case EV_NOTIFY:
			pl->arg.request = va_arg(ap, RSTATUS *);
			break;

		case EV_SYSTEM:
			pl->arg.system = va_arg(ap, SSTATUS *);
			break;

		case EV_PRINTER:
			pl->arg.printer = va_arg(ap, PSTATUS *);
			break;

		case EV_POLLPRINTER:
			pl->arg.printer = printer;
			break;

#if	defined(CHECK_CHILDREN)
		case EV_CHECKCHILD:
			break;
#endif	/* CHECK_CHILDREN */

		}
		break;
	    } /* EV_LATER */

	case EV_ALARM:
		Sig_Alrm = 0;

		/*
		 * The act of scheduling some of the ``laters'' may
		 * cause new ``laters'' to be added to the list.
		 * To ease the handling of the linked list, we first
		 * run through the list and move all events ready to
		 * be scheduled to another list. Then we schedule the
		 * events off the new list. This leaves the main ``later''
		 * list ready for new events.
		 */
		TempHead.next = 0;
		for (
			pl = (plprev = &LaterHead)->next;
			pl;
			pl = plnext
		) {
			plnext = pl->next;
			if (--pl->ticks)
				plprev = pl;
			else {
				plprev->next = plnext;

				pl->next = TempHead.next;
				TempHead.next = pl;
			}
		}

		for (pl = TempHead.next; pl; pl = plnext) {
			plnext = pl->next;
			switch (pl->event) {

			case EV_INTERF:
			case EV_ENABLE:
				pl->arg.printer->status &= ~PS_LATER;
				schedule (pl->event, pl->arg.printer);
				break;

			case EV_SLOWF:
			case EV_NOTIFY:
				schedule (pl->event, pl->arg.request);
				break;

			case EV_SYSTEM:
				schedule (pl->event, pl->arg.system);
				break;

			case EV_PRINTER:
			case EV_POLLPRINTER:
				schedule (pl->event, pl->arg.printer);
				break;

#if	defined(CHECK_CHILDREN)
			case EV_CHECKCHILD:
				/*
				 * Do this one only once.
				 */
				do_ev_checkchild = 1;
				break;
#endif	/* CHECK_CHILDREN */

			}
			Free ((char *)pl);
		}

#if	defined(CHECK_CHILDREN)
		if (do_ev_checkchild)
			schedule (EV_CHECKCHILD);
#endif	/* CHECK_CHILDREN */

		if (LaterHead.next)
			(void) alarm (CLOCK_TICK);
		break;

#if	defined(CHECK_CHILDREN)
	case EV_CHECKCHILD:
		ev_checkchild ();
		break;
#endif	/* CHECK_CHILDREN */

	case EV_POLLPRINTER:
		pps = va_arg(ap, PSTATUS *);
		ev_pollprinter (pps);
		break;
	}

Return:	va_end (ap);

	return;
}

/**
 ** maybe_schedule() - MAYBE SCHEDULE SOMETHING FOR A REQUEST
 **/

void
#ifdef	__STDC__
maybe_schedule (
	RSTATUS *		prs
)
#else
maybe_schedule (prs)
	RSTATUS			*prs;
#endif
{
	DEFINE_FNNAME (maybe_schedule)

	/*
	 * Use this routine if a request has been changed by some
	 * means so that it is ready for filtering or printing,
	 * but a previous filtering or printing process for this
	 * request MAY NOT have finished yet. If a process is still
	 * running, then the cleanup of that process will cause
	 * "schedule()" to be called. Calling "schedule()" regardless
	 * might make another request slip ahead of this request.
	 */

	/*
	 * "schedule()" will refuse if this request is filtering.
	 * It will also refuse if the request ``was'' filtering
	 * but the filter was terminated in "validate_request()",
	 * because we can not have heard from the filter process
	 * yet. Also, when called with a particular request,
	 * "schedule()" won't slip another request ahead.
	 */
	if (NEEDS_FILTERING(prs))
		schedule (EV_SLOWF, prs);

	else if (!(prs->request->outcome & RS_STOPPED))
		schedule (EV_INTERF, prs->printer);

	return;
}

/**
 ** ev_interf() - CHECK AND EXEC INTERFACE PROGRAM 
 **/

/*
 * Macro to check if the request needs a print wheel or character set (S)
 * and the printer (P) has it mounted or can select it. Since the request
 * has already been approved for the printer, we don't have to check the
 * character set, just the mount. If the printer has selectable character
 * sets, there's nothing to check so the request is ready to print.
 */
#define MATCH(PRS,PPS) \
	( \
		!(PPS)->printer->daisy \
	     || !(PRS)->pwheel_name \
	     || !((PRS)->status & RSS_PWMAND) \
	     || STREQU((PRS)->pwheel_name, NAME_ANY) \
	     || ( \
			(PPS)->pwheel_name \
		     && STREQU((PPS)->pwheel_name, (PRS)->pwheel_name) \
		) \
	)

static void
#ifdef	__STDC__
ev_interf (
	PSTATUS *		pps
)
#else
ev_interf (pps)
	PSTATUS			*pps;
#endif
{
	DEFINE_FNNAME (ev_interf)

	register RSTATUS	*prs;


	/*
	 * If the printer isn't tied up doing something
	 * else, and isn't disabled, see if there is a request
	 * waiting to print on it. Note: We don't include
	 * PS_FAULTED here, because simply having a printer
	 * fault (without also being disabled) isn't sufficient
	 * to keep us from trying again. (In fact, we HAVE TO
	 * try again, to see if the fault has gone away.)
	 *
	 * NOTE: If the printer is faulted but the filter controlling
	 * the printer is waiting for the fault to clear, a
	 * request will still be attached to the printer, as
	 * evidenced by "pps->request", so we won't try to
	 * schedule another request!
	 */
	if (pps->request || pps->status & (PS_DISABLED|PS_LATER|PS_BUSY))
		return;

	BEGIN_WALK_BY_PRINTER_LOOP (prs, pps)
	    /*
	     * Just because the printer isn't busy and the
	     * request is assigned to this printer, don't get the
	     * idea that the request can't be printing (RS_ACTIVE),
	     * because another printer may still have the request
	     * attached but we've not yet heard from the child
	     * process controlling that printer.
	     */
	    if (qchk_waiting(prs))

		if (pps->status & PS_REMOTE) {
			/*
			 * We have a request waiting to go to a remote
			 * Spooler. Let "rexec()" worry about getting
			 * a connection, etc.
			 */
			if (pps->system)
				(void) rexec (pps->system, REX_INTERF, pps, prs);

		} else if (prs->form == pps->form && MATCH(prs, pps)) {
			/*
			 * We have the waiting request, we have
			 * the ready (local) printer. If the exec fails
			 * because the fork failed, schedule a
			 * try later and claim we succeeded. The
			 * later attempt will sort things out,
			 * e.g. will re-schedule if the fork fails
			 * again.
			 */
			pps->request = prs;
			if (exec(EX_INTERF, pps) == 0) {
				pps->status |= PS_BUSY;
				return;
			}
			pps->request = 0;
			if (errno == EAGAIN) {
				load_str (&pps->dis_reason, CUZ_NOFORK);
				schedule (EV_LATER, WHEN_FORK, EV_ENABLE, pps);
				return;
			}
		}
	END_WALK_LOOP

	return;
}

/**
 ** ev_slowf() - CHECK AND EXEC SLOW FILTER
 **/

static int
#ifdef	__STDC__
ev_slowf (
	RSTATUS *		prs
)
#else
ev_slowf (prs)
	register RSTATUS	*prs;
#endif
{
	DEFINE_FNNAME (ev_slowf)

	register EXEC		*ep;


	/*
	 * Return -1 if no more can be executed (no more exec slots)
	 * or if it's unwise to execute any more (fork failed).
	 */

	if (!(ep = find_exec_slot(Exec_Slow, ET_SlowSize)))
		return (-1);

	if (
		!(prs->request->outcome & (RS_DONE|RS_HELD|RS_ACTIVE))
	     && NEEDS_FILTERING(prs)
	) {
	 	(prs->exec = ep)->ex.request = prs;
		if (exec(EX_SLOWF, prs) != 0) {
			ep->ex.request = 0;
			prs->exec = 0;
			if (errno == EAGAIN) {
				schedule (EV_LATER, WHEN_FORK, EV_SLOWF, prs);
				return (-1);
			}
		}
	}
	return (0);
}

/**
 ** ev_notify() - CHECK AND EXEC NOTIFICATION
 **/

static int
#ifdef	__STDC__
ev_notify (
	RSTATUS *		prs
)
#else
ev_notify (prs)
	register RSTATUS	*prs;
#endif
{
	DEFINE_FNNAME (ev_notify)

	register EXEC		*ep;

	register SSTATUS	*pss;
		 SSTATUS	*default_s5_pss;


	ENTRYP
	/*
	 * Return -1 if no more can be executed (no more exec slots)
	 * or if it's unwise to execute any more (fork failed, already
	 * sent one to remote side).
	 */

	/*
	 * If the job came from a remote machine, we forward the
	 * outcome of the request to the network manager for sending
	 * to the remote side.
	 */
	if (prs->request->actions & ACT_NOTIFY)
	{
		pss = search_stable (prs->secure->system);
		if (!pss)
		{
			/*
			**  Here I'm making an assumption: that callbacks
			**  will only be made between s5 machines,
			**  i.e. if we notify a remote machine then
			**  that remote machine will be a s5 machine.
			*/
			if (!(default_s5_pss = default_system(S5_PROTO)))
			{
				schedlog (
					"Tried to notify unknown system (%s)\n",
					prs->secure->system
				);
				prs->request->actions &= ~ACT_NOTIFY;
				return (0);  /* but try another request */
			}

			schedlog ("Using s5 protocol to notify remote system (%s)\n",
				prs->secure->system);

			/*
			**  Manufacture a SSTATUS structure.
			*/
			pss = (SSTATUS *)Calloc(1, sizeof(SSTATUS));
			pss->system = (SYSTEM *)Calloc(1, sizeof(SYSTEM));
			pss->exec = (EXEC *)Calloc(1, sizeof(EXEC));

			*(pss->system) = *(default_s5_pss->system);

			pss->system->name = strdup(prs->secure->system);

			addone ((void ***)&SStatus, pss, &ST_Size, &ST_Count);

			/*
			**  There is no mechanism for deleting the remote system
			**  from the Systems table.  Future enhancement.
			*/
		}
		prs->status |= RSS_SENDREMOTE;
		(void) rexec (pss, REX_NOTIFY, prs);
		return (-1); /* this one went--no more for now */

	/*
	 * If the job didn't come from a remote system,
	 * we'll try to start a process to send the notification
	 * to the user. But we only allow so many notifications
	 * to run at the same time, so we may not be able to
	 * do it.
	 */
	}
	else
	if (!(ep = find_exec_slot(Exec_Notify, ET_NotifySize)))
		return (-1);
	else
	if (prs->request->outcome & RS_NOTIFY &&
	   !(prs->request->outcome & RS_NOTIFYING))
	{
	 	(prs->exec = ep)->ex.request = prs;
		if (exec(EX_NOTIFY, prs) != 0) {
			ep->ex.request = 0;
		 	prs->exec = 0;
			if (errno == EAGAIN) {
				schedule (EV_LATER, WHEN_FORK, EV_NOTIFY, prs);
				return (-1);
			}
		}
	}
	return (0);
}

/**
 ** ev_system() - CHECK AND ``EXEC'' SYSTEM
 **/

static void
#ifdef	__STDC__
ev_system (
	SSTATUS *		pss
)
#else
ev_system (pss)
	register SSTATUS	*pss;
#endif
{
	DEFINE_FNNAME (ev_system)

	RSTATUS *		prs;


	/*
	 * If the connection to the system isn't busy and
	 * we have something to send there, send it!
	 */
	if (pss->exec->flags & (EXF_WAITJOB | EXF_WAITCHILD))
		return;

	/*
	 * Note: If the remote send is successful, leave because we can
	 * send only one at a time. If the remote send is not successful,
	 * leave because another try won't help.
	 */

	/*
	 * Look for the next cancellation that has to go
	 * out to this system:
	 */
	schedlog ("ev_system: checking requests for REX_CANCEL\n");
	for (prs = Request_List; prs; prs = prs->next) {
		if (
			PRINTING_AT(prs, pss)
		     && prs->request->outcome & RS_CANCELLED
		     && prs->status & RSS_SENDREMOTE
		) {
	 		(void) rexec (pss, REX_CANCEL, prs);
			return;
		}
	}
	/* As we now support the cancelling of remote requests,
	 * which we did issue, the 'Remote_Request_List' may contain
	 * some cancel notices. */
	for (prs = Remote_Request_List; prs; prs = prs->next) {
		if (
			PRINTING_AT(prs, pss)
		     && prs->request->outcome & RS_CANCELLED
		     && prs->status & RSS_SENDREMOTE
		) {
	 		(void) rexec (pss, REX_CANCEL, prs);
			return;
		}
	}

	/* Check if there are any 'moves' to go out (cs95-12901)
	 */
	schedlog ("ev_system: checking requests for REX_MOVE\n");
	for (prs = Request_List; prs; prs = prs->next) 
	{
		if (( PRINTING_AT(prs, pss)) && 
		    (( prs->status & (RSS_MOVE|RSS_SENDREMOTE)) ==
		                                     (RSS_MOVE|RSS_SENDREMOTE)))
		{
	 		(void) rexec (pss, REX_MOVE, prs);
			return;
		}
	}

	/* Look for the next status check that has to go
	 * out to this system:
	 */
	schedlog ("ev_system: checking requests for REX_STATUS\n");
	for ( prs = Status_List; prs; prs = prs->next)
	{
		if (prs->system == pss) 
		{						/* ul96-24701 */
 		  if (( prs->system->system->protocol == S5_PROTO) ||
		      ( prs->system->system->protocol == BSD_PROTO))
		  {
	 		(void) rexec (pss, REX_STATUS, prs);
			return;
		  }
		}
	}

	/* Look for the next print request that has to go
	 * out to this system (but check the printer is not disbled):
	 */
	schedlog ("ev_system: checking requests for REX_INTERF\n");
	for (prs = Request_List; prs; prs = prs->next) 
	{							/* ul97-04202 */
		if ( PRINTING_AT( prs, pss) && qchk_waiting( prs) &&
				!( prs->printer->status & PS_DISABLED)) 
		{
	 		(void) rexec (pss, REX_INTERF, prs->printer, prs);
			return;
		}
	}

	/*
	 * Look for the next notification that has to go
	 * out to this system:
	 */
	schedlog ("ev_system: checking requests for REX_NOTIFY\n");
	for (prs = Request_List; prs; prs = prs->next) {
		if (
			ORIGINATING_AT(prs, pss)
		     && prs->request->outcome & RS_NOTIFY
		     && prs->status & RSS_SENDREMOTE
		) {
	 		(void) rexec (pss, REX_NOTIFY, prs);
			return;
		}
	}
	return;
}

static void
#ifdef	__STDC__
ev_printer (
	PSTATUS *		pps
)
#else
ev_printer (pps)
	register PSTATUS	*pps;
#endif
{
	DEFINE_FNNAME (ev_printer)

	RSTATUS *		prs;


	/*
	 * If the connection to the system isn't busy and
	 * we have something to send there, send it!
	 */
	if (pps->system->exec->flags & (EXF_WAITJOB | EXF_WAITCHILD))
		return;

	/*
	 * Look for the next status check that has to go
	 * out to this printer:
	 */
	schedlog ("ev_printer: checking requests for REX_STATUS\n");
	for (prs = Status_List; prs; prs = prs->next)
		if (prs->printer == pps) {
	 		(void) rexec (prs->system, REX_STATUS, prs);
			return;
		}
	return;
}

/**
 ** ev_pollprinter() - send out a poll to printer
 **/

static void
#ifdef	__STDC__
ev_pollprinter (PSTATUS *pps)
#else
ev_pollprinter (pps)

register
PSTATUS	* pps;
#endif
{
	int	i;

	ENTRY ("ev_system")

	if (pps)
	{
		_ev_pollprinter (pps);
	}
	else
	{
		for (pps = walk_ptable(1); pps; pps = walk_ptable(0))
		{
			if (pps->system->system->protocol == BSD_PROTO ||
			    pps->system->system->protocol == NUC_PROTO)
			{
				_ev_pollprinter (pps);
			}
		}
	}
}
static void
#ifdef	__STDC__
_ev_pollprinter (PSTATUS *pps)
#else
_ev_pollprinter (pps)

register
PSTATUS	* pps;
#endif
{
	RSTATUS *	prs;

	ENTRY ("ev_system")

	for (prs = Request_List; prs; prs = prs->next)
	{
		if (prs->printer == pps &&
    			prs->request->outcome & RS_SENT)
		{
			mkreq(pps->system, prs->printer);
			schedule (EV_PRINTER, pps);
			schedlog (
				"_ev_pollprinter: status for %s on %s\n",
				prs->printer->printer->name,
				pps->system->system->name);
			if (pps->system->system->protocol == NUC_PROTO)
			    schedule (EV_LATER, WHEN_POLLNUC, EV_POLLPRINTER, pps);
			else
			    schedule (EV_LATER, WHEN_POLLBSD, EV_POLLPRINTER, pps);
			break;
		}
	}
}

/**
 ** find_exec_slot() - FIND AVAILABLE EXEC SLOT
 **/

static EXEC *
#ifdef	__STDC__
find_exec_slot (
	EXEC *			exec_table,
	int			size
)
#else
find_exec_slot (exec_table, size)
	register EXEC		*exec_table;
	int			size;
#endif
{
	DEFINE_FNNAME (find_exec_slot)

	register EXEC *		ep;
	register EXEC *		last_ep	= exec_table + size - 1;

	for (ep = exec_table; ep <= last_ep; ep++)
		if (ep->pid == 0)
			return (ep);

	return (0);
}
