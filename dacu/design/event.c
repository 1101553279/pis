struct event{
    u8_t type;
                // pcom
                // dcom
                // cast
                // listen
    u8_t op;
                // post
                // get
};

pcom:  
        post
            response
            host hangup     // from button: current dacu hangup
        get
            request         // from network
            end hangup      // from network: end point hangup

dcom:   
        post                // host control :from button / request timeout
            host request    
            host response
            host hangup
        get                 // receive msg
            end response 
            end request
            end hangup

broadcast: 
        uic idle
            none
        uic busy
            post
                request
            get
                response

listen:
        get
            uic start
            uic stop
        get 
            occ start
            occ stop
