static void
timed_wait(long sec,long usec,long early_usec) {
    fd_set mt;
    struct timeval timeout;
    int rc;

    FD_ZERO(&mt);
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    do  {
        rc = select(0,&mt,&mt,&mt,&timeout);
        if ( !timeout.tv_sec && timeout.tv_usec < early_usec )
            return;     /* Wait is good enough, exit */
    } while ( rc < 0 && timeout.tv_sec && timeout.tv_usec );
}
