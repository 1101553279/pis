
                            link two trains
    rec       uic               |
                                |
    dacu                dacu    |   dacu    ....
                                |
    pecu ......                 |
                                |
content:
1>>. intercom between passenger and driver
2>>. intercom between two driver
3>>. broadcast by driver
4>>. broadcast listenning

abbre:
    p -- passenger / pecu
    d -- driver / dacu
    md -- master driver / dacu
    sd -- slave driver / dacu
    r -- rec
    u -- uic

details:
1>>. intercom between passenger and driver
    step1: d <---- p   get: request

                    step2: d <---- p   get: cancel

    step3: d ----> p   post: response

    step4: d ----> p   post: hang up

    
structure:
    
    state   1. idle         // not busy
            2. pwait        // pecu request 
            3. setup        // intercom setup
            4. frozen       // intercom by other setup

    rcabno
    rdev
    rdevno  
    scabno  *       // link info    // for intercom get // intercom cancel
    sdev    *
    sdevno  *
    channel *


2>>. intercom between two driver
    as master:
        step1: md ----> sd  post: request

                    step2: md ----> sd  post: cancel

        step3: md <---- sd  get: respone

                    step4: md <---- sd   get: hang up

        step5: md ----> sd  post: hang up

    as slave:
        step1: sd <---- md  get: request

                    step2: sd <---- md  get: cancel

        step3: sd ----> md  post: respone

                    step4: sd ----> md  post: hang up

        step5: sd <---- md  get: hang up


3>>. broadcast by driver
        step1: d ----> r    post: request
        step2: d <---- r    get: response

4>>. broadcast listenning
        step1: d <---- u    post: start
        step2: d <---- u    post: stop


