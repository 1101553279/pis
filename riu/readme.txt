

riu componets

            led

rec         core        vs1003  -> riu
    
            sd


    state:      
        idle        idle status
        play_st     station playing status
        play_emg    emergency playing status
        tiu_play    tiu playing
        occ_play    occ playing
        vs_reset    vs1003 reseting

cmd functions:

    1>. play station start      ok  0x31
    2>. play station stop       ok  0x32
    3>. play emgerency start
    4>. play emgerency stop
    5>. tiu play start
    6>. tiu play stop
    7>. occ play start          ok  0x36
    8>. occ play stop           ok  0x37
    9>. query riu status
    10>. set riu volume size

